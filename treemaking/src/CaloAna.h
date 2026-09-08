#ifndef CALOANA_H__
#define CALOANA_H__

// Utility
#include <vector>
#include <utility>
#include <fstream>
#include <TMath.h>
#include <TRandom.h>
#include <TFile.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TGraph2D.h>
#include <TF2.h>
#include <cassert>
#include <sstream>
#include <string>
#include <algorithm>
#include <TLorentzVector.h>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>  // for gsl_rng_uniform_pos

// Fun4All
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllHistoManager.h>
#include <fun4all/Fun4AllReturnCodes.h>

// Event
#include <Event/Event.h>
#include <Event/packet.h>
#include <ffaobjects/EventHeaderv1.h>

//Trigger
#include <calotrigger/TriggerRunInfov1.h>
#include <calotrigger/TriggerAnalyzer.h>

// Jet base
#include <jetbase/Jetv1.h>
#include <jetbase/Jetv2.h>
#include <jetbase/JetContainer.h>
#include <jetbase/JetAlgo.h>
#include <jetbase/JetMap.h>
#include <jetbackground/TowerBackgroundv1.h>

// Global vertex
#include <globalvertex/GlobalVertexMap.h>
#include <globalvertex/GlobalVertexMapv1.h>
#include <globalvertex/MbdVertex.h>
#include <globalvertex/MbdVertexMapv1.h>
#include <globalvertex/GlobalVertex.h>

// G4
#include <g4main/PHG4Hit.h>
#include <g4main/PHG4HitContainer.h>
#include <g4main/PHG4Particle.h>
#include <g4main/PHG4VtxPoint.h>
#include <g4main/PHG4Shower.h>
#include <g4main/PHG4TruthInfoContainer.h>

// G4Cells includes
#include <g4detectors/PHG4Cell.h>
#include <g4detectors/PHG4CellContainer.h>

// Tower includes
#include <calobase/RawTower.h>
#include <calobase/RawTowerContainer.h>
#include <calobase/RawTowerGeom.h>
#include <calobase/RawTowerGeomContainer.h>
#include <calobase/RawTowerGeomContainer_Cylinderv1.h>
#include <calobase/TowerInfoContainerv1.h>
#include <calobase/TowerInfov1.h>
#include <calobase/TowerInfoContainerSimv1.h>
#include <calobase/TowerInfoSimv1.h>
#include <calobase/TowerInfoContainerv2.h>
#include <calobase/TowerInfov2.h>
#include <calobase/TowerInfoContainerv3.h>
#include <calobase/TowerInfov3.h>
#include <calobase/TowerInfoContainerv4.h>
#include <calobase/TowerInfov4.h>
#include <calobase/TowerInfoDefs.h>

// MBD
#include <mbd/MbdOut.h>
#include <mbd/MbdPmtContainer.h>
#include <mbd/MbdPmtContainerV1.h>
#include <mbd/MbdPmtSimContainerV1.h>
#include <mbd/MbdPmtHit.h>
#include <mbd/MbdGeom.h>

// Cluster includes
#include <calobase/RawCluster.h>
#include <calobase/RawClusterv1.h>
#include <calobase/RawClusterContainer.h>
#include <calobase/RawClusterUtility.h>

// Photon includes
#include <calobase/PhotonClusterv1.h>

// phool
#include <phool/getClass.h>
#include <phool/PHCompositeNode.h>

// HepMC
#include <phhepmc/PHHepMCGenEventMap.h>

#include "cluster.h"
#include "jet.h"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <HepMC/GenEvent.h>
#include <HepMC/GenVertex.h>
#pragma GCC diagnostic pop

//#include <HepMC/GenVertex.h>             // for GenVertex, GenVertex::partic...
//#include <HepMC/GenParticle.h>           // for GenParticle
#include <phhepmc/PHHepMCGenEvent.h>
//#include <phhepmc/PHHepMCGenEventMap.h>

// Centrality MB
#include <centrality/CentralityInfo.h>
#include <calotrigger/MinimumBiasInfo.h>
#include <centrality/CentralityInfov1.h>

#include <ffarawobjects/Gl1Packet.h>

// Forward declarations
class Fun4AllHistoManager;
class PHCompositeNode;
class TFile;
class TNtuple;
class TTree;
class TH2F;
class TH1;
class Gl1Packet;
class PHG4Shower;

class CaloAna : public SubsysReco
{
 public:
  //! constructor
  CaloAna(const std::string &name = "CaloAna",  const char* outname="DST-00021615-0000.root");

  //! destructor
  virtual ~CaloAna();

  //! full initialization
  int Init(PHCompositeNode *);
  void InitOutputFile();
  void InitTree();
  
