#include "TString.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"

void TopOffCheck(TString dir_loc="/nfs-7/userdata/bobak/ZMET2016_Hists_PostICHEP/", TString out_dir="/home/users/bhashemi/public_html/ZMET2016_PostICHEP/TopOffCheck/"){
	TFile *f_data = TFile::Open(dir_loc+"Data/ct_Z_Base_EdgeZ.root");
	TFile *f_MC = TFile::Open(dir_loc+"ct_Z_Base_EdgeZ.root");
	double sf = 0;

	gStyle->SetOptStat(kFALSE);

//------------------
// nVert Checking
//------------------
	TH1D *h_nVert_d = (TH1D*) ((TH1D*) (f_data->Get("zjets_nVert")))->Clone("h_nVert_d");
	TH1D *h_nVert_mc = (TH1D*) ((TH1D*) (f_MC->Get("zjets_nVert")))->Clone("h_nVert_mc");

	TCanvas * c_nVert = new TCanvas("c_nVert", "", 2000, 2000);

	h_nVert_d->SetMarkerStyle(20);
	
	sf = h_nVert_d->Integral()/h_nVert_mc->Integral();
	h_nVert_mc->Scale(sf);
	
	h_nVert_mc->SetTitle("Number of Verticies Data Vs. MC in Zjets.");
	h_njets_mc->SetAxisRange(0,50);
	h_nVert_mc->SetFillColor(9);
	h_nVert_mc->SetLineColor(9);
	
	h_nVert_mc->Draw("HIST");
	h_nVert_d->Draw("E1 SAME");

	TLegend *l_nVert;
	l_nVert = new TLegend(0.73, 0.73, 0.88, 0.88);

	l_nVert->SetLineColor(kWhite);  
	l_nVert->SetShadowColor(kWhite);
	l_nVert->SetFillColor(kWhite);
	l_nVert->AddEntry(h_nVert_d, "Data", "p");
	l_nVert->AddEntry(h_nVert_mc, "MC", "f");

	l_nVert->Draw("SAME");

	c_nVert->SaveAs(out_dir+"nvert.png");

//------------------
// nJets Checking
//------------------
	TH1D *h_njets_d = (TH1D*) ((TH1D*) (f_data->Get("zjets_njets")))->Clone("h_njets_d");
	TH1D *h_njets_mc = (TH1D*) ((TH1D*) (f_MC->Get("zjets_njets")))->Clone("h_njets_mc");

	TCanvas * c_njets = new TCanvas("c_njets", "", 2000, 2000);

	h_njets_d->SetMarkerStyle(20);
	
	sf = h_njets_d->Integral()/h_njets_mc->Integral();
	h_njets_mc->Scale(sf);
	
	h_njets_mc->SetTitle("Number of Jets Data Vs. MC in Zjets.");
	h_njets_mc->SetAxisRange(0,12);
	h_njets_mc->SetFillColor(9);
	h_njets_mc->SetLineColor(9);
	
	h_njets_mc->Draw("HIST SAME");
	h_njets_d->Draw("E1 SAME");

	TLegend *l_njets;
	l_njets = new TLegend(0.73, 0.73, 0.88, 0.88);

	l_njets->SetLineColor(kWhite);  
	l_njets->SetShadowColor(kWhite);
	l_njets->SetFillColor(kWhite);
	l_njets->AddEntry(h_njets_d, "Data", "p");
	l_njets->AddEntry(h_njets_mc, "MC", "f");

	l_njets->Draw("SAME");

	c_njets->SaveAs(out_dir+"njets.png");

//------------------
// HT Checking
//------------------
	TH1D *h_ht_d = (TH1D*) ((TH1D*) (f_data->Get("zjets_ht")))->Clone("h_ht_d");
	TH1D *h_ht_mc = (TH1D*) ((TH1D*) (f_MC->Get("zjets_ht")))->Clone("h_ht_mc");

	TCanvas * c_ht = new TCanvas("c_ht", "", 2000, 2000);

	h_ht_d->SetMarkerStyle(20);
	
	sf = h_ht_d->Integral()/h_ht_mc->Integral();
	h_ht_mc->Scale(sf);
	
	h_ht_mc->SetTitle("Number of Verticies Data Vs. MC in Zjets.");
	h_ht_mc->SetAxisRange(0,1000);
	h_ht_mc->SetFillColor(9);
	h_ht_mc->SetLineColor(9);
	
	h_ht_mc->Draw("HIST");
	h_ht_d->Draw("E1 SAME");

	TLegend *l_ht;
	l_ht = new TLegend(0.73, 0.73, 0.88, 0.88);

	l_ht->SetLineColor(kWhite);  
	l_ht->SetShadowColor(kWhite);
	l_ht->SetFillColor(kWhite);
	l_ht->AddEntry(h_ht_d, "Data", "p");
	l_ht->AddEntry(h_ht_mc, "MC", "f");

	l_ht->Draw("SAME");

	c_ht->SaveAs(out_dir+"ht.png");
}