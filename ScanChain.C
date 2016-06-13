// Usage:
// > root -b doAll.C

//
// 2016 MET study looper. Written by Bobak Hashemi May 13 2016
//

// C++
#include <iostream>
#include <vector>

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
TH1F *g_vpt_weight_hist;
TFile *g_weight_hist_file;
TString g_sample_name;

TH1I *numEvents; //Holds the number of events in the whole script and the number that pass various cuts 


bool passMETFilters(){
  bool pass = true;

  if ( phys.isData() ) {
    if ( phys.nVert() == 0 ) {
      numEvents->Fill(1);
      pass=false;
    }
    if (!phys.Flag_HBHENoiseFilter                   ()      ){ 
      pass=false;
      numEvents->Fill(2);
    }
    if (!phys.Flag_HBHEIsoNoiseFilter                ()      ){ 
      pass=false;
      numEvents->Fill(3);
    } 
    if ( phys.Flag_CSCTightHalo2015Filter_isLoaded ){
      if (!phys.Flag_CSCTightHalo2015Filter            ()      ){ 
        pass=false;
        numEvents->Fill(4);
      }
    }
    else{
      if (!phys.Flag_CSCTightHaloFilter            ()      ) { 
        pass=false;
        numEvents->Fill(4);
      }
    }
    if (!phys.Flag_EcalDeadCellTriggerPrimitiveFilter()      ) { 
      pass=false;
      numEvents->Fill(5);
    }
    if (!phys.Flag_goodVertices                      ()      ) { 
      pass=false;
      numEvents->Fill(6);
    }
    if (!phys.Flag_eeBadScFilter                     ()      ) { 
      pass=false;
      numEvents->Fill(7);
    }

  }
  return pass;
}

bool passBaseCut(){
  bool pass=true;
  if (! (phys.evt_passgoodrunlist() > 0)){ 
    pass=false; //golden json
    numEvents->Fill(8);
  } 
  if (! (phys.njets() >= 2) ){ 
    pass=false; //2 jet cut
    numEvents->Fill(9);
  } 

  return pass;
}

bool hasGoodZ(){
  bool pass = true;

  if( phys.nlep() < 2         ){ 
    pass= false; // require at least 2 good leptons
    numEvents->Fill(10);
  }
cout<<__LINE__<<endl;      

  
  if( phys.lep_pt().at(0) < 20        ) {
    pass = false; // leading lep pT > 20 GeV
    numEvents->Fill(11); 
  }
cout<<__LINE__<<endl;      
  
  if( phys.lep_pt().at(1)                < 20        ) {
cout<<__LINE__<<endl;      

    pass = false; // tailing lep pT > 20 GeV
cout<<__LINE__<<endl;      
    
    numEvents->Fill(12); 
cout<<__LINE__<<endl;      
    
  }
cout<<__LINE__<<endl;      
  
  if( abs(phys.lep_p4().at(0).eta())     > 2.4       ) {
    pass = false; // eta < 2.4
    numEvents->Fill(13); 
  }
cout<<__LINE__<<endl;      
  
  if( abs(phys.lep_p4().at(1).eta())     > 2.4       ) {
    pass = false; // eta < 2.4
    numEvents->Fill(14); 
  }
cout<<__LINE__<<endl;      
  
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
  
  //This is the original cu t selection
  if( abs(phys.lep_p4().at(0).eta()) > 1.4 && abs(phys.lep_p4().at(0).eta()) < 1.6 ){
    pass = false;
    numEvents->Fill(17);
  }
cout<<__LINE__<<endl;      

  if( abs(phys.lep_p4().at(1).eta()) > 1.4 && abs(phys.lep_p4().at(1).eta()) < 1.6 ) {
    pass = false; // veto xition region
    numEvents->Fill(18); 
  }
cout<<__LINE__<<endl;      
  
  if( phys.dRll() < 0.1 ) {
    pass = false;
    numEvents->Fill(19); 
  }
cout<<__LINE__<<endl;      

  if( !( phys.hyp_type() == 0 || phys.hyp_type() == 1 ) ) {
    pass = false; // require explicit dilepton event
    numEvents->Fill(20); 
  }
cout<<__LINE__<<endl;      
  
  if( !(phys.evt_type() == 0 ) ) {
    pass = false; // require opposite sign
    numEvents->Fill(21); 
  }
cout<<__LINE__<<endl;      
  
  if( !(phys.dilmass() > 81 && phys.dilmass() < 101) ) {
    pass = false; // on-Z
    numEvents->Fill(22); 
  }
cout<<__LINE__<<endl;      
  
  if( !(phys.dilpt() > 50) ){
    pass = false; // Z pT > 50 GeV
    numEvents->Fill(23); 
  }
cout<<__LINE__<<endl;      
  
  return pass;
}

