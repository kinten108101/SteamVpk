SVG_FILES:=$(wildcard src/*.svg)
DEPS:=$(foreach target,${SVG_FILES},$(patsubst src/%.svg,%.png,${target}))

all : ${DEPS}

%.png : src/%.svg
	inkscape $< -o $@