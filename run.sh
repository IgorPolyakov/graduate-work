#!/bin/bash
if [ -d "output" ]; then
	rm -r output
fi
app_name=./lukas_kanade_qt

img_1=input/frame00.png	#yosmite
img_2=input/frame01.png	#yosmite

#img_1=input/test1/img1000.jpg	#test data
#img_2=input/test1/img1800.jpg	#test data

$app_name -l $img_1 -r $img_2  -d
$app_name -l $img_1 -r $img_2  -i 2
$app_name -l $img_1 -r $img_2  -i 3
$app_name -l $img_1 -r $img_2  -i 4
$app_name -l $img_1 -r $img_2  -i 5
$app_name -l $img_1 -r $img_2  -i 6
$app_name -l $img_1 -r $img_2  -i 7
$app_name -l $img_1 -r $img_2  -i 8
$app_name -l $img_1 -r $img_2  -w 2
$app_name -l $img_1 -r $img_2  -w 3
$app_name -l $img_1 -r $img_2  -w 4
$app_name -l $img_1 -r $img_2  -w 5
$app_name -l $img_1 -r $img_2  -w 6
$app_name -l $img_1 -r $img_2  -w 7
