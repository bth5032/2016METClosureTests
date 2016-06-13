#include <iostream>
#include <stdexcept>

#include <TFile.h>
#include <TH1F.h>

using namespace std;

void makeWeightHisto(TString output_dir, TString primary_histos, TString secondary_histos, TString hist_name, TString primary_name, TString secondary_name)
{

  if (! output_dir.EndsWith("/")){
    output_dir+="/";
  }

  TFile * f_primary = TFile::Open(primary_histos , "READ"); //typically location to data hist
  TFile * f_secondary = TFile::Open(secondary_histos, "READ"); //typically location to zjets hist
  TH1F * h_primary;
  TH1F * h_secondary;

  if (f_primary && f_secondary) {
    h_primary = (TH1F*)f_primary->Get(primary_name+"_"+hist_name)->Clone("h_"+primary_name);
    h_secondary = (TH1F*)f_secondary->Get(secondary_name+"_"+hist_name)->Clone("h_"+secondary_name);
  }
  else{
    cout<<"Error, could not open baseline files, please check they exist where specified and try again"<<endl;
    exit(1);
  }

  h_secondary->Scale(1./h_secondary->GetSumOfWeights());
  h_primary->Scale(1./h_primary->GetSumOfWeights());

  TH1F * h_ratio = (TH1F*) h_primary->Clone("h_"+hist_name+"_ratio");
  h_ratio->Divide(h_secondary);

  TFile * file = TFile::Open(output_dir+hist_name+"_ratio.root","RECREATE");
  file->cd();
  h_ratio->Write();
  h_primary->Write();
  h_secondary->Write();
  file->Close();

  cout<<"Reweight histogram succesfully made at "<<output_dir+hist_name+"_ratio.root"<<endl;
  
  return ;
}
