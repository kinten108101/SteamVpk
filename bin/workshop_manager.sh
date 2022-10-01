#!/bin/sh
. "$(cd "${0%/*}/.." && echo ${PWD})/bin/init" _INTERNAL

MYLOGIN="/home/kinten/personal/steamcmd_keys.json"

read_mylogin ()
{
	username=$(cat "${MYLOGIN}" | jq ".accounts[0].name" | sed -z 's/\"//g')
	password=$(cat "${MYLOGIN}" | jq ".accounts[0].pass" | sed -z 's/\"//g')
	echo -e "${username},${password}" >&1
	#echo -e "output is ${username}" >&2
}

start_steamcmd ()
{
	username="$( cut -f 1 -d ',' <<< "$1")"
	password="$( cut -f 2 -d ',' <<< "$1")"
	steamcmd +force_install_dir '/home/kinten/.local/share/Steam' +login "${username}" "${passsword}";
}

workshop_get_tags ()
{
	html="${1}"
	processed="$(grep 'workshopTags' <<< ${html} | sed -z 's/<div data-panel="{&quot;type&quot;:&quot;PanelGroup&quot;}" class="workshopTags">//g;s/<span class="workshopTagsTitle">//g; s/&nbsp\;<\/span>//g;s/<\/a>//g')"
	IFS='>' read -ra lines <<< "${processed}"; processed=
	for line in "${lines[@]}"; do
		# remove whitespace lines
		line=`sed -z 's/^[[:space:]]\+//' <<< "${line}"`; [ "${line}" == '' ] && continue;
		# remove attributes
		line=`sed -z 's/<.*// ; s/,[[:space:]]//' <<< "${line}"`
		# Delete categories, which are supertags but we don't need them
		line=`sed -z '/:/d' <<< "${line}"`
		# Print
		[ "${line}" != '' ] && { echo ${line} 1>/dev/null; processed="${processed},${line}"; }
	done; processed=${processed:1}
	echo ${processed}
}
workshop_get_creators ()
{
	html="${1}"
	processed="$(sed -ne '/friendBlockContent"/,/br/p' <<< "${html}" | sed '/div/d' | sed 's/^[[:space:]]*// ; s/<br>[[:space:]]*$// ; s/|//g' )"
	out= ; while IFS='\n' read -r line; do
		out="${out},${line}"
	done <<< ${processed}; out=${out:1}
	echo "${out}"
}

