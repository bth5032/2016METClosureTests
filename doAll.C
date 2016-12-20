#include "TString.h"
#include "ScanChain.C"
#include "DefineDatasets.C"
#include "ConfigParser.C"
#include "makePtReweightHisto.C"

void runScanChain(ConfigParser* conf){
  cout<<"Using config:"<<endl;
  conf->print();

  if (conf->get("vpt_reweight") == "true"){
    cout<<"Making Reweight Histogram"<<endl;
    makePtReweightHisto(conf);
  }

  ScanChain(getTChain(conf->get("data_set")), conf);
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

int main(TString config_name, TString config_file="configs/run_modes.conf") {
  doAll(config_name, config_file);

  return 0;
}