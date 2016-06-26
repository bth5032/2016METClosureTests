#include "TString.h"
#include "ScanChain.C"
#include "DefineDatasets.C"
#include "ConfigParser.C"
#include "makeWeightHisto.C"

void doAll ( TString config_name, TString config_file="configs/run_modes.conf" ) {

  ConfigParser *conf = new ConfigParser(config_file.Data());

  if ( conf->loadConfig(config_name.Data()) ){

    cout<<"Using config:"<<endl;
    conf->print();
    
    if (conf->get("reweight") == "true"){
      makeWeightHisto(conf);
    }

    if (conf->get("zjets") == "true") {
      ScanChain(getZJetsChain(conf->get("data_set")), "zjets", conf); 
    }
    
    if (conf->get("gjets") == "true") {
      ScanChain(getGJetsChain(conf->get("data_set")), "gjets", conf);  
    }

    if (conf->get("data") == "true")
    {
      ScanChain(getDataChain(conf->get("data_set")), conf->get("data_type"), conf);  
    }
    return ;
  }

  else{
    cout<<"Configuration "<<config_name<<" could not be loaded, please check the name and config file, "<<config_file<<"and try again."<<endl;
    exit(1);
  }
}
