#!/bin/sh

_get_filename ()
{
	realpath="${1}"
	echo $( sed -z 's|^.*/||' <<< "${realpath}" )
}

pkg_prepare_local ()
{
	local_path=$( _put_prompt "Path to targeted vpk file: " )
	def_name="$( _get_filename "${local_path}" )"
	local_name=$( _put_prompt "Name of add-on (=${def_name}): " --def-value "${def_name}" )
	local_auths=$( _put_prompt "Author of add-on (= ): " ) # comma separated
	local_tags=$( _put_prompt "Category of add-on (= ): " ) # comma separated
	local_desc=$( _put_prompt "Description of add-on (=${local_desc}): " )
	echo "${local_name}|${local_auths}|${local_tags}|${local_desc}|LOCAL|${local_path}|${local_path}"
	# install = copy to folder + add entry
}

pkg_prepare_remote ()
{
	option_source=$( _fput 2 "Choose source [steamworkshop=1,gamemap=2]: " --input-type='ENUMS' --enums='1,2' )
	remote_srcid=$( _fput 2 "ID of file: " --input-type='DIGIT')
	remote_srcname= ;
	case ${option_source} in
		1) remote_srcname='STEAMWORKSHOP';;
		2) remote_srcname='GAMEMAPS';;
		*) ;;
	esac
	dl_pkg=`matesvpk download --source="${remote_dl_src}" --id=${remote_dl_id} --file --info-stdout`
	# name|auth|tag|desc|path
	remote_name=`cut -d'|' -f2 <<< dl_pkg` # extract
	remote_auth=`cut -d'|' -f3 <<< dl_pkg` # extract
	remote_tag=`cut -d'|' -f4 <<< dl_pkg` # extract
	remote_desc=`cut -d'|' -f5 <<< dl_pkg` # extract
	remote_srcpath=`cut -d'|' -f6 <<< dl_pkg` # extract
	echo >&1 "${remote_name}|${remote_auth}|${remote_tag}|${remote_desc}|${remote_srcname}|${remote_srcname}|${remote_srcid}|${remote_srcpath}"
}

pkg_install () # name auth|tag|desc|srcname|srcvalue|srcimportpath
{
	a_id= ;
	a_name=`cut -d'|' -f1 <<< "${1}"`
	a_auth=`cut -d'|' -f2 <<< "${1}"`
	a_tag=`cut -d'|' -f3 <<< "${1}"` 
	a_desc=`cut -d'|' -f4 <<< "${1}"`
	a_srcname=`cut -d'|' -f5 <<< "${1}"`
	a_srcvalue=`cut -d'|' -f6 <<< "${1}"`
	a_srcimportpath=`cut -d'|' -f7 <<< "${1}"`

	# TODO: Options to mv instead of cp
	cp ${a_srcimportpath} ${ADDONSDOCK_DIR}

	# Add entry
	ret=`jo -f - addons[]=$(eval "jo id=${arg_id} name="${arg_name}" author="${arg_auth}" ${args_tag} src[]="${arg_srcname}" src[]="${arg_srcvalue}" ") < "${RESRCDIR}/addondict.json"`
	jq <<< ${ret} > "${RESRCDIR}/addondict.json"	
}

addondict_clamp_priority ()
{
	opt_return=${1}
	#[ "${opt_returnMode}" = '' ] && { ulog 'Please set a return mode!' 'E'; mapp_exit 1; }
	# get priorities newline list
	list0="$(jq .addons[].priority < "${RESRCDIR}/addondict.json")" 
	# get largest / size
	listC=$(wc -l <<< ${list0})
	# parse list into comma-separated string list
	list1=
	while IFS=' ' read -r line; do
		list1="${list1},${line}"
	done <<< "${list0}";
	list1=${list1:1}
	# sort, return a string list of cached indices
	list2="$(bin/sort -t1 ${list1} -zk)"
	# clamp AKA reassign each number based on listay index
	for i in $(seq 1 ${listC}); do
		oldIdx=$(cut -d',' -f"${i}" <<< "${list2}")
		# here jq returns the whole file with new content, then we out
		# that as a replacement file... Not good. May as well ditch jq.
		ret=$(jq .addons[${oldIdx}].priority=${i}\
			< "${RESRCDIR}/addondict.json")
		jq <<< ${ret} > "${RESRCDIR}/addondict.json"
	done
	case ${opt_returnMode} in
		'') ;;
		'SIZE') echo ${listC} ;;
		*) ulog 'E' "${SHELL}::${0}: Unimplemented return mode!" ;;
		# (E:) Unimplemented return mode!
	esac
}

_put_info ()
{
	flag="${1}"
	message="${2}"
	logico= ;
	case ${flag} in
		1) logico="(E:)";;
		0) logico="(::)";;
		(*) maki_error;;
	esac
	echo -e "${logico} ${message}" >&2
}

_put_prompt ()
{
	args="$( getopt -n '_put_prompt' -o '' --long 'input-type:,enums:,def-value:' -- "$@" )";
	eval set -- "${args}";
	flag= ;message= ; defvalue= ; inputtype='UNSPECIFIED' ; enums= ; inputread=;
	while : ; do
		case $1 in
			(--input-type) inputtype="$2"; shift 2;;
			(--enums) enums="$2"; shift 2;;
			(--def-value) defvalue="$2"; shift 2;;
			(--) shift; break;;
			(*);;
		esac;
	done
	message="${1}"
	logico="(?:)";
	read -p "${logico} ${message}" inputread
	[ "${inputread}" = '' ] && inputread="${defvalue}"
	[ "${inputread}" = '' ] && mapp_error;
	cstat= ;
	case ${inputtype} in
		'DIGIT') grep '[^0-9]\+' <<< ${inputread} && cstat=0 || cstat=1;;
		'NONDIGIT') grep '[0-9]\+' <<< ${inputread} && cstat=0 || cstat=1;;
		'UNSPECIFIED') cstat=1;;
		'ENUMS')
			IFS=','
			for enum in ${enums}; do
				[ "${inputread}" = "${enum}" ] && { cstat=1; break; }
				cstat=0
			done; unset IFS;
			;;
	esac
	(( cstat )) && { echo "${inputread}"; } || { mapp_error; }
}

help_root ()
{
	cat <<END
usage: addondict_manager [options]... <command> [<args>]
options:
    -h, --help     show help menu
commands:
    import local   import add-on from disk 
    import remote  import add-on from remote repository
    export         export add-ons info in the form of an
                   addonorder.csv
END
	return 0
}

_main ()
{
	. "$(cd "${0%/*}" && echo ${PWD})/init" _INTERNAL
	SHELLNAME="$(sed -z 's|/||;s|\..\+$||' <<< ${0#${0%/*}})"

	[ $# = 0 ] && { help_root && exit 1; }

	ADDON_ID=
	ADDON_NAME=
	ADDON_CREATORS=
	ADDON_TAGS=
	ADDON_PRIORITY=
	ADDON_ARCHIVE_DIR=
	ADDON_SRC=
	ADDON_SRCID=
	



	while getopts ':hlr' option; do
		case ${option} in
			h) help_root && exit 0;;
			l) pkg_prepare_local; echo hi;;
			r) pkg_prepare_remote;;
			*) ;;
		esac
	done

}
_main $@