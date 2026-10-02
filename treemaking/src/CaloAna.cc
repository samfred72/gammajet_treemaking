#include <iostream>
#include "CaloAna.h"
using namespace std;

CaloAna::CaloAna(const std::string& name, const char* outname)
  : SubsysReco(name)
    , detector("HCALIN")
{
  outfilename = Form("%s",outname);
  gRandom->SetSeed(18); // for PPG18
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
    ifstream file = ifstream("/sphenix/user/samfred/projects/mbdt0/histmaking/MbdPmt.corr");
    string line;
    cout << "Getting MBD t0 corrections..." << endl;
    while (getline(file,line)) {
      int irunnum;
      float t0;
      istringstream iss(line);
      iss >> irunnum >> t0;
      if (irunnum == m_run_number) {
        mbd_t0corr = t0;
        break;
      }
    }
    // JER smearing templates (fractional sigma vs pt). Nominal/sysup/sysdown are each a full width curve.
    TFile fjer("/sphenix/user/samfred/projects/gammajet/treemaking/macros/jerband_smearing_templates.root");
    h_jer_smear_nominal = dynamic_cast<TH1D*>(fjer.Get("h_jer_smear_r04_pileup_EMfracJES_nominal"));
    h_jer_smear_up      = dynamic_cast<TH1D*>(fjer.Get("h_jer_smear_r04_pileup_EMfracJES_sysup"));
    h_jer_smear_down    = dynamic_cast<TH1D*>(fjer.Get("h_jer_smear_r04_pileup_EMfracJES_sysdown"));
    if (!h_jer_smear_nominal || !h_jer_smear_up || !h_jer_smear_down)
      throw std::runtime_error("could not load JER smearing templates from jerband_smearing_templates.root");
    h_jer_smear_nominal->SetDirectory(0);
    h_jer_smear_up     ->SetDirectory(0);
    h_jer_smear_down   ->SetDirectory(0);

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
  if (!isMC) {
    towerntuple->Branch("ScaledTriggerBit",ScaledTriggerBit,"ScaledTriggerBit[64]/O");
    towerntuple->Branch("LiveTriggerBit",LiveTriggerBit,"LiveTriggerBit[64]/O");
    towerntuple->Branch("Scaledowns",m_scaledowns,"Scaledowns[64]/I");
  }
  // MBD
  towerntuple->Branch("mbd_time",&m_mbd_time);
  // Clusters
  towerntuple->Branch("cluster_pt" ,&m_cluster_pt                 );
  towerntuple->Branch("cluster_e"  ,&m_cluster_e                  );
  towerntuple->Branch("cluster_eta",&m_cluster_eta                );
  towerntuple->Branch("cluster_phi",&m_cluster_phi                );
  towerntuple->Branch("cluster_showershape",m_cluster_showershape,"cluster_showershape[12]/F");
  towerntuple->Branch("cluster_bdt_scores",m_cluster_bdt_scores,"cluster_bdt_scores[11]/F");
  towerntuple->Branch("cluster_time",&m_cluster_time              );
  towerntuple->Branch("cluster_Z",&m_cluster_Z              );
  // Old BDT model, same (saturated) CEMC towers as the nominal cluster above
  towerntuple->Branch("cluster_pt_old" ,&m_cluster_pt_old                     );
  towerntuple->Branch("cluster_e_old"  ,&m_cluster_e_old                      );
  towerntuple->Branch("cluster_eta_old",&m_cluster_eta_old                    );
  towerntuple->Branch("cluster_phi_old",&m_cluster_phi_old                    );
  towerntuple->Branch("cluster_time_old",&m_cluster_time_old                  );
  towerntuple->Branch("cluster_showershape_old",m_cluster_showershape_old,"cluster_showershape_old[12]/F");
  towerntuple->Branch("cluster_bdt_score_old",&m_cluster_bdt_score_old        );
  if (isMC) {
    // No-pixel-saturation CEMC branch; node doesn't exist for real data
    towerntuple->Branch("cluster_pt_nosat" ,&m_cluster_pt_nosat                 );
    towerntuple->Branch("cluster_e_nosat"  ,&m_cluster_e_nosat                  );
    towerntuple->Branch("cluster_eta_nosat",&m_cluster_eta_nosat                );
    towerntuple->Branch("cluster_phi_nosat",&m_cluster_phi_nosat                );
    towerntuple->Branch("cluster_time_nosat",&m_cluster_time_nosat              );
    towerntuple->Branch("cluster_showershape_nosat",m_cluster_showershape_nosat,"cluster_showershape_nosat[12]/F");
    towerntuple->Branch("cluster_bdt_score_nosat",&m_cluster_bdt_score_nosat    );

    towerntuple->Branch("truth_cluster_pt" ,&m_truth_cluster_pt );
    towerntuple->Branch("truth_cluster_e"  ,&m_truth_cluster_e  );
    towerntuple->Branch("truth_cluster_eta",&m_truth_cluster_eta);
    towerntuple->Branch("truth_cluster_phi",&m_truth_cluster_phi);
    towerntuple->Branch("truth_cluster_iso3",&m_truth_cluster_iso3);
    towerntuple->Branch("truth_cluster_iso4",&m_truth_cluster_iso4);
  }
  // jets
  towerntuple->Branch("hasthirdjet", hasthirdjet   , "hasthirdjet[7]/O");
  towerntuple->Branch("jet_pt",      m_jet_pt      , "jet_pt[7]/F");
  towerntuple->Branch("jet_pt_calib",m_jet_pt_calib, "jet_pt_calib[7]/F");
  towerntuple->Branch("jet_pt_recalib",m_jet_pt_recalib, "jet_pt_recalib[7]/F");
  towerntuple->Branch("jet_pt_old",&m_jet_pt_old);
  towerntuple->Branch("jet_e",       m_jet_e       , "jet_e[7]/F");
  towerntuple->Branch("jet_eta",     m_jet_eta     , "jet_eta[7]/F");
  towerntuple->Branch("jet_phi",     m_jet_phi     , "jet_phi[7]/F");
  towerntuple->Branch("jet_emfrac",  m_jet_emfrac  , "jet_emfrac[7]/F");
  //towerntuple->Branch("jet_ihfrac",  m_jet_ihfrac  , "jet_ihfrac[7]/F");
  //towerntuple->Branch("jet_ohfrac",  m_jet_ohfrac  , "jet_ohfrac[7]/F");
  towerntuple->Branch("jet_time",    m_jet_time    , "jet_time[7]/F");
  towerntuple->Branch("thirdjet_pt", m_3jet_pt     , "thirdjet_pt[7]/F");
  towerntuple->Branch("thirdjet_eta",m_3jet_eta    , "thirdjet_eta[7]/F");
  towerntuple->Branch("thirdjet_phi",m_3jet_phi    , "thirdjet_phi[7]/F");
  if (isMC) {
    towerntuple->Branch("jet_pt_smear_reco",m_jet_pt_smear_reco, "jet_pt_smear_reco[7]/F");
    towerntuple->Branch("jet_pt_smear_high_reco",m_jet_pt_smear_high_reco, "jet_pt_smear_high_reco[7]/F");
    towerntuple->Branch("jet_pt_smear_low_reco",m_jet_pt_smear_low_reco, "jet_pt_smear_low_reco[7]/F");
    towerntuple->Branch("jet_pt_smear_truth",m_jet_pt_smear_truth, "jet_pt_smear_truth[7]/F");
    towerntuple->Branch("jet_pt_smear_high_truth",m_jet_pt_smear_high_truth, "jet_pt_smear_high_truth[7]/F");
    towerntuple->Branch("jet_pt_smear_low_truth",m_jet_pt_smear_low_truth, "jet_pt_smear_low_truth[7]/F");
    
    towerntuple->Branch("truth_jet_pt" ,m_truth_jet_pt , "truth_jet_pt[7]/F");
    towerntuple->Branch("truth_jet_e"  ,m_truth_jet_e  , "truth_jet_e[7]/F");
    towerntuple->Branch("truth_jet_eta",m_truth_jet_eta, "truth_jet_eta[7]/F");
    towerntuple->Branch("truth_jet_phi",m_truth_jet_phi, "truth_jet_phi[7]/F");
    towerntuple->Branch("hadron_p"     ,m_hadron_p     , "hadron_p[7]/F");
    towerntuple->Branch("jet_con_dr"   ,m_jet_con_dr   , "jet_con_dr[7]/F");
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
    Clear();
    return Fun4AllReturnCodes::ABORTEVENT;
  }
  return Fun4AllReturnCodes::EVENT_OK;
}