workshop_download ()
{
	username="$( cut -f 1 -d ',' <<< "$1")"
	password="$( cut -f 2 -d ',' <<< "$1")"
	fileId="$( cut -f 3 -d ',' <<< "$1")"; [ "${fileId}" == "" ] && { echo -e "You have not entered ID of the published workshop item!" ; exit 1; }
	vpkname="${fileId}"
	if [ "${DOWNLOAD_NAME}" != "$UNSET_CODE" ]; then
		vpkname="${DOWNLOAD_NAME}"
	fi

	HTML=
	if [ "${bDOWNLOAD_IMAGE}" = 'y' ] || [ "${bDOWNLOAD_INFO}" = 'y' ]; then
		HTML="$(curl "https://steamcommunity.com/sharedfiles/filedetails/?id=${fileId}" 2>/dev/null)"
	fi;

	# download thumbnail
	if [ "${bDOWNLOAD_IMAGE}" = 'y' ]; then
	IMG_ADDR="$(grep "previewImageMain" <<< "${HTML}" | sed 's/.*src=\"\(.*\)\".*/\1/ ; s/\?imw=.\+\&imh\=.\+\&/\?imw\=5000\&imh\=5000\&/ ; s/letterbox\=true/letterbox\=false/' )";
	curl "${IMG_ADDR}" --output "${DOWNLOADSDIR}/${vpkname}.jpg" 2>/dev/null;
	fi
	
	DOWNLOAD_PATH="${DOWNLOADSDIR}/${vpkname}.vpk"
	
	if [ "${bDOWNLOAD_FILE}" = 'y' ]; then
	eval steamcmd +force_install_dir "${WM_DIR}" +login "${username}" "${passsword}" +workshop_download_item 550 "${fileId}" +quit 1>&2;
	echo -e "" >&2 # Fix a bug where +quit results in steamcmd quitting without newline...
	downloaded_file="$(find "${WM_DIR}/steamapps/workshop/content/550/${fileId}" -type f)";
	mv "${downloaded_file}" "${DOWNLOAD_PATH}";
	fi;

	if [ "${bDOWNLOAD_INFO}" = 'y' ]; then
		# remote_dl_status|name|auth|tag|desc|path
		INFO_STATUS=0
		# Remember to strip '|'
		INFO_TITLE=`grep 'workshopItemTitle' <<< ${HTML} | sed -z 's/.*>\(.\+\)<.*/\1/ ; s/|//g'`
		#INFO_UPLOADER=`grep 'myworkshopfiles/?appid=550' <<< ${HTML} | sed -z 's/.*?appid=550">\(.\+\)<\/a>.*/\1/g;s/'\''s Workshop$// ; s/|//g'`
		INFO_CREATORS=`workshop_get_creators "${HTML}"`
		INFO_TAGS=`workshop_get_tags "${HTML}"`
		INFO_DESC=`grep "workshopItemDescription\"" <<< "${HTML}" | sed -z 's/.*id=\"highlightContent\">\(.*\)<\/div>.*/\1/g ; s/|//g'`
		INFO_PATH="${DOWNLOAD_PATH}"
		delim='|'
		echo -E "${INFO_TITLE}${delim}${INFO_CREATORS}${delim}${INFO_TAGS}${delim}${INFO_DESC}${delim}${INFO_PATH}"
		#result=(0 "${INFO_TITLE}" "${INFO_UPLOADER}" "${INFO_TAGS}" "${INFO_DESC}" "${INFO_PATH}")
		#echo -E ${result[@]}
	fi

	rm -r "${WM_DIR}"/*
	
}

HELPMENU="Usage: $0 [MODE] <VALUE>
	-h				Help menu;
	-s <username>,<password>	Start steam_cmd with real user and some custom configurations, the only way to download L4D2's workshop content;
"
if [ $# -eq 0 ]; then echo -e "$(</dev/stdin)"; fi;
UNSET_CODE='21'
bDOWNLOAD_IMAGE=n;
bDOWNLOAD_FILE=n;
DOWNLOAD_SOURCE=${UNSET_CODE};
DOWNLOAD_FILEID=${UNSET_CODE};
DOWNLOAD_NAME=${UNSET_CODE};
args= ; while [[ $@ != '' ]]; do
	case $1 in
		(--source)		args="${args} -s" && shift;;
		(--id)			args="${args} -i" && shift;;
		(--name)		args="${args} -n" && shift;;
		(--file)		args="${args} -f" && shift;;
		(--image)		args="${args} -m" && shift;;
		(--full)		args="${args} -fm" && shift;;
		(--info-stdout)	args="${args} -x" && shift;;
		(*) args="${args} ${1}" && shift;;
	esac
done
eval set -- "${args}"
while getopts ':s:i:n:ofmx' OPTION; do
	case ${OPTION} in
		(s) DOWNLOAD_SOURCE="${OPTARG}";;
		(i) DOWNLOAD_FILEID="${OPTARG}";;
		(n) DOWNLOAD_NAME="${OPTARG}";;
		(o) bDOWNLOAD_INFO=y;;
		(f) bDOWNLOAD_FILE=y;;
		(m) bDOWNLOAD_IMAGE=y;;
		(x) bDOWNLOAD_INFO=y;;
		(*) ulog "Unimplemented option in getopts!" 'E'; exit 1 ;;
	esac
done

if [ "${DOWNLOAD_FILEID}" != "${UNSET_CODE}" ] && [ "${DOWNLOAD_SOURCE}" != "${UNSET_CODE}" ]; then
	case ${DOWNLOAD_SOURCE} in
		(steamcommunity.com) workshop_download "$( read_mylogin ),${DOWNLOAD_FILEID}" && exit 0;;
		(gamemaps.com) ulog 'Stay tuned!' 'E' && exit 1;;
		(*) ulog 'Unimplemented source!' 'E' && exit 1;;
	esac
else ulog 'Both DB source and ID of published item provided!' 'E' ; exit 1;
fi;