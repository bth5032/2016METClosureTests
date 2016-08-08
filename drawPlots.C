#include <iostream>
#include <stdexcept>
#include <vector>

#include "TChain.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TColor.h"
#include "THStack.h"
#include "TGaxis.h"
#include "TCut.h"
#include "TH1D.h"

#include "ConfigParser.C"

using namespace std;

double errMult(double A, double B, double errA, double errB, double C) {
  return sqrt(C*C*(pow(errA/A,2) + pow(errB/B,2)));
}

void drawLatexFromTString(TString text, double x_low, double y_low){
  TLatex *lumitex = NULL;
  
  lumitex = new TLatex(x_low, y_low , text );      
  lumitex->SetNDC();    
  lumitex->SetTextSize(30);    
  lumitex->SetLineWidth(2);
  lumitex->SetTextFont(43);    
  lumitex->Draw();

  return;
}

//nothing

void drawCMSLatex(double luminosity){
  TLatex *lumitex = NULL;
  height=1.02-gPad->GetTopMargin();
  float left_margin = gPad->GetLeftMargin();

  // lumitex = new TLatex(0.66,0.955, Form("%.1f fb^{-1} (13 TeV)", luminosity) );    
  lumitex = new TLatex(.9-left_margin, height , Form("%.1f pb^{-1} (13 TeV)", luminosity*1000) );    
  // lumitex = new TLatex(0.66,0.955, Form("few pb^{-1} (13 TeV)") );    
  lumitex->SetNDC();    
  lumitex->SetTextSize(0.03);    
  lumitex->SetLineWidth(2);
  lumitex->SetTextFont(42);    
  lumitex->Draw();

  TLatex *cmstex = NULL;
  cmstex = new TLatex(left_margin, height, "#it{CMS #bf{Preliminary}}" );    
  cmstex->SetNDC();    
  cmstex->SetTextSize(0.03);    
  cmstex->SetLineWidth(2);
  cmstex->SetTextFont(62);    
  cmstex->Draw();

  return;
}

