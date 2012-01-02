# This is simple makefile and it works for my machines.
# If it won't work on your machine - then it's broken.
# I really don't care that much. If you feel like 
# autotools would fit in I'll pay your return ticket
# from another planet.


CFLAGS = -Wall -I src/ \
	 -DGFX_API_CAIRO \
	 -DGFX_API_OPENGL \
	 -DNO_CPP_DEMANGLE\
	`pkg-config --cflags \
		glib-2.0 \
		fontconfig \
		x11 \
		pango \
		cairo`#

LIBS = -lGL -lrt \
	`pkg-config --libs \
		fontconfig \
		x11 \
		cairo \
		pangocairo \
		pangoxft \
		pangoft2 \
		pangox \
		glu \
		glib-2.0`#

SRC = $(filter-out src/util/tester.c, $(shell find src/ -name "*.c"))
TEST = $(shell find src/ -name "*.test") 
OBS = $(subst .c,.o, $(subst src/,bin/,$(SRC)))
LIB_OBS = $(filter-out bin/first_lob.o,$(OBS))
TEST_OBS = $(subst .test,_test.o, $(subst src/,bin/,$(TEST)))
HDRS = $(shell find src/lob -name "*.h")
DIRS = $(shell for i in $(OBS); do dirname $$i; done)


all: prepare lib example

run:
	LD_LIBRARY_PATH=. ./first_lob

example: bin/first_lob.o
	gcc -o first_lob bin/first_lob.o -L. -lvob-c 

lib: $(LIB_OBS)
	gcc -shared -rdynamic -o libvob-c.so $(LIB_OBS) $(LIBS)


$(OBS):
	gcc -c $(CFLAGS) -o $@ $(subst .o,.c, $(subst bin/,src/,$@))

prepare:
	mkdir -p $(dir $(OBS))

$(TEST_OBS):
	gcc -x c -c $(CFLAGS) -o $@ $(subst _test.o,.test, $(subst bin/,src/,$@))

test: $(TEST_OBS)
	gcc -shared -rdynamic -o libtest.so $(TEST_OBS) -L. -lvob-c $(LIBS)
	gcc -o tester src/util/tester.c `pkg-config --cflags --libs glib-2.0 gmodule-2.0 libelf`
	LD_LIBRARY_PATH=. ./tester libtest.so $(CTEST)

clean:
	rm -rf bin/ first_lob libvob-c.so py/build

bind:
	python py/bind.py $(HDRS)
	(cd py && python setup.py build)
	echo "Done"

PYTEST ?= py/examples/rect.py
runpy:
	mkdir -p py/build/lib.linux-i686-2.6/libvob/lob/
	touch py/build/lib.linux-i686-2.6/libvob/lob.py
	PYTHONPATH=py/build/lib.linux-i686-2.6/ python $(PYTEST)

