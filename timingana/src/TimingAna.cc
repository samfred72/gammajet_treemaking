#include "TimingAna.h"

#include <calobase/RawTowerDefs.h>
#include <calobase/RawTowerGeom.h>
#include <calobase/RawTowerGeomContainer.h>
#include <calobase/TowerInfo.h>
#include <calobase/TowerInfoContainer.h>
#include <ffarawobjects/Gl1Packet.h>
#include <fun4all/Fun4AllReturnCodes.h>
#include <globalvertex/GlobalVertex.h>
#include <globalvertex/GlobalVertexMap.h>
#include <jetbase/Jet.h>
#include <jetbase/JetContainer.h>
#include <mbd/MbdOut.h>
#include <phool/PHCompositeNode.h>
#include <phool/getClass.h>

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

TimingAna::TimingAna(const std::string &name, const std::string &outfile)
  : SubsysReco(name)
  , m_outfilename(outfile)
{
}

int TimingAna::Init(PHCompositeNode * /*topNode*/)
{
  // Same per-run MBD t0 correction CaloAna uses.
  std::ifstream file("/sphenix/user/samfred/projects/mbdt0/histmaking/MbdPmt.corr");
  std::string line;
  while (std::getline(file, line))
  {
    int irun;
    float t0;
    std::istringstream iss(line);
    if (iss >> irun >> t0 && irun == m_run_number)
    {
      m_mbd_t0corr = t0;
      break;
    }
  }
  std::cout << "TimingAna: run " << m_run_number << ", MBD t0 correction " << m_mbd_t0corr
            << " ns, jet R = " << m_radius << std::endl;

  m_outfile = new TFile(m_outfilename.c_str(), "RECREATE");
  m_tree = new TTree("T", "leading-jet timing");

  m_tree->Branch("run", &m_run_number);
  m_tree->Branch("evt", &m_evt);
  m_tree->Branch("vz", &m_vz);
  m_tree->Branch("mbd_time", &m_mbd_time);
  m_tree->Branch("mbd_time_south", &m_mbd_time_south);
  m_tree->Branch("mbd_time_north", &m_mbd_time_north);
  m_tree->Branch("scaled_vector", &m_scaled_vector);
  m_tree->Branch("live_vector", &m_live_vector);
  m_tree->Branch("njets", &m_njets);

  m_tree->Branch("jet_pt", &m_jet_pt);
  m_tree->Branch("jet_pt_calib", &m_jet_pt_calib);
  m_tree->Branch("jet_e", &m_jet_e);
  m_tree->Branch("jet_eta", &m_jet_eta);
  m_tree->Branch("jet_phi", &m_jet_phi);
  m_tree->Branch("jet_emfrac", &m_jet_emfrac);
  m_tree->Branch("jet_time", &m_jet_time);
  m_tree->Branch("jet_time_em", &m_jet_time_em);
  m_tree->Branch("jet_time_ih", &m_jet_time_ih);
  m_tree->Branch("jet_time_oh", &m_jet_time_oh);
  m_tree->Branch("jet_time_hcal", &m_jet_time_hcal);
  m_tree->Branch("jet_time_emraw", &m_jet_time_emraw);
  m_tree->Branch("jet_e_em", &m_jet_e_em);
  m_tree->Branch("jet_e_ih", &m_jet_e_ih);
  m_tree->Branch("jet_e_oh", &m_jet_e_oh);
  m_tree->Branch("jet_ncomp", &m_jet_ncomp);

  m_tree->Branch("tw_calo", &m_tw_calo);
  m_tree->Branch("tw_ieta", &m_tw_ieta);
  m_tree->Branch("tw_iphi", &m_tw_iphi);
  m_tree->Branch("tw_e", &m_tw_e);
  m_tree->Branch("tw_time", &m_tw_time);
  m_tree->Branch("tw_time_std", &m_tw_time_std);
  m_tree->Branch("tw_chi2", &m_tw_chi2);
  m_tree->Branch("tw_status", &m_tw_status);
  m_tree->Branch("tw_isgood", &m_tw_isgood);

  m_tree->Branch("em_ieta", &m_em_ieta);
  m_tree->Branch("em_iphi", &m_em_iphi);
  m_tree->Branch("em_e", &m_em_e);
  m_tree->Branch("em_time", &m_em_time);
  m_tree->Branch("em_time_std", &m_em_time_std);
  m_tree->Branch("em_chi2", &m_em_chi2);
  m_tree->Branch("em_status", &m_em_status);
  m_tree->Branch("em_isgood", &m_em_isgood);

  return Fun4AllReturnCodes::EVENT_OK;
}

