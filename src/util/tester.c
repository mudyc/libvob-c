/** tester.c -- A unit test runner.
 *
 * Loads unit tests with ELF library and runs these tests. Test
 * failures are reported afterwards.
 *
 * Implementation note: operating system will clean up the memory.
 *
 * Compile:
 *   gcc -o tester tester.c `pkg-config --cflags --libs glib-2.0
gmodule-2.0 libelf`
 *
 * Usage:
 *   LD_LIBRARY_PATH=. ./tester libtest.so
 *
 */

/*
 * Written by Matti Katila 2009
 */


#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <glib.h>
#include <gmodule.h>
#include <unistd.h>
#include <err.h>
#include <libelf.h>
#include <string.h>

struct test_result {
       char *name;
       gboolean is_run;
       gboolean is_failed;
       GArray *stdout;
};


void report_failures(GPtrArray *results)
{
       int i;
       for (i=0; i<results->len; i++) {
               struct test_result *res =
                       g_ptr_array_index (results, i);
               if (res->is_run && res->is_failed) {
                       printf("--[FAILURE]--------------------------------\n");
                       printf("* %s\n", res->name);
                       write(1 /*stdout*/, res->stdout->data,
                             (size_t)res->stdout->len);
                       printf("\n");
               }
       }
}

static void run_one(GModule *handle, struct test_result *res)
{
       // XXX TODO: should stderr be handled?
       int stdout_pipe[2];
       int fork_result;
       pid_t child;

       void (*func)(void);

       char *LAST_WORDS = "TEST_DONE";

       if (pipe(stdout_pipe) == 0) {
               switch (child = fork()) {
               case -1:
                       err(1, "Error: fork failed");
               case 0: { // child

                       close(1); // stdout
                       // dup takes the stdout (=lowest free) now.
                       dup(stdout_pipe[1]);
                       close(stdout_pipe[0]);
                       close(stdout_pipe[1]);

                       // setup
                       // test case
                       if (g_module_symbol(handle, res->name, (gpointer) &func))
                               func();

                       // teardown

                       printf("%s", LAST_WORDS);

                       exit(0);
               }
               default: // parent
                       close(stdout_pipe[1]);

                       fd_set fds;
                       ssize_t val;

                       while (1) {
                               FD_ZERO(&fds);
                               FD_SET(stdout_pipe[0], &fds);
                               val = select(stdout_pipe[0]+1, &fds,
                                            NULL, NULL, NULL);
                               if (val == -1)
                                       break;
                               if (FD_ISSET(stdout_pipe[0], &fds)) {
                                       char buff[256];
                                       memset(buff, 0, sizeof(buff));
                                       val = read(stdout_pipe[0], buff,
                                                  sizeof(buff));
                                       if (val == 0)
                                               break;
                                       g_array_append_vals(res->stdout,
                                                           buff, val);
                               }
                       }
                       close(stdout_pipe[0]);

                       // what if children makes own children?
                       kill(child, SIGTERM);
                       kill(child, SIGKILL);

                       res->is_run = TRUE;
                       res->is_failed = TRUE;
                       if (res->stdout->len >= strlen(LAST_WORDS))
                               if (strcmp((char*)&res->stdout->data[res->stdout->len -
strlen(LAST_WORDS)], LAST_WORDS) ==  0)
                                       res->is_failed = FALSE;
	       }
       }

}

/**
 * @return array of <struct test_result>s.
 */
GPtrArray *run_tests(GModule *handle, GPtrArray *tests, gchar *test_name)
{
       int i;
       GPtrArray *ret = g_ptr_array_new();

       for (i=0; i<tests->len; i++) {
               struct test_result *res =
                       malloc(sizeof(struct test_result));
               memset(res, 0, sizeof(struct test_result));
               res->stdout = g_array_new(TRUE, FALSE, sizeof(char));

               res->name = g_ptr_array_index(tests, i);
	       if (test_name != NULL) {
		       if (strcmp(test_name,res->name) != 0) 
			       continue;

		       printf("Run without fork..\n");
		       void (*func)(void);
		       if (g_module_symbol(handle, res->name, (gpointer) &func))
                               func();
	       }
               printf("Run: %s: ", res->name);

               run_one(handle, res);

               printf("%s\n", res->is_failed? "FAILED": "OK");
               g_ptr_array_add(ret, res);
       }

       return ret;
}


