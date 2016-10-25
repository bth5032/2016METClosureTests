#include "TString.h"
#include "ScanChain.C"
#include "DefineDatasets.C"
#include "ConfigParser.C"
#include "makePtReweightHisto.C"

void runScanChain(ConfigParser* conf){
  cout<<"Using config:"<<endl;
  conf->print();

  if (conf->get("reweight") == "true"){
    cout<<"Making Reweight Histogram"<<endl;
    makePtReweightHisto(conf);
  }

  if (conf->get("data") == "true"){
    if (conf->get("zjets") == "true"){
      ScanChain(getDataZChain(conf->get("data_set")), conf->get("data_type"), conf);  
    }
    if (conf->get("gjets") == "true"){
      ScanChain(getDataPhotonChain(conf->get("data_set")), conf->get("data_type"), conf);  
    }
  }
  else if (conf->get("rares") == "true"){
    ScanChain(getRareChain(conf->get("data_set")), conf->get("data_set"), conf);
  }
  else{
    TChain * mc_chain = new TChain("t");
    if (conf->get("FSBKG") == "true"){
      mc_chain->Add(getFSMCChain(conf->get("data_set")));
    }
    if (conf->get("zjets") == "true" && ! (conf->get("FSBKG") == "true") ) {
      mc_chain->Add(getZJetsChain(conf->get("data_set")));
    }
    if (conf->get("gjets") == "true") {
      mc_chain->Add(getGJetsChain(conf->get("data_set")));
    }
    if (conf->get("rare") == "true"){
      mc_chain->Add(getRareChain(conf->get("data_set")));
    }

    ScanChain(mc_chain, conf->get("data_type"), conf);
  }
}

void doAll ( TString config_name, TString config_file="configs/run_modes.conf" ) {

  ConfigParser *conf = new ConfigParser(config_file.Data());

  if (config_name == "all") {
    while ( conf->loadNextConfig() )
    {
      runScanChain(conf);
    }
  }

  else if ( conf->loadConfig(config_name.Data()) ){
    runScanChain(conf);
  }

  else{
    cout<<"Configuration "<<config_name<<" could not be loaded, please check the name and config file, "<<config_file<<"and try again."<<endl;
    exit(1);
  }
}
