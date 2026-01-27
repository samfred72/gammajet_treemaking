#include <iostream>
#include "CaloAna.h"
using namespace std;

CaloAna::CaloAna(const std::string& name, const char* outname)
  : SubsysReco(name)
    , detector("HCALIN")
{
  outfilename = Form("%s",outname);
}

CaloAna::~CaloAna()
{
  //delete hm;
  delete towerntuple;
}

int CaloAna::Init(PHCompositeNode*)
{ 
  //triggeranalyzer = new TriggerAnalyzer();
  //triggeranalyzer->UseEmulator(useEmulator);
  try {
    InitOutputFile();
    InitTree();
  }  
  catch (const std::exception& e){
    std::cerr << "CaloAna::Init - Exception during init! For " << e.what() << " return -1" << std::endl;
    return -1;
  }
  TFile * fhist = TFile::Open("/sphenix/user/samfred/ppg11ana/Profile/newprof/profilehists.root","READ");
  profilehist = (TH1D*)fhist->Get("energyprof1D");

  return 0;
}

void CaloAna::InitOutputFile(){
  std::cout << "output filename : " << outfilename.c_str() << std::endl;
  outfile = new TFile(outfilename.c_str(), "RECREATE");
  if(!outfile || outfile->IsZombie()){
    throw std::runtime_error("Failed open file");
  }
}

