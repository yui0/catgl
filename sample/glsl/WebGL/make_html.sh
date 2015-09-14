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
}

count=0
for name in *.glsl
do
	make_html
	((count++))
done
