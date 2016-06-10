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
#include "TH1F.h"

#include "ConfigParser.C"

using namespace std;

TString drawTwoWithResidual(ConfigParser *conf){
  /* This method expects conf to have a plot config loaded in already. */
  TString errors="";

  TFile* f_primary = new TFile(conf->get("primary_path"));
  TFile* f_secondary = new TFile(conf->get("secondary_path"));

  cout << "Found files "<<endl;

  TString plot_name = conf->get("plot_name");
  double xmax = stod(conf->get("xmax"));
  double xmin = stod(conf->get("xmin"));
  double bin_size = stod(conf->get("bin_size"));
  TString hist_name=conf->get("hist_name");
  TString p_name=conf->get("primary_name");
  TString s_name=conf->get("secondary_name");
  TString xlabel=conf->get("xlabel");
  TString ylabel=conf->get("ylabel");
  TString plot_title=conf->get("title");


  cout << "Making Plots for: "<<plot_name<<endl;

  TH1F* p_hist = (TH1F*) ((TH1F*) f_primary->Get(primary_name+"_"+hist_name))->Clone("phist_"+plot_name);
  cout<<hist_name<<" found in "<<f_primary->GetName()<<endl;

  TH1F* s_hist = (TH1F*) ((TH1F*) f_secondary->Get(secondary_name+"_"+hist_name))->Clone("shist_"+plot_name);
  cout<<hist_name<<" found in "<<f_secondary->GetName()<<endl;

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
  // Normalize MC
  //===========================
  double numEventsData = p_hist->Integral(0,-1);
  double numEventsMC = s_hist->Integral(0,-1);
  double scaleFactor = ((double) numEventsData/numEventsMC);
  
  s_hist->Scale(scaleFactor);
  
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
  TH1F* clonedSecondary = (TH1F*) s_hist->Clone("clonedSecondary_forReweight_"+plot_name);
  TH1F* clonedPrimary = (TH1F*) p_hist->Clone("clonedPrimary_forReweight_"+plot_name);
  
  clonedSecondary->GetXaxis()->SetRangeUser(xmin, xmax);
  clonedPrimary->GetXaxis()->SetRangeUser(xmin,xmax);
  
  if (clonedSecondary->GetMaximum() < clonedPrimary->GetMaximum()){
      ymax = 1.2*clonedPrimary->GetMaximum();
  }
  else {
      ymax = 1.2*clonedSecondary->GetMaximum();   
  }
  
  delete clonedSecondary;
  delete clonedPrimary;
  
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
    double overflow_secondary = s_hist->GetBinContent(n_bins + 1);

    double max_primary = p_hist->Integral(p_hist->FindBin(xmax) - 1, n_bins);
    double max_secondary = s_hist->Integral(data->FindBin(xmax) - 1, n_bins);

    p_hist->SetBinContent(p_hist->FindBin(xmax) - 1, max_primary+overflow_primary);
    s_hist->SetBinContent(s_hist->FindBin(xmax) - 1, max_secondary+overflow_secondary);
  }
  
      
  
  plotpad->SetLeftMargin(0.15);
  h_axes->GetYaxis()->SetTitleOffset(1.3);
  h_axes->GetYaxis()->SetTitleSize(0.05);
  h_axes->GetYaxis()->SetLabelSize(0.04);
  
  cout<<"Drawing histograms"<<endl;
  h_axes->Draw();
  stack->Draw("HIST SAME");
  data->Draw("E1 SAME");
  
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
  
  TH1F* residual = (TH1F*) data->Clone("residual");
  residual->Divide(mc_sum);
  
  /*cout<<"Fixing error bars"<<endl;
  for (int count=1; count<=mc_sum->GetNbinsX(); count++){ 
    double relative_error = (mc_sum->GetBinError(count))/ (mc_sum->GetBinContent(count));
    residual->SetBinError(count, residual->GetBinContent(count)*relative_error);
  }*/
  
  cout<<"Building axes"<<endl;
  TH1F* h_axis_ratio = new TH1F(Form("%s_residual_axes",plot_name.Data()),"",residual->GetNbinsX(),xmin,xmax);
  
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
  
  cout<<"Saving..."<<endl;
  c->SaveAs(save_dir+plot_name+TString(".pdf"));
  c->SaveAs(save_dir+plot_name+TString(".png"));
  //c->SaveAs(save_dir+plot_name+TString(".root"));
  //c->SaveAs(save_dir+plot_name+TString(".C"));
  
  cout<<"Cleaning up plot variables"<<endl;
  delete l1;
  delete mc_sum;
  delete stack;
  delete zjets;
  delete fsbkg;
  //delete extra;
  delete data;
  delete residual;
  delete ratiopad;
  delete plotpad;
  delete fullpad;
  delete c;
  
  cout<<"next sample...\n"<<endl;
  
  cout<<"Cleaning up file variables"<<endl;
  f_DY->Close();
  f_TTbar->Close();
  if (do_extra) {
    f_ST->Close();
  f_VVV->Close();
  f_WW->Close();
  f_WZ->Close();
  f_ZZ->Close();
  }
  delete f_DY;
  delete f_TTbar;
  
  if (do_extra) {
  delete f_ST;
  delete f_VVV;
  delete f_WW;
  delete f_WZ;
  delete f_ZZ;
  }

  f_data->Close();
  delete f_data;
  return errors;
}

