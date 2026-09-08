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

// The called function
void event_displayer(int runnum=47289, bool isMC = 1, bool issmear = 0, const char * trigger = "Photon5")
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
  c->SaveAs("pdfs/trutheventdisplays.pdf[");
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
    if (!isphoton && isMC) {
      isj04 = false;
    }
    if (!isc || !isj04) continue;
    
    /*
    float dphi = abs(maxjet.phi - maxpho.phi);
    if (dphi > M_PI) dphi = 2*M_PI - dphi;
    bool isiso = maxpho.iso4 < 2;
    bool isbdt = maxpho.bdt > .8;

    if (maxpho.pt > 15 || maxpho.pt < 10) continue;
    if (dphi < anaclone.oppcut) continue;
    if (abs(maxpho.eta) > anaclone.etacut) continue;
    if (abs(maxjet.eta) > anaclone.etacut - 0.4) continue;
    if (!isiso || !isbdt) continue;
*/
    // Draw them all!
    c->Clear();
    hists[countfilled]->Draw();

    for (int i = 0; i < nTruthJets04; i++) {
      TLorentzVector jet = *(TLorentzVector*)truth_jet_4mom04->At(i);
      jet_object obj = make_jet(jet,0,0,0,0);
      if (obj.pt < anaclone.minjete04) continue;
      TMarker* jet1 = new TMarker(obj.eta,obj.phi, 24);
      jet1->SetMarkerSize(14);
      jet1->SetMarkerColor(kBlack);
      jet1->Draw();
    }
    
    for (int i = 0; i < nTruthClusters; i++) {
      TLorentzVector pho = *(TLorentzVector*)truth_cluster_4mom->At(i);
      pho_object obj = make_pho(pho,0,0,0,0);
      if (obj.pt < anaclone.minclustere) continue;
      TMarker* star1 = new TMarker(obj.eta,obj.phi, 29);
      star1->SetMarkerSize(1.5);
      if (obj.iso4 > 2) star1->SetMarkerColor(kBlack);
      else star1->SetMarkerColor(kBlue);
      star1->Draw();
    }

    c->SaveAs("pdfs/trutheventdisplays.pdf");
    countfilled++;
  }
  // the end

  c->SaveAs("pdfs/trutheventdisplays.pdf]");
  cout << endl << "Events found: " << countfilled << endl;

  std::cout << "All done!" << std::endl;
}
