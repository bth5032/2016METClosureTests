void FS_Mass_Window_Study(){
  vector<TFile*> on_files;
  vector<TFile*> above_files;
  vector<TFile*> below_files;

  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_onZ/Z_Base.root", read));
  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_onZ/TT_Base.root", read));
  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_onZ/VVV.root", read));
  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_onZ/ttv.root", read));
  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_onZ/wz.root", read));
  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_onZ/zz.root", read));

  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_aboveZ/Z_Base.root", read));
  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_aboveZ/TT_Base.root", read));
  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_aboveZ/VVV.root", read));
  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_aboveZ/ttv.root", read));
  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_aboveZ/wz.root", read));
  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_aboveZ/zz.root", read));

  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_belowZ/Z_Base.root", read));
  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_belowZ/TT_Base.root", read));
  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_belowZ/VVV.root", read));
  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_belowZ/ttv.root", read));
  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_belowZ/wz.root", read));
  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/baseline_belowZ/zz.root", read));


// ==================
//  Define Histograms
// ==================
  TH1D *onz_met = (TH1D*) ((TH1D*) on_files[0]->Get("met"))->Clone("onz_met");
  TH1D *offz_met = (TH1D*) ((TH1D*) above_files[0]->Get("met"))->Clone("onz_met");
  offz_met->Add((TH1D*) ((TH1D*) below_files[0]->Get("met")));
  
// ==================
//  Fill Histograms
// ==================

  for (int i = 1; i < (int) on_files.size(); i++){
    onz_met->Add((TH1D*) ((TH1D*) on_files[i]->Get("met")));
    offz_met->Add((TH1D*) ((TH1D*) below_files[i]->Get("met")));
    offz_met->Add((TH1D*) ((TH1D*) above_files[i]->Get("met")));
  }

// ==================
//  Print Plots
// ==================



}