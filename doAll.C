#include "TString.h"
#include "ScanChain.C"
#include "DefineDatasets.C"
#include "ConfigParser.C"

void doAll ( TString config_name, TString config_file="run_modes.conf" ) {

  cout<<"Using Histogram Directory: "<<histo_dir<<endl;

  ConfigParser *conf = new ConfigParser(config_file.Data());

  if ( conf->loadConfig(config_name.Data()) ){
    
    if (conf->get("zjets") == "true") {
      ScanChain(getZJetsChain(conf->get("data_set")), "zjets", conf); 
    }
    
    if (conf->get("gjets") == "true") {
      ScanChain(getGJetsChain(conf->get("data_set")), "gjets", conf);  
    }
    exit(0);
  
  }

  else{
    cout<<"Configuration "<<config_name<<" could not be loaded, please check the name and config file, "<<config_file<<"and try again."<<endl;
    exit(1);
  }
  
}
