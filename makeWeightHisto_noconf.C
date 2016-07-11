#include <iostream>
#include <stdexcept>

#include <TFile.h>
#include <TH1D.h>
#include <TString.h>


using namespace std;

void makeWeightHisto_noconf(TString output_location, TString infile1, TString infile2, TString hist1, TString hist2, TString output_hist_name)
{

  cout<<"\n\n";
  cout<<"Making new reweight histogram in file "<<output_location<<". Using "<<hist1<<" in "<<infile1<<" as target to make weights for "<<hist2<<" in "<<infile2<<"."<<endl;
  cout<<"Histogram will be named "<<output_hist_name<<endl;

  TFile * f_primary = TFile::Open(infile1 , "READ"); //typically location to data hist
  TFile * f_secondary = TFile::Open(infile2, "READ"); //typically location to zjets hist
  cout<<"Found input files for reweighting"<<endl;
  TH1D * h_primary;
  TH1D * h_secondary;
  TH1D * h_primary_scaled;
  TH1D * h_secondary_scaled;

  if (f_primary && f_secondary) {
    h_primary = (TH1D*)f_primary->Get(hist1)->Clone("h_"+hist1);
    h_secondary = (TH1D*)f_secondary->Get(hist2)->Clone("h_"+hist2);
  }
  else{
    cout<<"Error, could not open baseline files, please check they exist where specified and try again"<<endl;
    exit(1);
  }

  cout<<"Retrived Histograms"<<endl;

  TH1D * h_ratio_unscaled = (TH1D*) h_primary->Clone(hist2+"_ratio_unscaled");
  h_ratio_unscaled->Divide(h_secondary); 

  h_primary_scaled=(TH1D*) h_primary->Clone(hist1+"_scaled");
  h_secondary_scaled=(TH1D*) h_secondary->Clone(hist2+"_scaled");
  h_secondary_scaled->Scale(1./h_secondary->GetSumOfWeights());
  h_primary_scaled->Scale(1./h_primary->GetSumOfWeights());

  TH1D * h_ratio = (TH1D*) h_primary_scaled->Clone(output_hist_name);
  h_ratio->Divide(h_secondary_scaled);

  TFile * file = TFile::Open(output_location,"UPDATE");
  file->cd();
  h_ratio->Write();
  h_ratio_unscaled->Write();
  h_primary->Write();
  h_secondary->Write();
  h_primary_scaled->Write();
  h_secondary_scaled->Write();
  file->Close();

  cout<<"Reweight histogram succesfully made at "<<output_location<<endl;
  
  return ;
}