TString drawArbitraryNumberWithResidual(ConfigParser *conf){
  // This method expects conf to have a plot config loaded in already. 
  //In the conf, we expect there to be hist names of the form file_N_path,
  //hist_n_name, starting with 0 for the primary histogram, which is normally 
  //going to be the data events in our signal region. The rest of the hists, starting
  //from 1, are added to a THStack which is normalized to hist_0 in the bin 0-50. 
  //num_hists should be the number of the number of histograms in the plot.
  TString errors="";

  int num_hists=stoi(conf->get("num_hists"));

  if (num_hists < 2){
    return TString("Less than Two hists can not be turned into a residual plot, please call drawSingleTH1");
  } 

  //Add files from which to obtain histos
  vector<TFile*> hist_files (num_hists);
  for (int i = 0; i<num_hists; i++){
    hist_files[i]=new TFile(TString(conf->get("file_"+to_string(i)+"_path")));
  }
  cout << "Found files "<<endl;

  TString plot_name = conf->get("plot_name");
  double xmax = stod(conf->get("xmax"));
  double xmin = stod(conf->get("xmin"));
  double bin_size = stod(conf->get("bin_size"));
  
  //Get name of hist to read from file
  vector<TString> hist_names (num_hists);
  for (int i = 0; i<num_hists; i++){
    if (conf->get("hist_"+to_string(i)+"_name") != ""){
      hist_names[i]=conf->get("hist_"+to_string(i)+"_name");    
    }
    else{
      hist_names[i]= conf->get("hist_0_name");
    }
  }

  //Get labels for TLegend
  vector<TString> hist_labels (num_hists);
  for (int i = 0; i<num_hists; i++){
    hist_labels[i]=conf->get("hist_"+to_string(i)+"_label");    
  }  

  //Set prefix for retriving histogram
  vector<TString> hist_prefix (num_hists);
  for (int i = 0; i<num_hists; i++){
    hist_prefix[i]=conf->get("hist_"+to_string(i)+"_prefix");    
  }  

  cout<<"Hist names set"<<endl;
  TString xlabel=conf->get("xlabel");
  TString ylabel=conf->get("ylabel");
  TString save_dir=conf->get("save_dir");
  TString plot_title=conf->get("title");


  cout << "Making Plots for: "<<plot_name<<endl;

  vector<TH1D*> hists (num_hists);
  for (int i = 1; i<num_hists; i++){
    hists[i] = (TH1D*) ((TH1D*) hist_files[i]->Get(hist_prefix[i]+"_"+hist_names[i]))->Clone("hist_"+to_string(i)+"_"+plot_name);
    cout<<hist_names[i]<<" found in "<<hist_files[i]->GetName()<<endl;
  }  
  cout << "Histograms pulled from files, adding draw options"<<endl;
  
  //Add all the background hists to a stack.
  THStack * stack = new THStack(("stack_"+conf->get("Name")).c_str(), conf->get("title").c_str());
  cout<<__LINE__<<endl;
  for (int i=1; i<num_hists; i++)
  {
    stack->Add(hists[i]);
  } 

  cout<<__LINE__<<endl;


  //Create sum of background samples
  TH1D *bg_sum = (TH1D*) hists[1]->Clone("bg_sum_"+plot_name);
  bg_sum->SetTitle("Sum of background samples");
  
  cout<<__LINE__<<endl;
  
  for (int i=2; i<num_hists; i++){
    bg_sum->Add(hists[i]);
  }

  //============================================
  // Draw Data-MC Plots
  //============================================
  
  cout<<__LINE__<<endl;
  TCanvas * c = new TCanvas("c","",2000,2000);
  c->cd();
  gPad->SetRightMargin(0.05);
  gPad->Modified();
  gStyle->SetOptStat(kFALSE);
  TPad *fullpad = new TPad("fullpad", "fullpad", 0,0,1,1);
  cout<<__LINE__<<endl;
  fullpad->Draw();
  fullpad->cd();
  cout<<__LINE__<<endl;
  TPad *plotpad = new TPad("plotpad", "plotpad",0,0.2,1.0,0.99);
  
  plotpad->SetRightMargin(0.05);
  if (conf->get("ExtraRightMargin") == "true")
  {
    plotpad->SetRightMargin(0.08);
  }
  plotpad->SetBottomMargin(0.12);
  cout<<__LINE__<<endl;
  plotpad->Draw();
  plotpad->cd();
  cout<<__LINE__<<endl;
  if (conf->get("logy") == "true")
  {
    cout<<"Plot tagged for log y-axis"<<endl;
    plotpad->SetLogy();
  }
  cout<<__LINE__<<endl;
  TH1D * h_ptr;
  for (int i = 0; i<num_hists; i++){
     h_ptr = hists.at(i);
     h_ptr->Rebin(bin_size);
  }
  cout<<__LINE__<<endl;

  //===========================
  // Normalize
  //===========================
  cout<<__LINE__<<endl;
  if (conf->get("normalize") == "true")
  {
    double numEventsData;
    double numEventsMC;
    double scaleFactor;
    if (conf->get("norm_0_50") == "true")
    {
      numEventsData = hists[0]->Integral(hists[0]->FindBin(1),hists[0]->FindBin(49.9));
      numEventsMC = bg_sum->Integral(bg_sum->FindBin(1),bg_sum->FindBin(49.9));
    }

    else{
      numEventsData = hists[0]->Integral();
      numEventsMC = bg_sum->Integral();
    }
    cout<<__LINE__<<endl;

    scaleFactor = ((double) numEventsData/numEventsMC);
    for (int i = 1; i<num_hists; i++){
      hists[i]->Scale(scaleFactor);  
    }
  }
  cout<<__LINE__<<endl;


  //===========================
  // SET MC COLORS
  //===========================
  cout<<__LINE__<<endl;
  for (int i = 1; i<num_hists; i++){
    hists[i]->SetFillColor(i+1);
    hists[i]->SetFillStyle(1001);
  }
  cout<<__LINE__<<endl;
  hists[0]->SetMarkerStyle(20);

  //===========================
  // Find Plot Maxima
  //===========================
  cout<<__LINE__<<endl;
  double ymax = 0;
  TH1D* clonedBG = (TH1D*) bg_sum->Clone("clonedBG_forReweight_"+plot_name);
  TH1D* clonedPrimary = (TH1D*) hists[0]->Clone("clonedPrimary_forReweight_"+plot_name);
  cout<<__LINE__<<endl;
  clonedPrimary->GetXaxis()->SetRangeUser(xmin, xmax);
  clonedBG->GetXaxis()->SetRangeUser(xmin,xmax);
  cout<<__LINE__<<endl;
  if (clonedBG->GetMaximum() < clonedPrimary->GetMaximum()){
      ymax = 1.2*clonedPrimary->GetMaximum();
  }
  else {
      ymax = 1.2*clonedBG->GetMaximum();   
  }
  cout<<__LINE__<<endl;
  cout<<"Primary Max: "<< clonedPrimary->GetMaximum() << " Secondary Max: "<< clonedBG->GetMaximum() <<endl;
  cout<<"Proper plot maximum set to "<<ymax<<endl;
  
  delete clonedBG;
  delete clonedPrimary;
  cout<<__LINE__<<endl;
  
  TH2F* h_axes = new TH2F(Form("%s_axes",plot_name.Data()),plot_title,hists[0]->GetNbinsX(),xmin,xmax,1000,0.001,ymax);
  
  
  //-----------------------
  // AXES FIX
  //-----------------------
  
  cout<<"Setting axis names"<<endl;
  h_axes->GetXaxis()->SetTitle(xlabel);
  h_axes->GetYaxis()->SetTitle(ylabel);
  cout<<__LINE__<<endl;  

  TString stats_string;

  //===========================
  // Print Closure Stats
  //===========================


  if (conf->get("print_stats") == "true")
  {
    int low_val = stoi(conf->get("stats_low_val"));
    int high_val = stoi(conf->get("stats_high_val"));
    cout<<__LINE__<<endl;
    Double_t err_evts_in_interval_primary;
    double num_evts_in_interval_primary;

    Double_t err_evts_in_interval; 
    double num_evts_in_interval;
    cout<<__LINE__<<endl;
    for (int i=0; i<num_hists; i++){
      num_evts_in_interval = hists[i]->IntegralAndError(hists[i]->FindBin(low_val), hists[i]->FindBin(high_val-.001), err_evts_in_interval);
      if (i == 0){
        num_evts_in_interval_primary = num_evts_in_interval;
        err_evts_in_interval_primary = err_evts_in_interval;
      }
      cout<<__LINE__<<endl;
      stats_string = "Number of Events in "+hist_names[i]+" from "+conf->get("stats_low_val")+" to "+conf->get("stats_high_val")+" : "+to_string(num_evts_in_interval)+" Error: "+to_string(err_evts_in_interval);
      cout<<"STATS: "<<stats_string<<endl;
      drawLatexFromTString(stats_string, .52,.5+(0.02*i));
    }
    cout<<__LINE__<<endl;
  }
  
  //----------------------
  // ADD OVERFLOW BIN
  //----------------------
  cout<<__LINE__<<endl;
  if (conf->get("overflow")=="true"){
    cout<<"Plot tagged for overflow bin, building..."<<endl;
    double n_bins = hists[0]->GetNbinsX();
    double overflow, max;
    cout<<__LINE__<<endl;
    for (int i = 0; i<num_hists; i++){
      overflow = hists[i]->GetBinContent(n_bins + 1);
      max = hists[i]->Integral(hists[i]->FindBin(xmax-.001), n_bins);
      hists[i]->SetBinContent(hists[i]->FindBin(xmax-.001), max+overflow);
    }
    cout<<__LINE__<<endl;
  }
  
      
  
  plotpad->SetLeftMargin(0.15);
  h_axes->GetYaxis()->SetTitleOffset(1.3);
  h_axes->GetYaxis()->SetTitleSize(0.05);
  h_axes->GetYaxis()->SetLabelSize(0.04);
  cout<<__LINE__<<endl;
  cout<<"Drawing histograms"<<endl;
  h_axes->Draw();
  stack->Draw("HIST SAME");
  hists[0]->Draw("E1 SAME");

  plotpad->RedrawAxis();
  cout<<__LINE__<<endl;
  TLegend *l1;
  l1 = new TLegend(0.73, 0.73, 0.88, 0.88);
  
  l1->SetLineColor(kWhite);  
  l1->SetShadowColor(kWhite);
  l1->SetFillColor(kWhite);
  cout<<__LINE__<<endl;
  l1->AddEntry(hists[0], hist_labels[0], "p");
  for (int i = 1; i<num_hists; i++){
    l1->AddEntry(hists[i], hist_labels[i], "f");
  }
  cout<<__LINE__<<endl;

  l1->Draw("same");
  
  //--------------------------
  // Fill in Residual Plot
  //--------------------------
  
  cout<<"Getting ready for residual plots"<<endl;
  fullpad->cd();
  TPad *ratiopad = new TPad("ratiopad", "ratiopad" ,0.,0.,1,0.21);
  ratiopad->SetTopMargin(0.05);
  ratiopad->SetLeftMargin(0.15);
  ratiopad->SetBottomMargin(0.1);
  ratiopad->SetRightMargin(0.05);
  ratiopad->SetGridy();  // doesn't actually appear for some reason..
  ratiopad->Draw();
  ratiopad->cd();
  cout<<__LINE__<<endl;
  
  TH1D* residual = (TH1D*) hists[0]->Clone("residual");
  residual->Divide(bg_sum);
  cout<<__LINE__<<endl;
  //cout<<"Fixing error bars"<<endl;
  //for (int count=1; count<=mc_sum->GetNbinsX(); count++){ 
  //  double relative_error = (mc_sum->GetBinError(count))/ (mc_sum->GetBinContent(count));
  //  residual->SetBinError(count, residual->GetBinContent(count)*relative_error);
  //}
  
  cout<<"Building axes"<<endl;
  TH1D* h_axis_ratio = new TH1D(Form("%s_residual_axes",plot_name.Data()),"",residual->GetNbinsX(),xmin,xmax);
  cout<<__LINE__<<endl;
  h_axis_ratio->GetYaxis()->SetTitleOffset(0.33);
  h_axis_ratio->GetYaxis()->SetTitleSize(0.18);
  h_axis_ratio->GetYaxis()->SetNdivisions(5);
  h_axis_ratio->GetYaxis()->SetLabelSize(0.15);
  //h_axis_ratio->GetYaxis()->SetRangeUser(0.5,1.5);
  h_axis_ratio->GetYaxis()->SetRangeUser(0.001,2.0);
  h_axis_ratio->GetYaxis()->SetTitle("Data/MC");
  h_axis_ratio->GetXaxis()->SetTickLength(0.07);
  h_axis_ratio->GetXaxis()->SetTitleSize(0.);
  h_axis_ratio->GetXaxis()->SetLabelSize(0.);
  
  cout<<__LINE__<<endl;
  TLine* line1 = new TLine(xmin,1,xmax,1);
  line1->SetLineStyle(2);
  
  cout<<"Drawing ratio plot"<<endl;
  h_axis_ratio->Draw("axis");
  line1->Draw("same");
  residual->Draw("same");
  cout<<__LINE__<<endl;
  c->Update();
  c->cd();
  
  //Draw luminosity and CMS tag
  if (conf->get("luminosity_fb") != ""){
    plotpad->cd();
    drawCMSLatex(stod(conf->get("luminosity_fb")));
  }
  cout<<__LINE__<<endl;
  
  cout<<"Saving..."<<endl;
  c->SaveAs(save_dir+plot_name+TString(".pdf"));
  c->SaveAs(save_dir+plot_name+TString(".png"));
  //c->SaveAs(save_dir+plot_name+TString(".root"));
  //c->SaveAs(save_dir+plot_name+TString(".C"));
  cout<<__LINE__<<endl;
  cout<<"Cleaning up plot variables"<<endl;
  delete l1;
  hists.clear();
  hist_names.clear();
  hist_labels.clear();
  hist_prefix.clear();
  delete residual;
  delete ratiopad;
  delete plotpad;
  delete fullpad;
  delete c;
  cout<<__LINE__<<endl;
  for (int i = 0; i<num_hists; i++){
    hist_files[i]->Close();
  }
  hist_files.clear();
  cout<<__LINE__<<endl;
  return errors;
}

