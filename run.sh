#!/bin/bash
if [ -d "output" ]; then
	rm -r output
fi
app_name=./bin/lukas_kanade_qt

#img_1=input/frame00.png	#yosmite
#img_2=input/frame01.png	#yosmite

img_1=input/1/24_00_m_512x512_gray8.png
img_2=input/1/24_1_m_512x512_gray8.png

#img_1=input/test1/img1000.jpg	#test data
#img_2=input/test1/img1800.jpg	#test data

$app_name -l $img_1 -r $img_2  -i 1 -w 8
$app_name -l $img_1 -r $img_2  -i 2 -w 8
$app_name -l $img_1 -r $img_2  -i 3 -w 8
$app_name -l $img_1 -r $img_2  -i 4 -w 8
$app_name -l $img_1 -r $img_2  -i 5 -w 8 
$app_name -l $img_1 -r $img_2  -i 6 -w 8
$app_name -l $img_1 -r $img_2  -i 8 -w 8
