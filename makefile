CC:=gcc
CFLAGS_DEBUG:=-g -Wall -Wextra -std=c99

# RULE: List in alphabetical order
BIN_DEMO:=demo_workshop
DLIBS:=libmaki libkoji libmates
DLIBMAKI_SRC:=mkmath.c mkio.c mkstring.c
DLIBMAKI_HEAD:=mkmath.h mkio.h mkstring.h mktype.h
DLIBKOJI_SRC:=kjsort.c kjstring.c
DLIBKOJI_HEAD:=kjsort.h kjstring.h
DLIBMATES_SRC:=workshop_manager.c
DLIBMATES_HEAD:=workshop_manager.h
DLIBS_HEAD:=${DLIBMAKI_HEAD} ${DLIBKOJI_HEAD} ${DLIBMATES_HEAD}

VPATH:=src src/maki src/koji src/pak

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

## The Mates shared libary, basically the rest of the app's CLI functionality
lib/libmates.so : $(patsubst %.c,tmp/%.o,${DLIBMATES_SRC})
	${CC} -shared -o $@ $^

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