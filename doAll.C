#include "ScanChain.C"
#include "DefineDatasets.C"
#include "ConfigParser.C"

void doAll(TString config_name, TString config_file="run_modes.conf"){

  cout<<"Using Histogram Directory: "<<histo_dir<<endl;

  ConfigParser *conf = new ConfigParser(config_file);
  if ( conf->loadConfig(config_name) ){
    if (conf["zjets"] == "true"){
      ScanChain(getDataChain(conf["data_set"]), "zjets"); 
    }
    
    if (conf["gjets"] == "true"){
      ScanChain(getDYChain(conf["data_set"]), "gjets");  
    }
    exit(0);
  }
  else{
    cout<<"Configuration "<<config_name<<" could not be loaded, please check the name and config file, "<<config_file<<"and try again."<<endl;
    exit(1);
  }
  
}
