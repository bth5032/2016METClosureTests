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

    if (conf->get("mode") == "rwt"){
      TString out_dir=conf->get("histo_output_dir");
      makeWeightHisto(out_dir,out_dir+"/ct_zjets_base.root", out_dir+"/ct_gjets_base.root", "vpt");
    }

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
