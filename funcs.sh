#!/bin/bash
##
## Functions for doing 2016 MET Closure Tests
## Bobak Hashemi


function makePlots {
	root -l -b -q "drawPlots.C(\"$1\")"
}
function makeHistos {	
	root -l -b -q "doAll.C(\"$1\")"
}	

function mkplotdir {
	if [[ ! -d $1 ]]
	then
		mkdir -p $1
		cp ~/public_html/ZMET2016/index.php ${1}/
	fi
}

function makeAll {
	makeHistos Z_Base
	makeHistos G_Base
	makeHistos G_Reweight

	makePlots configs/singleplots.conf
	makePlots configs/ratioplots.conf
	makePlots configs/ratioplots_nowt.conf
	makePlots configs/cuts.conf
}