//====================================
// DY
//====================================
TChain * getZJetsChain(TString data_set){
  TChain *ch_Zjets = new TChain("t");

  if (data_set.Contains("inclusive")){
    //Inclusive HT
    ch_DY->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V07-06-09/zjets_m50_mgmlm.root");
    ch_DY->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V07-06-09/zjets_m50_mgmlm_1.root");
    ch_DY->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V07-06-09/zjets_m50_mgmlm_2.root");
    ch_DY->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V07-06-09/zjets_m50_mgmlm_3.root");
    ch_DY->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V07-06-09/zjets_m50_mgmlm_4.root");
    ch_DY->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V07-06-09/zjets_m50_mgmlm_5.root");
    ch_DY->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V07-06-09/zjets_m50_mgmlm_6.root");
    ch_DY->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V07-06-09/zjets_m50_mgmlm_7.root");
    ch_DY->Add("/nfs-7/userdata/ZMEToutput/output/ZMETbabies/V07-06-09/zjets_m50_mgmlm_8.root");
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
    //ch_Zjets->Add("/nfs-7/userdata/ZMEToutput/output/bobakBabies/V7680-hybrid-00-Bobak/zjets_m50_mgmlm_HT600Inf*");
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
