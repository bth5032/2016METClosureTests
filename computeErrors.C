#ifndef INCLUDED_COMPUTE_ERROS
#define INCLUDED_COMPUTE_ERROS

#include <vector>
#include <utility>
#include "TString.h"
#include "RooHistError.h"
#include "getEWKErrorNums.C"

using namespace std;

double err_mult(double A, double B, double errA, double errB) {
  /* A = data yeild in norm bin
  B = template yield in norm bin
  errA/errB = statistical error in those bins*/
  return sqrt((A/B)*(A/B)*(pow(errA/A,2) + pow(errB/B,2)));
}

vector<double> getMetTemplatesError(vector<double> stat_err, vector<double> bin_count, double normalization, int norm_bin, TString SR){
  /* stat_err == statistical error on the template bins
     bin count == bin count on template bins
     normalziation == bin count to which the sample was normalized
     normalziation_bg == bin count of the templates which was normalized to 'normalization'
     SR == name of signal region */
  vector<double> output_errors;

  normalization = err_mult(normalization, bin_count[norm_bin], sqrt(normalization), stat_err[norm_bin]);

  //=========
  // Input EWK and Closure Errors
  //=========

  vector<double> No_EWK_BinCount;
  
  pair<double, vector<double>> EWK_Data;
  double EWK_LowBin;
  vector<double> MC_Closure_Error;

  if (SR == "Strong_Btag_2j"){
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.2);
    MC_Closure_Error.push_back(.2);
    MC_Closure_Error.push_back(.2);
  }
  else if(SR == "Strong_Btag_4j"){
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
  }
  else if (SR == "Strong_Btag_6j"){
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.5);
    MC_Closure_Error.push_back(.5);
  }
  else if(SR == "Strong_Bveto_2j"){
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.15);
    MC_Closure_Error.push_back(.15);
    MC_Closure_Error.push_back(.15);
  }
  else if(SR == "Strong_Bveto_4j"){
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
  }
  else if(SR == "Strong_Bveto_6j"){
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.30);
    MC_Closure_Error.push_back(.30);
  }
  else if(SR == "TChiHZ"){
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.50);
    MC_Closure_Error.push_back(.50);
    MC_Closure_Error.push_back(.50);
  }
  else if(SR == "TChiWZ"){
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.30);
    MC_Closure_Error.push_back(.30);
    MC_Closure_Error.push_back(.30);
  }
  else if(SR == "2j"){
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.50);
    MC_Closure_Error.push_back(.50);
    MC_Closure_Error.push_back(.50);
  }
  else{
    throw std::invalid_argument("Invalid or missing SR set in config. Please check config variable \"SR\". (got \'"+SR+"\')");
  }

  EWK_Data = getEWKNumsForSample(SR);
  EWK_LowBin = EWK_Data.first;
  No_EWK_BinCount = EWK_Data.second;

  //The following are two ways to normalize the sample.
  //The first derives the scale factor for the EWK subtracted sample.
  //The second derives it for the non EWK subtracted sample.
  //The first is only there for testing purposes (it's how Vince does it).
  //double EWK_Norm = bin_count[0]/EWK_LowBin;
  //double EWK_Norm = bin_count[0]/No_EWK_BinCount[0];
  double EWK_Norm = normalization/No_EWK_BinCount[norm_bin];

  cout<<"Normalizing in bin"<<norm_bin<<"Normalization Target: "<<normalization<<" Num in EWK Sub Sample: "<<bin_count[norm_bin]<<" Num in NonEWKSub Sample: "<<No_EWK_BinCount[norm_bin]<<endl;
  cout<<"Derived scale factor "<<EWK_Norm<<" for Non EWK sample"<<endl;

  double err_bin; //error in bin
  double ewk_err; //EWK error in bin

  for (int i=0; i<stat_err.size(); i++){

    ewk_err = abs(bin_count[i] - EWK_Norm*No_EWK_BinCount[i]);

    cout<<"TRACE| Bin "<<i<<" ";
    err_bin = stat_err[i]*stat_err[i]; //Statistical Error
    cout<<"Stat Error: "<< stat_err[i];
    err_bin += bin_count[i]*bin_count[i]*MC_Closure_Error[i]*MC_Closure_Error[i]; //Closure Error
    cout<<" Closure Error: "<<bin_count[i]*MC_Closure_Error[i];
    err_bin += normalization*bin_count[i]*normalization*bin_count[i]; //Normalization of Zjets
    cout<<" Normalization: "<<normalization*bin_count[i];
    cout<<" Stat+Norm+Closure "<<sqrt(err_bin);
    err_bin += ewk_err*ewk_err; //EWK Subtraction
    cout<<" EWK Subtraction: "<<ewk_err;
    cout<<" Stat+Norm+Closure+EWK: "<<sqrt(err_bin)<<endl;

    output_errors.push_back(sqrt(err_bin));
  }

  return output_errors;
}

