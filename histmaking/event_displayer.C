#include <iostream>
#include "/sphenix/user/samfred/projects/gammajet/headers/ana.cxx"
#include "/sphenix/user/samfred/projects/gammajet/headers/TreeSetting.h" 

// global variables
ana anaclone;
float mbd_t0;

// Define histograms
TH2D * hists[100];
// Initialize histograms as needed
void inith() {
  for (int i = 0; i < 100; i++) {
    hists[i] = new TH2D(Form("hist%i",i),";eta;phi",100,-1.1,1.1,300,-M_PI,M_PI);
  }
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

// Finds the max jet on the opposite side of the detector given the max cluster 
jet_object getmaxjet(TClonesArray * jets, int n, float efrac[], float ifrac[], float ofrac[], float t[], pho_object pho) {
  jet_object max;
  for (int i = 0; i < n; i++) {
    TLorentzVector jet = *(TLorentzVector*)jets->At(i);
    jet_object obj = make_jet(jet,efrac[i],ifrac[i],ofrac[i],t[i]*17.6);
    float dr = anaclone.deltaR(pho,obj);
    float dphi = abs(obj.phi - pho.phi);
    if (dphi > M_PI) dphi = 2*M_PI - dphi;
    int eindex = anaclone.findPtBin(pho.pt);
    if (dr < anaclone.drcut) continue;
    if (obj.pt > max.pt) {
      max = obj;
    }
  }
  return max;
}
// Finds the max cluster
pho_object getmaxpho(TClonesArray * phos, int n, float showershapes[][14], float t[], float bdt[][11]) {
  pho_object max;
  for (int i = 0; i < n; i++) {
    TLorentzVector pho = *(TLorentzVector*)phos->At(i);
    float iso3 = showershapes[i][ 8] + showershapes[i][ 9] + showershapes[i][10]; 
    float iso4 = showershapes[i][11] + showershapes[i][12] + showershapes[i][13]; 
    pho_object obj = make_pho(pho,iso3,iso4,t[i]*17.6,bdt[i][0]);
    if (obj.pt > max.pt) {
      max = obj;
    }
  }
  return max;
}


// The called function
void event_displayer(int runnum=47289, bool isMC = 1, bool issmear = 0, const char * trigger = "Jet5")
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
  string filename = Form("/sphenix/user/samfred/projects/gammajet/histmaking/trees/gammajet_%s_unsmear.root",trigger);
  TFile *f = new TFile(Form("%s",filename.c_str()),"read");
  TTree *t = (TTree*) f->Get("towerntup");
  treesetup(t); // Here all the branches of the ttree are set
  float minclustere = anaclone.minclustere;

  // Min and max thresholds for MC samples
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

  TCanvas * c = new TCanvas("c","",400,900);
  gStyle->SetOptStat(0);
  gPad->SetTicks(1,1);
  c->SaveAs("eventdisplays.pdf[");
  int countfilled = 0;
  Long64_t nentries = t->GetEntriesFast();
  for (Long64_t e = 0; e < nentries; e++) {
    if (countfilled > 99) break;
    t->GetEntry(e);
    if (RunNumber != runnum) {
      runnum = RunNumber;
      t0corr = t0map[runnum];
    }
    if(e % 1000==0) std::cout << "entry " << e << "/" << nentries << " (" << (float)e/nentries*100. << "%)" << "\t\r" << std::flush;
    if (fabs(vz) > anaclone.vzcut) continue;
    mbd_t0 = (mbd_time_south + mbd_time_north)/2.0 - t0corr;
    
    bool isc = 1;
    bool isj02 = 1;
    bool isj04 = 1;
    bool isj06 = 1;
    bool isj08 = 1;
    bool isphoton = (trig == "Photon5" || trig == "Photon10" || trig == "Photon20");

    // Check if the event should be kept per Hanpu's thresholds
    if (isMC) {
      vector<float> truthcpt = findmaxpt(truth_cluster_4mom, nTruthClusters);
      vector<float> truthjpt04 = findmaxpt(truth_jet_4mom04, nTruthJets04);
      isc =   (truthcpt[0] > threshmap["cluster"][trig] && truthcpt[0] < threshmap_high["cluster"][trig]);
      isj04 = (truthjpt04[0] > threshmap["jet04"][trig] && truthjpt04[0] < threshmap_high["jet04"][trig]);
      // Because the cutoffs don't really work right with the Photon samples
      if (isphoton) {
        isj04 = true;
      }
      else isc = true;
    }
    
    // Now actually find the jet and photon objects
    pho_object maxpho = getmaxpho(cluster_4mom_sp, nClusters_sp, cluster_showershape_sp, cluster_time_sp, cluster_bdt_scores);
    jet_object maxjet = getmaxjet(jet_4mom04, nJets04, jet_emfrac04, jet_ihfrac04, jet_ohfrac04, jet_time04, maxpho);
    if (maxpho.pt < anaclone.minclustere) continue;
    if (maxjet.pt < anaclone.minjete04) continue;
    if (!isphoton && isMC && maxpho.pt > reco_threshmap_high["cluster"][trig]) {
      isj04 = false;
    }
    if (!isc || !isj04) continue;
    
    float dphi = abs(maxjet.phi - maxpho.phi);
    if (dphi > M_PI) dphi = 2*M_PI - dphi;
    bool isiso = maxpho.iso4 < 2;
    bool isbdt = maxpho.bdt > .8;

    if (maxpho.pt > 15 || maxpho.pt < 10) continue;
    if (dphi < anaclone.oppcut) continue;
    if (abs(maxpho.eta) > anaclone.etacut) continue;
    if (abs(maxjet.eta) > anaclone.etacut - 0.4) continue;
    if (!isiso || !isbdt) continue;

    // Draw them all!
    c->Clear();
    hists[countfilled]->Draw();

    for (int i = 0; i < nJets04; i++) {
      TLorentzVector jet = *(TLorentzVector*)jet_4mom04->At(i);
      jet_object obj = make_jet(jet,0,0,0,0);
      TMarker* jet1 = new TMarker(obj.eta,obj.phi, 24);
      jet1->SetMarkerSize(14);
      jet1->SetMarkerColor(kBlack);
      jet1->Draw();
    }
    TMarker* jet2 = new TMarker(maxjet.eta,maxjet.phi, 20);
    jet2->SetMarkerSize(14);
    jet2->SetMarkerColorAlpha(kRed,.8);
    jet2->Draw();
    
    for (int i = 0; i < nClusters_sp; i++) {
      TLorentzVector pho = *(TLorentzVector*)cluster_4mom_sp->At(i);
      pho_object obj = make_pho(pho,0,0,0,0);
      TMarker* star1 = new TMarker(obj.eta,obj.phi, 29);
      star1->SetMarkerSize(1.5);
      if (obj.iso4 > 2) star1->SetMarkerColor(kBlack);
      else star1->SetMarkerColor(kBlue);
      star1->Draw();
    }
    TMarker* star2 = new TMarker(maxpho.eta,maxpho.phi, 29);
    star2->SetMarkerSize(1.4);
    if (maxpho.iso4 > 2) star2->SetMarkerColor(kRed);
    else star2->SetMarkerColor(kGreen);
    star2->Draw();
  


    c->SaveAs("eventdisplays.pdf");
    countfilled++;
  }
  // the end

  c->SaveAs("eventdisplays.pdf]");
  cout << endl << "Events found: " << countfilled << endl;

  std::cout << "All done!" << std::endl;
}