TString drawTwoWithResidual(ConfigParser *conf){
  // This method expects conf to have a plot config loaded in already.
  TString errors="";

  TFile* f_primary = new TFile(TString(conf->get("primary_path")));
  TFile* f_secondary = new TFile(TString(conf->get("secondary_path")));

  cout << "Found files "<<endl;

  TString plot_name = conf->get("plot_name");
  double xmax = stod(conf->get("xmax"));
  double xmin = stod(conf->get("xmin"));
  double bin_size = stod(conf->get("bin_size"));
  TString hist_name=conf->get("hist_name");
  TString hist_name2=conf->get("hist_name");
  
  if (conf->get("hist_name2") != "")
  {
    hist_name2=conf->get("hist_name2");    
  }

  TString xlabel=conf->get("xlabel");
  TString ylabel=conf->get("ylabel");
  TString primary_name=conf->get("primary_name");
  TString secondary_name=conf->get("secondary_name");
  TString save_dir=conf->get("save_dir");
  TString plot_title=conf->get("title");


  cout << "Making Plots for: "<<plot_name<<endl;

  TH1D* p_hist = (TH1D*) ((TH1D*) f_primary->Get(primary_name+"_"+hist_name))->Clone("phist_"+plot_name);
  cout<<hist_name<<" found in "<<f_primary->GetName()<<endl;

  TH1D* s_hist = (TH1D*) ((TH1D*) f_secondary->Get(secondary_name+"_"+hist_name2))->Clone("shist_"+plot_name);
  cout<<hist_name2<<" found in "<<f_secondary->GetName()<<endl;

  cout << "Histograms pulled from files, adding draw options"<<endl;
  
  //============================================
  // Draw Data-MC Plots
  //============================================
  
  TCanvas * c = new TCanvas("c","",2000,2000);
  c->cd();
  gPad->SetRightMargin(0.05);
  gPad->Modified();
  gStyle->SetOptStat(kFALSE);
  TPad *fullpad = new TPad("fullpad", "fullpad", 0,0,1,1);
  
  fullpad->Draw();
  fullpad->cd();
  
  TPad *plotpad = new TPad("plotpad", "plotpad",0,0.2,1.0,0.99);
  
  plotpad->SetRightMargin(0.05);
  if (conf->get("ExtraRightMargin") == "true")
  {
    plotpad->SetRightMargin(0.08);
  }
  plotpad->SetBottomMargin(0.12);
  
  plotpad->Draw();
  plotpad->cd();
  
  if (conf->get("logy") == "true")
  {
    cout<<"Plot tagged for log y-axis"<<endl;
    plotpad->SetLogy();
  }
  
  p_hist->Rebin(bin_size);
  s_hist->Rebin(bin_size);
  
  //===========================
  // Normalize
  //===========================
  if (conf->get("normalize") == "true")
  {
    double numEventsData;
    double numEventsMC;
    double scaleFactor;
    if (conf->get("norm_0_50") == "true")
    {
      numEventsData = p_hist->Integral(p_hist->FindBin(1),p_hist->FindBin(49.9));
      numEventsMC = s_hist->Integral(p_hist->FindBin(1),s_hist->FindBin(49.9));
    }
    else{
      numEventsData = p_hist->Integral();
      numEventsMC = s_hist->Integral();
    }
    scaleFactor = ((double) numEventsData/numEventsMC);
    s_hist->Scale(scaleFactor);
  }


  //===========================
  // SET MC COLORS
  //===========================
  
  s_hist->SetFillColor(kAzure+5);
  s_hist->SetFillStyle(1001);
  
  p_hist->SetMarkerStyle(20);

  //===========================
  // Find Plot Maxima
  //===========================
  
  double ymax = 0;
  TH1D* clonedSecondary = (TH1D*) s_hist->Clone("clonedSecondary_forReweight_"+plot_name);
  TH1D* clonedPrimary = (TH1D*) p_hist->Clone("clonedPrimary_forReweight_"+plot_name);
  
  clonedSecondary->GetXaxis()->SetRangeUser(xmin, xmax);
  clonedPrimary->GetXaxis()->SetRangeUser(xmin,xmax);
  
  if (clonedSecondary->GetMaximum() < clonedPrimary->GetMaximum()){
      ymax = 1.2*clonedPrimary->GetMaximum();
  }
  else {
      ymax = 1.2*clonedSecondary->GetMaximum();   
  }
  cout<<"Primary Max: "<< clonedPrimary->GetMaximum() << " Secondary Max: "<< clonedSecondary->GetMaximum() <<endl;
  cout<<"Proper plot maximum set to "<<ymax<<endl;
  
  delete clonedSecondary;
  delete clonedPrimary;
  
  
  TH2F* h_axes = new TH2F(Form("%s_axes",plot_name.Data()),plot_title,p_hist->GetNbinsX(),xmin,xmax,1000,0.001,ymax);
  
  
  //-----------------------
  // AXES FIX
  //-----------------------
  
  cout<<"Setting axis names"<<endl;
  h_axes->GetXaxis()->SetTitle(xlabel);
  h_axes->GetYaxis()->SetTitle(ylabel);
  

  TString stat_string_1, stat_string_2, stat_string_3;

  //===========================
  // Print Closure Stats
  //===========================
  if (conf->get("print_stats") == "true")
  {
    int low_val = stoi(conf->get("stats_low_val"));
    int high_val = stoi(conf->get("stats_high_val"));

    Double_t p_evts_gtr150_err, s_evts_gtr150_err; 
    double p_evts_gtr150 = p_hist->IntegralAndError(p_hist->FindBin(low_val), p_hist->FindBin(high_val-.001), p_evts_gtr150_err);
    double s_evts_gtr150 = s_hist->IntegralAndError(s_hist->FindBin(low_val), s_hist->FindBin(high_val-.001), s_evts_gtr150_err);
    double ratio_evts_gtr150 = p_evts_gtr150/s_evts_gtr150;
    
    stat_string_1 = "Number of Events in "+primary_name+" from "+conf->get("stats_low_val")+" to "+conf->get("stats_high_val")+" : "+to_string(p_evts_gtr150)+" Error: "+to_string(p_evts_gtr150_err);

    stat_string_2 = "Number of Events in "+secondary_name+" from "+conf->get("stats_low_val")+" to "+conf->get("stats_high_val")+" : "+to_string(s_evts_gtr150)+" Error: "+to_string(s_evts_gtr150_err);

    stat_string_3 = "Ratio: "+to_string(ratio_evts_gtr150)+" Error : "+to_string(errMult(p_evts_gtr150, s_evts_gtr150, p_evts_gtr150_err, s_evts_gtr150_err, ratio_evts_gtr150));


    cout<<f_primary->GetName()<<" STATS: "<<stat_string_1<<endl;
    cout<<f_primary->GetName()<<" STATS: "<<stat_string_2<<endl;
    cout<<f_primary->GetName()<<" STATS: "<<stat_string_3<<endl;
  }
  
  //----------------------
  // ADD OVERFLOW BIN
  //----------------------
  if (conf->get("overflow")=="true"){
    cout<<"Plot tagged for overflow bin, building..."<<endl;
    double n_bins = p_hist->GetNbinsX();
    
    double overflow_primary = p_hist->GetBinContent(n_bins + 1);
    double overflow_secondary = s_hist->GetBinContent(n_bins + 1);

    double max_primary = p_hist->Integral(p_hist->FindBin(xmax-.001), n_bins);
    double max_secondary = s_hist->Integral(s_hist->FindBin(xmax-.001), n_bins);

    p_hist->SetBinContent(p_hist->FindBin(xmax-.001), max_primary+overflow_primary);
    s_hist->SetBinContent(s_hist->FindBin(xmax-.001), max_secondary+overflow_secondary);
  }
  
      
  
  plotpad->SetLeftMargin(0.15);
  h_axes->GetYaxis()->SetTitleOffset(1.3);
  h_axes->GetYaxis()->SetTitleSize(0.05);
  h_axes->GetYaxis()->SetLabelSize(0.04);
  
  cout<<"Drawing histograms"<<endl;
  h_axes->Draw();
  s_hist->Draw("HIST SAME");
  p_hist->Draw("E1 SAME");

  plotpad->RedrawAxis();
  
  TLegend *l1;
  l1 = new TLegend(0.73, 0.73, 0.88, 0.88);
  
  l1->SetLineColor(kWhite);  
  l1->SetShadowColor(kWhite);
  l1->SetFillColor(kWhite);
  l1->AddEntry(p_hist, primary_name, "p");
  l1->AddEntry(s_hist, secondary_name, "f");
  
  l1->Draw("same");
  
  //--------------------------
  // Fill in Residual Plot
  //--------------------------
  
  cout<<"Getting ready for residual plots"<<endl;
  fullpad->cd();
  TPad *ratiopad = new TPad("ratiopad", "ratiopad" ,0.,0.,1,0.21);
  ratiopad->SetTopMargin(0.05);
  ratiopad->SetLeftMargin(0.15);
  ratiopad->SetBottomMargin(0.1);
  ratiopad->SetRightMargin(0.05);
  ratiopad->SetGridy();  // doesn't actually appear for some reason..
  ratiopad->Draw();
  ratiopad->cd();
  
  TH1D* residual = (TH1D*) p_hist->Clone("residual");
  residual->Divide(s_hist);
  
  //cout<<"Fixing error bars"<<endl;
  //for (int count=1; count<=mc_sum->GetNbinsX(); count++){ 
  //  double relative_error = (mc_sum->GetBinError(count))/ (mc_sum->GetBinContent(count));
  //  residual->SetBinError(count, residual->GetBinContent(count)*relative_error);
  //}
  
  cout<<"Building axes"<<endl;
  TH1D* h_axis_ratio = new TH1D(Form("%s_residual_axes",plot_name.Data()),"",residual->GetNbinsX(),xmin,xmax);
  
  h_axis_ratio->GetYaxis()->SetTitleOffset(0.33);
  h_axis_ratio->GetYaxis()->SetTitleSize(0.18);
  h_axis_ratio->GetYaxis()->SetNdivisions(5);
  h_axis_ratio->GetYaxis()->SetLabelSize(0.15);
  //h_axis_ratio->GetYaxis()->SetRangeUser(0.5,1.5);
  h_axis_ratio->GetYaxis()->SetRangeUser(0.001,2.0);
  h_axis_ratio->GetYaxis()->SetTitle("Data/MC");
  h_axis_ratio->GetXaxis()->SetTickLength(0.07);
  h_axis_ratio->GetXaxis()->SetTitleSize(0.);
  h_axis_ratio->GetXaxis()->SetLabelSize(0.);
  
  TLine* line1 = new TLine(xmin,1,xmax,1);
  line1->SetLineStyle(2);
  
  cout<<"Drawing ratio plot"<<endl;
  h_axis_ratio->Draw("axis");
  line1->Draw("same");
  residual->Draw("same");
  
  c->Update();
  c->cd();
  
  //Draw luminosity and CMS tag
  if (conf->get("luminosity_fb") != ""){
    plotpad->cd();
    drawCMSLatex(stod(conf->get("luminosity_fb")));
  }
  
  drawLatexFromTString(stat_string_1, .52,.5);
  drawLatexFromTString(stat_string_2, .52, .52);
  drawLatexFromTString(stat_string_3, .52, .54);

  cout<<"Saving..."<<endl;
  c->SaveAs(save_dir+plot_name+TString(".pdf"));
  c->SaveAs(save_dir+plot_name+TString(".png"));
  //c->SaveAs(save_dir+plot_name+TString(".root"));
  //c->SaveAs(save_dir+plot_name+TString(".C"));
  
  cout<<"Cleaning up plot variables"<<endl;
  delete l1;
  delete p_hist;
  delete s_hist;
  delete residual;
  delete ratiopad;
  delete plotpad;
  delete fullpad;
  delete c;
  
  f_primary->Close();
  delete f_primary;
  f_secondary->Close();
  delete f_secondary;

  return errors;
}

