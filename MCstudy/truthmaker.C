#include <iostream>
#include "/sphenix/user/samfred/projects/gammajet/headers/ana.cxx"
#include "/sphenix/user/samfred/projects/gammajet/headers/TreeSetting.h" 

// global variables
ana anaclone;
float mbd_t0;
bool ISMC;
TH2D * hcutbdt = new TH2D("hcutbdt",";Cut type;BDT score",3,0,3,100,0,1);
TH1D * htest = new TH1D("htest",";eta;counts",100,-1.5,1.5);
// Doesn't actually loop. Just checks that the found photon and found jet are a correct match for each other.
// This is where the xJ plot is filled
vector<int> niso(3);
vector<int> nbdt(3);
vector<int> nphoeta(3);
vector<int> njeteta(3);
vector<int> ntime(3);  
vector<int> nphi(3);   
vector<int> nphoe(3);  
bool loop(jet_object jet,int jindex, pho_object pho, int nindex) {

  float jetval = jet.pt;
  float phoval = pho.pt;
  float val = jetval/phoval;
  
  int eindex = anaclone.findPtBin(pho.pt);
  int abcdindex = anaclone.findabcdBin(pho.pt);
  bool isiso = pho.iso4 < anaclone.isoBins[0];
  bool isbdt = pho.bdt > anaclone.bdtBins[0];
  float dphi = abs(jet.phi - pho.phi);
  if (dphi > M_PI) dphi = 2*M_PI - dphi;
 
  if (nindex == 2) htest->Fill((pho.eta));
  if (!isiso) {niso[nindex]++; }
  if (!isbdt) {nbdt[nindex]++; }
  //if (eindex < 0) {nphoe[nindex]++;  }
  if (abs(pho.eta) > anaclone.etacut) {nphoeta[nindex]++; }
  if (abs(jet.eta) > anaclone.etacut - 0.2*(jindex+1)) {njeteta[nindex]++; }
  if (!ISMC && abs(pho.t - jet.t) > anaclone.tcut) {ntime[nindex]++;  }
  if (dphi < anaclone.oppcut) {nphi[nindex]++; }
  
  if (!isiso) { return false;}
  if (!isbdt) { return false;}
  //if (eindex < 0) { return false; }
  if (abs(pho.eta) > anaclone.etacut) { return false;}
  if (abs(jet.eta) > anaclone.etacut - 0.2*(jindex+1)) { return false;}
  if (!ISMC && abs(pho.t - jet.t) > anaclone.tcut) { return false; }
  if (dphi < anaclone.oppcut) { return false;}

  return true;
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
  if (pho.pt <= 0) return max;
  for (int i = 0; i < n; i++) {
    TLorentzVector jet = *(TLorentzVector*)jets->At(i);
    jet_object obj = make_jet(jet,efrac[i],ifrac[i],ofrac[i],t[i]*17.6);
    float dr = anaclone.deltaR(pho,obj);
    float dphi = abs(obj.phi - pho.phi);
    if (dphi > M_PI) dphi = 2*M_PI - dphi;
    int eindex = anaclone.findPtBin(pho.pt);
    if (!ISMC && (mbd_t0 - obj.t > anaclone.thighcut || mbd_t0 - obj.t < anaclone.tlowcut)) continue;
    if (dr < anaclone.drcut) continue;
    if (obj.pt > max.pt) {
      max = obj;
    }
  }
  return max;
}

