#!/bin/sh
#. "$(dirname "$(realpath "$0")")/init" _INTERNAL ;
. "$(cd "${0%/*}" && echo ${PWD})/init" _INTERNAL
SHELLNAME=$(sed -z 's/\(.*\)\/// ; s/\..\+//' <<< $0) ;

VPKFILES=$( find "${ADDONSDOCK_DIR}" -type f | sed -n '/\.vpk$/p' | sed -z 's/\n/:/g') # -z so that each line ends with NULL instead of \n

img_make () {
	
	# If the vpk files does have an image
	IFS=':' for vpkfile in ${VPKFILES}; do
		vpkimg=$(echo ${vpkfile} | sed 's/\(.*\)\//\1\/thumbnails\//' | sed -n 's/\.vpk$/\.jpg/p')
		[ ! -f ${vpkimg} ] && vpkimg="${ADDONSDOCK_DIR}/thumbnails/default.png";
		gio set -t string "${vpkfile}" metadata::custom-icon "file://${vpkimg}";
		# An alternative conditional construct. Wondering if the parentheses
		#put that section into a subshell. Does it matter tho?
	done;
}
img_reset () {
	# For now I only know of setting icon to NULL. Better would be to 
	#remove the attribute, since it doesn't seem that files have this
	#attribute by default.
	IFS=':' for vpkfile in ${VPKFILES}; do
		gio set -t string "${vpkfile}" metadata::custom-icon "";
	done;
}

link_make () {
	NUM_CONFLICT=0
	IFS=':' for vpkfile in ${VPKFILES}; do
		vpkname=$(echo ${vpkfile} | sed -e 's/\(.*\)\///' -e 's/.vpk$//')
		vpklink="${ADDONS_DIR}/$(echo ${vpkfile} | sed 's/\(.*\)\///')"
		if [ ! -f "${vpklink}" ]; then
			ln -s "${vpkfile}" "${vpklink}";
		else echo -e "Conflict occured when linking ${vpkname}.vpk!";
			NUM_CONFLICT=$(( ${NUM_CONFLICT} + 1 ))
		fi;
	done;
	if [ ${NUM_CONFLICT} -gt 0 ]; then return 34; else return 0; fi;
}

link_delete () {
	vpklinks="$(find -P "${ADDONS_DIR}" -maxdepth 1)"
	vpklinks=$(sed -n -e '/.vpk$/p' <<< ${vpklinks})
	vpklinks=$(sed 's/.vpk^/.vpk\n/' <<< ${vpklinks})
	IFS=$'\n' for vpklink in ${vpklinks}; do
		rm "${vpklink}"
	done;
	# The whitespace in Left 4 Dead 2 directory is really messing with shell.
	#
	# SOLUTION 1: Temporarily set whitespaces to special charsets ("---"), then
	# in each loop, reset these charsets to whitespaces.
	#
	# SOLUTION 2: For this function, set IFS (Input Field Separator)
	# to newline, and reformat vpklinks accordingly. Once done, reset IFS.
	# Also notice how IFS is in single quotes. Newline is one char.
}

cleantemp() {
	rm -r "${TMPDIR}/"*
	# In the case of rm, I have to put * outside quotes. Dot didn't even work
}

diff_plus() {
	DIFF_INPUT="$1"
	
	DIFF_DIRS=""; N=0;
	IFS=';' for diff_input_entry in ${DIFF_INPUT}; do
		diff_dir="$( cut -f 2 -d ':' <<< "${diff_input_entry}" )"
		DIFF_DIRS="${DIFF_DIRS}:${diff_dir}"
		N=$(( ${N} + 1 ));
	done;
	DIFF_DIRS="$(echo "${DIFF_DIRS}" >&1 | sed 's/^://')"

	iIndexes=$(seq 1 $(( $N - 1)) )
	jIndexes=$(seq 1 $(( $N )) )
	for i in ${iIndexes}; do
		jIndexes=$( echo -e "${jIndexes}" | sed "/${i}/d" );
		for j in ${jIndexes}; do
			dirCurrent="$( cut -f ${i} -d ':' <<< ${DIFF_DIRS} )"
			dirTarget="$( cut -f ${j} -d ':' <<< ${DIFF_DIRS} )"
			echo "$(diff -sqr "${dirCurrent}/" "${dirTarget}")/" >&1;
		done;
	done;

}

vpkfiles_merger() {

	VPKNAMES="$1"
	# "$1" is a single item, but $1 isn't, wordsplitting wont be performed. I just worry about spaces.
	bSkipExtraction="$2"

	VPKDICT=":"
	case ${bSkipExtraction} in
		(0) 
		IFS=',' for vpkname in ${VPKNAMES}; do
			xtr_output="$(vpk_extract "${vpkname}" 1)"
			xtr_exit="$(cut -f 1 -d '|' <<< ${xtr_output})";
			xtr_vpkdir="$(cut -f 2 -d '|' <<< ${xtr_output})";
			VPKDICT="${VPKDICT};${vpkname}:${xtr_vpkdir}"
			if [[ "${xtr_exit}" == "19" ]]; then cleantemp; exit 1; fi;
		done;
		;;
		(1)
		IFS=',' for vpkname in ${VPKNAMES}; do
			vpkdir="${SRCDIR}/${vpkname}";
			VPKDICT="${VPKDICT};${vpkname}:${vpkdir}"
		done;
		;;
		(*)
		ulog "Unknown extraction option in ${0}!" E;
		;;
	esac;
	#VPKDICT="$(echo "${VPKDICT}" | sed 's/^:;//')"
	VPKDICT=${VPKDICT:2}

	output=`diff_plus "${VPKDICT}" | sed -e '/Only in/d' -e '/addoninfo/d' -e '/addonimage/d' `
	if [[ ${output} == "" ]]; then
		echo -en "(::) No conflicts found! VPK files are available for merging, proceed? [Y/n] " &&
		read n;
		if [[ $n == "Y" ]]; then
			c_vpkname=""
			while [[ ${c_vpkname} == "" ]]; do echo -en "(::) Enter name of new VPK: "; read c_vpkname; done;
			c_uuid="$(uuidgen)"
			c_vpkdir="${TMPDIR}/${c_vpkname}-${c_uuid}"; # Working in tempMode
			[ ! -d "${c_vpkdir}" ] && mkdir "${c_vpkdir}";

			c_addoninfo="${c_vpkdir}/addoninfo.txt"
			c_addontitle=""
			#[ ! -f "${vpkdirC}/addoninfo.txt" ] && touch "${vpkdirC}/addoninfo.txt";
			IFS=';' for vpkentry in ${VPKDICT}; do
				xtr_vpkname="$(cut -d':' -f1 <<< ${vpkentry})"; if [[ "${xtr_vpkname}" == "" ]]; then continue; fi;
				xtr_vpkdir="$(cut -d':' -f2 <<< ${vpkentry})"; if [[ "${xtr_vpkdir}" == "" ]]; then continue; fi;
				cp -r "${xtr_vpkdir}/." "${c_vpkdir}/";
				xtr_addoninfo="${xtr_vpkdir}/addoninfo.txt"
				xtr_addontitle=""; if [ -f "${xtr_addoninfo}" ]; then xtr_addontitle=$(cat "${xtr_addoninfo}" | sed -n '/addon[Tt]itle/p' | sed 's/.*\"\(.*\)\".*/\1/' >&1 ); fi;
				if [[ "${xtr_addontitle}" == "" ]]; then while [[ ${xtr_addontitle} == "" ]]; do echo -en "(::) Give an add-on title to ${xtr_vpkname}.vpk: "; read xtr_addontitle; done; fi;
				ulog "Add-on title of ${xtr_vpkname} is: ${xtr_addontitle}"
				c_addontitle="${c_addontitle} + ${xtr_addontitle}"
			done;
			c_addontitle="$( echo "${c_addontitle}" >&1 | sed 's/^ + //')"
			cat "${BASEDIR}/addoninfo_template.txt" | sed "s/addon[Tt]itle\(.*\)\"\"/addontitle\1\"${c_addontitle}\"/" > "${c_addoninfo}"
			echo -e "(::) Check out the new addoninfo.txt!"; read _;
			
			c_output="$(vpk_make "${c_vpkname}" 1 "${c_uuid}")";
			c_exit="$(cut -f 1 -d '|' <<< "${c_output}")"
			#vpkdirC="$(cut -f2 -d'|' <<< "${outputC}")"
			if [[ "${c_exit}" == "19" ]]; then cleantemp; exit 1;
			else cleantemp; exit 0; fi;
		else cleantemp; exit 0; fi;
	else ulog "Conflicts found:\n${output}" E ; ulog "Enter to exit..." ; read fwait ; cleantemp; exit 1; fi; # remember to cleantemp temp!
	# For some reason, when capture diff output, the output loses all newlines (UPDATE: that isn't happening now for some reason)
	# Talk about conditional/test statement: 
	# [] for unary operations (-eq,-ne,-gt,-lt),
	# [[]] for binary operations (==,<,>,...)
}