TString drawSingleTH1(ConfigParser *conf){
  /* This method expects conf to have a plot config loaded in already. */
  TString errors="";

  TFile* f_primary = new TFile(TString(conf->get("histogram_path")));

  cout << "Found files "<<endl;

  TString plot_name = conf->get("plot_name");
  TString plot_title = conf->get("title");
  double xmax = stod(conf->get("xmax"));
  double xmin = stod(conf->get("xmin"));
  double bin_size = stod(conf->get("bin_size"));
  TString hist_name=conf->get("hist_name");
  TString sample_name=conf->get("sample_name");
  TString xlabel=conf->get("xlabel");
  TString ylabel=conf->get("ylabel");
  TString save_dir=conf->get("save_dir");


  cout << "Making Plots for: "<<plot_name<<endl;

  TH1D* p_hist = (TH1D*) ((TH1D*) f_primary->Get(sample_name+"_"+hist_name))->Clone("phist_"+plot_name);
  cout<<hist_name<<" found in "<<f_primary->GetName()<<endl;


  cout << "Histograms pulled from files, adding draw options"<<endl;
  
  //============================================
  // Draw Data-MC Plots
  //============================================
  
  TCanvas * c = new TCanvas("c","",2000,2000);
  c->cd();
  gPad->SetRightMargin(0.05);
  gPad->Modified();
  gStyle->SetOptStat(kFALSE);
  TPad *fullpad = new TPad("fullpad", "fullpad", 0,0,1,1);
  
  fullpad->Draw();
  fullpad->cd();
    
  fullpad->SetRightMargin(0.05);
  if (conf->get("ExtraRightMargin") == "true")
  {
    fullpad->SetRightMargin(0.08);
  }
  fullpad->SetBottomMargin(0.12);
  
  fullpad->Draw();
  fullpad->cd();
  
  if (conf->get("logy") == "true")
  {
    cout<<"Plot tagged for log y-axis"<<endl;
    fullpad->SetLogy();
  }
  
  p_hist->Rebin(bin_size);
  
  //===========================
  // SET MC COLORS
  //===========================
  
  p_hist->SetFillColor(kAzure+5);
  p_hist->SetFillStyle(1001);

  //===========================
  // Find Plot Maxima
  //===========================
  
  double ymax = 0;

  ymax = 1.2*p_hist->GetMaximum();

  
  cout<<"Proper plot maximum set to "<<ymax<<endl;
  
  TH2F* h_axes = new TH2F(Form("%s_axes",plot_name.Data()),plot_title,p_hist->GetNbinsX(),xmin,xmax,1000,0.001,ymax);
  
  
  //-----------------------
  // AXES FIX
  //-----------------------
  
  cout<<"Setting axis names"<<endl;
  h_axes->GetXaxis()->SetTitle(xlabel);
  h_axes->GetYaxis()->SetTitle(ylabel);
  
  TString stat_string_1;

  //===========================
  // Print Closure Stats
  //===========================
  if (conf->get("print_stats") == "true")
  {
    int low_val = stoi(conf->get("stats_low_val"));
    int high_val = stoi(conf->get("stats_high_val"));

    Double_t p_evts_gtr150_err;
    double p_evts_gtr150 = p_hist->IntegralAndError(p_hist->FindBin(low_val), p_hist->FindBin(high_val-.001), p_evts_gtr150_err);
    
    stat_string_1 = "Number of Events in "+hist_name+" from "+conf->get("stats_low_val")+" to "+conf->get("stats_high_val")+" : "+to_string(p_evts_gtr150)+" Error: "+to_string(p_evts_gtr150_err);


    cout<<f_primary->GetName()<<" STATS: "<<stat_string_1<<endl;
  }
  
  //----------------------
  // ADD OVERFLOW BIN
  //----------------------
  if (conf->get("overflow")=="true"){
    cout<<"Plot tagged for overflow bin, building..."<<endl;
    double n_bins = p_hist->GetNbinsX();
    
    double overflow_primary = p_hist->GetBinContent(n_bins + 1);

    double max_primary = p_hist->Integral(p_hist->FindBin(xmax) - 1, n_bins);

    p_hist->SetBinContent(p_hist->FindBin(xmax) - 1, max_primary+overflow_primary);
  }      
  
  fullpad->SetLeftMargin(0.15);
  h_axes->GetYaxis()->SetTitleOffset(1.3);
  h_axes->GetYaxis()->SetTitleSize(0.05);
  h_axes->GetYaxis()->SetLabelSize(0.03);
  
  cout<<"Drawing histograms"<<endl;
  h_axes->Draw();
  p_hist->Draw("HIST SAME");
  
  fullpad->RedrawAxis();
  

  if (conf->get("luminosity_fb") != ""){
    fullpad->cd();
    drawCMSLatex(stod(conf->get("luminosity_fb")));
  }
  
  drawLatexFromTString(stat_string_1, .52,.5);

  cout<<"Saving..."<<endl;
  c->SaveAs(save_dir+plot_name+TString(".pdf"));
  c->SaveAs(save_dir+plot_name+TString(".png"));
  //c->SaveAs(save_dir+plot_name+TString(".root"));
  //c->SaveAs(save_dir+plot_name+TString(".C"));
  
  cout<<"Cleaning up plot variables"<<endl;
  delete p_hist;
  delete fullpad;
  delete c;

  f_primary->Close();
  delete f_primary;

  return errors;
}

