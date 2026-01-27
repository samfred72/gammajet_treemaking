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
  if (isMC) towerntuple->Branch("truth_vz",&truth_vz);
  towerntuple->Branch("ScaledTriggerBit",ScaledTriggerBit,"ScaledTriggerBit[64]/O");
  towerntuple->Branch("LiveTriggerBit",LiveTriggerBit,"LiveTriggerBit[64]/O");
  towerntuple->Branch("Scaledowns",m_scaledowns,"Scaledowns[64]/I");
  // MBD
  towerntuple->Branch("mbd_nhits_south",&m_mbd_nhits_south);
  towerntuple->Branch("mbd_nhits_north",&m_mbd_nhits_north);
  towerntuple->Branch("mbd_time_south",&m_mbd_time_south);
  towerntuple->Branch("mbd_time_north",&m_mbd_time_north);
  // Clusters
  towerntuple->Branch("nClusters_sp",&nClusters_sp, "nClusters_sp/S");
  towerntuple->Branch("cluster_4mom_sp","TClonesArray",&m_cluster_4mom_sp,32000,0);
  towerntuple->Branch("cluster_showershape_sp",m_cluster_showershape_sp,"cluster_showershape_sp[nClusters_sp][14]/F");
  towerntuple->Branch("cluster_bdt_scores",m_cluster_bdt_scores,"cluster_bdt_scores[nClusters_sp][11]/F");
  towerntuple->Branch("cluster_time_sp",m_cluster_time_sp,"cluster_time_sp[nClusters_sp]/F");
  towerntuple->Branch("cluster_iso_e02_sp",m_cluster_iso_e02_sp,"cluster_iso_e02_sp[nClusters_sp]/F");
  towerntuple->Branch("cluster_iso_e04_sp",m_cluster_iso_e04_sp,"cluster_iso_e04_sp[nClusters_sp]/F");
  //towerntuple->Branch("nClusters_ns",&nClusters_ns, "nClusters_ns/S");
  //towerntuple->Branch("cluster_4mom_ns","TClonesArray",&m_cluster_4mom_ns,32000,0);
  //towerntuple->Branch("cluster_showershape_ns",m_cluster_showershape_ns,"cluster_showershape_ns[nClusters_ns][14]/F");
  //towerntuple->Branch("cluster_time_ns",m_cluster_time_ns,"cluster_time_ns[nClusters_ns]/F");
  //towerntuple->Branch("cluster_iso_e02_ns",m_cluster_iso_e02_ns,"cluster_iso_e02_ns[nClusters_ns]/F");
  //towerntuple->Branch("cluster_iso_e04_ns",m_cluster_iso_e04_ns,"cluster_iso_e04_ns[nClusters_ns]/F");
  //towerntuple->Branch("cluster_iso_e06_ns",m_cluster_iso_e06_ns,"cluster_iso_e06_ns[nClusters_ns]/F");
  //towerntuple->Branch("cluster_iso_e08_ns",m_cluster_iso_e08_ns,"cluster_iso_e08_ns[nClusters_ns]/F");
  if (isMC) { 
    towerntuple->Branch("nTruthClusters",&nTruthClusters, "nTruthClusters/S");
    towerntuple->Branch("truth_cluster_4mom","TClonesArray",&m_truth_cluster_4mom,32000,0);
  }
  // jets
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
  if (isMC) {
    towerntuple->Branch("nTruthJets02",&nTruthJets02, "nTruthJets02/S");
    towerntuple->Branch("truth_jet_4mom02","TClonesArray",&m_truth_jet_4mom02,32000,0);
    towerntuple->Branch("nTruthJets04",&nTruthJets04, "nTruthJets04/S");
    towerntuple->Branch("truth_jet_4mom04","TClonesArray",&m_truth_jet_4mom04,32000,0);
    towerntuple->Branch("nTruthJets06",&nTruthJets06, "nTruthJets06/S");
    towerntuple->Branch("truth_jet_4mom06","TClonesArray",&m_truth_jet_4mom06,32000,0);
    towerntuple->Branch("nTruthJets08",&nTruthJets08, "nTruthJets08/S");
    towerntuple->Branch("truth_jet_4mom08","TClonesArray",&m_truth_jet_4mom08,32000,0);
  }
}