// Finds the highest-pt cluster in `photons` (above cluster_pt_cut, and passing the
// mbd-timing cut for data) and fills the out_* references from it. Shared by the
// nominal, old-BDT, and no-saturation photon branches, which all differ only in
// which PHOTONCLUSTER_CEMC* node they read.
void CaloAna::ProcessPhotonCandidate(RawClusterContainer* photons, Float_t& out_pt, Float_t& out_e,
                                      Float_t& out_eta, Float_t& out_phi, Float_t& out_time,
                                      Float_t out_showershape[12], Float_t& out_bdt_score)
{
  float maxclusterpt = 0;
  CLHEP::Hep3Vector vertex;
  vertex.set(vx,vy,vz);
  for (const auto& pair : photons->getClustersMap())
  {
    PhotonClusterv1 * photon = dynamic_cast<PhotonClusterv1*>(pair.second);
    CLHEP::Hep3Vector E_vec_cluster = RawClusterUtility::GetEVec(*photon, vertex);

    float clPt = E_vec_cluster.perp();
    float clEta = E_vec_cluster.pseudoRapidity();
    float clPhi = E_vec_cluster.phi();
    float clE = E_vec_cluster.mag();
    float clTime = (photon->get_shower_shape_parameter("mean_time"))*17.6;
    if (!isMC && (m_mbd_time - clTime > 4 || m_mbd_time - clTime < 0)) continue;
    if (clPt > maxclusterpt && clPt > cluster_pt_cut) {
      maxclusterpt = clPt;

      out_pt   = (clPt);
      out_e    = (clE);
      out_phi  = (clPhi);
      out_eta  = (clEta);
      out_time = (clTime);

      out_showershape[0] = (photon->get_shower_shape_parameter("et1"));
      out_showershape[1] = (photon->get_shower_shape_parameter("cluster_eta"));
      out_showershape[2] = (photon->get_shower_shape_parameter("cluster_phi"));
      out_showershape[3] = (photon->get_shower_shape_parameter("weta_cogx"));
      out_showershape[4] = (photon->get_shower_shape_parameter("wphi_cogx"));
      out_showershape[5] = (photon->get_shower_shape_parameter("e11")/photon->get_shower_shape_parameter("e33"));
      out_showershape[6] = (photon->get_shower_shape_parameter("e32")/photon->get_shower_shape_parameter("e35"));
      out_showershape[7] = (photon->get_shower_shape_parameter("drad"));
      out_showershape[8] = (photon->get_shower_shape_parameter("iso_03_emcal")+photon->get_shower_shape_parameter("iso_03_hcalin")+photon->get_shower_shape_parameter("iso_03_hcalout"));
      out_showershape[9] = (photon->get_shower_shape_parameter("iso_04_emcal")+photon->get_shower_shape_parameter("iso_04_hcalin")+photon->get_shower_shape_parameter("iso_04_hcalout"));
      out_showershape[10] = (photon->get_shower_shape_parameter("iso_topo_03"));
      out_showershape[11] = (photon->get_shower_shape_parameter("iso_topo_04"));
      out_bdt_score = (photon->get_shower_shape_parameter("bdt_score"));
    }
  }
}

