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
	# takes in the 
	if [[ $1 == "plots" ]]
	then
		makeAllForDir $2/A/Btag/ plots
		makeAllForDir $2/A/Bveto/ plots
		makeAllForDir $2/B/Btag/ plots
		makeAllForDir $2/B/Bveto/ plots
		
		makeAllForDir $2/ewkHiggs/ plots
		makeAllForDir $2/atlas/ plots
		makeAllForDir $2/edge/ plots
	elif [[ $1 == "hists" ]]
	then
		makeAllForDir $2/A/Btag/ hists
		makeAllForDir $2/A/Bveto/ hists
		makeAllForDir $2/B/Btag/ hists
		makeAllForDir $2/B/Bveto/ hists
		
		makeAllForDir $2/ewkHiggs/ hists
		makeAllForDir $2/atlas/ hists
		makeAllForDir $2/edge/ hists
	elif [[ $1 == "all" ]]
	then
		makeAllForDir $2/A/Btag/
		makeAllForDir $2/A/Bveto/
		makeAllForDir $2/B/Btag/
		makeAllForDir $2/B/Bveto/

		makeAllForDir $2/ewkHiggs/
		makeAllForDir $2/atlas/
		makeAllForDir $2/edge/
	else
		echo "Please choose a step to run over, hists, plots, or all"
	fi
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
	kill -9 `numjobs -v | grep "root -l" | xargs`
}