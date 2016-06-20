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
	if [[ ! -z $new_dir ]]
	then
		mkdir -p ${new_dir#*=}
		addIndexToDirTree ${new_dir#*=}
	fi
}

function makeAll {
	if [[ -a $1/run_modes.conf ]]
	then
		makeHistos Z_Base $1/run_modes.conf
		makeHistos G_Base $1/run_modes.conf
		makeHistos G_Reweight $1/run_modes.conf

		makePlots $1/singleplots.conf
		makePlots $1/ratioplots.conf
		makePlots $1/ratioplots_nowt.conf
		makePlots $1/cuts.conf
	else
		echo "Can not find $1/run_modes.conf"
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