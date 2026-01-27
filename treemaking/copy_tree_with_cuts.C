#include "TFile.h"
#include "TTree.h"
#include "TObject.h"

void copy_tree_with_cuts(int runnumber, const char * trigger = "MB") {
  // Open the input file
  TFile* input_file;
  if (runnumber != 0) input_file = TFile::Open(Form("/sphenix/tg/tg01/jets/samfred/gammajet_hadded/run%i.root",runnumber));
  else input_file = TFile::Open(Form("/sphenix/tg/tg01/jets/samfred/gammajet_MC_hadded/run28_%s.root",trigger));
  TTree* input_tree = (TTree*)input_file->Get("towerntup");

  // Create a new output file
  TFile* output_file;
  if (runnumber != 0) output_file = new TFile(Form("/sphenix/tg/tg01/jets/samfred/gammajet_short/run%i.root",runnumber), "RECREATE");
  else output_file = new TFile(Form("/sphenix/tg/tg01/jets/samfred/gammajet_MC_smear/run28_%s.root",trigger), "RECREATE");
  

  // Define the selection cut
  TString cut_string = "";

  // Copy the tree with the specified cut
  int m_runnumber=-999;
  float vx, vy, vz;
  bool LiveTriggerBit[64];
  bool ScaledTriggerBit[64];

  static const int Max_photon_size = 10000;
  static const int Max_jet_size = 10000;
  Short_t nClusters_sp = 0;
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
  Short_t nTruthClusters;
  Short_t nTruthJets02;
  Short_t nTruthJets04;
  Short_t nTruthJets06;
  Short_t nTruthJets08;
  TClonesArray * m_truth_cluster_4mom = new TClonesArray("TLorentzVector", Max_photon_size);
  TClonesArray * m_truth_jet_4mom02 = new TClonesArray("TLorentzVector", Max_jet_size);
  TClonesArray * m_truth_jet_4mom04 = new TClonesArray("TLorentzVector", Max_jet_size);
  TClonesArray * m_truth_jet_4mom06 = new TClonesArray("TLorentzVector", Max_jet_size);
  TClonesArray * m_truth_jet_4mom08 = new TClonesArray("TLorentzVector", Max_jet_size);

  TTree* towerntuple = new TTree("towerntup","Ntuple");
  // Global
  towerntuple->Branch("RunNumber",&m_runnumber);
  towerntuple->Branch("vz",&vz);
  towerntuple->Branch("LiveTriggerBit",LiveTriggerBit,"LiveTriggerBit[64]/O");
  towerntuple->Branch("ScaledTriggerBit",ScaledTriggerBit, "ScaledTriggerBit[64]/O");
  towerntuple->Branch("nClusters_sp",&nClusters_sp, "nClusters_sp/S");
  towerntuple->Branch("cluster_4mom_sp","TClonesArray",&m_cluster_4mom_sp,32000,0);
  towerntuple->Branch("cluster_showershape_sp",m_cluster_showershape_sp,"cluster_showershape_sp[nClusters_sp][8]/F");
  towerntuple->Branch("cluster_time_sp",m_cluster_time_sp,"cluster_time_sp[nClusters_sp]/F");
  towerntuple->Branch("cluster_iso_e02_sp",m_cluster_iso_e02_sp,"cluster_iso_e02_sp[nClusters_sp]/F");
  towerntuple->Branch("cluster_iso_e04_sp",m_cluster_iso_e04_sp,"cluster_iso_e04_sp[nClusters_sp]/F");
  towerntuple->Branch("cluster_iso_e06_sp",m_cluster_iso_e06_sp,"cluster_iso_e06_sp[nClusters_sp]/F");
  towerntuple->Branch("cluster_iso_e08_sp",m_cluster_iso_e08_sp,"cluster_iso_e08_sp[nClusters_sp]/F");
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
  if (runnumber == 0) { 
    towerntuple->Branch("nTruthClusters",&nTruthClusters, "nTruthClusters/S");
    towerntuple->Branch("nTruthJets02",&nTruthJets02, "nTruthJets02/S");
    towerntuple->Branch("nTruthJets04",&nTruthJets04, "nTruthJets04/S");
    towerntuple->Branch("nTruthJets06",&nTruthJets06, "nTruthJets06/S");
    towerntuple->Branch("nTruthJets08",&nTruthJets08, "nTruthJets08/S");
    towerntuple->Branch("truth_cluster_4mom","TClonesArray",&m_truth_cluster_4mom,32000,0);
    towerntuple->Branch("truth_jet_4mom02","TClonesArray",&m_truth_jet_4mom02,32000,0);
    towerntuple->Branch("truth_jet_4mom04","TClonesArray",&m_truth_jet_4mom04,32000,0);
    towerntuple->Branch("truth_jet_4mom06","TClonesArray",&m_truth_jet_4mom06,32000,0);
    towerntuple->Branch("truth_jet_4mom08","TClonesArray",&m_truth_jet_4mom08,32000,0);
  }

  input_tree->SetBranchAddress("RunNumber", &m_runnumber);
  input_tree->SetBranchAddress("vz", &vz);
  input_tree->SetBranchAddress("LiveTriggerBit", LiveTriggerBit);
  input_tree->SetBranchAddress("ScaledTriggerBit", ScaledTriggerBit);
  input_tree->SetBranchAddress("nClusters_sp", &nClusters_sp);
  input_tree->SetBranchAddress("cluster_4mom_sp", &m_cluster_4mom_sp);
  input_tree->SetBranchAddress("cluster_showershape_sp", m_cluster_showershape_sp);
  input_tree->SetBranchAddress("cluster_time_sp", m_cluster_time_sp);
  input_tree->SetBranchAddress("cluster_iso_e02_sp", m_cluster_iso_e02_sp);
  input_tree->SetBranchAddress("cluster_iso_e04_sp", m_cluster_iso_e04_sp);
  input_tree->SetBranchAddress("cluster_iso_e06_sp", m_cluster_iso_e06_sp);
  input_tree->SetBranchAddress("cluster_iso_e08_sp", m_cluster_iso_e08_sp);
  input_tree->SetBranchAddress("nJets02", &nJets02);
  input_tree->SetBranchAddress("nJets04", &nJets04);
  input_tree->SetBranchAddress("nJets06", &nJets06);
  input_tree->SetBranchAddress("nJets08", &nJets08);
  input_tree->SetBranchAddress("jet_4mom02", &m_jet_4mom02);
  input_tree->SetBranchAddress("jet_4mom04", &m_jet_4mom04);
  input_tree->SetBranchAddress("jet_4mom06", &m_jet_4mom06);
  input_tree->SetBranchAddress("jet_4mom08", &m_jet_4mom08);
  input_tree->SetBranchAddress("jet_emfrac02", m_jet_emfrac02);
  input_tree->SetBranchAddress("jet_emfrac04", m_jet_emfrac04);
  input_tree->SetBranchAddress("jet_emfrac06", m_jet_emfrac06);
  input_tree->SetBranchAddress("jet_emfrac08", m_jet_emfrac08);
  input_tree->SetBranchAddress("jet_ihfrac02", m_jet_ihfrac02);
  input_tree->SetBranchAddress("jet_ihfrac04", m_jet_ihfrac04);
  input_tree->SetBranchAddress("jet_ihfrac06", m_jet_ihfrac06);
  input_tree->SetBranchAddress("jet_ihfrac08", m_jet_ihfrac08);
  input_tree->SetBranchAddress("jet_ohfrac02", m_jet_ohfrac02);
  input_tree->SetBranchAddress("jet_ohfrac04", m_jet_ohfrac04);
  input_tree->SetBranchAddress("jet_ohfrac06", m_jet_ohfrac06);
  input_tree->SetBranchAddress("jet_ohfrac08", m_jet_ohfrac08);
  input_tree->SetBranchAddress("jet_time02", m_jet_time02);
  input_tree->SetBranchAddress("jet_time04", m_jet_time04);
  input_tree->SetBranchAddress("jet_time06", m_jet_time06);
  input_tree->SetBranchAddress("jet_time08", m_jet_time08);
  if (runnumber == 0) { 
    input_tree->SetBranchAddress("nTruthClusters",&nTruthClusters);
    input_tree->SetBranchAddress("nTruthJets02",&nTruthJets02);
    input_tree->SetBranchAddress("nTruthJets04",&nTruthJets04);
    input_tree->SetBranchAddress("nTruthJets06",&nTruthJets06);
    input_tree->SetBranchAddress("nTruthJets08",&nTruthJets08);
    input_tree->SetBranchAddress("truth_cluster_4mom",&m_truth_cluster_4mom);
    input_tree->SetBranchAddress("truth_jet_4mom02",&m_truth_jet_4mom02);
    input_tree->SetBranchAddress("truth_jet_4mom04",&m_truth_jet_4mom04);
    input_tree->SetBranchAddress("truth_jet_4mom06",&m_truth_jet_4mom06);
    input_tree->SetBranchAddress("truth_jet_4mom08",&m_truth_jet_4mom08);
  }

  for (int i = 0; i < input_tree->GetEntries(); i++) {
    input_tree->GetEntry(i);
    if (i % 1000 == 0) cout <<i << ": " << input_tree->GetEntries() << endl;
    int newncluster = nClusters_sp;
    for (int j = 0; j < nClusters_sp; j++) {
      TLorentzVector pho = *(TLorentzVector*)m_cluster_4mom_sp->At(j);
      if (pho.E() < 7) {
        newncluster--;
        TObject * o = m_cluster_4mom_sp->ConstructedAt(j);
        m_cluster_4mom_sp->Remove(o);
      }
    }
    m_cluster_4mom_sp->Compress();
    if (nJets02 == 0 && nJets04 == 0 && nJets06 == 0 && nJets08 == 0) continue;
    nClusters_sp = newncluster;
    if (nClusters_sp == 0) continue;
    towerntuple->Fill();
  }

  // Write the new tree to the output file and clean up
  output_file->WriteObject(towerntuple, "towerntup");
  cout << "Writing file to: " << output_file->GetName() << endl;

  delete input_tree;
  delete input_file;
  delete towerntuple;
  delete output_file;
}
