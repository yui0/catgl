#!/bin/sh
TEMPLATE_FILE=index.html.orig

mv *.html /tmp/

make_html() {
	out=`printf "%03d.html" ${count}`
	prev=`printf "%03d.html" $((${count}-1))`
	next=`printf "%03d.html" $((${count}+1))`

	k=1
	while read line
	do
		if [[ "${line}" =~ "@@GLSL@@" ]]; then
			((k++))
			while read include_line
			do
				echo -e "$include_line" >> ${out}
			done < ${name}
		elif [[ "${line}" =~ "@@PREV@@" ]]; then
			echo '<br><a href="'${prev}'">Prev</a>' >> ${out}
		elif [[ "${line}" =~ "@@NEXT@@" ]]; then
			echo '/<a href="'${next}'">Next</a>' >> ${out}
		else
			echo ${line} >> ${out}
		fi
	done < ${TEMPLATE_FILE}

	basename=${name##*/}
	filename=${basename%.*}
	args=
#	tex=`printf "%03d.jpg" ${count}`
	tex=${filename}".jpg"
	[ -r ${tex} ] && args='data-texture0="'${tex}'"'
	tex=${filename}"+.jpg"
	[ -r ${tex} ] && args=${args}' data-texture1="'${tex}'"'
	sed -e "s/@@ARGS@@/${args}/" -i ${out}
}

count=0
for name in *.glsl
do
	make_html
	((count++))
done
