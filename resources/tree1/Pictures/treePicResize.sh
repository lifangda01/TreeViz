RESIZE=50%
for pic in ./*.png;
do
	echo $pic
	convert -resize $RESIZE $pic -colors 256 PNG8:$pic
done
