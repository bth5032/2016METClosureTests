#include <vector>
#include <utility>
#include "TString.h"
#include "RooHistError.h"

using namespace std;

vector<double> getMetTemplatesError(vector<double> stat_err, vector<double> bin_count, double normalization_uncertainty, TString SR){

  vector<double> output_errors;

  //=========
  // Input EWK and Closure Errors
  //=========

  vector<double> EWK_Error;
  vector<double> MC_Closure_Error;

  if (SR == "A_btag"){
    MC_Closure_Error.push_back(.02);
    MC_Closure_Error.push_back(.07);
    MC_Closure_Error.push_back(.2);
    MC_Closure_Error.push_back(.2);
    MC_Closure_Error.push_back(.4);
    MC_Closure_Error.push_back(.4);

    EWK_Error.push_back(35.2428);
    EWK_Error.push_back(73.5586);
    EWK_Error.push_back(33.9857);
    EWK_Error.push_back(9.37907);
    EWK_Error.push_back(7.98075);
    EWK_Error.push_back(18.0243);
  }
  else if(SR == "A_bveto"){
    MC_Closure_Error.push_back(.02);
    MC_Closure_Error.push_back(.1);
    MC_Closure_Error.push_back(.2);
    MC_Closure_Error.push_back(.2);
    MC_Closure_Error.push_back(.2);
    MC_Closure_Error.push_back(.25);

    EWK_Error.push_back(4.39784);
    EWK_Error.push_back(244.131);
    EWK_Error.push_back(114.411);
    EWK_Error.push_back(96.1903);
    EWK_Error.push_back(47.231);
    EWK_Error.push_back(32.8677);
  }
  else if (SR == "B_btag"){
    MC_Closure_Error.push_back(.01);
    MC_Closure_Error.push_back(.1);
    MC_Closure_Error.push_back(.2);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.3);
    MC_Closure_Error.push_back(.3);

    EWK_Error.push_back(721.195);
    EWK_Error.push_back(958.259);
    EWK_Error.push_back(388.289);
    EWK_Error.push_back(170.377);
    EWK_Error.push_back(72.4261);
    EWK_Error.push_back(54.3328);
  }
  else if(SR == "B_bveto"){
    MC_Closure_Error.push_back(.01);
    MC_Closure_Error.push_back(.06);
    MC_Closure_Error.push_back(.1);
    MC_Closure_Error.push_back(.1);
    MC_Closure_Error.push_back(.35);
    MC_Closure_Error.push_back(.35);

    EWK_Error.push_back(137.293);
    EWK_Error.push_back(271.125);
    EWK_Error.push_back(212.483);
    EWK_Error.push_back(99.2568);
    EWK_Error.push_back(35.4253);
    EWK_Error.push_back(10.2263);
  }
  else if(SR == "ATLAS"){
    MC_Closure_Error.push_back(.02);
    MC_Closure_Error.push_back(.1);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.35);
    MC_Closure_Error.push_back(.4);

    EWK_Error.push_back(190.301);
    EWK_Error.push_back(248.185);
    EWK_Error.push_back(141.267);
    EWK_Error.push_back(90.1497);
    EWK_Error.push_back(82.3914);
  }
  else if(SR == "TChiWZ"){
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);

    EWK_Error.push_back(429.72);
    EWK_Error.push_back(746.685);
    EWK_Error.push_back(401.665);
    EWK_Error.push_back(209.264);
    EWK_Error.push_back(62.5741);
    EWK_Error.push_back(65.9222);
  }
  else if(SR == "EdgeZ"){
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);

    EWK_Error.push_back(3749.37);
    EWK_Error.push_back(10119.1);
    EWK_Error.push_back(4455.25);
    EWK_Error.push_back(2006.42);
    EWK_Error.push_back(892.348);
    EWK_Error.push_back(451.202);
  }

  double err_bin; //error in bin

  for (int i=0; i<stat_err.size(); i++){
    err_bin = stat_err[i]*stat_err[i]; //Statistical Error
    err_bin += bin_count[i]*bin_count[i]*MC_Closure_Error[i]*MC_Closure_Error[i]; //Closure Error
    err_bin += normalization_uncertainty*normalization_uncertainty; //Normalization of Zjets
    err_bin += EWK_Error[i]*EWK_Error[i]; //Normalization of Zjets

    output_errors.push_back(sqrt(err_bin));
  }

  return output_errors;
}