vpk_extract() {
	# OPTARG: vpkname (file in vpk_in)
	vpkname="$1"
	bTempMode="$2"

	vpkfile="${VPKINDIR}/${vpkname}.vpk"
	vpkdir="unknowndir"
	if [ ! -f "${vpkfile}" ]; then
		echo -e "(!E) File ${vpkfile} doesn't exist!" >&2;
		echo -e "19|${vpkdir}" >&1 && return 1;
	fi;
	case ${bTempMode} in 
		(0) vpkdir="${SRCDIR}/${vpkname}";;
		(1) if [ ! -d "${TMPDIR}" ]; then mkdir "${TMPDIR}"; fi; vpkdir="${TMPDIR}/${vpkname}-$(uuidgen)";;
		(*) echo -e "(!E) Unknown extraction mode." >&2 && echo -e "19|${vpkdir}" >&1 && return 1;;
	esac;

	# The weirdest bit is how the rest of the function continues even 
	# before this conditional check finishes.
	#
	# Also, it seems that if there's both exit/return and echo >$1 then the latter takes precedence.

	echo -e "(::) Extracting ${vpkname}.vpk:" >&2;
	vpk "${vpkfile}" -x "${vpkdir}/" >&2;
	# Lesson learned: vpk still output exit code 0 once error. Just realized...
	# So what, check if stderror is empty?
	# This means that this line of code is useless now: if [ $? -eq 0 ]; then echo -e "Error encountered, exiting." >&2; echo -n "19" >&1; exit; fi;
	echo -e "(::) Extracted archive into directory: ${vpkdir}" >&2;
	echo -e "0|${vpkdir}" >&1; return 0;
	# This is interesting. Read up on file descriptor.
	#
	# Bash functions can only return int (status num). To mimic modern
	# functions in returning string, echo to stdout because stdout is
	# what a var captures in the case of var=$(func). To log anything
	# else, use stderr. So for error messages I redirected them with
	# >&2, while for return string I use >$1.
	#
	# Been wondering why the return string was not just vpkdir.
	# Turns out the vpk command also output, but I forgot to channel it
	# to stderr.
	#
	# Learned from a tutorial that to suppress outputs, send them to a
	# blackhole file called `/dev/null`. 
}

