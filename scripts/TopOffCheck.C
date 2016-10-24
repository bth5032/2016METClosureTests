#include "TString.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"

void TopOffCheck(TString dir_loc="/nfs-7/userdata/bobak/ZMET2016_Hists_PostICHEP/", TString out_dir="/home/users/bhashemi/public_html/ZMET2016_PostICHEP/TopOffCheck/"){
	TFile *f_data = TFile::Open(dir_loc+"Data/ct_Z_Base_EdgeZ.root");
	TFile *f_MC = TFile::Open(dir_loc+"ct_Z_Base_EdgeZ.root");
	double sf = 0;
//------------------
// nVert Checking
//------------------
	TH1D *h_nVert_d = (TH1D*) ((TH1D*) (f_data->Get("zjets_nVert")))->Clone("h_nVert_d");
	TH1D *h_nVert_mc = (TH1D*) ((TH1D*) (f_MC->Get("zjets_nVert")))->Clone("h_nVert_mc");

	TCanvas * c_nVert = new TCanvas("c_nVert", "", 2000, 2000);

	h_nVert_d->SetTitle("Number of Verticies Data Vs. MC in Zjets.");
	h_nVert_d->Draw("E1");
	sf = h_nVert_d->Integral()/h_nVert_mc->Integral();
	h_nVert_mc->Scale(sf);
	h_nVert_mc->SetFillColor(9);
	h_nVert_mc->SetLineColor(9);
	h_nVert_mc->Draw("HIST SAME");


	TLegend *l_nVert;
	l_nVert = new TLegend(0.73, 0.73, 0.88, 0.88);

	l_nVert->SetLineColor(kWhite);  
	l_nVert->SetShadowColor(kWhite);
	l_nVert->SetFillColor(kWhite);
	l_nVert->AddEntry(h_nVert_d, "Data", "p");
	l_nVert->AddEntry(h_nVert_mc, "MC", "f");

	c_nVert->SaveAs(out_dir+"nvert.png");
}