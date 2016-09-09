#include "TChain.h"
#include "TString.h"

//====================================
// DY
//====================================
TChain * getZJetsChain(TString data_set) {
  TChain *ch_Zjets = new TChain("t");

  if (data_set.Contains("inclusive")){
    //Inclusive HT
    ch_Zjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/dy_m50_mgmlm_ext1*");
  }

  if (data_set.Contains("100200")){
    //HT 100-200 
    ch_Zjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/dy_m50_mgmlm_ht100_ext1*");
  }

  if (data_set.Contains("200400")){
    //HT 200-400 
    ch_Zjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/dy_m50_mgmlm_ht200_ext1*");
  }

  if (data_set.Contains("400600")){
    //HT 400-600 
    ch_Zjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/dy_m50_mgmlm_ht400_ext1*");
  }

  if (data_set.Contains("600Inf")){
    //HT 600-Inf 
    ch_Zjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/dy_m50_mgmlm_ht600_ext1*");
  }

  return ch_Zjets;
}

//====================================
// Gamma Jets
//====================================
TChain * getGJetsChain(TString data_set){
  TChain *ch_Gjets = new TChain("t");
 
  if (data_set.Contains("40100") && (!data_set.Contains("REALMETISO"))){
    //HT 40-100 
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/gjetsht40_dr0p4_mgmlm*");
  }
  if (data_set.Contains("100200") && (!data_set.Contains("REALMETISO"))){
    //HT 100-200 
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/gjetsht100_dr0p4_mgmlm*");
  }
  if (data_set.Contains("200400") && (!data_set.Contains("REALMETISO"))){
    //HT 200-400 
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/gjetsht200_dr0p4_mgmlm*");
  }
  if (data_set.Contains("400600") && (!data_set.Contains("REALMETISO"))){
    //HT 400-600 
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/gjetsht400_dr0p4_mgmlm*");
  }
  if (data_set.Contains("600Inf") && (!data_set.Contains("REALMETISO"))){
    //HT 600-Inf 
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/gjetsht600_dr0p4_mgmlm*");
  }

  if (data_set.Contains("GammaRealMET")){
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/ttbar_1ltbr_mgmlm.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/ttbar_1ltop_mgmlm.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wgjets_incl_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht100_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht1200_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht200_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht2500_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht400_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht600_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht800_amcnlo.root");
  }
  else if (data_set.Contains("JustWjetRealMET")){
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht100_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht1200_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht200_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht2500_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht400_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht600_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht800_amcnlo.root");
  }
  else if (data_set.Contains("JustWGjetRealMET")){
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wgjets_incl_amcnlo.root");
  }
  else if (data_set.Contains("JustTTBarRealMET")){
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/ttbar_1ltbr_mgmlm.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/ttbar_1ltop_mgmlm.root");
  }
  return ch_Gjets;
}

//====================================
// Rare Samples
//====================================
TChain * getRareChain(TString data_set){
  TChain *ch_rares = new TChain("t");
 
  if (data_set.Contains("wz")){
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-05_ichep/wz_3lnu_powheg*.root");
  }

  if (data_set.Contains("zz")) {
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-05_ichep/zz_2l2n_powheg*.root");
  }

  if (data_set.Contains("vvv")){
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-05_ichep/www_incl_amcnlo*.root");
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-05_ichep/wwz_incl_amcnlo*.root");
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-05_ichep/wzz_incl_amcnlo*.root");
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-05_ichep/zzz_incl_amcnlo*.root");
  }

  if (data_set.Contains("ttv")){
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-05_ichep/ttz_2l2n_amcnlo*.root");
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-05_ichep/ttz_qq_amcnlo*.root");
  }

  return ch_rares;
}

//====================================
// Photon Data
//====================================
TChain * getDataPhotonChain(TString trigger){
  TChain *ch_Data = new TChain("t");

  //Single Photon Trigger
  if (trigger.Contains("SinglePhoton")){
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/data_2016B_Prompt_ph_v2*");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/data_2016C_Prompt_ph_v2*");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/data_2016D_Prompt_ph_v2*");
    
    //EWK Subtraction
    if (trigger.Contains("EWKSub")){
      ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wgjets_incl_amcnlo*.root");
      ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/ttbar_1ltbr_mgmlm*.root");
      ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/ttbar_1ltop_mgmlm*.root");
      ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/znunugamma_ptg130_mgmlm*.root");
    }
  }

  if (trigger.Contains("GammaRealMET")){
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/ttbar_1ltbr_mgmlm.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/ttbar_1ltop_mgmlm.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wgjets_incl_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht100_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht1200_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht200_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht2500_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht400_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht600_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht800_amcnlo.root");
  }
  else if (trigger.Contains("JustWjetRealMET")){
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht100_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht1200_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht200_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht2500_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht400_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht600_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wjets_ht800_amcnlo.root");
  }
  else if (trigger.Contains("JustWGjetRealMET")){
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/wgjets_incl_amcnlo.root");
  }
  else if (trigger.Contains("JustTTBarRealMET")){
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/ttbar_1ltbr_mgmlm.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/ttbar_1ltop_mgmlm.root");
  }

  return ch_Data;
}

//====================================
// Leptonic Data
//====================================
TChain * getDataZChain(TString trigger){
  TChain *ch_Data = new TChain("t");

  // E/Mu Trigger
  if (trigger.Contains("ee")){
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/data_2016B_Prompt_ee_v2*");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/data_2016C_Prompt_ee_v2*");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/data_2016D_Prompt_ee_v2*");
  }

  if (trigger.Contains("em")){
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/data_2016B_Prompt_em_v2*");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/data_2016C_Prompt_em_v2*");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/data_2016D_Prompt_em_v2*");
  }

  if (trigger.Contains("mm")){
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/data_2016B_Prompt_mm_v2*");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/data_2016C_Prompt_mm_v2*");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-04_ichep/data_2016D_Prompt_mm_v2*");
  }

  return ch_Data;
}