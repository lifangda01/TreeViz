# When using blender to merge ply to obj, the name sequence is reversed
# This script makes sure branch 0 is the first branch in obj file
total=14
i=1
j=$((total-i))
for ply in b*.ply;
do
	echo b${i}.ply to b${j}t${i}.ply
	mv b${i}.ply b${j}t${i}.ply
	let i+=1
	let j-=1
done
