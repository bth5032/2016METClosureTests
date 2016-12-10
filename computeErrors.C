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

void printTemplatesDebug(const vector<double> &prediction, const vector<double> &prediction_err, const vector<double> &stat_err, const vector<double> &closure_err, const vector<double> &norm_err, const vector<double> &ewk_err, const vector<pair<double, double>> &bin_edge){
  /* Prints a latex table of the sources of error that go into the templates */ 
  cout<<fixed;
  cout<<setprecision(2);

  cout<<"TEMPLATEDEBUG: \\begin{tabular} {l | l | l | l | l | l}"<<endl;
  cout<<"TEMPLATEDEBUG: MET Bin & Prediction & Closure (ratio) & Normalization (ratio) & Statistical (ratio) & EWK Sub (ratio) \\\\ \\hline"<<endl;
  for (int i = 0; i<bin_edge.size(); i++){
    cout<<"TEMPLATEDEBUG: "<<(int) bin_edge[i].first<<"-"<<(int) bin_edge[i].second<<" & "<<prediction[i]<<" \pm "<<prediction_err[i]<<" & "<<closure_err[i]<<" ("<<closure_err[i]/prediction_err[i]<<") & "<<norm_err[i]<<" ("<<norm_err[i]/prediction_err[i]<<") & "<<stat_err[i]<<" ("<<stat_err[i]/prediction_err[i]<<") & "<<ewk_err[i]<<<<" ("<<ewk_err[i]/prediction_err[i]<<") \\\\";
    if (i == (int) bin_edge.size() -1 ){
      cout<<" \\hline";
    }
    cout<<endl;
  }
  cout<<"TEMPLATEDEBUG: \\end{tabular}"<<endl;
}

vector<double> getMetTemplatesError(const vector<double> &stat_err, const vector<double> &bin_count, double normalization, int norm_bin, const vector<pair<double, double>> &bin_edge, TString SR){
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
  else if(SR == "baseline"){
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.00);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
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
  double EWK_Norm = bin_count[norm_bin]/No_EWK_BinCount[norm_bin];

  cout<<"Normalizing in bin "<<norm_bin<<" Normalization Target/Num in EWK Sub Sample: "<<bin_count[norm_bin]<<" Num in NonEWKSub Sample: "<<No_EWK_BinCount[norm_bin]<<endl;
  cout<<"Derived scale factor "<<EWK_Norm<<" for Non EWK sample"<<endl;

  double err_bin; //error in bin

  vector<double> closure_err, norm_err, ewk_err;

  for (int i=0; i<stat_err.size(); i++){

    ewk_err.push_back(abs(bin_count[i] - EWK_Norm*No_EWK_BinCount[i]));

    cout<<"TRACE| Bin "<<i<<" ";
    err_bin = stat_err[i]*stat_err[i]; //Statistical Error
    cout<<"Stat Error: "<< stat_err[i];
    err_bin += bin_count[i]*bin_count[i]*MC_Closure_Error[i]*MC_Closure_Error[i]; //Closure Error
    closure_err.push_back(bin_count[i]*MC_Closure_Error[i]);
    cout<<" Closure Error: "<<closure_err[i];
    err_bin += normalization*bin_count[i]*normalization*bin_count[i]; //Normalization of Zjets
    norm_err.push_back(normalization*bin_count[i]);
    cout<<" Normalization: "<<norm_err[i];
    cout<<" Stat+Norm+Closure "<<sqrt(err_bin);
    err_bin += ewk_err[i]*ewk_err[i]; //EWK Subtraction
    cout<<" EWK Subtraction: "<<ewk_err[i];
    cout<<" Stat+Norm+Closure+EWK: "<<sqrt(err_bin)<<endl;

    output_errors.push_back(sqrt(err_bin));
  }

  printTemplatesDebug(bin_count, output_errors, stat_err, closure_err, norm_err, ewk_err, bin_edge);

  return output_errors;
}

pair<vector<double>,vector<double>> getFSError(const vector<double> &bin_count, double RSFOF){
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

vector<double> getRareSamplesError(const vector<double> &stat_err, const vector<double> &bin_count, double scale, double scale_unc){
  double err_bin;

  vector<double> error;

  //σ^2 = stat_err^2 + (scale*bin_count*.5)^2
  for(int i=0; i<stat_err.size(); i++){
    err_bin = 0;
    err_bin += scale*scale*scale_unc*scale_unc*bin_count[i]*bin_count[i];
    err_bin += stat_err[i]*stat_err[i];

    error.push_back(sqrt(err_bin));
  }

  return error;
}

void printErrors(const vector<double> temp_err, const vector<double> &rare_err, const pair<vector<double>, vector<double>> &fs_err, const vector<double> &bin_low){
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

void printCounts(const vector<double> temp_count, const vector<double> &temp_err, const vector<double> &rare_count, const vector<double> &rare_err, const vector<double> &fs_count, const pair<vector<double>,vector<double>> &fs_err, const vector<pair<double,double>> &bin_low, const vector<double> &data_count, double RSFOF /*Really just the scale factor*/){
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

void printLatexCounts(const vector<double> temp_count, const vector<double> &temp_err, const vector<double> &rare_count, const vector<double> &rare_err, const vector<double> &fs_count, const pair<vector<double>,vector<double>> &fs_err, const vector<pair<double,double>> &bin_low, const vector<double> &data_count, double RSFOF /*Really just the scale factor*/){
  
  cout<<fixed;
  cout<<setprecision(1);

  cout<<"LATEXTABLE: \\begin{tabular} {";
  for (int i = 0; i < (int) temp_count.size(); i++){
    cout<<"l | "; 
  }
  cout<<"l }"<<endl;
  
  cout<<"LATEXTABLE: Sample ";
  cout<<setprecision(0);
  for (int i = 0; i<temp_err.size(); i++){
    cout<<" & "<<(int) bin_low[i].first<<"-"<<(int) bin_low[i].second;
  }
  cout<<" \\\\ \\hline "<<endl;
  cout<<setprecision(1);
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
  vector<pair<double,double>> bin_edge;
  bin_edge.push_back(make_pair(0,50));
  bin_edge.push_back(make_pair(50,100));
  bin_edge.push_back(make_pair(100,150));
  bin_edge.push_back(make_pair(150,225));
  bin_edge.push_back(make_pair(225,6001));
  
  vector<double> temp_stat_err = {100,10,5,2,1};
  vector<double> temp_bin_count = {6947.05,1634.16,90.83,14.22,8};

  vector<double> FS_bin_count = {35.7,85.6,61.7,34.7,26};

  vector<double> rare_stat_err = {5,2,1,.2,.01};
  vector<double> rare_bin_count = {12.2,18.3,9,7.9,8.9};

  vector<double> temp_err = getMetTemplatesError(temp_stat_err, temp_bin_count, sqrt(6995), 1, bin_edge, "2j");
  pair<vector<double>,vector<double>> FS_err = getFSError(FS_bin_count, 1.087);
  vector<double> rare_err = getRareSamplesError(rare_stat_err, rare_bin_count, 1.5, .5);
  cout<<"====================================\n\n\n";
  printErrors(temp_err, rare_err, FS_err, bin_low);
}

#endif
