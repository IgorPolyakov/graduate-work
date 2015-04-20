#!/bin/bash
if [ -d "output" ]; then
	rm -r output
fi
app_name=./bin/lukas_kanade_qt

file=list
#iteration=1
#size_grid=5
#size_window=8

img_2=input/frame00.png				#yosmite
img_1=input/frame01.png				#yosmite

#img_2=input/1/24_00_m_512x512_gray8.png
#img_1=input/1/24_1_m_512x512_gray8.png

#img_1=input/test1/img1.jpg		#test data
#img_2=input/test1/img1800.jpg		#test data

if [ ! -f "$file" ] ; then
	# if not create the file
	touch "$file"
fi

echo "$img_1" > "$file"
echo "$img_2" >> "$file"

for i in {0..1..1}
  do
	$app_name -l $file  -i $(($iteration + $i)) -w $size_window -d -g $size_grid
	echo	$app_name -l $file	-i $(($iteration + $i)) -w $size_window -d -g $size_grid
 done

rm list