#!/bin/sh
jsonobj_rawfile=$( realpath "$1")
# echo $(eval 'cat ${jsonobj_rawfile}')
jsonobj_filtered= ; while IFS= read -r line; do
	line=`sed -z 's/\/\/.*//g' <<< "${line}"`;
	[[ `sed -z 's/\t//g ; s/,[[:space:]]*$//g' <<< ${line}` == '' ]] && continue;
	jsonobj_filtered="${jsonobj_filtered}${line}";
	#echo -E ${line}
	printf '%s\n' "$line";
done < "${jsonobj_rawfile}"
echo -e DONE
jsonobj_filtered="$( sed -z 's/,[[:space:]]\+"/,"/g ; s/\n//g ; s/\t//g ; s/,}/}/g ; s/,]/]/g ; s/, }/}/g' <<< ${jsonobj_filtered} )"
echo ${jsonobj_filtered}