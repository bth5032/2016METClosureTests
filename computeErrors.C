#include <vector>
#include <utility>
#include "TString.h"
#include "RooHistError.h"

using namespace std;

double err_mult(double A, double B, double errA, double errB) {
  /* A = data yeild in norm bin
  B = template yield in norm bin
  errA/errB = statistical error in those bins*/
  return sqrt((A/B)*(A/B)*(pow(errA/A,2) + pow(errB/B,2)));
}

vector<double> getMetTemplatesError(vector<double> stat_err, vector<double> bin_count, double normalization, TString SR){

  vector<double> output_errors;

  double template_low_bin_count = bin_count[0];
  double template_low_bin_error = stat_err[0];

  normalization = err_mult(normalization, bin_count[0], sqrt(normalization), stat_err[0]);

  //=========
  // Input EWK and Closure Errors
  //=========

  vector<double> No_EWK_BinCount;
  double EWK_LowBin;
  vector<double> MC_Closure_Error;

  if (SR == "A_btag"){
    MC_Closure_Error.push_back(.02);
    MC_Closure_Error.push_back(.07);
    MC_Closure_Error.push_back(.2);
    MC_Closure_Error.push_back(.2);
    MC_Closure_Error.push_back(.4);
    MC_Closure_Error.push_back(.4);

    EWK_LowBin = 42700.4034049053;

    No_EWK_BinCount.push_back(42735.6462299816);
    No_EWK_BinCount.push_back(13040.3035059966);
    No_EWK_BinCount.push_back(961.179201767482);
    No_EWK_BinCount.push_back(275.403015374794);
    No_EWK_BinCount.push_back(29.7438917754633);
    No_EWK_BinCount.push_back(45.7241551041078);
  }
  else if(SR == "A_bveto"){
    MC_Closure_Error.push_back(.02);
    MC_Closure_Error.push_back(.1);
    MC_Closure_Error.push_back(.2);
    MC_Closure_Error.push_back(.2);
    MC_Closure_Error.push_back(.2);
    MC_Closure_Error.push_back(.25);

    EWK_LowBin = 267769.503918135;

    No_EWK_BinCount.push_back(267773.901753913);
    No_EWK_BinCount.push_back(74647.7423379096);
    No_EWK_BinCount.push_back(5435.29060061884);
    No_EWK_BinCount.push_back(968.266188748323);
    No_EWK_BinCount.push_back(654.697936344355);
    No_EWK_BinCount.push_back(115.101182467609);
  }
  else if (SR == "B_btag"){
    MC_Closure_Error.push_back(.01);
    MC_Closure_Error.push_back(.1);
    MC_Closure_Error.push_back(.2);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.3);
    MC_Closure_Error.push_back(.3);
    
    EWK_LowBin = 164153.245918587;
    
    No_EWK_BinCount.push_back(164874.440945203);
    No_EWK_BinCount.push_back(33269.4533825993);
    No_EWK_BinCount.push_back(2271.01231960854);
    No_EWK_BinCount.push_back(452.51087730012);
    No_EWK_BinCount.push_back(141.150849300329);
    No_EWK_BinCount.push_back(19.431625988652);
  }
  else if(SR == "B_bveto"){
    MC_Closure_Error.push_back(.01);
    MC_Closure_Error.push_back(.06);
    MC_Closure_Error.push_back(.1);
    MC_Closure_Error.push_back(.1);
    MC_Closure_Error.push_back(.35);
    MC_Closure_Error.push_back(.35);

    EWK_LowBin = 600575.996079145;

    No_EWK_BinCount.push_back(600713.289517667);
    No_EWK_BinCount.push_back(110603.531253203);
    No_EWK_BinCount.push_back(5438.10365165714);
    No_EWK_BinCount.push_back(874.468338071326);
    No_EWK_BinCount.push_back(132.357553895084);
    No_EWK_BinCount.push_back(145.249685506808);
  }
  else if(SR == "ATLAS"){
    MC_Closure_Error.push_back(.02);
    MC_Closure_Error.push_back(.1);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.35);
    MC_Closure_Error.push_back(.4);

    EWK_LowBin = 162062.45358702;

    No_EWK_BinCount.push_back(162252.754145217);
    No_EWK_BinCount.push_back(38349.076835869);
    No_EWK_BinCount.push_back(2206.38123230649);
    No_EWK_BinCount.push_back(387.413146916147);
    No_EWK_BinCount.push_back(221.374639691808);
  }
  else if(SR == "TChiWZ"){
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);

    EWK_LowBin = 267770;

    No_EWK_BinCount.push_back(429.72);
    No_EWK_BinCount.push_back(746.685);
    No_EWK_BinCount.push_back(401.665);
    No_EWK_BinCount.push_back(209.264);
    No_EWK_BinCount.push_back(62.5741);
    No_EWK_BinCount.push_back(65.9222);
  }
  else if(SR == "EdgeZ"){
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);
    MC_Closure_Error.push_back(.25);

    EWK_LowBin = 31076507.4929719;

    No_EWK_BinCount.push_back(31080256.8594184);
    No_EWK_BinCount.push_back(2967944.23267151);
    No_EWK_BinCount.push_back(62709.0494743355);
    No_EWK_BinCount.push_back(9635.22882731112);
    No_EWK_BinCount.push_back(2689.44657056686);
    No_EWK_BinCount.push_back(1058.18303606238);
  }

  //double EWK_Norm = bin_count[0]/EWK_LowBin;
  double EWK_Norm = bin_count[0]/No_EWK_BinCount[0];

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

  vector<double> error_up;
  vector<double> error_dn;

  double bin_up, bin_dn;
  for (int i = 0; i<bin_count.size(); i++){
    RooHistError::instance().getPoissonInterval(bin_count[i], bin_dn, bin_up);

    cout<<"bin count "<<bin_count[i]<<" Error_up "<<bin_up<<" Error_dn "<<bin_dn<<endl; 
    bin_up = RSFOF*RSFOF*(bin_up - bin_count[i])*(bin_up - bin_count[i]) + RSFOF_unc*RSFOF_unc*bin_count[i]*bin_count[i];
    bin_dn = RSFOF*RSFOF*(bin_count[i] - bin_dn)*(bin_count[i] - bin_dn) + RSFOF_unc*RSFOF_unc*bin_count[i]*bin_count[i];

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

void printCounts(vector<double> temp_count, vector<double> temp_err, vector<double> rare_count, vector<double> rare_err, vector<double> fs_count, pair<vector<double>,vector<double>> fs_err, vector<pair<double,double>> bin_low, vector<double> data_count, double RSFOF){
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
  cout<<"LATEXTABLE: \\begin{tabular} {l | l | l | l | l | l | l }"<<endl;
  cout<<"LATEXTABLE: Sample ";
  for (int i = 0; i<temp_err.size(); i++){
    cout<<bin_low[i].first<<"-"<<bin_low[i].second<<" & ";
  }
  cout<<" \\\\ \\hline "<<endl;
  cout<<"LATEXTABLE: Template ";
  for (int i = 0; i<temp_err.size(); i++){
    cout<<temp_count[i]<<"$\\pm$"<<temp_err[i]<<" & ";
  }
  cout<<" \\\\" <<endl;
  cout<<"LATEXTABLE: FS ";
  for (int i = 0; i<fs_err.first.size(); i++){
    cout<<"$"<<RSFOF*fs_count[i]<<"^{"<<fs_err.first[i]<<"}_{"<<fs_err.second[i]<<"} ";
  }
  cout<<" \\\\" <<endl;
    cout<<"LATEXTABLE: Rares ";
  for (int i = 0; i<rare_err.size(); i++){
    cout<<rare_count[i]<<"$\\pm$"<<rare_err[i]<<" & ";
  }
  cout<<" \\\\" <<endl;
  cout<<"LATEXTABLE: Sum ";
  for (int i = 0; i<temp_err.size(); i++){
    cout<<"$"<<temp_count[i]+RSFOF*fs_count[i]+rare_count[i]<<"^{"<<sqrt(temp_err[i]*temp_err[i]+rare_err[i]*rare_err[i]+fs_err.first[i]*fs_err.first[i])<<"}_{"<<sqrt(temp_err[i]*temp_err[i]+rare_err[i]*rare_err[i]+fs_err.second[i]*fs_err.second[i])<<"}$ & ";
  }
  cout<<"\\\\ \\hline"<<endl;
  cout<<"LATEXTABLE: Data ";
  for (int i = 0; i<temp_err.size(); i++){
    cout<<data_count[i]<<" & ";
  }
  cout<<" \\\\ "<<endl;
  cout<<"LATEXTABLE: \\end{tabular}"<<endl;
}

void computeErrors(){
  vector<double> bin_low = {0,50,100,150,225,6001};
  
  vector<double> temp_stat_err = {100,10,5,2,1};
  vector<double> temp_bin_count = {6947.05,1634.16,90.83,14.22,8};

  vector<double> FS_bin_count = {35.7,85.6,61.7,34.7,26};

  vector<double> rare_stat_err = {5,2,1,.2,.01};
  vector<double> rare_bin_count = {12.2,18.3,9,7.9,8.9};

  vector<double> temp_err = getMetTemplatesError(temp_stat_err, temp_bin_count, sqrt(6995),"ATLAS");
  pair<vector<double>,vector<double>> FS_err = getFSError(FS_bin_count, 1.087);
  vector<double> rare_err = getRareSamplesError(rare_stat_err, rare_bin_count);
  cout<<"====================================\n\n\n";
  printErrors(temp_err, rare_err, FS_err, bin_low);
}
