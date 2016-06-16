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

TString drawTwoWithResidual(ConfigParser *conf){
  /* This method expects conf to have a plot config loaded in already. */
  TString errors="";

  TFile* f_primary = new TFile(TString(conf->get("primary_path")));
  TFile* f_secondary = new TFile(TString(conf->get("secondary_path")));

  cout << "Found files "<<endl;

  TString plot_name = conf->get("plot_name");
  double xmax = stod(conf->get("xmax"));
  double xmin = stod(conf->get("xmin"));
  double bin_size = stod(conf->get("bin_size"));
  TString hist_name=conf->get("hist_name");
  TString xlabel=conf->get("xlabel");
  TString ylabel=conf->get("ylabel");
  TString primary_name=conf->get("primary_name");
  TString secondary_name=conf->get("secondary_name");
  TString save_dir=conf->get("save_dir");
  TString plot_title=conf->get("title");


  cout << "Making Plots for: "<<plot_name<<endl;

  TH1D* p_hist = (TH1D*) ((TH1D*) f_primary->Get(primary_name+"_"+hist_name))->Clone("phist_"+plot_name);
  cout<<hist_name<<" found in "<<f_primary->GetName()<<endl;

  TH1D* s_hist = (TH1D*) ((TH1D*) f_secondary->Get(secondary_name+"_"+hist_name))->Clone("shist_"+plot_name);
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
  // Normalize
  //===========================
  if (conf->get("normalize") == "true")
  {
    double numEventsData = p_hist->Integral();
    double numEventsMC = s_hist->Integral();
    double scaleFactor = ((double) numEventsData/numEventsMC);
    
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
    double max_secondary = s_hist->Integral(s_hist->FindBin(xmax) - 1, n_bins);

    p_hist->SetBinContent(p_hist->FindBin(xmax) - 1, max_primary+overflow_primary);
    s_hist->SetBinContent(s_hist->FindBin(xmax) - 1, max_secondary+overflow_secondary);
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
  
  /*cout<<"Fixing error bars"<<endl;
  for (int count=1; count<=mc_sum->GetNbinsX(); count++){ 
    double relative_error = (mc_sum->GetBinError(count))/ (mc_sum->GetBinContent(count));
    residual->SetBinError(count, residual->GetBinContent(count)*relative_error);
  }*/
  
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
  h_axes->GetYaxis()->SetLabelSize(0.04);
  
  cout<<"Drawing histograms"<<endl;
  h_axes->Draw();
  p_hist->Draw("HIST SAME");
  
  fullpad->RedrawAxis();
  
  if (conf->get("luminosity_fb") != ""){
    fullpad->cd();
    drawCMSLatex(stod(conf->get("luminosity_fb")));
  }

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
    cout<<i<<": "<<bin_label<<endl;
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
  
  TGaxis::SetMaxDigits(1);
  TGaxis::SetExponentOffset(-0.07, -.99, "y"); // X and Y offset for Y axis
  TGaxis::SetExponentOffset(-.8, -0.07, "x"); // X and Y offset for X axis
  
  ConfigParser *configs=new ConfigParser(config_file.Data());
  
  while(configs->loadNextConfig()) {
    if (configs->get("PLOT_TYPE") == "ratio"){
      drawTwoWithResidual(configs);
    }
    else if (configs->get("PLOT_TYPE") == "single"){
      drawSingleTH1(configs);
    }
    else if (configs->get("PLOT_TYPE") == "debug"){
      drawCutDebug(configs);
    }
  }
  
  cout<<errors<<endl;
  return;
}

