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
	fname_hist="hist_out_"${1//\//_}
	fname_plots="plots_out_"${1//\//_}	

	echo $1 > $fname_hist
	makeHistosForDir $1 >> $fname_hist 2>&1


	echo $1 > $fname_plots
	makePlotsForDir $1 >> $fname_plots 2>&1
}

function makeAllForDir {
	_makeAllForDir $1 &
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
	if [[ $1 == "plots" ]]
	then
		makePlotsForDir configs/A/Btag/ > A_Btag.plots_out  2>&1 &
		makePlotsForDir configs/A/Bveto/ > A_Bveto.plots_out  2>&1 &
		makePlotsForDir configs/B/Btag/ > B_Btag.plots_out  2>&1 &
		makePlotsForDir configs/B/Bveto/ > B_Bveto.plots_out  2>&1 &
		
		makePlotsForDir configs/ewkHiggs/ > ewkHiggs.plots_out  2>&1 &
		makePlotsForDir configs/atlas/ > atlas.plots_out  2>&1 &
		makePlotsForDir configs/edge/ > edge.plots_out  2>&1 &
	elif [[ $1 == "histos" ]]
	then
		makeHistosForDir configs/A/Btag/ > A_Btag.hist_out 2>&1 &
		makeHistosForDir configs/A/Bveto/ > A_Bveto.hist_out 2>&1 &
		makeHistosForDir configs/B/Btag/ > B_Btag.hist_out 2>&1 &
		makeHistosForDir configs/B/Bveto/ > B_Bveto.hist_out 2>&1 &
		
		makeHistosForDir configs/ewkHiggs/ > ewkHiggs.hist_out 2>&1 &
		makeHistosForDir configs/atlas/ > atlas.hist_out 2>&1 &
		makeHistosForDir configs/edge/ > edge.hist_out 2>&1 &
	else
		makeAllForDir configs/A/Btag/ > A_Btag.out 2>&1 &
		makeAllForDir configs/A/Bveto/ > A_Bveto.out 2>&1 &
		makeAllForDir configs/B/Btag/ > B_Btag.out 2>&1 &
		makeAllForDir configs/B/Bveto/ > B_Bveto.out 2>&1 &

		makeAllForDir configs/ewkHiggs/ > ewkHiggs.out 2>&1 &
		makeAllForDir configs/atlas/ > atlas.out 2>&1 &
		makeAllForDir configs/edge/ > edge.out 2>&1 &
	fi
}

function numjobs {
	psout=`ps aux | grep bhashemi | grep "configs/"` 

	if [[ $1 == "v" ]]
	then 
		echo "$psout"
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