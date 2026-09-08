void combine_funcs() {
  TFile * f = TFile::Open("funcs_insitu.root","RECREATE");
  const char * rnames[7] = {"R02","R03","R04","R05","R06","R07","R08"};
  for (int i = 0; i < 7; i++) {
    const char * rname = rnames[i];
    TFile * fi = TFile::Open(Form("funcs_insitu_%s.root",rname),"READ");
    TF1 * f1 = (TF1*)fi->Get("finsitu");
    TF1 * f2 = (TF1*)fi->Get("fhigh");
    TF1 * f3 = (TF1*)fi->Get("flow");
    f1->SetName(Form("insitu_%s",rname));
    f2->SetName(Form("insitu_high_error_%s",rname));
    f3->SetName(Form("insitu_low_error_%s",rname));
    f1->SetTitle("");
    f2->SetTitle("");
    f3->SetTitle("");
    f->cd();
    f1->Write();
    f2->Write();
    f3->Write();
    delete fi;
    delete f1;
    delete f2;
    delete f3;
  }
}

