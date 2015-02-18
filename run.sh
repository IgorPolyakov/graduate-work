#!/bin/bash
if [ -d "output" ]; then
	rm -r output
fi
app_name=./lukas_kanade_qt
img_1=input/frame01
img_2=input/frame00

$app_name -l $img_1.png -r $img_2.png  -d
$app_name -l $img_1.png -r $img_2.png  -i 2
$app_name -l $img_1.png -r $img_2.png  -i 3
$app_name -l $img_1.png -r $img_2.png  -i 4
$app_name -l $img_1.png -r $img_2.png  -i 5
$app_name -l $img_1.png -r $img_2.png  -i 6
$app_name -l $img_1.png -r $img_2.png  -i 7
$app_name -l $img_1.png -r $img_2.png  -i 8
$app_name -l $img_1.png -r $img_2.png  -w 2
$app_name -l $img_1.png -r $img_2.png  -w 3
$app_name -l $img_1.png -r $img_2.png  -w 4
$app_name -l $img_1.png -r $img_2.png  -w 5
$app_name -l $img_1.png -r $img_2.png  -w 6
$app_name -l $img_1.png -r $img_2.png  -w 7