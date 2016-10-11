#include "TString.h"
#include "TH1.h"
#include "TFile.h"
#include <vector>
#include <utility>
#include <iostream>
#include <iomanip> 

using namespace std;

vector<TString> getFileLocation(TString sample_name){
  vector<TString> fnames;
  if (sample_name=="A_Btag"){
    fnames.push_back("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data/ct_G_Reweight_A_btag.root");
    fnames.push_back("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data_NoEWKSub/ct_G_Reweight_A_btag.root");
  }
  else if (sample_name=="A_Bveto"){
    fnames.push_back("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data/ct_G_Reweight_A_bveto.root");
    fnames.push_back("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data_NoEWKSub/ct_G_Reweight_A_bveto.root");
  }
  else if (sample_name=="B_Btag"){
    fnames.push_back("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data/ct_G_Reweight_B_btag.root");
    fnames.push_back("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data_NoEWKSub/ct_G_Reweight_B_btag.root");
  }
  else if (sample_name=="B_Bveto"){
    fnames.push_back("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data/ct_G_Reweight_B_bveto.root");
    fnames.push_back("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data_NoEWKSub/ct_G_Reweight_B_bveto.root");
  }
  else if (sample_name=="EdgeZ"){
    fnames.push_back("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data/ct_G_Reweight_EdgeZ.root");
    fnames.push_back("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data_NoEWKSub/ct_G_Reweight_EdgeZ.root");
  }
  else if (sample_name=="ATLAS"){
    fnames.push_back("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data/ct_G_Reweight_ATLAS.root");
    fnames.push_back("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data_NoEWKSub/ct_G_Reweight_ATLAS.root");
  }
  else if (sample_name=="EWKHiggs"){
    fnames.push_back("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data/ct_G_Reweight_EWK_Higgs.root");
    fnames.push_back("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data_NoEWKSub/ct_G_Reweight_EWK_Higgs.root");
  }

  return fnames;
}

pair<double, vector<double>> getEWKNumsForSample(TString sample_name){
  cout<<setprecision(15);
  vector<TString> fnames = getFileLocation(sample_name);

  TFile* sub_file = TFile::Open(fnames[0]);
  TFile* no_sub_file = TFile::Open(fnames[1]);

  TH1D* sub_hist = (TH1D*) ((TH1D*) sub_file->Get("gjets_type1MET"));
  TH1D* no_sub_hist = (TH1D*) ((TH1D*) no_sub_file->Get("gjets_type1MET"));   

  vector<double> bins, noSubNums;
  double lowbin_withEwkSub;

  if(sample_name == "ATLAS"){
    bins.push_back(0);
    bins.push_back(50);
    bins.push_back(100);
    bins.push_back(150);
    bins.push_back(225);
    bins.push_back(6001);
  }
  else{
    bins.push_back(0);
    bins.push_back(50);
    bins.push_back(100);
    bins.push_back(150);
    bins.push_back(225);
    bins.push_back(300);
    bins.push_back(6001);
  }

  cout<<"Deriving EWK Subtraction numbers:"<<endl;
  for (std::vector<double>::iterator i = bins.begin(); (i+1) != bins.end(); i++){
    count_in_sub = sub_hist->Integral(sub_hist->FindBin(*i), sub_hist->FindBin(*(i+1))-.05);
    count_in_no = no_sub_hist->Integral(no_sub_hist->FindBin(*i), no_sub_hist->FindBin(*(i+1))-.05);
    
    cout<<"bin: "<<*i<<"-"<<*(i+1)<<" sub: "<<count_in_sub<<" no sub: "<<count_in_no<<" diff: "<<count_in_no - count_in_sub<<endl;

    if (i == bins.begin()){
      lowbin_withEwkSub = count_in_sub;
    }
      noSubNums.push_back(count_in_no);
  }

  return make_pair(lowbin_withEwkSub, noSubNums);
}

/*void printDiff(TString a, TString b, TString c){
  cout<<setprecision(15);

  TFile* norm_file = TFile::Open(c);
  TFile* sub_file = TFile::Open(a);
  TFile* no_sub_file = TFile::Open(b); 

  TH1D* sub_hist = (TH1D*) ((TH1D*) sub_file->Get("gjets_type1MET"));
  TH1D* no_sub_hist = (TH1D*) ((TH1D*) no_sub_file->Get("gjets_type1MET"));
  TH1D* norm_hist = (TH1D*) ((TH1D*) norm_file->Get("zjets_type1MET"));

  double count_in_sub, count_in_no, norm_count;

  norm_count = norm_hist->Integral(norm_hist->FindBin(0), norm_hist->FindBin(49.9));
  double scale_sub = norm_count/(sub_hist->Integral(sub_hist->FindBin(0), sub_hist->FindBin(49.9)));
  double scale_no_sub = norm_count/(no_sub_hist->Integral(no_sub_hist->FindBin(0), no_sub_hist->FindBin(49.9)));

  //sub_hist->Scale(scale_sub);
  //no_sub_hist->Scale(scale_no_sub);
  //no_sub_hist->Scale(scale_sub);


  vector<double> bins;

  if(a.Contains("ATLAS")){
    bins.push_back(0);
    bins.push_back(50);
    bins.push_back(100);
    bins.push_back(150);
    bins.push_back(225);
    bins.push_back(6001);
  }
  else{
    bins.push_back(0);
    bins.push_back(50);
    bins.push_back(100);
    bins.push_back(150);
    bins.push_back(225);
    bins.push_back(300);
    bins.push_back(6001);
  }
  cout<<a<<endl;
  for (std::vector<double>::iterator i = bins.begin(); (i+1) != bins.end(); i++){
    count_in_sub = sub_hist->Integral(sub_hist->FindBin(*i), sub_hist->FindBin(*(i+1))-.05);
    count_in_no = no_sub_hist->Integral(no_sub_hist->FindBin(*i), no_sub_hist->FindBin(*(i+1))-.05);
    cout<<"bin: "<<*i<<"-"<<*(i+1)<<" sub: "<<count_in_sub<<" no sub: "<<count_in_no<<" diff: "<<count_in_no - count_in_sub<<endl;
  }
}*/

void getEWKErrorNums(){
  vector<TString> SR;

  SR.push_back("A_Btag");
  SR.push_back("A_Bveto");
  SR.push_back("B_Btag");
  SR.push_back("B_Bveto");
  SR.push_back("ATLAS");
  SR.push_back("EdgeZ");

  for(vector<TString>::iterator it = SR.begin(); it != SR.end(); ++it) {
    cout<<"==========="<<endl;
    getEWKNumsForSample(*it);
 }
}