vpk_make() {
	vpkname="$1"
	bTempMode="$2"
	uuid="$3"
	vpkfile="${VPKOUTDIR}/${vpkname}.vpk"
	vpkdir="unknowndir"
	#if [ -f "${vpkfile}" ]; then
	#	echo -e "(!E) Output file named ${vpkname}.vpk already exists!" >&2;
	#	echo -e "19|${vpkdir}" >&1;
	#	return 1;
	#fi;	
	case ${bTempMode} in
		(0) vpkdir="${SRCDIR}/${vpkname}";;
		(1) if [ ! -d "${TMPDIR}" ]; then mkdir "${TMPDIR}"; fi; vpkdir="${TMPDIR}/${vpkname}-${uuid}/";;
		(*) echo -e "(!E) Unknown making mode." >&2; echo -e "19|${vpkdir}" >&1; return 1;;
	esac;

	vpk "${vpkfile}" -cv 1 -c "${vpkdir}"
	echo -e "0|${vpkdir}"; return 0;
}

mapp_getopts () {

}

usage_report() {
	# addons_subdirs=$(find ${ADDONSDOCK_DIR} -type d | sed '/thumbnails/d' | sed -n '/addons_dock.\+/p')
	# It's infuriating when I don't know when to use -n. d and s commands
	# don't repeat read buffer, but p does.
	#du -sh ${addons_subdirs}
	#du -Ssh "${ADDONSDOCK_DIR}"
	# Count
	#NUM_ADDONS=$(( $( ( tr -cd ':' <<< ${VPKFILES} ) | wc -c ) + 1 )) # method one, fast but i don't understand well. It's counting byte, sure, but...
	NUM_ADDONS=$(( $( (grep -o ':' <<< ${VPKFILES} ) | wc -l ) + 1 )) # method two, easy to understand, but not as fast
	echo -e "Number of active add-ons: ${NUM_ADDONS}"
}