int CaloAna::process_towers(PHCompositeNode* topNode)
{
  if (isMC && ProcessTruth(topNode)) {
    Clear();
    return 1;
  }
  if (ProcessGlobalEventInfo(topNode)) {
    Clear();
    return 1;
  }

  // MBD energy and hits
  MbdOut * mbdout = (findNode::getClass<MbdOut>(topNode, "MbdOut"));
  if (mbdout) {
    float time_south = mbdout->get_time(0);
    float time_north = mbdout->get_time(1);
    m_mbd_time = (time_south + time_north) / 2.0 - mbd_t0corr;
  }


  // Nominal photon reco (current BDT model, saturated CEMC towers)
  RawClusterContainer * photons0 = findNode::getClass<RawClusterContainer>(topNode, "PHOTONCLUSTER_CEMC0");
  if (photons0) {
    ProcessPhotonCandidate(photons0, m_cluster_pt, m_cluster_e, m_cluster_eta, m_cluster_phi,
                            m_cluster_time, m_cluster_showershape, m_cluster_bdt_scores[9]);
  }

  // Old BDT model, same (saturated) CEMC towers as nominal -- see photon/oldphoton
  // in Fun4All_macro.C / MCFun4All_macro.C
  RawClusterContainer * photons_old = findNode::getClass<RawClusterContainer>(topNode, "PHOTONCLUSTER_CEMC_OLD0");
  if (photons_old) {
    ProcessPhotonCandidate(photons_old, m_cluster_pt_old, m_cluster_e_old, m_cluster_eta_old,
                            m_cluster_phi_old, m_cluster_time_old, m_cluster_showershape_old,
                            m_cluster_bdt_score_old);
  }

  // MC-only: no-pixel-saturation CEMC branch (see MCFun4All_macro.C). The node
  // doesn't exist for real data, so photons_nosat stays null and these fields
  // stay at their Clear()'d 0.
  if (isMC) {
    RawClusterContainer * photons_nosat = findNode::getClass<RawClusterContainer>(topNode, "PHOTONCLUSTER_CEMC_NOSAT0");
    if (photons_nosat) {
      ProcessPhotonCandidate(photons_nosat, m_cluster_pt_nosat, m_cluster_e_nosat, m_cluster_eta_nosat,
                              m_cluster_phi_nosat, m_cluster_time_nosat, m_cluster_showershape_nosat,
                              m_cluster_bdt_score_nosat);
    }
  }

  if (!isMC && m_cluster_pt == 0) {
    Clear();
    return 1;
  }
  
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
    Clear();
    return 1;
  }

  for (int ir = 0; ir < m_nRadii; ir++) {
    JetContainer * _jets = findNode::getClass<JetContainer>(topNode, m_jet_nodenames[ir]);
    JetContainer * _jets_calib = findNode::getClass<JetContainer>(topNode, m_jet_calib_nodenames[ir]);
    if (!_jets) {
      std::cout << "JetContainer is missing" << std::endl;
      Clear();
      return 1;
    }
    // Old JES calibration (r=0.4 only) -- shares the same input jets/indexing as
    // _jets_calib, so the leading jet's old-calib pt is read via the same index ij.
    JetContainer * _jets_calib_old = nullptr;
    if (ir == m_oldCalibRadiusIndex) {
      _jets_calib_old = findNode::getClass<JetContainer>(topNode, m_jet_calib_old_nodename);
    }
    // Fill the jet information.
    std::vector<std::vector<Jet*>> _both_jets;
    std::transform(_jets->begin(), _jets->end(), _jets_calib->begin(),
                   std::back_inserter(_both_jets),
                   [](Jet * j, Jet * jc) {
                      std::vector<Jet*> v = {j,jc}; 
                      return v;
                   });


    // Truth matching
    JetContainer * truthjets = findNode::getClass<JetContainer>(topNode, m_truth_jet_nodenames[ir]);
    // truth_pt_by_reco[i] is the matched truth jet pt for reco jet index i (same index as _both_jets),
    // or -1 if that reco jet has no truth match. smear_pt() indexes into this directly.
    std::vector<float> truth_pt_by_reco;

    if (isMC) {
      truth_pt_by_reco.assign(_jets_calib->size(), -1);
      struct Pair { float pt_r, pt_t, dr; bool isvalid = 1; int i_r, i_t; };
      std::vector<std::vector<Pair>> pairs(_jets_calib->size(), std::vector<Pair>(truthjets->size()));
      std::vector<Pair> pair_list;

      int ireco = 0;
      for (auto jet_r : *_jets_calib) {
        int itruth = 0;
        if (jet_r->get_pt() < jet_calib_pt_cut) {ireco++; continue; }
        for (auto jet_t : *truthjets) {
          Pair temp;
          temp.pt_r = jet_r->get_pt();
          temp.pt_t = jet_t->get_pt();
          temp.dr = DeltaR(jet_r->get_eta(), jet_r->get_phi(), jet_t->get_eta(), jet_t->get_phi());
          temp.i_r = ireco;
          temp.i_t = itruth;
          temp.isvalid = 1;
          pairs[ireco][itruth] = temp;
          pair_list.push_back(temp);
          //std::cout << std::fixed << std::setprecision(2) << pairs[ireco][itruth].dr << "," << pairs[ireco][itruth].pt_r << "," << pairs[ireco][itruth].pt_t << " ";
          itruth++;
        }
        //std::cout << std::endl;
        ireco++;
      }
      //std::cout << endl;
      std::sort(pair_list.begin(), pair_list.end(),
          [](const Pair& a, const Pair& b) {
          return a.dr < b.dr;
          });

      for (int ip = 0; ip < pair_list.size(); ip++) {
        Pair p = pair_list.at(ip);
        float dr = p.dr;
        if (dr >= m_radii[ir] * 3.0/4.0) break;

        if (!pairs[p.i_r][p.i_t].isvalid) continue;

        truth_pt_by_reco[p.i_r] = p.pt_t;
        for (int ireco = 0; ireco < _jets_calib->size(); ireco++) {
          pairs[ireco][p.i_t].isvalid = 0;
        }
        for (int itruth = 0; itruth < truthjets->size(); itruth++) {
          pairs[p.i_r][itruth].isvalid = 0;
        }
      }
    }


    // for third jet systematics: every jet that passes the pT prefilter, is outside the
    // photon's cone, and (data) is in time. Only the leading one is stored as the recoil
    // jet, and the highest of the rest as the third jet, so unfolder.cc can vary the veto
    // threshold without storing a jet vector.
    struct ThirdJetCand { unsigned ij; float pt, eta, phi; };
    std::vector<ThirdJetCand> thirdjet_cands;
    int ileadjet = -1;
    float maxjetpt = 0;
    int npassingjets[m_nRadii] = { 0 };

    for (unsigned ij = 0; ij < _both_jets.size(); ij++) {
      Jet * _jet = _both_jets.at(ij).at(0);
      Jet * _jet_calib = _both_jets.at(ij).at(1);
      float e = _jet->get_e();
      float pt = _jet->get_pt();
      float pt_calib = _jet_calib->get_pt();
      float pt_recalib = pt_calib/0.90;
      float eta = _jet->get_eta();
      float phi = _jet->get_phi();
      float pt_smear_reco      = rand.Gaus(pt_calib, pt_calib*h_jer_smear_nominal->Interpolate(pt_calib));
      float pt_smear_high_reco = rand.Gaus(pt_calib, pt_calib*h_jer_smear_up     ->Interpolate(pt_calib));
      float pt_smear_low_reco  = rand.Gaus(pt_calib, pt_calib*h_jer_smear_down   ->Interpolate(pt_calib));

      float pt_smear_truth      = smear_pt(ij, pt_calib, truth_pt_by_reco,  0);
      float pt_smear_high_truth = smear_pt(ij, pt_calib, truth_pt_by_reco, +1);
      float pt_smear_low_truth  = smear_pt(ij, pt_calib, truth_pt_by_reco, -1);

      float dr = DeltaR(eta, phi, m_cluster_eta, m_cluster_phi);
      if (dr < 0.4) m_cluster_Z += m_cluster_pt / pt_calib;
      if (pt < jet_pt_cut && // Uncalibrated jets
          pt_calib < jet_calib_pt_cut &&  // JES calibrated
          pt_recalib < jet_calib_pt_cut &&  // JES calibrated + fake insitu
          (!isMC || (isMC &&
          pt_smear_reco       < jet_calib_pt_cut && // JES and JER on reco
          pt_smear_high_reco  < jet_calib_pt_cut && // JES and JER on reco
          pt_smear_low_reco   < jet_calib_pt_cut && // JES and JER on reco
          pt_smear_truth      < jet_calib_pt_cut && // JES and JER on truth
          pt_smear_high_truth < jet_calib_pt_cut && // JES and JER on truth+uncertainty
          pt_smear_low_truth  < jet_calib_pt_cut))) continue; // JES and JER on truth-uncertainty
      if (dr < m_radii[ir]) continue;

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
      if (jet_time_count > 0) jet_time = (jet_time / jet_time_count)*17.6;
      else jet_time = -999;
      // Jet timing window (data only): -2 < t_MBD - t_jet < 5 ns, widened from 0-4 ns (Oct 2026).
      // The 0-4 ns window removed in-time jets with a pT- and EM-fraction-dependent efficiency
      // (EMCal/HCal time offsets, oHCal energy dependence); the photon cluster keeps 0-4 ns.
      if (!isMC && (m_mbd_time - jet_time > 5 || m_mbd_time - jet_time < -2)) continue;
      // Counted only after the photon-cone and timing cuts, so out-of-time jets no longer
      // veto data events (they never could in MC, which has no timing cut).
      npassingjets[ir]++;
      thirdjet_cands.push_back({ij, isMC ? pt_smear_truth : pt_calib, eta, phi});
      if (pt < maxjetpt) continue;
      ileadjet = ij;
      maxjetpt = pt;


      m_jet_pt_calib     [ir] = (pt_calib);
      m_jet_pt_recalib   [ir] = (pt_recalib);
      if (ir == m_oldCalibRadiusIndex && _jets_calib_old && ij < _jets_calib_old->size()) {
        Jet * _jet_calib_old = _jets_calib_old->get_jet(ij);
        if (_jet_calib_old) m_jet_pt_old = rand.Gaus(_jet_calib_old->get_pt(), _jet_calib_old->get_pt()*(isMC ? 0.1 : 0.0));
      }
      m_jet_pt_smear_reco      [ir] = (pt_smear_reco);
      m_jet_pt_smear_high_reco [ir] = (pt_smear_high_reco);
      m_jet_pt_smear_low_reco  [ir] = (pt_smear_low_reco);
      m_jet_pt_smear_truth     [ir] = (pt_smear_truth);
      m_jet_pt_smear_high_truth[ir] = (pt_smear_high_truth);
      m_jet_pt_smear_low_truth [ir] = (pt_smear_low_truth);
      m_jet_pt           [ir] = (pt);
      m_jet_e            [ir] = (e);
      m_jet_eta          [ir] = (_jet->get_eta());
      m_jet_phi          [ir] = (_jet->get_phi());
      m_jet_emfrac       [ir] = (emcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e));
      //m_jet_ihfrac  [ir] = (ihcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e));
      //m_jet_ohfrac  [ir] = (ohcal_calo_e/(emcal_calo_e + ihcal_calo_e + ohcal_calo_e));
      m_jet_time    [ir] = (jet_time);
    }
    // The photon's own jet is no longer counted, hence > 1 rather than the old > 2.
    hasthirdjet[ir] = (npassingjets[ir] > 1);
    // Highest-pT candidate other than the leading jet. The old version kept the previous
    // leader whenever a new leader was found, which misses a true second jet that comes
    // after the leader in the container (e.g. jets 10, 20, 15 stored 10, not 15).
    const ThirdJetCand * third = nullptr;
    for (const auto & c : thirdjet_cands) {
      if ((int) c.ij == ileadjet) continue;
      if (!third || c.pt > third->pt) third = &c;
    }
    if (third) {
      m_3jet_pt[ir] = third->pt;
      m_3jet_eta[ir] = third->eta;
      m_3jet_phi[ir] = third->phi;
    }
  }
  
  float maxjetpt = max({m_jet_pt_smear_reco[0], m_jet_pt_smear_reco[1],m_jet_pt_smear_reco[2],m_jet_pt_smear_reco[3],
                        m_jet_pt_smear_reco[4], m_jet_pt_smear_reco[5],m_jet_pt_smear_reco[6]});
  
  
  bool has_reco = (m_cluster_pt > 0);
  bool has_truth = isMC && m_truth_cluster_pt > 0;
  if (!has_reco && !has_truth) { Clear(); return 1; }
  
  towerntuple->Fill();
  Clear();

  return 0;

} 


