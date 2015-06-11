#!/bin/bash
cd bin
if [ -d "output" ]; then
	rm -r output
fi

app_name=./lucas_kanade

file=list
iteration=10
size_grid=16
size_window=16

#img_2=../input/yoshmite/frame00.png				#yosmite
#img_1=../input/yoshmite/frame01.png				#yosmite

img_2=../input/gray_texure/test_0_0.png
img_1=../input/gray_texure/test_1_0.png

#img_1=../input/deformation_real_img/img1.jpg		#test data
#img_2=../input/deformation_real_img/img1800.jpg	#test data

#img_1=../input/deformation_real_img/img1.jpg		#test data
#img_2=../input/deformation_real_img/img1800.jpg	#test data

if [ ! -f "$file" ] ; then
	# if not create the file
	touch "$file"
fi

echo "$img_1" > "$file"
echo "$img_2" >> "$file"

for (( j = 1; j < 100; j++ ));
#for j in 2 4 8 16 24 32 40 48 52 64 72 128
#for j in 64
	do
	#statements
	#for i in {0..1..1}
	for i in 1
	   do
		#echo	$app_name -l $file	-i $iteration -w $(($size_window + $i)) -g $size_grid
		#$app_name -l $file  -i $iteration -w $(($size_window + $i)) -g $size_grid #> /dev/null
		#echo	$app_name -l $file	-i $iteration -w $size_window -g $size_grid 
		$app_name -l $file  -i $j -w $size_window -g $size_grid #-b $j #> /dev/null

	 done
done
rm list