HELPMENU="Usage: $0 [MODE] (VALUE)
	[MODE]=
	-h	Help menu
	-i	Report disk usage of VPKs at dock
	-m	Set thumbnails for VPKs at dock; images are in \`addons_dock/*/thumbnails/\`
	-r	Revert thubmnails to default file icons (WIP)
	-l	Establish links for VPKs from dock to game's add-ons folder
	-d	Remove all links and potential VPKs at game's add-ons folder
	-e	Extract a VPK file in \`vpk_in/\`. [VALUE]=name of VPK file.
"

trap "cleantemp" EXIT
# Run commands/callback on event (signal)

# Linux's getopt is not POSIX-compliant but supports long options 
# unlike getopt. May have to write an abstraction.
if [ $# -eq 0 ]; then echo -e "${HELPMENU}"; exit 0; fi;
ARGS="$( getopt -n addons_manager -o 'himrldf:z:' --long 'help,info,make-thumbnail,remove-thumbnail,make-link,remove-link,unpack:,merge:,pack:,merge-dirs:' -- "$@" )"
eval set -- "${ARGS}"
while : ; do
	case "$1" in
		(-h|--help)				echo -e "${HELPMENU}" && shift;;
		(-i|--stat) 			usage_report && shift;;
		(-m|--make-thumbnail) 	img_make && echo -e "Thumbnails set." && shift;;
		(-r|--remove-thumbnail) img_reset && echo -e "Thumbnails reset." && shift;;
		(-l|--make-link)		link_make && echo -e "OK!" && shift;;
		# WIP: Status checking
		(-d|--remove-link)		link_delete && echo -e "Links deleted!" && shift;;
		(-e|--unpack)			vpk_extract "$2" 0 && shift 2;; #OPTARG=vpkname
		(-f|--merge)			vpkfiles_merger "$2" 0 && shift 2;; #OPTARG=vpknameA,vpknameB,*
		(-v|--pack)				vpk_make "$2" 0 && shift 2;; #OPTARG=vpkname
		(-z|--merge-dirs)		vpkfiles_merger "$2" 1 && shift 2;; #OPTARG=vpknameA,vpknameB,*
		(--) shift; break;;
		(*) echo "Unimplemented option"; exit 12;; # Impossible case 
	esac
done;
