#!/usr/bin/env python
import argparse
import os

parser = argparse.ArgumentParser(prog = 'sandbox', description = 'Submit your sandbox program source codes here')
parser.add_argument('samples')
parser.add_argument('-g', '--target-gui-apps', action = 'store_true')
arguments = parser.parse_args()
print(arguments);
print(arguments.samples.split( sep = ','));
comm1="${CC} ${CFLAGS_DEBUG} -L'lib/' -Wl,-rpath='lib/' -o $@ $< $(patsubst lib%,-l%,${LIBS})".format('gcc','',libs)
comm2="${CC} ${CFLAGS_DEBUG} $$(pkg-config --cflags gtk4) -L'lib/' -Wl,-rpath='lib/' -o $@ $< $(patsubst lib%,-l%,${LIBS}) $$(pkg-config --libs gtk4)"
