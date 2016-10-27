#include "TString.h"
#include "TFile.h"
#include "TChain.h"

using namespace std;

void checkreco(){
	TChain *ch_ee_rereco = new TChain("t");
	TChain *ch_mm_rereco = new TChain("t");
	TChain *ch_em_rereco = new TChain("t");
	TChain *ch_ee_prompt = new TChain("t");
	TChain *ch_mm_prompt = new TChain("t");
	TChain *ch_em_prompt = new TChain("t");


	ch_ee_rereco->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016C_23sep2016rereco_ee_v1*.root");
	ch_mm_rereco->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016C_23sep2016rereco_mm_v1*.root");
	ch_em_rereco->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016C_23sep2016rereco_em_v1*.root");
	
	ch_ee_prompt->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016C_Prompt_ee_v2*.root");
	ch_mm_prompt->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016C_Prompt_mm_v2*.root");
	ch_em_prompt->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016C_Prompt_em_v2*.root");

	TH1D* h_ee_re = new TH1D("h_ee_re", "", 20,81,101);
	TH1D* h_mm_re = new TH1D("h_mm_re", "", 20,81,101);
	TH1D* h_em_re = new TH1D("h_em_re", "", 20,81,101);

	TH1D* h_ee_pr = new TH1D("h_ee_pr", "", 20,81,101);
	TH1D* h_mm_pr = new TH1D("h_mm_pr", "", 20,81,101);
	TH1D* h_em_pr = new TH1D("h_em_pr", "", 20,81,101);

	ch_ee_rereco->Draw("dilmass>>h_ee_re", "dilmass >= 81 && dilmass <= 101 && hyp_type == 0 && lep_pt[0] >= 25 && lep_pt[1] >= 20 && abs(lep_p4[0].eta()) < 2.4 && abs(lep_p4[1].eta()) < 2.4 && evt_type == 0 && (abs(abs(lep_p4[1].eta()) - 1.5) > 0.1)");
	ch_mm_rereco->Draw("dilmass>>h_ee_re", "dilmass >= 81 && dilmass <= 101 && hyp_type == 1 && lep_pt[0] >= 25 && lep_pt[1] >= 20 && abs(lep_p4[0].eta()) < 2.4 && abs(lep_p4[1].eta()) < 2.4 && evt_type == 0 && (abs(abs(lep_p4[1].eta()) - 1.5) > 0.1)");
	ch_em_rereco->Draw("dilmass>>h_ee_re", "dilmass >= 81 && dilmass <= 101 && hyp_type == 2 && lep_pt[0] >= 25 && lep_pt[1] >= 20 && abs(lep_p4[0].eta()) < 2.4 && abs(lep_p4[1].eta()) < 2.4 && evt_type == 0 && (abs(abs(lep_p4[1].eta()) - 1.5) > 0.1)");
	
	ch_ee_prompt->Draw("dilmass>>h_ee_re", "dilmass >= 81 && dilmass <= 101 && hyp_type == 0 && lep_pt[0] >= 25 && lep_pt[1] >= 20 && abs(lep_p4[0].eta()) < 2.4 && abs(lep_p4[1].eta()) < 2.4 && evt_type == 0 && (abs(abs(lep_p4[1].eta()) - 1.5) > 0.1)");
	ch_mm_prompt->Draw("dilmass>>h_ee_re", "dilmass >= 81 && dilmass <= 101 && hyp_type == 1 && lep_pt[0] >= 25 && lep_pt[1] >= 20 && abs(lep_p4[0].eta()) < 2.4 && abs(lep_p4[1].eta()) < 2.4 && evt_type == 0 && (abs(abs(lep_p4[1].eta()) - 1.5) > 0.1)");
	ch_em_prompt->Draw("dilmass>>h_ee_re", "dilmass >= 81 && dilmass <= 101 && hyp_type == 2 && lep_pt[0] >= 25 && lep_pt[1] >= 20 && abs(lep_p4[0].eta()) < 2.4 && abs(lep_p4[1].eta()) < 2.4 && evt_type == 0 && (abs(abs(lep_p4[1].eta()) - 1.5) > 0.1)");

	/*ch_ee_rereco->Draw("dilmass>>h_ee_re", "dilmass >= 81 && dilmass <= 101 && hyp_type == 0 && lep_pt[0] >= 25 && lep_pt[1] >= 20 && abs(lep_p4[0].eta()) < 2.4 && abs(lep_p4[1].eta()) < 2.4 && evt_type == 0 && ( ! abs(lep_p4[1].eta()) > 1.4 && abs(phys.lep_p4[1].eta()) < 1.6 ) && ( ! abs(lep_p4[0].eta()) > 1.4 && abs(phys.lep_p4[0].eta()) < 1.6 )");
	ch_mm_rereco->Draw("dilmass>>h_mm_re", "dilmass >= 81 && dilmass <= 101 && hyp_type == 1 && lep_pt[0] >= 25 && lep_pt[1] >= 20 && abs(lep_p4[0].eta()) < 2.4 && abs(lep_p4[1].eta()) < 2.4 && evt_type == 0 && ( ! abs(lep_p4[1].eta()) > 1.4 && abs(phys.lep_p4[1].eta()) < 1.6 ) && ( ! abs(lep_p4[0].eta()) > 1.4 && abs(phys.lep_p4[0].eta()) < 1.6 ) ");
	ch_em_rereco->Draw("dilmass>>h_em_re", "dilmass >= 81 && dilmass <= 101 && hyp_type == 2 && lep_pt[0] >= 25 && lep_pt[1] >= 20 && abs(lep_p4[0].eta()) < 2.4 && abs(lep_p4[1].eta()) < 2.4 && evt_type == 0 && ( ! abs(lep_p4[1].eta()) > 1.4 && abs(phys.lep_p4[1].eta()) < 1.6 ) && ( ! abs(lep_p4[0].eta()) > 1.4 && abs(phys.lep_p4[0].eta()) < 1.6 ) ");

	ch_ee_prompt->Draw("dilmass>>h_ee_pr", "dilmass >= 81 && dilmass <= 101 && hyp_type == 0 && lep_pt[0] >= 25 && lep_pt[1] >= 20 && abs(lep_p4[0].eta()) < 2.4 && abs(lep_p4[1].eta()) < 2.4 && evt_type == 0 && ( ! abs(lep_p4[1].eta()) > 1.4 && abs(phys.lep_p4[1].eta()) < 1.6 ) && ( ! abs(lep_p4[0].eta()) > 1.4 && abs(phys.lep_p4[0].eta()) < 1.6 ) ");
	ch_mm_prompt->Draw("dilmass>>h_mm_pr", "dilmass >= 81 && dilmass <= 101 && hyp_type == 1 && lep_pt[0] >= 25 && lep_pt[1] >= 20 && abs(lep_p4[0].eta()) < 2.4 && abs(lep_p4[1].eta()) < 2.4 && evt_type == 0 && ( ! abs(lep_p4[1].eta()) > 1.4 && abs(phys.lep_p4[1].eta()) < 1.6 ) && ( ! abs(lep_p4[0].eta()) > 1.4 && abs(phys.lep_p4[0].eta()) < 1.6 ) ");
	ch_em_prompt->Draw("dilmass>>h_em_pr", "dilmass >= 81 && dilmass <= 101 && hyp_type == 2 && lep_pt[0] >= 25 && lep_pt[1] >= 20 && abs(lep_p4[0].eta()) < 2.4 && abs(lep_p4[1].eta()) < 2.4 && evt_type == 0 && ( ! abs(lep_p4[1].eta()) > 1.4 && abs(phys.lep_p4[1].eta()) < 1.6 ) && ( ! abs(lep_p4[0].eta()) > 1.4 && abs(phys.lep_p4[0].eta()) < 1.6 ) ");*/

	cout<<"E E in Prompt: "<<h_ee_pr->Integral()<<" in Rereco: "<<h_ee_re->Integral()<<endl;
	cout<<"Mu Mu in Prompt: "<<h_mm_pr->Integral()<<" in Rereco: "<<h_mm_re->Integral()<<endl;
	cout<<"E Mu in Prompt: "<<h_em_pr->Integral()<<" in Rereco: "<<h_em_re->Integral()<<endl;
}