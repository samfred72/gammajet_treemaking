#include <iostream>
#include <iterator>
#include "/sphenix/user/samfred/projects/gammajet/headers/ana.cxx"
#include "/sphenix/user/samfred/projects/gammajet/headers/TreeSetting.h" 

// global variables
ana anaclone;
float mbd_t0;

// Define histograms
TH1D * hratio     [anaclone.nPtBins][anaclone.nJetR][anaclone.nIsoBdtBins][4]; // 4 accounts for a,b,c,d regions
TH1D * hratio_max [anaclone.nPtBins][anaclone.nJetR][anaclone.nIsoBdtBins][4]; // 4 accounts for a,b,c,d regions

TH1D * hdeltaphi      [anaclone.nPtBins];
TH1D * hdeltaphiprecut[anaclone.nPtBins];
TH2D * hisobdt        [anaclone.nPtBins];

TH1D * hbdt[11]; // 11 is the number of models

TH1D * hmtminusjt           [anaclone.nJetR];
TH1D * hctminusjt           [anaclone.nJetR];
TH1D * hiso                 [anaclone.nJetR];
TH1D * hemfrac              [anaclone.nJetR];
TH1D * hjetpt               [anaclone.nJetR];
TH1D * htruthjetpt          [anaclone.nJetR];
TH1D * htruthjetptspec      [anaclone.nJetR];
TH1D * htruthjetptanti      [anaclone.nJetR];
TH1D * hjetptprecut         [anaclone.nJetR];
TH1D * htruthjetptprecut    [anaclone.nJetR];
TH1D * htruthjetptprecutspec[anaclone.nJetR];
TH1D * htruthjetptprecutanti[anaclone.nJetR];
TH1D * hjeteta              [anaclone.nJetR];
TH2D * hjetetaphi           [anaclone.nJetR];
TH2D * hiso2d               [anaclone.nJetR];

TH1D * hfrag = new TH1D("hfrag",";cluster Z without iso;counts",100,0,1);
TH1D * hfragiso = new TH1D("hfragiso",";cluster Z with iso;counts",100,0,1);
TH1D * hmbdt = new TH1D("hmbdt",";time [ns]; counts",100,-10,10);
TH1D * hclustert = new TH1D("hclustert",";time [ns]; counts",100,-10,10);
TH1D * hjett = new TH1D("hjett",";time [ns]; counts",100,-10,10);
TH1D * hmtminusct = new TH1D("hmtminusct",";t_{mbd}-t_{cluster} [ns]",100,-10,10);
TH1D * hclusterptprecut = new TH1D("hclusterptprecut",";cluster p_{T,max};counts",100,0,100);
TH1D * htruthclusterptprecut = new TH1D("htruthclusterptprecut",";cluster p_{T,max};counts",100,0,100);
TH1D * hdeltar = new TH1D("hdeltar",";dr [eta,phi];counts",100,0,4);
TH1D * hclusterpt = new TH1D("hclusterpt",";cluster p_{T,max};counts",100,0,100);
TH1D * htruthclusterpt = new TH1D("htruthclusterpt",";cluster p_{T,max};counts",100,0,100);
TH1D * hclustereta = new TH1D("hclustereta",";leading cluster #eta;counts",100,-1.2,1.2);
TH2D * hclusteretaphi = new TH2D("hclusteretaphi",";leading cluster #eta;leading cluster #phi",100,-1.2,1.2,100,-M_PI,M_PI);
TH2D * hmct = new TH2D("hmct",";mbd time [ns]; cluster time [ns]",100,-10,10,100,-10,10);
TH2D * hmjt = new TH2D("hmjt",";mbd time [ns]; jet time [ns]",100,-10,10,100,-10,10);
TH2D * hcjt = new TH2D("hcjt",";cluster time [ns]; jet time [ns]",100,-10,10,100,-10,10);

