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
 
  if (data_set.Contains("40100")){
    //HT 40-100 
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/bobakBabies/V7680-hybrid-00-Bobak/Gjets_mgmlm_HT40100*");
  }
  if (data_set.Contains("100200")){
    //HT 100-200 
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/bobakBabies/V7680-hybrid-00-Bobak/Gjets_mgmlm_HT100200*");
  }
  if (data_set.Contains("200400")){
    //HT 200-400 
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/bobakBabies/V7680-hybrid-00-Bobak/Gjets_mgmlm_HT200400*");
  }
  if (data_set.Contains("400600")){
    //HT 400-600 
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/bobakBabies/V7680-hybrid-00-Bobak/Gjets_mgmlm_HT400600*");
  }
  if (data_set.Contains("600Inf")){
    //HT 600-Inf 
    ch_Gjets->Add("/nfs-7/userdata/ZMEToutput/output/bobakBabies/V7680-hybrid-00-Bobak/Gjets_mgmlm_HT600Inf*");
  }
  return ch_Gjets;
}

//====================================
// Rare Samples
//====================================
TChain * getRareChain(TString data_set){
  TChain *ch_rares = new TChain("t");
 
  if (data_set.Contains("wz")){
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-07-00/wz_3lnu_powheg*.root");
  }

  if (data_set.Contains("zz")){
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-07-00/zz_2l2nu_powheg*.root");
  }

  if (data_set.Contains("vvv")){
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-07-00/www_incl_amcnlo*.root");
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-07-00/wwz_incl_amcnlo*.root");
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-07-00/wzz_incl_amcnlo*.root");
    ch_rares->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V08-07-00/zzz_incl_mgmlm*.root");
  }

  if (data_set.Contains("ttv")){
    ch_rares->Add("/hadoop/cms/store/user/cwelke/uaf8/dilepbabies/V08-07-00_ttw_ln_amcnlo/wjets*.root");
    ch_rares->Add("/hadoop/cms/store/user/cwelke/uaf8/dilepbabies/V08-07-00_ttz_llnn_amcnlo/unknown*.root");
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