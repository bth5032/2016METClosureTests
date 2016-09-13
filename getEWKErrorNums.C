#include "TString.h"
#include "TH1.h"
#include "TFile.h"
#include <vector>
#include <utility>
#include <iostream>

using namespace std;

void printDiff(TString a, TString b){
  TFile* sub_file = TFile::Open(a);
  TFile* no_sub_file = TFile::Open(b); 

  TH1D* sub_hist = (TH1D*) ((TH1D*) sub_file->Get("gjets_type1MET"));
  TH1D* no_sub_hist = (TH1D*) ((TH1D*) no_sub_file->Get("gjets_type1MET"));

  double count_in_sub, count_in_no;

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

  for (std::vector<double>::iterator i = bins.begin(); i != bins.end(); i++){
    count_in_sub = sub_hist->Integral(sub_hist->FindBin(*i), sub_hist->FindBin(*(i+1))-.05);
    count_in_no = no_sub_hist->Integral(no_sub_hist->FindBin(*i), no_sub_hist->FindBin(*(i+1))-.05);
    cout<<"bin: "<<*i<<"-"<<*(i+1)<<" sub: "<<count_in_sub<<" no sub: "<<count_in_no<<" diff: "<<count_in_no - count_in_sub<<endl;
  }
}

void getEWKErrorNums(){
  vector<pair<TString, TString>> files;

  files.push_back(make_pair("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data/ct_G_Reweight_ATLAS.root", "/nfs-7/userdata/bobak/GJetsClosureTests2016/Data_NoEWKSub/ct_G_Reweight_ATLAS.root"));

  files.push_back(make_pair("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data/ct_G_Reweight_A_bveto.root", "/nfs-7/userdata/bobak/GJetsClosureTests2016/Data_NoEWKSub/ct_G_Reweight_A_bveto.root"));

  files.push_back(make_pair("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data/ct_G_Reweight_A_btag.root", "/nfs-7/userdata/bobak/GJetsClosureTests2016/Data_NoEWKSub/ct_G_Reweight_A_btag.root"));

  files.push_back(make_pair("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data/ct_G_Reweight_B_bveto.root", "/nfs-7/userdata/bobak/GJetsClosureTests2016/Data_NoEWKSub/ct_G_Reweight_B_bveto.root"));

  files.push_back(make_pair("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data/ct_G_Reweight_B_btag.root", "/nfs-7/userdata/bobak/GJetsClosureTests2016/Data_NoEWKSub/ct_G_Reweight_B_btag.root"));

  files.push_back(make_pair("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data/ct_G_Reweight_ATLAS.root", "/nfs-7/userdata/bobak/GJetsClosureTests2016/Data_NoEWKSub/ct_G_Reweight_EdgeZ.root"));

  files.push_back(make_pair("/nfs-7/userdata/bobak/GJetsClosureTests2016/Data/ct_G_Reweight_EWK_Higgs.root", "/nfs-7/userdata/bobak/GJetsClosureTests2016/Data_NoEWKSub/ct_G_Reweight_EWK_Higgs.root"));

  for(vector<pair<TString, TString>>::iterator it = files.begin(); it != files.end(); ++it) {
    printDiff(it->first, it->second);
    cout<<"==========="<<endl;
 }
}