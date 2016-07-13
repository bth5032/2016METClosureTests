#! /bin/bash
#This script will check which events pass through my code vs. Vinces code and print out a list 
#of event numbers which I can check in ScanChain

VinceOutputsFile=$1
echo "Pulling Vince's outputs from file: "$VinceOutputsFile
MyOutputsFile=$2
echo "Pulling my outputs from file: "$MyOutputsFile

cat VinceOutputsFile | grep "^[0-9]*$" | sort -n > vinceevents.tmp
cat MyOutputsFile | grep "EVENT-LIST" | cut -d' ' -f2 | sort -n > myevents.tmp

diff vinceevents.tmp myevents.tmp | grep '>' > inMineNotVince.tmp
diff vinceevents.tmp myevents.tmp | grep '>' > inVinceNotMine.tmp

echo "set<int> inVinceNotMine = {"`cat inVinceNotMine.tmp | xargs | tr ' ' ','`"}"
echo "set<int> inMineNotVince = {"`cat inMineNotVince.tmp | xargs | tr ' ' ','`"}"