#!/bin/bash
##
## Functions for doing 2016 MET Closure Tests
## Bobak Hashemi


function makePlots {
	root -l -b -q "drawPlots.C()"
}
function makeHistos {	
	root -l -b -q "doAll.C(\"$1\")"
}	

function makeDirectories {
	if [[ ! -d $CT_PLOTS_OUTPUT_DIR ]]
	then
		mkdir -p $CT_PLOTS_OUTPUT_DIR
		cp ~/public_html/ZMET2016/index.php ${CT_PLOTS_OUTPUT_DIR}/
	fi

	if [[ ! -d $CT_HISTO_DIR ]]
	then
		mkdir -p $CT_HISTO_DIR
	fi
}
