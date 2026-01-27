#include <iostream>
#include "/sphenix/user/samfred/projects/gammajet/headers/ana.cxx"
#include "/sphenix/user/samfred/projects/gammajet/headers/TreeSetting.h" 

ana anaclone;
float mbd_t0;
bool ISMC;

TH1D * hratio     [anaclone.nPtBins][anaclone.nJetR][4]; // 4 accounts for a,b,c,d regions
TH1D * hratio_3jet[anaclone.nPtBins][anaclone.nJetR][4];
TH1D * hdeltaphi[anaclone.nPtBins];
TH1D * hdeltaphiprecut[anaclone.nPtBins];
TH1D * hmbdt = new TH1D("hmbdt",";time [ns]; counts",100,-10,10);
TH1D * hclustert = new TH1D("hclustert",";time [ns]; counts",100,-10,10);
TH1D * hjett = new TH1D("hjett",";time [ns]; counts",100,-10,10);
TH2D * hmct = new TH2D("hmct",";mbd time [ns]; cluster time [ns]",100,-10,10,100,-10,10);
TH2D * hmjt = new TH2D("hmjt",";mbd time [ns]; jet time [ns]",100,-10,10,100,-10,10);
TH2D * hcjt = new TH2D("hcjt",";cluster time [ns]; jet time [ns]",100,-10,10,100,-10,10);
TH1D * hmtminusct = new TH1D("hmtminusct",";t_{mbd}-t_{cluster} [ns]",100,-10,10);
TH1D * hmtminusjt[anaclone.nJetR];
TH1D * hctminusjt[anaclone.nJetR];
TH1D * hiso[anaclone.nJetR];
TH2D * hiso2d[anaclone.nJetR];
TH1D * hemfrac[anaclone.nJetR];
TH1D * hjetpt[anaclone.nJetR];
TH1D * htruthjetpt[anaclone.nJetR];
TH1D * htruthjetptspec[anaclone.nJetR];
TH1D * htruthjetptanti[anaclone.nJetR];
TH1D * hclusterpt = new TH1D("hclusterpt",";cluster p_{T,max};counts",100,0,100);
TH1D * htruthclusterpt = new TH1D("htruthclusterpt",";cluster p_{T,max};counts",100,0,100);
TH1D * hjetptprecut[anaclone.nJetR];
TH1D * htruthjetptprecut[anaclone.nJetR];
TH1D * htruthjetptprecutspec[anaclone.nJetR];
TH1D * htruthjetptprecutanti[anaclone.nJetR];
TH1D * hclusterptprecut = new TH1D("hclusterptprecut",";cluster p_{T,max};counts",100,0,100);
TH1D * htruthclusterptprecut = new TH1D("htruthclusterptprecut",";cluster p_{T,max};counts",100,0,100);
TH2D * hratiojet[anaclone.nPtBins][anaclone.nJetR];
TH2D * hratiopho[anaclone.nPtBins][anaclone.nJetR];
TH2D * hratio2d = new TH2D("hratio2d",";jet p_{T,max}; cluster p_{T,max}",200,0,100,200,0,100); 
TH1D * hdeltar = new TH1D("hdeltar",";dr [eta,phi];counts",100,0,4);
TH1D * hbdt[11]; // 11 is the number of models
TH2D * hisobdt[anaclone.nabcdbins];
TH1D * hjeteta[anaclone.nJetR];
TH2D * hjetetaphi[anaclone.nJetR];
TH1D * hfrag = new TH1D("hfrag",";cluster Z without iso;counts",100,0,1);
TH1D * hfragiso = new TH1D("hfragiso",";cluster Z with iso;counts",100,0,1);
TH2D * htruthjetcluster = new TH2D("htruthjetcluster",";lead p_{T}^{jet}; lead p_{T}^{photon}",100,0,100,100,0,100);
TH2D * htruthratio = new TH2D("htruthratio",";Leading p_{T}^{truth photon};p_{T}^{matched truth jet}/p_{T}^{truth photon}",70,0,70,100,0,5);