TString drawSingleTH1(ConfigParser *conf){
  /* This method expects conf to have a plot config loaded in already. */
  TString errors="";

  TFile* f_primary = new TFile(conf->get("histogram_path"));

  cout << "Found files "<<endl;

  TString plot_name = conf->get("plot_name");
  double xmax = stod(conf->get("xmax"));
  double xmin = stod(conf->get("xmin"));
  double bin_size = stod(conf->get("bin_size"));
  TString hist_name=conf->get("hist_name");
  TString xlabel=conf->get("xlabel");
  TString ylabel=conf->get("ylabel");
  TString save_dir=conf->get("save_dir");


  cout << "Making Plots for: "<<plot_name<<endl;

  TH1F* p_hist = (TH1F*) ((TH1F*) f_primary->Get(primary_name+"_"+hist_name))->Clone("phist_"+plot_name);
  cout<<hist_name<<" found in "<<f_primary->GetName()<<endl;

  TH1F* s_hist = (TH1F*) ((TH1F*) f_secondary->Get(secondary_name+"_"+hist_name))->Clone("shist_"+plot_name);
  cout<<hist_name<<" found in "<<f_secondary->GetName()<<endl;

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
  // Normalize MC
  //===========================
  double numEventsData = p_hist->Integral(0,-1);
  double numEventsMC = s_hist->Integral(0,-1);
  double scaleFactor = ((double) numEventsData/numEventsMC);
  
  s_hist->Scale(scaleFactor);
  
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
  TH1F* clonedSecondary = (TH1F*) s_hist->Clone("clonedSecondary_forReweight_"+plot_name);
  TH1F* clonedPrimary = (TH1F*) p_hist->Clone("clonedPrimary_forReweight_"+plot_name);
  
  clonedSecondary->GetXaxis()->SetRangeUser(xmin, xmax);
  clonedPrimary->GetXaxis()->SetRangeUser(xmin,xmax);
  
  if (clonedSecondary->GetMaximum() < clonedPrimary->GetMaximum()){
      ymax = 1.2*clonedPrimary->GetMaximum();
  }
  else {
      ymax = 1.2*clonedSecondary->GetMaximum();   
  }
  
  delete clonedSecondary;
  delete clonedPrimary;
  
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
    double overflow_secondary = s_hist->GetBinContent(n_bins + 1);

    double max_primary = p_hist->Integral(p_hist->FindBin(xmax) - 1, n_bins);
    double max_secondary = s_hist->Integral(data->FindBin(xmax) - 1, n_bins);

    zjets->SetBinContent(zjets->FindBin(xmax) - 1, max_zjets+overflow_zjets);
    gjets->SetBinContent(data->FindBin(xmax) - 1, max_data+overflow_data);
  }
  
      
  
  plotpad->SetLeftMargin(0.15);
  h_axes->GetYaxis()->SetTitleOffset(1.3);
  h_axes->GetYaxis()->SetTitleSize(0.05);
  h_axes->GetYaxis()->SetLabelSize(0.04);
  
  cout<<"Drawing histograms"<<endl;
  h_axes->Draw();
  stack->Draw("HIST SAME");
  data->Draw("E1 SAME");
  
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
  
  TH1F* residual = (TH1F*) data->Clone("residual");
  residual->Divide(mc_sum);
  
  /*cout<<"Fixing error bars"<<endl;
  for (int count=1; count<=mc_sum->GetNbinsX(); count++){ 
    double relative_error = (mc_sum->GetBinError(count))/ (mc_sum->GetBinContent(count));
    residual->SetBinError(count, residual->GetBinContent(count)*relative_error);
  }*/
  
  cout<<"Building axes"<<endl;
  TH1F* h_axis_ratio = new TH1F(Form("%s_residual_axes",plot_name.Data()),"",residual->GetNbinsX(),xmin,xmax);
  
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
  
  cout<<"Saving..."<<endl;
  c->SaveAs(save_dir+plot_name+TString(".pdf"));
  c->SaveAs(save_dir+plot_name+TString(".png"));
  //c->SaveAs(save_dir+plot_name+TString(".root"));
  //c->SaveAs(save_dir+plot_name+TString(".C"));
  
  cout<<"Cleaning up plot variables"<<endl;
  delete l1;
  delete mc_sum;
  delete stack;
  delete zjets;
  delete fsbkg;
  //delete extra;
  delete data;
  delete residual;
  delete ratiopad;
  delete plotpad;
  delete fullpad;
  delete c;
  
  cout<<"next sample...\n"<<endl;
  
  cout<<"Cleaning up file variables"<<endl;
  f_DY->Close();
  f_TTbar->Close();
  if (do_extra) {
    f_ST->Close();
  f_VVV->Close();
  f_WW->Close();
  f_WZ->Close();
  f_ZZ->Close();
  }
  delete f_DY;
  delete f_TTbar;
  
  if (do_extra) {
  delete f_ST;
  delete f_VVV;
  delete f_WW;
  delete f_WZ;
  delete f_ZZ;
  }

  f_data->Close();
  delete f_data;
  return errors;
}

void drawPlots(TString config_file)
{
  vector<TString> plot_names;
  TString errors="";
  
  ConfigParser *singleplot_configs=new ConfigParser(config_file);
  
  while(singleplot_configs->loadNextConfig()) {
    drawSingleTH1(singleplot_configs);
  }
  
  cout<<errors<<endl;
  return;
}

