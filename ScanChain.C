// Usage:
// > root -b doAll.C

//
// 2016 MET study looper. Written by Bobak Hashemi May 13 2016
//

// C++
#include <iostream>
#include <vector>
#include <set>
#include <utility>


// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"
#include "TEfficiency.h"
#include "TH1.h"
#include "TH2.h"

// ZMET2016
#include "ZMET2016.cc"

// CORE
//You can not include headers!!! This is not compiled code.
#include "CoreTools/dorky.cc"
#include "CoreTools/goodrun.cc"
#include "/home/users/bhashemi/Projects/GIT/CORE/Tools/MT2/MT2Utility.cc"
#include "/home/users/bhashemi/Projects/GIT/CORE/Tools/MT2/MT2.cc"
// Configuration parsing
#include "ConfigParser.C"
#include "ConfigHelper.C"

using namespace std;
using namespace zmet;
using namespace duplicate_removal;

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

//Global Vars
ConfigParser *conf;
int nDuplicates=0;

vector<pair <TH1D*, TString> > g_reweight_pairs;
TDirectory *rootdir = gDirectory->GetDirectory("Rint:");
TH1D *g_pileup_hist, *g_l1prescale_hist22, *g_l1prescale_hist30, *g_l1prescale_hist36;
TEfficiency *g_vpt_eff_barrel, *g_vpt_eff_endcap; 
TFile *g_weight_hist_file, *g_pileup_hist_file, *g_l1prescale_file;
TString g_sample_name;
TFile* currentFile = 0;

TH1I *numEvents; //Holds the number of events in the whole script and the number that pass various cuts 

//set<int> inVinceNotMine = {4589,8308,11893,14101,14746,19058,23180,28337,35374,42297,56194,60844,62242,64496,94423,95813,96364,97305,108841,115916,120306,127729,132182,137726,149809,151328,153127,182754,187629,191053,198339,202662,206473,214221,216796,221953,223390,225608,235943,244158,250884,251983,257113,259935,261309,270523,274748,277608,294101,316484,318876,318957,329729,330631,332962,333411,334846,340820,349907,351289,357272,357425,369136,370324,374306,374516,381210,383337,385393,390720,392522,396041,398289,413797,420484};

//set<int> inMineNotVince = {65603,206256,215885,352922,823174,928555,1058265,1295652,1601624,1731568,1948640};

//set<long> inspection_set = {470836340,726705664,1268760057,1113504849,173918891,3089654438};

bool printStats = false;
bool printFail = false;

pair<int, int> getMostBlike(){
  /* returns two most B-like jet indicies */
  int first_index = 0;
  int second_index = 1;

  if (phys.jets_csv().at(first_index) < phys.jets_csv().at(second_index)){
    first_index = 1;
    second_index = 0;
  }

  for (int i = 2; i < (int) phys.jets_p4().size(); i++){
    if (phys.jets_csv().at(first_index) < phys.jets_csv().at(i)){
      second_index = first_index;
      first_index = i;
    }
    else if (phys.jets_csv().at(second_index) < phys.jets_csv().at(i)){
      second_index = i;
    }
  }

  return make_pair(first_index, second_index);
}

pair<int,int> getClosestBPairToHiggsMass(){
  int first = 0;
  int second = 1;
  
  if (phys.jets_p4().size()<2){
    cout<<"Going to throw error finding closest B pair: Less than two jets!"<<endl;
  }

  double dist = abs((phys.jets_p4().at(0)+phys.jets_p4().at(1)).M() - 125.0);

  for (int i = 0; i < (int) phys.jets_p4().size(); i++) {
    for (int j = i+1; j < (int) phys.jets_p4().size(); j++) {
      if (abs((phys.jets_p4().at(i)+phys.jets_p4().at(j)).M() - 125 ) < dist){
        first = i;
        second = j;
      }
    }
  }

  return make_pair(first,second);
}

double getMT2ForBjets(bool select_highest_csv=false){
  /*This function gets the MT2 built out of the two Bjets in an event, no guarentee is made about selecting the highest csv jets*/
  double mt2;
  if (select_highest_csv){
    pair<int, int> b_index = getMostBlike();
    //make sure first index points to the higher csv of the first two jets
    mt2=MT2(phys.met_T1CHS_miniAOD_CORE_pt(), phys.met_T1CHS_miniAOD_CORE_phi(), phys.jets_p4().at(b_index.first), phys.jets_p4().at(b_index.second), 0, 0);
  }
  else{
    // MT2( MET_MAGNITUDE, MET_PHI, P4_LEPTON_1, P4_LEPTON_2, MASS_INVISIBLE_PARTICLE, Bool Verbose)
    mt2=MT2(phys.met_T1CHS_miniAOD_CORE_pt(), phys.met_T1CHS_miniAOD_CORE_phi(), phys.jets_medb_p4().at(0), phys.jets_medb_p4().at(1), 0, 0);
  }

  return mt2;
}

double getMT2HiggsZ(bool select_highest_closest_higgs_mass=false){

  double mt2; 

  if(select_highest_closest_higgs_mass){
    pair<int, int> jet_indexes = getClosestBPairToHiggsMass();
    // MT2( MET_MAGNITUDE, MET_PHI, P4_LEPTON_1, P4_LEPTON_2, MASS_INVISIBLE_PARTICLE, Bool Verbose)
    mt2=MT2(phys.met_T1CHS_miniAOD_CORE_pt(), phys.met_T1CHS_miniAOD_CORE_phi(), phys.jets_p4().at(jet_indexes.first)+phys.jets_p4().at(jet_indexes.second), lep_p4().at(0)+lep_p4().at(1), 0, 0);
  }
  else{
    // MT2( MET_MAGNITUDE, MET_PHI, P4_LEPTON_1, P4_LEPTON_2, MASS_INVISIBLE_PARTICLE, Bool Verbose)
    mt2=MT2(phys.met_T1CHS_miniAOD_CORE_pt(), phys.met_T1CHS_miniAOD_CORE_phi(), phys.jets_medb_p4().at(0)+phys.jets_medb_p4().at(1), lep_p4().at(0)+lep_p4().at(1), 0, 0);
  }

  return mt2;
}

double getPrescaleWeight(){
  //cout<<__LINE__<<endl;
  //cout<<"Getting Prescale Weights"<<endl;
  
  if( (phys.HLT_Photon165_R9Id90_HE10_IsoM() > 0 || phys.HLT_Photon165_HE10() > 0) && phys.gamma_pt().at(0) > 180. ) return 1;
  else if( phys.HLT_Photon120_R9Id90_HE10_IsoM() > 0 && phys.gamma_pt().at(0) > 135. ) return phys.HLT_Photon120_R9Id90_HE10_IsoM();
  else if( phys.HLT_Photon90_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 105. ) return phys.HLT_Photon90_R9Id90_HE10_IsoM();
  else if( phys.HLT_Photon75_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 85. ) return phys.HLT_Photon75_R9Id90_HE10_IsoM();
  else if( phys.HLT_Photon50_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 55. ) return phys.HLT_Photon50_R9Id90_HE10_IsoM();
  else if( phys.HLT_Photon36_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) < 55. && phys.gamma_pt().at(0) > 40. ) {
    return /*g_l1prescale_hist36->GetBinContent(g_l1prescale_hist36->FindBin(phys.nVert())) */ 166 /*134*/;
  }
  else if( phys.HLT_Photon30_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) < 40. && phys.gamma_pt().at(0) > 33. ){
    return /*g_l1prescale_hist30->GetBinContent(g_l1prescale_hist30->FindBin(phys.nVert())) */ 354 /*269*/;
  }
  else if( phys.HLT_Photon22_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) < 33. ) {
    return /*g_l1prescale_hist22->GetBinContent(g_l1prescale_hist22->FindBin(phys.nVert())) */ 1871 /*1667*/;
  }
  //cout<<__LINE__<<endl;
  return 1; // should not get here
}

bool passMETFilters(){
  bool pass = true;

  if ( phys.isData() ) {
    if ( phys.nVert() == 0 ) {
      numEvents->Fill(1);
      //if (printFail) cout<<phys.evt()<<" :Failed nVerts cut"<<endl;
      pass=false;
    }
    if (!phys.Flag_HBHENoiseFilter                   ()      ){ 
      pass=false;
      //if (printFail) cout<<phys.evt()<<" :Failed HBHENoiseFilter cut"<<endl;
      numEvents->Fill(2);
    }
    if (!phys.Flag_HBHEIsoNoiseFilter                ()      ){ 
      pass=false;
      //if (printFail) cout<<phys.evt()<<" :Failed HBHEIsoNoiseFilter cut"<<endl;
      numEvents->Fill(3);
    } 
    /*if (!phys.Flag_CSCTightHalo2015Filter            ()      ){ 
      pass=false;
      //if (printFail) cout<<phys.evt()<<" :Failed CSCTightHalo2015Filter cut"<<endl;
      numEvents->Fill(4);
    }*/
    if (!phys.Flag_EcalDeadCellTriggerPrimitiveFilter()      ) { 
      pass=false;
      //if (printFail) cout<<phys.evt()<<" :Failed EcalDeadCellTriggerPrimativeFilter cut"<<endl;
      numEvents->Fill(5);
    }
    if (!phys.Flag_goodVertices                      ()      ) { 
      pass=false;
      //if (printFail) cout<<phys.evt()<<" :Failed goodVerticies cut"<<endl;
      numEvents->Fill(6);
    }
    if (!phys.Flag_eeBadScFilter                     ()      ) { 
      pass=false;
      //if (printFail) cout<<phys.evt()<<" :Failed eeBadScFilter cut"<<endl;
      numEvents->Fill(7);
    }
    if (!phys.Flag_globalTightHalo2016            ()      ){ 
      pass=false;
      //if (printFail) cout<<phys.evt()<<" :Failed CSCTightHalo2015Filter cut"<<endl;
      numEvents->Fill(4);
    }
    if (!phys.Flag_badMuonFilter            ()      ){ 
      pass=false;
      //if (printFail) cout<<phys.evt()<<" :Failed CSCTightHalo2015Filter cut"<<endl;
      numEvents->Fill(50);
    }
    if (!phys.Flag_badChargedCandidateFilter            ()      ){ 
      pass=false;
      //if (printFail) cout<<phys.evt()<<" :Failed CSCTightHalo2015Filter cut"<<endl;
      numEvents->Fill(51);
    }
  }
  if (pass){
    //if (printPass) cout<<phys.evt()<<": Passes MET Filters"<<endl;
  }
  return pass;
}

