void FS_Mass_Window_Study(){

  TString output_dir="/home/users/bhashemi/public_html/ZMET2016_NovemberClean/FS_mass_window_studies/baseline_ratios/";

  vector<TFile*> on_files;
  vector<TFile*> above_files;
  vector<TFile*> below_files;

  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_onZ/Z_Base.root", "read"));
  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_onZ/TT_Base.root", "read"));
  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_onZ/VVV.root", "read"));
  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_onZ/ttv.root", "read"));
  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_onZ/wz.root", "read"));
  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_onZ/zz.root", "read"));

  cout<<__LINE__<<endl;

  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_aboveZ/Z_Base.root", "read"));
  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_aboveZ/TT_Base.root", "read"));
  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_aboveZ/VVV.root", "read"));
  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_aboveZ/ttv.root", "read"));
  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_aboveZ/wz.root", "read"));
  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_aboveZ/zz.root", "read"));

  cout<<__LINE__<<endl;

  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_belowZ/Z_Base.root", "read"));
  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_belowZ/TT_Base.root", "read"));
  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_belowZ/VVV.root", "read"));
  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_belowZ/ttv.root", "read"));
  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_belowZ/wz.root", "read"));
  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_belowZ/zz.root", "read"));


  cout<<__LINE__<<endl;

// ==================
//  Define Histograms
// ==================
  TH1D *onz_met = (TH1D*) ((TH1D*) on_files[0]->Get("type1MET"))->Clone("onz_met");
  cout<<__LINE__<<endl;
  TH1D *offz_met = (TH1D*) ((TH1D*) above_files[0]->Get("type1MET"))->Clone("onz_met");
  cout<<__LINE__<<endl;
  offz_met->Add((TH1D*) ((TH1D*) below_files[0]->Get("type1MET")));
  cout<<__LINE__<<endl;

// ==================
//  Fill Histograms
// ==================
  cout<<__LINE__<<endl;
  for (int i = 1; i < (int) on_files.size(); i++){
    onz_met->Add((TH1D*) ((TH1D*) on_files[i]->Get("type1MET")));
    offz_met->Add((TH1D*) ((TH1D*) below_files[i]->Get("type1MET")));
    offz_met->Add((TH1D*) ((TH1D*) above_files[i]->Get("type1MET")));
  }
  cout<<__LINE__<<endl;
// ==================
//  Print Plots
// ==================

  double binning[11] = {0,25,50,75,100,150,200,250,300,400,500};
  TH1D *onz_met_varbin = (TH1D*) onz_met->Clone("onz_met_varbin");
  TH1D *offz_met_varbin = (TH1D*) offz_met->Clone("offz_met_varbin");


  onz_met_varbin = (TH1D*) onz_met_varbin->Rebin(10, "onz_met_varbin2", binning);
  offz_met_varbin = (TH1D*) offz_met_varbin->Rebin(10, "offz_met_varbin2", binning);
  //-------------
  // MET 10GeV bins
  //-------------

  TCanvas *c1 = new TCanvas("c1", "", 2000, 2000);
  c1->cd();

  cout<<__LINE__<<endl;

  //gPad->SetLogy(1);
  gStyle->SetOptStat(kFALSE);

  onz_met->SetTitle("OnZ MET / Off Z MET");
  onz_met->SetXTitle("Ratio");
  onz_met->SetYTitle("OnZ/OffZ");
  
  onz_met->Rebin(10);
  offz_met->Rebin(10);

  onz_met->Divide(offz_met);

  onz_met->GetXaxis()->SetRangeUser(0,800);

  onz_met->Draw("E1");

  c1->SaveAs(output_dir+"MET.png");

  //-------------
  // MET Variable bins
  //-------------

  TCanvas *c2 = new TCanvas("c2", "", 2000, 2000);
  c2->cd();

  cout<<__LINE__<<endl;

  //gPad->SetLogy(1);
  gStyle->SetOptStat(kFALSE);

  onz_met_varbin->SetTitle("OnZ MET / Off Z MET");
  onz_met_varbin->SetXTitle("Ratio");
  onz_met_varbin->SetYTitle("OnZ/OffZ");

  onz_met_varbin->Divide(offz_met_varbin);

  onz_met_varbin->GetXaxis()->SetRangeUser(0,800);

  onz_met_varbin->Draw("E1");

  c2->SaveAs(output_dir+"MET_varbin.png");


}