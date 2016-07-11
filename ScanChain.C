// Usage:
// > root -b doAll.C

//
// 2016 MET study looper. Written by Bobak Hashemi May 13 2016
//

// C++
#include <iostream>
#include <vector>
#include <set>


// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"

// ZMET2016
#include "ZMET2016.cc"

// CORE
#include "dorky.cc"

// Configuration parsing
#include "ConfigParser.C"

using namespace std;
using namespace zmet;
using namespace duplicate_removal;

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

//Global Vars
ConfigParser *conf;
int nDuplicates=0;
TH1D *g_vpt_weight_hist, *g_pileup_hist, *g_l1prescale_hist22, *g_l1prescale_hist30, *g_l1prescale_hist36;
TEfficiency *g_vpt_eff_barrel, *g_vpt_eff_endcap; 
TFile *g_weight_hist_file, *g_pileup_hist_file, *g_l1prescale_file;
TString g_sample_name;

TH1I *numEvents; //Holds the number of events in the whole script and the number that pass various cuts 

set<int> inVinceNotMine = {48459,104538,128205,376293,551825,620848,950618,1062947,1202682,1208144,1487916,1655035,1833726};

set<int> inMineNotVince = {65603,206256,215885,352922,823174,928555,1058265,1295652,1601624,1731568,1948640};

bool printPass = false;
bool printFail = false;

double getPrescaleWeight(){
  if( !( phys.HLT_Photon22_R9Id90_HE10_IsoM()  > 0 ||
     phys.HLT_Photon30_R9Id90_HE10_IsoM()  > 0 ||
     phys.HLT_Photon36_R9Id90_HE10_IsoM()  > 0 ||
     phys.HLT_Photon50_R9Id90_HE10_IsoM()  > 0 ||
     phys.HLT_Photon75_R9Id90_HE10_IsoM()  > 0 || 
     phys.HLT_Photon90_R9Id90_HE10_IsoM()  > 0 || 
     phys.HLT_Photon120_R9Id90_HE10_IsoM() > 0 ||
     phys.HLT_Photon165_R9Id90_HE10_IsoM() > 0 ||
     phys.HLT_Photon165_HE10() > 0
     ) ) return 0;
  //cout<<__LINE__<<endl;
  if(     (phys.HLT_Photon165_R9Id90_HE10_IsoM() > 0 ||
       phys.HLT_Photon165_HE10() > 0)            ) return phys.HLT_Photon165_R9Id90_HE10_IsoM();
  else if( phys.HLT_Photon120_R9Id90_HE10_IsoM() > 0 ) return phys.HLT_Photon120_R9Id90_HE10_IsoM();
  else if( phys.HLT_Photon90_R9Id90_HE10_IsoM()  > 0 ) return phys.HLT_Photon90_R9Id90_HE10_IsoM();
  else if( phys.HLT_Photon75_R9Id90_HE10_IsoM()  > 0 ) return phys.HLT_Photon75_R9Id90_HE10_IsoM();

  else if( phys.HLT_Photon50_R9Id90_HE10_IsoM()  > 0 &&                               phys.gamma_pt().at(0) > 50 ) return phys.HLT_Photon50_R9Id90_HE10_IsoM();
  //cout<<__LINE__<<endl;
  if( phys.HLT_Photon36_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) < 50 ) {
    return phys.HLT_Photon36_R9Id90_HE10_IsoM() * g_l1prescale_hist36->GetBinContent(g_l1prescale_hist36->FindBin(phys.nVert()));
  }
  else if( phys.HLT_Photon30_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 33 ){
    return phys.HLT_Photon30_R9Id90_HE10_IsoM() * g_l1prescale_hist30->GetBinContent(g_l1prescale_hist30->FindBin(phys.nVert()));
  }
  if( phys.HLT_Photon22_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) < 33 ) {
    return phys.HLT_Photon22_R9Id90_HE10_IsoM() *  g_l1prescale_hist22->GetBinContent(g_l1prescale_hist22->FindBin(phys.nVert()));
  }
  // else if( phys.HLT_Photon22_R9Id90_HE10_IsoM()  > 0 ) return 0;
  //cout<<__LINE__<<endl;
  return -1; // should not get here
}

bool passMETFilters(){
  bool pass = true;

  if ( phys.isData() ) {
    if ( phys.nVert() == 0 ) {
      numEvents->Fill(1);
      if (printFail) cout<<phys.evt()<<" :Failed nVerts cut"<<endl;
      pass=false;
    }
    if (!phys.Flag_HBHENoiseFilter                   ()      ){ 
      pass=false;
      if (printFail) cout<<phys.evt()<<" :Failed HBHENoiseFilter cut"<<endl;
      numEvents->Fill(2);
    }
    if (!phys.Flag_HBHEIsoNoiseFilter                ()      ){ 
      pass=false;
      if (printFail) cout<<phys.evt()<<" :Failed HBHEIsoNoiseFilter cut"<<endl;
      numEvents->Fill(3);
    } 
    if (!phys.Flag_CSCTightHalo2015Filter            ()      ){ 
      pass=false;
      if (printFail) cout<<phys.evt()<<" :Failed CSCTightHalo2015Filter cut"<<endl;
      numEvents->Fill(4);
    }
    if (!phys.Flag_EcalDeadCellTriggerPrimitiveFilter()      ) { 
      pass=false;
      if (printFail) cout<<phys.evt()<<" :Failed EcalDeadCellTriggerPrimativeFilter cut"<<endl;
      numEvents->Fill(5);
    }
    if (!phys.Flag_goodVertices                      ()      ) { 
      pass=false;
      if (printFail) cout<<phys.evt()<<" :Failed goodVerticies cut"<<endl;
      numEvents->Fill(6);
    }
    if (!phys.Flag_eeBadScFilter                     ()      ) { 
      pass=false;
      if (printFail) cout<<phys.evt()<<" :Failed eeBadScFilter cut"<<endl;
      numEvents->Fill(7);
    }

  }
  if (pass){
    if (printPass) cout<<phys.evt()<<": Passes MET Filters"<<endl;
  }
  return pass;
}

