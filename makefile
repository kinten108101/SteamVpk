.PHONY:=libs curl smkl mates cjson execs adwaita

COMPILER ?= gcc

LIBS_TBA:=curl smkl mates cjson adwaita
LIBSMKL_DIR:=src/smkl
LIBSMKL_SRC:=$(notdir $(wildcard ${LIBSMKL_DIR}/*.c))
LIBMAIN_DIR:=src/download src/archival src/utils
LIBMAIN_SRC:=$(foreach DIR,${LIBMAIN_DIR},$(notdir $(wildcard ${DIR}/*.c)))

DEBUG_CFLAGS:=-g -Wall -Wextra -std=c99
LIBMATES_CFLAGS:=

VPATH:=${LIBSMKL_DIR} ${LIBMAIN_DIR} src/bootstrap

libs : dircheck make_vars

dircheck :
	mkdir -p dump/libmates
	mkdir -p dump/libsmkl

make_vars : ${LIBS_TBA} misc_config
	echo -e "#DONOTEDIT\nCC=${COMPILER}\nEXEC_CFLAGS=${EXEC_CFLAGS}\nEXEC_LIBS=${EXEC_LIBS}" > $@

misc_config :
	$(eval EXEC_CFLAGS+=${DEBUG_CFLAGS})

mates : lib/libmates.so
	$(eval EXEC_LIBS+=-lmates)

lib/libmates.so : $(patsubst %.c,dump/libmates/%.o,${LIBMAIN_SRC})
	${COMPILER} -shared -o $@ $^

dump/libmates/%.o : %.c
	$(eval LIBMATES_CFLAGS+=-I'vendors/cjson') \
		$(eval LIBMATES_CFLAGS+=$(shell pkg-config --libs libadwaita-1)) \
		${COMPILER} ${DEBUG_CFLAGS} ${LIBMATES_CFLAGS} -fPIC -c -o $@ $^

smkl : lib/libsmkl.so
	$(eval EXEC_LIBS+=-lsmkl)

lib/libsmkl.so : $(patsubst %.c,dump/libsmkl/%.o,${LIBSMKL_SRC})
	${COMPILER} -shared -o $@ $^

dump/libsmkl/%.o : %.c
	${COMPILER} ${DEBUG_CFLAGS} -fPIC -c -o $@ $^

cjson : lib/libcjson.so
	$(eval EXEC_LIBS+=-lcjson)

lib/libcjson.so : vendors/cjson/cJSON.c
	${COMPILER} ${DEBUG_CFLAGS} -fPIC -I'vendors/cjson/' -c -o dump/cJSON.o $^ && \
		${COMPILER} -shared -o $@ dump/cJSON.o

curl :
	$(eval EXEC_LIBS+=$(shell curl-config --libs))

adwaita :
	$(eval EXEC_LIBS+=$(shell pkg-config --libs libadwaita-1))

execs : execs_load_config $(patsubst %.c,bin/%,${EXEC_FILES})

execs_load_config :
	$(eval EXEC_CFLAGS+=$(shell cat make_vars | sed -n '3p' | sed 's/EXEC_CFLAGS=//'))
	$(eval EXEC_LIBS+=$(shell cat make_vars | sed -n '4p' | sed 's/EXEC_LIBS=//'))
ifeq ($(EXCEPTION_GUI_DEV),y)
	$(eval EXEC_CFLAGS+=$(shell pkg-config --cflags libadwaita-1))
endif

bin/% : %.c
	${COMPILER} ${EXEC_CFLAGS} -L'lib' -Wl,-rpath='lib' \
		-o $@ $< ${EXEC_LIBS}

clean:
	rm -rf dump/*
	rm -rf lib/*
	rm -rf bin/*

# -rpath='lib' to add dir to search path OPTIONS, -L to add dir to search path ._. ??