pair<vector<double>,vector<double>> getFSError(vector<double> bin_count){
  double RSFOF_unc = 0.026; //ICHEP 2016

  vector<double> error_up;
  vector<double> error_dn;

  double bin_up, bin_dn;
  for (int i = 0; i<bin_count.size(); i++){
    RooHistError::instance().getPoissonInterval(bin_count[i], bin_up, bin_dn);

    cout<<"bin count "<<bin_count[i]<<" Error_up "<<bin_up<<" Error_dn "<<bin_dn<<endl; 
    bin_up = (bin_up - bin_count[i])*(bin_up - bin_count[i]) + RSFOF_unc*RSFOF_unc*bin_count[i]*bin_count[i];
    bin_dn = (bin_count[i] - bin_dn)*(bin_count[i] - bin_dn) + RSFOF_unc*RSFOF_unc*bin_count[i]*bin_count[i];

    error_up.push_back(sqrt(bin_up));
    error_dn.push_back(sqrt(bin_dn));
  }

  return make_pair(error_up, error_dn);
}

vector<double> getRareSamplesError(vector<double> stat_err, vector<double> bin_count){
  double catch_all_error = .5; //This is the amount of error to assign as a catchall number for the MC prediction. That is, we expect MC to correctly predict the answer to within this percentage.
  double err_bin = 0;

  vector<double> error;

  for(int i=0; i<stat_err.size(); i++){
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

void printCounts(vector<double> temp_count, vector<double> temp_err, vector<double> rare_count, vector<double> rare_err, vector<double> fs_count, pair<vector<double>,vector<double>> fs_err, vector<pair<int,int>> bin_low, vector<double> data_count){
  cout<<"STATTABLE: Sample ";
  for (int i = 0; i<temp_err.size(); i++){
    cout<<bin_low[i].first<<"-"<<bin_low[i].second<<" ";
  }
  cout<<endl;
  cout<<"STATTABLE: Template ";
  for (int i = 0; i<temp_err.size(); i++){
    cout<<temp_count[i]<<"+/-"<<temp_err[i]<<" ";
  }
  cout<<endl;
  cout<<"STATTABLE: FS ";
  for (int i = 0; i<fs_err.first.size(); i++){
    cout<<fs_count[i]<<"+"<<fs_err.first[i]<<"-"<<fs_err.second[i]<<" ";
  }
  cout<<endl;
  cout<<"STATTABLE: Rares ";
  for (int i = 0; i<rare_err.size(); i++){
    cout<<rare_count[i]<<"+/-"<<rare_err[i]<<" ";
  }
  cout<<endl;
  cout<<"STATTABLE: Sum ";
  for (int i = 0; i<temp_err.size(); i++){
    cout<<temp_count[i]+fs_count[i]+rare_count[i]<<"+"<<temp_err[i]+rare_err[i]+fs_err.first[i]<<"-"<<temp_err[i]+rare_err[i]+fs_err.second[i]<<" ";
  }
  cout<<endl;
  cout<<"STATTABLE: Data ";
  for (int i = 0; i<temp_err.size(); i++){
    cout<<data_count[i]<<" ";
  }
  cout<<endl;
}

void computeErrors(){
  vector<double> bin_low = {0,50,100,150,225,6001};
  
  vector<double> temp_stat_err = {100,10,5,2,1};
  vector<double> temp_bin_count = {6947.05,1634.16,90.83,14.22,8};

  vector<double> FS_bin_count = {35.7,85.6,61.7,34.7,26};

  vector<double> rare_stat_err = {5,2,1,.2,.01};
  vector<double> rare_bin_count = {12.2,18.3,9,7.9,8.9};

  vector<double> temp_err = getMetTemplatesError(temp_stat_err, temp_bin_count, sqrt(6995),"ATLAS");
  pair<vector<double>,vector<double>> FS_err = getFSError(FS_bin_count);
  vector<double> rare_err = getRareSamplesError(rare_stat_err, rare_bin_count);
  cout<<"====================================\n\n\n";
  printErrors(temp_err, rare_err, FS_err, bin_low);
}