pair<vector<double>,vector<double>> getFSError(vector<double> bin_count, double RSFOF){
  double RSFOF_unc = 0.026; //ICHEP 2016
  double kappa_unc = 0.03; //ICHEP 2016

  vector<double> error_up;
  vector<double> error_dn;

  double bin_up, bin_dn;
  for (int i = 0; i<bin_count.size(); i++){
    RooHistError::instance().getPoissonInterval(bin_count[i], bin_dn, bin_up);

    cout<<"bin count "<<bin_count[i]<<" Error_up "<<bin_up<<" Error_dn "<<bin_dn<<endl; 
    bin_up = RSFOF*RSFOF*(bin_up - bin_count[i])*(bin_up - bin_count[i]) + RSFOF_unc*RSFOF_unc*bin_count[i]*bin_count[i] + kappa_unc*kappa_unc*bin_count[i]*bin_count[i];
    bin_dn = RSFOF*RSFOF*(bin_count[i] - bin_dn)*(bin_count[i] - bin_dn) + RSFOF_unc*RSFOF_unc*bin_count[i]*bin_count[i] + kappa_unc*kappa_unc*bin_count[i]*bin_count[i];

    error_up.push_back(sqrt(bin_up));
    error_dn.push_back(sqrt(bin_dn));
  }

  return make_pair(error_up, error_dn);
}

vector<double> getRareSamplesError(vector<double> stat_err, vector<double> bin_count){
  double catch_all_error = .5; //This is the amount of error to assign as a catchall number for the MC prediction. That is, we expect MC to correctly predict the answer to within this percentage.
  double err_bin;

  vector<double> error;

  for(int i=0; i<stat_err.size(); i++){
    err_bin = 0;
    err_bin += catch_all_error*catch_all_error*bin_count[i]*bin_count[i];
    err_bin += stat_err[i]*stat_err[i];

    error.push_back(sqrt(err_bin));
  }

  return error;
}

void printErrors(vector<double> temp_err, vector<double> rare_err, pair<vector<double>,vector<double>> fs_err, vector<double> bin_low){
  cout<<"Sample ";
  for (int i = 0; i<temp_err.size(); i++){
    cout<<bin_low[i]<<"-"<<bin_low[i+1]<<" ";
  }
  cout<<endl;
  cout<<"Template ";
  for (int i = 0; i<temp_err.size(); i++){
    cout<<"+/-"<<temp_err[i]<<" ";
  }
  cout<<endl;
  cout<<"FS ";
  for (int i = 0; i<fs_err.first.size(); i++){
    cout<<"+"<<fs_err.first[i]<<"-"<<fs_err.second[i]<<" ";
  }
  cout<<endl;
  cout<<"Rares ";
  for (int i = 0; i<rare_err.size(); i++){
    cout<<"+/-"<<rare_err[i]<<" ";
  }
  cout<<endl;
  cout<<"Sum ";
  for (int i = 0; i<temp_err.size(); i++){
    cout<<"+"<<temp_err[i]+rare_err[i]+fs_err.first[i]<<"-"<<temp_err[i]+rare_err[i]+fs_err.second[i]<<" ";
  }
  cout<<endl;
}

void printCounts(vector<double> temp_count, vector<double> temp_err, vector<double> rare_count, vector<double> rare_err, vector<double> fs_count, pair<vector<double>,vector<double>> fs_err, vector<pair<double,double>> bin_low, vector<double> data_count, double RSFOF){
  cout<<"STATTABLE: Sample ";
  for (int i = 0; i<temp_err.size(); i++){
    cout<<bin_low[i].first<<"-"<<bin_low[i].second<<" ";
  }
  cout<<endl;
  cout<<"STATTABLE: Template ";
  for (int i = 0; i<temp_err.size(); i++){
    if (temp_count[i] < 0){
      temp_count[i] = 0;
    }
    cout<<temp_count[i]<<"+/-"<<temp_err[i]<<" ";
  }
  cout<<endl;
  cout<<"STATTABLE: FS ";
  for (int i = 0; i<fs_err.first.size(); i++){
    cout<<RSFOF*fs_count[i]<<"+"<<fs_err.first[i]<<"-"<<fs_err.second[i]<<" ";
  }
  cout<<endl;
    cout<<"STATTABLE: Rares ";
  for (int i = 0; i<rare_err.size(); i++){
    cout<<rare_count[i]<<"+/-"<<rare_err[i]<<" ";
  }
  cout<<endl;
  cout<<"STATTABLE: Sum ";
  for (int i = 0; i<temp_err.size(); i++){
    cout<<temp_count[i]+RSFOF*fs_count[i]+rare_count[i]<<"+"<<sqrt(temp_err[i]*temp_err[i]+rare_err[i]*rare_err[i]+fs_err.first[i]*fs_err.first[i])<<"-"<<sqrt(temp_err[i]*temp_err[i]+rare_err[i]*rare_err[i]+fs_err.second[i]*fs_err.second[i])<<" ";
  }
  cout<<endl;
  cout<<"STATTABLE: Data ";
  for (int i = 0; i<temp_err.size(); i++){
    cout<<data_count[i]<<" ";
  }
  cout<<endl;
}