bool hasGoodPhoton(){
  bool pass = true;

  if( phys.ngamma() <  1 ) {
    pass = false; // require at least 1 good photon
    numEvents->Fill(24);
  }
  
  if( phys.evt_type() != 2 ) {
    pass = false; // photon + jets events
    numEvents->Fill(25);
  }
  
  if( phys.gamma_pt().at(0) < 22 ) {
    pass = false; // photon pt > 22 GeV
    numEvents->Fill(26);
  }
  
  if( abs(phys.gamma_p4().at(0).eta()) > 1.4 && abs(phys.gamma_p4().at(0).eta()) < 1.6 ) {
    pass = false; // veto xition region
    numEvents->Fill(27);
  }
  
  if( abs(phys.gamma_p4().at(0).eta()) > 2.4 ) {
    pass = false; // photon in EC or EB
    numEvents->Fill(28);
  }
  
  if( phys.gamma_hOverE().at(0) > 0.1 ) {
    pass = false; // H/E < 0.1
    numEvents->Fill(29);
  }
  
  // if( phys.matched_neutralemf()          < 0.7   ) return false; // jet neutral EM fraction cut
  
  if( phys.matched_emf() < 0.7 ) {
    pass = false; // jet neutral EM fraction cut
    numEvents->Fill(30);
  }
  
  if( acos( cos( phys.gamma_phi().at(0) - phys.met_phi() ) ) < 0.14 ) {
    pass = false; // kill photons aligned with MET
    numEvents->Fill(31);
  }
  
  if( phys.elveto() ) {
    pass = false; // veto pixel match
    numEvents->Fill(32);
  }
  
  return pass;  
}