void inith() {
  for (int i = 0; i < anaclone.nPtBins; i++) {
    for (int j = 0; j < anaclone.nJetR; j++) {
      for (int k = 0; k < 4; k++) {
        hratio[i][j][k] = new TH1D(Form("hratio_%i_%i_%i",i,j,k),";p_{T}^{jet}/p_{T}^{#gamma};normalized counts",100000,0,2);
        hratio_3jet[i][j][k] = new TH1D(Form("hratio_3jet_%i_%i_%i",i,j,k),";p_{T}^{jet}/p_{T}^{#gamma};normalized counts",100000,0,2);
      }
      hratiojet[i][j] = new TH2D(Form("hratiojet_%i_%i",i,j),";p_{T}^{jet}/p_{T}^{#gamma};p_{T}^{jet}",100,0,5,100,0,100);
      hratiopho[i][j] = new TH2D(Form("hratiopho_%i_%i",i,j),";p_{T}^{jet}/p_{T}^{#gamma};p_{T}^{jet}",100,0,5,100,0,100);
    }
    hdeltaphi[i] = new TH1D(Form("hdeltaphi%i",i),";|#phi_{#gamma} - #phi_{leading jet}|;counts",100,0,M_PI);
    hdeltaphiprecut[i] = new TH1D(Form("hdeltaphiprecut%i",i),";|#phi_{#gamma} - #phi_{leading jet}|;counts",100,0,M_PI);
  }
  for (int i = 0; i < anaclone.nabcdbins; i++) {
    hisobdt[i] = new TH2D(Form("hisobdt%i",i),";cluster iso;bdt score",100,-1,20,100,0,1);
  }
  for (int i = 0; i < anaclone.nJetR; i++) {
    hjetpt[i] =                    new TH1D(Form("hjetpt%i",i),";jet p_{T,max};counts",100,0,100);
    htruthjetpt[i] =               new TH1D(Form("htruthjetpt%i",i),";jet p_{T,max};counts",100,0,100);
    htruthjetptspec[i] =           new TH1D(Form("htruthjetptspec%i",i),";jet p_{T,max};counts",100,0,100);
    htruthjetptanti[i] =           new TH1D(Form("htruthjetptanti%i",i),";jet p_{T,max};counts",100,0,100);
    hjetptprecut[i] =              new TH1D(Form("hjetptprecut%i",i),";jet p_{T,max};counts",100,0,100);
    htruthjetptprecut[i] =         new TH1D(Form("htruthjetptprecut%i",i),";jet p_{T,max};counts",100,0,100);
    htruthjetptprecutspec[i] =     new TH1D(Form("htruthjetptprecutspec%i",i),";jet p_{T,max};counts",100,0,100);
    htruthjetptprecutanti[i] =     new TH1D(Form("htruthjetptprecutanti%i",i),";jet p_{T,max};counts",100,0,100);
    hemfrac[i] =                   new TH1D(Form("hemfrac%i",i),";jet emfrac R=0.2;counts",120,-0.1,1.1);
    hmtminusjt[i] =                new TH1D(Form("hmtminusjt%i",i),";t_{mbd}-t_{jet} [ns]",100,-10,10);
    hctminusjt[i] =                new TH1D(Form("hctminusjt%i",i),";t_{cluster}-t_{jet} [ns]",100,-10,10);
    hiso[i] =                      new TH1D(Form("hiso%i",i),";iso E R02;counts",100,-1,50);
    hiso2d[i] =                    new TH2D(Form("hiso2d%i",i),";cluster E;iso E R02",100,0,50,100,0,50);
    hjeteta[i] =                   new TH1D(Form("hjeteta%i",i),";#eta_jet;counts",100,-1.1,1.1);
    hjetetaphi[i] =                new TH2D(Form("hjetetaphi%i",i),";#eta_{jet};#phi_{jet}",100,-1.1,1.1,100,-M_PI,M_PI);
  }
  for (int i = 0; i < 11; i++) {
    hbdt[i] = new TH1D(Form("hbdt%i",i),";bdt score;counts",120,-0.1,1.1);
  }
}
float getZ(pho_object pho, TClonesArray * jets, int n,bool isiso) {
  jet_object closest;
  float closestdr = 100;
  float Z = 0;
  for (int i = 0; i < n; i++) {
    TLorentzVector jet = *(TLorentzVector*)jets->At(i);
    jet_object obj = make_jet(jet,0,0,0,0);
    float dr = anaclone.deltaR(pho,obj);
    if (dr < 0.4 && (!isiso && dr < closestdr) || (isiso && dr < closestdr && pho.iso4 < 1.375+0.176*pho.pt)) {
      closestdr = dr;
      Z = pho.pt/obj.pt;
    }
  }
  return Z;
}


