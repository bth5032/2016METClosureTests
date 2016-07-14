#include "TChain.h"
#include "TString.h"

//====================================
// DY
//====================================
TChain * getZJetsChain(TString data_set) {
  TChain *ch_Zjets = new TChain("t");

  if (data_set.Contains("inclusive")){
    //Inclusive HT
    ch_Zjets->Add("/nfs-7/userdata/ZMEToutput/output/bobakBabies/V7680-hybrid-00-Bobak/zjets_m50_mgmlm_zjetsInclusive_*");
  }

  if (data_set.Contains("100200")){
    //HT 100-200 
    ch_Zjets->Add("/nfs-7/userdata/ZMEToutput/output/bobakBabies/V7680-hybrid-00-Bobak/zjets_m50_mgmlm_HT100200*");
  }

  if (data_set.Contains("200400")){
    //HT 200-400 
    ch_Zjets->Add("/nfs-7/userdata/ZMEToutput/output/bobakBabies/V7680-hybrid-00-Bobak/zjets_m50_mgmlm_HT200400*");
  }

  if (data_set.Contains("400600")){
    //HT 400-600 
    ch_Zjets->Add("/nfs-7/userdata/ZMEToutput/output/bobakBabies/V7680-hybrid-00-Bobak/zjets_m50_mgmlm_HT400600*");
  }

  if (data_set.Contains("600Inf")){
    //HT 600-Inf 
    ch_Zjets->Add("/nfs-7/userdata/ZMEToutput/output/bobakBabies/V7680-hybrid-00-Bobak/zjets_miniAODv2_m50_mgmlm_HT600Inf_ext1*");
    //Nothing here for now.
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
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/bobakBabies/V7680-hybrid-00-Bobak/Gjets_mgmlm_HT40100*");
  }
  if (data_set.Contains("100200") && (!data_set.Contains("REALMETISO"))){
    //HT 100-200 
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/bobakBabies/V7680-hybrid-00-Bobak/Gjets_mgmlm_HT100200*");
  }
  if (data_set.Contains("200400") && (!data_set.Contains("REALMETISO"))){
    //HT 200-400 
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/bobakBabies/V7680-hybrid-00-Bobak/Gjets_mgmlm_HT200400*");
  }
  if (data_set.Contains("400600") && (!data_set.Contains("REALMETISO"))){
    //HT 400-600 
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/bobakBabies/V7680-hybrid-00-Bobak/Gjets_mgmlm_HT400600*");
  }
  if (data_set.Contains("600Inf") && (!data_set.Contains("REALMETISO"))){
    //HT 600-Inf 
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/bobakBabies/V7680-hybrid-00-Bobak/Gjets_mgmlm_HT600Inf*");
  }

  if (data_set.Contains("GammaRealMET")){
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/ttbar_1ltbr_mgmlm.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/ttbar_1ltop_mgmlm.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wgjets_incl_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht100_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht1200_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht200_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht2500_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht400_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht600_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht800_amcnlo.root");
  }
  else if (data_set.Contains("JustWjetRealMET")){
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht100_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht1200_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht200_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht2500_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht400_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht600_amcnlo.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht800_amcnlo.root");
  }
  else if (data_set.Contains("JustWGjetRealMET")){
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wgjets_incl_amcnlo.root");
  }
  else if (data_set.Contains("JustTTBarRealMET")){
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/ttbar_1ltbr_mgmlm.root");
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/ttbar_1ltop_mgmlm.root");
  }
  return ch_Gjets;
}

//====================================
// Rare Samples
//====================================
TChain * getRareChain(TString data_set){
  TChain *ch_rares = new TChain("t");
 
  if (data_set.Contains("wz")){
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wz_3lnu_powheg*.root");
  }

  if (data_set.Contains("zz")) {
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/zz_2l2n_powheg*.root");
  }

  if (data_set.Contains("vvv")){
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/www_incl_mgmlm*.root");
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wwz_incl_mgmlm*.root");
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wzz_incl_mgmlm*.root");
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/zzz_incl_mgmlm*.root");
  }

  if (data_set.Contains("ttv")){
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/ttz_2l2n_amcnlo*.root");
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/ttz_qq_amcnlo*.root");
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
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-07-01/data_2016B_Prompt_ph_v1.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-07-01/data_2016B_Prompt_ph_v2.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-07-01/data_2016B_Prompt_ph_v2_1.root");
  }

    if (trigger.Contains("GammaRealMET")){
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/ttbar_1ltbr_mgmlm.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/ttbar_1ltop_mgmlm.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wgjets_incl_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht100_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht1200_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht200_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht2500_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht400_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht600_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht800_amcnlo.root");
  }
  else if (trigger.Contains("JustWjetRealMET")){
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht100_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht1200_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht200_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht2500_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht400_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht600_amcnlo.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wjets_ht800_amcnlo.root");
  }
  else if (trigger.Contains("JustWGjetRealMET")){
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/wgjets_incl_amcnlo.root");
  }
  else if (trigger.Contains("JustTTBarRealMET")){
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/ttbar_1ltbr_mgmlm.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-11-00/ttbar_1ltop_mgmlm.root");
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
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-07-01/data_2016B_Prompt_ee_v1*");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-07-01/data_2016B_Prompt_ee_v2*");
  }

  if (trigger.Contains("em")){
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-07-01/data_2016B_Prompt_em_v1*");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-07-01/data_2016B_Prompt_em_v2*");
  }

  if (trigger.Contains("mm")){
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-07-01/data_2016B_Prompt_mm_v1.root");
    ch_Data->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-07-01/data_2016B_Prompt_mm_v2.root");
  }

  return ch_Data;
}