bool hasGoodBoson() {
  if (g_sample_name == "zjets") {
    return hasGoodZ();
  }
  else if (g_sample_name == "gjets"){
    return hasGoodPhoton();
  }
  else{
    return false;
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

double getWeight(){
  /*Gets the proper weight for the sample. */
  double weight=1;

  // If we don't have data use scale to 1 fb^-1. 
  if ( ! ( phys.isData() ) ) {
    weight *= phys.evt_scale1fb();
  }

  if ( conf->get("vpt_reweight") == "true" ) {
    weight *= g_vpt_weight_hist->GetBinContent(g_vpt_weight_hist->FindBin(bosonPt()));
  }
  return weight;
}

bool isDuplicate(){
  if( phys.isData() ) {
    DorkyEventIdentifier id(phys.run(), phys.evt(), phys.lumi());
    if (is_duplicate(id) ){
      ++nDuplicates;
      return true;
    }
  }
  return false;
}

int ScanChain( TChain* chain, TString sampleName, ConfigParser *configuration, bool fast = true, int nEvents = -1) {
  /* Runs through baby files and makes histogram files. 
  
  Inputs:
  chain -- contains the files to make the histograms from, 
  sampleName -- for bookkeeping since we make histograms from many different samples. 
  savePath -- where the histogram .root files end up
  mode -- Various options can be set using different 'modes'. Initially this will only be used to determine the event weights, for mode "base" no extra weights are applied, for mode "vpt_reweight" the program expects "base" histograms to already be made, and to have had a reweight histogram available.
  do_MET_filters -- determines whether passMETFilters() is a baseline criteria for events to make it into histos. 
  */  

  //Set Global Vars
  g_sample_name=sampleName;
  conf=configuration;
  TString savePath = conf->get("histo_output_dir");
  TString mode = conf->get("mode");

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");
  
  //=======================================
  // Define Histograms
  //=======================================
  
  TDirectory *rootdir = gDirectory->GetDirectory("Rint:");

  cout<<"Opening file "<<savePath+"ct_"+sampleName+"_"+mode+".root"<<endl;
  TFile * output = new TFile(savePath+"ct_"+sampleName+"_"+mode+".root", "recreate");

  numEvents = new TH1I(sampleName+"_numEvents", "Number of events in "+sampleName, 50, 0, 50);
  numEvents->SetDirectory(rootdir);

  //MET Histos
  TH1F *t1met = new TH1F(sampleName+"_type1MET", "Type 1 MET for "+sampleName, 6000,0,6000);
  t1met->SetDirectory(rootdir);
  t1met->Sumw2();

  TH1F *rawmet = new TH1F(sampleName+"_rawMET", "Raw MET for "+sampleName, 6000,0,6000);
  rawmet->SetDirectory(rootdir);
  rawmet->Sumw2();

  TH1F *ht = new TH1F(sampleName+"_ht", "Scalar sum of hadronic pt (HT) for "+sampleName, 6000,0,6000);
  ht->SetDirectory(rootdir);
  ht->Sumw2();

  TH1F *numMETFilters = new TH1F(sampleName+"_numMETFilters", "Number of MET Filters passed for events in "+sampleName, 50,0,50);
  numMETFilters->SetDirectory(rootdir);
  numMETFilters->Sumw2();

  TH1F *vpt = new TH1F(sampleName+"_vpt", "Boson Pt for events in "+sampleName, 6000,0,6000);
  vpt->SetDirectory(rootdir);
  vpt->Sumw2();

  TH1F *njets = new TH1F(sampleName+"_njets", "Number of jets for events in "+sampleName, 50,0,50);
  njets->SetDirectory(rootdir);
  njets->Sumw2();

  TH1F *nbtags_m = new TH1F(sampleName+"_nbtags_m", "Number of \"medium\" B-tagged jets for events in "+sampleName, 6000,0,6000);
  nbtags_m->SetDirectory(rootdir);
  nbtags_m->Sumw2();

  TH1F *nbtags_l = new TH1F(sampleName+"_nbtags_l", "Number of \"loose\" B-tagged jets for events in "+sampleName, 6000,0,6000);
  nbtags_l->SetDirectory(rootdir);
  nbtags_l->Sumw2();

  TH1F *nbtags_t = new TH1F(sampleName+"_nbtags_t", "Number of \"tight\" B-tagged jets for events in "+sampleName, 6000,0,6000);
  nbtags_t->SetDirectory(rootdir);
  nbtags_t->Sumw2();

  cout<<"Histograms initialized"<<endl;
  //cout<<__LINE__<<endl;

  //Set up manual vertex reweighting.  
  if( conf->get("vpt_reweight") == "true" ){
    g_weight_hist_file = TFile::Open(savePath+"vpt_ratio.root","READ");
    g_vpt_weight_hist = (TH1F*)g_weight_hist_file->Get("h_vpt_ratio")->Clone("h_vpt_weight");
    g_vpt_weight_hist->SetDirectory(rootdir);
    g_weight_hist_file->Close();
  }

cout<<__LINE__<<endl;
  // Loop over events to Analyze
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();
  if( nEvents >= 0 ) nEventsChain = nEvents;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
cout<<__LINE__<<endl;
  // File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TFile file( currentFile->GetTitle() );
    TTree *tree = (TTree*)file.Get("t");
    if(fast) TTreeCache::SetLearnEntries(10); //What does this do?
    if(fast) tree->SetCacheSize(128*1024*1024); //What does this do?
    phys.Init(tree); //Loads in all the branches
cout<<__LINE__<<endl;
    // Loop over Events in current file
    if( nEventsTotal >= nEventsChain ) continue;
    unsigned int nEventsTree = tree->GetEntriesFast();
    for( unsigned int event = 0; event < nEventsTree; ++event) {
      numEvents->Fill(0);
      // Get Event Content
      if( nEventsTotal >= nEventsChain ) continue;
      if(fast) tree->LoadTree(event);
      phys.GetEntry(event);
      ++nEventsTotal;
cout<<__LINE__<<endl;    
      // Progress
      ZMET2016::progress( nEventsTotal, nEventsChain );
cout<<__LINE__<<endl;
      //=======================================
      // Analysis Code
      //=======================================
cout<<__LINE__<<endl;      
      //Set up event weight
      double weight = getWeight();
      if ( isDuplicate() ) continue; // check for duplicates
cout<<__LINE__<<endl;      

      if (! passBaseCut()) continue; // Base Cut
cout<<__LINE__<<endl;      

      if (! hasGoodBoson()) continue; // Boson Specific Cuts
cout<<__LINE__<<endl;      

      if (conf->get("do_MET_filters") == "true" && (! passMETFilters())) continue; ///met filters
cout<<__LINE__<<endl;      
      //Fill in Histos
      double sumMETFilters = phys.Flag_HBHENoiseFilter()+phys.Flag_HBHEIsoNoiseFilter()+phys.Flag_CSCTightHaloFilter()+phys.Flag_EcalDeadCellTriggerPrimitiveFilter()+phys.Flag_goodVertices()+phys.Flag_eeBadScFilter();
cout<<__LINE__<<endl;      
      numMETFilters->Fill(sumMETFilters);
      if (phys.met_T1CHS_miniAOD_CORE_pt() != 0) t1met->Fill(phys.met_T1CHS_miniAOD_CORE_pt(), weight);
      if (phys.met_rawPt() != 0) rawmet->Fill(phys.met_rawPt(), weight);
      if (phys.ht() != 0) ht->Fill(phys.ht(), weight);
      if (bosonPt() != 0) vpt->Fill(bosonPt(), weight);
      njets->Fill(phys.njets(), weight);
      nbtags_m->Fill(phys.nBJetMedium(), weight);
      nbtags_l->Fill(phys.nBJetLoose(), weight);
      nbtags_t->Fill(phys.nBJetTight(), weight);

cout<<__LINE__<<endl;
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
  vpt->Write();
  njets->Write();
  nbtags_m->Write();
  nbtags_l->Write();
  nbtags_t->Write();

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
