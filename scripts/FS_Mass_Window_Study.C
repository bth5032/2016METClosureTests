void updateOverflow( TH1D * &hist, double xmax ){

  int overflowbin = hist->FindBin(xmax-0.01);
  for( int bini = overflowbin; bini < hist->GetNbinsX(); bini++ ){
    hist->SetBinContent( overflowbin, hist->GetBinContent( overflowbin ) + hist->GetBinContent( bini + 1 ) ); 
    hist->SetBinError  ( overflowbin, sqrt( pow(hist->GetBinError  ( overflowbin ), 2 ) + pow( hist->GetBinError( bini + 1 ), 2 ) ) );  
    hist->SetBinContent( bini + 1, 0 );
    hist->SetBinError  ( bini + 1, 0 );
  }
}

void FS_Mass_Window_Study(){

  TString id="baseline_MET100_MT2100_1B";
  //TString id="baseline_MET100_MT2100";
  //TString id="baseline_MET100";
  //TString id="baseline";

  TString output_dir="/home/users/bhashemi/public_html/ZMET2016_NovemberClean/FS_mass_window_studies/"+id+"_ratios/";

  vector<TFile*> on_files;
  vector<TFile*> above_files;
  vector<TFile*> below_files;

  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_onZ/Z_Base.root", "read"));
  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_onZ/TT_dilep.root", "read"));
  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_onZ/TT_1lep.root", "read"));
  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_onZ/ww.root", "read"));
  //on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_onZ/singletop.root", "read"));
  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_onZ/ttw.root", "read"));
  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_onZ/VVV.root", "read"));
  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_onZ/ttv.root", "read"));
  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_onZ/wz.root", "read"));
  on_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_onZ/zz.root", "read"));

  cout<<__LINE__<<endl;

  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_aboveZ/Z_Base.root", "read"));
  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_aboveZ/TT_dilep.root", "read"));
  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_aboveZ/TT_1lep.root", "read"));
  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_aboveZ/ww.root", "read"));
  //above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_aboveZ/singletop.root", "read"));
  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_aboveZ/ttw.root", "read"));
  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_aboveZ/VVV.root", "read"));
  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_aboveZ/ttv.root", "read"));
  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_aboveZ/wz.root", "read"));
  above_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_aboveZ/zz.root", "read"));

  cout<<__LINE__<<endl;

  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_belowZ/Z_Base.root", "read"));
  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_belowZ/TT_dilep.root", "read"));
  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_belowZ/TT_1lep.root", "read"));
  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_belowZ/ww.root", "read"));
  //below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_belowZ/singletop.root", "read"));
  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_belowZ/ttw.root", "read"));
  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_belowZ/VVV.root", "read"));
  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_belowZ/ttv.root", "read"));
  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_belowZ/wz.root", "read"));
  below_files.push_back(TFile::Open("/nfs-7/userdata/bobak/ZMET2016_Hists_NovemberClean/FS_mass_window_studies/"+id+"_belowZ/zz.root", "read"));


  cout<<__LINE__<<endl;

// ==================
//  Define Histograms
// ==================
  TH1D *onz_met = (TH1D*) ((TH1D*) on_files[0]->Get("type1MET"))->Clone("onz_met");
  cout<<__LINE__<<endl;
  TH1D *offz_met = (TH1D*) ((TH1D*) above_files[0]->Get("type1MET"))->Clone("offz_met");
  cout<<__LINE__<<endl;
  offz_met->Add((TH1D*) ((TH1D*) below_files[0]->Get("type1MET")));
  cout<<__LINE__<<endl;

  TH1D *onz_njets = (TH1D*) ((TH1D*) on_files[0]->Get("njets"))->Clone("onz_njets");
  cout<<__LINE__<<endl;
  TH1D *offz_njets = (TH1D*) ((TH1D*) above_files[0]->Get("njets"))->Clone("offz_njets");
  cout<<__LINE__<<endl;
  offz_njets->Add((TH1D*) ((TH1D*) below_files[0]->Get("njets")));
  cout<<__LINE__<<endl;

  TH1D *onz_nbjets = (TH1D*) ((TH1D*) on_files[0]->Get("nbtags_m"))->Clone("onz_nbjets");
  cout<<__LINE__<<endl;
  TH1D *offz_nbjets = (TH1D*) ((TH1D*) above_files[0]->Get("nbtags_m"))->Clone("offz_nbjets");
  cout<<__LINE__<<endl;
  offz_nbjets->Add((TH1D*) ((TH1D*) below_files[0]->Get("nbtags_m")));
  cout<<__LINE__<<endl;

  TH1D *onz_ht = (TH1D*) ((TH1D*) on_files[0]->Get("ht"))->Clone("onz_ht");
  cout<<__LINE__<<endl;
  TH1D *offz_ht = (TH1D*) ((TH1D*) above_files[0]->Get("ht"))->Clone("offz_ht");
  cout<<__LINE__<<endl;
  offz_ht->Add((TH1D*) ((TH1D*) below_files[0]->Get("ht")));
  cout<<__LINE__<<endl;

// ==================
//  Fill Histograms
// ==================
  cout<<__LINE__<<endl;
  for (int i = 1; i < (int) on_files.size(); i++){
    onz_met->Add((TH1D*) ((TH1D*) on_files[i]->Get("type1MET")));
    offz_met->Add((TH1D*) ((TH1D*) below_files[i]->Get("type1MET")));
    offz_met->Add((TH1D*) ((TH1D*) above_files[i]->Get("type1MET")));

    onz_njets->Add((TH1D*) ((TH1D*) on_files[i]->Get("njets")));
    offz_njets->Add((TH1D*) ((TH1D*) below_files[i]->Get("njets")));
    offz_njets->Add((TH1D*) ((TH1D*) above_files[i]->Get("njets")));

    onz_nbjets->Add((TH1D*) ((TH1D*) on_files[i]->Get("nbtags_m")));
    offz_nbjets->Add((TH1D*) ((TH1D*) below_files[i]->Get("nbtags_m")));
    offz_nbjets->Add((TH1D*) ((TH1D*) above_files[i]->Get("nbtags_m")));

    onz_ht->Add((TH1D*) ((TH1D*) on_files[i]->Get("ht")));
    offz_ht->Add((TH1D*) ((TH1D*) below_files[i]->Get("ht")));
    offz_ht->Add((TH1D*) ((TH1D*) above_files[i]->Get("ht")));
  }
  cout<<__LINE__<<endl;
// ==================
//  Rebin Hists
// ==================

  double met_binning[8] = {0,25,50,75,100,150,250,600};
  TH1D *onz_met_varbin = (TH1D*) onz_met->Clone("onz_met_varbin");
  TH1D *offz_met_varbin = (TH1D*) offz_met->Clone("offz_met_varbin");

  onz_met_varbin = (TH1D*) onz_met_varbin->Rebin(7, "onz_met_varbin2", met_binning);
  offz_met_varbin = (TH1D*) offz_met_varbin->Rebin(7, "offz_met_varbin2", met_binning);

  onz_met_varbin3 = (TH1D*) onz_met_varbin->Clone("onz_met_varbin3");
  offz_met_varbin3 = (TH1D*) offz_met_varbin->Clone("offz_met_varbin3");

  double ht_binning[4] = {0,200,500,1000};
  TH1D *onz_ht_varbin = (TH1D*) onz_ht->Clone("onz_ht_varbin");
  TH1D *offz_ht_varbin = (TH1D*) offz_ht->Clone("onz_ht_varbin");

  onz_ht_varbin = (TH1D*) onz_ht_varbin->Rebin(3, "onz_ht_varbin2", ht_binning);
  offz_ht_varbin = (TH1D*) offz_ht_varbin->Rebin(3, "onz_ht_varbin2", ht_binning);

  onz_ht_finebin = (TH1D*) onz_ht->Clone("onz_ht_finebin");
  offz_ht_finebin = (TH1D*) offz_ht->Clone("offz_ht_finebin");

  onz_ht_finebin->Rebin(100);
  offz_ht_finebin->Rebin(100);

  //cout<<"601 bin: "<<onz_met_varbin->FindBin(601)<<" 6001 bin: "<<onz_met_varbin->FindBin(6001)<<endl;

// ==================
//  Draw Errrrthang
// ==================
  //-------------
  // MET 10GeV bins
  //-------------
    TCanvas *c1 = new TCanvas("c1", "", 2000, 2000);
    c1->cd();

    cout<<__LINE__<<endl;

    //gPad->SetLogy(1);
    gPad->SetLeftMargin(.1);
    gStyle->SetOptStat(kFALSE);

    onz_met->SetTitle("Ratio of Events on Z to inclusive M_{ll}, binned in E^{miss}_{T}");
    onz_met->SetXTitle("Ratio / 10 GeV");
    onz_met->SetYTitle("E^{miss}_{T}");
    onz_met->GetYaxis()->SetTitleOffset(1.4);
    
    onz_met->Rebin(10);
    offz_met->Rebin(10);

    updateOverflow(onz_met, 600);
    updateOverflow(offz_met, 600);

    offz_met->Add(onz_met);
    
    onz_met->Divide(onz_met,offz_met,1,1,"B");

    onz_met->GetXaxis()->SetRangeUser(0,600);

    onz_met->Draw("E1");

    c1->SaveAs(output_dir+"MET.png");

  //-------------
  // MET Variable bins
  //-------------

    TCanvas *c2 = new TCanvas("c2", "", 2000, 2000);
    c2->cd();

    cout<<__LINE__<<endl;

    //gPad->SetLogy(1);
    gPad->SetLeftMargin(.1);
    gStyle->SetOptStat(kFALSE);

    onz_met_varbin->SetTitle("Ratio of Events on Z to off Z M_{ll}, binned in E^{miss}_{T} (sum squared errors)");
    onz_met_varbin->SetXTitle("E^{miss}_{T}");
    onz_met_varbin->SetYTitle("Ratio");
    onz_met_varbin->GetYaxis()->SetTitleOffset(1.4);
    
    updateOverflow(onz_met_varbin, 600);
    updateOverflow(offz_met_varbin, 600);

    onz_met_varbin->Divide(offz_met_varbin);

    onz_met_varbin->GetXaxis()->SetRangeUser(0,600);

    onz_met_varbin->Draw("E1");

    c2->SaveAs(output_dir+"MET_varbin_sumsqerrs.png");

  //-------------
  // MET Variable bins (Binomial Errors)
  //-------------

    TCanvas *c3 = new TCanvas("c3", "", 2000, 2000);
    c3->cd();

    cout<<__LINE__<<endl;

    //gPad->SetLogy(1);
    gPad->SetLeftMargin(.1);
    gStyle->SetOptStat(kFALSE);

    updateOverflow(onz_met_varbin3, 600);
    updateOverflow(offz_met_varbin3, 600);

    onz_met_varbin3->SetTitle("Ratio of Events on Z to inclusive M_{ll}, binned in E^{miss}_{T}");
    onz_met_varbin3->SetXTitle("E^{miss}_{T}");
    onz_met_varbin3->SetYTitle("Ratio");
    onz_met_varbin3->GetYaxis()->SetTitleOffset(1.4);
    
    offz_met_varbin3->Add(onz_met_varbin3);
    
    onz_met_varbin3->Divide(onz_met_varbin3,offz_met_varbin3,1,1,"B");

    onz_met_varbin3->GetXaxis()->SetRangeUser(0,600);

    onz_met_varbin3->Draw("E1");

    c3->SaveAs(output_dir+"MET_varbin.png");

  //-------------
  // HT Variable bins fine
  //-------------

    TCanvas *c4 = new TCanvas("c4", "", 2000, 2000);
    c4->cd();

    cout<<__LINE__<<endl;

    //gPad->SetLogy(1);
    gPad->SetLeftMargin(.1);
    gStyle->SetOptStat(kFALSE);

    updateOverflow(onz_ht_finebin, 1000);
    updateOverflow(offz_ht_finebin, 1000);

    onz_ht_finebin->SetTitle("Ratio of Events on Z to inclusive M_{ll}, binned in H_{T}");
    onz_ht_finebin->SetXTitle("H_{T}");
    onz_ht_finebin->SetYTitle("Ratio / 100 GeV");
    onz_ht_finebin->GetYaxis()->SetTitleOffset(1.4);
    
    offz_ht_finebin->Add(onz_ht_finebin);
    
    onz_ht_finebin->Divide(onz_ht_finebin,offz_ht_finebin,1,1,"B");

    onz_ht_finebin->GetXaxis()->SetRangeUser(0,1000);

    onz_ht_finebin->Draw("E1");

    c4->SaveAs(output_dir+"HT_finebin.png");

  //-------------
  // HT Variable bins
  //-------------

    TCanvas *c5 = new TCanvas("c5", "", 2000, 2000);
    c5->cd();

    cout<<__LINE__<<endl;

    //gPad->SetLogy(1);
    gPad->SetLeftMargin(.1);
    gStyle->SetOptStat(kFALSE);

    updateOverflow(onz_ht_varbin, 1000);
    updateOverflow(offz_ht_varbin, 1000);

    onz_ht_varbin->SetTitle("Ratio of Events on Z to inclusive M_{ll}, binned in H_{T}");
    onz_ht_varbin->SetXTitle("H_{T}");
    onz_ht_varbin->SetYTitle("Ratio / 100 GeV");
    onz_ht_varbin->GetYaxis()->SetTitleOffset(1.4);

    
    offz_ht_varbin->Add(onz_ht_varbin);
    
    onz_ht_varbin->Divide(onz_ht_varbin,offz_ht_varbin,1,1,"B");

    onz_ht_varbin->GetXaxis()->SetRangeUser(0,600);
    onz_ht_varbin->GetYaxis()->SetRangeUser(0,0.1);

    onz_ht_varbin->Draw("E1");

    c5->SaveAs(output_dir+"HT.png");

  //-------------
  // NJets
  //-------------

    TCanvas *c6 = new TCanvas("c6", "", 2000, 2000);
    c6->cd();

    cout<<__LINE__<<endl;

    //gPad->SetLogy(1);
    gPad->SetLeftMargin(.1);
    gStyle->SetOptStat(kFALSE);

    updateOverflow(onz_njets, 1000);
    updateOverflow(offz_njets, 1000);

    onz_njets->SetTitle("Ratio of Events on Z to inclusive M_{ll}, binned in Number of Jets");
    onz_njets->SetXTitle("Number of Jets");
    onz_njets->SetYTitle("Ratio");
    onz_njets->GetYaxis()->SetTitleOffset(1.4);
    
    offz_njets->Add(onz_njets);
    
    onz_njets->Divide(onz_njets,offz_njets,1,1,"B");

    onz_njets->GetXaxis()->SetRangeUser(0,10);

    onz_njets->Draw("E1");

    c6->SaveAs(output_dir+"njets.png");

  //-------------
  // Num Btagged Jets
  //-------------

    TCanvas *c7 = new TCanvas("c7", "", 2000, 2000);
    c7->cd();

    cout<<__LINE__<<endl;

    //gPad->SetLogy(1);
    gPad->SetLeftMargin(.1);
    gStyle->SetOptStat(kFALSE);

    updateOverflow(onz_nbjets, 1000);
    updateOverflow(offz_nbjets, 1000);

    onz_nbjets->SetTitle("Ratio of Events on Z to inclusive M_{ll}, binned in Number of B tags (csv medium)");
    onz_nbjets->SetXTitle("Number of B tags (csv medium)");
    onz_nbjets->SetYTitle("Ratio");
    onz_nbjets->GetYaxis()->SetTitleOffset(1.4);
    
    offz_nbjets->Add(onz_nbjets);
    
    onz_nbjets->Divide(onz_nbjets,offz_nbjets,1,1,"B");

    onz_nbjets->GetXaxis()->SetRangeUser(0,10);

    onz_nbjets->Draw("E1");

    c7->SaveAs(output_dir+"nbjets.png");


}