void CaloAna::Clear() {
  m_mbd_time = 0;
 
  m_cluster_pt   = 0;
  m_cluster_e    = 0;
  m_cluster_eta  = 0;
  m_cluster_phi  = 0;
  m_cluster_time = 0;
  for (int i = 0; i < 11; i++) {
    m_cluster_bdt_scores[i] = 0;
  }
  for (int i = 0; i < 14; i++) {
    m_cluster_showershape[i] = 0;
  }

  m_cluster_pt_old   = 0;
  m_cluster_e_old    = 0;
  m_cluster_eta_old  = 0;
  m_cluster_phi_old  = 0;
  m_cluster_time_old = 0;
  m_cluster_bdt_score_old = 0;
  for (int i = 0; i < 12; i++) {
    m_cluster_showershape_old[i] = 0;
  }

  m_jet_pt_old = 0;
  for (int ir = 0; ir < m_nRadii; ir++) {
    m_jet_pt           [ir] = 0;
    m_jet_pt_calib     [ir] = 0;
    m_jet_pt_recalib   [ir] = 0;
    m_jet_pt_smear_reco      [ir] = 0;
    m_jet_pt_smear_high_reco [ir] = 0;
    m_jet_pt_smear_low_reco  [ir] = 0;
    m_jet_pt_smear_truth     [ir] = 0;
    m_jet_pt_smear_high_truth[ir] = 0;
    m_jet_pt_smear_low_truth [ir] = 0;
    m_jet_e            [ir] = 0;
    m_jet_eta          [ir] = 0;
    m_jet_phi          [ir] = 0;
    m_jet_emfrac       [ir] = 0;
    //m_jet_ihfrac  [ir] = 0;
    //m_jet_ohfrac  [ir] = 0;
    m_jet_time    [ir] = 0;
    // Were never reset before, so events without a third jet carried the previous event's values.
    hasthirdjet[ir] = 0;
    m_3jet_pt  [ir] = 0;
    m_3jet_eta [ir] = 0;
    m_3jet_phi [ir] = 0;
  }
  if (isMC) {

    m_cluster_pt_nosat   = 0;
    m_cluster_e_nosat    = 0;
    m_cluster_eta_nosat  = 0;
    m_cluster_phi_nosat  = 0;
    m_cluster_time_nosat = 0;
    m_cluster_bdt_score_nosat = 0;
    for (int i = 0; i < 12; i++) {
      m_cluster_showershape_nosat[i] = 0;
    }

    m_truth_cluster_pt  = 0;
    m_truth_cluster_e   = 0;
    m_truth_cluster_eta = 0;
    m_truth_cluster_phi = 0;
    for (int ir = 0; ir < m_nRadii; ir++) {
      m_truth_jet_pt [ir] = 0;
      m_truth_jet_e  [ir] = 0;
      m_truth_jet_eta[ir] = 0;
      m_truth_jet_phi[ir] = 0;
      m_hadron_p[ir] = 0;
      m_jet_con_dr[ir] = 0;
    }
  }
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
  m_truthinfo = findNode::getClass<PHG4TruthInfoContainer>(topNode, "G4TruthInfo");
  m_genevtmap = findNode::getClass<PHHepMCGenEventMap>(topNode,"PHHepMCGenEventMap");
  if(!m_truthinfo) {std::cout << "no truth info node... just skip the whole part.." << std::endl; return 1;}
  if (m_truthinfo)
  {
    PHG4VtxPoint *gvertex = m_truthinfo->GetPrimaryVtx(m_truthinfo->GetPrimaryVertexIndex());
    truth_vz = gvertex->get_z();
    truth_vx = gvertex->get_x();
    truth_vy = gvertex->get_y();
  }


  ProcessFillTruthPhotonParticle(truth_vz);

  for (int ir = 0; ir < m_nRadii; ir++) {
    JetContainer * truthjets = findNode::getClass<JetContainer>(topNode, m_truth_jet_nodenames[ir]);
    float maxjetpt = 0;
    bool found = false;
    //if (ir == 2) cout << m_truth_cluster_pt << " " << m_truth_cluster_eta << " " << m_truth_cluster_phi << endl;
    //if (ir == 2 && fabs(m_truth_cluster_pt - 26.7886) < 0.001 && fabs(m_truth_cluster_eta - 0.661972) < 0.001 && fabs(m_truth_cluster_phi - (-0.846589)) < 0.001) {
    //  found = true;
    //}
    if(truthjets)
    {
      for(unsigned i = 0; i < truthjets->size(); ++i)
      {
        Jet* jet = truthjets->get_jet(i);
        float pt  = jet->get_pt();
        float e   = jet->get_e();
        float eta = jet->get_eta();
        float phi = jet->get_phi();
        float dr = DeltaR(m_truth_cluster_eta, m_truth_cluster_phi, eta, phi);
        
        //if (found) cout << pt << " " << eta << " " << phi << " " << dr << endl;
        
        if (dr < m_radii[ir]) continue;
        if (pt < jet_pt_cut) continue;
        if (pt > maxjetpt) {
          maxjetpt = pt;
          m_truth_jet_pt [ir] = (pt);
          m_truth_jet_e  [ir] = (e);
          m_truth_jet_eta[ir] = (eta);
          m_truth_jet_phi[ir] = (phi);
         
          float max_p = 0;
          float max_dr = 0;
          // Get the maximum p hadron in this jet
          for (auto comp : jet->get_comp_vec()) {
            int barcode = comp.second;  // truth particle ID
            //std::cout << "Found particle with barcode = " << barcode << endl;

            PHG4Particle* particle = m_truthinfo->GetParticle(barcode);
            if (!particle) continue;

            // Basic hadron check (PDG code)
            int pdg = particle->get_pid();

            float px = particle->get_px();
            float py = particle->get_py();
            float pz = particle->get_pz();
            float p = std::sqrt(px*px + py*py + pz*pz);
            float h_eta = calc_eta(p, pz);
            float h_phi = std::atan2(py, px);
            float dr = DeltaR(eta, phi, h_eta, h_phi);
            
            if (dr > max_dr) {
              max_dr = dr;
            }
            
            // crude hadron selection: exclude leptons & photons
            if (abs(pdg) < 100) continue;


            if (p > max_p) {
              max_p = p;
            }
          }
          m_hadron_p[ir] = max_p;
          m_jet_con_dr[ir] = max_dr;
        }
      }
    }
  }
  return 0;
}