/**
 * @return true in success.
 */
gboolean fetch_tests_from_lib(gchar *lib, GPtrArray *tests, GModule **handle)
{
       int fd;
       Elf *e;
       Elf_Scn *scn;
       Elf32_Shdr *shdr;
       size_t shstrndx;


       if (elf_version(EV_CURRENT) == EV_NONE )
               errx(1, "Error: elf library is out of date.\n");

       if ((fd = open(lib, O_RDONLY, 0)) < 0)
               err(1, "Error in opening library file. ");

       if ((e = elf_begin(fd, ELF_C_READ, NULL)) == NULL)
               errx(1, "Error in elf_begin(): %s\n", elf_errmsg(-1));

       if (elf_kind(e) != ELF_K_ELF)
               errx(1, "Loaded file is not ELF library.\n");

       if (elf_getshstrndx(e, &shstrndx) == 0)
               errx(1, "Error in getshstrndx(): %s.", elf_errmsg(-1));

       if (!g_module_supported())
               errx(1, "GModule not supported by the underlying system.\n");

       *handle = g_module_open(lib, 0);
       if (*handle == NULL)
               errx(1, "Could not open the dynamic library: %s. ",
                    g_module_error());
                    //"Please check the LD_LIBRARY_PATH variable.\n");

       scn = NULL;
       while ((scn = elf_nextscn(e, scn)) != NULL) {
               if ((shdr = elf32_getshdr(scn)) == 0)
                       errx(1, "getshdr() failed: %s.", elf_errmsg(-1));

               if (shdr->sh_type != SHT_SYMTAB)
                       continue;

               Elf_Data *data = NULL;
               if ((data = elf_getdata(scn, data)) == NULL
                   || data->d_size == 0)
                       errx(1, "ELF section had no data!\n");


               Elf32_Sym *esym = (Elf32_Sym*) data->d_buf;
               Elf32_Sym *lastsym = (Elf32_Sym*)
                       ((char*) data->d_buf + data->d_size);
               /* now loop through the symbol table and print it*/
               for (; esym < lastsym; esym++){
                       if ((esym->st_value == 0) ||
                           (ELF32_ST_BIND(esym->st_info)== STB_WEAK) ||
                           (ELF32_ST_BIND(esym->st_info)== STB_NUM) ||
                           (ELF32_ST_TYPE(esym->st_info)!= STT_FUNC))
                               continue;
                       char *name = elf_strptr(e, shdr->sh_link,
                                         (size_t)esym->st_name);
                       if (g_regex_match_simple("^test_", name, 0, 0))
                           g_ptr_array_add(tests, strdup(name));
               }
       }
       return TRUE;
}

void usage()
{
       printf("A simple unit test runner.\n"
              "\n"
              "Usage: ./tester LIBNAME\n"
              "\n"
              "Where LIBNAME is a shared library file containing all\n"
              "test functions. The library should be linked with \n"
              "-rdynamic or symbol names are not exposed.\n"
              );
       exit(EXIT_FAILURE);
}


/**
 * @return true in success.
 */
gboolean parse_args(int argc, char **argv,
		    char **file, char **test)
{
       struct stat info;


       if (argc < 2) {
               printf("Error: No LIBNAME given\n\n");
               return FALSE;
       }

       if (stat(argv[1], &info))
               errx(1, "Error: Could not find file %s", argv[1]);
       *file = argv[1];

       if (argc == 3)
	       *test = argv[2];
       return TRUE;
}

int main(int argc, char **argv)
{
       gchar *test_lib_file = NULL;
       gchar *test_func_name = NULL;
       GPtrArray *tests = g_ptr_array_new();
       GModule *handle = NULL;

       if (!parse_args(argc, argv, &test_lib_file, &test_func_name))
               usage();

       if (!fetch_tests_from_lib(test_lib_file, tests, &handle))
               usage();

       GPtrArray *results = run_tests(handle, tests, test_func_name);

       report_failures(results);


       return EXIT_SUCCESS;
}
