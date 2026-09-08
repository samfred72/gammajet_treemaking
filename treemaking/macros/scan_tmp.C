void scan_tmp() {
  TFile *f = TFile::Open("/sphenix/user/samfred/projects/gammajet/treemaking/macros/testtree.root");
  TTree *t = (TTree*)f->Get("towerntup");
  std::cout << "entries: " << t->GetEntries() << std::endl;
  t->Scan("cluster_pt:cluster_pt_nosat:cluster_e:cluster_e_nosat:cluster_eta:cluster_eta_nosat:cluster_bdt_scores[0]:cluster_bdt_score_nosat","cluster_pt>0","",30);
}