void CaloAna::ProcessFillTruthPhotonParticle(float truthvz){

  m_hepmc_by_barcode.clear();
  m_g4_by_id.clear();
  m_g4_by_barcode.clear();
  m_seen_barcodes.clear();

  float etamin = -10;//= GetShiftedEta(truthvz,-2);
  float etamax = 10;//= GetShiftedEta(truthvz,2);
  for(auto &ev : *m_genevtmap){
    PHHepMCGenEvent* evt = ev.second;
    if(!evt || !evt->getEvent()) continue;

    HepMC::GenEvent* h = evt->getEvent();
    for(auto it=h->particles_begin(); it!=h->particles_end(); ++it){
      HepMC::GenParticle* p = *it;
      m_hepmc_by_barcode[p->barcode()] = p;
    }
  }

  PHG4TruthInfoContainer::ConstRange range = m_truthinfo->GetPrimaryParticleRange();
  for(auto it=range.first; it!=range.second; ++it){
    PHG4Particle* p = it->second;
    m_g4_by_id[p->get_track_id()] = p;
    m_g4_by_barcode[p->get_barcode()] = p;
  }

  float maxclusterpt = 0;
  for(auto it=range.first; it!=range.second; ++it){
    PHG4Particle* g4p = it->second;
    if(!g4p) continue;
    if(g4p->get_pid() != 22) continue;

    int bc = g4p->get_barcode();
    if(m_seen_barcodes.count(bc)) continue;
    m_seen_barcodes.insert(bc);

    PHG4Particle* top_g4 = g4_to_top(g4p);
    if(!top_g4) continue; 

    int hepmc_bc = top_g4->get_barcode();
    HepMC::GenParticle* hep = get_hepmc_particle(hepmc_bc);

    if(!hep) continue;

    int root_pdg = find_hepmc_pdg(hep);
    if(is_hadron(root_pdg)) continue;

    float px = g4p->get_px();
    float py = g4p->get_py();
    float pz = g4p->get_pz();
    float e = g4p->get_e();

    float phi = std::atan2(py,px);
    float p = std::sqrt(px*px+py*py+pz*pz);
    float pt = std::sqrt(px*px+py*py);
    float eta = calc_eta(p, pz);
    if(eta > etamax || eta < etamin) continue;
    if (pt < cluster_pt_cut) continue;
    if (pt < maxclusterpt) continue;
    maxclusterpt = pt;
    float et = e / std::cosh(eta);
    // get isolation
    float iso3 = 0;
    float iso4 = 0;
    for (auto it2=range.first; it2!=range.second; ++it2) {
      PHG4Particle* par_g4p = it2->second;
      int par_id = par_g4p->get_pid();
      //if (par_g4p->status() != 0) continue;
      if (abs(par_id) == 12 || abs(par_id) == 14 || abs(par_id) == 16 || abs(par_id) == 18) continue; // no neutrinos
      float par_px = par_g4p->get_px();
      float par_py = par_g4p->get_py();
      float par_pz = par_g4p->get_pz();
      float par_e  = par_g4p->get_e();
      float par_p = std::sqrt(par_px*par_px+par_py*par_py+par_pz*par_pz);
      float par_eta = calc_eta(par_p, par_pz);
      float par_phi = std::atan2(par_py, par_px);
      float par_et = par_e / std::cosh(par_eta);
      float dr = DeltaR(par_eta, par_phi, eta, phi);
      //if (dr < 0.01) continue;
      if (dr < 0.3 && par_et > 0.07) iso3 += par_et;
      if (dr < 0.4 && par_et > 0.07) iso4 += par_et;
    }
    iso3 -= et;
    iso4 -= et;

    m_truth_cluster_pt   = (pt);
    m_truth_cluster_e    = (e);
    m_truth_cluster_eta  = (eta);
    m_truth_cluster_phi  = (phi);
    m_truth_cluster_iso3 = (iso3);
    m_truth_cluster_iso4 = (iso4);
  }
}


