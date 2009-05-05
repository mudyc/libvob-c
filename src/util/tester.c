// (c): Matti J. Katila
#include <err.h>
#include <fcntl.h>
#include <libelf.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>
#include <string.h>

#include <signal.h>

#include "util/array.h"
#include "util/strings.h"


void run_test(char *name) {
	// XXX TODO: should stderr be handled?
	int stdout_pipe[2];
	int fork_result;
	pid_t child;

	if (pipe(stdout_pipe) == 0) {
		//fcntl(stdout_pipe[0], O_NONBLOCK);
		//fcntl(stdout_pipe[1], O_NONBLOCK);
		switch (child = fork()) {
		case -1: 
			err(1, "fork failed");
		case 0: { // child
			close(1); // stdout
			dup(stdout_pipe[1]); // dup takes the stdout (=lowest free) now.
			close(stdout_pipe[0]);
			close(stdout_pipe[1]);

			// setup
			// test case
			// teardown
			
			printf("TEST_DONE\n");

			exit(0);
		}
		// parent
		default:
			close(stdout_pipe[1]);

			fd_set fds;
			int val;

			while (1) {
				FD_ZERO(&fds);
				FD_SET(stdout_pipe[0], &fds);
				val = select(stdout_pipe[0]+1, &fds, NULL, NULL, NULL);
				if (val == -1)
					break;
				if (FD_ISSET(stdout_pipe[0], &fds)) {
					char buff[256];
					memset(buff, 0, sizeof(buff));
					val = read(stdout_pipe[0], buff, sizeof(buff));
					if (val == 0)
						break;
					printf("FOO: %d %s\n", val, buff);
				}
			}
			

			printf("Done..\n");

			close(stdout_pipe[0]);

			// what if children makes own children?
			kill(child, SIGTERM);
			kill(child, SIGKILL);
		}
	}

}


/* Given Elf header, Elf_Scn, and Elf32_Shdr 
 * print out the symbol table 
 */
void print_symbols(Elf *elf, Elf_Scn *scn, Elf32_Shdr *shdr, UtilArray *arr)
{
	Elf_Data *data;
	char *name;
	data = 0;

	if ((data = elf_getdata(scn, data)) == 0 || data->d_size == 0){
		/* error or no data */
		fprintf(stderr,"Section had no data!n");
		exit(-1);
	}
	/*now print the symbols*/
	Elf32_Sym *esym = (Elf32_Sym*) data->d_buf;
	Elf32_Sym *lastsym = (Elf32_Sym*) ((char*) data->d_buf + data->d_size);
	/* now loop through the symbol table and print it*/
	for (; esym < lastsym; esym++){
		if ((esym->st_value == 0) ||
		    (ELF32_ST_BIND(esym->st_info)== STB_WEAK) ||
		    (ELF32_ST_BIND(esym->st_info)== STB_NUM) ||
		    (ELF32_ST_TYPE(esym->st_info)!= STT_FUNC)) 
			continue;
		name = elf_strptr(elf,shdr->sh_link , (size_t)esym->st_name);
		if(!name){
			fprintf(stderr,"%sn",elf_errmsg(elf_errno()));
			exit(-1);
		}
		//printf("%d: %s\n",number++, name);
		util_arr_add(arr, util_str_create(name));
	}
}



int main(int argc, char **argv)
{
	int fd, i;

	Elf *e;
        Elf_Scn *scn;
        Elf32_Shdr *shdr;
        size_t shstrndx;

	UtilArray *arr = util_arr_create();

	if (argc == 2) {
		if (elf_version(EV_CURRENT) == EV_NONE) {
			printf("argh\n");
			abort();
		}

		if ((fd = open(argv[1], O_RDONLY, 0)) < 0) {
			printf("argh2\n");
			exit(1);
		}

		if ((e = elf_begin(fd, ELF_C_READ, NULL)) == NULL)
			errx(1, "elf_begin() failed: %s.",
			     elf_errmsg(-1));


		if (elf_kind(e) != ELF_K_ELF)
			errx(EX_DATAERR, "%s is not an ELF object.", argv[1]);

		if (elf_getshstrndx(e, &shstrndx) == 0)
			errx(EX_SOFTWARE, "getshstrndx() failed: %s.",
			     elf_errmsg(-1));

		scn = NULL;
		while ((scn = elf_nextscn(e, scn)) != NULL) { 
			if ((shdr = elf32_getshdr(scn)) == 0)
				errx(EX_SOFTWARE, "getshdr() failed: %s.",
				     elf_errmsg(-1));

			if (shdr->sh_type == SHT_SYMTAB)
				print_symbols(e, scn, shdr, arr);
		}


	}

	for (i=0; i<arr->size; i++) {
		UtilString *str = util_arr_get_str(arr, i);
		if (util_str_startswith_(str, "test")) {
			printf("%d: %s\n", i, str->buff);
			run_test(str->buff);
		}
		else
			printf("non test: %d: %s\n", i, str->buff);
	}
	return 0;
}