void loop(jet_object jet,int jindex, pho_object pho, bool isthirdjet = 0) {

  float dphi = abs(jet.phi - pho.phi);
  if (dphi > M_PI) dphi = 2*M_PI - dphi;
  float jetval = jet.pt;
  float phoval = pho.pt;
  float val = jetval/phoval;
  int emindex = -1;
  //bool isiso = pho.iso4 < 1.403+0.162*pho.pt;
  //bool isbdt = pho.bdt > 0.08+0.006*pho.pt;
  bool isiso = pho.iso4 < 2;
  bool isbdt = pho.bdt > .8;

  int eindex = anaclone.findPtBin(pho.pt);
  int abcdindex = anaclone.findabcdBin(pho.pt);
  
  if (!isthirdjet) {
    if (eindex >= 0) hdeltaphi[eindex]->Fill(dphi);
  }
  
  if (abs(pho.eta) > anaclone.etacut) return;
  if (abs(jet.eta) > anaclone.etacut - 0.2*(jindex+1)) return;
  if (!ISMC && abs(pho.t - jet.t) > anaclone.tcut) return;

  if (abcdindex >= 0) hisobdt[abcdindex]->Fill(pho.iso4,pho.bdt);
 
  if (dphi < anaclone.oppcut) return;
  
  float frag = getZ(pho,   jet_4mom04, nJets04,0);
  float fragiso = getZ(pho,jet_4mom04, nJets04,1);
  if (frag > 0) hfrag->Fill(frag);
  if (fragiso > 0) hfragiso->Fill(fragiso);
  
  hiso[0]->Fill(pho.iso3);
  hiso[1]->Fill(pho.iso4);
  hiso2d[0]->Fill(pho.pt,pho.iso3);
  hiso2d[1]->Fill(pho.pt,pho.iso4);
  hratio2d->Fill(jetval,phoval);
  
  if (eindex < 0) return;
  hratiojet[eindex][jindex]->Fill(val,jetval);
  hratiopho[eindex][jindex]->Fill(val,phoval);
  
  
  if (isiso && isbdt)   hratio[eindex][jindex][0]->Fill(val);
  if (isiso && !isbdt)  hratio[eindex][jindex][1]->Fill(val);
  if (!isiso && isbdt)  hratio[eindex][jindex][2]->Fill(val);
  if (!isiso && !isbdt) hratio[eindex][jindex][3]->Fill(val);
  
  if (isthirdjet) {
    if (isiso && isbdt)   hratio_3jet[eindex][jindex][0]->Fill(val);
    if (isiso && !isbdt)  hratio_3jet[eindex][jindex][1]->Fill(val);
    if (!isiso && isbdt)  hratio_3jet[eindex][jindex][2]->Fill(val);
    if (!isiso && !isbdt) hratio_3jet[eindex][jindex][3]->Fill(val);
  }
  
}

vector<float> findmaxpt(TClonesArray * mom, int n) {
  float maxpt = 0;
  float maxptspec = 0;
  float maxptanti = 0;
  for (int i = 0; i < n; i++) {
    TLorentzVector vec = *(TLorentzVector*)mom->At(i);
    
    bool isnotphoton = true;
    for (int j = 0; j < nTruthClusters; j++) {
      TLorentzVector pho = *(TLorentzVector*)truth_cluster_4mom->At(j);
      float dr = anaclone.deltaR(vec,pho);
      if (dr < 0.2) {
        isnotphoton = false;
        break;
      }
    }
    
    if (vec.Pt() > maxpt) {
      maxpt = vec.Pt();
    }
    if (vec.Pt() > maxptspec && isnotphoton) {
      maxptspec = vec.Pt();
    }
    if (vec.Pt() > maxptanti && !isnotphoton) {
      maxptanti = vec.Pt();
    }

  }
  // spec is away from a photon, anti is matched to photon
  return {maxpt,maxptspec,maxptanti};
}

TH1D * test = new TH1D("test","",100,-.1,.1);
jet_object getmaxjet(TClonesArray * jets, int n, float efrac[], float ifrac[], float ofrac[], float t[], pho_object pho) {
  jet_object max;
  for (int i = 0; i < n; i++) {
    TLorentzVector jet = *(TLorentzVector*)jets->At(i);
    jet_object obj = make_jet(jet,efrac[i],ifrac[i],ofrac[i],t[i]*17.6);
    float dr = anaclone.deltaR(pho,obj);
    test->Fill(obj.pt-obj.et);
    hdeltar->Fill(dr);
    float dphi = abs(obj.phi - pho.phi);
    if (dphi > M_PI) dphi = 2*M_PI - dphi;
    int eindex = anaclone.findPtBin(pho.pt);
    if (eindex >= 0) hdeltaphiprecut[eindex]->Fill(dphi);
    if (!ISMC && (mbd_t0 - obj.t > anaclone.thighcut || mbd_t0 - obj.t < anaclone.tlowcut)) continue;
    if (dr < anaclone.drcut) continue;
    if (obj.pt > max.pt) {
      max = obj;
    }
  }
  return max;
}
pho_object getmaxpho(TClonesArray * phos, int n, float showershapes[][14], float t[], float bdt[][11]) {
  pho_object max;
  for (int i = 0; i < n; i++) {
    TLorentzVector pho = *(TLorentzVector*)phos->At(i);
    for (int j = 0; j < 11; j++) {
      hbdt[j]->Fill(bdt[i][j]);
    }
    float iso3 = showershapes[i][ 8] + showershapes[i][ 9] + showershapes[i][10]; 
    float iso4 = showershapes[i][11] + showershapes[i][12] + showershapes[i][13]; 
    pho_object obj = make_pho(pho,iso3,iso4,t[i]*17.6,bdt[i][0]);
    if (!ISMC && (mbd_t0 - obj.t > anaclone.thighcut || mbd_t0 - obj.t < anaclone.tlowcut)) continue;
    if (obj.pt > max.pt) {
      max = obj;
    }
  }
  return max;
}