  //! event processing method
  int process_event(PHCompositeNode *);
  int ProcessGlobalEventInfo(PHCompositeNode *);
  int ProcessTruth(PHCompositeNode *);
  void ProcessFillTruthPhotonParticle(float truthvz);
  PHG4Particle * g4_to_top(PHG4Particle*);
  HepMC::GenParticle* get_hepmc_particle(int barcode);
  int find_hepmc_pdg(HepMC::GenParticle* p);
  int process_towers(PHCompositeNode *);
  void ProcessPhotonCandidate(RawClusterContainer *photons, Float_t &out_pt, Float_t &out_e,
                               Float_t &out_eta, Float_t &out_phi, Float_t &out_time,
                               Float_t out_showershape[12], Float_t &out_bdt_score);
  void Clear();

  //! end of run method
  int End(PHCompositeNode *);

  bool is_hadron(int pdg) {return (std::abs(pdg) >= 23 && std::abs(pdg) != 2212 ); }
  float calc_eta(float p, float pz);
  Double_t GetShiftedEta(float _vz, float _eta);
  Double_t DeltaR(float x1, float x2, float y1, float y2);
  Double_t smear_pt(int ijet, float pt_calib, const std::vector<float> &truth_pt_by_jet, int sign = 0);

  void SetIsMC(bool ismc) { isMC = ismc; };
  void SetMbdZVtxCut(float _zvtxcut)
  {
    if(_zvtxcut<=0) IsVtxCut = false;
    else IsVtxCut = true;
    vzcut = _zvtxcut;
  };

  void SetRunNumber(int RunNumber)
  {
    m_run_number = RunNumber;
  };

  void SetScaledowns(int scaledowns[])
  {
    for(int i=0; i<64;i++){
      m_scaledowns[i] = scaledowns[i];
    }
  };

  void Detector(const std::string &name) { detector = name; }
  void ProcessClearBranchVar();

 protected:
  TRandom rand;
  TH1D *h_jerband_quaddiff = nullptr; // pt-dependent JER resolution, same source smear_reco and smear_pt draw from
  std::string detector;
  std::string outfilename;
  Fun4AllHistoManager *hm = nullptr;
  TFile *outfile = nullptr;
  TTree *towerntuple = nullptr;
  bool isMC = 0;

  Long64_t mbdlivecount=0;
  int count=0;
  int count_mbdvtx=0;
  float vzcut;
  bool IsVtxCut = true;
  int inputDimx = 31;
  int inputDimy = 31;

  int m_run_number=-999;
  float vx, vy, vz;
  float truth_vx, truth_vy, truth_vz;
  bool ScaledTriggerBit[64];
  bool LiveTriggerBit[64];
  int m_scaledowns[64];
  
  float m_mbd_time;
  float mbd_t0corr;

  float cluster_pt_cut = 10/1.011; // For EM Scale uncertainty
  float jet_pt_cut = 3;
  float jet_calib_pt_cut = 5;
  static const int m_nRadii = 7;
  // Index into m_radii/m_jet_nodenames for r=0.4, matched to AntiKt_unsubtracted_r04_calib_old
  static const int m_oldCalibRadiusIndex = 2;
  std::string m_jet_calib_old_nodename = "AntiKt_unsubtracted_r04_calib_old";
  float smear[m_nRadii] =    {0.107, 0.107, 0.100, 0.097, 0.087, 0.097, 0.091};
  float smearvar[m_nRadii] = {0.024, 0.024, 0.018, 0.021, 0.015, 0.012, 0.028};
  
  static constexpr float m_radii[m_nRadii] = {0.2,0.3,0.4,0.5,0.6,0.7,0.8};
  std::string m_jet_nodenames[m_nRadii] = {
    "AntiKt_unsubtracted_r02",
    "AntiKt_unsubtracted_r03",
    "AntiKt_unsubtracted_r04",
    "AntiKt_unsubtracted_r05",
    "AntiKt_unsubtracted_r06",
    "AntiKt_unsubtracted_r07",
    "AntiKt_unsubtracted_r08"
  };
  std::string m_jet_calib_nodenames[m_nRadii] = {
    "AntiKt_unsubtracted_r02_calib",
    "AntiKt_unsubtracted_r03_calib",
    "AntiKt_unsubtracted_r04_calib",
    "AntiKt_unsubtracted_r05_calib",
    "AntiKt_unsubtracted_r06_calib",
    "AntiKt_unsubtracted_r07_calib",
    "AntiKt_unsubtracted_r08_calib"
  };
  std::string m_truth_jet_nodenames[m_nRadii] = {
    "AntiKt_Truth_r02",
    "AntiKt_Truth_r03",
    "AntiKt_Truth_r04",
    "AntiKt_Truth_r05",
    "AntiKt_Truth_r06",
    "AntiKt_Truth_r07",
    "AntiKt_Truth_r08"
  };
  
