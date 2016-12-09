#!/bin/bash
##
## Functions for doing 2016 MET Closure Tests
## Bobak Hashemi


function makePlots {
	mkdirs $1 plots
	nice -n 19 root -l -b -q "drawPlots.C(\"$1\")"
}

function makeHistos {	
	mkdirs $2 hists
	nice -n 19 root -l -b -q "doAll.C+(\"$1\", \"$2\")"
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
	
	HIST_OUTPUT_LOCATION=`cat ConfigHelper.C | grep "TString HIST_OUTPUT_LOCATION=" |sed "s/.*HIST_OUTPUT_LOCATION=\"\(.*\)\";*/\1/g"`
	PLOT_OUTPUT_LOCATION=`cat ConfigHelper.C | grep "TString PLOT_OUTPUT_LOCATION=" | sed "s/.*PLOT_OUTPUT_LOCATION=\"\(.*\)\";*/\1/g"`
}

function mkdirs {
	conf_filename=$1
	setOutputLocations $conf_filename 

	if [[ $2 == "hists" ]]
	then
		#Make Hist output location if it's not there
		if [[ ! -d ${HIST_OUTPUT_LOCATION}${SR_IDENTITY} ]]
		then	
			mkdir -p ${HIST_OUTPUT_LOCATION}${SR_IDENTITY}
		fi

		#Legacy Style For Hist Configs
		new_dir=`grep DEFAULT::histo_output_dir < $conf_filename`
		if [[ ! -z $new_dir ]]
		then
			mkdir -p ${new_dir#*=}
		fi
	fi

	if [[ $2 == "plots" ]]
	then
		#Make plot output location if it's not there.
		if [[ ! -d ${PLOT_OUTPUT_LOCATION}${SR_IDENTITY}`basename $conf_filename .conf`"/Debug/" ]]
		then
			mkdir -p ${PLOT_OUTPUT_LOCATION}${SR_IDENTITY}`basename $conf_filename .conf`"/Debug/" #Make up to path/to/config/Debug
			addIndexToDirTree ${PLOT_OUTPUT_LOCATION}${SR_IDENTITY}`basename $conf_filename .conf`"/Debug/" #Add index.php to each new folder
		fi

		#Legacy Style For Plots Configs
		new_dir=`grep DEFAULT::save_dir < $conf_filename`
		for l in `echo $new_dir`
		do
			if [[ ! -d ${l#*=} ]]
			then
				mkdir -p ${l#*=}"/Debug"
				addIndexToDirTree ${l#*=}"/Debug"
			fi
		done
	fi
}

function _makeAllForDir {
	fname_hist=${1//\//_} #turn path into _ seperated
	fname_hist=${fname_hist//__/_} #remove double underline string
	fname_hist=${fname_hist%_}.hist_out #remove trailing _, add extension

	fname_plots=${1//\//_} #turn path into _ seperated string
	fname_plots=${fname_plots//__/_} #remove double underline
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
	if [[ $# < 2 ]]
	then
		echo "makeAllForDir <path_to_configs> <all/hists/plots>"
	else
		_makeAllForDir $1 $2 &
	fi
	
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
	for i in `ls ${1}/*plots*.conf`
	do
		makePlots $i
	done
}

function addIndexToDirTree {
	#Adds the file at ~/public_html/ZMET2016/index.php into everything inside of the ~/public_html/ClosureTests/ directory for the directory given as $1.
	topdir=$1

	while [[ ${topdir%ZMET2016_NovemberClean*} == "/home/users/bhashemi/public_html/" ]]
	do
		
		if [[ ! -a ${topdir}/index.php ]]
		then
			cp ~/public_html/index.php ${topdir}/index.php
		fi

		topdir=`dirname $topdir`
	
	done
}

function makeAllConfigs {
	if [[ $# < 2 ]]
	then
		echo "makeAllConfigs <all/plots/hists> <path_to_configs>"
		return
	fi

	for i in Strong_Btag/2j/ Strong_Btag/4j/ Strong_Btag/6j/ Strong_Bveto/2j/ Strong_Bveto/4j/ Strong_Bveto/6j/ TChiHZ/ 2j/ ICHEP/ ICHEP_baseline/ baseline/ baseline_18fb/
	do
		makeAllForDir $2/${i} $1
	done
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
