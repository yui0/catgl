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
			echo '<li class="ui-pagination-prev"><a href="'${prev}'">Prev</a></li>' >> ${out}
		elif [[ "${line}" =~ "@@NEXT@@" ]]; then
			echo '<li class="ui-pagination-next"><a href="'${next}'">Next</a></li>' >> ${out}
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