TString drawCutDebug(ConfigParser *conf){
  /* This method expects conf to have a plot config loaded in already. */
  TString errors="";

  TFile* f_primary = new TFile(TString(conf->get("histogram_path")));

  cout << "Found files "<<endl;

  TString plot_name = conf->get("plot_name");
  TString plot_title = conf->get("title");
  double xmax = stod(conf->get("xmax"));
  double xmin = stod(conf->get("xmin"));
  double bin_size = stod(conf->get("bin_size"));
  TString hist_name=conf->get("hist_name");
  TString sample_name=conf->get("sample_name");
  TString xlabel=conf->get("xlabel");
  TString ylabel=conf->get("ylabel");
  TString save_dir=conf->get("save_dir");


  cout << "Making Plots for: "<<plot_name<<endl;

  TH1D* p_hist = (TH1D*) ((TH1D*) f_primary->Get(sample_name+"_"+hist_name))->Clone("phist_"+plot_name);
  cout<<hist_name<<" found in "<<f_primary->GetName()<<endl;


  cout << "Histograms pulled from files, adding draw options"<<endl;
  
  //============================================
  // Draw Data-MC Plots
  //============================================
  
  TCanvas * c = new TCanvas("c","",2000,2000);
  c->cd();
  gPad->SetRightMargin(0.05);
  gPad->Modified();
  gStyle->SetOptStat(kFALSE);
  TPad *fullpad = new TPad("fullpad", "fullpad", 0,0,1,1);
  
  fullpad->Draw();
  fullpad->cd();
    
  fullpad->SetRightMargin(0.05);
  if (conf->get("ExtraRightMargin") == "true")
  {
    fullpad->SetRightMargin(0.08);
  }
  fullpad->SetBottomMargin(0.3);
  
  fullpad->Draw();
  fullpad->cd();
  
  if (conf->get("logy") == "true")
  {
    cout<<"Plot tagged for log y-axis"<<endl;
    fullpad->SetLogy();
  }
  
  p_hist->Rebin(bin_size);
  
  //===========================
  // SET MC COLORS
  //===========================
  
  p_hist->SetFillColor(kAzure+5);
  p_hist->SetFillStyle(1001);

  //===========================
  // Find Plot Maxima
  //===========================
  
  double ymax = 0;

  ymax = 1.2*p_hist->GetMaximum();

  cout<<"Proper plot maximum set to "<<ymax<<endl;
  
  TH2F* h_axes = new TH2F(Form("%s_axes",plot_name.Data()),plot_title,p_hist->GetNbinsX(),xmin,xmax,1000,0.001,ymax);
  
  
  //-----------------------
  // AXES FIX
  //-----------------------
  
  cout<<"Setting axis names"<<endl;
  h_axes->GetXaxis()->SetTitle(xlabel);
  h_axes->GetYaxis()->SetTitle(ylabel);
  
  
  //----------------------
  // ADD OVERFLOW BIN
  //----------------------
  if (conf->get("overflow")=="true"){
    cout<<"Plot tagged for overflow bin, building..."<<endl;
    double n_bins = p_hist->GetNbinsX();
    
    double overflow_primary = p_hist->GetBinContent(n_bins + 1);

    double max_primary = p_hist->Integral(p_hist->FindBin(xmax) - 1, n_bins);

    p_hist->SetBinContent(p_hist->FindBin(xmax) - 1, max_primary+overflow_primary);
  }

  //----------------------
  // SET AXIS LABELS
  //----------------------
  ConfigParser label_conf(conf->get("labels_file"));
  label_conf.loadConfig("Error Labels");
  TString bin_label;
  for (int i = xmin; i<xmax; i++)
  {
    bin_label=label_conf[to_string(i)];
    bin_label+=" ("+to_string((int) p_hist->GetBinContent(h_axes->FindBin(i)))+")";
    h_axes->GetXaxis()->SetBinLabel(h_axes->FindBin(i), bin_label);
  }  
  h_axes->GetXaxis()->LabelsOption("v");
  h_axes->GetXaxis()->SetLabelSize(.015);
  
  fullpad->SetLeftMargin(0.15);
  h_axes->GetYaxis()->SetTitleOffset(1.3);
  h_axes->GetYaxis()->SetTitleSize(0.05);
  h_axes->GetYaxis()->SetLabelSize(0.04);
  
  cout<<"Drawing histograms"<<endl;
  h_axes->Draw();
  p_hist->Draw("HIST SAME");
  
  fullpad->RedrawAxis();
  
  cout<<"Saving..."<<endl;
  c->SaveAs(save_dir+plot_name+TString(".pdf"));
  c->SaveAs(save_dir+plot_name+TString(".png"));
  //c->SaveAs(save_dir+plot_name+TString(".root"));
  //c->SaveAs(save_dir+plot_name+TString(".C"));
  
  cout<<"Cleaning up plot variables"<<endl;
  delete p_hist;
  delete fullpad;
  delete c;

  f_primary->Close();
  delete f_primary;

  return errors;
}

void drawPlots(TString config_file)
{
  vector<TString> plot_names;
  TString errors="";
  

  ConfigParser *configs=new ConfigParser(config_file.Data());
  
  TGaxis::SetExponentOffset(-0.07, 0, "y"); // X and Y offset for Y axis
  TGaxis::SetExponentOffset(-.8, -0.07, "x"); // X and Y offset for X axis


  while(configs->loadNextConfig()) {
    if (configs->get("PLOT_TYPE") == "ratio"){
      errors=drawArbitraryNumberWithResidual(configs);
    }
    else if (configs->get("PLOT_TYPE") == "single"){
      errors=drawSingleTH1(configs);
    }
    else if (configs->get("PLOT_TYPE") == "debug"){
      errors=drawCutDebug(configs);
    }
  }
  
  cout<<errors<<endl;
  return;
}