bool passBaseCut(){
  bool pass=true;
  //if (printStats) { cout<<"goodrun : "<<goodrun(phys.evt(), phys.lumi())<<" "; }
  //if (printStats) { cout<<"njets : "<<phys.njets()<<" "; }
  
  if (phys.isData()){
    if (! (goodrun(phys.run(), phys.lumi()))){ 
      pass=false; //golden json
      //if (printFail) cout<<phys.evt()<<" :Failed golden JSON cut"<<endl;
      numEvents->Fill(8);
    }
  }

  //Old Method, using branch
  /*if (! (phys.evt_passgoodrunlist() > 0)){ 
    pass=false; //golden json
    //if (printFail) cout<<phys.evt()<<" :Failed golden JSON cut"<<endl;
    numEvents->Fill(8);
  } */
  if (! (phys.njets() >= 2) ){ 
    pass=false; //2 jet cut
    //if (printFail) cout<<phys.evt()<<" :Failed 2 Jets cut"<<endl;
    numEvents->Fill(9);
  }

  int nLepVeto = (conf->get("event_type") == "photon") ? 1 : 3; //Veto 1 lepton for gjets, 3 leptons for dilepton samples

  if( (phys.nisoTrack_mt2() + phys.nlep()) >= nLepVeto){
    pass=false; //third lepton veto
    //if (printFail) cout<<phys.evt()<<" :Failed extra lepton veto"<<endl;
    numEvents->Fill(54);
  }

  //if (printPass) cout<<phys.evt()<<": Passes Base Cuts"<<endl;
  return pass;
}

bool passPhotonEmulatedTrigger() {
  if( phys.nisoTrack_5gev()           > 0    ) return false;
  if( phys.gamma_r9()            .at(0) < 0.92 ) return false;
  if( phys.gamma_hOverE()        .at(0) > 0.2  ) return false;
  if( phys.gamma_hollowtkiso03() .at(0) > 5    ) return false;
  if( abs(phys.gamma_eta().at(0)) < 1.4 && phys.gamma_ecpfclusiso().at(0) > 4 + phys.gamma_pt().at(0) * 0.0053  ) return false;
  if( abs(phys.gamma_eta().at(0)) < 1.4 && phys.gamma_hcpfclusiso().at(0) > 8 + phys.gamma_pt().at(0) * 0.014   ) return false;
  if( abs(phys.gamma_eta().at(0)) > 1.6 && phys.gamma_ecpfclusiso().at(0) > 4 + phys.gamma_pt().at(0) * 0.0034  ) return false;
  if( abs(phys.gamma_eta().at(0)) > 1.6 && phys.gamma_hcpfclusiso().at(0) > 8 + phys.gamma_pt().at(0) * 0.0139  ) return false;

  return true;
}

bool passPhotonTriggers(){

  if( ((phys.HLT_Photon165_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon165_R9Id90_HE10_IsoM() > 0) || (phys.HLT_Photon165_HE10_matchedtophoton() && phys.HLT_Photon165_HE10() > 0)) && phys.gamma_pt().at(0) > 180 ) return true;
  else if( phys.HLT_Photon120_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon120_R9Id90_HE10_IsoM() > 0 && phys.gamma_pt().at(0) > 135 && phys.gamma_pt().at(0) < 180 ) return true;
  else if( phys.HLT_Photon90_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon90_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 105 && phys.gamma_pt().at(0) < 135  ) return true;
  else if( phys.HLT_Photon75_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon75_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 85 && phys.gamma_pt().at(0) < 105   ) return true;
  else if( phys.HLT_Photon50_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon50_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 55 && phys.gamma_pt().at(0) < 85    ) return true;
  else if( phys.HLT_Photon36_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon36_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 40 && phys.gamma_pt().at(0) < 55    ) return true;
  else if( phys.HLT_Photon30_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon30_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 33 && phys.gamma_pt().at(0) < 40    ) return true;
  else if( phys.HLT_Photon22_R9Id90_HE10_IsoM_matchedtophoton() && phys.HLT_Photon22_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) < 33 ) return true;
  return false;
}

bool passMuonTriggers(){
  if ( phys.isData() ){
    //cout<<__LINE__<<endl;
    if ( conf->get("use_muon_DZ_triggers") == "true" ){
      //cout<<"Using DZ triggers"<<endl;
      //cout<<__LINE__<<endl;
      //if (printStats) { cout<<"HLT_DoubleMu: "<<phys.HLT_DoubleMu()<<" HLT_DoubleMu_tk: "<<phys.HLT_DoubleMu_tk()<<" "<<" HLT_DoubleMu_noiso: "<<phys.HLT_DoubleMu_noiso()<<" "; }
      return (phys.HLT_DoubleMu() || phys.HLT_DoubleMu_tk() || phys.HLT_DoubleMu_noiso());
    }
    else{
      //cout<<"Using Non DZ triggers"<<endl;
      //cout<<__LINE__<<endl;
      //if (printStats) { cout<<"HLT_DoubleMu_nonDZ: "<<phys.HLT_DoubleMu_nonDZ()<<" HLT_DoubleMu_tk_nonDZ: "<<phys.HLT_DoubleMu_tk_nonDZ()<<" "<<" HLT_DoubleMu_noiso: "<<phys.HLT_DoubleMu_noiso()<<" "; }
      return (phys.HLT_DoubleMu_nonDZ() || phys.HLT_DoubleMu_tk_nonDZ() || phys.HLT_DoubleMu_noiso());
    } 
  }
  else{
    //cout<<__LINE__<<endl;
    return true; //MC always passes
  }
}

bool passElectronTriggers(){
  if ( phys.isData()){
    //cout<<__LINE__<<endl;
    //if (printStats) { cout<<"HLT_DoubleEl_DZ_2: "<<phys.HLT_DoubleEl_DZ_2()<<" HLT_DoubleEl_noiso: "<<phys.HLT_DoubleEl_noiso()<<" "; }
    return (phys.HLT_DoubleEl_DZ_2() || phys.HLT_DoubleEl_noiso() );
  }
  else{
    //cout<<__LINE__<<endl;
    return true; //MC always passes
  }
}

bool passEMuTriggers(){
  if (! phys.isData()){
    return true;
  }
  else{
    return (phys.HLT_MuEG() || phys.HLT_MuEG_2() || phys.HLT_MuEG_noiso());
  }
}

bool passLeptonHLTs(){
  if (conf->get("dil_sign") == "same"){
    return passEMuTriggers();
  }
  else{
    //cout<<__LINE__<<endl;
    if ( phys.hyp_type() == 1 ){ //Muon Event
      return passMuonTriggers();
    }
    else if ( phys.hyp_type() == 0 ){ //Electron Event
      return passElectronTriggers();
    }
    else{ //Emu event, should never happen since this is only called from hasGoodZ()
      return false; 
    }
  }
}

