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

function setOutputLocations {
	if [[ $# > 0 ]]
	then
		if [[ ${1:(-4)} == "conf" ]]
		then
			SR_IDENTITY=${1#*configs/}
			SR_IDENTITY=`dirname $SR_IDENTITY`/
		else
			echo "You must give the location to a .conf file."
			return 0
		fi
	fi
	
	HIST_OUTPUT_LOCATION=`sed '4q;d' ConfigHelper.C | sed "s/.*HIST_OUTPUT_LOCATION=\"\(.*\)\";*/\1/g"`
	PLOT_OUTPUT_LOCATION=`sed '5q;d' ConfigHelper.C | sed "s/.*PLOT_OUTPUT_LOCATION=\"\(.*\)\";*/\1/g"`
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

	setOutputLocations $conf_filename

	mkdir -p ${HIST_OUTPUT_LOCATION}${SR_IDENTITY}
	mkdir -p ${PLOT_OUTPUT_LOCATION}${SR_IDENTITY}
	addIndexToDirTree ${PLOT_OUTPUT_LOCATION}${SR_IDENTITY}
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

	while [[ ${topdir%ZMET2016_PostICHEP*} == "/home/users/bhashemi/public_html/" ]]
	do
		
		if [[ ! -a ${topdir}/index.php ]]
		then
			cp ~/public_html/index.php ${topdir}/index.php
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

	if [[ -z $HIST_OUTPUT_LOCATION || -z $SR_IDENTITY ]]
	then
		echo "please run setOutputLocations <path_to_run_modes.conf> first"
		return 1
	fi

	scp uaf:${HIST_OUTPUT_LOCATION}${SR_IDENTITY}*.root histos/ 
}

function killjobs {
	kill -9 `ps aux | grep "^bhashemi" | grep "root" | head -n-1 | cut -d' ' -f2 | xargs`
}

function addHists {
	if [[ $# < 2 ]]
	then
		echo "Calls hadd for you and determines which files you mean and where to put output by file's config Name"
		echo "addHists <output_fname> <input_1> <input_2> ..."
		return 1
	fi

	if [[ -z $HIST_OUTPUT_LOCATION || -z $SR_IDENTITY ]]
	then
		echo "please run setOutputLocations <path_to_run_modes.conf> first"
		return 1
	fi

	HADD_LIST=${HIST_OUTPUT_LOCATION}${SR_IDENTITY}${2}".root"

	for arg in ${@:3}
	do
		HADD_LIST="$HADD_LIST ${HIST_OUTPUT_LOCATION}${SR_IDENTITY}${arg}.root" 
	done

	echo "Running: hadd ${HIST_OUTPUT_LOCATION}${SR_IDENTITY}$1.root $HADD_LIST"

	hadd ${HIST_OUTPUT_LOCATION}${SR_IDENTITY}$1.root $HADD_LIST
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
