#!/bin/sh
. "$(cd "${0%/*}/.." && echo ${PWD})/bin/init" _INTERNAL

start_l4d2 ()
{
	steam steam://rungameid/550
}

if [ $# -eq 0 ]; then echo -e "No command provided"; exit 0; fi;

while getopts ':s' option; do
	case ${option} in
		(s) start_l4d2;;
		(*) ulog 'Unimplemented option in ${SHELLNAME}' 'E';;
	esac
done