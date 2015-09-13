#!/bin/sh
TEMPLATE_FILE=index.html.orig
DATA_FILE=01.glsl

# 改行文字を「\\n」という3文字の文字列に置換
# 最終行でなければループ、ループ内で次の行を読みこみ、記憶していた文字列に追加
# ∴全行読み込み一度に置換
#TEXT=`sed ':loop; N; $! b loop; s/\n/\\\\n/g' ${DATA_FILE}`
#TEXT=`awk -F\n -v ORS='\\\\n' '{print}' ${DATA_FILE}`
# TEMPLATE_FILE中の【target】という文字列の個所をDATA_FILEの内容で置換
#sed -re "s|@@GLSL@@|${TEXT}|g" ${TEMPLATE_FILE}

mv *.html /tmp/

k=1
while read line
do
	if [[ "${line}" == "<?php" ]] || [[ "${line}" == "?>" ]]; then
	:
	elif [[ "${line}" =~ "@@GLSL@@" ]]; then
		((k++))
		#file = $line
		while read include_line
		do
		#echo $include_line | grep -Ev '<\?php|\?>' >> index.html
		#done < ${file:9:9}
		echo -e "$include_line" >> index.html
		done < ${DATA_FILE}
		# cat file$k.php >> newfile.php
	else
		echo $line >> index.html
	fi
done < ${TEMPLATE_FILE}