void printLatexCounts(vector<double> temp_count, vector<double> temp_err, vector<double> rare_count, vector<double> rare_err, vector<double> fs_count, pair<vector<double>,vector<double>> fs_err, vector<pair<double,double>> bin_low, vector<double> data_count, double RSFOF){
  
  cout<<fixed;
  cout<<setprecision(1);

  cout<<"LATEXTABLE: \\begin{tabular} {";
  for (int i = 0; i < (int) temp_count.size(); i++){
    cout<<"l | "; 
  }
  cout<<"l }"<<endl;
  
  cout<<"LATEXTABLE: Sample ";
  for (int i = 0; i<temp_err.size(); i++){
    cout<<" & "<<bin_low[i].first<<"-"<<bin_low[i].second;
  }
  cout<<" \\\\ \\hline "<<endl;
  cout<<"LATEXTABLE: Template ";
  for (int i = 0; i<temp_err.size(); i++){
    cout<<" & "<<temp_count[i]<<"$\\pm$"<<temp_err[i];
  }
  cout<<" \\\\" <<endl;
  cout<<"LATEXTABLE: FS ";
  for (int i = 0; i<fs_err.first.size(); i++){
    cout<<" & "<<"$"<<RSFOF*fs_count[i]<<"^{"<<fs_err.first[i]<<"}_{"<<fs_err.second[i]<<"}$ ";
  }
  cout<<" \\\\" <<endl;
    cout<<"LATEXTABLE: Rares ";
  for (int i = 0; i<rare_err.size(); i++){
    cout<<" & "<<rare_count[i]<<"$\\pm$"<<rare_err[i];
  }
  cout<<" \\\\ \\hline" <<endl;
  cout<<"LATEXTABLE: Sum ";
  for (int i = 0; i<temp_err.size(); i++){
    cout<<" & "<<"$"<<temp_count[i]+RSFOF*fs_count[i]+rare_count[i]<<"^{"<<sqrt(temp_err[i]*temp_err[i]+rare_err[i]*rare_err[i]+fs_err.first[i]*fs_err.first[i])<<"}_{"<<sqrt(temp_err[i]*temp_err[i]+rare_err[i]*rare_err[i]+fs_err.second[i]*fs_err.second[i])<<"}$ ";
  }
  cout<<"\\\\ \\hline"<<endl;
  cout<<"LATEXTABLE: Data ";
  for (int i = 0; i<temp_err.size(); i++){
    cout<<" & "<<data_count[i];
  }
  cout<<endl;
  cout<<"LATEXTABLE: \\end{tabular}"<<endl;
}

void computeErrors(){
  vector<double> bin_low = {0,50,100,150,225,6001};
  
  vector<double> temp_stat_err = {100,10,5,2,1};
  vector<double> temp_bin_count = {6947.05,1634.16,90.83,14.22,8};

  vector<double> FS_bin_count = {35.7,85.6,61.7,34.7,26};

  vector<double> rare_stat_err = {5,2,1,.2,.01};
  vector<double> rare_bin_count = {12.2,18.3,9,7.9,8.9};

  vector<double> temp_err = getMetTemplatesError(temp_stat_err, temp_bin_count, sqrt(6995), 1, "ATLAS");
  pair<vector<double>,vector<double>> FS_err = getFSError(FS_bin_count, 1.087);
  vector<double> rare_err = getRareSamplesError(rare_stat_err, rare_bin_count);
  cout<<"====================================\n\n\n";
  printErrors(temp_err, rare_err, FS_err, bin_low);
}

#endif
