void AddRareHists(TString sample_name, TString location){
  TFile* f_combined = new TFile(location+"/ct_combined_"+sample_name+".root", "RECREATE");

  TFile* f_vvv = new TFile(location+"/ct_VVV_"+sample_name+".root", "READ");
  TFile* f_ttv = new TFile(location+"/ct_ttv_"+sample_name+".root", "READ");
  TFile* f_zz = new TFile(location+"/ct_zz_"+sample_name+".root", "READ");
  TFile* f_wz = new TFile(location+"/ct_wz_"+sample_name+".root", "READ");

  TIter n(f_vvv->GetListOfKeys());
  TH1D* combined;
  TString name;
  TKey *key;
  int i = 0; //Make sure don't get infinite loops if something is messed up.
  while ((key=(TKey*)n()) && i<50){
    cout<<"Found Object "<<key->GetName()<<endl;
    combined = (TH1D*) ((TH1D*) key->ReadObj())->Clone("combined");
    name=key->GetName();

    name.ReplaceAll("vvv", "combined");
    combined->SetName(name);
    
    name.ReplaceAll("combined", "ttv");
    combined->Add((TH1D*) f_ttv->Get(name));
    //cout<<name<<endl;

    name.ReplaceAll("ttv", "wz");
    combined->Add((TH1D*) f_wz->Get(name));
    //cout<<name<<endl;

    name.ReplaceAll("wz", "zz");
    combined->Add((TH1D*) f_zz->Get(name));
    //cout<<name<<endl;

    f_combined->cd();
    combined->Write();

    i++;
  }
}