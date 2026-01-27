#ifndef CALOANA_H__
#define CALOANA_H__

// Utility
#include <vector>
#include <fstream>
#include <TMath.h>
#include <TFile.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TH2.h>
#include <TGraph2D.h>
#include <TF2.h>
#include <cassert>
#include <sstream>
#include <string>
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
  void ProcessFillTruthParticle(PHCompositeNode *, float truthvz);

  //! end of run method
  int End(PHCompositeNode *);

  Double_t GetShiftedEta(float _vz, float _eta);

  int process_g4hits(PHCompositeNode *);
  int process_g4cells(PHCompositeNode *);
  int process_towers(PHCompositeNode *);
  int process_clusters(PHCompositeNode *);
  bool FindConversion(PHG4TruthInfoContainer *, int trackid, float energy);
  int Getpeaktime(TH1 *h);

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
  std::string detector;
  std::string outfilename;
  Fun4AllHistoManager *hm = nullptr;
  TFile *outfile = nullptr;
  TTree *towerntuple = nullptr;
  TH1D * profilehist;
  TH1D * histlumicount;
  
  Long64_t mbdlivecount=0;
  int count=0;
  int count_mbdvtx=0;
  float vzcut;
  bool IsVtxCut = true;
  int inputDimx = 31;
  int inputDimy = 31;

  int m_run_number=-999;
  float vx, vy, vz;
  bool ScaledTriggerBit[64];
  bool LiveTriggerBit[64];
  int m_scaledowns[64];
  bool istriggeredns = false;
  bool istriggeredsp = false; 
  
  int m_mbd_nhits_south = 0;
  int m_mbd_nhits_north = 0;
  float m_mbd_time_south;
  float m_mbd_time_north;

  static const int Max_photon_size = 10000;
  static const int Max_jet_size = 10000;
  float cluster_emin = 7;
  float jet_emin_cut = 2;
  Short_t nClusters_sp = 0;
  Short_t nClusters_ns = 0;
  Short_t nJets02 = 0;
  Short_t nJets04 = 0;
  Short_t nJets06 = 0;
  Short_t nJets08 = 0;
 
  TClonesArray * m_cluster_4mom_sp = new TClonesArray("TLorentzVector", Max_photon_size);
  float m_cluster_showershape_sp[Max_photon_size][6];
  float m_cluster_time_sp[Max_photon_size];
  float m_cluster_iso_e02_sp[Max_photon_size];
  float m_cluster_iso_e04_sp[Max_photon_size];
  float m_cluster_iso_e06_sp[Max_photon_size];
  float m_cluster_iso_e08_sp[Max_photon_size];
  TClonesArray * m_cluster_4mom_ns = new TClonesArray("TLorentzVector", Max_photon_size);
  float m_cluster_showershape_ns[Max_photon_size][8];
  float m_cluster_time_ns[Max_photon_size];
  float m_cluster_iso_e02_ns[Max_photon_size];
  float m_cluster_iso_e04_ns[Max_photon_size];
  float m_cluster_iso_e06_ns[Max_photon_size];
  float m_cluster_iso_e08_ns[Max_photon_size];
  TClonesArray * m_truth_cluster_4mom = new TClonesArray("TLorentzVector", Max_photon_size);
  float m_truth_cluster_showershape[Max_photon_size][8];
  float m_truth_cluster_time[Max_photon_size];

  TClonesArray * m_jet_4mom02 = new TClonesArray("TLorentzVector", Max_jet_size);
  float m_jet_emfrac02[Max_jet_size];
  float m_jet_ihfrac02[Max_jet_size];
  float m_jet_ohfrac02[Max_jet_size];
  float m_jet_time02[Max_jet_size];
  TClonesArray * m_jet_4mom04 = new TClonesArray("TLorentzVector", Max_jet_size);
  float m_jet_emfrac04[Max_jet_size];
  float m_jet_ihfrac04[Max_jet_size];
  float m_jet_ohfrac04[Max_jet_size];
  float m_jet_time04[Max_jet_size];
  TClonesArray * m_jet_4mom06 = new TClonesArray("TLorentzVector", Max_jet_size);
  float m_jet_emfrac06[Max_jet_size];
  float m_jet_ihfrac06[Max_jet_size];
  float m_jet_ohfrac06[Max_jet_size];
  float m_jet_time06[Max_jet_size];
  TClonesArray * m_jet_4mom08 = new TClonesArray("TLorentzVector", Max_jet_size);
  float m_jet_emfrac08[Max_jet_size];
  float m_jet_ihfrac08[Max_jet_size];
  float m_jet_ohfrac08[Max_jet_size];
  float m_jet_time08[Max_jet_size];
  
  TClonesArray * m_truth_jet_4mom02 = new TClonesArray("TLorentzVector", Max_jet_size);
  float m_truth_jet_emfrac02[Max_jet_size];
  float m_truth_jet_ihfrac02[Max_jet_size];
  float m_truth_jet_ohfrac02[Max_jet_size];
  float m_truth_jet_time02[Max_jet_size];
  TClonesArray * m_truth_jet_4mom04 = new TClonesArray("TLorentzVector", Max_jet_size);
  float m_truth_jet_emfrac04[Max_jet_size];
  float m_truth_jet_ihfrac04[Max_jet_size];
  float m_truth_jet_ohfrac04[Max_jet_size];
  float m_truth_jet_time04[Max_jet_size];
  TClonesArray * m_truth_jet_4mom06 = new TClonesArray("TLorentzVector", Max_jet_size);
  float m_truth_jet_emfrac06[Max_jet_size];
  float m_truth_jet_ihfrac06[Max_jet_size];
  float m_truth_jet_ohfrac06[Max_jet_size];
  float m_truth_jet_time06[Max_jet_size];
  TClonesArray * m_truth_jet_4mom08 = new TClonesArray("TLorentzVector", Max_jet_size);
  float m_truth_jet_emfrac08[Max_jet_size];
  float m_truth_jet_ihfrac08[Max_jet_size];
  float m_truth_jet_ohfrac08[Max_jet_size];
  float m_truth_jet_time08[Max_jet_size];

};

#endif
