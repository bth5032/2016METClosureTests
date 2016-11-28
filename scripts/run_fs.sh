. funcs.sh

function makeAllFSHists {
  makeHistos ${sample_name} configs/FS_mass_window_studies/${config_id}_onZ/run_modes.conf; 
  makeHistos ${sample_name} configs/FS_mass_window_studies/${config_id}_aboveZ/run_modes.conf; 
  makeHistos ${sample_name} configs/FS_mass_window_studies/${config_id}_belowZ/run_modes.conf; 
}

function makeFSStackPlots {
  makePlots configs/FS_mass_window_studies/${config_id}_onZ/stackplots.conf; 
  makePlots configs/FS_mass_window_studies/${config_id}_aboveZ/stackplots.conf; 
  makePlots configs/FS_mass_window_studies/${config_id}_belowZ/stackplots.conf
}

echo "Enter the name of any config for which you'd like make histograms (space seperated): "
echo "Options: "  
ls configs/FS_mass_window_studies/ | sed -e 's/_onZ//g' -e 's/_belowZ//g' -e 's/_aboveZ//g' -e 's/_ratios//g' | sort | uniq

read which_hists
hist_list=( $which_hists )

echo "Enter the name of any samples you want to remkae hists for (space seperated): "
echo "Options: "

grep "Name=" configs/FS_mass_window_studies/${hist_list[0]}_onZ/run_modes.conf | sed 's/Name=//g'

read which_samples

echo -n "Remake ratio plots (y/n)? "

read opt_remake_ratio


for hist in $which_hists
do
  config_id=$hist
  for sample in $which_samples
  do
    sample_name=$sample
    makeAllFSHists
  done
  
  makeFSStackPlots
done

if [[ $opt_remake_ratio == "y" ]]
then 
  for hist in $which_hists 
  do
    code=`grep "id = \"$hist\"" scripts/FS_Mass_Window_Study.C | sed 's/.*dataset == \([0-9]*\).*/\1/g'`
    root -l -b -q "scripts/FS_Mass_Window_Study.C($code)"
  done
fi