bool hasGoodZ(){
  if( phys.nlep() < 2         ){ 
    numEvents->Fill(10);
    //if (printFail) cout<<phys.evt()<<" :Failed 2 lepton Z cut"<<endl;
    return false; // require at least 2 good leptons
  }
  //if (printStats) { cout<<"Number of Leptons: "<<phys.nlep()<<" "; }
  
  //cout<<__LINE__<<endl;

  if( phys.lep_pt().at(0) < 25        ) {
    numEvents->Fill(11); 
    //if (printFail) cout<<phys.evt()<<" :Failed lep1 pt < 25 Z cut"<<endl;
    return false; // leading lep pT > 25 GeV
  }
  //if (printStats) { cout<<"lep1 pt: "<<phys.lep_pt().at(0)<<" "; }

  //cout<<__LINE__<<endl;

  if( phys.lep_pt().at(1) < 20        ) {
    numEvents->Fill(12); 
    //if (printFail) cout<<phys.evt()<<" :Failed lep2 pt < 20 Z cut"<<endl;
    return false; // tailing lep pT > 20 GeV      
  }
  //if (printStats) { cout<<"lep2 pt: "<<phys.lep_pt().at(1)<<" "; }
  
  //cout<<__LINE__<<endl;

  if( abs(phys.lep_p4().at(0).eta())     > 2.4       ) {
    numEvents->Fill(13); 
    //if (printFail) cout<<phys.evt()<<" :Failed lep1 eta > 2.4 Z cut"<<endl;
    return false; // eta < 2.4
  }    
  //if (printStats) { cout<<"lep1 eta: "<<phys.lep_p4().at(0).eta()<<" "; }

  //cout<<__LINE__<<endl;
  
  if( abs(phys.lep_p4().at(1).eta())     > 2.4       ) {
    numEvents->Fill(14); 
    //if (printFail) cout<<phys.evt()<<" :Failed lep2 eta > 2.4 Z cut"<<endl;
    return false; // eta < 2.4
  }
  //if (printStats) { cout<<"lep2 eta: "<<phys.lep_p4().at(1).eta()<<" "; }

  //cout<<__LINE__<<endl;

  if (! passLeptonHLTs()){
    numEvents->Fill(15);
    //if (printFail) cout<<phys.evt()<<" :Failed HLT Z cut"<<endl;
    return false;
  }

  //cout<<__LINE__<<endl;

  //This is the original cut selection
  if( abs(phys.lep_p4().at(0).eta()) > 1.4 && abs(phys.lep_p4().at(0).eta()) < 1.6 ){
    numEvents->Fill(17);
    //if (printFail) cout<<phys.evt()<<" :Failed lep1 in xition region Z cut"<<endl;
    return false;
  }

  //cout<<__LINE__<<endl;

  if( abs(phys.lep_p4().at(1).eta()) > 1.4 && abs(phys.lep_p4().at(1).eta()) < 1.6 ) {
    numEvents->Fill(18); 
    //if (printFail) cout<<phys.evt()<<" :Failed lep2 in xition region Z cut"<<endl;
    return false; // veto xition region
  }

  //cout<<__LINE__<<endl;
  
  if( phys.dRll() < 0.1 ) {
    numEvents->Fill(19); 
    //if (printFail) cout<<phys.evt()<<" :Failed deltaR Z cut"<<endl;
    return false;
  }
  //if (printStats) { cout<<"DeltaR_ll: "<<phys.dRll()<<" "; }

  //cout<<__LINE__<<endl;

  if (conf->get("dil_sign") == "same"){ //only true for ttbar estimate
    if (! (phys.hyp_type() == 2) ){ //require explicit emu event
      numEvents->Fill(20); 
      //if (printFail) cout<<phys.evt()<<" :Failed not explicit e/mu Z cut, for ttbar only"<<endl;
      return false; // require explicit opposite flavor event
    }
    //if (printStats) { cout<<"hyp_type: "<<phys.hyp_type()<<" "; }
  }
  else{
    //require explicit hypothesis type
    if( !( phys.hyp_type() == 0 || phys.hyp_type() == 1 ) ) {
        numEvents->Fill(20); 
        //if (printFail) cout<<phys.evt()<<" :Failed explicit mu/mu or e/e Z cut"<<endl;
        return false; // require explicit same flavor event
    }
    //if (printStats) { cout<<"hyp_type: "<<phys.hyp_type()<<" "; } 
  }

  //cout<<__LINE__<<endl;

  if( !(phys.evt_type() == 0 ) ) {
    numEvents->Fill(21); 
    //if (printFail) cout<<phys.evt()<<" :Failed evt_type=0 Z cut"<<endl;
    return false; // require opposite sign
  }
  //if (printStats) { cout<<"evt_type: "<<phys.evt_type()<<" "; }

  //cout<<__LINE__<<endl;
  //Set up Dilepton Masses
  double dilmass_low, dilmass_high;

  if (conf->get("dilmass_low") == "") {
    dilmass_low = 86;
  }
  else{
    dilmass_low = stod(conf->get("dilmass_low"));
  }

  if (conf->get("dilmass_high") == ""){
    dilmass_high = 96;
  }
  else{
    dilmass_high = stod(conf->get("dilmass_high"));
  }

  if( phys.dilmass() < dilmass_low ) {
    numEvents->Fill(22); 
    //if (printFail) cout<<phys.evt()<<" :Failed Z mass window Z cut"<<endl;
    return false; // on-Z
  }
  if( phys.dilmass() > dilmass_high && dilmass_high != -1 ) {
    numEvents->Fill(22); 
    //if (printFail) cout<<phys.evt()<<" :Failed Z mass window Z cut"<<endl;
    return false; // on-Z
  }
  //if (printStats) { cout<<"mass_ll: "<<phys.dilmass()<<" "; }

  //cout<<__LINE__<<endl;
  
  /*if( !(phys.dilpt() > 22) ){
    numEvents->Fill(23); 
    return false; // Z pT > 22 GeV
  }*/

  //if (printPass) cout<<phys.evt()<<": Passes good Z Cuts"<<endl;
  return true;
}

bool hasGoodPhoton(){
  if( phys.ngamma() <  1 ) {
    numEvents->Fill(24);
    //if (printFail) cout<<phys.evt()<<" :Failed at least 1 photon cut"<<endl;
    return false; // require at least 1 good photon
  }
  
  if( phys.evt_type() != 2 ) {
    numEvents->Fill(25);
    //if (printFail) cout<<phys.evt()<<" :Failed explicitly evt_type2 photon cut"<<endl;
    return false; // photon + jets events
  }
  
  if( phys.gamma_pt().at(0) < 22 ) {
    numEvents->Fill(26);
    //if (printFail) cout<<phys.evt()<<" :Failed pt < 22 photon cut"<<endl;
    return false; // photon pt > 22 GeV
  }
  
  if( abs(phys.gamma_p4().at(0).eta()) > 1.4 && abs(phys.gamma_p4().at(0).eta()) < 1.6 ) {
    numEvents->Fill(27);
    //if (printFail) cout<<phys.evt()<<" :Failed gamm ain xition region photon cut"<<endl;
    return false; // veto xition region
  }
  
  if( abs(phys.gamma_p4().at(0).eta()) > 2.4 ) {
    numEvents->Fill(28);
    //if (printFail) cout<<phys.evt()<<" :Failed gamma eta > 2.4 photon cut"<<endl;
    return false; // photon in EC or EB
  }
  
  if( phys.gamma_hOverE().at(0) > 0.1 ) {
    numEvents->Fill(29);
    //if (printFail) cout<<phys.evt()<<" :Failed gamma hOverE photon cut"<<endl;
    return false; // H/E < 0.1
  }
  
  // if( phys.matched_neutralemf()          < 0.7   ) return false; // jet neutral EM fraction cut
  
  if( phys.matched_emf() < 0.7 ) {
    numEvents->Fill(30);
    //if (printFail) cout<<phys.evt()<<" :Failed matched_emf photon cut"<<endl;
    return false; // jet neutral EM fraction cut
  }
  
  if( acos( cos( phys.gamma_phi().at(0) - phys.met_phi() ) ) < 0.14 ) {
    numEvents->Fill(31);
    //if (printFail) cout<<phys.evt()<<" :Failed photons aligned with MET photon cut"<<endl;
    return false; // kill photons aligned with MET
  }
  
  if( phys.elveto() ) {
    numEvents->Fill(32);
    //if (printFail) cout<<phys.evt()<<" :Failed electron pixel veto photon cut"<<endl;
    return false; // veto pixel match
  }

  if (phys.nlep() > 0 ){
    numEvents->Fill(49);
    //if (printFail) cout<<phys.evt()<<" :Failed nleps in photon cut for REALMET"<<endl;
    return false; // lepton veto
  }

  if (phys.isData() && (! passPhotonTriggers()) ){
    numEvents->Fill(52);
    //if (printFail) cout<<phys.evt()<<" :Failed Photon trigger cut"<<endl;
    return false;
  }

  if (! passPhotonEmulatedTrigger() ){
    numEvents->Fill(53);
    //if (printFail) cout<<phys.evt()<<" :Failed emulated photon trigger"<<endl;
    return false;
  }
  
  //if (printPass) cout<<phys.evt()<<": Passes good gamma Cuts"<<endl;
  return true;  
}

bool hasGoodBoson() {
  if ( conf->get("event_type") == "photon") {
    return hasGoodPhoton();
  }
  else {
    return hasGoodZ();
  }
}

double bosonPt(){
  // Returns boson Pt, determines whether sample is gjets or zjets first.
  if (conf->get("event_type") == "dilepton") {
    return phys.dilpt();
  }
  else{
    if (phys.evt_type() == 2 && phys.ngamma() > 0){
      return phys.gamma_pt().at(0);
    }
    else
      return 0;
  }
}

double getEff(){
  if (abs(phys.gamma_p4().at(0).eta()) < 1.4){
    return g_vpt_eff_barrel->GetEfficiency(g_vpt_eff_barrel->FindFixBin(phys.gamma_pt().at(0)));
  }
  else{
    return g_vpt_eff_endcap->GetEfficiency(g_vpt_eff_endcap->FindFixBin(phys.gamma_pt().at(0))); 
  }
}

void readyReweightHists(){
  TString conf_name = conf->get("Name");

  //cout<<"FINDFIND Adding "<<conf->get("Name");

  cout<<"Reweighting with "<<TString(conf->get("histo_output_dir")+"ct_"+conf->get("rwt_var")+"_"+conf->get("signal_region")+"_rwt.root")<<endl;
  TString rwt_hist_name = "h_"+conf->get("rwt_var")+"_ratio";
  TFile *reweight_file = TFile::Open( TString(conf->get("histo_output_dir")+"ct_"+conf->get("rwt_var")+"_"+conf->get("signal_region")+"_rwt.root"), "READ");
  g_reweight_pairs.push_back(make_pair( (TH1D*) reweight_file->Get(rwt_hist_name)->Clone(TString("reweight_hist_")+conf->get("rwt_var")),conf->get("rwt_var")));
  g_reweight_pairs.back().first->SetDirectory(rootdir);
  reweight_file->Close();

  while (conf->get("weight_from") != "" ){
    conf->loadConfig(conf->get("weight_from"));
    //cout<<"FINDFIND Adding "<<conf->get("Name");
    cout<<"Reweighting with "<<TString(conf->get("histo_output_dir")+"ct_"+conf->get("rwt_var")+"_"+conf->get("signal_region")+"_rwt.root")<<endl;
    rwt_hist_name = "h_"+conf->get("rwt_var")+"_ratio";
    reweight_file = TFile::Open( TString(conf->get("histo_output_dir")+"ct_"+conf->get("rwt_var")+"_"+conf->get("signal_region")+"_rwt.root"), "READ");
    g_reweight_pairs.push_back(make_pair( (TH1D*) reweight_file->Get(rwt_hist_name)->Clone(TString("reweight_hist_")+conf->get("rwt_var")),conf->get("rwt_var")));
    g_reweight_pairs.back().first->SetDirectory(rootdir);
    reweight_file->Close();      
  }

  conf->loadConfig(conf_name.Data());
  cout<<"Reweight hists loaded, proceeding with conf "<<conf->get("Name")<<endl;
}

double getReweight(){
  double weight = 1;
  
  TH1D* rwt_hist;
  TString rwt_var;
  //cout<<"Size: "<<g_reweight_pairs.size()<<endl;
  for (int i=0; i<(int)g_reweight_pairs.size(); i++){
    rwt_hist = g_reweight_pairs.at(i).first;
    rwt_var = g_reweight_pairs.at(i).second;
    //cout<<rwt_var<<endl;

    if (rwt_var == "vpt"){
      //cout<<"Addign vpt weight: "<<rwt_hist->GetBinContent(rwt_hist->FindBin(bosonPt()))<<endl;
      weight *= rwt_hist->GetBinContent(rwt_hist->FindBin(bosonPt()));
    }
    else if (rwt_var == "ht_wide"){
      //cout<<"Adding HT weight: "<<rwt_hist->GetBinContent(rwt_hist->FindBin(phys.ht()))<<endl;
      weight *= rwt_hist->GetBinContent(rwt_hist->FindBin(phys.ht())); 
    }

  }

  return weight;
}

