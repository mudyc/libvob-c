#!/bin/bash -v
set -x

# clean
rm first_lob
rm -r bin/*

#preconditions
mkdir -p \
    bin/gfx/cairo \
    bin/gfx/opengl \
    bin/vob \
    bin/vob/vobs \
    bin/lob \
    bin/util 

# ==========
# compile 
# ==========

CFLAGS="-Wall -I src `pkg-config --cflags glib-2.0`"

#example
gcc $CFLAGS \
    -o bin/first_lob.o -c src/first_lob.c

# vobs
gcc $CFLAGS \
    -o bin/vob/scene.o -c src/vob/scene.c
gcc $CFLAGS \
    -o bin/vob/coords.o -c src/vob/coords.c
gcc $CFLAGS \
    -o bin/vob/vobs/rect.o -c src/vob/vobs/rect.c

#lobs
gcc $CFLAGS \
    -o bin/lob/api.o -c src/lob/api.c
gcc $CFLAGS \
    -o bin/lob/main.o -c src/lob/main.c

#utils
gcc $CFLAGS \
    -o bin/util/regions.o -c src/util/regions.c
gcc $CFLAGS \
    -o bin/util/strings.o -c src/util/strings.c
gcc $CFLAGS \
    -o bin/util/strings.test -c src/util/strings.test
gcc $CFLAGS \
    -o bin/util/array.o -c src/util/array.c
gcc $CFLAGS \
    -o bin/util/hashtable.o -c src/util/hashtable.c
gcc $CFLAGS \
    -o bin/util/test.o -c src/util/test.c
gcc $CFLAGS \
    -o bin/util/tester.o -c src/util/tester.c
gcc $CFLAGS -DNO_CPP_DEMANGLE\
    -o bin/util/sigsegv.o -c src/util/sigsegv.c

# gfx
gcc $CFLAGS -DGFX_API_CAIRO -DGFX_API_OPENGL\
    -o bin/gfx/gfx_api.o -c src/gfx/gfx_api.c
gcc $CFLAGS \
    -o bin/gfx/animation.o -c src/gfx/animation.c
gcc $CFLAGS \
    -o bin/gfx/render.o -c src/gfx/render.c

# gfx - cairo
gcc $CFLAGS -DGFX_API_CAIRO \
    -o bin/gfx/cairo/cairo.o -c src/gfx/cairo/cairo.c \
   `pkg-config --cflags cairo`

# gfx -opengl
gcc $CFLAGS -DGFX_API_OPENGL \
    -o bin/gfx/opengl/opengl.o -c src/gfx/opengl/opengl.c
gcc $CFLAGS -DGFX_API_OPENGL \
    -o bin/gfx/opengl/render.o -c src/gfx/opengl/render.c
gcc $CFLAGS \
    -o bin/gfx/opengl/coords.o -c src/gfx/opengl/coords.c
gcc $CFLAGS \
    -o bin/gfx/opengl/vobs.o -c src/gfx/opengl/vobs.c




# ===========
# link
# ===========
gcc -rdynamic -shared -o libvob-c.so \
    bin/vob/scene.o \
    bin/vob/coords.o \
    bin/vob/vobs/rect.o \
    bin/lob/api.o \
    bin/lob/main.o \
    bin/util/regions.o \
    bin/util/strings.o \
    bin/util/array.o \
    bin/util/hashtable.o \
    bin/util/sigsegv.o \
    bin/gfx/gfx_api.o \
    bin/gfx/animation.o \
    bin/gfx/render.o \
    bin/gfx/cairo/cairo.o \
    bin/gfx/opengl/opengl.o \
    bin/gfx/opengl/render.o \
    bin/gfx/opengl/coords.o \
    bin/gfx/opengl/vobs.o \
   `pkg-config --libs x11 cairo glib-2.0` -lGL -lrt

gcc -rdynamic -shared -o libvob-c-test.so \
    bin/util/test.o \
    bin/util/strings.test

gcc -o tester bin/util/tester.o \
    bin/util/array.o \
    `pkg-config --libs libelf` -L. -lvob-c

gcc -o first_lob \
    bin/first_lob.o \
    -L. -lvob-c 