void CaloAna::InitTree(){
  towerntuple = new TTree("towerntup", "Ntuple");
  // Global
  towerntuple->Branch("RunNumber",&m_run_number);
  towerntuple->Branch("vz",&vz);
  towerntuple->Branch("truth_vz",&vz);
  towerntuple->Branch("istriggeredsp",&istriggeredsp);
  towerntuple->Branch("istriggeredns",&istriggeredns);
  // Clusters
  towerntuple->Branch("nClusters_sp",&nClusters_sp, "nClusters_sp/S");
  towerntuple->Branch("cluster_4mom_sp","TClonesArray",&m_cluster_4mom_sp,32000,0);
  towerntuple->Branch("cluster_showershape_sp",m_cluster_showershape_sp,"cluster_showershape_sp[nClusters_sp][8]/F");
  towerntuple->Branch("cluster_time_sp",m_cluster_time_sp,"cluster_time_sp[nClusters_sp]/F");
  towerntuple->Branch("cluster_iso_e02_sp",m_cluster_iso_e02_sp,"cluster_iso_e02_sp[nClusters_sp]/F");
  towerntuple->Branch("cluster_iso_e04_sp",m_cluster_iso_e04_sp,"cluster_iso_e04_sp[nClusters_sp]/F");
  towerntuple->Branch("cluster_iso_e06_sp",m_cluster_iso_e06_sp,"cluster_iso_e06_sp[nClusters_sp]/F");
  towerntuple->Branch("cluster_iso_e08_sp",m_cluster_iso_e08_sp,"cluster_iso_e08_sp[nClusters_sp]/F");
  towerntuple->Branch("nClusters_ns",&nClusters_ns, "nClusters_ns/S");
  towerntuple->Branch("cluster_4mom_ns","TClonesArray",&m_cluster_4mom_ns,32000,0);
  towerntuple->Branch("cluster_showershape_ns",m_cluster_showershape_ns,"cluster_showershape_ns[nClusters_ns][8]/F");
  towerntuple->Branch("cluster_time_ns",m_cluster_time_ns,"cluster_time_ns[nClusters_ns]/F");
  towerntuple->Branch("cluster_iso_e02_ns",m_cluster_iso_e02_ns,"cluster_iso_e02_ns[nClusters_ns]/F");
  towerntuple->Branch("cluster_iso_e04_ns",m_cluster_iso_e04_ns,"cluster_iso_e04_ns[nClusters_ns]/F");
  towerntuple->Branch("cluster_iso_e06_ns",m_cluster_iso_e06_ns,"cluster_iso_e06_ns[nClusters_ns]/F");
  towerntuple->Branch("cluster_iso_e08_ns",m_cluster_iso_e08_ns,"cluster_iso_e08_ns[nClusters_ns]/F");
  towerntuple->Branch("nTruthClusters",&nTruthClusters, "nTruthClusters/S");
  towerntuple->Branch("truth_cluster_4mom","TClonesArray",&m_truth_cluster_4mom,32000,0);
  towerntuple->Branch("truth_cluster_time",m_truth_cluster_time,"truth_cluster_time[nTruthClusters]/F");

  //jets
  towerntuple->Branch("nJets02",&nJets02, "nJets02/S");
  towerntuple->Branch("jet_4mom02","TClonesArray",&m_jet_4mom02,32000,0);
  towerntuple->Branch("jet_emfrac02",m_jet_emfrac02,"jet_emfrac02[nJets02]/F");
  towerntuple->Branch("jet_ihfrac02",m_jet_ihfrac02,"jet_ihfrac02[nJets02]/F");
  towerntuple->Branch("jet_ohfrac02",m_jet_ohfrac02,"jet_ohfrac02[nJets02]/F");
  towerntuple->Branch("jet_time02",m_jet_time02,"jet_time02[nJets02]/F");
  towerntuple->Branch("nJets04",&nJets04, "nJets04/S");
  towerntuple->Branch("jet_4mom04","TClonesArray",&m_jet_4mom04,32000,0);
  towerntuple->Branch("jet_emfrac04",m_jet_emfrac04,"jet_emfrac04[nJets04]/F");
  towerntuple->Branch("jet_ihfrac04",m_jet_ihfrac04,"jet_ihfrac04[nJets04]/F");
  towerntuple->Branch("jet_ohfrac04",m_jet_ohfrac04,"jet_ohfrac04[nJets04]/F");
  towerntuple->Branch("jet_time04",m_jet_time04,"jet_time04[nJets04]/F");
  towerntuple->Branch("nJets06",&nJets06, "nJets06/S");
  towerntuple->Branch("jet_4mom06","TClonesArray",&m_jet_4mom06,32000,0);
  towerntuple->Branch("jet_emfrac06",m_jet_emfrac06,"jet_emfrac06[nJets06]/F");
  towerntuple->Branch("jet_ihfrac06",m_jet_ihfrac06,"jet_ihfrac06[nJets06]/F");
  towerntuple->Branch("jet_ohfrac06",m_jet_ohfrac06,"jet_ohfrac06[nJets06]/F");
  towerntuple->Branch("jet_time06",m_jet_time06,"jet_time06[nJets06]/F");
  towerntuple->Branch("nJets08",&nJets08, "nJets08/S");
  towerntuple->Branch("jet_4mom08","TClonesArray",&m_jet_4mom08,32000,0);
  towerntuple->Branch("jet_emfrac08",m_jet_emfrac08,"jet_emfrac08[nJets08]/F");
  towerntuple->Branch("jet_ihfrac08",m_jet_ihfrac08,"jet_ihfrac08[nJets08]/F");
  towerntuple->Branch("jet_ohfrac08",m_jet_ohfrac08,"jet_ohfrac08[nJets08]/F");
  towerntuple->Branch("jet_time08",m_jet_time08,"jet_time08[nJets08]/F");

  towerntuple->Branch("nTruthJets02",&nTruthJets02, "nTruthJets02/S");
  towerntuple->Branch("truth_jet_4mom02","TClonesArray",&m_truth_jet_4mom02,32000,0);
  towerntuple->Branch("truth_jet_emfrac02",m_truth_jet_emfrac02,"truth_jet_emfrac02[nJets02]/F");
  towerntuple->Branch("truth_jet_ihfrac02",m_truth_jet_ihfrac02,"truth_jet_ihfrac02[nJets02]/F");
  towerntuple->Branch("truth_jet_ohfrac02",m_truth_jet_ohfrac02,"truth_jet_ohfrac02[nJets02]/F");
  towerntuple->Branch("truth_jet_time02",m_truth_jet_time02,"truth_jet_time02[nJets02]/F");
  towerntuple->Branch("nTruthJets04",&nTruthJets04, "nTruthJets04/S");
  towerntuple->Branch("truth_jet_4mom04","TClonesArray",&m_truth_jet_4mom04,32000,0);
  towerntuple->Branch("truth_jet_emfrac04",m_truth_jet_emfrac04,"truth_jet_emfrac04[nJets04]/F");
  towerntuple->Branch("truth_jet_ihfrac04",m_truth_jet_ihfrac04,"truth_jet_ihfrac04[nJets04]/F");
  towerntuple->Branch("truth_jet_ohfrac04",m_truth_jet_ohfrac04,"truth_jet_ohfrac04[nJets04]/F");
  towerntuple->Branch("truth_jet_time04",m_truth_jet_time04,"truth_jet_time04[nJets04]/F");
  towerntuple->Branch("nTruthJets06",&nTruthJets06, "nTruthJets06/S");
  towerntuple->Branch("truth_jet_4mom06","TClonesArray",&m_truth_jet_4mom06,32000,0);
  towerntuple->Branch("truth_jet_emfrac06",m_truth_jet_emfrac06,"truth_jet_emfrac06[nJets06]/F");
  towerntuple->Branch("truth_jet_ihfrac06",m_truth_jet_ihfrac06,"truth_jet_ihfrac06[nJets06]/F");
  towerntuple->Branch("truth_jet_ohfrac06",m_truth_jet_ohfrac06,"truth_jet_ohfrac06[nJets06]/F");
  towerntuple->Branch("truth_jet_time06",m_truth_jet_time06,"truth_jet_time06[nJets06]/F");
  towerntuple->Branch("nTruthJets08",&nTruthJets08, "nTruthJets08/S");
  towerntuple->Branch("truth_jet_4mom08","TClonesArray",&m_truth_jet_4mom08,32000,0);
  towerntuple->Branch("truth_jet_emfrac08",m_truth_jet_emfrac08,"truth_jet_emfrac08[nJets08]/F");
  towerntuple->Branch("truth_jet_ihfrac08",m_truth_jet_ihfrac08,"truth_jet_ihfrac08[nJets08]/F");
  towerntuple->Branch("truth_jet_ohfrac08",m_truth_jet_ohfrac08,"truth_jet_ohfrac08[nJets08]/F");
  towerntuple->Branch("truth_jet_time08",m_truth_jet_time08,"truth_jet_time08[nJets08]/F");
}

int CaloAna::process_event(PHCompositeNode* topNode)
{
  if(!topNode){
    std::cerr << "CaloAna::Init - topnode PHCompositeNode not valid! return -1" << std::endl;
    return -1;
  }
  if(count % 1000 == 0) std::cout << "event : " << count  << std::endl;
  count++;
  if (process_towers(topNode)) {
    return Fun4AllReturnCodes::ABORTEVENT;
  }
  return Fun4AllReturnCodes::EVENT_OK;
}

