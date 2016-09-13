#include <vector>
#include "configParser.C"
#include "TString.h"

configParser *conf;

double getMetTemplatesError(std::vector<double> sample_statistical_errors, double normalization_uncertainty){
  //need MC closure bins
  //need EWK subtraction bins

  double EWK_Error = sqrt(178.2*178.2 + 539*539 + 2365*2365 + 766*766 + 21700*21700 + 1916*1916 + 752*752);
  double MC_Closure_Error;

  
}

double getRareSamplesError(std::vector<double> sample_statistical_errors, std::vector<double> sample_counts) {
  double catch_all_error = .5 //This is the amount of error to assign as a catchall number for the MC prediction. That is, we expect MC to correctly predict the answer to within this percentage.

  double error = 0;

  for(std::vector<double> it = sample_statistical_errors.begin(); it != sample_statistical_errors.end(); it++){
    error += (*it)*(*it);
  }
  for(std::vector<double> it = sample_counts.begin(); it != sample_counts.end(); it++){
    error += catch_all_error*(*it)*catch_all_error*(*it);
  }

  return sqrt(error);
}

void produceErrorTable(){
  int bin=1;
  int low, high;
  while (conf->get("bin_"+to_string(bin)+"_low") != "" ){
    low = stoi(conf->get("bin_"+to_string(bin)+"_low"));
    high = stoi(conf->get("bin_"+to_string(bin)+"_high"));

  }
}

void computeErrors(TString config_path){
  conf = new configParser(config_path);

  while(conf->loadNextConfig()){
    produceErrorTable();
  }
}