  bool hasthirdjet[m_nRadii] = { 0 };
  Float_t m_hadron_p[m_nRadii] = { 0 };
  Float_t m_jet_con_dr[m_nRadii] = { 0 };

  Float_t m_cluster_pt = 0;
  Float_t m_cluster_e = 0;
  Float_t m_cluster_eta = 0;
  Float_t m_cluster_phi = 0;
  Float_t m_cluster_time = 0;
  Float_t m_cluster_showershape[14] = { 0 };
  Float_t m_cluster_bdt_scores[11] = { 0 };
  Float_t m_cluster_Z = { 0 };

  // Old BDT model, same (saturated) CEMC towers as the nominal cluster above --
  // reads PHOTONCLUSTER_CEMC_OLD0 (see photon/oldphoton in Fun4All_macro.C /
  // MCFun4All_macro.C). Present for both data and MC.
  Float_t m_cluster_pt_old = 0;
  Float_t m_cluster_e_old = 0;
  Float_t m_cluster_eta_old = 0;
  Float_t m_cluster_phi_old = 0;
  Float_t m_cluster_time_old = 0;
  Float_t m_cluster_showershape_old[12] = { 0 };
  Float_t m_cluster_bdt_score_old = 0;

  // MC-only: no-pixel-saturation CEMC branch (see MCFun4All_macro.C). Reads
  // PHOTONCLUSTER_CEMC_NOSAT0, which doesn't exist for real data.
  Float_t m_cluster_pt_nosat = 0;
  Float_t m_cluster_e_nosat = 0;
  Float_t m_cluster_eta_nosat = 0;
  Float_t m_cluster_phi_nosat = 0;
  Float_t m_cluster_time_nosat = 0;
  Float_t m_cluster_showershape_nosat[12] = { 0 };
  Float_t m_cluster_bdt_score_nosat = 0;

  Float_t m_truth_cluster_pt = 0;
  Float_t m_truth_cluster_e = 0;
  Float_t m_truth_cluster_eta = 0;
  Float_t m_truth_cluster_phi = 0;
  Float_t m_truth_cluster_iso3 = 0;
  Float_t m_truth_cluster_iso4 = 0;
  Float_t m_truth_cluster_time = 0;

  Float_t m_jet_pt           [m_nRadii];
  Float_t m_jet_pt_calib     [m_nRadii];
  Float_t m_jet_pt_recalib   [m_nRadii];
  // Old JES calibration methodology, r=0.4 only (AntiKt_unsubtracted_r04_calib_old,
  // see jetCalibOld in Fun4All_macro.C / MCFun4All_macro.C). Present for both data and MC.
  Float_t m_jet_pt_old = 0;
  Float_t m_jet_pt_smear_reco      [m_nRadii];
  Float_t m_jet_pt_smear_high_reco [m_nRadii];
  Float_t m_jet_pt_smear_low_reco  [m_nRadii];
  Float_t m_jet_pt_smear_truth     [m_nRadii];
  Float_t m_jet_pt_smear_high_truth[m_nRadii];
  Float_t m_jet_pt_smear_low_truth [m_nRadii];
  Float_t m_jet_e            [m_nRadii];
  Float_t m_jet_eta          [m_nRadii];
  Float_t m_jet_phi          [m_nRadii];
  Float_t m_jet_emfrac       [m_nRadii];
  Float_t m_jet_ihfrac       [m_nRadii];
  Float_t m_jet_ohfrac       [m_nRadii];
  Float_t m_jet_time         [m_nRadii];
  Float_t m_3jet_pt          [m_nRadii];
  Float_t m_3jet_dr          [m_nRadii];

  Float_t m_truth_jet_pt     [m_nRadii];
  Float_t m_truth_jet_e      [m_nRadii];
  Float_t m_truth_jet_eta    [m_nRadii];
  Float_t m_truth_jet_phi    [m_nRadii];
  Float_t m_truth_jet_emfrac [m_nRadii]; 
  Float_t m_truth_jet_ihfrac [m_nRadii];
  Float_t m_truth_jet_ohfrac [m_nRadii];
  Float_t m_truth_jet_time   [m_nRadii];


  PHHepMCGenEventMap* m_genevtmap = nullptr;
  PHG4TruthInfoContainer* m_truthinfo = nullptr;

  std::unordered_map<int, HepMC::GenParticle*> m_hepmc_by_barcode;
  std::unordered_map<int, PHG4Particle*> m_g4_by_id;
  std::unordered_map<int, PHG4Particle*> m_g4_by_barcode;
  std::unordered_set<int> m_seen_barcodes;
};

#endif