double scale1fbFix(){
  /*This method stores fixes to the evt_scale1fb in the event of file corruptions. It's basically just a lookup table*/

  if TString(currentFile->GetTitle()).Contains("sttw_antitop_nofullhaddecay_powheg"){
    return 1.03;
  }
  else{
    return 1;
  }
}

double getWeight(){
  /*Gets the proper weight for the sample. */
  double weight=1;
  //cout<<__LINE__<<endl;
  // If we don't have data use scale to 1 fb^-1. 
  if ( ! ( phys.isData() ) ) {
    if (phys.evt_scale1fb() > 1){
      weight=0;
    }
    else{
      weight *= phys.evt_scale1fb();
    }
    //Weight to some other lumi
    if ( conf->get("scaleTofb") != "" ){
      weight *= stod(conf->get("scaleTofb"));
    }
    if (TString(conf->get("data_set")).Contains("SinglePhoton") && (! TString(currentFile->GetTitle()).Contains("Prompt_ph")) ){
      weight *= -26.4; //EWK Subtraction
    }
  }
  //cout<<__LINE__<<endl;

  if ( conf->get("reweight") == "true" ) {
    weight *= getReweight();
  }

  if ( conf->get("reweight_eff") == "true" && conf->get("event_type") == "photon" && phys.ngamma() > 0){
    weight *= getEff();
  }
  //cout<<__LINE__<<endl;

 if ((! phys.isData()) && conf->get("event_type") != "photon" ){
    if (conf->get("susy_mc") != "true" && conf->get("pileup_reweight") == "true"){
      weight*=g_pileup_hist->GetBinContent(g_pileup_hist->FindBin(phys.nTrueInt()));
    }
    if (phys.hyp_type() == 0) weight *= 0.963;
    if (phys.hyp_type() == 1) weight *= 0.947;
    if (phys.hyp_type() == 2) weight *= 0.899;
    
    if (phys.nlep() > 1){
      weight*=phys.weightsf_lepid().at(0);
      weight*=phys.weightsf_lepid().at(1);
  
      weight*=phys.weightsf_lepiso().at(0);
      weight*=phys.weightsf_lepiso().at(1);
  
      weight*=phys.weightsf_lepip().at(0);
      weight*=phys.weightsf_lepip().at(1);

      weight*=phys.weightsf_lepreco().at(0);
      weight*=phys.weightsf_lepreco().at(1);
    }
    
    weight*=phys.weight_btagsf();
  }
  //cout<<__LINE__<<endl;

  if (phys.isData() && conf->get("event_type") == "photon" && phys.ngamma() > 0){
    weight *= getPrescaleWeight();
  }
  //cout<<__LINE__<<endl;

  /*if (weight < 0){
    cout<<"Negative Weight: "<<weight<<" "<<phys.evt()<<endl;
  }*/

  weight *= scale1fbFix();

  return weight;
}

bool isDuplicate(){
  //cout<<__LINE__<<endl;
  if( phys.isData() ) {
    DorkyEventIdentifier id(phys.run(), phys.evt(), phys.lumi());
    //cout<<__LINE__<<endl;
    if (is_duplicate(id) ){
      ++nDuplicates;
      //cout<<__LINE__<<endl;
      //if (printFail) cout<<phys.evt()<<" :Is a duplicate"<<endl;
      return true;
    }
    //cout<<__LINE__<<endl;
  }
  //cout<<__LINE__<<endl;
  //if (printPass) cout<<phys.evt()<<": Passes not a duplicate"<<endl;
  return false;
}

