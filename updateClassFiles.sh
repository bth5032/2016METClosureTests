pushd /home/users/bhashemi/Projects/GIT/Software/makeCMS3ClassFiles/
git pull 
git checkout root6
cd makeCMS3ClassFiles
root -l -b -q "makeCMS3ClassFiles.C(\"/home/users/bhashemi/Projects/GIT/Vince/CMSSW_8_0_7/src/ZMET2016/dilepbabymaker/unknown_test_80_data_ee.root\", \"t\", \"ZMET2016\", \"zmet\", \"phys\")"
mv ZMET.* /home/users/bhashemi/Projects/GIT/2016METClosureTests/
popd