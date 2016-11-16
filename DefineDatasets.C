#include "TChain.h"
#include "TString.h"

TChain * getTChain(TString data_set) {
  TChain *ch = new TChain("t");

//====================================
// DY MC
//====================================
  if (data_set.Contains("ZMC-inclusive")){
    //Inclusive HT
    //cout<<"Adding ZMC-inclusive"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/dy_m50_mgmlm_ext1*");
  }

  if (data_set.Contains("ZMC-100200")){
    //HT 100-200 
    //cout<<"Adding ZMC-100200"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/dy_m50_mgmlm_ht100_ext1*");
  }

  if (data_set.Contains("ZMC-200400")){
    //HT 200-400 
    //cout<<"Adding ZMC-200400"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/dy_m50_mgmlm_ht200_ext1*");
  }

  if (data_set.Contains("ZMC-400600")){
    //HT 400-600 
    //cout<<"Adding ZMC-400600"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/dy_m50_mgmlm_ht400_ext1*");
  }

  if (data_set.Contains("ZMC-600Inf")){
    //HT 600-Inf 
    //cout<<"Adding ZMC-600Inf"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/dy_m50_mgmlm_ht600_ext1*");
  }

  if (data_set.Contains("ZMC-fullmc")){
    //ZZ Sample
    //cout<<"Adding ZMC-100200"<<endl;
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/zz_2l2q_amcnlo*.root" ));
    
    //WZ Sample
    //cout<<"Adding ZMC-100200"<<endl;
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wz_2l2q_amcnlo*.root" ));
  }

//====================================
// SUSY MC
//====================================
  if (data_set.Contains("TChiHZ")){
    //TChiHZ Signal
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-22-02/tchihz_80x_v2*.root");
  }

//====================================
// Gamma Jets MC
//====================================
  if (data_set.Contains("GammaMC-40100") && (!data_set.Contains("REALMETISO"))){
    //HT 40-100 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/gjetsht40_dr0p05_mgmlm*");
  }
  if (data_set.Contains("GammaMC-100200") && (!data_set.Contains("REALMETISO"))){
    //HT 100-200 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/gjetsht100_dr0p05_mgmlm*");
  }
  if (data_set.Contains("GammaMC-200400") && (!data_set.Contains("REALMETISO"))){
    //HT 200-400 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/gjetsht200_dr0p05_mgmlm*");
  }
  if (data_set.Contains("GammaMC-400600") && (!data_set.Contains("REALMETISO"))){
    //HT 400-600 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/gjetsht400_dr0p05_mgmlm*");
  }
  if (data_set.Contains("GammaMC-600Inf") && (!data_set.Contains("REALMETISO"))){
    //HT 600-Inf 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/gjetsht600_dr0p05_mgmlm*");
  }

  if (data_set.Contains("GammaMC-wjets-inclusive")){ 
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_incl_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht100_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht200_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht400_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht600_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht800_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht1200_mgmlm*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht2500_mgmlm*");

  }
  if (data_set.Contains("GammaMC-gammaZtoNuNu")){
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/znunugamma*.root");
  }

  if (data_set.Contains("GammaMC-GammaRealMET")){
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
  if (data_set.Contains("FSMC-dilep")){
    //cout<<"Adding Dilepton Sample"<<endl;
    //ch_fs->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_dilep_mgmlm*.root"));
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_dilep_powheg*.root"));
  }

  //cout<<"Entries: "<<ch_fs->GetEntries()<<endl;

  if (data_set.Contains("FSMC-1lep")){
    //cout<<"Adding 1 lepton Sample"<<endl;
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltbr_mgmlm*.root"));
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltop_mgmlm*.root"));
  }
  
  //cout<<"Entries: "<<ch_fs->GetEntries()<<endl;

  if (data_set.Contains("FSMC-singletop")){
    //cout<<"Adding Single Top Sample"<<endl;
    //ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/sttw_antitop_powheg*.root"));
    //ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/sttw_top_powheg*.root"));
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/sttw_antitop_nofullhaddecay_powheg.root"));
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/sttw_top_nofullhaddecay_powheg.root"));
  }

  //cout<<"Entries: "<<ch_fs->GetEntries()<<endl;
  
  if (data_set.Contains("FSMC-ww")){
    //cout<<"Adding WW Sample"<<endl;
    ch->Add(Form("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ww_2l2nu_powheg*.root"));
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/www_incl_amcnlo*.root");
  }

  if (data_set.Contains("FSMC-ttw")){
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttw_ln_amcnlo*.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttw_qq_amcnlo*.root");
  }

//====================================
// Rare MC
//====================================

  if (data_set.Contains("RareMC-wz")){
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wz_3lnu_powheg*.root");
  }

  if (data_set.Contains("RareMC-zz")) {
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/zz_2l2n_powheg*.root");
  }

  if (data_set.Contains("RareMC-vvv")){
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wwz_incl_amcnlo*.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wzz_incl_amcnlo*.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/zzz_incl_amcnlo*.root");
  }

  if (data_set.Contains("RareMC-ttv")){
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttz_2l2n_amcnlo*.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttz_qq_amcnlo*.root");
  }

//====================================
// Photon Data
//====================================

  //Single Photon Trigger
  if (data_set.Contains("GammaData-SinglePhoton")){
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016B_Prompt_ph_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016C_Prompt_ph_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016D_Prompt_ph_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016E_Prompt_ph_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016F_Prompt_ph_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016G_Prompt_ph_v1*");    
    
    //EWK Subtraction
    if (data_set.Contains("GammaData-EWKSub")){
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wgjets_incl_amcnlo*.root");
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltbr_mgmlm*.root");
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltop_mgmlm*.root");
      ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/znunugamma_ptg130_mgmlm*.root");
    }
  }

  if (data_set.Contains("GammaData-GammaRealMET")){
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
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht100_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht1200_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht200_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht2500_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht400_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht600_amcnlo.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wjets_ht800_amcnlo.root");
  }
  if (data_set.Contains("GammaData-JustWGjetRealMET")){
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/wgjets_incl_amcnlo.root");
  }
  if (data_set.Contains("GammaData-JustTTBarRealMET")){
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltbr_mgmlm.root");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/ttbar_1ltop_mgmlm.root");
  }

//====================================
// Leptonic Data
//====================================

  if (data_set.Contains("DileptonData-ee")){
    cout<<"Adding EE Trigger Data"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016B_Prompt_ee_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016C_Prompt_ee_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016D_Prompt_ee_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016E_Prompt_ee_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016F_Prompt_ee_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016G_Prompt_ee_v1*");
  }

  if (data_set.Contains("DileptonData-em")){
    cout<<"Adding EMu Trigger Data"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016B_Prompt_em_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016C_Prompt_em_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016D_Prompt_em_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016E_Prompt_em_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016F_Prompt_em_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016G_Prompt_em_v1*");
  }

  if (data_set.Contains("DileptonData-mm")){
    cout<<"Adding MuMu Trigger Data"<<endl;
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016B_Prompt_mm_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016C_Prompt_mm_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016D_Prompt_mm_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016E_Prompt_mm_v2*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016F_Prompt_mm_v1*");
    ch->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-10/data_2016G_Prompt_mm_v1*");
  }

//====================================
// End Building TChain
//====================================

  return ch;
} 