bool passSignalRegionCuts(){
  
  //Njets Min Cut
  if (conf->get("Njets_min") != ""){
    if (phys.njets() < stod(conf->get("Njets_min"))){
      numEvents->Fill(34);
      //if (printFail) cout<<phys.evt()<<" :Failed min jets cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  //Njets Max Cut
  if (conf->get("Njets_max") != ""){
    if (phys.njets() > stod(conf->get("Njets_max"))){
      numEvents->Fill(35);
      //if (printFail) cout<<phys.evt()<<" :Failed max jets cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;
  //if (printStats) { cout<<"NbjetsMed: "<<phys.nBJetMedium()<<" "; }

  //Num Bottom jets Min Cut
  if (conf->get("NBjets_min") != ""){
    if (phys.nBJetMedium() < stod(conf->get("NBjets_min"))){
      numEvents->Fill(36);
      //if (printFail) cout<<phys.evt()<<" :Failed min bjet cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  //Num Bottom jets Max Cut
  if (conf->get("NBjets_max") != ""){
    if (phys.nBJetMedium() > stod(conf->get("NBjets_max"))){
      numEvents->Fill(37);
      //if (printFail) cout<<phys.evt()<<" :Failed max bjet cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;
  //if (printStats) { cout<<"dphi_metj1: "<<phys.dphi_metj1()<<" "; }
  //Leading Jet/MET Phi min
  if (conf->get("dPhi_MET_j1") != ""){
    if (phys.dphi_metj1() < stod(conf->get("dPhi_MET_j1"))){
      numEvents->Fill(38);
      //if (printFail) cout<<phys.evt()<<" :Failed dPhi MET with jet 1 cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;
  //if (printStats) { cout<<"dphi_metj2: "<<phys.dphi_metj2()<<" "; }
  //Trailing Jet/MET Phi min
  if (conf->get("dPhi_MET_j2") != ""){
    if (phys.dphi_metj2() < stod(conf->get("dPhi_MET_j2"))){
      numEvents->Fill(39);
      //if (printFail) cout<<phys.evt()<<" :Failed dPhi MET with jet 2 cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;
  //if (printStats) { cout<<"mt2b: "<<phys.mt2b()<<" "; }
  //MT2b min
  if (conf->get("MT2b_min") != "" && conf->get("event_type") != "photon"){
    if (phys.mt2b() < stod(conf->get("MT2b_min"))){
      numEvents->Fill(40);
      //if (printFail) cout<<phys.evt()<<" :Failed MT2b cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;
  //if (printStats) { cout<<"mt2b: "<<phys.mt2b()<<" "; }
  //MT2 min
  if (conf->get("MT2_min") != "" && conf->get("event_type") != "photon"){
    if (phys.mt2() < stod(conf->get("MT2_min"))){
      numEvents->Fill(59);
      //if (printFail) cout<<phys.evt()<<" :Failed MT2b cut"<<endl;
      return false;
    }
  }


  //cout<<__LINE__<<endl;
  //HT min
  if (conf->get("HT_min") != ""){
  //if (printStats) { cout<<"ht: "<<phys.ht()<<" "; }
    if (phys.ht() < stod(conf->get("HT_min"))){
      numEvents->Fill(41);
      //if (printFail) cout<<phys.evt()<<" :Failed sum HT min cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;
  //DiBottom mass difference from Higgs Mass
  if (conf->get("mbb_mh_diff") != ""){
  //if (printStats) { cout<<"mbb_mh_diff: "<<abs(phys.mbb_csv() - 125)<<" "; }
    if (abs(phys.mbb_csv() - 125) < stod(conf->get("mbb_mh_diff"))){
      numEvents->Fill(42);
      //if (printFail) cout<<phys.evt()<<" :Failed sum mbb_mh diff cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  //Wierd ATLAS SR cut
  if (conf->get("sum_HT_pt_pt") != ""){
    double pt;
    
    if (phys.evt_type() == 2 && phys.ngamma() > 0){
      pt = phys.gamma_pt().at(0);
    }
    else{
      pt = phys.lep_pt().at(0) + phys.lep_pt().at(1);
    }
    //if (printStats) { cout<<"sum_HT_pt_pt: "<<abs(phys.ht() + pt )<<" "; }
    if ( abs(phys.ht() + pt ) < stod(conf->get("sum_HT_pt_pt") ) ){
      numEvents->Fill(43);
      //if (printFail) cout<<phys.evt()<<" :Failed sum HT pt pt cut"<<endl;
      return false;
    }
  }
  
  if (conf->get("lep1_pt_min") != "" && conf->get("event_type") != "photon" ){
    if ( phys.lep_pt().at(0) < stod( conf->get("lep1_pt_min") )){
      numEvents->Fill(45);
      //if (printFail) cout<<phys.evt()<<" :Failed lep1 min pt"<<endl;
      return false;
    }
  }

  if (conf->get("lep2_pt_min") != "" && conf->get("event_type") != "photon" ){
    if ( phys.lep_pt().at(1) < stod( conf->get("lep2_pt_min") )){
      numEvents->Fill(46);
      //if (printFail) cout<<phys.evt()<<" :Failed lep2 min pt cut"<<endl;
      return false;
    }
  }

  if (conf->get("MET_min") != ""){
    if ( phys.met_T1CHS_miniAOD_CORE_pt() < stod( conf->get("MET_min") )){
      numEvents->Fill(56);
      //if (printFail) cout<<phys.evt()<<" :Failed lep2 min pt cut"<<endl;
      return false;
    }
  }

  if (conf->get("MET_max") != ""){
    if ( phys.met_T1CHS_miniAOD_CORE_pt() > stod( conf->get("MET_max") )){
      numEvents->Fill(57);
      //if (printFail) cout<<phys.evt()<<" :Failed lep2 min pt cut"<<endl;
      return false;
    }
  }

  if (conf->get("Mbb_max") != ""){
    if ( phys.mbb_bpt() > stod( conf->get("Mbb_max") )){
      numEvents->Fill(58);
      //if (printFail) cout<<phys.evt()<<" :Failed lep2 min pt cut"<<endl;
      return false;
    }
  }


  //if (printPass) cout<<phys.evt()<<": Passes Signal Region Cuts"<<endl;
  return true;
}

bool passRareCuts(){
  
  bool hasrealmet = true;
  bool realzpair  = true;
  
  if( g_sample_name == "vvv" || g_sample_name == "ttv" ){
    //cout<<"Checking for rare cuts"<<endl;
    hasrealmet = false;
    realzpair  = false;
    
    for( size_t genind = 0; genind < phys.genPart_motherId().size(); genind++ ){
      if( (abs(phys.genPart_motherId().at(genind)) == 24 || phys.genPart_motherId().at(genind) == 23) &&
        phys.genPart_status().at(genind) == 23 &&
        (abs(phys.genPart_pdgId().at(genind))==12 ||
         abs(phys.genPart_pdgId().at(genind))==14 ||
         abs(phys.genPart_pdgId().at(genind))==16) ){
        // cout<<"mom "<<phys.genPart_motherId().at(genind) << " | stat "<< phys.genPart_status().at(genind) <<  " | id "<< phys.genPart_pdgId().at(genind) << endl;
        hasrealmet = true;
      }
      
      if( (phys.genPart_motherId().at(genind) == 23 || phys.genPart_grandmaId().at(genind) == 23) &&
        (phys.genPart_status().at(genind) == 23 || phys.genPart_status().at(genind) == 1) &&
        (abs(phys.genPart_pdgId().at(genind))==11 ||
         abs(phys.genPart_pdgId().at(genind))==13) ){
        realzpair = true;
      }
    }
  }
  //if (printStats) { cout<<"HasGenZ: "<<realzpair<<" "; }
  //if (printStats) { cout<<"HasGenMET: "<<hasrealmet<<" "; }
  if ( ! hasrealmet ){
    numEvents->Fill(47);
    //if (printFail) cout<<phys.evt()<<" :Failed Has Real MET Rare Cut"<<endl;    
    return false;
  }
  else if ( ! realzpair ){
    numEvents->Fill(48);
    //if (printFail) cout<<phys.evt()<<" :Failed Has Real Z Rare Cut"<<endl;    
    return false;
  }
  
  //if (printPass) cout<<phys.evt()<<": Passes Has Real MET Rare Cut"<<endl;
  //if (printPass) cout<<phys.evt()<<": Passes Has Real Z Pair Rare Cut"<<endl;
  return true;
}

bool passSUSYSingalCuts(){
  //Njets Min Cut
  if (conf->get("mass_chi") != ""){
    if (phys.mass_chi() != stod(conf->get("mass_chi"))){
      numEvents->Fill(55);
      //if (printFail) cout<<phys.evt()<<" :Failed mass chi cut"<<endl;
      return false;
    }
  } 

  return true; 
}

int ScanChain( TChain* chain, ConfigParser *configuration, bool fast = true, int nEvents = -1) {
  /* Runs through baby files and makes histogram files. 
  
  Inputs:
  chain -- contains the files to make the histograms from, 
  configuration -- pointer to the configuration object
  */  
  //cout<<__LINE__<<endl;
  //Set Global Vars
  conf=configuration;
  //cout<<__LINE__<<endl;
  g_sample_name=conf->get("Name");

  TString savePath = getOutputDir(conf, "hist");
  //cout<<__LINE__<<endl;
  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");
//=======================================
// Define Histograms
//=======================================

  clear_list(); //Event duplicate removal clear list

  cout<<"Opening file "<<TString(savePath+conf->get("Name")+".root")<<endl;
  TFile * output = new TFile(TString(savePath+conf->get("Name")+".root"), "recreate");

  numEvents = new TH1I("numEvents", "Number of events in "+g_sample_name, 60, 0, 60);
  numEvents->SetDirectory(rootdir);

  //MET Histos
  TH1D *t1met = new TH1D("type1MET", "Type 1 MET for "+g_sample_name, 6000,0,6000);
  t1met->SetDirectory(rootdir);
  t1met->Sumw2();

  const int n_metbins_wide_std = 6;
  const double metbins_wide_std[n_metbins_wide_std+1] = {0, 50, 100, 150, 225, 300, 500};

  TH1D *t1met_widebin = new TH1D("type1MET_widebin", "Type 1 MET for "+g_sample_name, n_metbins_wide_std, metbins_wide_std);
  t1met_widebin->SetDirectory(rootdir);
  t1met_widebin->Sumw2();

  //MET Histos
  TH1D *nVert = new TH1D("nVert", "Number of verticies in "+g_sample_name, 150,0,150);
  nVert->SetDirectory(rootdir);
  nVert->Sumw2();

  TH1D *rawmet = new TH1D("rawMET", "Raw MET for "+g_sample_name, 6000,0,6000);
  rawmet->SetDirectory(rootdir);
  rawmet->Sumw2();

  TH1D *mt2 = new TH1D("mt2", "MT2 for "+g_sample_name, 500,0,500);
  mt2->SetDirectory(rootdir);
  mt2->Sumw2();

  TH1D *dilmass;
  
  if (conf->get("wide_dilmass") == "true"){
    dilmass = new TH1D("dilmass", "Dilepton Mass for "+g_sample_name, 500,0,500);
  }
  else{
    dilmass = new TH1D("dilmass", "Dilepton Mass for "+g_sample_name, 20,81,101);
  }  
  
  dilmass->SetDirectory(rootdir);
  dilmass->Sumw2();

  TH1D *mt2b = new TH1D("mt2b", "MT2b for "+g_sample_name, 6000,0,6000);
  mt2b->SetDirectory(rootdir);
  mt2b->Sumw2();

  TH1D *nlep = new TH1D("nlep", "Number of Leptons for "+g_sample_name, 20,0,20);
  nlep->SetDirectory(rootdir);
  nlep->Sumw2();

  TH1D *nisotrack = new TH1D("nisotrack", "Number of Iso Track Leptons (MT2 style) for "+g_sample_name, 20,0,20);
  nisotrack->SetDirectory(rootdir);
  nisotrack->Sumw2();

  TH1D *dphi_jet1_met = new TH1D("dphi_jet1_met", "#Delta#Phi(jet_{1}, E^{miss}_{T}) for "+g_sample_name, 100,0,3.15);
  dphi_jet1_met->SetDirectory(rootdir);
  dphi_jet1_met->Sumw2();

  TH1D *dphi_jet2_met = new TH1D("dphi_jet2_met", "#Delta#Phi(jet_{2}, E^{miss}_{T}) for "+g_sample_name, 100,0,3.15);
  dphi_jet2_met->SetDirectory(rootdir);
  dphi_jet2_met->Sumw2();

  TH1D *ht = new TH1D("ht", "Scalar sum of hadronic pt (HT) for "+g_sample_name, 6000,0,6000);
  ht->SetDirectory(rootdir);
  ht->Sumw2();

  TH1D *ht_wide = new TH1D("ht_wide", "Scalar sum of hadronic pt (HT) for "+g_sample_name, 60,0,6000);
  ht_wide->SetDirectory(rootdir);
  ht_wide->Sumw2();

  TH1D *gen_ht = new TH1D("genht", "Scalar sum of generated hadronic pt (Gen HT) for "+g_sample_name, 6000,0,6000);
  gen_ht->SetDirectory(rootdir);
  gen_ht->Sumw2();

  TH1D *numMETFilters = new TH1D("numMETFilters", "Number of MET Filters passed for events in "+g_sample_name, 50,0,50);
  numMETFilters->SetDirectory(rootdir);
  numMETFilters->Sumw2();

  const int n_ptbins = 23;
  const double ptbins[n_ptbins+1] = {0, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 110, 120, 130, 140, 150, 225, 300, 6000};

  const int n_ptbins_std = 10;
  const double ptbins_std[n_ptbins_std+1] = {0, 33, 40, 55, 85, 105, 135, 180, 200, 300, 6000};

  const int n_ptbins_atlas = 9;
  const double ptbins_atlas[n_ptbins_atlas+1] = {0, 33, 40, 55, 85, 105, 135, 180, 200, 6000};

  TH1D *vpt;
  
  if (conf->get("signal_region") == "ATLAS")
  {
    vpt = new TH1D("vpt", "Boson Pt for events in "+g_sample_name, n_ptbins_atlas, ptbins_atlas); 
  }
  else if (conf->get("signal_region") == "VincePhotonPT"){
    vpt = new TH1D("vpt", "Boson Pt for events in "+g_sample_name, 6000, 0, 6000);  
  }

  else{
    vpt = new TH1D("vpt", "Boson Pt for events in "+g_sample_name, n_ptbins_std, ptbins_std);  
  }

  vpt->SetDirectory(rootdir);
  vpt->Sumw2();

  TH1D *vpt_flat = new TH1D("vpt_flat", "Boson P_{T} for events in "+g_sample_name, 6000,0,6000);
  vpt_flat->SetDirectory(rootdir);
  vpt_flat->Sumw2();

  TH1D *njets = new TH1D("njets", "Number of jets for events in "+g_sample_name, 50,0,50);
  njets->SetDirectory(rootdir);
  njets->Sumw2();

  TH1D *nbtags_m = new TH1D("nbtags_m", "Number of \"medium\" B-tagged jets for events in "+g_sample_name, 50,0,50);
  nbtags_m->SetDirectory(rootdir);
  nbtags_m->Sumw2();

  TH1D *nbtags_l = new TH1D("nbtags_l", "Number of \"loose\" B-tagged jets for events in "+g_sample_name, 50,0,50);
  nbtags_l->SetDirectory(rootdir);
  nbtags_l->Sumw2();

  TH1D *nbtags_t = new TH1D("nbtags_t", "Number of \"tight\" B-tagged jets for events in "+g_sample_name, 50,0,50);
  nbtags_t->SetDirectory(rootdir);
  nbtags_t->Sumw2();

  TH1D *nVert_HLT_Photon165_R9Id90_HE10_IsoM = new TH1D("nVert_HLT_Photon165_R9Id90_HE10_IsoM", "Number of verticies for HLT_Photon165_R9Id90_HE10_IsoM",150,0,150);
  TH1D *nVert_HLT_Photon120_R9Id90_HE10_IsoM = new TH1D("nVert_HLT_Photon120_R9Id90_HE10_IsoM", "Number of verticies for HLT_Photon120_R9Id90_HE10_IsoM",150,0,150);
  TH1D *nVert_HLT_Photon90_R9Id90_HE10_IsoM = new TH1D("nVert_HLT_Photon90_R9Id90_HE10_IsoM", "Number of verticies for HLT_Photon90_R9Id90_HE10_IsoM",150,0,150);
  TH1D *nVert_HLT_Photon75_R9Id90_HE10_IsoM = new TH1D("nVert_HLT_Photon75_R9Id90_HE10_IsoM", "Number of verticies for HLT_Photon75_R9Id90_HE10_IsoM",150,0,150);
  TH1D *nVert_HLT_Photon50_R9Id90_HE10_IsoM = new TH1D("nVert_HLT_Photon50_R9Id90_HE10_IsoM", "Number of verticies for HLT_Photon50_R9Id90_HE10_IsoM",150,0,150);
  TH1D *nVert_HLT_Photon36_R9Id90_HE10_IsoM = new TH1D("nVert_HLT_Photon36_R9Id90_HE10_IsoM", "Number of verticies for HLT_Photon36_R9Id90_HE10_IsoM",150,0,150);
  TH1D *nVert_HLT_Photon30_R9Id90_HE10_IsoM = new TH1D("nVert_HLT_Photon30_R9Id90_HE10_IsoM", "Number of verticies for HLT_Photon30_R9Id90_HE10_IsoM",150,0,150);
  TH1D *nVert_HLT_Photon22_R9Id90_HE10_IsoM = new TH1D("nVert_HLT_Photon22_R9Id90_HE10_IsoM", "Number of verticies for HLT_Photon22_R9Id90_HE10_IsoM",150,0,150);

  /*TH1D *pt_HLT_Photon165_R9Id90_HE10_IsoM = new TH1D("pt_HLT_Photon165_HE10_IsoM", "P_{T} for HLT_Photon165_R9Id90_HE10_IsoM",6000,0,6000);
  TH1D *pt_HLT_Photon120_R9Id90_HE10_IsoM = new TH1D("pt_HLT_Photon120_R9Id90_HE10_IsoM", "P_{T} for HLT_Photon120_R9Id90_HE10_IsoM",6000,0,6000);
  TH1D *pt_HLT_Photon90_R9Id90_HE10_IsoM = new TH1D("pt_HLT_Photon90_R9Id90_HE10_IsoM", "P_{T} for HLT_Photon90_R9Id90_HE10_IsoM",6000,0,6000);
  TH1D *pt_HLT_Photon75_R9Id90_HE10_IsoM = new TH1D("pt_HLT_Photon75_R9Id90_HE10_IsoM", "P_{T} for HLT_Photon75_R9Id90_HE10_IsoM",6000,0,6000);
  TH1D *pt_HLT_Photon50_R9Id90_HE10_IsoM = new TH1D("pt_HLT_Photon50_R9Id90_HE10_IsoM", "P_{T} for HLT_Photon50_R9Id90_HE10_IsoM",6000,0,6000);
  TH1D *pt_HLT_Photon36_R9Id90_HE10_IsoM = new TH1D("pt_HLT_Photon36_R9Id90_HE10_IsoM", "P_{T} for HLT_Photon36_R9Id90_HE10_IsoM",6000,0,6000);
  TH1D *pt_HLT_Photon30_R9Id90_HE10_IsoM = new TH1D("pt_HLT_Photon30_R9Id90_HE10_IsoM", "P_{T} for HLT_Photon30_R9Id90_HE10_IsoM",6000,0,6000);
  TH1D *pt_HLT_Photon22_R9Id90_HE10_IsoM = new TH1D("pt_HLT_Photon22_R9Id90_HE10_IsoM", "P_{T} for HLT_Photon22_R9Id90_HE10_IsoM",6000,0,6000);*/

  TH1D *sum_mlb, *m_bb_csv, *m_bb_bpt, *mt2j, *sum_pt_z_bb, *mt2_fromb, *mt2_hz;
  TH2D *MT2_MT2B, *MT2_MT2_fromb, *MT2_MT2_HZ;

  if(conf->get("signal_region") == "TChiHZ"){
    sum_mlb = new TH1D("sum_mlb", "#Sigma M_{lb} for "+g_sample_name, 6000,0,6000);
    sum_mlb->SetDirectory(rootdir);
    sum_mlb->Sumw2();

    m_bb_csv = new TH1D("m_bb_csv", "M_{bb} by CSV for "+g_sample_name, 6000,0,6000);
    m_bb_csv->SetDirectory(rootdir);
    m_bb_csv->Sumw2();

    m_bb_bpt = new TH1D("m_bb_bpt", "M_{bb} by B P_{T} for "+g_sample_name, 6000,0,6000);
    m_bb_bpt->SetDirectory(rootdir);
    m_bb_bpt->Sumw2();

    mt2j = new TH1D("mt2j", "MT2j for "+g_sample_name, 6000,0,6000);
    mt2j->SetDirectory(rootdir);
    mt2j->Sumw2();

    mt2_fromb = new TH1D("mt2_fromb", "MT2 From Bjets for "+g_sample_name, 6000,0,6000);
    mt2_fromb->SetDirectory(rootdir);
    mt2_fromb->Sumw2();

    mt2_hz = new TH1D("mt2_hz", "MT2 From Higgs and Z for "+g_sample_name, 6000,0,6000);
    mt2_hz->SetDirectory(rootdir);
    mt2_hz->Sumw2();

    sum_pt_z_bb = new TH1D("sum_pt_z_bb", "P_{T}(Z) + P_{T}(BB) for "+g_sample_name, 6000,0,6000);
    sum_pt_z_bb->SetDirectory(rootdir);
    sum_pt_z_bb->Sumw2();

    MT2_MT2B = new TH2D("MT2_MT2B", "MT2 vs. MT2b for "+g_sample_name, 6000, 0, 6000, 6000, 0, 6000);
    MT2_MT2B->SetDirectory(rootdir);
    MT2_MT2B->Sumw2();

    MT2_MT2_fromb = new TH2D("MT2_MT2_fromb", "MT2 vs. MT2(made from b-tagged jets) for "+g_sample_name, 6000, 0, 6000, 6000, 0, 6000);
    MT2_MT2_fromb->SetDirectory(rootdir);
    MT2_MT2_fromb->Sumw2();

    MT2_MT2_HZ = new TH2D("MT2_MT2_HZ", "MT2(from leptons) vs. MT2(from Higgs and Z) for "+g_sample_name, 6000, 0, 6000, 6000, 0, 6000);
    MT2_MT2_HZ->SetDirectory(rootdir);
    MT2_MT2_HZ->Sumw2();
  }

  cout<<"Histograms initialized"<<endl;
  //cout<<__LINE__<<endl;
//==============
// Setup Stuff Pulled From External Files
//==============
  int eventsInFile;
  //Set up manual vertex reweighting.  
  if( conf->get("reweight") == "true" ){
    readyReweightHists();
  }

  if(conf->get("data") != "true"){
    cout<<"Pileup reweighting with nvtx_ratio_true_26p4fb.root"<<endl;
    g_pileup_hist_file = TFile::Open("auxFiles/nvtx_ratio_true_26p4fb.root", "READ");
    //cout<<__LINE__<<endl;
    g_pileup_hist = (TH1D*)g_pileup_hist_file->Get("h_vtx_ratio")->Clone("h_pileup_weight");
    //cout<<__LINE__<<endl;
    g_pileup_hist->SetDirectory(rootdir);
    //cout<<__LINE__<<endl;
    g_pileup_hist_file->Close();
  }
  //cout<<__LINE__<<endl;
  /*if( conf->get("data") == "true" && conf->get("gjets")=="true" && (! (conf->get("dil_sign") == "opposite")) ){
    cout<<"Pileup reweighting with "<<savePath+"L1PrescaleWeight_"+conf->get("signal_region")+".root"<<endl;
    g_l1prescale_file = TFile::Open(savePath+"L1PrescaleWeight_"+conf->get("signal_region")+".root", "READ");
    
    g_l1prescale_hist36 = (TH1D*)g_l1prescale_file->Get("rwt_nVert_HLT_Photon36_R9Id90_HE10_IsoM")->Clone("l1prescaleWeight36");
    g_l1prescale_hist36->SetDirectory(rootdir);

    g_l1prescale_hist30 = (TH1D*)g_l1prescale_file->Get("rwt_nVert_HLT_Photon30_R9Id90_HE10_IsoM")->Clone("l1prescaleWeight30");
    g_l1prescale_hist30->SetDirectory(rootdir);

    g_l1prescale_hist22 = (TH1D*)g_l1prescale_file->Get("rwt_nVert_HLT_Photon22_R9Id90_HE10_IsoM")->Clone("l1prescaleWeight22");
    g_l1prescale_hist22->SetDirectory(rootdir);

    g_l1prescale_file->Close();
  }*/
  //cout<<__LINE__<<endl;
  
  if( conf->get("reweight_eff") == "true" ){
    cout<<"Reweighting for Effeciency with trigeff_Photon165_zmet2016.root"<<endl;
    TFile weight_eff_file("auxFiles/trigeff_Photon165_zmet2016.root", "READ");
    
    //barrel
    g_vpt_eff_barrel = (TEfficiency*)weight_eff_file.Get("h_pt_eb_eff_jetht")->Clone("g_vpt_eff_barrel");
    g_vpt_eff_barrel->SetDirectory(rootdir);

    //endcap
    g_vpt_eff_endcap = (TEfficiency*)weight_eff_file.Get("h_pt_ee_eff_jetht")->Clone("g_vpt_eff_barrel");
    g_vpt_eff_endcap->SetDirectory(rootdir);
    
    weight_eff_file.Close();
  }

  //cout<<__LINE__<<endl;
  //set goodrun list
  const char* json_file = "auxFiles/golden_json_260916_26p4fb_snt.txt"; // 26.4 fb
  cout<<"Setting good run list: "<<json_file<<endl;
  set_goodrun_file(json_file);

  //cout<<__LINE__<<endl;
  // Loop over events to Analyze
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();
  int eventCount=0;
  cout<<"DATASET: "<<conf->get("data_set")<<endl;
  if( nEvents >= 0 ) nEventsChain = nEvents;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  //cout<<__LINE__<<endl;
//==============
// File Loop
//==============
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TFile file( currentFile->GetTitle() );
    TTree *tree = (TTree*)file.Get("t");
    if(fast) TTreeCache::SetLearnEntries(10); //What does this do?
    if(fast) tree->SetCacheSize(128*1024*1024); //What does this do?
    phys.Init(tree); //Loads in all the branches
    //cout<<__LINE__<<endl;
    eventsInFile = 0;
//===========================================
// Loop over Events in current file
//===========================================
    if( nEventsTotal >= nEventsChain ) continue;
    unsigned int nEventsTree = tree->GetEntriesFast();
    for( unsigned int event = 0; event < nEventsTree; ++event) {
      numEvents->Fill(0);
      // Get Event Content
      if( nEventsTotal >= nEventsChain ) continue;
      if(fast) tree->LoadTree(event);
      phys.GetEntry(event);
      ++nEventsTotal;
      //cout<<__LINE__<<endl;    
      // Progress
      ZMET2016::progress( nEventsTotal, nEventsChain );
      //eventsInFile++;
      //if (eventsInFile > 100) continue;
      //cout<<__LINE__<<endl;
//=======================================
// Debugging And Odd Corrections Before Cuts
//=======================================
      printStats = false;
      printFail = false;
      
      //Low HT from Zjets inclusive sample
      if ( (! phys.isData()) && TString(conf->get("data_set")).Contains("ZMC-inclusive")){
        //cout<<"Zjets MC event"<<endl;
        if( ! TString(currentFile->GetTitle()).Contains("_ht") ){
          //cout<<"File: "<<currentFile->GetTitle()<<" with gen_ht: "<<phys.gen_ht()<<endl;
          if( phys.gen_ht() > 100 ) {
            //cout<<"skipped"<<endl;
            numEvents->Fill(44);
            continue;
          }
        }
      }
      //Low HT from Wjets inclusive sample
      if ( conf->get("data_set") == "GammaMC-wjets-inclusive"  && conf->get("event_type") == "photon" ){
        //cout<<"Zjets MC event"<<endl;
        if( TString(currentFile->GetTitle()).Contains("wjets_incl") ){
          //cout<<"File: "<<currentFile->GetTitle()<<" with gen_ht: "<<phys.gen_ht()<<endl;
          if( phys.gen_ht() > 100 ) {
            //cout<<"skipped"<<endl;
            numEvents->Fill(44);
            continue;
          }
        }
      }
      /*if ( inspection_set.count(phys.evt()) != 0){
        printStats=true;
      }*/
      /*if (conf->get("data_set") == "ttv"){
        //cout<<__LINE__<<endl;
        if ( inVinceNotMine.count(phys.evt()) != 0){
          printFail = true;
          if (phys.nlep() > 0){
            cout<<phys.evt()<<" - jets: "<<phys.njets()<<" lep2_pt: "<<phys.lep_pt().at(1)<<" genZ: "<<phys.genPart_motherId().at(0)<<" dilmass: "<<phys.dilmass()<<"evt_type: "<<phys.evt_type()<<" nlep: "<<phys.nlep()<<endl;
          }
          else{
            cout<<phys.evt()<<" - jets: "<<phys.njets()<<" genZ: "<<phys.genPart_motherId().at(0)<<" dilmass: "<<phys.dilmass()<<"evt_type: "<<phys.evt_type()<<" nlep: "<<phys.nlep()<<endl; 
          }
        }
        if ( inMineNotVince.count(phys.evt()) != 0){
          printPass = true;
        }
      }*/
//=======================================
// Cuts
//=======================================
      //cout<<__LINE__<<endl;      
      //Set up event weight
      /*if (event % 10000 == 0){
        cout<<"Weight: "<<weight<<endl;
      }*/
      if ( isDuplicate() ){
        //cout<<"Failed Duplicate"<<endl;
        continue;
      } // check for duplicates
      //cout<<__LINE__<<endl;      

      if (! passBaseCut()){ 
        //cout<<"Failed Baseline"<<endl;
        continue; 
      }// Base Cut
      //cout<<__LINE__<<endl;      

      if (! hasGoodBoson()){
        //cout<<"Failed Good Boson"<<endl;
        continue; // Boson Specific Cuts
      }
      //cout<<__LINE__<<endl;      

      if (! passSignalRegionCuts()){ 
        //cout<<"Failed SR"<<endl;
        continue; // Signal Region Cuts
      }
      //cout<<__LINE__<<endl;

      if (conf->get("rares") == "true"){ 
        if ( ! passRareCuts() ){
          //cout<<"Failed Rare Cuts"<<endl;
          continue;
        } //Rare Sample Selections
      }

      if (conf->get("susy_mc") == "true"){
        if (! passSUSYSingalCuts()){
          //cout<<"Failed SUSY Signal"<<endl;
          continue;  
        }
      }
      //double weight=1;
      double weight = getWeight();
//=======================================
// Analysis Code
//=======================================
      if (conf->get("do_MET_filters") == "true" && (! passMETFilters())) continue; ///met filters
      //cout<<__LINE__<<endl;
      //cout<<"Event Weight "<<weight<<endl;      
      //Fill in Histos
      double sumMETFilters = phys.Flag_HBHENoiseFilter()+phys.Flag_HBHEIsoNoiseFilter()+phys.Flag_CSCTightHaloFilter()+phys.Flag_EcalDeadCellTriggerPrimitiveFilter()+phys.Flag_goodVertices()+phys.Flag_eeBadScFilter();
      //cout<<__LINE__<<endl;      
      numMETFilters->Fill(sumMETFilters);

      /*if (weight < 0){
         cout<<"Negative Weight2: "<<weight<<" "<<phys.evt()<<endl;
      }
      */
      if (phys.met_T1CHS_miniAOD_CORE_pt() != 0) {
        t1met->Fill(phys.met_T1CHS_miniAOD_CORE_pt(), weight);
        t1met_widebin->Fill(phys.met_T1CHS_miniAOD_CORE_pt(), weight);
      }
      if (phys.met_rawPt() != 0) rawmet->Fill(phys.met_rawPt(), weight);
      if (phys.ht() != 0) {
        ht->Fill(phys.ht(), weight);
        ht_wide->Fill(phys.ht(), weight);
      }
      if (phys.gen_ht() != 0) gen_ht->Fill(phys.gen_ht(), weight);
      if (bosonPt() != 0){ 
        vpt->Fill(bosonPt(), weight); 
        vpt_flat->Fill(bosonPt(), weight); 
      }
      njets->Fill(phys.njets(), weight);
      nbtags_m->Fill(phys.nBJetMedium(), weight);
      nbtags_l->Fill(phys.nBJetLoose(), weight);
      nbtags_t->Fill(phys.nBJetTight(), weight);
      nVert->Fill(phys.nVert(), weight);
      nlep->Fill(phys.nlep(), weight);
      //cout<<"Filling nisotrack"<<endl;
      nisotrack->Fill(phys.nisoTrack_mt2(), weight);
      //cout<<__LINE__<<endl;
      if (phys.mt2() != 0 ) mt2->Fill(phys.mt2(), weight);
      //cout<<__LINE__<<endl;
      if (phys.mt2b() != 0 ) mt2b->Fill(phys.mt2b(), weight);
      //cout<<__LINE__<<endl;
      dphi_jet1_met->Fill(acos(cos(phys.met_T1CHS_miniAOD_CORE_phi() - phys.jets_p4().at(0).phi())), weight);
      //cout<<__LINE__<<endl;
      dphi_jet2_met->Fill(acos(cos(phys.met_T1CHS_miniAOD_CORE_phi() - phys.jets_p4().at(1).phi())), weight);
      //cout<<__LINE__<<endl;
      dilmass->Fill(phys.dilmass(), weight);

//=======================================
// Signal Region Specific Histos
//=======================================
      double pt_bb;
      if (conf->get("signal_region") == "TChiHZ"){
        sum_mlb->Fill(phys.sum_mlb(), weight);

        m_bb_csv->Fill(phys.mbb_csv(), weight);
        m_bb_bpt->Fill(phys.mbb_bpt(), weight);
    
        if (phys.mt2j() != 0 ) mt2j->Fill(phys.mt2j(), weight);

        double mt2_val_fromb = getMT2ForBjets();
        if (mt2_val_fromb != 0) mt2_fromb->Fill(mt2_val_fromb, weight);
        
        double mt2_val_hz = getMT2HiggsZ();
        if (mt2_val_hz != 0) mt2_hz->Fill(mt2_val_hz, weight);

        if (phys.mt2() != 0 && phys.mt2b() != 0 ) MT2_MT2B->Fill(phys.mt2(), phys.mt2b(), weight);

        if (phys.mt2() != 0 && mt2_val_fromb != 0 ) MT2_MT2_fromb->Fill(phys.mt2(), mt2_val_fromb, weight);

        if (phys.mt2() != 0 && mt2_val_hz != 0 ) MT2_MT2_HZ->Fill(phys.mt2(), mt2_val_hz, weight);

        if (phys.nBJetMedium() >= 2){
          pair<int,int> b_index = getMostBlike();
          
          double bb_pt = (phys.jets_p4().at(b_index.first) + phys.jets_p4().at(b_index.second)).pt();
          sum_pt_z_bb->Fill(bb_pt+phys.dilpt(), weight);
        }
    }

//=======================================
// Debugging And Odd Corrections After Cuts
//=======================================
      /*if (conf->get("rares") == "true"){
        //cout<<__LINE__<<endl;
        //cout<<"EVENT-LIST "<<eventCount<<" : "<<phys.evt()<<endl;
          //cout<<__LINE__<<endl;
        cout<<"EVENT-LIST "<<eventCount<<" : "<<phys.evt()<<" "<<phys.met_T1CHS_miniAOD_CORE_pt()<<endl;
        eventCount++;
        if ( inVinceNotMine.count(phys.evt()) != 0){
          //printPass = true;
        }
        if ( inMineNotVince.count(phys.evt()) != 0){
          printFail = true;
        }
      }*/
      eventCount++;
      
      //if (printStats) {cout<<"Event: "<<phys.evt()<<endl;}

      //if(phys.met_T1CHS_miniAOD_CORE_pt() >= 300){
      //  cout<<"Event: "<<phys.evt()<<" MET: "<<phys.met_T1CHS_miniAOD_CORE_pt()<<" njets: "<<phys.njets()<<" nbtags: "<<phys.nBJetMedium()<<" HT: "<<phys.ht()<<endl;
      //}

      /*if(phys.HLT_Photon165_R9Id90_HE10_IsoM() > 0) pt_HLT_Photon165_R9Id90_HE10_IsoM->Fill(bosonPt());
      if(phys.HLT_Photon120_R9Id90_HE10_IsoM() > 0) pt_HLT_Photon120_R9Id90_HE10_IsoM->Fill(bosonPt());
      if(phys.HLT_Photon90_R9Id90_HE10_IsoM() > 0) pt_HLT_Photon90_R9Id90_HE10_IsoM->Fill(bosonPt());
      if(phys.HLT_Photon75_R9Id90_HE10_IsoM() > 0) pt_HLT_Photon75_R9Id90_HE10_IsoM->Fill(bosonPt());
      if(phys.HLT_Photon50_R9Id90_HE10_IsoM() > 0) pt_HLT_Photon50_R9Id90_HE10_IsoM->Fill(bosonPt());
      if(phys.HLT_Photon36_R9Id90_HE10_IsoM() > 0) pt_HLT_Photon36_R9Id90_HE10_IsoM->Fill(bosonPt());
      if(phys.HLT_Photon30_R9Id90_HE10_IsoM() > 0) pt_HLT_Photon30_R9Id90_HE10_IsoM->Fill(bosonPt());
      if(phys.HLT_Photon22_R9Id90_HE10_IsoM() > 0) pt_HLT_Photon22_R9Id90_HE10_IsoM->Fill(bosonPt());*/

      if ( phys.isData() && conf->get("event_type") == "photon" && conf->get("data") == "true" && phys.ngamma() > 0) //if photon data
      {
        //cout<<__LINE__<<endl;
        if( ( phys.HLT_Photon22_R9Id90_HE10_IsoM()  > 0 || phys.HLT_Photon30_R9Id90_HE10_IsoM()  > 0 || phys.HLT_Photon36_R9Id90_HE10_IsoM()  > 0 || phys.HLT_Photon50_R9Id90_HE10_IsoM()  > 0 || phys.HLT_Photon75_R9Id90_HE10_IsoM()  > 0 || phys.HLT_Photon90_R9Id90_HE10_IsoM()  > 0 || phys.HLT_Photon120_R9Id90_HE10_IsoM() > 0 || phys.HLT_Photon165_R9Id90_HE10_IsoM() > 0 || phys.HLT_Photon165_HE10() > 0) ){
          //cout<<__LINE__<<endl;
          if( (phys.HLT_Photon165_R9Id90_HE10_IsoM() > 0 || phys.HLT_Photon165_HE10() > 0) && phys.gamma_pt().at(0) > 180 ){
            nVert_HLT_Photon165_R9Id90_HE10_IsoM->Fill(phys.nVert(), weight);
            //cout<<__LINE__<<endl;
          }
          else if( phys.HLT_Photon120_R9Id90_HE10_IsoM() > 0 && phys.gamma_pt().at(0) > 135 ){
            nVert_HLT_Photon120_R9Id90_HE10_IsoM->Fill(phys.nVert(), weight);
            //cout<<__LINE__<<endl;
          }
          else if( phys.HLT_Photon90_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 105 ){
            nVert_HLT_Photon90_R9Id90_HE10_IsoM->Fill(phys.nVert(), weight);
            //cout<<__LINE__<<endl;
          }
          else if( phys.HLT_Photon75_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 85 ){
            nVert_HLT_Photon75_R9Id90_HE10_IsoM->Fill(phys.nVert(), weight);
            //cout<<__LINE__<<endl;
          }
          else if( phys.HLT_Photon50_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 55 ){
            nVert_HLT_Photon50_R9Id90_HE10_IsoM->Fill(phys.nVert(), weight);
            //cout<<__LINE__<<endl;
          }
          else if( phys.HLT_Photon36_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) < 55 && phys.gamma_pt().at(0) > 40 ){
            nVert_HLT_Photon36_R9Id90_HE10_IsoM->Fill(phys.nVert(), weight);
            //cout<<__LINE__<<endl;
          }
          else if( phys.HLT_Photon30_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) < 40 && phys.gamma_pt().at(0) > 33 ){
            nVert_HLT_Photon30_R9Id90_HE10_IsoM->Fill(phys.nVert(), weight);
            //cout<<__LINE__<<endl;
          }
          else if( phys.HLT_Photon22_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) < 33 ){ 
            //cout<<__LINE__<<endl;
            nVert_HLT_Photon22_R9Id90_HE10_IsoM->Fill(phys.nVert(), weight);
          }
          //cout<<__LINE__<<endl;
        }
      }
    }
    // Clean Up
    //cout<<__LINE__<<endl;
    delete tree;
    //cout<<__LINE__<<endl;
    file.Close();
  }
  cout<<"Num events passed: "<<eventCount<<endl;
  if ( nEventsChain != nEventsTotal ) {
    cout << Form( "ERROR: number of events from files (%d) is not equal to total number of events (%d)", nEventsChain, nEventsTotal ) << endl;
  }  
  //cout<<__LINE__<<endl;
//=======================================
// Write Out Histos
//=======================================

  output->cd();

  //Write out histograms to file
  numEvents->Write();
  //cout<<__LINE__<<endl;
  numMETFilters->Write();
  //cout<<__LINE__<<endl;
  t1met->Write();
  //cout<<__LINE__<<endl;
  t1met_widebin->Write();
  //cout<<__LINE__<<endl;
  rawmet->Write();
  //cout<<__LINE__<<endl;
  ht->Write();
  //cout<<__LINE__<<endl;
  ht_wide->Write();
  //cout<<__LINE__<<endl;
  gen_ht->Write();
  //cout<<__LINE__<<endl;
  vpt->Write();
  //cout<<__LINE__<<endl;
  vpt_flat->Write();
  //cout<<__LINE__<<endl;
  njets->Write();
  //cout<<__LINE__<<endl;
  nlep->Write();
  //cout<<__LINE__<<endl;
  nisotrack->Write();
  //cout<<__LINE__<<endl;
  nbtags_m->Write();
  //cout<<__LINE__<<endl;
  nbtags_l->Write();
  //cout<<__LINE__<<endl;
  nbtags_t->Write();
  //cout<<__LINE__<<endl;
  nVert->Write();
  //cout<<__LINE__<<endl;
  mt2->Write();
  //cout<<__LINE__<<endl;
  mt2b->Write();
  //cout<<__LINE__<<endl;
  dphi_jet1_met->Write();
  //cout<<__LINE__<<endl;
  dphi_jet2_met->Write();
  //cout<<__LINE__<<endl;
  dilmass->Write();
  //cout<<__LINE__<<endl;

  if (conf->get("signal_region") == "TChiHZ"){
    sum_mlb->Write();
    //cout<<__LINE__<<endl;
    m_bb_csv->Write();
    //cout<<__LINE__<<endl;
    m_bb_bpt->Write();
    //cout<<__LINE__<<endl;
    mt2j->Write();
    //cout<<__LINE__<<endl;
    mt2_fromb->Write();
    //cout<<__LINE__<<endl;
    mt2_hz->Write();
    //cout<<__LINE__<<endl;
    sum_pt_z_bb->Write();
    //cout<<__LINE__<<endl;

    //2D hists
    MT2_MT2B->Write();
    //cout<<__LINE__<<endl;
    MT2_MT2_fromb->Write();
    //cout<<__LINE__<<endl;
    MT2_MT2_HZ->Write();
    //cout<<__LINE__<<endl;
  }

  if ( conf->get("event_type") == "photon" && conf->get("data") == "true" ) //if photon data
  {
    nVert_HLT_Photon165_R9Id90_HE10_IsoM->Write();
    nVert_HLT_Photon120_R9Id90_HE10_IsoM->Write();
    nVert_HLT_Photon90_R9Id90_HE10_IsoM->Write();
    nVert_HLT_Photon75_R9Id90_HE10_IsoM->Write();
    nVert_HLT_Photon50_R9Id90_HE10_IsoM->Write();
    nVert_HLT_Photon36_R9Id90_HE10_IsoM->Write();
    nVert_HLT_Photon30_R9Id90_HE10_IsoM->Write();
    nVert_HLT_Photon22_R9Id90_HE10_IsoM->Write();


    /*pt_HLT_Photon165_R9Id90_HE10_IsoM->Write();
    pt_HLT_Photon120_R9Id90_HE10_IsoM->Write();
    pt_HLT_Photon90_R9Id90_HE10_IsoM->Write();
    pt_HLT_Photon75_R9Id90_HE10_IsoM->Write();
    pt_HLT_Photon50_R9Id90_HE10_IsoM->Write();
    pt_HLT_Photon36_R9Id90_HE10_IsoM->Write();
    pt_HLT_Photon30_R9Id90_HE10_IsoM->Write();
    pt_HLT_Photon22_R9Id90_HE10_IsoM->Write();*/
  }
  //cout<<__LINE__<<endl;

  //close output file
  output->Write();
  output->Close();
  g_reweight_pairs.clear();
  //cout<<__LINE__<<endl;
  // return
  bmark->Stop("benchmark");
  cout << endl;
  cout << nEventsTotal << " Events Processed" << endl;
  cout << nDuplicates << " Duplicates" << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time:	" << Form( "%.01f", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time:	" << Form( "%.01f", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;
  return 0;
}