// Raw EMCal tower -> retower bin, using the geometry the same way RetowerCEMC::get_fraction
// does (EMCal tower centre looked up in the iHCal binning).
void TimingAna::BuildRawEmcalMap(PHCompositeNode *topNode)
{
  TowerInfoContainer *emraw = findNode::getClass<TowerInfoContainer>(topNode, "TOWERINFO_CALIB_CEMC");
  RawTowerGeomContainer *geomEM = findNode::getClass<RawTowerGeomContainer>(topNode, "TOWERGEOM_CEMC");
  RawTowerGeomContainer *geomIH = findNode::getClass<RawTowerGeomContainer>(topNode, "TOWERGEOM_HCALIN");
  if (!emraw || !geomEM || !geomIH)
  {
    std::cout << "TimingAna: missing TOWERINFO_CALIB_CEMC or geometry nodes, raw EMCal towers not stored" << std::endl;
    m_have_map = true;  // don't retry every event
    return;
  }
  unsigned int n = emraw->size();
  m_raw_to_reta.assign(n, -1);
  m_raw_to_rphi.assign(n, -1);
  for (unsigned int ch = 0; ch < n; ch++)
  {
    unsigned int key = emraw->encode_key(ch);
    int ieta = emraw->getTowerEtaBin(key);
    int iphi = emraw->getTowerPhiBin(key);
    RawTowerGeom *g = geomEM->get_tower_geometry(RawTowerDefs::encode_towerid(RawTowerDefs::CalorimeterId::CEMC, ieta, iphi));
    if (!g) continue;
    m_raw_to_reta[ch] = geomIH->get_etabin(g->get_eta());
    m_raw_to_rphi[ch] = geomIH->get_phibin(g->get_phi());
  }
  m_have_map = true;
}

void TimingAna::ClearVectors()
{
  m_tw_calo.clear();
  m_tw_ieta.clear();
  m_tw_iphi.clear();
  m_tw_e.clear();
  m_tw_time.clear();
  m_tw_time_std.clear();
  m_tw_chi2.clear();
  m_tw_status.clear();
  m_tw_isgood.clear();
  m_em_ieta.clear();
  m_em_iphi.clear();
  m_em_e.clear();
  m_em_time.clear();
  m_em_time_std.clear();
  m_em_chi2.clear();
  m_em_status.clear();
  m_em_isgood.clear();
}

