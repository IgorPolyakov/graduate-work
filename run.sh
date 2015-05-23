#!/bin/bash
cd bin
if [ -d "output" ]; then
	rm -r output
fi

app_name=./lucas_kanade

file=list
iteration=10
size_grid=16
size_window=10

#img_2=../input/yoshmite/frame00.png				#yosmite
#img_1=../input/yoshmite/frame01.png				#yosmite

#img_2=../input/gray_texure/24_00_m_512x512_gray8.png
#img_1=../input/gray_texure/24_1_m_512x512_gray8.png

#img_1=../input/deformation_real_img/img1.jpg		#test data
#img_2=../input/deformation_real_img/img1800.jpg	#test data

img_1=../input/deformation_real_img/img1.jpg		#test data
img_2=../input/deformation_real_img/img1800.jpg		#test data

if [ ! -f "$file" ] ; then
	# if not create the file
	touch "$file"
fi

echo "$img_1" > "$file"
echo "$img_2" >> "$file"

for i in {1..100..5}
  do
	echo	$app_name -l $file	-i $iteration -w $(($size_window + $i)) -g $size_grid
	$app_name -l $file  -i $iteration -w $(($size_window + $i)) -g $size_grid > /dev/null
 done

rm list