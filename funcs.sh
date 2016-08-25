#!/bin/bash
##
## Functions for doing 2016 MET Closure Tests
## Bobak Hashemi


function makePlots {
	mkdirs $1
	root -l -b -q "drawPlots.C(\"$1\")"
}

function makeHistos {	
	mkdirs $2
	root -l -b -q "doAll.C+(\"$1\", \"$2\")"
}	

function mkdirs {
	conf_filename=$1
	#For Histo Configs
	new_dir=`grep DEFAULT::histo_output_dir < $conf_filename`
	if [[ ! -z $new_dir ]]
	then
		mkdir -p ${new_dir#*=}
	fi

	#For Plots Configs
	new_dir=`grep DEFAULT::save_dir < $conf_filename`
	for l in `echo $new_dir`
	do
		if [[ ! -d ${l#*=} ]]
		then
			mkdir -p ${l#*=}
			addIndexToDirTree ${l#*=}
		fi
	done
}

function _makeAllForDir {
	fname_hist=${1//\//_}
	fname_hist=${fname_hist%_}.hist_out #remove trailing _, add extension

	fname_plots=${1//\//_}
	fname_plots=${fname_plots%_}.plots_out #remove trailing _, add extension

	if [[ $2 == "hists" ]]
	then
		echo $1 > outputs/$fname_hist
		makeHistosForDir $1 >> outputs/$fname_hist 2>&1
	elif [[ $2 == "plots" ]]
	then
		echo $1 > outputs/$fname_plots
		makePlotsForDir $1 >> outputs/$fname_plots 2>&1
	elif [[ $2 == "all" ]]
	then
		echo $1 > outputs/$fname_hist
		makeHistosForDir $1 >> outputs/$fname_hist 2>&1

		echo $1 > outputs/$fname_plots
		makePlotsForDir $1 >> outputs/$fname_plots 2>&1
	else
		echo "Please choose a step to run over, hists, plots, or all"
	fi
}

function makeAllForDir {
	_makeAllForDir $1 $2 &
}

function makeHistosForDir {
	if [[ -a $1/run_modes.conf ]]
	then
		#makeHistos Z_Base $1/run_modes.conf
		#makeHistos G_Base $1/run_modes.conf
		#makeHistos G_Reweight $1/run_modes.conf
		makeHistos all $1/run_modes.conf
	else
		echo "Can not find $1/run_modes.conf"
	fi
}

function makePlotsForDir {
	if [[ -a $1/singleplots.conf ]]
	then
		makePlots $1/singleplots.conf
	else
		echo "Can not find $1/singleplots.conf"
	fi

	if [[ -a $1/ratioplots.conf ]]
	then
		makePlots $1/ratioplots.conf
	else
		echo "Can not find $1/ratioplots.conf"
	fi

	if [[ -a $1/ratioplots_nowt.conf ]]
	then
		makePlots $1/ratioplots_nowt.conf
	else
		echo "Can not find $1/ratioplots_nowt.conf"
	fi

	if [[ -a $1/ratioplots_HT.conf ]]
	then
		makePlots $1/ratioplots_HT.conf
	fi

	if [[ -a $1/cuts.conf ]]
	then
		makePlots $1/cuts.conf
	else
		echo "Can not find $1/cuts.conf"
	fi

	if [[ -a $1/statsplots.conf ]]
	then
		makePlots $1/statsplots.conf
	else
		echo "Can not find $1/statsplots.conf"
	fi

	if [[ -a $1/nvert.conf ]]
	then
		makePlots $1/nvert.conf
	fi
}

function addIndexToDirTree {
	#Adds the file at ~/public_html/ZMET2016/index.php into everything inside of the ~/public_html/ClosureTests/ directory for the directory given as $1.
	topdir=$1

	while [[ ${topdir%ClosureTests*} == "/home/users/bhashemi/public_html/" ]]
	do
		
		if [[ ! -a ${topdir}/index.php ]]
		then
			cp ~/public_html/ZMET2016/index.php ${topdir}/index.php
		fi

		topdir=`dirname $topdir`
	
	done
}

function makeAllConfigs {
	makeAllForDir $2/A/Btag/ $1
	makeAllForDir $2/A/Bveto/ $1
	makeAllForDir $2/B/Btag/ $1
	makeAllForDir $2/B/Bveto/ $1
	
	makeAllForDir $2/ewkHiggs/ $1
	makeAllForDir $2/atlas/ $1
	makeAllForDir $2/edge/ $1
}

function numjobs {
	psout=`ps aux | grep bhashemi | grep "configs/"` 

	if [[ $1 == "v" ]]
	then 
		echo "$psout" | grep "root -l"
	fi

	echo "scale=3; "`echo "$psout" | wc  -l`"*(1/2) - 1/2" | bc;
}

function pullHists {
	dname=`cat $1/run_modes.conf | grep DEFAULT::histo_output_dir=`
	dname=${dname#*=}

	srname=`cat $1/run_modes.conf | grep signal_region=`
	srname=${srname#*=}

	scp uaf:$dname/ct_*${srname}*.root histos/ 
}

function pullOutput {
	scp uaf:~/Projects/GIT/2016METClosureTests/*.hist_out outputs/
	scp uaf:~/Projects/GIT/2016METClosureTests/*.plots_out outputs/
	scp uaf:~/Projects/GIT/2016METClosureTests/hist_out* outputs/
	scp uaf:~/Projects/GIT/2016METClosureTests/plots_out* outputs/
}

function killjobs {
	kill -9 `ps aux | grep "^bhashemi" | grep "root" | head -n-1 | cut -d' ' -f2 | xargs`
}

function addRareHists {
	for i in ATLAS A_btag A_bveto B_btag B_bveto EdgeZ EWK_Higgs
	do 
		root -l -b- -q "AddRareHists.C(\"$i\", \"/nfs-7/userdata/bobak/GJetsClosureTests2016/Rares/\")" 
	done
}

function makeRareHists {
	makeHistos all configs/Rares/A/Btag/run_modes.conf > outputs/configs_Rares_A_Btag.hist_out 2>&1 &
	makeHistos all configs/Rares/A/Bveto/run_modes.conf > outputs/configs_Rares_A_Bveto.hist_out 2>&1 &
	makeHistos all configs/Rares/B/Btag/run_modes.conf > outputs/configs_Rares_B_Btag.hist_out 2>&1 &
	makeHistos all configs/Rares/B/Bveto/run_modes.conf > outputs/configs_Rares_B_Bveto.hist_out 2>&1 &
	makeHistos all configs/Rares/edge/run_modes.conf > outputs/configs_Rares_edge.hist_out 2>&1 &
	makeHistos all configs/Rares/atlas/run_modes.conf > outputs/configs_Rares_atlas.hist_out 2>&1 &
	makeHistos all configs/Rares/ewkHiggs/run_modes.conf > outputs/configs_Rares_ewkHiggs.hist_out 2>&1 &
}

function getSRs {
	if [[ $1 == "dirs" ]]
	then
		echo "A/Btag/"
		echo "A/Bveto/"
		echo "B/Btag/"
		echo "B/Bveto/"
		echo "edge/"
		echo "atlas/"
		echo "ewkHiggs/"
	else
		echo "A_btag"
		echo "A_bveto"
		echo "B_btag"
		echo "B_bveto"
		echo "EdgeZ"
		echo "ATLAS"
		echo "EWK_Higgs"
	fi
}

function makeL1PrescaleWeightHists {
	OutputDir=/nfs-7/userdata/bobak/GJetsClosureTests2016/Data/
	rm ${OutputDir}L1PrescaleWeight*.root

	for j in nVert_HLT_Photon22_R9Id90_HE10_IsoM nVert_HLT_Photon30_R9Id90_HE10_IsoM nVert_HLT_Photon36_R9Id90_HE10_IsoM
	do
		for i in `getSRs`
		do
			output_location=${OutputDir}L1PrescaleWeight_$i.root
			infile1=${OutputDir}ct_Z_Base_$i.root
			infile2=${OutputDir}ct_G_Base_$i.root
			hist1="zjets_nVert"
			hist2="gjets_"$j
			output_hist_name="rwt_"$j
			root -l -b -q "makeWeightHisto_noconf.C(\"${output_location}\",\"${infile1}\",\"${infile2}\",\"${hist1}\",\"${hist2}\",\"${output_hist_name}\")"
		done
	done
}

function closureTable {
	if [[ $# < 1 ]]
	then
		echo "closureTable <path_to_plots_file>"
		return
	fi

	cat $1 | grep "STATS" | cut -d' ' -f3,4,5,6,7,8,9,10,11,12,13,14,15 > lines.tmp

	title[0]="Sample" # Holds the bin
	zjets[0]="Zjets"
	gjets[0]="Gjets"
	ratio[0]="Ratio"
	i=1
	j=0

	while read -r l 
	do
		#echo "$l"
		if [[ $j == "0" ]]
		then
			title[$i]=`echo $l | cut -d' ' -f 7,9 | sed 's/\([[:digit:]]*\) \([[:digit:]]*\)/\1-\2/g' `
			zjets[$i]=`echo $l | cut -d' ' -f 11`
			zjets[$i]=${zjets[$i]}"+/-"`echo $l | cut -d' ' -f 13`
			#echo $j $i
			j=$((j+1))
		elif [[ $j == "1" ]]
		then
			gjets[$i]=`echo $l | cut -d' ' -f 11`
			gjets[$i]=${gjets[$i]}"+/-"`echo $l | cut -d' ' -f 13`
			#echo $j $i
			j=$((j+1))
		elif [[ $j == "2" ]]
		then
			ratio[$i]=`echo $l | cut -d' ' -f 2`
			ratio[$i]=${ratio[$i]}"+/-"`echo $l | cut -d' ' -f 5`
			#echo $j $i
			i=$((i+1))
			j=0
		fi
	done < lines.tmp
	
	#echo $i

	echo "======================"

	for k in `seq 0 $((i-1))`
	do
		echo -n ${title[$k]}" "
	done
	echo ""

	for k in `seq 0 $((i-1))`
	do
		echo -n ${zjets[$k]}" "
	done
	echo ""

	for k in `seq 0 $((i-1))`
	do
		echo -n ${gjets[$k]}" "
	done
	echo ""

	for k in `seq 0 $((i-1))`
	do
		echo -n ${ratio[$k]}" "
	done

	echo ""

	#	Cleanup
	unset title
	unset zjets
	unset gjets
	unset ratio
	unset i
	unset j
	rm lines.tmp
}

function getPredictionTable {
	cat $1 | grep STATS | sed -e 's/Data Yield/Signal/g' -e 's/Flavor Symmetric/FSBKG/g' -e 's/Z prediction/ZJets/g' | cut -d' ' -f 2,4,6,8,10 > lines.tmp

	title[0]="Sample" # Holds the bin
	signal[0]="Signal"
	zjets[0]="MET_Template"
	FSBKG[0]="Flavor_Symmetric"
	ZZ[0]="ZZ"
	WZ[0]="WZ"
	TTV[0]="TTV"
	VVV[0]="VVV"
	SumBG[0]="Sum_of_BG"
	i=1 #number for columns, equal to number of MET regions
	j=0 #number for BGs/Signal. equal to a row number

	while read -r l 
	do
		larr=($l)
		if [[ $j == "0" ]]
		then
			title[$i]=${larr[1]}"-"${larr[2]}
			signal[$i]=${larr[3]}"+/-"${larr[4]}
			#echo $j $i
			j=$((j+1))
		elif [[ $j == "1" ]]
		then
			ZZ[$i]=${larr[3]}"+/-"${larr[4]}
			#echo $j $i
			j=$((j+1))
		elif [[ $j == "2" ]]
		then
			WZ[$i]=${larr[3]}"+/-"${larr[4]}
			#echo $j $i
			j=$((j+1))
		elif [[ $j == "3" ]]
		then
			VVV[$i]=${larr[3]}"+/-"${larr[4]}
			#echo $j $i
			j=$((j+1))
		elif [[ $j == "4" ]]
		then
			TTV[$i]=${larr[3]}"+/-"${larr[4]}
			#echo $j $i
			j=$((j+1))
		elif [[ $j == "5" ]]
		then
			FSBKG[$i]=${larr[3]}"+/-"${larr[4]}
			#echo $j $i
			j=$((j+1))
		elif [[ $j == "6" ]]
		then
			zjets[$i]=${larr[3]}"+/-"${larr[4]}
			#echo $j $i
			j=$((j+1))
		elif [[ $j == "7" ]]
		then
			SumBG[$i]=${larr[3]}"+/-"${larr[4]}
			#echo $j $i
			i=$((i+1))
			j=0
		fi
	done < lines.tmp
	
	#echo $i

	echo "======================"

	for k in `seq 0 $((i-1))`
	do
		echo -n ${title[$k]}" "
	done
	echo ""

	for k in `seq 0 $((i-1))`
	do
		echo -n ${signal[$k]}" "
	done
	echo ""

	for k in `seq 0 $((i-1))`
	do
		echo -n ${SumBG[$k]}" "
	done
	echo ""

	for k in `seq 0 $((i-1))`
	do
		echo -n ${zjets[$k]}" "
	done
	echo ""

	for k in `seq 0 $((i-1))`
	do
		echo -n ${FSBKG[$k]}" "
	done
	echo ""

	for k in `seq 0 $((i-1))`
	do
		echo -n ${ZZ[$k]}" "
	done
	echo ""

	for k in `seq 0 $((i-1))`
	do
		echo -n ${WZ[$k]}" "
	done
	echo ""

	for k in `seq 0 $((i-1))`
	do
		echo -n ${TTV[$k]}" "
	done
	echo ""

	for k in `seq 0 $((i-1))`
	do
		echo -n ${VVV[$k]}" "
	done
	echo ""


	echo -n "SumRares "
	for k in `seq 1 $((i-1))`
	do
		vvv=${VVV[$k]}
		ttv=${TTV[$k]}
		wz=${WZ[$k]}
		zz=${ZZ[$k]}
		echo -n `echo ${vvv%\+*}+${ttv%\+*}+${wz%\+*}+${zz%\+*} | bc`" "
	done
	echo ""

	#	Cleanup
	unset title
	unset signal
	unset SumBG
	unset zjets
	unset FSBKG
	unset ZZ
	unset WZ
	unset TTV
	unset VVV
	unset vvv
	unset zz
	unset wz
	unset ttv
	unset i
	unset j
	rm lines.tmp
}