#!/bin/sh
. "$(cd ${0%/*} && echo ${PWD})/init" _INTERNAL

case $1 in
	(download) shift; exec "${BINDIR}/workshop_manager" $@;;
	(addons) shift; exec "${BINDIR}/addons_manager" $@;;
	(game) shift; exec "${BINDIR}/game_manager" $@;;
	(*) ulog "Unimplemented option in matesvpk" 'E';;
esac