int CaloAna::process_event(PHCompositeNode* topNode)
{
  if(!topNode){
    std::cerr << "CaloAna::Init - topnode PHCompositeNode not valid! return -1" << std::endl;
    return -1;
  }
  if(count % 1000 == 0) std::cout << "event : " << count  << std::endl;
  if(isMC && count % 1000 != 0 && count % 10 == 0) std::cout << "event : " << count  << std::endl;
  count++;
  if (process_towers(topNode)) {
    return Fun4AllReturnCodes::ABORTEVENT;
  }
  return Fun4AllReturnCodes::EVENT_OK;
}

int CaloAna::process_towers(PHCompositeNode* topNode)
{
  if (isMC && ProcessTruth(topNode)) {
    return 1;
  }
  if (ProcessGlobalEventInfo(topNode)) {
    return 1;
  }

  // MBD energy and hits
  MbdOut * mbdout = (findNode::getClass<MbdOut>(topNode, "MbdOut"));
  if (mbdout) {
    m_mbd_nhits_south = mbdout->get_npmt(0);
    m_mbd_nhits_north = mbdout->get_npmt(1);
    m_mbd_time_south = mbdout->get_time(0);
    m_mbd_time_north = mbdout->get_time(1);
  }

  // Clusters
  //TowerInfoContainer* offlinetowers = (findNode::getClass<TowerInfoContainer>(topNode, "TOWERINFO_CALIB_CEMC"));
  //if (!offlinetowers) {
  //  std::cout << "No offline towers node!" << std::endl;
  //  return 1;
  //}

  // split cluster loop
  RawClusterContainer * photons0 = findNode::getClass<RawClusterContainer>(topNode, "PHOTONCLUSTER_CEMC0");
  
  nClusters_sp = 0;
  for (const auto& pair : photons0->getClustersMap()) 
  {
    PhotonClusterv1 * photon = dynamic_cast<PhotonClusterv1*>(pair.second);
    CLHEP::Hep3Vector vertex;
    vertex.set(vx,vy,vz);
    CLHEP::Hep3Vector E_vec_cluster = RawClusterUtility::GetEVec(*photon, vertex);
    
    float clPt = E_vec_cluster.perp();
    float clEta = E_vec_cluster.pseudoRapidity();
    float clPhi = E_vec_cluster.phi();
    float clE = E_vec_cluster.mag();
    if (clPt < cluster_emin) { continue; }
    
    TLorentzVector vphoton1;
    vphoton1.SetPtEtaPhiE(clPt,clEta,clPhi,clE);
    
    new ((*m_cluster_4mom_sp)[nClusters_sp]) TLorentzVector(vphoton1);
   
    m_cluster_showershape_sp[nClusters_sp][0] = photon->get_shower_shape_parameter("et1");
    m_cluster_showershape_sp[nClusters_sp][1] = photon->get_shower_shape_parameter("cluster_eta");
    m_cluster_showershape_sp[nClusters_sp][2] = photon->get_shower_shape_parameter("cluster_phi");
    m_cluster_showershape_sp[nClusters_sp][3] = photon->get_shower_shape_parameter("weta_cogx");
    m_cluster_showershape_sp[nClusters_sp][4] = photon->get_shower_shape_parameter("wphi_cogx");
    m_cluster_showershape_sp[nClusters_sp][5] = photon->get_shower_shape_parameter("e11_over_e33");
    m_cluster_showershape_sp[nClusters_sp][6] = photon->get_shower_shape_parameter("e32_over_e35");
    m_cluster_showershape_sp[nClusters_sp][7] = photon->get_shower_shape_parameter("drad");
    m_cluster_showershape_sp[nClusters_sp][8] = photon->get_shower_shape_parameter("iso_03_emcal");
    m_cluster_showershape_sp[nClusters_sp][9] = photon->get_shower_shape_parameter("iso_03_hcalin");
    m_cluster_showershape_sp[nClusters_sp][10] = photon->get_shower_shape_parameter("iso_03_hcalout");
    m_cluster_showershape_sp[nClusters_sp][11] = photon->get_shower_shape_parameter("iso_04_emcal");
    m_cluster_showershape_sp[nClusters_sp][12] = photon->get_shower_shape_parameter("iso_04_hcalin");
    m_cluster_showershape_sp[nClusters_sp][13] = photon->get_shower_shape_parameter("iso_04_hcalout");
    m_cluster_time_sp[nClusters_sp] = photon->get_shower_shape_parameter("mean_time");
    
    m_cluster_iso_e02_sp[nClusters_sp] = photon->get_et_iso(2,0,1);
    m_cluster_iso_e04_sp[nClusters_sp] = photon->get_et_iso(4,0,1);
     
    nClusters_sp++;
  }
    
  RawClusterContainer * photons1 = findNode::getClass<RawClusterContainer>(topNode, "PHOTONCLUSTER_CEMC1");
  RawClusterContainer * photons2 = findNode::getClass<RawClusterContainer>(topNode, "PHOTONCLUSTER_CEMC2");
  RawClusterContainer * photons3 = findNode::getClass<RawClusterContainer>(topNode, "PHOTONCLUSTER_CEMC3");
  RawClusterContainer * photons4 = findNode::getClass<RawClusterContainer>(topNode, "PHOTONCLUSTER_CEMC4");
  RawClusterContainer * photons5 = findNode::getClass<RawClusterContainer>(topNode, "PHOTONCLUSTER_CEMC5");
  RawClusterContainer * photons6 = findNode::getClass<RawClusterContainer>(topNode, "PHOTONCLUSTER_CEMC6");
  RawClusterContainer * photons7 = findNode::getClass<RawClusterContainer>(topNode, "PHOTONCLUSTER_CEMC7");
  RawClusterContainer * photons8 = findNode::getClass<RawClusterContainer>(topNode, "PHOTONCLUSTER_CEMC8");
  RawClusterContainer * photons9 = findNode::getClass<RawClusterContainer>(topNode, "PHOTONCLUSTER_CEMC9");
  RawClusterContainer * photons10 = findNode::getClass<RawClusterContainer>(topNode, "PHOTONCLUSTER_CEMC10");
 
  RawClusterContainer * photons[11] = {photons0,photons1,photons2,photons3,photons4,photons5,photons6,photons7,photons8,photons9,photons10};
  for (int i = 0; i < 11; i++) {  
    int nClusters_all = 0;
    for (const auto& pair : photons[i]->getClustersMap()) 
    { 
      PhotonClusterv1 * photon = dynamic_cast<PhotonClusterv1*>(pair.second);
      m_cluster_bdt_scores[nClusters_all][i] = photon->get_shower_shape_parameter("bdt_score");
      nClusters_all++;
    }
  }
  
  
 /* 
  //mother cluster loop
  RawClusterContainer *clustersEM_ns = (findNode::getClass<RawClusterContainer>(topNode, "CEMC_CLUSTERINFO_MOTHER"));
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
    if (clE < cluster_emin) { continue; }

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
  */
  
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
    float pt = _jet->get_pt();
    float e = _jet->get_e();
    if (isMC && smear > 0) { pt = rand.Gaus(pt,pt*smear); e = pt * TMath::CosH(_jet->get_eta()); }
    if (pt < jet_emin_cut) continue;

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
    jet_4mom.SetPtEtaPhiE(pt,_jet->get_eta(),_jet->get_phi(),e);
    new ((*m_jet_4mom02)[nJets02]) TLorentzVector(jet_4mom);
    m_jet_emfrac02[nJets02] = emcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_ihfrac02[nJets02] = ihcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_ohfrac02[nJets02] = ohcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_time02[nJets02] = jet_time;
    nJets02++;
  }
  nJets04 = 0;
  for (auto _jet : *_jets04) {
    float pt = _jet->get_pt();
    float e = _jet->get_e();
    if (isMC) { pt = rand.Gaus(pt,pt*smear); e = pt * TMath::CosH(_jet->get_eta()); }
    if (pt < jet_emin_cut) continue;

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
    jet_4mom.SetPtEtaPhiE(pt,_jet->get_eta(),_jet->get_phi(),e);
    new ((*m_jet_4mom04)[nJets04]) TLorentzVector(jet_4mom);
    m_jet_emfrac04[nJets04] = emcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_ihfrac04[nJets04] = ihcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_ohfrac04[nJets04] = ohcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_time04[nJets04] = jet_time;
    nJets04++;
  }
  nJets06 = 0;
  for (auto _jet : *_jets06) {
    float pt = _jet->get_pt();
    float e = _jet->get_e();
    if (isMC) { pt = rand.Gaus(pt,pt*smear); e = pt * TMath::CosH(_jet->get_eta()); }
    if (pt < jet_emin_cut) continue;

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
    jet_4mom.SetPtEtaPhiE(pt,_jet->get_eta(),_jet->get_phi(),e);
    new ((*m_jet_4mom06)[nJets06]) TLorentzVector(jet_4mom);
    m_jet_emfrac06[nJets06] = emcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_ihfrac06[nJets06] = ihcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_ohfrac06[nJets06] = ohcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_time06[nJets06] = jet_time;
    nJets06++;
  }
  nJets08 = 0;
  for (auto _jet : *_jets08) {
    float pt = _jet->get_pt();
    float e = _jet->get_e();
    if (isMC) { pt = rand.Gaus(pt,pt*smear); e = pt * TMath::CosH(_jet->get_eta()); }
    if (pt < jet_emin_cut) continue;

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
    jet_4mom.SetPtEtaPhiE(pt,_jet->get_eta(),_jet->get_phi(),e);
    new ((*m_jet_4mom08)[nJets08]) TLorentzVector(jet_4mom);
    m_jet_emfrac08[nJets08] = emcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_ihfrac08[nJets08] = ihcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_ohfrac08[nJets08] = ohcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e);
    m_jet_time08[nJets08] = jet_time;
    nJets08++;
  }

  if (nClusters_sp == 0 || (nJets02 == 0 && nJets04 == 0 && nJets06 == 0 && nJets08 == 0)) return 1;
  
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


  return 0;
}

