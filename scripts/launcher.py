"""This script will be an interactive way to launch the production of new histograms and plots.

To make hists, it looks through the direcotry given when running (or configs/ by default) to find 
all the configs available to run over (looking for run_modes.conf)

To make plots it does the same looking for other .conf files with plots in the name.

When making hists, it reads the configs and asks you which of the samples you want to run over.
Then it asks if there are any plots you want to remake in the same directory as the run_modes file."""