// Initialize histograms as needed
void inith() {
  for (int i = 0; i < anaclone.nPtBins; i++) {
    for (int j = 0; j < anaclone.nJetR; j++) {
      for (int l = 0; l < anaclone.nIsoBdtBins; l++) {
        for (int k = 0; k < 4; k++) {
          hratio[i][j][l][k] = new TH1D(Form("hratio_%i_%i_%i_%i",i,j,l,k),";p_{T}^{jet}/p_{T}^{#gamma};normalized counts",100000,0,2);
        }
      }
    }
    hdeltaphi[i] = new TH1D(Form("hdeltaphi%i",i),";|#phi_{#gamma} - #phi_{leading jet}|;counts",100,0,M_PI);
    hdeltaphiprecut[i] = new TH1D(Form("hdeltaphiprecut%i",i),";|#phi_{#gamma} - #phi_{leading jet}|;counts",100,0,M_PI);
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

// Gets the fragmentation function of the cluster
// Only considers jets within R=0.4 of the cluster. Has option for isolation energy or not
float getZ(pho_object pho, TClonesArray * jets, int n,bool isiso) {
  jet_object closest;
  float closestdr = 100;
  float Z = 0;
  for (int i = 0; i < n; i++) {
    TLorentzVector jet = *(TLorentzVector*)jets->At(i);
    jet_object obj = make_jet(jet,0,0,0,0);
    float dr = anaclone.deltaR(pho,obj);
    if (dr < 0.4 && (!isiso && dr < closestdr) || (isiso && dr < closestdr && pho.iso4 < 2)) {
      closestdr = dr;
      Z = pho.pt/obj.pt;
    }
  }
  return Z;
}

// Finds the truth pT for clusters and jets. 
// Returns {max, max (unmatched to a photon), max (matched to a photon)}. The last two are for the jet samples
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

vector<float> atov(float * arr) {
  std::vector<float> vec(arr, arr + anaclone.MaxJets);
  return vec;
}

// The called function
void histmaker2(int runnum=47289, bool isMC = 0, bool issmear = 0, const char * trigger = "Jet5")
{
  gSystem->Load("libcaloana.so");
  inith();
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
   

  // Filename management
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
  if (!t) {
    cout << "This file is empty!" << endl;
    return;
  }
  cout << "Using file: " << filename << endl;
  treesetup(t); // Here all the branches of the ttree are set

  const char * wfilename;
  if (isMC && issmear) wfilename = Form("/sphenix/user/samfred/projects/gammajet/histmaking/MChists/hists%s_smear.root",trigger);
  else if (isMC && !issmear) wfilename = Form("/sphenix/user/samfred/projects/gammajet/histmaking/MChists/hists%s_unsmear.root",trigger);
  else if (runnum == 0 && !isMC) wfilename = Form("/sphenix/user/samfred/projects/gammajet/histmaking/hists/hists.root");
  else wfilename = Form("/sphenix/user/samfred/projects/gammajet/histmaking/hists/hists%i.root",runnum);
  TFile *wf = new TFile(wfilename,"recreate");

  // Min and max thresholds for MC samples
  string strig = trigger;
  string trig = strig.substr(0,strig.find("_"));
  if (isMC) cout << trig << endl;
  
  // -1: cluster
  //  0: jet R=0.2
  //  1: jet R=0.4
  //  2: jet R=0.6
  //  3: jet R=0.8
  map<int, map<string,int>> threshmap = {
    {-1,{{"Jet5", 0},{"Jet10", 0},{"Jet20", 0},{"Jet30", 0},{"Jet50", 0},{"Jet70",  0},{"Photon5", 0},{"Photon10",12},{"Photon20", 24}}},
    { 0,{{"Jet5", 0},{"Jet10",12},{"Jet20",20},{"Jet30",31},{"Jet50",50},{"Jet70", 70},{"Photon5", 0},{"Photon10", 0},{"Photon20",  0}}},
    { 1,{{"Jet5", 0},{"Jet10",14},{"Jet20",22},{"Jet30",35},{"Jet50",52},{"Jet70", 71},{"Photon5", 0},{"Photon10", 0},{"Photon20",  0}}},
    { 2,{{"Jet5", 0},{"Jet10",17},{"Jet20",35},{"Jet30",45},{"Jet50",63},{"Jet70", 79},{"Photon5", 0},{"Photon10", 0},{"Photon20",  0}}},
    { 3,{{"Jet5", 0},{"Jet10",20},{"Jet20",40},{"Jet30",50},{"Jet50",65},{"Jet70", 80},{"Photon5", 0},{"Photon10", 0},{"Photon20",  0}}}
  };
  map<int, map<string,int>> threshmap_high = {
    {-1,{{"Jet5", 0},{"Jet10", 0},{"Jet20", 0},{"Jet30", 0},{"Jet50", 0},{"Jet70",  0},{"Photon5",12},{"Photon10",24},{"Photon20",100}}},
    { 0,{{"Jet5",12},{"Jet10",20},{"Jet20",31},{"Jet30",50},{"Jet50",70},{"Jet70",200},{"Photon5", 0},{"Photon10", 0},{"Photon20",  0}}},
    { 1,{{"Jet5",14},{"Jet10",22},{"Jet20",35},{"Jet30",52},{"Jet50",71},{"Jet70",200},{"Photon5", 0},{"Photon10", 0},{"Photon20",  0}}},
    { 2,{{"Jet5",17},{"Jet10",35},{"Jet20",45},{"Jet30",63},{"Jet50",79},{"Jet70",200},{"Photon5", 0},{"Photon10", 0},{"Photon20",  0}}},
    { 3,{{"Jet5",20},{"Jet10",40},{"Jet20",50},{"Jet30",65},{"Jet50",80},{"Jet70",200},{"Photon5", 0},{"Photon10", 0},{"Photon20",  0}}}
  };
  map<int, map<string,int>> reco_threshmap_high = {
    {-1,{{"Jet5",15},{"Jet10",20},{"Jet20",30},{"Jet30",40},{"Jet50",60},{"Jet70",200},{"Photon5", 0},{"Photon10", 0},{"Photon20", 0}}},
    { 0,{{"Jet5", 0},{"Jet10", 0},{"Jet20", 0},{"Jet30", 0},{"Jet50", 0},{"Jet70",  0},{"Photon5", 0},{"Photon10", 0},{"Photon20", 0}}},
    { 1,{{"Jet5", 0},{"Jet10", 0},{"Jet20", 0},{"Jet30", 0},{"Jet50", 0},{"Jet70",  0},{"Photon5", 0},{"Photon10", 0},{"Photon20", 0}}},
    { 2,{{"Jet5", 0},{"Jet10", 0},{"Jet20", 0},{"Jet30", 0},{"Jet50", 0},{"Jet70",  0},{"Photon5", 0},{"Photon10", 0},{"Photon20", 0}}},
    { 3,{{"Jet5", 0},{"Jet10", 0},{"Jet20", 0},{"Jet30", 0},{"Jet50", 0},{"Jet70",  0},{"Photon5", 0},{"Photon10", 0},{"Photon20", 0}}}
  };

  int count_isc = 0;
  vector<int> count_isj(anaclone.nJetR);
  Long64_t nentries = t->GetEntriesFast();
  for (Long64_t e = 0; e < nentries; e++) {
    t->GetEntry(e);
    if (RunNumber != runnum) {
      runnum = RunNumber;
      t0corr = t0map[runnum];
    }
    if(e % 1000==0) std::cout << "entry " << e << "/" << nentries << " (" << (float)e/nentries*100. << "%)" << "\t\r" << std::flush;
    if (fabs(vz) > anaclone.vzcut) continue;
    if (!isMC && !ScaledTriggerBit[27]) continue;
    mbd_t0 = (mbd_time_south + mbd_time_north)/2.0 - t0corr;
    
    // Check if the MC event should be kept
    bool isc = 1;
    vector<bool> isj = {1,1,1,1};
    vector<TClonesArray*> truth_jet_4mom = {truth_jet_4mom02, truth_jet_4mom04, truth_jet_4mom06, truth_jet_4mom08};
    vector<int> nTruthJets = {nTruthJets02, nTruthJets04, nTruthJets06, nTruthJets08};
    bool isphoton = (trig == "Photon5" || trig == "Photon10" || trig == "Photon20");

    if (isMC) {
      // Check the clusters
      vector<float> truthcpt = findmaxpt(truth_cluster_4mom, nTruthClusters);
      isc = (isphoton ? (truthcpt[0] > threshmap[-1][trig] && truthcpt[0] < threshmap_high[-1][trig]) : 1);
      if (truthcpt[0] > 0) htruthclusterptprecut->Fill(truthcpt[0]);
      if (isc) htruthclusterpt->Fill(truthcpt[0]);
      
      // Check the jets
      for (int i = 0; i < anaclone.nJetR; i++) {
        vector<float> truthjpt = findmaxpt(truth_jet_4mom[i], nTruthJets[i]);
        isj[i] = (isphoton ? 1 : (truthjpt[0] > threshmap[i][trig] && truthjpt[0] < threshmap_high[i][trig]));
        if (truthjpt[0] > 0) htruthjetptprecut[i]->Fill(truthjpt[0]);
        if (truthjpt[1] > 0) htruthjetptprecutspec[i]->Fill(truthjpt[1]);
        if (truthjpt[2] > 0) htruthjetptprecutanti[i]->Fill(truthjpt[2]);
        if (isj[i] && isc) {
          htruthjetpt[i]->Fill(truthjpt[0]);
          htruthjetptspec[i]->Fill(truthjpt[1]);
          htruthjetptanti[i]->Fill(truthjpt[2]);
        }
      }
      count_isc += isc;
      for (int i = 0; i < anaclone.nJetR; i++) {
        count_isj[i] += isj[i];
      }
    }
    
    // Store all the jets into nice arrays
    vector<int> nJets = {nJets02,nJets04,nJets06,nJets08};
    vector<TClonesArray*> jet_4mom = {jet_4mom02,jet_4mom04,jet_4mom06,jet_4mom08};
    vector<vector<float>> efrac =    {atov(jet_emfrac02),atov(jet_emfrac04),atov(jet_emfrac06),atov(jet_emfrac08)};
    vector<vector<float>> ifrac =    {atov(jet_ihfrac02),atov(jet_ihfrac04),atov(jet_ihfrac06),atov(jet_ihfrac08)};
    vector<vector<float>> ofrac =    {atov(jet_ohfrac02),atov(jet_ohfrac04),atov(jet_ohfrac06),atov(jet_ohfrac08)};
    vector<vector<float>> jet_time = {atov(jet_time02),  atov(jet_time04),  atov(jet_time06),  atov(jet_time08)};
    
    // Now actually find the jet and photon objects
    pho_object maxpho;
    vector<jet_object> maxjet(anaclone.nJetR);
    for (int ip = 0; ip < nClusters_sp; ip++) {
      bool paired = false;

      TLorentzVector pho_4mom = *(TLorentzVector*)cluster_4mom_sp->At(ip);
      float iso3 = cluster_showershape_sp[ip][ 8] + cluster_showershape_sp[ip][ 9] + cluster_showershape_sp[ip][10]; 
      float iso4 = cluster_showershape_sp[ip][11] + cluster_showershape_sp[ip][12] + cluster_showershape_sp[ip][13]; 

      // The photon in question
      pho_object pho = make_pho(pho_4mom,iso3,iso4,cluster_time_sp[ip]*17.6,cluster_bdt_scores[ip][0]);
      if (!isMC && (mbd_t0 - pho.t > anaclone.thighcut || mbd_t0 - pho.t < anaclone.tlowcut)) continue; 
      
      int ipt = anaclone.findPtBin(pho.pt);
      if (ipt < 0) continue;
      
      // Get the ABCD info
      float phoval = pho.pt;
      vector<bool> isiso(anaclone.nIsoBdtBins);
      vector<bool> isbdt(anaclone.nIsoBdtBins);
      vector<int> iabcd(anaclone.nIsoBdtBins);
      for (int iib = 0; iib < anaclone.nIsoBdtBins; iib++) {
        isiso[iib] = pho.iso4 < anaclone.isoBins[iib];
        isbdt[iib] = pho.bdt > anaclone.bdtBins[iib];
        iabcd[iib] = (((isiso[iib] << 0b1) | isbdt[iib]) ^ 0b11); // silly bitwise operations to map isiso+isbdt->A,B,C,D (index 0,1,2,3)
      }

      // Now check the jets
      for (int ir = 0; ir < anaclone.nJetR; ir++) {
        jet_object bestjet;
        for (int ij = 0; ij < nJets[ir]; ij++) {
          TLorentzVector jet_4m = *(TLorentzVector*)jet_4mom[ir]->At(ij);
          
          // The jet in question
          jet_object jet = make_jet(jet_4m,efrac[ir][ij],ifrac[ir][ij],ofrac[ir][ij],jet_time[ir][ij]*17.6);
          if (!isMC && (mbd_t0 - jet.t > anaclone.thighcut || mbd_t0 - jet.t < anaclone.tlowcut)) continue;
          float dr = anaclone.deltaR(pho,jet);  
          hdeltar->Fill(dr);
          if (dr < anaclone.drcut) continue;
          if (jet.pt > bestjet.pt) bestjet = jet;
          
        }

        // gather variables
        float jetval = bestjet.pt;
        float val = jetval/phoval;
        float dphi = (abs(bestjet.phi - pho.phi) > M_PI ? 2*M_PI - abs(bestjet.phi - pho.phi) : abs(bestjet.phi - pho.phi));
        hdeltaphiprecut[ipt]->Fill(dphi);
        
        // See if the photon-jet pair pass kinematic cuts
        if (!isMC && abs(pho.t - bestjet.t) > anaclone.tcut) continue;
        if (abs(pho.eta) > anaclone.etacut) continue;
        if (abs(bestjet.eta) > anaclone.etacut - anaclone.JetRs[ir]) continue;
        if (dphi < anaclone.oppcut) continue;

        for (int iib = 0; iib < anaclone.nIsoBdtBins; iib++) {
          hratio[ipt][ir][iib][iabcd[iib]]->Fill(val); 
        }
        paired = true;
        if (bestjet.pt > maxjet[ir].pt) maxjet[ir] = bestjet;
      }
      // Histogram filling...
      for (int j = 0; j < 11; j++) {
        hbdt[j]->Fill(cluster_bdt_scores[ip][j]);
      }
      float frag =    getZ(pho, jet_4mom04, nJets04,0);
      float fragiso = getZ(pho, jet_4mom04, nJets04,1);
      if (paired && frag > 0) hfrag->Fill(frag);
      if (paired && fragiso > 0) hfragiso->Fill(fragiso);
      if (paired) hisobdt[ipt]->Fill(pho.iso4,pho.bdt);
      hiso[0]->Fill(pho.iso3);
      hiso[1]->Fill(pho.iso4);
      hiso2d[0]->Fill(pho.pt,pho.iso3);
      hiso2d[1]->Fill(pho.pt,pho.iso4);
      
      if (paired && pho.pt > maxpho.pt) maxpho = pho;
    }


    // cluster and jet kinematics
    if (maxpho.pt > 0) {
      hclusterptprecut->Fill(maxpho.pt);
      if (isc) { hclusterpt->Fill(maxpho.pt); hclustereta->Fill(maxpho.eta); hclusteretaphi->Fill(maxpho.eta,maxpho.phi); }
    }
    for (int i = 0; i < anaclone.nJetR; i++) {
      if (maxjet[i].pt > 0) {
        hjetptprecut[i]->Fill(maxjet[i].pt);
        if (isj[i]) { hjetpt[i]->Fill(maxjet[i].pt); hjeteta[i]->Fill(maxjet[i].eta); hjetetaphi[i]->Fill(maxjet[i].eta,maxjet[i].phi); } 
      }
    }
    
    if (!isMC) {
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

      if (maxpho.pt > 0)   hmtminusct->Fill(mbd_t0-maxpho.t);
      for (int i = 0; i < anaclone.nJetR; i++) {
        if (maxjet[i].pt > 0) hmtminusjt[i]->Fill(mbd_t0-maxjet[i].t);
        if (maxjet[i].pt > 0) hctminusjt[i]->Fill(maxpho.t-maxjet[i].t);
      }
    }
  }

  // the end
  for (int i = 0; i < anaclone.nPtBins; i++) {
    for (int j = 0; j < anaclone.nJetR; j++) {
      for (int l = 0; l < anaclone.nIsoBdtBins; l++) {
        for (int k = 0; k < 4; k++) {
          hratio[i][j][l][k]->Write();
        }
      }
    }
  }
  for (int i = 0; i < anaclone.nPtBins; i++) {
    hdeltaphi[i]->Write();
    hdeltaphiprecut[i]->Write();
    hisobdt[i]->Write();
  }
  for (int i = 0; i < anaclone.nJetR; i++) {
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
  for (int i = 0; i < 11; i++) {
    hbdt[i]->Write();
  }
  hclusterpt->Write();
  htruthclusterpt->Write();
  hclusterptprecut->Write();
  htruthclusterptprecut->Write();
  hclustereta->Write();
  hclusteretaphi->Write();
  hfrag->Write();
  hfragiso->Write();
  hdeltar->Write();
  hmbdt->Write();
  hclustert->Write();
  hjett->Write();
  hmtminusct->Write();
  hmct->Write();
  hmjt->Write();
  hcjt->Write();

  std::cout << std::endl << "All done!" << std::endl;
  if (isMC) {
    cout << "Events with cluster: " << count_isc  << "/" << nentries << ": " << (int)((float)count_isc /(float)nentries*100) << "%" << endl;
    for (int i = 0; i < anaclone.nJetR; i++) {
    cout << Form("Events with jet R=0.%i:   ",2*(i+1)) << count_isj[i] << "/" << nentries << ": " << (int)((float)count_isj[i]/(float)nentries*100) << "%" << endl;
    }
  }
  return;
}
