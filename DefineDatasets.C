#include "TChain.h"
#include "TString.h"

TChain * getTChain(TString data_set) {
  TChain *ch = new TChain("t");

//====================================
// DY MC
//====================================
  if (data_set.Contains("ZMC-inclusive")){
    //Inclusive HT
    cout<<"Adding ZMC-inclusive"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/dy_m50_mgmlm_ext1*");
  }

  if (data_set.Contains("ZMC-100200")){
    //HT 100-200 
    cout<<"Adding ZMC-100200"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/dy_m50_mgmlm_ht100_ext1*");
  }

  if (data_set.Contains("ZMC-200400")){
    //HT 200-400 
    cout<<"Adding ZMC-200400"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/dy_m50_mgmlm_ht200_ext1*");
  }

  if (data_set.Contains("ZMC-400600")){
    //HT 400-600 
    cout<<"Adding ZMC-400600"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/dy_m50_mgmlm_ht400_ext1*");
  }

  if (data_set.Contains("ZMC-600Inf")){
    //HT 600-Inf 
    cout<<"Adding ZMC-600Inf"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/dy_m50_mgmlm_ht600_ext1*");
  }

  if (data_set.Contains("ZMC-fullmc")){
    //ZZ Sample
    cout<<"Adding ZMC-100200"<<endl;
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/zz_2l2q_amcnlo*.root" ));
    
    //WZ Sample
    cout<<"Adding ZMC-100200"<<endl;
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wz_2l2q_amcnlo*.root" ));
  }
  if (data_set.Contains("ZMC-low-mass")){
    cout<<"Adding ZMC Low Mass sample"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/dy_m1050_mgmlm*.root");
  }

//====================================
// SUSY MC
//====================================
  if (data_set.Contains("TChiHZ")){
    //TChiHZ Signal
    cout<<"Adding TChiHZ Monte Carlo"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-02/tchihz_80x_v2*.root");
  }

//====================================
// Gamma Jets MC
//====================================
  if (data_set.Contains("GammaMC-40100")){
    //HT 40-100
    cout<<"Adding GammaMC-40100"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/gjetsht40_dr0p05_mgmlm*");
  }
  if (data_set.Contains("GammaMC-100200")){
    //HT 100-200 
    cout<<"Adding GammaMC-100200"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/gjetsht100_dr0p05_mgmlm*");
  }
  if (data_set.Contains("GammaMC-200400")){
    //HT 200-400
    cout<<"Adding GammaMC-200400"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/gjetsht200_dr0p05_mgmlm*");
  }
  if (data_set.Contains("GammaMC-400600")){
    //HT 400-600 
    cout<<"Adding GammaMC-400600"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/gjetsht400_dr0p05_mgmlm*");
  }
  if (data_set.Contains("GammaMC-600Inf")){
    //HT 600-Inf 
    cout<<"Adding GammaMC-600+"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/gjetsht600_dr0p05_mgmlm*");
  }

  if (data_set.Contains("GammaMC-justWJets")){ 
    cout<<"Adding GammaMC-wjets-inclusive"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_incl_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht100_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht200_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht400_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht600_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht800_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht1200_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht2500_mgmlm*");
  }

  if (data_set.Contains("GammaMC-WGamma")){ 
    //This set combines WJets with WGJets and is intended to take only prompt photons from WGjets and only non-prompt photons from WJets

    cout<<"Adding GammaMC-WGamma"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_incl_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht100_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht200_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht400_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht600_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht800_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht1200_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht2500_mgmlm*");

    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wgjets_incl_mgmlm*");

  }

  if (data_set.Contains("GammaMC-gammaZtoNuNu")){
    cout<<"Adding GammaMC-gammaZtoNuNu"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/znunugamma*.root");
  }

  if (data_set.Contains("GammaMC-wgjets-inclusive-amc")){
    cout<<"Adding GammaMC WGJets AMC@NLO Sample"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wgjets_incl_amcnlo*.root");
  }

  if (data_set.Contains("GammaMC-wgjets-inclusive-mgmlm")){
    cout<<"Adding GammaMC WGJets Madgraph Sample"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wgjets_incl_mgmlm*.root");
  }

  if (data_set.Contains("GammaMC-GammaRealMET")){
    cout<<"Adding GammaMC-GammaRealMET"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltbr_mgmlm.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltop_mgmlm.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wgjets_incl_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht100_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht1200_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht200_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht2500_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht400_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht600_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht800_amcnlo.root");
  }

//====================================
// Flavor Symmetric MC
//====================================
  if (data_set.Contains("FSMC-dilep-mgmlm")){
    cout<<"Adding FSMC-dilep-mgmlm (smaller stats sample)"<<endl; 
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_dilep_mgmlm*.root"));
  }

  if (data_set.Contains("FSMC-dilep-powheg")){
    cout<<"Adding FSMC-dilep-powheg (large stats sample)"<<endl; 
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_dilep_powheg*.root")); //larger stats
  }

  //cout<<"Entries: "<<ch_fs->GetEntries()<<endl;

  if (data_set.Contains("FSMC-1lep")){
    cout<<"Adding FSMC-1lep"<<endl; 
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltbr_mgmlm*.root"));
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltop_mgmlm*.root"));
  }
  
  //cout<<"Entries: "<<ch_fs->GetEntries()<<endl;

  if (data_set.Contains("FSMC-singletop-inclusive")){
    cout<<"Adding FSMC-singletop"<<endl; 
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/sttw_antitop_powheg*.root"));
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/sttw_top_powheg*.root"));
  }

  if (data_set.Contains("FSMC-singletop-nofullhaddecay")){
    cout<<"Adding FSMC-singletop with no fully hadronic decay"<<endl; 
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/sttw_antitop_nofullhaddecay_powheg.root"));
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/sttw_top_nofullhaddecay_powheg.root"));
  }

  //cout<<"Entries: "<<ch_fs->GetEntries()<<endl;
  
  if (data_set.Contains("FSMC-ww")){
    cout<<"Adding FSMC-ww"<<endl; 
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ww_2l2nu_powheg*.root"));
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/www_incl_amcnlo*.root");
  }

  if (data_set.Contains("FSMC-ttw")){
    cout<<"Adding FSMC-ttw"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttw_ln_amcnlo*.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttw_qq_amcnlo*.root");
  }

//====================================
// Rare MC
//====================================

  if (data_set.Contains("RareMC-wz")){
    cout<<"Adding RareMC-wz"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wz_3lnu_powheg*.root");
  }

  if (data_set.Contains("RareMC-zz")) {
    cout<<"Adding RareMC-zz"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/zz_2l2n_powheg*.root");
  }

  if (data_set.Contains("RareMC-vvv")){
    cout<<"Adding RareMC-vvv"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wwz_incl_amcnlo*.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wzz_incl_amcnlo*.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/zzz_incl_amcnlo*.root");
  }

  if (data_set.Contains("RareMC-ttz")){
    cout<<"Adding RareMC-ttz"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttz_2l2n_amcnlo_ext*.root");
    //ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttz_2l2n_amcnlo*.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttz_qq_amcnlo*.root");
  }

//====================================
// Photon Data
//====================================

  //Single Photon Trigger
  if (data_set.Contains("GammaData-SinglePhoton") && (! data_set.Contains("GammaData-SinglePhoton-Skimmed")) ){
    cout<<"Adding GammaData-SinglePhoton"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016B_23sep2016rereco_ph_v3*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016C_23sep2016rereco_ph_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016D_23sep2016rereco_ph_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016E_23sep2016rereco_ph_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016F_23sep2016rereco_ph_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016G_23sep2016rereco_ph_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_ph_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_ph_v3*");
    
    //EWK Subtraction
    if (data_set.Contains("GammaData-EWKSub")){
      cout<<"Adding EWK Subtraction Samples"<<endl;       
      //============
      // W+Gamma+Jets
      //============
      //This is the Wjets sample, it is intended to have events with a prompt photon vetod
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_incl_mgmlm*");
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht100_mgmlm*");
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht200_mgmlm*");
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht400_mgmlm*");
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht600_mgmlm*");
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht800_mgmlm*");
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht1200_mgmlm*");
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht2500_mgmlm*");

      //This is the W+Gamma+Jets, it is inteded to have events with non-prompt photons vetod
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wgjets_incl_mgmlm*");
      //============
      // Gamma+Z->NuNu
      //============
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/znunugamma_ptg130_mgmlm*.root");
      //============
      // TTbar
      //============
      //1lep
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltbr_mgmlm*.root");
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltop_mgmlm*.root");
      //dilep
      ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_dilep_mgmlm*.root"));
      //============
      // Single Top
      //============
      ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/sttw_antitop_nofullhaddecay_powheg.root"));
      ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/sttw_top_nofullhaddecay_powheg.root"));
    }
  }

  if (data_set.Contains("GammaData-GammaRealMET")){
    cout<<"Adding GammaData-GammaRealMET"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltbr_mgmlm.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltop_mgmlm.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wgjets_incl_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht100_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht1200_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht200_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht2500_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht400_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht600_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht800_amcnlo.root");
  }
  if (data_set.Contains("GammaData-JustWjetRealMET")){
    cout<<"Adding GammaData-JustWjetRealMET"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht100_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht1200_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht200_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht2500_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht400_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht600_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht800_amcnlo.root");
  }
  if (data_set.Contains("GammaData-JustWGjetRealMET")){
    cout<<"Adding GammaData-JustWGjetRealMET"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wgjets_incl_amcnlo.root");
  }
  if (data_set.Contains("GammaData-JustTTBarRealMET")){
    cout<<"Adding GammaData-JustTTBarRealMET"<<endl; 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltbr_mgmlm.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltop_mgmlm.root");
  }

//====================================
// Leptonic Data
//====================================
  if (data_set.Contains("DileptonData-ee") && (! data_set.Contains("DileptonData-ee-Skimmed")) ){
    cout<<"Adding EE Trigger Data"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016B_23sep2016rereco_ee_v3*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016C_23sep2016rereco_ee_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016D_23sep2016rereco_ee_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016E_23sep2016rereco_ee_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016F_23sep2016rereco_ee_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016G_23sep2016rereco_ee_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_ee_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_ee_v3*");
  }

  if (data_set.Contains("DileptonData-em") && (! data_set.Contains("DileptonData-em-Skimmed")) ){
    cout<<"Adding EMu Trigger Data"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016B_23sep2016rereco_em_v3*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016C_23sep2016rereco_em_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016D_23sep2016rereco_em_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016E_23sep2016rereco_em_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016F_23sep2016rereco_em_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016G_23sep2016rereco_em_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_em_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_em_v3*");
  }

  if (data_set.Contains("DileptonData-mm") && (! data_set.Contains("DileptonData-mm-Skimmed")) ){
    cout<<"Adding MuMu Trigger Data"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016B_23sep2016rereco_mm_v3*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016C_23sep2016rereco_mm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016D_23sep2016rereco_mm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016E_23sep2016rereco_mm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016F_23sep2016rereco_mm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016G_23sep2016rereco_mm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_mm_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_mm_v3*");
  }

  if (data_set.Contains("SingleLeptonData-SingleMu")){
    cout<<"Adding Single Muon Trigger Data"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016B_23sep2016rereco_sm_v3*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016C_23sep2016rereco_sm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016D_23sep2016rereco_sm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016E_23sep2016rereco_sm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016F_23sep2016rereco_sm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016G_23sep2016rereco_sm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_sm_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-05/data_2016H_Prompt_sm_v3*");
  }

//====================================
// Skimmed Leptonic Data
//====================================
  if (data_set.Contains("DileptonData-ee-Skimmed")){
    cout<<"Adding Skimmed EE Trigger Data"<<endl;
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016B_23sep2016rereco_ee_v3*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016C_23sep2016rereco_ee_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016D_23sep2016rereco_ee_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016E_23sep2016rereco_ee_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016F_23sep2016rereco_ee_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016G_23sep2016rereco_ee_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016H_Prompt_ee_v2*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016H_Prompt_ee_v3*");
  }

  if (data_set.Contains("DileptonData-em-Skimmed")){
    cout<<"Adding Skimmed EMu Trigger Data"<<endl;
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016B_23sep2016rereco_em_v3*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016C_23sep2016rereco_em_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016D_23sep2016rereco_em_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016E_23sep2016rereco_em_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016F_23sep2016rereco_em_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016G_23sep2016rereco_em_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016H_Prompt_em_v2*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016H_Prompt_em_v3*");
  }

  if (data_set.Contains("DileptonData-mm-Skimmed")){
    cout<<"Adding Skimmed MuMu Trigger Data"<<endl;
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016B_23sep2016rereco_mm_v3*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016C_23sep2016rereco_mm_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016D_23sep2016rereco_mm_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016E_23sep2016rereco_mm_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016F_23sep2016rereco_mm_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016G_23sep2016rereco_mm_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016H_Prompt_mm_v2*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016H_Prompt_mm_v3*");
  }

//====================================
// Skimmed Photon Data
//====================================

  //Single Photon Trigger
  if (data_set.Contains("GammaData-SinglePhoton-Skimmed")){
    cout<<"Adding GammaData-SinglePhoton-Skimmed"<<endl; 
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016B_23sep2016rereco_ph_v3*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016C_23sep2016rereco_ph_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016D_23sep2016rereco_ph_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016E_23sep2016rereco_ph_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016F_23sep2016rereco_ph_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016G_23sep2016rereco_ph_v1*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016H_Prompt_ph_v2*");
    ch->Add("/home/users/cwelke/ZMetbabyskims/V08-22-05/skim_data_2016H_Prompt_ph_v3*");
    
    //EWK Subtraction
    if (data_set.Contains("GammaData-EWKSub")){
      cout<<"Adding EWK Subtraction Samples"<<endl;       
      //============
      // W+Gamma+Jets
      //============
      //This is the Wjets sample, it is intended to have events with a prompt photon vetod
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_incl_mgmlm*");
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht100_mgmlm*");
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht200_mgmlm*");
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht400_mgmlm*");
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht600_mgmlm*");
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht800_mgmlm*");
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht1200_mgmlm*");
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wjets_ht2500_mgmlm*");

      //This is the W+Gamma+Jets, it is inteded to have events with non-prompt photons vetod
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-04/wgjets_incl_mgmlm*");
      //============
      // Gamma+Z->NuNu
      //============
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/znunugamma_ptg130_mgmlm*.root");
      //============
      // TTbar
      //============
      //1lep
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltbr_mgmlm*.root");
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltop_mgmlm*.root");
      //dilep
      ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_dilep_mgmlm*.root"));
      //============
      // Single Top
      //============
      ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/sttw_antitop_nofullhaddecay_powheg.root"));
      ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/sttw_top_nofullhaddecay_powheg.root"));
    }
  }

//====================================
// End Building TChain
//====================================

  return ch;
} 
