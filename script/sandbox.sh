#!/usr/bin/env sh
. "$(cd "${0%/*}/.." && echo ${PWD})/script/init" _INTERNAL

print_help ()
{
	cat <<END
usage: sandbox <command>
commands:
    -h, --help              Help menu
    -g, --gui
    -c, --generic
END
}

if [ $# -eq 0 ]; then echo -e "${HELPMENU}"; exit 0; fi;
ARGS="$( getopt -n sandbox -o 'hg:c:' --long 'help,gui:,generic:' -- "$@" )"
eval set -- "${ARGS}"
while : ; do
	case "$1" in
		(-h|--help)				echo -e "${HELPMENU}" && shift;;
		(-g|--gui)				make SAMPLE_GUI="$2" && shift 2;;
		(-c|--generic)			make SAMPLE_GENERIC="$2" && shift 2;;
		(--) shift; break;;
		(*) echo "Unimplemented option"; exit 12;; # Impossible case 
	esac
done;