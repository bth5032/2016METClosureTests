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


bool passMETFilters(){

  if ( phys.isData() ) {
    if ( phys.nVert() == 0 ) return false;
    if (!phys.Flag_HBHENoiseFilter                   ()      ) return false;
    if (!phys.Flag_HBHEIsoNoiseFilter                ()      ) return false;
    if ( phys.Flag_CSCTightHalo2015Filter_isLoaded ){
      if (!phys.Flag_CSCTightHalo2015Filter            ()      ) return false;  
    }
    else{
      if (!phys.Flag_CSCTightHaloFilter            ()      ) return false;
    }
    if (!phys.Flag_EcalDeadCellTriggerPrimitiveFilter()      ) return false;
    if (!phys.Flag_goodVertices                      ()      ) return false;
    if (!phys.Flag_eeBadScFilter                     ()      ) return false;

  }
  return true;
}

bool passBaseCut(){
  
  if (! (phys.evt_passgoodrunlist() > 0)) return false; //golden json
  if (! (phys.njets() >= 2) ) return false; //2 jet cut

  return true;
}

bool hasGoodZ(){
  if( phys.nlep()                        < 2         ) return false; // require at least 2 good leptons
  if( phys.lep_pt().at(0)                < 20        ) return false; // leading lep pT > 20 GeV
  if( phys.lep_pt().at(1)                < 20        ) return false; // tailing lep pT > 20 GeV
  if( abs(phys.lep_p4().at(0).eta())     > 2.4       ) return false; // eta < 2.4
  if( abs(phys.lep_p4().at(1).eta())     > 2.4       ) return false; // eta < 2.4
  //This is the augmented cut selection.
  LorentzVector zp4 = phys.lep_p4().at(1) + phys.lep_p4().at(2);
  if( abs(zp4.eta())   > 1.4 &&
      abs(zp4.eta())   < 1.6   ) return false; // veto xition region
  if( abs(zp4.eta())   > 2.4   ) return false; // Z in EC or EB
  /*
  //This is the original cu t selection
  if( abs(phys.lep_p4().at(0).eta())     > 1.4 &&
    abs(phys.lep_p4().at(0).eta())     < 1.6       ) return false;
  if( abs(phys.lep_p4().at(1).eta())     > 1.4 &&
    abs(phys.lep_p4().at(1).eta())     < 1.6       ) return false; // veto xition region
  */
  if( phys.dRll()                        < 0.1       ) return false;

  if( !(phys.hyp_type() == 0 ||              
    phys.hyp_type() == 1 ||              
    phys.hyp_type() == 2 )                       ) return false; // require explicit dilepton event
  if( !(phys.evt_type() == 0 )                       ) return false; // require opposite sign
  if( !(phys.dilmass() > 81 && phys.dilmass() < 101) ) return false; // on-Z
  if( !(phys.dilpt() > 50)                           ) return false; // Z pT > 50 GeV
  return true;
}

bool hasGoodPhoton(){
  if( phys.ngamma()                      <  1    ) return false; // require at least 1 good photon
  if( phys.evt_type()                    != 2    ) return false; // photon + jets events
  if( phys.gamma_pt().at(0)              < 22    ) return false; // photon pt > 22 GeV
  if( abs(phys.gamma_p4().at(0).eta())   > 1.4 &&
      abs(phys.gamma_p4().at(0).eta())   < 1.6   ) return false; // veto xition region
  if( abs(phys.gamma_p4().at(0).eta())   > 2.4   ) return false; // photon in EC or EB
  if( phys.gamma_hOverE().at(0)          > 0.1   ) return false; // H/E < 0.1   
  // if( phys.matched_neutralemf()          < 0.7   ) return false; // jet neutral EM fraction cut
  if( phys.matched_emf()                 < 0.7   ) return false; // jet neutral EM fraction cut
  if( acos( cos( phys.gamma_phi().at(0)      
         - phys.met_phi() ) )    < 0.14  ) return false; // kill photons aligned with MET
  if( phys.elveto()                              ) return false; // veto pixel match
  return true;  
}

double bosonPt(){
  // Returns boson Pt, determines whether sample is gjets or zjets first.
  if (g_sample_name == "zjets") {
    return phys.dilpt();
  }
  else{
    return phys.gamma_pt().at(0);
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

  TH1I *numEvents = new TH1I("numEvents_"+sampleName, "Number of events in "+sampleName, 1, 0, 1);
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


  //Set up manual vertex reweighting.  
  if( conf->get("vpt_reweight") == "true" ){
    g_weight_hist_file = TFile::Open(savePath+"vpt_ratio.root","READ");
    g_vpt_weight_hist = (TH1F*)g_weight_hist_file->Get("h_vpt_ratio")->Clone("h_vpt_weight");
    g_vpt_weight_hist->SetDirectory(rootdir);
    g_weight_hist_file->Close();
  }

  // Loop over events to Analyze
  unsigned int nEventsTotal = 0;
  unsigned int nEventsChain = chain->GetEntries();
  if( nEvents >= 0 ) nEventsChain = nEvents;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;

  // File Loop
  while ( (currentFile = (TFile*)fileIter.Next()) ) {

    // Get File Content
    TFile file( currentFile->GetTitle() );
    TTree *tree = (TTree*)file.Get("t");
    if(fast) TTreeCache::SetLearnEntries(10); //What does this do?
    if(fast) tree->SetCacheSize(128*1024*1024); //What does this do?
    phys.Init(tree); //Loads in all the branches
    
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
    
      // Progress
      ZMET2016::progress( nEventsTotal, nEventsChain );

      //=======================================
      // Analysis Code
      //=======================================
      //Set up event weight
      double weight = getWeight();
      if ( isDuplicate() ) continue; // check for duplicates
      if (! passBaseCut()) continue; // Base Cut
      if (config->get("do_MET_filters") == "true" && (! passMETFilters())) continue; ///met filters
      
      //Fill in Histos
      double sumMETFilters = phys.Flag_HBHENoiseFilter()+phys.Flag_HBHEIsoNoiseFilter()+phys.Flag_CSCTightHaloFilter()+phys.Flag_EcalDeadCellTriggerPrimitiveFilter()+phys.Flag_goodVertices()+phys.Flag_eeBadScFilter();
      
      numMETFilters->Fill(sumMETFilters);
      if (phys.met_T1CHS_miniAOD_CORE_pt() != 0) t1met->Fill(phys.met_T1CHS_miniAOD_CORE_pt(), weight);
      if (phys.met_rawPt() != 0) rawmet->Fill(phys.met_rawPt(), weight);
      if (phys.ht() != 0) ht->Fill(phys.ht(), weight);
      if (phys.vpt() != 0) vpt->Fill(bosonPt(sampleName), weight);
      njets->Fill(phys.njets(), weight);
      nbtags_m->Fill(phys.nBJetMedium(), weight);
      nbtags_l->Fill(phys.nBJetLoose(), weight);
      nbtags_t->Fill(phys.nBJetTight(), weight);


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
