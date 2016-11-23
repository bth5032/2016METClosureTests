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
#include <fstream>


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

// Analysis Specific
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

/* returns two most B-like jet indicies */
pair<int, int> getMostBlike();

/*Finds the pair of B tagged jets (csv medium) with dijet mass closest to the mass of the higgs*/
pair<int,int> getClosestBPairToHiggsMass();

/*This function gets the MT2 built out of the two Bjets in an event, no guarentee 
is made about selecting the highest csv jets*/
double getMT2ForBjets(bool select_highest_csv=false);

/*Builds MT2 for the two leading Bjets unless select_closest_higgs_mass is set, in which case it 
builds it out of the two bjets with dijet mass nearest the mass of the higgs.*/
double getMT2HiggsZ(bool select_highest_closest_higgs_mass=false);

/*Returns boson Pt, determines whether sample is gjets or zjets first*/
double bosonPt();

/*Reads the efficiency from the g_vpt_eff_barrel histogram for the pt of the leading
photon in the event*/
double getEff();

/* Builds the MT from the lepton at index id and the MET vector (assumes massless particles)*/
double getMTLepMET(short id=0);

/* Builds the delta R (sqrt(dPhi^2 + dEta^2)) between the lepton at index id and the leading photon*/
double getdRGammaLep(short id=0);

//=============================
// Triggers
//=============================
/*Checks that the event passes an "emulated photon trigger"*/
bool passPhotonEmulatedTrigger();

/*Ensures the event is within the efficiency plateu of the highest pt trigger it passed*/
bool passPhotonTriggers();

/*MC passes immediately, ensures data events were gathered from di-muon triggers*/
bool passMuonTriggers();

/*MC passes immediately, ensures data events were gathered from di-electron triggers*/
bool passElectronTriggers();

/*MC passes immediately, ensures data events were gathered from EMu triggers*/
bool passEMuTriggers();

/*Helper method which chooses which above method to call. Calls EMu if the dil_flavor is emu, otherwise uses
the hyp_type to determine which to call. Events fail if they are hyp_type 2 and not tagged for emu*/
bool passLeptonHLTs();

//=============================
// Has Good Event Functions
//=============================
/*Lepton quality and Z mass cuts*/
bool hasGoodZ();

/*Photon quality cuts*/
bool hasGoodPhoton();

/*Method for testing whether the event has a good gamma mu pair trigger requirements are on the photon.
  It just checks muon quality stuff and then calls hasGoodPhoton()*/
bool hasGoodGammaMu();

/*Just a helper method that chooses which hasGood method to call based on the config event_type*/
bool hasGoodEvent();

//=============================
// Event Weight Assignment
//=============================

/*Goes through the chain of weight_from config options down to a config which does not have weight_from and
then adds a pair (config_name, hist_file) to the vector g_reweight_pairs.

NEEDS TO BE UPDATED WITH NEW CODE FIXES*/
void readyReweightHists();

/*Loads the reweight hists from g_reweight_pairs and multiplies returns the weight associated with the proper
bin in the histogram*/
double getReweight();

/*This method stores fixes to the evt_scale1fb in the event of file corruptions. 
It's basically just a lookup table*/
double scale1fbFix();

/*Main function for determining the weights for each event*/
double getWeight();

/*Returns the weight associated with the photon prescales*/
double getPrescaleWeight();

//=============================
// Cuts
//=============================

/*Holds the cuts for all the signal regions, basically all the cuts that are turned on with a config option*/
bool passSignalRegionCuts();

/*Checks for a gen Neutrino (Real MET) and a gen Z (Real Z), only should be run when running
over samples tagged as "rares". This is only neccesary for the full prediction.*/
bool passRareCuts();

/*Checks for cuts that are spcific to SUSY samples like choosing a particular mass point*/
bool passSUSYSingalCuts();

/*Front end method to "Dorky" duplicate removal*/
bool isDuplicate();

/*Checks for MET filters*/
bool passMETFilters();

/*Holds baseline cuts*/
bool passBaseCut();

/*Obvi the event looper*/
int ScanChain( TChain* chain, ConfigParser *configuration, bool fast = true, int nEvents = -1);