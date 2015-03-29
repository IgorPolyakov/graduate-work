#!/bin/bash
if [ -d "output" ]; then
	rm -r output
fi
app_name=./bin/lukas_kanade_qt

iteration=1
size_grid=10
size_window=8

#img_2=input/frame00.png				#yosmite
#img_1=input/frame01.png				#yosmite

#img_2=input/1/24_00_m_512x512_gray8.png
#img_1=input/1/24_1_m_512x512_gray8.png

img_1=input/test1/img1.jpg		#test data
img_2=input/test1/img1800.jpg		#test data

for i in {0..1..1}
  do
	$app_name -l $img_1 -r $img_2  -i $(($iteration + $i)) -w $size_window
	echo	$app_name -l $img_1 -r $img_2  -i $(($iteration + $i)) -w $size_window
 done
