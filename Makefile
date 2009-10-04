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
		x11 \
		cairo`#

LIBS = -lGL -lrt \
	`pkg-config --libs \
		x11 \
		cairo \
		glib-2.0`#

SRC = $(shell find src/ -name "*.c") 
OBS = $(subst .c,.o, $(subst src/,bin/,$(SRC)))
LIB_OBS = $(filter-out bin/first_lob.o,$(OBS))
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

clean:
	rm -rf bin/ first_lob libvob-c.so


