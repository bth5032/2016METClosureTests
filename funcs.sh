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
	root -l -b -q "doAll.C(\"$1\", \"$2\")"
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
		makeHistos Z_Base $1/run_modes.conf
		makeHistos G_Base $1/run_modes.conf
		makeHistos G_Reweight $1/run_modes.conf
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
	kill -9 `numjobs v | head -n-1 | cut -d' ' -f3 | xargs`
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

	i=0
	cat $1 | grep "STATS" | cut -d' ' -f3,4,5,6,7,8,9,10,11,12,13,14,15 | while read l 
	do
		if [[ $i == 0 ]]
		then
			echo $l | cut -d' ' -f 5,7,8,9,11,12,13
			i=$((i+1))
		elif [[ $i == 1 ]]
		then
			echo $l | cut -d' ' -f 5,7,8,9,11,12,13
			i=$((i+1))
		elif [[ $i == 2 ]]
		then
			echo $l
			i=0
		fi
	done
}