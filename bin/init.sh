#!/bin/sh
[ "$1" != "_INTERNAL" ] && { echo -e "Accessing internal file without permisison. Exiting..."; exit 1; };
SHELLNAME="$(sed -z 's|/||;s|\..\+$||' <<< ${0#${0%/*}})"
ulog () {
	content="$1"
	flag="$2"
	logIcon="(??)"
	case ${flag} in
		(E) logIcon="(!E)";;
		(I|*) logIcon="(::)";;
	esac;
	echo -e "$0 ${logIcon} ${content}" >&2;
}

#BASEDIR="$( dirname "$(realpath "$0")" | sed 's/\/bin//' )"
BASEDIR="$(cd "${0%/*}"/.. && echo ${PWD})"
RESRCDIR="${BASEDIR}/resrc"
#SPACES="$( sed -z 's/.* .*/ /g ; s/\n//g' <<< ${BASEDIR} )"
bSAFEPATH="$(cat "${RESRCDIR}/settings.json" | jq .b_safe_path)"
if [ "$( grep ' ' <<< ${BASEDIR} )" != "" ]; then 
	echo -en "Spaces found in path string...";
	case ${bSAFEPATH} in
		(1) echo " Exiting..."; exit 1;;
		(*)	echo -e "";; # Postprocessing if necessary
	esac;
fi;
bDEBUG="$(cat "${RESRCDIR}/settings.json" | jq .b_debug)"
GAMEDIR=""; case ${bDEBUG} in
	(1) GAMEDIR="/home/kinten/.local/share/Steam/steamapps/common/Left 4 Dead 2";;
	(0) GAMEDIR="$( jq .steam_game_path <<< "${BASEDIR}/settings.json" | sed -z 's/\"//g' )";;
	(*) ulog "This should not happen!" E ; exit 1;;
esac
ADDONS_DIR="${GAMEDIR}/left4dead2/addons"
SRCDIR="${BASEDIR}/garageas"
BINDIR="${BASEDIR}/bin"
TMPDIR="${BASEDIR}/tmp"
VPKINDIR="${BASEDIR}/vpk_ina"
VPKOUTDIR="${BASEDIR}/vpk_outa"
DOWNLOADSDIR="${BASEDIR}/dls"
ADDONSDOCK_DIR="${BASEDIR}/addons_dock"
WM_DIR="${TMPDIR}"