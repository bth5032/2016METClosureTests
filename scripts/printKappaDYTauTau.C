/* Root Script which prints the amount of DY to Tau Tau in the Strong 0 B tag region for the Kappa study */

#include "scripts/FS_Mass_Window_Study.C"

void printKappaDYTauTau(){
  //Fill these vectors with all the relevent files
  vector<TFile*> on_files;
  vector<TFile*> above_files;
  vector<TFile*> below_files;

  fillFiles(on_files, above_files, below_files, "baseline_MET100_0B");

  TH1D *onz_other, offz_other;
  TH1D *onz_DY, *offz_DY;

  //Fill in DY histos with just DY data
  onz_DY->Add((TH1D*) ((TH1D*) on_files[0]->Get("type1MET")));
  offz_DY->Add((TH1D*) ((TH1D*) below_files[0]->Get("type1MET")));
  offz_DY->Add((TH1D*) ((TH1D*) above_files[0]->Get("type1MET")));

  //Fill in 'others' histos with all but DY
  for (int i = 1; i < (int) on_files.size(); i++){
    onz_others->Add((TH1D*) ((TH1D*) on_files[i]->Get("type1MET")));
    offz_others->Add((TH1D*) ((TH1D*) below_files[i]->Get("type1MET")));
    offz_others->Add((TH1D*) ((TH1D*) above_files[i]->Get("type1MET")));
  }

  cout<<"Num on Z DY 100-150: "<<onz_DY->Integral(101,150)<<endl;
  cout<<"Num off Z DY 100-150: "<<offz_DY->Integral(101,150)<<endl;
  cout<<"Num on Z others 100-150: "<<onz_others->Integral(101,150)<<endl;
  cout<<"Num off Z others 100-150: "<<offz_others->Integral(101,150)<<endl;

  cout<<"Num on Z DY 150-250: "<<onz_DY->Integral(151,250)<<endl;
  cout<<"Num off Z DY 150-250: "<<offz_DY->Integral(151,250)<<endl;
  cout<<"Num on Z others 150-250: "<<onz_others->Integral(151,250)<<endl;
  cout<<"Num off Z others 150-250: "<<offz_others->Integral(151,250)<<endl;


  cout<<"Num on Z DY 250+: "<<onz_DY->Integral(250,6001)<<endl;
  cout<<"Num off Z DY 250+: "<<offz_DY->Integral(250,6001)<<endl;
  cout<<"Num on Z others 250+: "<<onz_others->Integral(250,6001)<<endl;
  cout<<"Num off Z others 250+: "<<offz_others->Integral(250,6001)<<endl;

}