int CaloAna::ProcessGlobalEventInfo(PHCompositeNode* topNode){

  vz=-999;
  vx=-999;
  vy=-999;
  if (isMC) {
    MbdVertexMap *mbdvtxmap = findNode::getClass<MbdVertexMap>(topNode,"MbdVertexMap");
    bool isglbvtx=true;
    if(!mbdvtxmap || mbdvtxmap->empty()) { 
      isglbvtx = false;
    }
    if (isglbvtx) {
      MbdVertex *bvertex = nullptr;
      for (MbdVertexMap::ConstIter mbditer= mbdvtxmap->begin(); mbditer != mbdvtxmap->end(); ++mbditer)
      {
        bvertex = mbditer->second;
      }
      if(!bvertex) { std::cout << "could not find globalvtxmap iter :: set vtx to (-999,-999,-999)" << std::endl;}
      else if (bvertex) {
        vz = bvertex->get_z();
        vy = bvertex->get_y();
        vx = bvertex->get_x();
        count_mbdvtx++;
      }
    }
  }
  else {
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

int CaloAna::ProcessTruth(PHCompositeNode* topNode) {
  PHG4TruthInfoContainer *truthinfo = findNode::getClass<PHG4TruthInfoContainer>(topNode, "G4TruthInfo");
  if(!truthinfo) {std::cout << "no truth info node... just skip the whole part.." << std::endl; return 1;}
  if (truthinfo)
  {
    PHG4VtxPoint *gvertex = truthinfo->GetPrimaryVtx(truthinfo->GetPrimaryVertexIndex());
    truth_vz = gvertex->get_z();
    truth_vx = gvertex->get_x();
    truth_vy = gvertex->get_y();
  }

  PHG4TruthInfoContainer::Range range = truthinfo->GetPrimaryParticleRange();

  m_truth_cluster_4mom->Clear();

  nTruthClusters = 0;
  for (PHG4TruthInfoContainer::ConstIterator iter = range.first; iter != range.second; ++iter)
  {
    PHG4Particle* g4particle = iter->second;
    if (!truthinfo->is_primary(g4particle)) continue;
    if (g4particle->get_parent_id() != 0) continue;
    if (g4particle->get_pid() != 22) continue;

    TLorentzVector t;
    t.SetPxPyPzE(g4particle->get_px (), g4particle->get_py (), g4particle->get_pz (), g4particle->get_e ());
    new ((*m_truth_cluster_4mom)[nTruthClusters]) TLorentzVector(t);

    nTruthClusters++;
  }

  JetContainer * truthjets02 = findNode::getClass<JetContainer>(topNode, "AntiKt_Truth_r02");
  nTruthJets02 = 0;
  if(truthjets02)
  {
    for(unsigned i = 0; i < truthjets02->size(); ++i)
    {
      Jet* jet = truthjets02->get_jet(i);
      if (jet->get_pt() < jet_emin_cut) continue; 
      float pt = jet->get_pt();
      float e = jet->get_e();
      float eta = jet->get_eta();
      float phi = jet->get_phi();
      TLorentzVector t;
      t.SetPtEtaPhiE(pt,eta,phi,e);
      new ((*m_truth_jet_4mom02)[nTruthJets02]) TLorentzVector(t);
      nTruthJets02++;
    }
  }
  JetContainer * truthjets04 = findNode::getClass<JetContainer>(topNode, "AntiKt_Truth_r04");
  nTruthJets04 = 0;
  if(truthjets04)
  {
    for(unsigned i=0; i < truthjets04->size(); ++i)
    {
      Jet* jet = truthjets04->get_jet(i);
      if (jet->get_pt() < jet_emin_cut) continue; 
      float pt = jet->get_pt();
      float e = jet->get_e();
      float eta = jet->get_eta();
      float phi = jet->get_phi();
      TLorentzVector t;
      t.SetPtEtaPhiE(pt,eta,phi,e);
      new ((*m_truth_jet_4mom04)[nTruthJets04]) TLorentzVector(t);
      nTruthJets04++;
    }
  }
  JetContainer * truthjets06 = findNode::getClass<JetContainer>(topNode, "AntiKt_Truth_r06");
  nTruthJets06 = 0;
  if(truthjets06)
  {
    for(unsigned i=0; i < truthjets06->size(); ++i)
    {
      Jet* jet = truthjets06->get_jet(i);
      if (jet->get_pt() < jet_emin_cut) continue; 
      float pt = jet->get_pt();
      float e = jet->get_e();
      float eta = jet->get_eta();
      float phi = jet->get_phi();
      TLorentzVector t;
      t.SetPtEtaPhiE(pt,eta,phi,e);
      new ((*m_truth_jet_4mom06)[nTruthJets06]) TLorentzVector(t);
      nTruthJets06++;
    }
  }
  JetContainer * truthjets08 = findNode::getClass<JetContainer>(topNode, "AntiKt_Truth_r08");
  nTruthJets08 = 0;
  if(truthjets08)
  {
    for(unsigned i=0; i < truthjets08->size(); ++i)
    {
      Jet* jet = truthjets08->get_jet(i);
      if (jet->get_pt() < jet_emin_cut) continue; 
      float pt = jet->get_pt();
      float e = jet->get_e();
      float eta = jet->get_eta();
      float phi = jet->get_phi();
      TLorentzVector t;
      t.SetPtEtaPhiE(pt,eta,phi,e);
      new ((*m_truth_jet_4mom08)[nTruthJets08]) TLorentzVector(t);
      nTruthJets08++;
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
Double_t CaloAna::DeltaR(TLorentzVector pho1, TLorentzVector pho2) {
  double phi1 = pho1.Phi();
  double phi2 = pho2.Phi();
  double eta1 = pho1.Eta();
  double eta2 = pho2.Eta();
  double dr = TMath::Sqrt((phi1-phi2)*(phi1-phi2) - (eta1-eta2)*(eta1-eta2));
  return dr;
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
