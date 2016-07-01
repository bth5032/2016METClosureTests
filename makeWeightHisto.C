#include <iostream>
#include <stdexcept>

#include <TFile.h>
#include <TH1D.h>

#include "ConfigParser.C"

using namespace std;

void makeWeightHisto(ConfigParser * conf)
{
  TString hist_name = conf->get("rwt_var");
  
  TString primary_name = conf->get("primary_name");
  TString secondary_name = conf->get("secondary_name");
  
  TString primary_histos = conf->get("histo_output_dir")+"ct_Z_Base_"+conf->get("signal_region")+".root";
  TString secondary_histos = conf->get("histo_output_dir")+"ct_G_Base_"+conf->get("signal_region")+".root";
  
  TString output_location = conf->get("histo_output_dir")+"ct_"+hist_name+"_"+conf->get("signal_region")+"_rwt.root";


  TFile * f_primary = TFile::Open(primary_histos , "READ"); //typically location to data hist
  TFile * f_secondary = TFile::Open(secondary_histos, "READ"); //typically location to zjets hist
  cout<<"Found files for reweighting"<<endl;
  TH1D * h_primary;
  TH1D * h_secondary;
  TH1D * h_primary_scaled;
  TH1D * h_secondary_scaled;

  if (f_primary && f_secondary) {
    h_primary = (TH1D*)f_primary->Get(primary_name+"_"+hist_name)->Clone("h_"+primary_name);
    h_secondary = (TH1D*)f_secondary->Get(secondary_name+"_"+hist_name)->Clone("h_"+secondary_name);
  }
  else{
    cout<<"Error, could not open baseline files, please check they exist where specified and try again"<<endl;
    exit(1);
  }

  TH1D * h_ratio_unscaled = (TH1D*) h_primary->Clone("h_"+hist_name+"_ratio_unscaled");
  h_ratio_unscaled->Divide(h_secondary); 

  h_primary_scaled=(TH1D*) h_primary->Clone("h_"+primary_name+"_scaled");
  h_secondary_scaled=(TH1D*) h_secondary->Clone("h_"+secondary_name+"_scaled");
  h_secondary_scaled->Scale(1./h_secondary->GetSumOfWeights());
  h_primary_scaled->Scale(1./h_primary->GetSumOfWeights());

  TH1D * h_ratio = (TH1D*) h_primary_scaled->Clone("h_"+hist_name+"_ratio");
  h_ratio->Divide(h_secondary_scaled);

  TFile * file = TFile::Open(output_location,"RECREATE");
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