int CaloAna::process_towers(PHCompositeNode* topNode)
{
  if (ProcessGlobalEventInfo(topNode)) {
    return 1;
  }

  // MBD energy and hits
  MbdOut * mbdout = static_cast<MbdOut*>(findNode::getClass<MbdOut>(topNode, "MbdOut"));
  if (mbdout) {
    m_mbd_nhits_south = mbdout->get_npmt(0);
    m_mbd_nhits_north = mbdout->get_npmt(1);
    m_mbd_time_south = mbdout->get_time(0);
    m_mbd_time_north = mbdout->get_time(1);
  }
  
  // Clusters
  TowerInfoContainer* offlinetowers = static_cast<TowerInfoContainer*>(findNode::getClass<TowerInfoContainer>(topNode, "TOWERINFO_CALIB_CEMC"));
  if (!offlinetowers) {
    std::cout << "No offline towers node!" << std::endl;
    return 1;
  }
  // split cluster loop
  RawClusterContainer *clustersEM = static_cast<RawClusterContainer*>(findNode::getClass<RawClusterContainer>(topNode, "CLUSTERINFO_CEMC"));
  RawClusterContainer::ConstIterator hiter;
  RawClusterContainer::ConstRange begin_end = clustersEM->getClusters(); 

  nClusters_sp = 0;
  bool keepevent = false;
  for(hiter = begin_end.first; hiter != begin_end.second; ++hiter)
  {
    RawCluster* rawcluster = hiter->second;
    CLHEP::Hep3Vector vertex;
    vertex.set(vx,vy,vz);
    CLHEP::Hep3Vector E_vec_cluster = RawClusterUtility::GetEVec(*rawcluster, vertex);

    float clPt = E_vec_cluster.perp();
    float clEta = E_vec_cluster.pseudoRapidity();
    float clPhi = E_vec_cluster.phi();
    float clE = E_vec_cluster.mag();
    if (clE > cluster_emin) {keepevent = true; istriggeredsp = true; }

    TLorentzVector vphoton1;
    vphoton1.SetPtEtaPhiE(clPt,clEta,clPhi,clE);
    std::vector<float> showershape = rawcluster->get_shower_shapes(0.07);

    float nearby_time = 0;
    float nearby_energy = 0;
    float iso_e02 = 0;
    float iso_e04 = 0;
    float iso_e06 = 0;
    float iso_e08 = 0;

    int etacenter = std::floor(showershape[4] + 0.5);
    int phicenter = std::floor(showershape[5] + 0.5);
    int vectorSize = inputDimx * inputDimy;

    vector<float> vcluster(49);
    int vecindex = 0;
    int xlength = int((inputDimx - 1) / 2);
    int ylength = int((inputDimy - 1) / 2);
    for (int ieta = etacenter - ylength; ieta <= etacenter + ylength; ieta++)
    {
      for (int iphi = phicenter - xlength; iphi <= phicenter + xlength; iphi++)
      {
        int index = (ieta - etacenter + ylength) * inputDimx + iphi - phicenter + xlength;
        if(ieta < 0 || ieta >=96){
          if ((index % inputDimx > (inputDimx - 1)/2 - 4 && index % inputDimx < (inputDimx - 1)/2 + 4) &&
              (index / inputDimy > (inputDimy - 1)/2 - 4 && index / inputDimy < (inputDimy - 1)/2 + 4)) {
            vcluster[vecindex] = 0;
            vecindex++;
          }
          continue;
        }
        int mappediphi = iphi;
        if (mappediphi < 0) mappediphi += 256;
        if (mappediphi > 255) mappediphi -= 256;
        
        unsigned int towerinfokey = TowerInfoDefs::encode_emcal(ieta, mappediphi);
        TowerInfo *towerinfo = offlinetowers->get_tower_at_key(towerinfokey);
        float te = towerinfo->get_energy();
        float tt = towerinfo->get_time();

        if ((index % inputDimx > (inputDimx - 1)/2 - 4 && index % inputDimx < (inputDimx - 1)/2 + 4) &&
            (index / inputDimy > (inputDimy - 1)/2 - 4 && index / inputDimy < (inputDimy - 1)/2 + 4)) {
          vcluster[vecindex] = (te > 0.07 ? te : 0);
          vecindex++;
        }

        float distance = TMath::Sqrt((etacenter-ieta)*(etacenter-ieta) + (phicenter-mappediphi)*(phicenter-mappediphi));
        if (distance < 0.17/0.024) {
          if (!std::isnan(te) && !std::isnan(tt) && te > 0.07) {
            nearby_time += te*tt;
            nearby_energy += te;
          }
        }

        if (distance < 0.2/0.024) iso_e02 += te;
        if (distance < 0.4/0.024) iso_e04 += te;
        if (distance < 0.6/0.024) iso_e06 += te;
        if (distance < 0.8/0.024) iso_e08 += te;

      }
    }
    if (nearby_energy != 0) {
      nearby_time /= nearby_energy;
    }
    else nearby_time = -999;
    m_cluster_iso_e02_sp[nClusters_sp] = iso_e02 - vphoton1.E();
    m_cluster_iso_e04_sp[nClusters_sp] = iso_e04 - vphoton1.E();
    m_cluster_iso_e06_sp[nClusters_sp] = iso_e06 - vphoton1.E();
    m_cluster_iso_e08_sp[nClusters_sp] = iso_e08 - vphoton1.E();
    new ((*m_cluster_4mom_sp)[nClusters_sp]) TLorentzVector(vphoton1);
    m_cluster_time_sp[nClusters_sp] = nearby_time;

    // showershapes
    float e11 = vcluster.at(24);
    float e33 = 0;
    float xcm = 0;
    float ycm = 0;
    float etot = 0;
    for (int i = 0; i < 7; i++) {
      for (int j = 0; j < 7; j++) {
        int I = i*7 + j;
        float e = vcluster.at(I);
        //cluster->Fill(i,j,e);
        if (e < 0.07) { vcluster.at(I) = 0; e = 0;}
        if (i >= 2 && i <= 4 && j >= 2 && j <= 4) e33 += e;
        xcm += i*e;
        ycm += j*e;
        etot += e;
      }
    }
    xcm = xcm/etot - 3;
    ycm = ycm/etot - 3;
    float et1;
    float diag;
    float leg1;
    float leg2;
    if (xcm > 0 && ycm > 0) {
      et1 = vcluster.at(24)+vcluster.at(25)+vcluster.at(31)+vcluster.at(32);
      diag = vcluster.at(24)+vcluster.at(32);
      leg1 = vcluster.at(24)+vcluster.at(25);
      leg2 = vcluster.at(24)+vcluster.at(21);
    }
    if (xcm > 0 && ycm < 0) {
      et1 = vcluster.at(24)+vcluster.at(23)+vcluster.at(31)+vcluster.at(30);
      diag = vcluster.at(24)+vcluster.at(30);
      leg1 = vcluster.at(24)+vcluster.at(23);
      leg2 = vcluster.at(24)+vcluster.at(31);
    }
    if (xcm < 0 && ycm > 0) {
      et1 = vcluster.at(24)+vcluster.at(25)+vcluster.at(17)+vcluster.at(18);
      diag = vcluster.at(24)+vcluster.at(18);
      leg1 = vcluster.at(24)+vcluster.at(17);
      leg2 = vcluster.at(24)+vcluster.at(25);
    }
    if (xcm < 0 && ycm < 0) {
      et1 = vcluster.at(24)+vcluster.at(23)+vcluster.at(17)+vcluster.at(16);
      diag = vcluster.at(24)+vcluster.at(16);
      leg1 = vcluster.at(24)+vcluster.at(17);
      leg2 = vcluster.at(24)+vcluster.at(23);
    }

    et1 /= etot;
    int jint = (ycm < 0 ? 2 : 3);
    float e32 = 0;
    for (int i = 2; i < 5; i++) {
      for (int j = jint; j < jint + 2; j++) {
        int I = i*7+j;
        e32 += vcluster.at(I);
      }
    }
    float e35 = 0;
    for (int i = 2; i < 5; i++) {
      for (int j = 1; j < 6; j++) {
        int I = i*7+j;
        e35 += vcluster.at(I);
      }
    }
    float weta_cogx = 0;

    for (int i = 0; i < 7; i++) {
      for (int j = 0; j < 7; j++) {
        int I = i*7+j;
        if (i==3 && j==3) continue;
        float di_float= i - (3+((showershape[4])-floor(showershape[4]+0.5)));
        weta_cogx += vcluster.at(I)*di_float*di_float;
      }
    }
    weta_cogx /= etot;

    m_cluster_showershape_sp[nClusters_sp][0] = e11;
    m_cluster_showershape_sp[nClusters_sp][1] = e33;
    m_cluster_showershape_sp[nClusters_sp][2] = e32;
    m_cluster_showershape_sp[nClusters_sp][3] = e35;
    m_cluster_showershape_sp[nClusters_sp][4] = et1;
    m_cluster_showershape_sp[nClusters_sp][5] = weta_cogx;

    nClusters_sp++;

  }
  
  //mother cluster loop
  RawClusterContainer *clustersEM_ns = static_cast<RawClusterContainer*>(findNode::getClass<RawClusterContainer>(topNode, "CEMC_CLUSTERINFO_MOTHER"));
  begin_end = clustersEM_ns->getClusters(); 
  nClusters_ns = 0;
  for(hiter = begin_end.first; hiter != begin_end.second; ++hiter)
  {
    RawCluster* rawcluster = hiter->second;
    CLHEP::Hep3Vector vertex;
    vertex.set(vx,vy,vz);
    CLHEP::Hep3Vector E_vec_cluster = RawClusterUtility::GetEVec(*rawcluster, vertex);

    float clPt = E_vec_cluster.perp();
    float clEta = E_vec_cluster.pseudoRapidity();
    float clPhi = E_vec_cluster.phi();
    float clE = E_vec_cluster.mag();
    if (clE > cluster_emin) {keepevent = true; istriggeredns = true; }

    TLorentzVector vphoton1;
    vphoton1.SetPtEtaPhiE(clPt,clEta,clPhi,clE);
    std::vector<float> showershape = rawcluster->get_shower_shapes(0.07);

    float nearby_time = 0;
    float nearby_energy = 0;
    float iso_e02 = 0;
    float iso_e04 = 0;
    float iso_e06 = 0;
    float iso_e08 = 0;

    int etacenter = std::floor(showershape[4] + 0.5);
    int phicenter = std::floor(showershape[5] + 0.5);
    int vectorSize = inputDimx * inputDimy;

    vector<float> vcluster(49);
    int vecindex = 0;
    int xlength = int((inputDimx - 1) / 2);
    int ylength = int((inputDimy - 1) / 2);
    for (int ieta = etacenter - ylength; ieta <= etacenter + ylength; ieta++)
    {
      for (int iphi = phicenter - xlength; iphi <= phicenter + xlength; iphi++)
      {
        int index = (ieta - etacenter + ylength) * inputDimx + iphi - phicenter + xlength;
        if(ieta < 0 || ieta >=96){
          if ((index % inputDimx > (inputDimx - 1)/2 - 4 && index % inputDimx < (inputDimx - 1)/2 + 4) &&
              (index / inputDimy > (inputDimy - 1)/2 - 4 && index / inputDimy < (inputDimy - 1)/2 + 4)) {
            vcluster[vecindex] = 0;
            vecindex++;
          }
          continue;
        }
        int mappediphi = iphi;
        if (mappediphi < 0) mappediphi += 256;
        if (mappediphi > 255) mappediphi -= 256;
        unsigned int towerinfokey = TowerInfoDefs::encode_emcal(ieta, mappediphi);

        TowerInfo *towerinfo = offlinetowers->get_tower_at_key(towerinfokey);
        float te = towerinfo->get_energy();
        float tt = towerinfo->get_time();
        
        if ((index % inputDimx > (inputDimx - 1)/2 - 4 && index % inputDimx < (inputDimx - 1)/2 + 4) &&
            (index / inputDimy > (inputDimy - 1)/2 - 4 && index / inputDimy < (inputDimy - 1)/2 + 4)) {
          vcluster[vecindex] = (te > 0.07 ? te : 0);
          vecindex++;
        }
        
        float distance = TMath::Sqrt((etacenter-ieta)*(etacenter-ieta) + (phicenter-mappediphi)*(phicenter-mappediphi));
        
        if (distance < 0.17/0.024) {
          if (!std::isnan(te) && !std::isnan(tt) && te > 0.07) {
            nearby_time += te*tt;
            nearby_energy += te;
          }
        }

        if (distance < 0.2/0.024 && te > 0.07) iso_e02 += te;
        if (distance < 0.4/0.024 && te > 0.07) iso_e04 += te;
        if (distance < 0.6/0.024 && te > 0.07) iso_e06 += te;
        if (distance < 0.8/0.024 && te > 0.07) iso_e08 += te;
        
      }
    }
    if (nearby_energy != 0) {
      nearby_time /= nearby_energy;
    }
    else nearby_time = -999;
    
    m_cluster_iso_e02_ns[nClusters_ns] = iso_e02 - vphoton1.E();
    m_cluster_iso_e04_ns[nClusters_ns] = iso_e04 - vphoton1.E();
    m_cluster_iso_e06_ns[nClusters_ns] = iso_e06 - vphoton1.E();
    m_cluster_iso_e08_ns[nClusters_ns] = iso_e08 - vphoton1.E();
    new ((*m_cluster_4mom_ns)[nClusters_ns]) TLorentzVector(vphoton1);
    for (int i = 0 ; i < 8; i++) {
      m_cluster_showershape_ns[nClusters_ns][i] = showershape.at(i);
    }
    m_cluster_time_ns[nClusters_ns] = nearby_time;
    
    // showershapes
    float e11 = vcluster.at(24);
    float e33 = 0;
    float xcm = 0;
    float ycm = 0;
    float etot = 0;
    for (int i = 0; i < 7; i++) {
      for (int j = 0; j < 7; j++) {
        int I = i*7 + j;
        float e = vcluster.at(I);
        //cluster->Fill(i,j,e);
        if (e < 0.07) { vcluster.at(I) = 0; e = 0;}
        if (i >= 2 && i <= 4 && j >= 2 && j <= 4) e33 += e;
        xcm += i*e;
        ycm += j*e;
        etot += e;
      }
    }
    xcm = xcm/etot - 3;
    ycm = ycm/etot - 3;
    float et1;
    float diag;
    float leg1;
    float leg2;
    if (xcm > 0 && ycm > 0) {
      et1 = vcluster.at(24)+vcluster.at(25)+vcluster.at(31)+vcluster.at(32);
      diag = vcluster.at(24)+vcluster.at(32);
      leg1 = vcluster.at(24)+vcluster.at(25);
      leg2 = vcluster.at(24)+vcluster.at(21);
    }
    if (xcm > 0 && ycm < 0) {
      et1 = vcluster.at(24)+vcluster.at(23)+vcluster.at(31)+vcluster.at(30);
      diag = vcluster.at(24)+vcluster.at(30);
      leg1 = vcluster.at(24)+vcluster.at(23);
      leg2 = vcluster.at(24)+vcluster.at(31);
    }
    if (xcm < 0 && ycm > 0) {
      et1 = vcluster.at(24)+vcluster.at(25)+vcluster.at(17)+vcluster.at(18);
      diag = vcluster.at(24)+vcluster.at(18);
      leg1 = vcluster.at(24)+vcluster.at(17);
      leg2 = vcluster.at(24)+vcluster.at(25);
    }
    if (xcm < 0 && ycm < 0) {
      et1 = vcluster.at(24)+vcluster.at(23)+vcluster.at(17)+vcluster.at(16);
      diag = vcluster.at(24)+vcluster.at(16);
      leg1 = vcluster.at(24)+vcluster.at(17);
      leg2 = vcluster.at(24)+vcluster.at(23);
    }

    et1 /= etot;
    int jint = (ycm < 0 ? 2 : 3);
    float e32 = 0;
    for (int i = 2; i < 5; i++) {
      for (int j = jint; j < jint + 2; j++) {
        int I = i*7+j;
        e32 += vcluster.at(I);
      }
    }
    float e35 = 0;
    for (int i = 2; i < 5; i++) {
      for (int j = 1; j < 6; j++) {
        int I = i*7+j;
        e35 += vcluster.at(I);
      }
    }
    float weta_cogx = 0;

    for (int i = 0; i < 7; i++) {
      for (int j = 0; j < 7; j++) {
        int I = i*7+j;
        if (i==3 && j==3) continue;
        float di_float= i - (3+((showershape[4])-floor(showershape[4]+0.5)));
        weta_cogx += vcluster.at(I)*di_float*di_float;
      }
    }
    weta_cogx /= etot;

    m_cluster_showershape_ns[nClusters_ns][0] = e11;
    m_cluster_showershape_ns[nClusters_ns][1] = e33;
    m_cluster_showershape_ns[nClusters_ns][2] = e32;
    m_cluster_showershape_ns[nClusters_ns][3] = e35;
    m_cluster_showershape_ns[nClusters_ns][4] = et1;
    m_cluster_showershape_ns[nClusters_ns][5] = weta_cogx;
    nClusters_ns++;
  }
  if (!keepevent) return 1;
  std::cout << "Event kept!!!!!!!" << std::endl;
  
  // Jets
  // Get the towerinfo containers
  TowerInfoContainer *_emcal_towers = nullptr;
  TowerInfoContainer *_ihcal_towers = nullptr;
  TowerInfoContainer *_ohcal_towers = nullptr;
  _emcal_towers = findNode::getClass<TowerInfoContainer>(topNode, "TOWERINFO_CALIB_CEMC_RETOWER");
  _ihcal_towers = findNode::getClass<TowerInfoContainer>(topNode, "TOWERINFO_CALIB_HCALIN");
  _ohcal_towers = findNode::getClass<TowerInfoContainer>(topNode, "TOWERINFO_CALIB_HCALOUT");
  if (!_emcal_towers || !_ihcal_towers || !_ohcal_towers) {
    std::cout << "TowerInfoContainer is missing" << std::endl;
    return 1;
  }
  
  JetContainer* _jets02 = findNode::getClass<JetContainer>(topNode, "AntiKt_unsubtracted_r02");
  JetContainer* _jets04 = findNode::getClass<JetContainer>(topNode, "AntiKt_unsubtracted_r04");
  JetContainer* _jets06 = findNode::getClass<JetContainer>(topNode, "AntiKt_unsubtracted_r06");
  JetContainer* _jets08 = findNode::getClass<JetContainer>(topNode, "AntiKt_unsubtracted_r08");
  if (!_jets02 || !_jets04 || !_jets06 || !_jets08) {
    std::cout << "JetContainer is missing" << std::endl;
    return 1;
  }
  // Fill the jet information.
  nJets02 = 0;
  for (auto _jet : *_jets02) {
    if (_jet->get_e() < jet_emin_cut) continue;

    float emcal_calo_e = 0;
    float ihcal_calo_e = 0;
    float ohcal_calo_e = 0;
    float jet_time = 0;
    float jet_time_count = 0;
    for (auto comp: _jet->get_comp_vec()) {
	    unsigned int channel = comp.second;
      // emcal bits
      if (comp.first == 14 || comp.first == 29 || comp.first == 25 || comp.first == 28) {
        TowerInfo *_tower = _emcal_towers->get_tower_at_channel(channel);
	      float jet_tower_e = _tower->get_energy();
        float jet_tower_time = _tower->get_time();
        emcal_calo_e += jet_tower_e;
        if (jet_tower_e > 0.1) {
          jet_time += jet_tower_time * jet_tower_e;
          jet_time_count += jet_tower_e;
        }
	    } 
      // ihcal  bits
      else if (comp.first == 15 ||  comp.first == 30 || comp.first == 26) {
        TowerInfo *_tower = _ihcal_towers->get_tower_at_channel(channel);
	      float jet_tower_e = _tower->get_energy();
        float jet_tower_time = _tower->get_time();
        ihcal_calo_e += jet_tower_e;
        if (jet_tower_e > 0.1) {
          jet_time += jet_tower_time * jet_tower_e;
          jet_time_count += jet_tower_e;
        }
	    } 
      // ohcal bits
      else if (comp.first == 16 || comp.first == 31 || comp.first == 27) {
        TowerInfo *_tower = _ohcal_towers->get_tower_at_channel(channel);
	      float jet_tower_e = _tower->get_energy();
        float jet_tower_time = _tower->get_time();
        ohcal_calo_e += jet_tower_e;
        if (jet_tower_e > 0.1) {
          jet_time += jet_tower_time * jet_tower_e;
          jet_time_count += jet_tower_e;
        }
      }
	  }
    if (jet_time_count > 0) jet_time = jet_time / jet_time_count;
    else jet_time = -999;

    TLorentzVector jet_4mom;
    jet_4mom.SetPtEtaPhiE(_jet->get_pt(),_jet->get_eta(),_jet->get_phi(),_jet->get_e());
    new ((*m_jet_4mom02)[nJets02]) TLorentzVector(jet_4mom);
    m_jet_emfrac02[nJets02] = emcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_ihfrac02[nJets02] = ihcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_ohfrac02[nJets02] = ohcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_time02[nJets02] = jet_time;
    nJets02++;
  }
  nJets04 = 0;
  for (auto _jet : *_jets04) {
    if (_jet->get_e() < jet_emin_cut) continue;

    float emcal_calo_e = 0;
    float ihcal_calo_e = 0;
    float ohcal_calo_e = 0;
    float jet_time = 0;
    float jet_time_count = 0;
    for (auto comp: _jet->get_comp_vec()) {
	    unsigned int channel = comp.second;
      // emcal bits
      if (comp.first == 14 || comp.first == 29 || comp.first == 25 || comp.first == 28) {
        TowerInfo *_tower = _emcal_towers->get_tower_at_channel(channel);
	      float jet_tower_e = _tower->get_energy();
        float jet_tower_time = _tower->get_time();
        emcal_calo_e += jet_tower_e;
        if (jet_tower_e > 0.1) {
          jet_time += jet_tower_time * jet_tower_e;
          jet_time_count += jet_tower_e;
        }
	    } 
      // ihcal  bits
      else if (comp.first == 15 ||  comp.first == 30 || comp.first == 26) {
        TowerInfo *_tower = _ihcal_towers->get_tower_at_channel(channel);
	      float jet_tower_e = _tower->get_energy();
        float jet_tower_time = _tower->get_time();
        ihcal_calo_e += jet_tower_e;
        if (jet_tower_e > 0.1) {
          jet_time += jet_tower_time * jet_tower_e;
          jet_time_count += jet_tower_e;
        }
	    } 
      // ohcal bits
      else if (comp.first == 16 || comp.first == 31 || comp.first == 27) {
        TowerInfo *_tower = _ohcal_towers->get_tower_at_channel(channel);
	      float jet_tower_e = _tower->get_energy();
        float jet_tower_time = _tower->get_time();
        ohcal_calo_e += jet_tower_e;
        if (jet_tower_e > 0.1) {
          jet_time += jet_tower_time * jet_tower_e;
          jet_time_count += jet_tower_e;
        }
      }
	  }
    if (jet_time_count > 0) jet_time = jet_time / jet_time_count;
    else jet_time = -999;

    TLorentzVector jet_4mom;
    jet_4mom.SetPtEtaPhiE(_jet->get_pt(),_jet->get_eta(),_jet->get_phi(),_jet->get_e());
    new ((*m_jet_4mom04)[nJets04]) TLorentzVector(jet_4mom);
    m_jet_emfrac04[nJets04] = emcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_ihfrac04[nJets04] = ihcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_ohfrac04[nJets04] = ohcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_time04[nJets04] = jet_time;
    nJets04++;
  }
  nJets06 = 0;
  for (auto _jet : *_jets06) {
    if (_jet->get_e() < jet_emin_cut) continue;

    float emcal_calo_e = 0;
    float ihcal_calo_e = 0;
    float ohcal_calo_e = 0;
    float jet_time = 0;
    float jet_time_count = 0;
    for (auto comp: _jet->get_comp_vec()) {
	    unsigned int channel = comp.second;
      // emcal bits
      if (comp.first == 14 || comp.first == 29 || comp.first == 25 || comp.first == 28) {
        TowerInfo *_tower = _emcal_towers->get_tower_at_channel(channel);
	      float jet_tower_e = _tower->get_energy();
        float jet_tower_time = _tower->get_time();
        emcal_calo_e += jet_tower_e;
        if (jet_tower_e > 0.1) {
          jet_time += jet_tower_time * jet_tower_e;
          jet_time_count += jet_tower_e;
        }
	    } 
      // ihcal  bits
      else if (comp.first == 15 ||  comp.first == 30 || comp.first == 26) {
        TowerInfo *_tower = _ihcal_towers->get_tower_at_channel(channel);
	      float jet_tower_e = _tower->get_energy();
        float jet_tower_time = _tower->get_time();
        ihcal_calo_e += jet_tower_e;
        if (jet_tower_e > 0.1) {
          jet_time += jet_tower_time * jet_tower_e;
          jet_time_count += jet_tower_e;
        }
	    } 
      // ohcal bits
      else if (comp.first == 16 || comp.first == 31 || comp.first == 27) {
        TowerInfo *_tower = _ohcal_towers->get_tower_at_channel(channel);
	      float jet_tower_e = _tower->get_energy();
        float jet_tower_time = _tower->get_time();
        ohcal_calo_e += jet_tower_e;
        if (jet_tower_e > 0.1) {
          jet_time += jet_tower_time * jet_tower_e;
          jet_time_count += jet_tower_e;
        }
      }
	  }
    if (jet_time_count > 0) jet_time = jet_time / jet_time_count;
    else jet_time = -999;

    TLorentzVector jet_4mom;
    jet_4mom.SetPtEtaPhiE(_jet->get_pt(),_jet->get_eta(),_jet->get_phi(),_jet->get_e());
    new ((*m_jet_4mom06)[nJets06]) TLorentzVector(jet_4mom);
    m_jet_emfrac06[nJets06] = emcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_ihfrac06[nJets06] = ihcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_ohfrac06[nJets06] = ohcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_time06[nJets06] = jet_time;
    nJets06++;
  }
  nJets08 = 0;
  for (auto _jet : *_jets08) {
    if (_jet->get_e() < jet_emin_cut) continue;

    float emcal_calo_e = 0;
    float ihcal_calo_e = 0;
    float ohcal_calo_e = 0;
    float jet_time = 0;
    float jet_time_count = 0;
    for (auto comp: _jet->get_comp_vec()) {
	    unsigned int channel = comp.second;
      // emcal bits
      if (comp.first == 14 || comp.first == 29 || comp.first == 25 || comp.first == 28) {
        TowerInfo *_tower = _emcal_towers->get_tower_at_channel(channel);
	      float jet_tower_e = _tower->get_energy();
        float jet_tower_time = _tower->get_time();
        emcal_calo_e += jet_tower_e;
        if (jet_tower_e > 0.1) {
          jet_time += jet_tower_time * jet_tower_e;
          jet_time_count += jet_tower_e;
        }
	    } 
      // ihcal  bits
      else if (comp.first == 15 ||  comp.first == 30 || comp.first == 26) {
        TowerInfo *_tower = _ihcal_towers->get_tower_at_channel(channel);
	      float jet_tower_e = _tower->get_energy();
        float jet_tower_time = _tower->get_time();
        ihcal_calo_e += jet_tower_e;
        if (jet_tower_e > 0.1) {
          jet_time += jet_tower_time * jet_tower_e;
          jet_time_count += jet_tower_e;
        }
	    } 
      // ohcal bits
      else if (comp.first == 16 || comp.first == 31 || comp.first == 27) {
        TowerInfo *_tower = _ohcal_towers->get_tower_at_channel(channel);
	      float jet_tower_e = _tower->get_energy();
        float jet_tower_time = _tower->get_time();
        ohcal_calo_e += jet_tower_e;
        if (jet_tower_e > 0.1) {
          jet_time += jet_tower_time * jet_tower_e;
          jet_time_count += jet_tower_e;
        }
      }
	  }
    if (jet_time_count > 0) jet_time = jet_time / jet_time_count;
    else jet_time = -999;

    TLorentzVector jet_4mom;
    jet_4mom.SetPtEtaPhiE(_jet->get_pt(),_jet->get_eta(),_jet->get_phi(),_jet->get_e());
    new ((*m_jet_4mom08)[nJets08]) TLorentzVector(jet_4mom);
    m_jet_emfrac08[nJets08] = emcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_ihfrac08[nJets08] = ihcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_ohfrac08[nJets08] = ohcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_time08[nJets08] = jet_time;
    nJets08++;
  }
  
  towerntuple->Fill();

  m_mbd_nhits_south = 0;
  m_mbd_nhits_north = 0;
  m_mbd_time_south = 0;
  m_mbd_time_north = 0;
  m_cluster_4mom_sp->Clear();
  m_cluster_4mom_ns->Clear();
  m_jet_4mom02->Clear();
  m_jet_4mom04->Clear();
  m_jet_4mom06->Clear();
  m_jet_4mom08->Clear();
  istriggeredsp = false;
  istriggeredns = false;


  return 0;
}