PHG4Particle* CaloAna::g4_to_top(PHG4Particle* p)
{
  PHG4Particle* cur = p;
  while(cur){
    int parid = cur->get_parent_id();
    if(parid <= 0) return cur; 

    auto it = m_g4_by_id.find(parid);
    if(it == m_g4_by_id.end()) return cur;

    PHG4Particle* parent = it->second;
    if(is_hadron(parent->get_pid())) return nullptr; 
    cur = parent;
  }
  return nullptr;
}

HepMC::GenParticle* CaloAna::get_hepmc_particle(int barcode)
{
  auto it = m_hepmc_by_barcode.find(barcode);
  if(it == m_hepmc_by_barcode.end()) return nullptr;
  return it->second;
}

int CaloAna::find_hepmc_pdg(HepMC::GenParticle* p)
{
  if(!p) return 0;

  HepMC::GenParticle* cur = p;

  while(true){
    HepMC::GenVertex* vtx = cur->production_vertex();
    if(!vtx) break;

    if(vtx->particles_in_const_begin() == vtx->particles_in_const_end())
      break;

    HepMC::GenParticle* parent = *(vtx->particles_in_const_begin());
    if(!parent) break;

    if(is_hadron(parent->pdg_id())) return parent->pdg_id();

    cur = parent;
  }

  return cur->pdg_id();
}
float CaloAna::calc_eta(float p, float pz) {
  float d=p-std::fabs(pz);
  if(p<=0||d<=0) return 0;
  float n=p+std::fabs(pz);
  float e=0.5*std::log(n/d);
  return (pz<0 ? -e : e);
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
Double_t CaloAna::DeltaR(float x1, float y1, float x2, float y2) {
  float deta = std::abs(x1-x2);
  float dphi = std::abs(y1-y2);
  if (dphi > M_PI) dphi -= 2*M_PI;

  double dr = TMath::Sqrt(deta*deta + dphi*dphi);
  return dr;
}

Double_t CaloAna::smear_pt(int ijet, float pt_calib, const std::vector<float> &truth_pt_by_jet, int sign) {
  if (!h_jer_smear_nominal || ijet < 0 || ijet >= (int)truth_pt_by_jet.size())
    return pt_calib; // no histogram or out of range
  // Unmatched jets (truth_pt_by_jet < 0) fall back to pt_calib as the resolution-lookup
  // reference instead of skipping smearing entirely - same convention pt_smear_reco/
  // pt_smear_high_reco/pt_smear_low_reco already use unconditionally. Returning pt_calib
  // unsmeared here left every jet below jet_calib_pt_cut (forced unmatched by the
  // truth-pairing prefilter above, which only attempts pairing for jets already passing
  // jet_calib_pt_cut) with zero smearing, while jets just above jet_calib_pt_cut got the
  // full (matched) smear - that discontinuity piled up an artificial excess of jets right
  // at/below jet_calib_pt_cut in jet_pt_smear_truth.
  bool matched = truth_pt_by_jet.at(ijet) >= 0;
  float pt_ref = matched ? truth_pt_by_jet.at(ijet) : pt_calib;
  const TH1D *h_width = (sign > 0) ? h_jer_smear_up : (sign < 0) ? h_jer_smear_down : h_jer_smear_nominal;
  float width = h_width->Interpolate(pt_ref);
  //std::cout << "Smearing jet with pt: " << pt_calib << " with width: " << pt_ref << "*" << width << std::endl;
  return rand.Gaus(pt_calib, pt_ref*width);
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
