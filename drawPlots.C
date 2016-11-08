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

#include "/home/users/bhashemi/Projects/GIT/Software/tableMaker/CTable.cpp"

#include "computeErrors.C"
#include "ConfigParser.C"


using namespace std;

std::array<int, 14> ROOT_COLOR_PALATE = {46,8,9,38,40,2,30,6,28,42,3,5,7,41};

TString parseLatex(TString opt){
  opt.ReplaceAll("\\","#");
  return opt;
}

vector<double> parseVector(TString opt){
  vector<double> ret;
  TString token;
  Ssiz_t from=0;
  //cout<<"got vector in string form: "<<opt<<endl;
  while(opt.Tokenize(token, from, "[,]")){
    token.ReplaceAll("[", "");
    token.ReplaceAll("]", "");
    token.ReplaceAll(",", "");
    token.ReplaceAll(" ", "");
    //cout<<"token: "<<token<<endl;
    ret.push_back(stod(token.Data()));
  }
  return ret;
}

double errMult(double A, double B, double errA, double errB, double C) {
  return sqrt(C*C*(pow(errA/A,2) + pow(errB/B,2)));
}

//____________________________________________________________________________
// returns the error on C = A/(A+B) 
// note that if A and B are integers, simplifies to sqrt((C * (1-C)) / (A+B))
// or thinking of an efficiency, sqrt((eff * (1-eff)) / N)
float err_binomial(float A, float B, float errA, float errB) {
 return (1/pow(A+B,2)) * sqrt(pow(B*errA,2) + pow(A*errB,2));
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

bool TH1DIntegralSort(TH1D* hist_1, TH1D* hist_2){
  return (hist_1->Integral() < hist_2->Integral()) ;
}

void drawCMSLatex(double luminosity){
  TLatex *lumitex = NULL;
  double height=1.01-gPad->GetTopMargin();
  float left_margin = gPad->GetLeftMargin();

  // lumitex = new TLatex(0.66,0.955, Form("%.1f fb^{-1} (13 TeV)", luminosity) );
  if (luminosity < 1){  
    lumitex = new TLatex(.9-left_margin, height , Form("%.1f pb^{-1} (13 TeV)", luminosity*1000) );    
  }
  else{  
    lumitex = new TLatex(.9-left_margin, height , Form("%.1f fb^{-1} (13 TeV)", luminosity) );    
  }    
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
  double bin_size;
  if (conf->get("bin_size") != ""){
    bin_size = stod(conf->get("bin_size"));
  }
  else{
    bin_size=1;
  }
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
    hist_labels[i]=parseLatex(conf->get("hist_"+to_string(i)+"_label"));    
  }  

  //Set prefix for retriving histogram
  vector<TString> hist_prefix (num_hists);
  for (int i = 0; i<num_hists; i++){
    hist_prefix[i]=conf->get("hist_"+to_string(i)+"_prefix");    
  }  

  cout<<"Hist names set"<<endl;
  TString xlabel=parseLatex(conf->get("xlabel"));
  TString ylabel=parseLatex(conf->get("ylabel"));
  TString save_dir=conf->get("save_dir");
  TString plot_title=parseLatex(conf->get("title"));


  cout << "Making Plots for: "<<plot_name<<endl;

  vector<TH1D*> hists (num_hists);
  for (int i = 0; i<num_hists; i++){
    hists[i] = (TH1D*) ((TH1D*) hist_files[i]->Get(hist_prefix[i]+"_"+hist_names[i]))->Clone("hist_"+to_string(i)+"_"+plot_name);
    cout<<hist_names[i]<<" found in "<<hist_files[i]->GetName()<<endl;
    cout<<hist_labels[i]<<" Bin 1 Content: "<<hists[i]->GetBinContent(1)<<endl;
  }  
  cout << "Histograms pulled from files, adding draw options"<<endl;
  
  //cout<<__LINE__<<endl;


  //============================================
  // Draw Data-MC Plots
  //============================================
  
  //cout<<__LINE__<<endl;
  TCanvas * c = new TCanvas("c","",2000,2000);
  c->cd();
  gPad->SetRightMargin(0.05);
  gPad->Modified();
  gStyle->SetOptStat(kFALSE);
  TPad *fullpad = new TPad("fullpad", "fullpad", 0,0,1,1);
  //cout<<__LINE__<<endl;
  fullpad->Draw();
  fullpad->cd();
  //cout<<__LINE__<<endl;
  TPad *plotpad = new TPad("plotpad", "plotpad",0,0.2,1.0,0.99);
  
  plotpad->SetRightMargin(0.05);
  if (conf->get("ExtraRightMargin") == "true")
  {
    plotpad->SetRightMargin(0.08);
  }
  plotpad->SetBottomMargin(0.12);
  //cout<<__LINE__<<endl;
  plotpad->Draw();
  plotpad->cd();
  //cout<<__LINE__<<endl;
  if (conf->get("logy") == "true")
  {
    cout<<"Plot tagged for log y-axis"<<endl;
    plotpad->SetLogy();
  }
  //cout<<__LINE__<<endl;
  if (conf->get("bin_size") != ""){
    for (int i = 0; i<num_hists; i++){
      hists[i]->Rebin(bin_size);
    }
  }
  else if (conf->get("binning") != ""){
    cout<<"here"<<endl;
    vector<double> binning = parseVector(conf->get("binning"));
    for (int i = 0; i<num_hists; i++){
      hists[i] = (TH1D*) hists[i]->Rebin(binning.size()-1, TString(hist_names[i]+"_rebin"), &binning[0]);
    }
  }

  //===========================
  // Normalize
  //===========================
  //cout<<__LINE__<<endl;
  TH1D* clonedBG_norm = NULL;
  TH1D* clonedPrimary_norm = (TH1D*) hists[0]->Clone("clonedPrimary_forNorm_"+plot_name);
  
  //Add scale factors like RSFOF
  for (int i=0; i < num_hists; i++){
    if (conf->get("hist_"+to_string(i)+"_scale") != ""){
      hists[i]->Scale(stod(conf->get("hist_"+to_string(i)+"_scale")));
    }
  }

  if (conf->get("normalize") == "true"){
    TString hist_nums_for_norm = conf->get("normalize_hist_nums");
    cout<<"Normalizing hists: "<<hist_nums_for_norm<<endl;
    //cout<<__LINE__<<endl;
    
    //This if statement is used when there is 
    //special normalization being done. This part of
    //the code makes a proxy for the BG sum and for the
    //primary hist (hists[0]). The primary proxy is just a clone,
    //the BG proxy is the sum of all the hists that are named in 
    //hist_nums_for_norm. If subrtact_non_normed is also true then
    //the hists not marked to be normalized will be subtracted from the 
    //primary before the scale factors are calculated.
    
    if (hist_nums_for_norm != ""){ //if not all hists to be normed.
      //cout<<__LINE__<<endl;
      for (int i=1; i<num_hists; i++){ //for each hist
        if (hist_nums_for_norm.Contains(to_string(i))){ //if hist is in list to be normed
          cout<<"Adding hist "<<i<<" to normalization BG."<<endl;
          //cout<<__LINE__<<endl;
          if (clonedBG_norm == NULL){ //Make new clonedBG
            //cout<<__LINE__<<endl;
            clonedBG_norm = (TH1D*) hists[i]->Clone("clonedBG_forNorm_"+plot_name);
            cout<<hist_labels[i]<<": original-num "<<hists[i]->GetBinContent(1)<<endl;
          }
          else{
            //cout<<__LINE__<<endl;
            clonedBG_norm->Add(hists[i]); //Add to clonedBG
            cout<<hist_labels[i]<<": original-num "<<hists[i]->GetBinContent(1)<<endl;
          }
          //cout<<__LINE__<<endl;
        }
      }
    }
    else{ //otherwise put all hists in clonedBG
      clonedBG_norm = (TH1D*) hists[1]->Clone("clonedBG_forNorm_"+plot_name);
      for (int i=2; i<num_hists; i++){
        clonedBG_norm->Add(hists[i]);
      }
    }
    //cout<<__LINE__<<endl;

    //check to make sure bg hist is not empty
    if (clonedBG_norm == NULL){
      return TString("Check the normalize_hist_nums opt, no hists in range labeled for normalization");
    }
    //cout<<__LINE__<<endl;

    if (conf->get("subtract_non_normed")=="true"){
      //cout<<__LINE__<<endl;
      for (int i=1; i<num_hists; i++){
        //cout<<__LINE__<<endl;
        if( ! hist_nums_for_norm.Contains(to_string(i))){
          //cout<<__LINE__<<endl;
          cout<<hist_labels[i]<<": subtracting "<<hists[i]->GetBinContent(1)<<endl;
          clonedPrimary_norm->Add(hists[i], -1); //subtract
        }
      }
    }

    //cout<<__LINE__<<endl;
    double numEventsData;
    double numEventsMC;
    double scaleFactor;
    if (conf->get("norm_0_50") == "true")
    {
      numEventsData = clonedPrimary_norm->Integral(clonedPrimary_norm->FindBin(0),clonedPrimary_norm->FindBin(49.9));
      numEventsMC = clonedBG_norm->Integral(clonedBG_norm->FindBin(0),clonedBG_norm->FindBin(49.9));
    }
    else{
      numEventsData = clonedPrimary_norm->Integral();
      numEventsMC = clonedBG_norm->Integral();
    }
    //cout<<__LINE__<<endl;
    cout<<"Num Events Primary: "<<numEventsData<<endl;
    cout<<"Num Events BG: "<<numEventsMC<<endl;
    //rescale everything to scale factor
    scaleFactor = ((double) numEventsData/numEventsMC);
    cout<<"Applying scale factor "<<scaleFactor<<" to MET templates hist."<<endl;
    for (int i = 1; i<num_hists; i++){
      if (hist_nums_for_norm.Contains(to_string(i)) || hist_nums_for_norm == "" ){
        hists[i]->Scale(scaleFactor);  //if hist is marked for norm or no hists marked for norms.
        cout<<hist_labels[i]<<" count: "<<hists[i]->GetBinContent(1)<<endl;
      }
    }
  }

  //Create sum of background samples
  TH1D *bg_sum = (TH1D*) hists[1]->Clone("bg_sum_"+plot_name);
  bg_sum->SetTitle("Sum of background samples");
  
  //cout<<__LINE__<<endl;
  for (int i=2; i<num_hists; i++){
    bg_sum->Add(hists[i]);
  }
  //cout<<__LINE__<<endl;

  for (int i = 0; i<num_hists; i++){
    cout<<hist_labels[i]<<": after-reweight "<<hists[i]->GetBinContent(1)<<endl;
  }

  delete clonedPrimary_norm;
  delete clonedBG_norm;
  //cout<<__LINE__<<endl;

  //===========================
  // SET MC COLORS
  //===========================
  //cout<<__LINE__<<endl;
  for (int i = 1; i<num_hists; i++){
    //cout<<__LINE__<<endl;
    hists[i]->SetFillColor(ROOT_COLOR_PALATE[(i-1) % ROOT_COLOR_PALATE.size()]);
    //cout<<__LINE__<<endl;
    hists[i]->SetFillStyle(1001);
  }
  //cout<<__LINE__<<endl;
  hists[0]->SetMarkerStyle(20);

  //===========================
  // BUILD LEGEND
  //===========================

  TLegend *l1;
  l1 = new TLegend(0.73, 0.73, 0.88, 0.88);
  
  l1->SetLineColor(kWhite);  
  l1->SetShadowColor(kWhite);
  l1->SetFillColor(kWhite);
  l1->SetTextSize(.02);
  //cout<<__LINE__<<endl;
  l1->AddEntry(hists[0], hist_labels[0], "p");
  for (int i = 1; i<num_hists; i++){
    l1->AddEntry(hists[i], hist_labels[i], "f");
  }
  //cout<<__LINE__<<endl;

  //===========================
  // MAKE STACK
  //===========================
  //Add all the background hists to a stack.
  THStack * stack = new THStack(("stack_"+conf->get("Name")).c_str(), conf->get("title").c_str());
  //cout<<__LINE__<<endl;
  

  sort(hists.begin()+1, hists.end(), TH1DIntegralSort);
  
  for (int i=1; i<num_hists; i++)
  {
    stack->Add(hists[i]);
  } 



  //===========================
  // Find Plot Maxima
  //===========================
  //cout<<__LINE__<<endl;
  double ymax = 0;
  double ymin = 0.1;
  TH1D* clonedBG = (TH1D*) bg_sum->Clone("clonedBG_forReweight_"+plot_name);
  TH1D* clonedPrimary = (TH1D*) hists[0]->Clone("clonedPrimary_forReweight_"+plot_name);
  //cout<<__LINE__<<endl;
  clonedPrimary->GetXaxis()->SetRangeUser(xmin, xmax);
  clonedBG->GetXaxis()->SetRangeUser(xmin,xmax);
  //cout<<__LINE__<<endl;
  if (conf->get("ymax") != ""){
    ymax = stod(conf->get("ymax"));
  }
  else{
    if (clonedBG->GetMaximum() < clonedPrimary->GetMaximum()){
        ymax = 1.2*clonedPrimary->GetMaximum();
    }
    else {
        ymax = 1.2*clonedBG->GetMaximum();   
    }
    if (conf->get("logy") == "true"){
      ymax *= 10;
    }
  }

  if (conf->get("ymin") != ""){
    ymin = stod(conf->get("ymin"));
  }
  //cout<<__LINE__<<endl;
  cout<<"Primary Max: "<< clonedPrimary->GetMaximum() << " Secondary Max: "<< clonedBG->GetMaximum() <<endl;
  cout<<"Proper plot maximum set to "<<ymax<<endl;
  
  delete clonedBG;
  delete clonedPrimary;
  //cout<<__LINE__<<endl;
  
  TH2F* h_axes = new TH2F(Form("%s_axes",plot_name.Data()),plot_title,hists[0]->GetNbinsX(),xmin,xmax,1000,ymin,ymax);
  
  
  //-----------------------
  // AXES FIX
  //-----------------------
  
  cout<<"Setting axis names"<<endl;
  h_axes->GetXaxis()->SetTitle(xlabel);
  h_axes->GetYaxis()->SetTitle(ylabel);
  //cout<<__LINE__<<endl;  

  gStyle->SetTitleW(0.6);
  gStyle->SetTitleH(0.06);
  gStyle->SetTitleFont(12);

  //===========================
  // Print Closure Stats
  //===========================


  if (conf->get("print_stats") == "true"){
    vector<pair<double,double>> stats_bins;
    int j = 0;
    
    while (conf->get("stats_"+to_string(j)+"_low_val") != "" ){
      stats_bins.push_back(make_pair(stod(conf->get("stats_"+to_string(j)+"_low_val")),stod(conf->get("stats_"+to_string(j)+"_high_val"))));
      j++;
    }

    if(conf->get("simple_errors") == "true"){
      vector<vector<pair<double, double>>> stats; //holds a pair of count error for each sample, and the bg sum
      double count, error;
      vector<pair<double,double>> stat_row;
      
      //cout<<__LINE__<<endl;
      //Loop over the stats bins
      // Build Table ========================================================
      for(int i = 0 ; i < (int)hists.size(); i++){
        for(int st_bin=0; st_bin < (int) stats_bins.size(); st_bin++){
          //cout<<__LINE__<<endl;
          count = hists[i]->IntegralAndError(hists[i]->FindBin(stats_bins[st_bin].first), hists[i]->FindBin(stats_bins[st_bin].second), error);
          stat_row.push_back(make_pair(count,error));
        }
        stats.push_back(stat_row);
        stat_row.clear();
      }
      //Tack on BG sum row
      for(int st_bin=0; st_bin < (int) stats_bins.size(); st_bin++){
        count = bg_sum->IntegralAndError(bg_sum->FindBin(stats_bins[st_bin].first), bg_sum->FindBin(stats_bins[st_bin].second), error);
        stat_row.push_back(make_pair(count,error)); 
      } 
      stats.push_back(stat_row);
      // End Table Building ==================================================

      // Print Table =========================================================
      CTable table;
      //Set Column Labels
      //cout<<__LINE__<<endl;
      table.setTitle(Form("Efficiencies for %s",plot_name.Data()));
      for (int st_bin=0; st_bin < (int) stats_bins.size(); st_bin++){
        //cout<<__LINE__<<endl;
        table.setColLabel(Form("%f-%f",stats_bins[st_bin].first, stats_bins[st_bin].second), st_bin);
      }
      //cout<<__LINE__<<endl;

      //Output Rows for samples
      for(int row = 0; row <= (int) hists.size(); row++ ){
        if (row == hists.size()){
          table.setRowLabel("Sum of BG", hists.size());  
        }
        else{
          table.setRowLabel(hist_labels[row], row);
        }
        for(int col=0; col < (int) stats_bins.size(); col++){
          //cout<<__LINE__<<endl;
          table.setCell(Form("%f+/-%f; Eff: %f", stats[row][col].first, stats[row][col].second, stats[row][col].first/stats[row][0].first), row, col);
        }
      }
      //cout<<__LINE__<<endl;

      table.setPrecision(2);

      table.print();
      table.saveTex(Form("efficiency_table_%s.tex", plot_name.Data()));

    }
    else{  
      double normalization = hists[0]->Integral(0,hists[0]->FindBin(49.9));

      vector<double> template_count;
      vector<double> template_error;


      double t_err; //placeholder for template error

      double r_err;

      vector<double> rare_count, TTV_count, VVV_count, WZ_count, ZZ_count;
      vector<double> rare_err, TTV_err, VVV_err, WZ_err, ZZ_err;

      vector<double> FS_count;

      for (int i=0; i < num_hists; i++){
        if (conf->get("hist_"+to_string(i)+"_scale") != ""){
          hists[i]->Scale(1/stod(conf->get("hist_"+to_string(i)+"_scale")));
        }
      }

      vector<double> signal_count;
      //cout<<__LINE__<<endl;

      //Fill in all the bin counts here
      for (int i = 0; i < stats_bins.size(); i++){
        signal_count.push_back(hists[0]->Integral(hists[0]->FindBin(stats_bins[i].first), hists[0]->FindBin(stats_bins[i].second - 0.001)));
        FS_count.push_back(hists[5]->Integral(hists[5]->FindBin(stats_bins[i].first), hists[5]->FindBin(stats_bins[i].second - 0.001)));
        
        //cout<<__LINE__<<endl;
        
        template_count.push_back(hists[6]->IntegralAndError(hists[6]->FindBin(stats_bins[i].first), hists[6]->FindBin(stats_bins[i].second - 0.001), t_err));
        template_error.push_back(t_err);
        
        //cout<<__LINE__<<endl;
        
        ZZ_err.push_back(0);
        WZ_err.push_back(0);
        VVV_err.push_back(0);
        TTV_err.push_back(0);

        ZZ_count.push_back(hists[1]->IntegralAndError(hists[1]->FindBin(stats_bins[i].first), hists[1]->FindBin(stats_bins[i].second - 0.001), ZZ_err[i]));

        //cout<<__LINE__<<endl;

        ZZ_count[i] += hists[2]->IntegralAndError(hists[2]->FindBin(stats_bins[i].first), hists[2]->FindBin(stats_bins[i].second - 0.001), r_err);
        ZZ_err[i] = sqrt(ZZ_err[i]*ZZ_err[i]+r_err*r_err);
        //WZ_count.push_back(hists[2]->IntegralAndError(hists[2]->FindBin(stats_bins[i].first), hists[2]->FindBin(stats_bins[i].second - 0.001), WZ_err[i]));

        //cout<<__LINE__<<endl;

        VVV_count.push_back(hists[3]->IntegralAndError(hists[3]->FindBin(stats_bins[i].first), hists[3]->FindBin(stats_bins[i].second - 0.001), VVV_err[i]));

        //cout<<__LINE__<<endl;

        TTV_count.push_back(hists[4]->IntegralAndError(hists[4]->FindBin(stats_bins[i].first), hists[4]->FindBin(stats_bins[i].second - 0.001), TTV_err[i]));
        //cout<<__LINE__<<endl;
      }

      //Compute Rare Sample Errors
      ZZ_err = getRareSamplesError(ZZ_err, ZZ_count);
      //WZ_err = getRareSamplesError(WZ_err, WZ_count);
      VVV_err = getRareSamplesError(VVV_err, VVV_count);
      TTV_err = getRareSamplesError(TTV_err, TTV_count);


      vector<double> temp_err = getMetTemplatesError(template_error, template_count, normalization, conf->get("SR"));
      //cout<<__LINE__<<endl;
      pair<vector<double>,vector<double>> FS_err = getFSError(FS_count, stod(conf->get("hist_5_scale")));
      //cout<<__LINE__<<endl;

      for (int i = 0; i < ZZ_err.size(); i++){
        //rare_count.push_back(ZZ_count[i]+WZ_count[i]+VVV_count[i]+TTV_count[i]);
        //rare_err.push_back(sqrt(ZZ_err[i]*ZZ_err[i] + WZ_err[i]*WZ_err[i] + VVV_err[i]*VVV_err[i] + TTV_err[i]*TTV_err[i]));

        rare_count.push_back(ZZ_count[i]+VVV_count[i]+TTV_count[i]);
        rare_err.push_back(sqrt(ZZ_err[i]*ZZ_err[i] + VVV_err[i]*VVV_err[i] + TTV_err[i]*TTV_err[i]));
      }

      printCounts(template_count, temp_err, rare_count, rare_err, FS_count, FS_err, stats_bins, signal_count, stod(conf->get("hist_5_scale")));
      printLatexCounts(template_count, temp_err, rare_count, rare_err, FS_count, FS_err, stats_bins, signal_count, stod(conf->get("hist_5_scale")));
      //cout<<__LINE__<<endl;

      for (int i=0; i < num_hists; i++){
        if (conf->get("hist_"+to_string(i)+"_scale") != ""){
          hists[i]->Scale(stod(conf->get("hist_"+to_string(i)+"_scale")));
        }
      }
    }
  }
  
  //----------------------
  // ADD OVERFLOW BIN
  //----------------------
  //cout<<__LINE__<<endl;
  if (conf->get("overflow")=="true"){
    cout<<"Plot tagged for overflow bin, building..."<<endl;
    double n_bins = hists[0]->GetNbinsX();
    double overflow, max;
    //cout<<__LINE__<<endl;
    for (int i = 0; i<num_hists; i++){
      overflow = hists[i]->GetBinContent(n_bins + 1);
      max = hists[i]->Integral(hists[i]->FindBin(xmax-.001), n_bins);
      hists[i]->SetBinContent(hists[i]->FindBin(xmax-.001), max+overflow);
    }
    overflow = bg_sum->GetBinContent(n_bins + 1);
    max = bg_sum->Integral(bg_sum->FindBin(xmax-.001), n_bins);
    bg_sum->SetBinContent(bg_sum->FindBin(xmax-.001), max+overflow);
    //cout<<__LINE__<<endl;
  }
  
      
  
  plotpad->SetLeftMargin(0.15);
  h_axes->GetYaxis()->SetTitleOffset(1.3);
  h_axes->GetYaxis()->SetTitleSize(0.05);
  h_axes->GetYaxis()->SetLabelSize(0.04);
  //cout<<__LINE__<<endl;
  cout<<"Drawing histograms"<<endl;
  h_axes->Draw();
  stack->Draw("HIST SAME");
  hists[0]->Draw("E1 SAME");

  plotpad->RedrawAxis();
  //cout<<__LINE__<<endl;

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
  //cout<<__LINE__<<endl;
  
  TH1D* residual = (TH1D*) hists[0]->Clone("residual");
  residual->Divide(bg_sum);
  //cout<<__LINE__<<endl;
  //cout<<"Fixing error bars"<<endl;
  //for (int count=1; count<=mc_sum->GetNbinsX(); count++){ 
  //  double relative_error = (mc_sum->GetBinError(count))/ (mc_sum->GetBinContent(count));
  //  residual->SetBinError(count, residual->GetBinContent(count)*relative_error);
  //}
  
  cout<<"Building axes"<<endl;
  TH1D* h_axis_ratio = new TH1D(Form("%s_residual_axes",plot_name.Data()),"",residual->GetNbinsX(),xmin,xmax);
  //cout<<__LINE__<<endl;
  h_axis_ratio->GetYaxis()->SetTitleOffset(0.33);
  h_axis_ratio->GetYaxis()->SetTitleSize(0.16);
  //h_axis_ratio->GetYaxis()->SetTitleFont(12);
  h_axis_ratio->GetYaxis()->SetNdivisions(5);
  h_axis_ratio->GetYaxis()->SetLabelSize(0.15);
  //h_axis_ratio->GetYaxis()->SetRangeUser(0.5,1.5);
  h_axis_ratio->GetYaxis()->SetRangeUser(0.001,2.0);
  
  if(conf->get("ratio_yaxis") != ""){
    h_axis_ratio->GetYaxis()->SetTitle(parseLatex(conf->get("ratio_yaxis")));  
  }
  else{
    h_axis_ratio->GetYaxis()->SetTitle("#frac{Signal}{Prediction}");
  }

  h_axis_ratio->GetXaxis()->SetTickLength(0.07);
  h_axis_ratio->GetXaxis()->SetTitleSize(0.005);
  h_axis_ratio->GetXaxis()->SetLabelSize(0.);

  h_axis_ratio->GetYaxis()->CenterTitle();
  
  //cout<<__LINE__<<endl;
  TLine* line1 = new TLine(xmin,1,xmax,1);
  line1->SetLineStyle(2);
  
  cout<<"Drawing ratio plot"<<endl;
  h_axis_ratio->Draw("axis");
  line1->Draw("same");
  residual->Draw("same");
  //cout<<__LINE__<<endl;
  c->Update();
  c->cd();
  
  //Draw luminosity and CMS tag
  if (conf->get("luminosity_fb") != ""){
    plotpad->cd();
    drawCMSLatex(stod(conf->get("luminosity_fb")));
  }
  //cout<<__LINE__<<endl;

  cout<<"Saving..."<<endl;
  c->SaveAs(save_dir+plot_name+TString(".pdf"));
  c->SaveAs(save_dir+plot_name+TString(".png"));
  //c->SaveAs(save_dir+plot_name+TString(".root"));
  //c->SaveAs(save_dir+plot_name+TString(".C"));
  //cout<<__LINE__<<endl;
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
  //cout<<__LINE__<<endl;
  for (int i = 0; i<num_hists; i++){
    hist_files[i]->Close();
  }
  hist_files.clear();
  //cout<<__LINE__<<endl;
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

