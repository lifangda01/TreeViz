# source folder of old format is first argument
# converted destination folder is second argument

mkdir -p $2/{BranchCircles,Cylinder,PLY,TrunkCircles}
cp $1/trunk_points_mesh.ply $2/PLY/
cp -r $1/getbranchply/*.ply $2/PLY/
cp -r $1/all/clusters/cylinderpara_*.txt $2/Cylinder/
cp -r $1/features/feature_*.txt $2/Cylinder/
cp -r $1/merged/paralayer_*.txt $2/TrunkCircles/
cp -R $1/all/all/* $2/BranchCircles
echo "Copying finished."
