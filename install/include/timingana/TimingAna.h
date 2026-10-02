#ifndef TIMINGANA_H
#define TIMINGANA_H

// Jet-timing study module. For each event, takes the leading (raw pT) jet at one radius,
// with NO timing requirement, and stores:
//  - the event's MBD time (t0-corrected as in CaloAna) and vertex,
//  - jet-level times: the CaloAna definition (energy-weighted over all constituent towers
//    with E > 0.1 GeV) plus the same quantity per calorimeter (EMCal retower, iHCal, oHCal)
//    and from the raw (non-retowered) EMCal towers,
//  - every constituent tower (EMCal retower / iHCal / oHCal) with energy, time, chi2, status,
//  - every raw EMCal tower that lies under one of the jet's EMCal retowers.
// All times are in ns (tower time in samples * 17.6 ns/sample, as in CaloAna).
//
// With SetTimeNodePrefix("TOWERINFO_CALIB_TIMING"), tower times (tw_time, em_time and the
// jet-level times) are read from Dading Chen's CaloTowerTimeCalibration sidecar nodes
// (TOWERINFO_CALIB_TIMING_<det>: same energies, corrected times; NaN where uncalibrated),
// while tw_time_std / em_time_std keep the standard TOWERINFO_CALIB times. The sidecar has
// no retowered EMCal node, so EMCal retower times (tw_calo == 0, jet_time_em) stay standard.

#include <fun4all/SubsysReco.h>

#include <TFile.h>
#include <TTree.h>

#include <string>
#include <vector>

class PHCompositeNode;
class TowerInfoContainer;

class TimingAna : public SubsysReco
{
 public:
  TimingAna(const std::string &name = "TimingAna", const std::string &outfile = "timing.root");
  ~TimingAna() override = default;

  int Init(PHCompositeNode *topNode) override;
  int process_event(PHCompositeNode *topNode) override;
  int End(PHCompositeNode *topNode) override;

  void SetRunNumber(int run) { m_run_number = run; }
  void SetJetRadius(float r) { m_radius = r; }
  void SetMinJetPt(float pt) { m_min_jet_pt = pt; }  // raw pT floor on the leading jet
  void SetTimeNodePrefix(const std::string &p) { m_time_prefix = p; }

 private:
  void BuildRawEmcalMap(PHCompositeNode *topNode);
  void ClearVectors();

  static constexpr float m_ns_per_sample = 17.6;
  static constexpr float m_tower_e_min = 0.1;  // CaloAna's jet-time tower threshold [GeV]
  static constexpr int m_neta_hcal = 24;
  static constexpr int m_nphi_hcal = 64;

  std::string m_outfilename;
  std::string m_time_prefix;  // empty: standard tower times only
  TFile *m_outfile = nullptr;
  TTree *m_tree = nullptr;

  int m_run_number = 0;
  float m_radius = 0.4;
  float m_min_jet_pt = 3;
  float m_mbd_t0corr = 0;
  long m_nevents = 0;

  // raw EMCal channel -> retower (HCal-grid) bin, built once from the geometry
  bool m_have_map = false;
  std::vector<int> m_raw_to_reta;
  std::vector<int> m_raw_to_rphi;

  // event
  int m_evt = 0;
  float m_vz = -999;
  float m_mbd_time = -999;  // (south + north)/2 - t0corr, as CaloAna
  float m_mbd_time_south = -999;
  float m_mbd_time_north = -999;
  ULong64_t m_scaled_vector = 0;
  ULong64_t m_live_vector = 0;
  int m_njets = 0;  // jets in the container above m_min_jet_pt (raw)

  // leading jet
  float m_jet_pt = 0;
  float m_jet_pt_calib = 0;
  float m_jet_e = 0;
  float m_jet_eta = 0;
  float m_jet_phi = 0;
  float m_jet_emfrac = 0;
  float m_jet_time = -999;      // CaloAna definition
  float m_jet_time_em = -999;   // EMCal retowers only, same weighting/threshold
  float m_jet_time_ih = -999;
  float m_jet_time_oh = -999;
  float m_jet_time_hcal = -999; // iHCal + oHCal together
  float m_jet_time_emraw = -999; // raw EMCal towers under the jet's retowers, E > 0.1 GeV
  float m_jet_e_em = 0;
  float m_jet_e_ih = 0;
  float m_jet_e_oh = 0;
  int m_jet_ncomp = 0;

  // constituent towers
  std::vector<int> m_tw_calo;  // 0 = EMCal retower, 1 = iHCal, 2 = oHCal
  std::vector<int> m_tw_ieta;
  std::vector<int> m_tw_iphi;
  std::vector<float> m_tw_e;
  std::vector<float> m_tw_time;
  std::vector<float> m_tw_time_std;
  std::vector<float> m_tw_chi2;  // for EMCal retowers RetowerCEMC stores the bad-area fraction here
  std::vector<int> m_tw_status;
  std::vector<bool> m_tw_isgood;

  // raw EMCal towers under the jet's EMCal retowers
  std::vector<int> m_em_ieta;
  std::vector<int> m_em_iphi;
  std::vector<float> m_em_e;
  std::vector<float> m_em_time;
  std::vector<float> m_em_time_std;
  std::vector<float> m_em_chi2;
  std::vector<int> m_em_status;
  std::vector<bool> m_em_isgood;
};

#endif