int CaloAna::ProcessGlobalEventInfo(PHCompositeNode* topNode){

  vz=-999;
  vx=-999;
  vy=-999;
  GlobalVertexMap *globalvtxmap = findNode::getClass<GlobalVertexMap>(topNode,"GlobalVertexMap");
  if(!globalvtxmap) return Fun4AllReturnCodes::ABORTEVENT;
  if(globalvtxmap->empty()){ 
    return Fun4AllReturnCodes::ABORTEVENT;
  }
  GlobalVertex *bvertex= nullptr;
  for (GlobalVertexMap::ConstIter globaliter= globalvtxmap->begin(); globaliter != globalvtxmap->end(); ++globaliter)
  {
    bvertex = globaliter->second;
  }
  if(!bvertex) {
    std::cout << "could not find globalvtxmap iter :: set vtx to (-999,-999,-999)" << std::endl;
  }
  else {
    vz = bvertex->get_z();
    vy = bvertex->get_y();
    vx = bvertex->get_x();
    count_mbdvtx++;
  }

  Gl1Packet *gl1_packet = findNode::getClass<Gl1Packet>(topNode, "14001");
  if (gl1_packet) {
    uint64_t gl1_scaledtriggervector = gl1_packet->lValue(0, "ScaledVector");
    uint64_t gl1_livetriggervector = gl1_packet->lValue(0, "TriggerVector");
    for (int i = 0; i < 64; i++)
    {
      ScaledTriggerBit[i] = ((gl1_scaledtriggervector >> i) & 0x1U);
      LiveTriggerBit[i] = ((gl1_livetriggervector >> i) & 0x1U);
    }
  }

  return 0;
}

Double_t CaloAna::GetShiftedEta(float _vz, float _eta) {
  double radius = 93;
  double theta = 2*atan(exp(-_eta));
  double z = radius / tan(theta);
  double zshifted = z - _vz;
  double thetashifted = atan2(radius,zshifted);
  double etashifted = -log(tan(thetashifted/2.0));
  return etashifted;
}

int CaloAna::End(PHCompositeNode* /*topNode*/) {
  std::cout << "end..! " << std::endl;
  std::cout << "Events with mbd vertex " << count_mbdvtx << " out of " << count << std::endl;
  if(outfile){
    outfile->cd();
    towerntuple->Write();
    outfile->Write();
    outfile->Close();
    delete outfile;
    outfile=nullptr;
  }
  return 0;
}
