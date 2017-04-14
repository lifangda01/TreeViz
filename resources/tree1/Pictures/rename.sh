# first should always be least 10
first=28
default=277
total=289
j=$first
for pic in colorraw*.png;
do 
	echo "colorraw$j.png" to "temp$j.png"
	mv "colorraw$j.png" "temp$j.png"
	let j+=1
done
k=$first
i=1
j=$((total-default))
for pic in temp*.png;
do 
	if [ "$k" -ge "$default" ];
	then
		while [[ ${#i} -lt 3 ]] ; do
		    i="0${i}"
		done
		echo "temp$k.png" to "new$i.png"
		mv "temp$k.png" "new$i.png"
		let i+=1
	else
		t=$((k+j))
		while [[ ${#t} -lt 3 ]] ; do
		    t="0${t}"
		done
		echo "temp$k.png" to "new$t.png"
		mv "temp$k.png" "new$t.png"
	fi
	let k+=1	
done
rm temp*.png
echo $k