bool passBaseCut(){
  bool pass=true;
  if (! (phys.evt_passgoodrunlist() > 0)){ 
    pass=false; //golden json
    if (printFail) cout<<phys.evt()<<" :Failed golden JSON cut"<<endl;
    numEvents->Fill(8);
  } 
  if (! (phys.njets() >= 2) ){ 
    pass=false; //2 jet cut
    if (printFail) cout<<phys.evt()<<" :Failed 2 Jets cut"<<endl;
    numEvents->Fill(9);
  }

  if (printPass) cout<<phys.evt()<<": Passes Base Cuts"<<endl;
  return pass;
}

bool passMuonTriggers(){
  if ( phys.isData() ){
    //cout<<__LINE__<<endl;
    if ( conf->get("use_muon_DZ_triggers") == "true" ){
      //cout<<"Using DZ triggers"<<endl;
      //cout<<__LINE__<<endl;
      return (phys.HLT_DoubleMu() || phys.HLT_DoubleMu_tk() || phys.HLT_DoubleMu_noiso());
    }
    else{
      //cout<<"Using Non DZ triggers"<<endl;
      //cout<<__LINE__<<endl;
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
    return (phys.HLT_DoubleEl_DZ_2() || phys.HLT_DoubleEl_noiso() );
  }
  else{
    //cout<<__LINE__<<endl;
    return true; //MC always passes
  }
}

bool passHLTs(){
  if (conf->get("data_set") == "em"){
    if (phys.HLT_MuEG() || phys.HLT_MuEG_2() || phys.HLT_MuEG_noiso()){
      //good Mu/E event
      //cout<<__LINE__<<endl;
    }
    else{
      //cout<<__LINE__<<endl;
      if (printFail) cout<<phys.evt()<<" :Failed good Mu/E cut"<<endl;
      return false;
    }
  }
  else{
    //cout<<__LINE__<<endl;
    if ( phys.hyp_type() == 1 ){ //Muon Event
      //cout<<__LINE__<<endl;
      if ( ! passMuonTriggers() ){
        //cout<<__LINE__<<endl;
        if (printFail) cout<<phys.evt()<<" :Failed Muon Trigger cuts"<<endl;
        return false; 
      }
    }
    else if ( phys.hyp_type() == 0 ){
      //cout<<__LINE__<<endl;
      if ( ! passElectronTriggers() ){
        //cout<<__LINE__<<endl;
        if (printFail) cout<<phys.evt()<<" :Failed electron trigger cuts"<<endl;
        return false;
      }
    }
    else{ //hyp_type == 2 and it's not an emu event for the TTbar estimate.
      //cout<<__LINE__<<endl;
      if (printFail) cout<<phys.evt()<<" :Failed not E/Mu event with hyp_type 2 in TTBar estimate cut"<<endl;
      return false; 
    }
  }

  //cout<<__LINE__<<endl;

  if (printPass) cout<<phys.evt()<<": Passes HLT Cuts"<<endl;
  return true;
}

bool hasGoodZ(){
  if( phys.nlep() < 2         ){ 
    numEvents->Fill(10);
    if (printFail) cout<<phys.evt()<<" :Failed 2 lepton Z cut"<<endl;
    return false; // require at least 2 good leptons
  }
  
  //cout<<__LINE__<<endl;

  if( phys.lep_pt().at(0) < 25        ) {
    numEvents->Fill(11); 
    if (printFail) cout<<phys.evt()<<" :Failed lep1 pt < 25 Z cut"<<endl;
    return false; // leading lep pT > 25 GeV
  }

  //cout<<__LINE__<<endl;

  if( phys.lep_pt().at(1) < 20        ) {
    numEvents->Fill(12); 
    if (printFail) cout<<phys.evt()<<" :Failed lep2 pt < 20 Z cut"<<endl;
    return false; // tailing lep pT > 20 GeV      

  }   
  
  //cout<<__LINE__<<endl;

  if( abs(phys.lep_p4().at(0).eta())     > 2.4       ) {
    numEvents->Fill(13); 
    if (printFail) cout<<phys.evt()<<" :Failed lep1 eta > 2.4 Z cut"<<endl;
    return false; // eta < 2.4
  }    

  //cout<<__LINE__<<endl;
  
  if( abs(phys.lep_p4().at(1).eta())     > 2.4       ) {
    numEvents->Fill(14); 
    if (printFail) cout<<phys.evt()<<" :Failed lep2 eta > 2.4 Z cut"<<endl;
    return false; // eta < 2.4
  }

  //cout<<__LINE__<<endl;

  if (! passHLTs()){
    numEvents->Fill(15);
    if (printFail) cout<<phys.evt()<<" :Failed HLT Z cut"<<endl;
    return false;
  }

  //cout<<__LINE__<<endl;
  /*
  //This is the augmented cut selection.
  LorentzVector zp4 = phys.lep_p4().at(1) + phys.lep_p4().at(2);
  if( abs(zp4.eta()) > 1.4 && abs(zp4.eta())   < 1.6) {
    pass = false; // veto xition region
    numEvents->Fill(15); 
  }
  
  if( abs(zp4.eta())   > 2.4   ) {
    pass = false; // Z in EC or EB
    numEvents->Fill(16); 
  } 
  */
  
  //This is the original cut selection
  if( abs(phys.lep_p4().at(0).eta()) > 1.4 && abs(phys.lep_p4().at(0).eta()) < 1.6 ){
    numEvents->Fill(17);
    if (printFail) cout<<phys.evt()<<" :Failed lep1 in xition region Z cut"<<endl;
    return false;
  }

  //cout<<__LINE__<<endl;

  if( abs(phys.lep_p4().at(1).eta()) > 1.4 && abs(phys.lep_p4().at(1).eta()) < 1.6 ) {
    numEvents->Fill(18); 
    if (printFail) cout<<phys.evt()<<" :Failed lep2 in xition region Z cut"<<endl;
    return false; // veto xition region
  }

  //cout<<__LINE__<<endl;
  
  if( phys.dRll() < 0.1 ) {
    numEvents->Fill(19); 
    if (printFail) cout<<phys.evt()<<" :Failed deltaR Z cut"<<endl;
    return false;
  }

  //cout<<__LINE__<<endl;

  if (conf->get("data_set") == "em"){
    if (! (phys.hyp_type() == 2) ){
      numEvents->Fill(20); 
      if (printFail) cout<<phys.evt()<<" :Failed not explicit e/mu Z cut, for ttbar only"<<endl;
      return false; // require explicit opposite flavor event
    }
  }
  else{
    if( !( phys.hyp_type() == 0 || phys.hyp_type() == 1 ) ) {
        numEvents->Fill(20); 
        if (printFail) cout<<phys.evt()<<" :Failed explicit mu/mu or e/e Z cut"<<endl;
        return false; // require explicit same flavor event
    }
  }

  //cout<<__LINE__<<endl;

  if( !(phys.evt_type() == 0 ) ) {
    numEvents->Fill(21); 
    if (printFail) cout<<phys.evt()<<" :Failed evt_type=0 Z cut"<<endl;
    return false; // require opposite sign
  }

  //cout<<__LINE__<<endl;
  
  if( !(phys.dilmass() > 81 && phys.dilmass() < 101) ) {
    numEvents->Fill(22); 
    if (printFail) cout<<phys.evt()<<" :Failed Z mass window Z cut"<<endl;
    return false; // on-Z
  }

  //cout<<__LINE__<<endl;
  
  /*if( !(phys.dilpt() > 22) ){
    numEvents->Fill(23); 
    return false; // Z pT > 22 GeV
  }*/

  if (printPass) cout<<phys.evt()<<": Passes good Z Cuts"<<endl;
  return true;
}

bool hasGoodPhoton(){
  if( phys.ngamma() <  1 ) {
    numEvents->Fill(24);
    if (printFail) cout<<phys.evt()<<" :Failed at least 1 photon cut"<<endl;
    return false; // require at least 1 good photon
  }
  
  if( phys.evt_type() != 2 ) {
    numEvents->Fill(25);
    if (printFail) cout<<phys.evt()<<" :Failed explicitly evt_type2 photon cut"<<endl;
    return false; // photon + jets events
  }
  
  if( phys.gamma_pt().at(0) < 22 ) {
    numEvents->Fill(26);
    if (printFail) cout<<phys.evt()<<" :Failed pt < 22 photon cut"<<endl;
    return false; // photon pt > 22 GeV
  }
  
  if( abs(phys.gamma_p4().at(0).eta()) > 1.4 && abs(phys.gamma_p4().at(0).eta()) < 1.6 ) {
    numEvents->Fill(27);
    if (printFail) cout<<phys.evt()<<" :Failed gamm ain xition region photon cut"<<endl;
    return false; // veto xition region
  }
  
  if( abs(phys.gamma_p4().at(0).eta()) > 2.4 ) {
    numEvents->Fill(28);
    if (printFail) cout<<phys.evt()<<" :Failed gamma eta > 2.4 photon cut"<<endl;
    return false; // photon in EC or EB
  }
  
  if( phys.gamma_hOverE().at(0) > 0.1 ) {
    numEvents->Fill(29);
    if (printFail) cout<<phys.evt()<<" :Failed gamma hOverE photon cut"<<endl;
    return false; // H/E < 0.1
  }
  
  // if( phys.matched_neutralemf()          < 0.7   ) return false; // jet neutral EM fraction cut
  
  if( phys.matched_emf() < 0.7 ) {
    numEvents->Fill(30);
    if (printFail) cout<<phys.evt()<<" :Failed matched_emf photon cut"<<endl;
    return false; // jet neutral EM fraction cut
  }
  
  if( acos( cos( phys.gamma_phi().at(0) - phys.met_phi() ) ) < 0.14 ) {
    numEvents->Fill(31);
    if (printFail) cout<<phys.evt()<<" :Failed photons aligned with MET photon cut"<<endl;
    return false; // kill photons aligned with MET
  }
  
  if( phys.elveto() ) {
    numEvents->Fill(32);
    if (printFail) cout<<phys.evt()<<" :Failed electron pixel veto photon cut"<<endl;
    return false; // veto pixel match
  }
  
  if (printPass) cout<<phys.evt()<<": Passes good gamma Cuts"<<endl;
  return true;  
}

bool hasGoodBoson() {
  if (g_sample_name == "gjets") {
    return hasGoodPhoton();
  }
  else {
    return hasGoodZ();
  }
}

double bosonPt(){
  // Returns boson Pt, determines whether sample is gjets or zjets first.
  if (g_sample_name == "zjets") {
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

double getWeight(){
  /*Gets the proper weight for the sample. */
  double weight=1;

  // If we don't have data use scale to 1 fb^-1. 
  if ( ! ( phys.isData() ) ) {
    weight *= phys.evt_scale1fb();
  }

  if ( conf->get("reweight") == "true" ) {
    weight *= g_vpt_weight_hist->GetBinContent(g_vpt_weight_hist->FindBin(bosonPt()));
  }

  if ( conf->get("reweight_eff") == "true" && g_sample_name == "gjets" && phys.ngamma() > 0){
    weight *= getEff();
  }

  if (conf->get("rares") == "true"){
    weight*=g_pileup_hist->GetBinContent(g_pileup_hist->FindBin(phys.nTrueInt()));
  }

  if (phys.isData() && conf->get("data_type") == "gjets" && conf->get("data") == "true" && phys.ngamma() > 0){
    weight *= getPrescaleWeight();
  }

  return weight;
}

bool isDuplicate(){
  if( phys.isData() ) {
    DorkyEventIdentifier id(phys.run(), phys.evt(), phys.lumi());
    if (is_duplicate(id) ){
      ++nDuplicates;
      if (printFail) cout<<phys.evt()<<" :Is a duplicate"<<endl;
      return true;
    }
  }
  if (printPass) cout<<phys.evt()<<": Passes not a duplicate"<<endl;
  return false;
}

bool passSignalRegionCuts(){
  
  //Njets Min Cut
  if (conf->get("Njets_min") != ""){
    if (phys.njets() < stod(conf->get("Njets_min"))){
      numEvents->Fill(34);
      if (printFail) cout<<phys.evt()<<" :Failed min jets cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  //Njets Max Cut
  if (conf->get("Njets_max") != ""){
    if (phys.njets() > stod(conf->get("Njets_max"))){
      numEvents->Fill(35);
      if (printFail) cout<<phys.evt()<<" :Failed max jets cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  //Num Bottom jets Min Cut
  if (conf->get("NBjets_min") != ""){
    if (phys.nBJetMedium() < stod(conf->get("NBjets_min"))){
      numEvents->Fill(36);
      if (printFail) cout<<phys.evt()<<" :Failed min bjet cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  //Num Bottom jets Max Cut
  if (conf->get("NBjets_max") != ""){
    if (phys.nBJetMedium() > stod(conf->get("NBjets_max"))){
      numEvents->Fill(37);
      if (printFail) cout<<phys.evt()<<" :Failed max bjet cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  //Leading Jet/MET Phi min
  if (conf->get("dPhi_MET_j1") != ""){
    if (phys.dphi_metj1() < stod(conf->get("dPhi_MET_j1"))){
      numEvents->Fill(38);
      if (printFail) cout<<phys.evt()<<" :Failed dPhi MET with jet 1 cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  //Trailing Jet/MET Phi min
  if (conf->get("dPhi_MET_j2") != ""){
    if (phys.dphi_metj2() < stod(conf->get("dPhi_MET_j2"))){
      numEvents->Fill(39);
      if (printFail) cout<<phys.evt()<<" :Failed dPhi MET with jet 2 cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  //MT2b min
  if (conf->get("MT2b") != ""){
    if (phys.mt2b() < stod(conf->get("MT2b")) && g_sample_name == "zjets"){
      numEvents->Fill(40);
      if (printFail) cout<<phys.evt()<<" :Failed MT2b cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  //HT min
  if (conf->get("HT_min") != ""){
    if (phys.ht() < stod(conf->get("HT_min"))){
      numEvents->Fill(41);
      if (printFail) cout<<phys.evt()<<" :Failed sum HT min cut"<<endl;
      return false;
    }
  }

  //cout<<__LINE__<<endl;

  //DiBottom mass difference from Higgs Mass
  if (conf->get("mbb_mh_diff") != ""){
    if (abs(phys.mbb_csv() - 125) < stod(conf->get("mbb_mh_diff"))){
      numEvents->Fill(42);
      if (printFail) cout<<phys.evt()<<" :Failed sum mbb_mh diff cut"<<endl;
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
    
    if ( abs(phys.ht() + pt ) < stod(conf->get("sum_HT_pt_pt") ) ){
      numEvents->Fill(43);
      if (printFail) cout<<phys.evt()<<" :Failed sum HT pt pt cut"<<endl;
      return false;
    }
  }
  
  if (conf->get("lep1_pt_min") != "" && g_sample_name=="zjets" ){
    if ( phys.lep_pt().at(0) < stod( conf->get("lep1_pt_min") )){
      numEvents->Fill(45);
      if (printFail) cout<<phys.evt()<<" :Failed lep1 min pt"<<endl;
      return false;
    }
  }

  if (conf->get("lep2_pt_min") != "" && g_sample_name=="zjets" ){
    if ( phys.lep_pt().at(1) < stod( conf->get("lep2_pt_min") )){
      numEvents->Fill(46);
      if (printFail) cout<<phys.evt()<<" :Failed lep2 min pt cut"<<endl;
      return false;
    }
  }


  if (printPass) cout<<phys.evt()<<": Passes Signal Region Cuts"<<endl;
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
      
      if( phys.genPart_motherId().at(genind) == 23 &&
        phys.genPart_status().at(genind) == 23 &&
        (abs(phys.genPart_pdgId().at(genind))==11 ||
         abs(phys.genPart_pdgId().at(genind))==13) ){
        realzpair = true;
      }
    }
  }
  
  if ( ! hasrealmet ){
    numEvents->Fill(47);
    if (printFail) cout<<phys.evt()<<" :Failed Has Real MET Rare Cut"<<endl;    
    return false;
  }
  else if ( ! realzpair ){
    numEvents->Fill(48);
    if (printFail) cout<<phys.evt()<<" :Failed Has Real Z Rare Cut"<<endl;    
    return false;
  }
  
  if (printPass) cout<<phys.evt()<<": Passes Has Real MET Rare Cut"<<endl;
  if (printPass) cout<<phys.evt()<<": Passes Has Real Z Pair Rare Cut"<<endl;
  return true;
}

int ScanChain( TChain* chain, TString sampleName, ConfigParser *configuration, bool fast = true, int nEvents = -1) {
  /* Runs through baby files and makes histogram files. 
  
  Inputs:
  chain -- contains the files to make the histograms from, 
  sampleName -- for bookkeeping since we make histograms from many different samples. 
  configuration -- pointer to the configuration object
  */  

  //Set Global Vars
  g_sample_name=sampleName;
  conf=configuration;
  TString savePath = conf->get("histo_output_dir");

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");
  
//=======================================
// Define Histograms
//=======================================
  
  TDirectory *rootdir = gDirectory->GetDirectory("Rint:");

  cout<<"Opening file "<<savePath+"ct_"+conf->get("Name")+"_"+conf->get("signal_region")+".root"<<endl;
  TFile * output = new TFile(savePath+"ct_"+conf->get("Name")+"_"+conf->get("signal_region")+".root", "recreate");

  numEvents = new TH1I(sampleName+"_numEvents", "Number of events in "+sampleName, 50, 0, 50);
  numEvents->SetDirectory(rootdir);

  //MET Histos
  TH1D *t1met = new TH1D(sampleName+"_type1MET", "Type 1 MET for "+sampleName, 6000,0,6000);
  t1met->SetDirectory(rootdir);
  t1met->Sumw2();

  //MET Histos
  TH1D *nVert = new TH1D(sampleName+"_nVert", "Number of verticies in "+sampleName, 150,0,150);
  nVert->SetDirectory(rootdir);
  nVert->Sumw2();

  TH1D *rawmet = new TH1D(sampleName+"_rawMET", "Raw MET for "+sampleName, 6000,0,6000);
  rawmet->SetDirectory(rootdir);
  rawmet->Sumw2();

  TH1D *ht = new TH1D(sampleName+"_ht", "Scalar sum of hadronic pt (HT) for "+sampleName, 6000,0,6000);
  ht->SetDirectory(rootdir);
  ht->Sumw2();

  TH1D *gen_ht = new TH1D(sampleName+"_genht", "Scalar sum of generated hadronic pt (Gen HT) for "+sampleName, 6000,0,6000);
  gen_ht->SetDirectory(rootdir);
  gen_ht->Sumw2();

  TH1D *numMETFilters = new TH1D(sampleName+"_numMETFilters", "Number of MET Filters passed for events in "+sampleName, 50,0,50);
  numMETFilters->SetDirectory(rootdir);
  numMETFilters->Sumw2();

  const int n_ptbins = 23;
  const double ptbins[n_ptbins+1] = {0, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 110, 120, 130, 140, 150, 225, 300, 6000};

  const int n_ptbins_std = 9;
  const double ptbins_std[n_ptbins_std+1] = {0, 33, 50, 75, 90, 120, 165, 200, 300, 6000};

  const int n_ptbins_atlas = 8;
  const double ptbins_atlas[n_ptbins_atlas+1] = {0, 33, 50, 75, 90, 120, 165, 200, 6000};

  TH1D *vpt;
  
  if (conf->get("signal_region") == "ATLAS")
  {
    vpt = new TH1D(sampleName+"_vpt", "Boson Pt for events in "+sampleName, n_ptbins_atlas, ptbins_atlas); 
  }
  else if (conf->get("signal_region") == "VincePhotonPT"){
    vpt = new TH1D(sampleName+"_vpt", "Boson Pt for events in "+sampleName, 6000, 0, 6000);  
  }

  else{
    vpt = new TH1D(sampleName+"_vpt", "Boson Pt for events in "+sampleName, n_ptbins_std, ptbins_std);  
  }

  vpt->SetDirectory(rootdir);
  vpt->Sumw2();

  TH1D *vpt_150;
  TH1D *vpt_225;
  TH1D *vpt_300;

  TH1D *met_150;
  TH1D *met_225;
  TH1D *met_300;

  if (conf->get("signal_region") == "VincePhotonPT"){
    vpt_150 = new TH1D(sampleName+"_vpt150", "Boson Pt for events with > 150GeV of MET in "+sampleName, 6000, 0, 6000);  
    vpt_225 = new TH1D(sampleName+"_vpt225", "Boson Pt for events with > 225GeV of MET in "+sampleName, 6000, 0, 6000);  
    vpt_300 = new TH1D(sampleName+"_vpt300", "Boson Pt for events with > 300GeV of MET in "+sampleName, 6000, 0, 6000);  

    met_150 = new TH1D(sampleName+"_met150", "MET for events with > 150GeV of MET in "+sampleName, 6000, 0, 6000);  
    met_225 = new TH1D(sampleName+"_met225", "MET for events with > 225GeV of MET in "+sampleName, 6000, 0, 6000);  
    met_300 = new TH1D(sampleName+"_met300", "MET for events with > 300GeV of MET in "+sampleName, 6000, 0, 6000);  

    vpt_150->SetDirectory(rootdir);
    vpt_150->Sumw2();

    vpt_225->SetDirectory(rootdir);
    vpt_225->Sumw2();

    vpt_300->SetDirectory(rootdir);
    vpt_300->Sumw2();

    met_150->SetDirectory(rootdir);
    met_150->Sumw2();

    met_225->SetDirectory(rootdir);
    met_225->Sumw2();

    met_300->SetDirectory(rootdir);
    met_300->Sumw2();
  }


  TH1D *njets = new TH1D(sampleName+"_njets", "Number of jets for events in "+sampleName, 50,0,50);
  njets->SetDirectory(rootdir);
  njets->Sumw2();

  TH1D *nbtags_m = new TH1D(sampleName+"_nbtags_m", "Number of \"medium\" B-tagged jets for events in "+sampleName, 50,0,50);
  nbtags_m->SetDirectory(rootdir);
  nbtags_m->Sumw2();

  TH1D *nbtags_l = new TH1D(sampleName+"_nbtags_l", "Number of \"loose\" B-tagged jets for events in "+sampleName, 50,0,50);
  nbtags_l->SetDirectory(rootdir);
  nbtags_l->Sumw2();

  TH1D *nbtags_t = new TH1D(sampleName+"_nbtags_t", "Number of \"tight\" B-tagged jets for events in "+sampleName, 50,0,50);
  nbtags_t->SetDirectory(rootdir);
  nbtags_t->Sumw2();

  TH1D *nVert_HLT_Photon165_R9Id90_HE10_IsoM = new TH1D(sampleName+"_nVert_HLT_Photon165_R9Id90_HE10_IsoM", "Number of verticies for HLT_Photon165_R9Id90_HE10_IsoM",150,0,150);
  TH1D *nVert_HLT_Photon120_R9Id90_HE10_IsoM = new TH1D(sampleName+"_nVert_HLT_Photon120_R9Id90_HE10_IsoM", "Number of verticies for HLT_Photon120_R9Id90_HE10_IsoM",150,0,150);
  TH1D *nVert_HLT_Photon90_R9Id90_HE10_IsoM = new TH1D(sampleName+"_nVert_HLT_Photon90_R9Id90_HE10_IsoM", "Number of verticies for HLT_Photon90_R9Id90_HE10_IsoM",150,0,150);
  TH1D *nVert_HLT_Photon75_R9Id90_HE10_IsoM = new TH1D(sampleName+"_nVert_HLT_Photon75_R9Id90_HE10_IsoM", "Number of verticies for HLT_Photon75_R9Id90_HE10_IsoM",150,0,150);
  TH1D *nVert_HLT_Photon50_R9Id90_HE10_IsoM = new TH1D(sampleName+"_nVert_HLT_Photon50_R9Id90_HE10_IsoM", "Number of verticies for HLT_Photon50_R9Id90_HE10_IsoM",150,0,150);
  TH1D *nVert_HLT_Photon36_R9Id90_HE10_IsoM = new TH1D(sampleName+"_nVert_HLT_Photon36_R9Id90_HE10_IsoM", "Number of verticies for HLT_Photon36_R9Id90_HE10_IsoM",150,0,150);
  TH1D *nVert_HLT_Photon30_R9Id90_HE10_IsoM = new TH1D(sampleName+"_nVert_HLT_Photon30_R9Id90_HE10_IsoM", "Number of verticies for HLT_Photon30_R9Id90_HE10_IsoM",150,0,150);
  TH1D *nVert_HLT_Photon22_R9Id90_HE10_IsoM = new TH1D(sampleName+"_nVert_HLT_Photon22_R9Id90_HE10_IsoM", "Number of verticies for HLT_Photon22_R9Id90_HE10_IsoM",150,0,150);


  cout<<"Histograms initialized"<<endl;
  //cout<<__LINE__<<endl;

  //Set up manual vertex reweighting.  
  if( conf->get("reweight") == "true" ){
    cout<<"Reweighting with "<<TString(conf->get("histo_output_dir")+"ct_"+conf->get("rwt_var")+"_"+conf->get("signal_region")+"_rwt.root")<<endl;
    g_weight_hist_file = TFile::Open( TString(conf->get("histo_output_dir")+"ct_"+conf->get("rwt_var")+"_"+conf->get("signal_region")+"_rwt.root"), "READ");
    g_vpt_weight_hist = (TH1D*)g_weight_hist_file->Get("h_vpt_ratio")->Clone("h_vpt_weight");
    g_vpt_weight_hist->SetDirectory(rootdir);
    g_weight_hist_file->Close();
  }

  if( conf->get("rares") == "true" ){
    cout<<"Pileup reweighting with nvtx_ratio_4p0fb.root"<<endl;
    g_pileup_hist_file = TFile::Open("nvtx_ratio_4p0fb.root", "READ");
    g_pileup_hist = (TH1D*)g_pileup_hist_file->Get("h_vtx_ratio")->Clone("h_pileup_weight");
    g_pileup_hist->SetDirectory(rootdir);
    g_pileup_hist_file->Close();
  }

  if( conf->get("data") == "true" ){
    cout<<"Pileup reweighting with "<<savePath+"L1PrescaleWeight_"+conf->get("signal_region")+".root"<<endl;
    g_l1prescale_file = TFile::Open(savePath+"L1PrescaleWeight_"+conf->get("signal_region")+".root", "READ");
    
    g_l1prescale_hist36 = (TH1D*)g_l1prescale_file->Get("rwt_nVert_HLT_Photon36_R9Id90_HE10_IsoM")->Clone("l1prescaleWeight36");
    g_l1prescale_hist36->SetDirectory(rootdir);

    g_l1prescale_hist30 = (TH1D*)g_l1prescale_file->Get("rwt_nVert_HLT_Photon30_R9Id90_HE10_IsoM")->Clone("l1prescaleWeight30");
    g_l1prescale_hist30->SetDirectory(rootdir);

    g_l1prescale_hist22 = (TH1D*)g_l1prescale_file->Get("rwt_nVert_HLT_Photon22_R9Id90_HE10_IsoM")->Clone("l1prescaleWeight22");
    g_l1prescale_hist22->SetDirectory(rootdir);

    g_l1prescale_file->Close();
  }
  
  if( conf->get("reweight_eff") == "true" ){
    cout<<"Reweighting for Effeciency with trigeff_Photon165_zmet2016.root"<<endl;
    TFile weight_eff_file("trigeff_Photon165_zmet2016.root", "READ");
    
    //barrel
    g_vpt_eff_barrel = (TEfficiency*)weight_eff_file.Get("h_pt_eb_eff_jetht")->Clone("g_vpt_eff_barrel");
    g_vpt_eff_barrel->SetDirectory(rootdir);

    //endcap
    g_vpt_eff_endcap = (TEfficiency*)weight_eff_file.Get("h_pt_ee_eff_jetht")->Clone("g_vpt_eff_barrel");
    g_vpt_eff_endcap->SetDirectory(rootdir);
    
    weight_eff_file.Close();
  }


  //cout<<__LINE__<<endl;
  // Loop over events to Analyze
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();
  if( nEvents >= 0 ) nEventsChain = nEvents;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
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
      //cout<<__LINE__<<endl;
//=======================================
// Analysis Code
//=======================================
      printPass = false;
      printFail = false;
      if( TString(currentFile->GetTitle()).Contains("zjetsInclusive") ){
        if( phys.gen_ht() > 100 ) {
          numEvents->Fill(44);
          continue;
        }
      }
      
      if (conf->get("data_set") == "wz"){
        //cout<<__LINE__<<endl;
        /*if (phys.met_T1CHS_miniAOD_CORE_pt() >= 225){
          //cout<<__LINE__<<endl;
          cout<<"EVENT-LIST "<<phys.evt()<<" "<<phys.met_T1CHS_miniAOD_CORE_pt()<<endl;
        }*/
        if ( inVinceNotMine.count(phys.evt()) != 0){
          printPass = true;
        }
        if ( inMineNotVince.count(phys.evt()) != 0){
          printFail = true;
        }
      }

      //cout<<__LINE__<<endl;      
      //Set up event weight
      double weight = getWeight();
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

      if (conf->get("do_MET_filters") == "true" && (! passMETFilters())) continue; ///met filters
      //cout<<__LINE__<<endl;      
      //Fill in Histos
      double sumMETFilters = phys.Flag_HBHENoiseFilter()+phys.Flag_HBHEIsoNoiseFilter()+phys.Flag_CSCTightHaloFilter()+phys.Flag_EcalDeadCellTriggerPrimitiveFilter()+phys.Flag_goodVertices()+phys.Flag_eeBadScFilter();
      //cout<<__LINE__<<endl;      
      numMETFilters->Fill(sumMETFilters);
      if (phys.met_T1CHS_miniAOD_CORE_pt() != 0) t1met->Fill(phys.met_T1CHS_miniAOD_CORE_pt(), weight);
      if (phys.met_rawPt() != 0) rawmet->Fill(phys.met_rawPt(), weight);
      if (phys.ht() != 0) ht->Fill(phys.ht(), weight);
      if (phys.gen_ht() != 0) gen_ht->Fill(phys.gen_ht(), weight);
      if (bosonPt() != 0) vpt->Fill(bosonPt(), weight);
      njets->Fill(phys.njets(), weight);
      nbtags_m->Fill(phys.nBJetMedium(), weight);
      nbtags_l->Fill(phys.nBJetLoose(), weight);
      nbtags_t->Fill(phys.nBJetTight(), weight);
      nVert->Fill(phys.nVert(), weight);
      //cout<<__LINE__<<endl;

      //cout<<__LINE__<<endl;


      //Vince's Photon plots
      if (conf->get("signal_region") == "VincePhotonPT" && phys.HLT_Photon165_R9Id90_HE10_IsoM()){
        if (phys.met_T1CHS_miniAOD_CORE_pt() >= 150){
          met_150->Fill(phys.met_T1CHS_miniAOD_CORE_pt(), weight);
          vpt_150->Fill(bosonPt(), weight);
        } 
        if (phys.met_T1CHS_miniAOD_CORE_pt() >= 225){
          met_225->Fill(phys.met_T1CHS_miniAOD_CORE_pt(), weight);
          vpt_225->Fill(bosonPt(), weight);
        } 
        if (phys.met_T1CHS_miniAOD_CORE_pt() >= 300){
          met_300->Fill(phys.met_T1CHS_miniAOD_CORE_pt(), weight);
          vpt_300->Fill(bosonPt(), weight);
        } 
      }

      if ( phys.isData() && conf->get("data_type") == "gjets" && conf->get("data") == "true" && phys.ngamma() > 0) //if photon data
      {
        //cout<<__LINE__<<endl;
        if( ( phys.HLT_Photon22_R9Id90_HE10_IsoM()  > 0 || phys.HLT_Photon30_R9Id90_HE10_IsoM()  > 0 || phys.HLT_Photon36_R9Id90_HE10_IsoM()  > 0 || phys.HLT_Photon50_R9Id90_HE10_IsoM()  > 0 || phys.HLT_Photon75_R9Id90_HE10_IsoM()  > 0 || phys.HLT_Photon90_R9Id90_HE10_IsoM()  > 0 || phys.HLT_Photon120_R9Id90_HE10_IsoM() > 0 || phys.HLT_Photon165_R9Id90_HE10_IsoM() > 0 || phys.HLT_Photon165_HE10() > 0) ){
          //cout<<__LINE__<<endl;
          if( (phys.HLT_Photon165_R9Id90_HE10_IsoM() > 0 || phys.HLT_Photon165_HE10() > 0) ){
            nVert_HLT_Photon165_R9Id90_HE10_IsoM->Fill(phys.nVert(), weight);
            //cout<<__LINE__<<endl;
          }
          else if( phys.HLT_Photon120_R9Id90_HE10_IsoM() > 0 ){
            nVert_HLT_Photon120_R9Id90_HE10_IsoM->Fill(phys.nVert(), weight);
            //cout<<__LINE__<<endl;
          }
          else if( phys.HLT_Photon90_R9Id90_HE10_IsoM()  > 0 ){
            nVert_HLT_Photon90_R9Id90_HE10_IsoM->Fill(phys.nVert(), weight);
            //cout<<__LINE__<<endl;
          }
          else if( phys.HLT_Photon75_R9Id90_HE10_IsoM()  > 0 ){
            nVert_HLT_Photon75_R9Id90_HE10_IsoM->Fill(phys.nVert(), weight);
            //cout<<__LINE__<<endl;
          }
          else if( phys.HLT_Photon50_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 50 ){
            nVert_HLT_Photon50_R9Id90_HE10_IsoM->Fill(phys.nVert(), weight);
            //cout<<__LINE__<<endl;
          }
          else if( phys.HLT_Photon36_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) < 50 ){
            nVert_HLT_Photon36_R9Id90_HE10_IsoM->Fill(phys.nVert(), weight);
            //cout<<__LINE__<<endl;
          }
          else if( phys.HLT_Photon30_R9Id90_HE10_IsoM()  > 0 && phys.gamma_pt().at(0) > 33 ){
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
    delete tree;
    file.Close();
  }

  if ( nEventsChain != nEventsTotal ) {
    cout << Form( "ERROR: number of events from files (%d) is not equal to total number of events (%d)", nEventsChain, nEventsTotal ) << endl;
  }
  
//=======================================
// Write Out Histos
//=======================================

  output->cd();

  //Write out histograms to file
  numEvents->Write();
  numMETFilters->Write();
  t1met->Write();
  rawmet->Write();
  ht->Write();
  gen_ht->Write();
  vpt->Write();
  njets->Write();
  nbtags_m->Write();
  nbtags_l->Write();
  nbtags_t->Write();
  nVert->Write();
  
  if (conf->get("signal_region") == "VincePhotonPT"){
    met_150->Write();
    vpt_150->Write();
    met_225->Write();
    vpt_225->Write();
    met_300->Write();
    vpt_300->Write();
  }

  if ( phys.isData() && conf->get("data_type") == "gjets" && conf->get("data") == "true" ) //if photon data
  {
    nVert_HLT_Photon165_R9Id90_HE10_IsoM->Write();
    nVert_HLT_Photon120_R9Id90_HE10_IsoM->Write();
    nVert_HLT_Photon90_R9Id90_HE10_IsoM->Write();
    nVert_HLT_Photon75_R9Id90_HE10_IsoM->Write();
    nVert_HLT_Photon50_R9Id90_HE10_IsoM->Write();
    nVert_HLT_Photon36_R9Id90_HE10_IsoM->Write();
    nVert_HLT_Photon30_R9Id90_HE10_IsoM->Write();
    nVert_HLT_Photon22_R9Id90_HE10_IsoM->Write();
  }

  //close output file
  output->Write();
  output->Close();
  
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