bool hasthirdjet(TClonesArray * jets, int n, pho_object lpho, jet_object ljet) {
  for (int i = 0; i < n; i++) {
    TLorentzVector jet = *(TLorentzVector*) jets->At(i);
    jet_object obj = make_jet(jet,0,0,0,0);
    if (anaclone.deltaR(obj,lpho) > anaclone.drcut && anaclone.deltaR(obj,ljet) > anaclone.drcut) return true;
  }
  return false;
}

void histmaker(int runnum=47289, bool isMC = 0, bool issmear = 0, const char * trigger = "Jet5")
{
  gSystem->Load("libcaloana.so");
  inith();
  ISMC=isMC;
  map<int,float> t0map;
  ifstream file = ifstream("/sphenix/user/samfred/projects/mbdt0/histmaking/MbdPmt.corr");
  string line;
  while (getline(file,line)) {
    int irunnum;
    float t0;
    istringstream iss(line);
    iss >> irunnum >> t0;
    t0map[irunnum] = t0;
  }
  t0map[0] = 0;
  float t0corr = t0map[runnum];
   

  const char *dir;
  if (isMC && issmear) dir = Form("/sphenix/tg/tg01/jets/samfred/gammajet_MC_smear");
  else if (isMC && !issmear) dir = Form("/sphenix/tg/tg01/jets/samfred/gammajet_MC_unsmear");
  else dir =  Form("/sphenix/tg/tg01/jets/samfred/gammajet_calojet_hadded");
  string filename;
  if (isMC) filename = Form("%s/run28_%s.root",dir,trigger);
  else filename = Form("%s/run%i.root",dir,runnum);
  if (runnum == 0 && !isMC) filename = "gammajet_calojet.root";
  TFile *f = new TFile(Form("%s",filename.c_str()),"read");
  TTree *t = (TTree*) f->Get("towerntup");
  treesetup(t);
  float minclustere = anaclone.minclustere;

  const char * wfilename;
  if (isMC && issmear) wfilename = Form("/sphenix/user/samfred/projects/gammajet/histmaking/MChists/hists%s_smear.root",trigger);
  else if (isMC && !issmear) wfilename = Form("/sphenix/user/samfred/projects/gammajet/histmaking/MChists/hists%s_unsmear.root",trigger);
  else if (runnum == 0 && !isMC) wfilename = Form("/sphenix/user/samfred/projects/gammajet/histmaking/hists/hists.root");
  else wfilename = Form("/sphenix/user/samfred/projects/gammajet/histmaking/hists/hists%i.root",runnum);
  TFile *wf = new TFile(wfilename,"recreate");


  //map<string, map<string,int>> threshmap = {
  //  {"cluster",{{"MB",0},{"Jet5", 8},{"Jet10",16},{"Jet20",26},{"Jet30",35}}},
  //  {"jet02",  {{"MB",0},{"Jet5", 5},{"Jet10",12},{"Jet20",20},{"Jet30",31}}},
  //  {"jet04",  {{"MB",0},{"Jet5", 7},{"Jet10",14},{"Jet20",22},{"Jet30",35}}},
  //  {"jet06",  {{"MB",0},{"Jet5",11},{"Jet10",17},{"Jet20",35},{"Jet30",45}}},
  //  {"jet08",  {{"MB",0},{"Jet5",12},{"Jet10",20},{"Jet20",40},{"Jet30",50}}}
  //};
  //map<string, map<string,int>> threshmap_high = {
  //  {"cluster",{{"MB", 8},{"Jet5",16},{"Jet10",26},{"Jet20",35},{"Jet30",100}}},
  //  {"jet02",  {{"MB", 5},{"Jet5",12},{"Jet10",20},{"Jet20",31},{"Jet30",100}}},
  //  {"jet04",  {{"MB", 7},{"Jet5",14},{"Jet10",22},{"Jet20",35},{"Jet30",100}}},
  //  {"jet06",  {{"MB",11},{"Jet5",17},{"Jet10",35},{"Jet20",45},{"Jet30",100}}},
  //  {"jet08",  {{"MB",12},{"Jet5",20},{"Jet10",40},{"Jet20",50},{"Jet30",100}}}
  //};
  string strig = trigger;
  string trig = strig.substr(0,strig.find("_"));
  cout << trig << endl;
  map<string, map<string,int>> threshmap = {
    {"cluster",{{"Jet5", 0},{"Jet10", 0},{"Jet20", 0},{"Jet30", 0},{"Jet50", 0},{"Jet70",  0},{"Photon5", 0},{"Photon10",12},{"Photon20", 24}}},
    {"jet02",  {{"Jet5", 0},{"Jet10",12},{"Jet20",20},{"Jet30",31},{"Jet50",50},{"Jet70", 70},{"Photon5", 0},{"Photon10", 0},{"Photon20",  0}}},
    {"jet04",  {{"Jet5", 0},{"Jet10",14},{"Jet20",22},{"Jet30",35},{"Jet50",52},{"Jet70", 71},{"Photon5", 0},{"Photon10", 0},{"Photon20",  0}}},
    {"jet06",  {{"Jet5", 0},{"Jet10",17},{"Jet20",35},{"Jet30",45},{"Jet50",63},{"Jet70", 79},{"Photon5", 0},{"Photon10", 0},{"Photon20",  0}}},
    {"jet08",  {{"Jet5", 0},{"Jet10",20},{"Jet20",40},{"Jet30",50},{"Jet50",65},{"Jet70", 80},{"Photon5", 0},{"Photon10", 0},{"Photon20",  0}}}
  };
  map<string, map<string,int>> threshmap_high = {
    {"cluster",{{"Jet5", 0},{"Jet10", 0},{"Jet20", 0},{"Jet30", 0},{"Jet50", 0},{"Jet70",  0},{"Photon5",12},{"Photon10",24},{"Photon20",100}}},
    {"jet02",  {{"Jet5",12},{"Jet10",20},{"Jet20",31},{"Jet30",50},{"Jet50",70},{"Jet70",200},{"Photon5", 0},{"Photon10", 0},{"Photon20",  0}}},
    {"jet04",  {{"Jet5",14},{"Jet10",22},{"Jet20",35},{"Jet30",52},{"Jet50",71},{"Jet70",200},{"Photon5", 0},{"Photon10", 0},{"Photon20",  0}}},
    {"jet06",  {{"Jet5",17},{"Jet10",35},{"Jet20",45},{"Jet30",63},{"Jet50",79},{"Jet70",200},{"Photon5", 0},{"Photon10", 0},{"Photon20",  0}}},
    {"jet08",  {{"Jet5",20},{"Jet10",40},{"Jet20",50},{"Jet30",65},{"Jet50",80},{"Jet70",200},{"Photon5", 0},{"Photon10", 0},{"Photon20",  0}}}
  };
  map<string, map<string,int>> reco_threshmap_high = {
    {"cluster",{{"Jet5",15},{"Jet10",20},{"Jet20",30},{"Jet30",40},{"Jet50",60},{"Jet70",200},{"Photon5", 0},{"Photon10", 0},{"Photon20", 0}}},
    {"jet02",  {{"Jet5", 0},{"Jet10", 0},{"Jet20", 0},{"Jet30", 0},{"Jet50", 0},{"Jet70",  0},{"Photon5", 0},{"Photon10", 0},{"Photon20", 0}}},
    {"jet04",  {{"Jet5", 0},{"Jet10", 0},{"Jet20", 0},{"Jet30", 0},{"Jet50", 0},{"Jet70",  0},{"Photon5", 0},{"Photon10", 0},{"Photon20", 0}}},
    {"jet06",  {{"Jet5", 0},{"Jet10", 0},{"Jet20", 0},{"Jet30", 0},{"Jet50", 0},{"Jet70",  0},{"Photon5", 0},{"Photon10", 0},{"Photon20", 0}}},
    {"jet08",  {{"Jet5", 0},{"Jet10", 0},{"Jet20", 0},{"Jet30", 0},{"Jet50", 0},{"Jet70",  0},{"Photon5", 0},{"Photon10", 0},{"Photon20", 0}}}
  };

  int count_isc = 0;
  int count_isj2 = 0;
  int count_isj4 = 0;
  int count_isj6 = 0;
  int count_isj8 = 0;
  Long64_t nentries = t->GetEntriesFast();
  for (Long64_t e = 0; e < nentries; e++) {
    t->GetEntry(e);
    if (RunNumber != runnum) {
      runnum = RunNumber;
      t0corr = t0map[runnum];
    }
    if(e % 1000==0) std::cout << "entry " << e << "/" << nentries << " (" << (float)e/nentries*100. << "%)" << "\t\r" << std::flush;
    //cout << vz << " " << ScaledTriggerBit[27] << endl;
    if (fabs(vz) > anaclone.vzcut) continue;
    //if (!isMC && !ScaledTriggerBit[27]) continue;
    mbd_t0 = (mbd_time_south + mbd_time_north)/2.0 - t0corr;
    
    // time histos
    hmbdt->Fill(mbd_t0);
    for (int i = 0; i < nJets04; i++) {
      hjett->Fill(jet_time04[i]*17.6);
      hmjt->Fill(mbd_t0,jet_time04[i]*17.6);
    }
    for (int i = 0; i < nClusters_sp; i++) {
      hclustert->Fill(cluster_time_sp[i]*17.6);
      hmct->Fill(mbd_t0,cluster_time_sp[i]*17.6);
    }
    
    bool isc = 1;
    bool isj02 = 1;
    bool isj04 = 1;
    bool isj06 = 1;
    bool isj08 = 1;
    bool isphoton = (trig == "Photon5" || trig == "Photon10" || trig == "Photon20");

    if (isMC) {
      vector<float> truthcpt = findmaxpt(truth_cluster_4mom, nTruthClusters);
      vector<float> truthjpt02 = findmaxpt(truth_jet_4mom02, nTruthJets02);
      vector<float> truthjpt04 = findmaxpt(truth_jet_4mom04, nTruthJets04);
      vector<float> truthjpt06 = findmaxpt(truth_jet_4mom06, nTruthJets06);
      vector<float> truthjpt08 = findmaxpt(truth_jet_4mom08, nTruthJets08);
      isc =   (truthcpt[0] > threshmap["cluster"][trig] && truthcpt[0] < threshmap_high["cluster"][trig]);
      isj02 = (truthjpt02[0] > threshmap["jet02"][trig] && truthjpt02[0] < threshmap_high["jet02"][trig]);
      isj04 = (truthjpt04[0] > threshmap["jet04"][trig] && truthjpt04[0] < threshmap_high["jet04"][trig]);
      isj06 = (truthjpt06[0] > threshmap["jet06"][trig] && truthjpt06[0] < threshmap_high["jet06"][trig]);
      isj08 = (truthjpt08[0] > threshmap["jet08"][trig] && truthjpt08[0] < threshmap_high["jet08"][trig]);
      // Because the cutoffs don't really work right with the Photon samples
      if (isphoton) {
        isj02 = true;
        isj04 = true;
        isj06 = true;
        isj08 = true;
      }
      else isc = true;

      
      if (truthcpt[0] > 0) htruthclusterptprecut->Fill(truthcpt[0]);
      //if (truthcpt[0] > 20) {
      if (truthjpt02[0] > 0) htruthjetptprecut[0]->Fill(truthjpt02[0]);
      if (truthjpt02[1] > 0) htruthjetptprecutspec[0]->Fill(truthjpt02[1]);
      if (truthjpt02[2] > 0) htruthjetptprecutanti[0]->Fill(truthjpt02[2]);
      if (truthjpt04[0] > 0) htruthjetptprecut[1]->Fill(truthjpt04[0]);
      if (truthjpt04[1] > 0) htruthjetptprecutspec[1]->Fill(truthjpt04[1]);
      if (truthjpt04[2] > 0) htruthjetptprecutanti[1]->Fill(truthjpt04[2]);
      if (truthjpt06[0] > 0) htruthjetptprecut[2]->Fill(truthjpt06[0]);
      if (truthjpt06[1] > 0) htruthjetptprecutspec[2]->Fill(truthjpt06[1]);
      if (truthjpt06[2] > 0) htruthjetptprecutanti[2]->Fill(truthjpt06[2]);
      if (truthjpt08[0] > 0) htruthjetptprecut[3]->Fill(truthjpt08[0]);
      if (truthjpt08[1] > 0) htruthjetptprecutspec[3]->Fill(truthjpt08[1]);
      if (truthjpt08[2] > 0) htruthjetptprecutanti[3]->Fill(truthjpt08[2]);
      //}

      if (isc) htruthclusterpt->Fill(truthcpt[0]);
      if (isj02 && isc) {
        htruthjetpt[0]->Fill(truthjpt02[0]);
        htruthjetptspec[0]->Fill(truthjpt02[1]);
        htruthjetptanti[0]->Fill(truthjpt02[2]);
      }
      if (isj04 && isc) {
        htruthjetpt[1]->Fill(truthjpt04[0]);
        htruthjetptspec[1]->Fill(truthjpt04[1]);
        htruthjetptanti[1]->Fill(truthjpt04[2]);
      }
      if (isj06 && isc) {
        htruthjetpt[2]->Fill(truthjpt06[0]);
        htruthjetptspec[2]->Fill(truthjpt06[1]);
        htruthjetptanti[2]->Fill(truthjpt06[2]);
      }
      if (isj08 && isc) {
        htruthjetpt[3]->Fill(truthjpt08[0]);
        htruthjetptspec[3]->Fill(truthjpt08[1]);
        htruthjetptanti[3]->Fill(truthjpt08[2]);
      }

      htruthjetcluster->Fill(truthjpt04[2],truthcpt[0]);
      if (truthcpt[0] > 0 && truthjpt04[2] > 0) htruthratio->Fill(truthcpt[0],truthjpt04[2]/truthcpt[0]);
    }
    pho_object maxpho = getmaxpho(cluster_4mom_sp, nClusters_sp, cluster_showershape_sp, cluster_time_sp, cluster_bdt_scores);
    if (maxpho.pt < anaclone.minclustere) continue;
    if (!isphoton && isMC && maxpho.pt > reco_threshmap_high["cluster"][trig]) {
      isj02 = false;
      isj04 = false;
      isj06 = false;
      isj08 = false;
    }
    count_isc += isc;
    count_isj2 += isj02;
    count_isj4 += isj04;
    count_isj6 += isj06;
    count_isj8 += isj08;

    jet_object maxjet02 = getmaxjet(jet_4mom02, nJets02, jet_emfrac02, jet_ihfrac02, jet_ohfrac02, jet_time02, maxpho);
    jet_object maxjet04 = getmaxjet(jet_4mom04, nJets04, jet_emfrac04, jet_ihfrac04, jet_ohfrac04, jet_time04, maxpho);
    jet_object maxjet06 = getmaxjet(jet_4mom06, nJets06, jet_emfrac06, jet_ihfrac06, jet_ohfrac06, jet_time06, maxpho);
    jet_object maxjet08 = getmaxjet(jet_4mom08, nJets08, jet_emfrac08, jet_ihfrac08, jet_ohfrac08, jet_time08, maxpho);
    if (maxpho.pt > 0)   hclusterptprecut->Fill(maxpho.pt);
    if (maxjet02.pt > 0) hjetptprecut[0]->Fill(maxjet02.pt);
    if (maxjet04.pt > 0) hjetptprecut[1]->Fill(maxjet04.pt);
    if (maxjet06.pt > 0) hjetptprecut[2]->Fill(maxjet06.pt);
    if (maxjet08.pt > 0) hjetptprecut[3]->Fill(maxjet08.pt);
    if (isc && maxpho.pt > 0)     hclusterpt->Fill(maxpho.pt);
    if (isj02 && maxjet02.pt > 0) { hjetpt[0]->Fill(maxjet02.pt); hjeteta[0]->Fill(maxjet02.eta); hjetetaphi[0]->Fill(maxjet02.eta,maxjet02.phi); } 
    if (isj04 && maxjet04.pt > 0) { hjetpt[1]->Fill(maxjet04.pt); hjeteta[1]->Fill(maxjet04.eta); hjetetaphi[1]->Fill(maxjet04.eta,maxjet04.phi); }
    if (isj06 && maxjet06.pt > 0) { hjetpt[2]->Fill(maxjet06.pt); hjeteta[2]->Fill(maxjet06.eta); hjetetaphi[2]->Fill(maxjet06.eta,maxjet06.phi); }
    if (isj08 && maxjet08.pt > 0) { hjetpt[3]->Fill(maxjet08.pt); hjeteta[3]->Fill(maxjet08.eta); hjetetaphi[3]->Fill(maxjet08.eta,maxjet08.phi); }
    if (!isMC) {
      if (maxpho.pt > 0)   hmtminusct->Fill(mbd_t0-maxpho.t);
      if (maxjet02.pt > 0) hmtminusjt[0]->Fill(mbd_t0-maxjet02.t);
      if (maxjet04.pt > 0) hmtminusjt[1]->Fill(mbd_t0-maxjet04.t);
      if (maxjet06.pt > 0) hmtminusjt[2]->Fill(mbd_t0-maxjet06.t);
      if (maxjet08.pt > 0) hmtminusjt[3]->Fill(mbd_t0-maxjet08.t);
      if (maxjet02.pt > 0) hctminusjt[0]->Fill(maxpho.t-maxjet02.t);
      if (maxjet04.pt > 0) hctminusjt[1]->Fill(maxpho.t-maxjet04.t);
      if (maxjet06.pt > 0) hctminusjt[2]->Fill(maxpho.t-maxjet06.t);
      if (maxjet08.pt > 0) hctminusjt[3]->Fill(maxpho.t-maxjet08.t);
    }

    if ((isMC && isc && isj02 && maxjet02.pt > anaclone.minjete02) || (!isMC && maxjet02.pt > anaclone.minjete02)) loop(maxjet02,0, maxpho);
    if ((isMC && isc && isj04 && maxjet04.pt > anaclone.minjete04) || (!isMC && maxjet04.pt > anaclone.minjete04)) loop(maxjet04,1, maxpho);
    if ((isMC && isc && isj06 && maxjet06.pt > anaclone.minjete06) || (!isMC && maxjet06.pt > anaclone.minjete06)) loop(maxjet06,2, maxpho);
    if ((isMC && isc && isj08 && maxjet08.pt > anaclone.minjete08) || (!isMC && maxjet08.pt > anaclone.minjete08)) loop(maxjet08,3, maxpho);
   
    if (!hasthirdjet(jet_4mom02,nJets02,maxpho,maxjet02)) {
      if ((isMC && isc && isj02 && maxjet02.pt > anaclone.minjete02) || (!isMC && maxjet02.pt > anaclone.minjete02)) loop(maxjet02,0, maxpho, 1);
    }
    if (!hasthirdjet(jet_4mom04,nJets04,maxpho,maxjet04)) {
      if ((isMC && isc && isj04 && maxjet04.pt > anaclone.minjete04) || (!isMC && maxjet04.pt > anaclone.minjete04)) loop(maxjet04,1, maxpho, 1);
    }
    if (!hasthirdjet(jet_4mom06,nJets06,maxpho,maxjet06)) {
      if ((isMC && isc && isj06 && maxjet06.pt > anaclone.minjete06) || (!isMC && maxjet06.pt > anaclone.minjete06)) loop(maxjet06,2, maxpho, 1);
    }
    if (!hasthirdjet(jet_4mom08,nJets08,maxpho,maxjet08)) {
      if ((isMC && isc && isj08 && maxjet08.pt > anaclone.minjete08) || (!isMC && maxjet08.pt > anaclone.minjete08)) loop(maxjet08,3, maxpho, 1);
    }

  }
  hdeltar->Write();
  hmbdt->Write();
  hclustert->Write();
  hjett->Write();
  hmtminusct->Write();
  hmct->Write();
  hmjt->Write();
  hcjt->Write();
  for (int i = 0; i < anaclone.nPtBins; i++) {
    for (int j = 0; j < 4; j++) {
      for (int k = 0; k < 4; k++) {
        hratio[i][j][k]->Write();
        hratio_3jet[i][j][k]->Write();
      }
      hratiojet[i][j]->Write();
      hratiopho[i][j]->Write();
    }
    hdeltaphi[i]->Write();
    hdeltaphiprecut[i]->Write();
  }
  for (int i = 0; i < 4; i++) {
    hemfrac[i]->Write();
    hjetpt[i]->Write();
    hjetptprecut[i]->Write();
    htruthjetpt[i]->Write();
    htruthjetptspec[i]->Write();
    htruthjetptanti[i]->Write();
    htruthjetptprecut[i]->Write();
    htruthjetptprecutspec[i]->Write();
    htruthjetptprecutanti[i]->Write();
    hiso[i]->Write();
    hiso2d[i]->Write();
    hmtminusjt[i]->Write();
    hctminusjt[i]->Write();
    hjeteta[i]->Write();
    hjetetaphi[i]->Write();
    
  }
  for (int i = 0; i < anaclone.nabcdbins; i++) {
    hisobdt[i]->Write();
  }

  for (int i = 0; i < 11; i++) {
    hbdt[i]->Write();
  }
  hratio2d->Write();
  hclusterpt->Write();
  htruthclusterpt->Write();
  hclusterptprecut->Write();
  htruthclusterptprecut->Write();
  test->Write();
  hfrag->Write();
  hfragiso->Write();
  htruthjetcluster->Write();
  htruthratio->Write();

  std::cout << std::endl << "All done!" << std::endl;
  if (isMC) {
    cout << "Events with cluster: " << count_isc  << "/" << nentries << ": " << (int)((float)count_isc /(float)nentries*100) << "%" << endl;
    cout << "Events with jet02:   " << count_isj2 << "/" << nentries << ": " << (int)((float)count_isj2/(float)nentries*100) << "%" << endl;
    cout << "Events with jet04:   " << count_isj4 << "/" << nentries << ": " << (int)((float)count_isj4/(float)nentries*100) << "%" << endl;
    cout << "Events with jet06:   " << count_isj6 << "/" << nentries << ": " << (int)((float)count_isj6/(float)nentries*100) << "%" << endl;
    cout << "Events with jet08:   " << count_isj8 << "/" << nentries << ": " << (int)((float)count_isj8/(float)nentries*100) << "%" << endl;
  }
}
