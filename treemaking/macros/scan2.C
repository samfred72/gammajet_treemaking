void scan2() {
  TFile *f = TFile::Open("/sphenix/user/samfred/projects/gammajet/treemaking/macros/testtree.root");
  TTree *t = (TTree*)f->Get("towerntup");
  std::cout << "entries: " << t->GetEntries() << std::endl;
  t->SetScanField(0);
  t->Scan("cluster_pt:cluster_pt_nosat:(cluster_pt_nosat-cluster_pt):cluster_e:cluster_e_nosat","cluster_pt>0","precision=15",30);
}
