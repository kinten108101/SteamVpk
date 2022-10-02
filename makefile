CC:=gcc
CFLAGS_DEBUG:=-g -Wall -Wextra -std=c99

# RULE: List in alphabetical order
BIN_DEMO:=demo_sort
DLIBS:=libmaki libkoji
DLIBMAKI_SRC:=maki_math.c maki_stdio.c maki_string.c
DLIBMAKI_HEAD:=maki_math.h maki_stdio.h maki_string.h maki_type.h
DLIBKOJI_SRC:=koji_sort.c koji_string.c
DLIBKOJI_HEAD:=koji_sort.h koji_string.h
DLIBS_HEAD:=${DLIBMAKI_HEAD} ${DLIBKOJI_HEAD}

VPATH:=src src/lib_maki src/lib_koji

# vpath %.c src:src/pak:src/cli

# VPATH works better with files that have existed aka src/*/%.c
# This makes sense, since src files are user-organized while most
# intermediate binaries are one-level-tree flat.

# -- default setup

all : | shared_libs demo

# -- build recipes

static_libs : lib/libmaki.a
shared_libs : $(patsubst %,lib/%.so,${DLIBS})
demo : $(patsubst %,bin/%,${BIN_DEMO})

## Demo: reader
## build : bin/reader
##
## bin/reader : reader.c pak.c lib/libmaki.a
##	${CC} ${CFLAGS_DEBUG} -I'src/include/' -o $@ $^ -L'lib/' -lmaki

## The Maki static library
## lib/libmaki.a : ###
##	ar rv --output lib $@ $^

## The Maki shared library
lib/libmaki.so : $(patsubst %.c,tmp/%.o,${DLIBMAKI_SRC})
	${CC} -shared -o $@ $^

## The Koji shared library
lib/libkoji.so : $(patsubst %.c,tmp/%.o,${DLIBKOJI_SRC})
	${CC} -shared -o $@ $^

## Common
tmp/%.o : %.c
	${CC} ${CFLAGS_DEBUG} -I'src/' -c -o $@ $^

bin/% : %.c ${DLIBS_HEAD}
	${CC} ${CFLAGS_DEBUG} -I'src/' -L'lib/' -Wl,-rpath='lib/' \
	 -o $@ $< $(patsubst lib%,-l%,${DLIBS})

# --- misc recipes

clean:
	rm -rf tmp/*
	rm -rf lib/*