int TimingAna::process_event(PHCompositeNode *topNode)
{
  m_evt = m_nevents++;
  if (m_nevents % 1000 == 0) std::cout << "TimingAna: event " << m_nevents << std::endl;
  if (!m_have_map) BuildRawEmcalMap(topNode);
  ClearVectors();

  // ---- event-level ----
  m_vz = -999;
  GlobalVertexMap *vtxmap = findNode::getClass<GlobalVertexMap>(topNode, "GlobalVertexMap");
  if (vtxmap && !vtxmap->empty()) m_vz = vtxmap->begin()->second->get_z();

  m_mbd_time = m_mbd_time_south = m_mbd_time_north = -999;
  MbdOut *mbdout = findNode::getClass<MbdOut>(topNode, "MbdOut");
  if (mbdout)
  {
    m_mbd_time_south = mbdout->get_time(0);
    m_mbd_time_north = mbdout->get_time(1);
    m_mbd_time = (m_mbd_time_south + m_mbd_time_north) / 2.0 - m_mbd_t0corr;
  }

  m_scaled_vector = m_live_vector = 0;
  Gl1Packet *gl1 = findNode::getClass<Gl1Packet>(topNode, "14001");
  if (gl1)
  {
    m_scaled_vector = gl1->lValue(0, "ScaledVector");
    m_live_vector = gl1->lValue(0, "TriggerVector");
  }

  // ---- leading jet, no timing requirement ----
  int ir10 = std::lround(m_radius * 10);
  std::string node = "AntiKt_unsubtracted_r0" + std::to_string(ir10);
  JetContainer *jets = findNode::getClass<JetContainer>(topNode, node);
  JetContainer *jets_calib = findNode::getClass<JetContainer>(topNode, node + "_calib");
  TowerInfoContainer *emre = findNode::getClass<TowerInfoContainer>(topNode, "TOWERINFO_CALIB_CEMC_RETOWER");
  TowerInfoContainer *ih = findNode::getClass<TowerInfoContainer>(topNode, "TOWERINFO_CALIB_HCALIN");
  TowerInfoContainer *oh = findNode::getClass<TowerInfoContainer>(topNode, "TOWERINFO_CALIB_HCALOUT");
  TowerInfoContainer *emraw = findNode::getClass<TowerInfoContainer>(topNode, "TOWERINFO_CALIB_CEMC");
  // Corrected-time containers (same channel indexing); fall back to standard if not set.
  TowerInfoContainer *ih_t = ih, *oh_t = oh, *emraw_t = emraw;
  if (!m_time_prefix.empty())
  {
    ih_t = findNode::getClass<TowerInfoContainer>(topNode, m_time_prefix + "_HCALIN");
    oh_t = findNode::getClass<TowerInfoContainer>(topNode, m_time_prefix + "_HCALOUT");
    emraw_t = findNode::getClass<TowerInfoContainer>(topNode, m_time_prefix + "_CEMC");
    if (!ih_t || !oh_t || !emraw_t)
    {
      std::cout << "TimingAna: missing " << m_time_prefix << "_* timing nodes, skipping event" << std::endl;
      return Fun4AllReturnCodes::ABORTEVENT;
    }
  }
  if (!jets || !emre || !ih || !oh)
  {
    std::cout << "TimingAna: missing jet or tower node, skipping event" << std::endl;
    return Fun4AllReturnCodes::ABORTEVENT;
  }

  Jet *lead = nullptr;
  unsigned ilead = 0;
  m_njets = 0;
  for (unsigned ij = 0; ij < jets->size(); ij++)
  {
    Jet *j = jets->get_jet(ij);
    if (j->get_pt() < m_min_jet_pt) continue;
    m_njets++;
    if (!lead || j->get_pt() > lead->get_pt())
    {
      lead = j;
      ilead = ij;
    }
  }
  if (!lead) return Fun4AllReturnCodes::ABORTEVENT;

  m_jet_pt = lead->get_pt();
  m_jet_e = lead->get_e();
  m_jet_eta = lead->get_eta();
  m_jet_phi = lead->get_phi();
  m_jet_pt_calib = (jets_calib && ilead < jets_calib->size()) ? jets_calib->get_jet(ilead)->get_pt() : -999;

  // Energy-weighted time sums, E > m_tower_e_min, per calorimeter (0 EM retower, 1 iH, 2 oH).
  double esum[3] = {0, 0, 0};    // all constituent energy (for emfrac, as CaloAna)
  double twsum[3] = {0, 0, 0};   // sum E*t over towers above threshold
  double tesum[3] = {0, 0, 0};   // sum E over towers above threshold
  bool inJet[m_neta_hcal][m_nphi_hcal] = {};

  m_jet_ncomp = 0;
  for (auto comp : lead->get_comp_vec())
  {
    int calo = -1;
    TowerInfoContainer *cont = nullptr, *tcont = nullptr;
    if (comp.first == 14 || comp.first == 29 || comp.first == 25 || comp.first == 28) { calo = 0; cont = emre; tcont = emre; }
    else if (comp.first == 15 || comp.first == 30 || comp.first == 26) { calo = 1; cont = ih; tcont = ih_t; }
    else if (comp.first == 16 || comp.first == 31 || comp.first == 27) { calo = 2; cont = oh; tcont = oh_t; }
    if (calo < 0) continue;
    unsigned int ch = comp.second;
    TowerInfo *t = cont->get_tower_at_channel(ch);
    if (!t) continue;
    unsigned int key = cont->encode_key(ch);
    int ieta = cont->getTowerEtaBin(key);
    int iphi = cont->getTowerPhiBin(key);
    float e = t->get_energy();
    float time_std = t->get_time() * m_ns_per_sample;
    TowerInfo *tt = tcont->get_tower_at_channel(ch);
    float time = tt ? tt->get_time() * m_ns_per_sample : time_std;

    m_jet_ncomp++;
    m_tw_calo.push_back(calo);
    m_tw_ieta.push_back(ieta);
    m_tw_iphi.push_back(iphi);
    m_tw_e.push_back(e);
    m_tw_time.push_back(time);
    m_tw_time_std.push_back(time_std);
    m_tw_chi2.push_back(t->get_chi2());
    m_tw_status.push_back(t->get_status());
    m_tw_isgood.push_back(t->get_isGood());

    esum[calo] += e;
    if (e > m_tower_e_min && std::isfinite(time))
    {
      twsum[calo] += e * time;
      tesum[calo] += e;
    }
    if (calo == 0 && ieta >= 0 && ieta < m_neta_hcal && iphi >= 0 && iphi < m_nphi_hcal) inJet[ieta][iphi] = true;
  }

  auto wmean = [](double tw, double te) { return te > 0 ? float(tw / te) : -999.f; };
  m_jet_e_em = esum[0];
  m_jet_e_ih = esum[1];
  m_jet_e_oh = esum[2];
  m_jet_emfrac = esum[0] / (esum[0] + esum[1] + esum[2]);
  m_jet_time = wmean(twsum[0] + twsum[1] + twsum[2], tesum[0] + tesum[1] + tesum[2]);
  m_jet_time_em = wmean(twsum[0], tesum[0]);
  m_jet_time_ih = wmean(twsum[1], tesum[1]);
  m_jet_time_oh = wmean(twsum[2], tesum[2]);
  m_jet_time_hcal = wmean(twsum[1] + twsum[2], tesum[1] + tesum[2]);

  // Raw EMCal towers under the jet's retowers.
  double rtw = 0, rte = 0;
  if (emraw && !m_raw_to_reta.empty())
  {
    for (unsigned int ch = 0; ch < emraw->size() && ch < m_raw_to_reta.size(); ch++)
    {
      int re = m_raw_to_reta[ch], rp = m_raw_to_rphi[ch];
      if (re < 0 || re >= m_neta_hcal || rp < 0 || rp >= m_nphi_hcal || !inJet[re][rp]) continue;
      TowerInfo *t = emraw->get_tower_at_channel(ch);
      if (!t) continue;
      unsigned int key = emraw->encode_key(ch);
      float e = t->get_energy();
      float time_std = t->get_time() * m_ns_per_sample;
      TowerInfo *tt = emraw_t->get_tower_at_channel(ch);
      float time = tt ? tt->get_time() * m_ns_per_sample : time_std;
      m_em_ieta.push_back(emraw->getTowerEtaBin(key));
      m_em_iphi.push_back(emraw->getTowerPhiBin(key));
      m_em_e.push_back(e);
      m_em_time.push_back(time);
      m_em_time_std.push_back(time_std);
      m_em_chi2.push_back(t->get_chi2());
      m_em_status.push_back(t->get_status());
      m_em_isgood.push_back(t->get_isGood());
      if (t->get_isGood() && e > m_tower_e_min && std::isfinite(time))
      {
        rtw += e * time;
        rte += e;
      }
    }
  }
  m_jet_time_emraw = wmean(rtw, rte);

  m_tree->Fill();
  return Fun4AllReturnCodes::EVENT_OK;
}

int TimingAna::End(PHCompositeNode * /*topNode*/)
{
  std::cout << "TimingAna: " << m_tree->GetEntries() << " entries from " << m_nevents
            << " events -> " << m_outfilename << std::endl;
  m_outfile->cd();
  m_tree->Write();
  m_outfile->Close();
  return Fun4AllReturnCodes::EVENT_OK;
}