// The called function
void cutflowmaker(int runnum=47289, bool isMC = 0, bool issmear = 0, const char * trigger = "Jet5", bool isfull = 1)
{
  gSystem->Load("libcaloana.so");
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
   

  // Filename management
  const char *dir;
  if (isMC && issmear) dir = Form("/sphenix/tg/tg01/jets/samfred/gammajet_MC_smear");
  else if (isMC && !issmear) dir = Form("/sphenix/tg/tg01/jets/samfred/gammajet_MC_unsmear");
  else dir =  Form("/sphenix/tg/tg01/jets/samfred/gammajet_calojet_hadded");
  string filename;
  if (isMC) filename = Form("%s/run28_%s.root",dir,trigger);
  else filename = Form("%s/run%i.root",dir,runnum);
  if (runnum == 0 && !isMC) filename = "gammajet_calojet.root";
  cout << "using file: " << filename << endl;
  TFile *f = new TFile(Form("%s",filename.c_str()),"read");
  TTree *t = (TTree*) f->Get("towerntup");
  if (!t) {
    cout << "This file is empty!" << endl;
    return;
  }
  treesetup(t); // Here all the branches of the ttree are set
  float minclustere = anaclone.minclustere;

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

  int npho = 0;
  int npholoose = 0;
  int nphotight = 0;
  int ncan = 0;
  int ncanloose = 0;
  int ncantight = 0;
  int njet = 0;
  int njetloose = 0;
  int njettight = 0;
  int npair = 0;
  int npairloose = 0;
  int npairtight = 0;
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
    if (!isMC && !ScaledTriggerBit[27]) continue;
    mbd_t0 = (mbd_time_south + mbd_time_north)/2.0 - t0corr;
    
    bool isc = 1;
    bool isj04 = 1;
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
    pho_object maxpho;
    pho_object maxpholoose;
    pho_object maxphotight;
    for (int i = 0; i < nClusters_sp; i++) {
      TLorentzVector pho = *(TLorentzVector*)cluster_4mom_sp->At(i);
      if (pho.Pt() < 10) continue;
      float iso3     = cluster_showershape_sp[i][ 8] + cluster_showershape_sp[i][ 9] + cluster_showershape_sp[i][10]; 
      float iso4     = cluster_showershape_sp[i][11] + cluster_showershape_sp[i][12] + cluster_showershape_sp[i][13]; 
      
      bool e1133     = cluster_showershape_sp[i][5] < 0.98;
      bool wetacogx  = cluster_showershape_sp[i][3] < 0.6;
      bool et1       = 0.60 < cluster_showershape_sp[i][0] && cluster_showershape_sp[i][0] < 1;
      bool e3235     = 0.80 < cluster_showershape_sp[i][6] && cluster_showershape_sp[i][6] < 1;
      bool wetacogxt = 0.00 < cluster_showershape_sp[i][3] && cluster_showershape_sp[i][3] < 0.15*pho.Pt();
      bool wphicogxt = 0.00 < cluster_showershape_sp[i][3] && cluster_showershape_sp[i][3] < 0.15*pho.Pt();
      bool e1133t    = 0.40 < cluster_showershape_sp[i][5] && e1133;
      bool et1t      = 0.90 < cluster_showershape_sp[i][0] && cluster_showershape_sp[i][0] < 1;
      bool e3235t    = 0.92 < cluster_showershape_sp[i][6] && cluster_showershape_sp[i][6] < 1;

      
      bool isloose = e1133 && wetacogx && et1 && e3235 && (wetacogxt + wphicogxt + e1133t + et1t + e3235t <= 3);
      bool istight = e1133 && wetacogx && et1 && e3235 && wetacogxt && wphicogxt && e1133t && et1t && e3235t;
      //bool isloose = wetacogx && et1;// && (wetacogxt + wphicogxt + e1133t + et1t + e3235t <= 3);
      //bool istight = wetacogx && et1 && wetacogxt && wphicogxt && et1t;

      npho++;
      npholoose += isloose;
      nphotight += istight;

      if (!isloose && !istight) hcutbdt->Fill(0.0,cluster_bdt_scores[i][0]);
      if ( isloose && !istight) hcutbdt->Fill(1.0,cluster_bdt_scores[i][0]);
      if (!isloose &&  istight) hcutbdt->Fill(2.0,cluster_bdt_scores[i][0]);
      pho_object obj = make_pho(pho,iso3,iso4,cluster_time_sp[i]*17.6,cluster_bdt_scores[i][0]);
      if (!ISMC && (mbd_t0 - obj.t > anaclone.thighcut || mbd_t0 - obj.t < anaclone.tlowcut)) continue;
      if (obj.pt > maxpho.pt) {
        maxpho = obj;
      }
      if (isloose && obj.pt > maxpholoose.pt) {
        maxpholoose = obj;
      }
      if (istight && obj.pt > maxphotight.pt) {
        maxphotight = obj;
      }
    }
    
    if (maxpho.pt > 0) ncan++;
    if (maxpholoose.pt > 0) ncanloose++;
    if (maxphotight.pt > 0) ncantight++;
    
    if (!isphoton && isMC && maxpho.pt > reco_threshmap_high["cluster"][trig]) {
      isj04 = false;
    }

    jet_object maxjet =      getmaxjet(jet_4mom04, nJets04, jet_emfrac04, jet_ihfrac04, jet_ohfrac04, jet_time04, maxpho);
    jet_object maxjetloose = getmaxjet(jet_4mom04, nJets04, jet_emfrac04, jet_ihfrac04, jet_ohfrac04, jet_time04, maxpholoose);
    jet_object maxjettight = getmaxjet(jet_4mom04, nJets04, jet_emfrac04, jet_ihfrac04, jet_ohfrac04, jet_time04, maxphotight);

    if (maxjet.pt > 0) njet++;
    if (maxjetloose.pt > 0) njetloose++;
    if (maxjettight.pt > 0) njettight++;

    // Fill the xJ histograms
    if ((isMC && isc && isj04 && maxjet.pt > anaclone.minjete04)      || (!isMC && maxjet.pt > anaclone.minjete04)) npair += loop(maxjet,1, maxpho,0);
    if ((isMC && isc && isj04 && maxjetloose.pt > anaclone.minjete04) || (!isMC && maxjetloose.pt > anaclone.minjete04)) npairloose += loop(maxjetloose, 1, maxpholoose,1);
    if ((isMC && isc && isj04 && maxjettight.pt > anaclone.minjete04) || (!isMC && maxjettight.pt > anaclone.minjete04)) npairtight += loop(maxjettight, 1, maxphotight,2);

  }
  // the end

  std::cout << std::endl << "All done!" << std::endl;
  cout << "                | All clusters |     loose cuts |    tight cuts" << endl;
  cout << "   Total number |" << setw(13) << npho << " |" << setw(15) << npholoose << " |" << setw(14) << nphotight << endl;
  cout << "   photon found |" << setw(13) << ncan << " |" << setw(15) << ncanloose << " |" << setw(14) << ncantight << endl;
  cout << "      Jet found |" << setw(13) << njet << " |" << setw(15) << njetloose << " |" << setw(14) << njettight << endl;
  cout << "      Fails iso |" << setw(13) << niso[0] << " |"<< setw(15) << niso[1] << " |"<< setw(14) << niso[2] << endl;
  cout << "      Fails bdt |" << setw(13) << nbdt[0] << " |"<< setw(15) << nbdt[1] << " |"<< setw(14) << nbdt[2] << endl;
  //cout << "    Fails pho E |" << setw(13) << nphoe[0]   << " |"<< setw(15) << nphoe[1]   << " |"<< setw(14) << nphoe[2]   << endl;
  cout << "  Fails pho eta |" << setw(13) << nphoeta[0] << " |"<< setw(15) << nphoeta[1] << " |"<< setw(14) << nphoeta[2] << endl;
  cout << "  Fails jet eta |" << setw(13) << njeteta[0] << " |"<< setw(15) << njeteta[1] << " |"<< setw(14) << njeteta[2] << endl;
  cout << "     Fails dphi |" << setw(13) << nphi[0]    << " |"<< setw(15) << nphi[1]    << " |"<< setw(14) << nphi[2]    << endl;
  cout << "     Fails time |" << setw(13) << ntime[0]   << " |"<< setw(15) << ntime[1]   << " |"<< setw(14) << ntime[2]   << endl;
  cout << "     Pair found |" << setw(13) << npair << " |" << setw(15) << npairloose << " |" << setw(14) << npairtight << endl;
  return;
}
