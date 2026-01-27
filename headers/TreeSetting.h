#ifndef TreeSetting_h
#define TreeSetting_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "TClonesArray.h"
#include "vector"

const int MaxClusters = 10000;
const int MaxJets = 10000;
Int_t           RunNumber;
Float_t         vz;
Bool_t          ScaledTriggerBit[64];
Bool_t          LiveTriggerBit[64];
Int_t           Scaledowns[64];
Bool_t          istriggeredsp;
Bool_t          istriggeredns;
Float_t         mbd_nhits_south;
Float_t         mbd_nhits_north;
Float_t         mbd_time_south;
Float_t         mbd_time_north;
Short_t         nClusters_sp;
Short_t         nClusters_ns;
Short_t         nTruthClusters;
TClonesArray    *cluster_4mom_sp;
TClonesArray    *cluster_4mom_ns;
TClonesArray    *truth_cluster_4mom;
Float_t         cluster_showershape_sp[MaxClusters][14];
Float_t         cluster_showershape_ns[MaxClusters][14];
Float_t         cluster_time_sp[MaxClusters];   //[nClusters_sp]
Float_t         cluster_time_ns[MaxClusters];   //[nClusters_ns]
Float_t         cluster_iso_e02_sp[MaxClusters];   //[nClusters_sp]
Float_t         cluster_iso_e04_sp[MaxClusters];   //[nClusters_sp]
Float_t         cluster_iso_e06_sp[MaxClusters];   //[nClusters_sp]
Float_t         cluster_iso_e08_sp[MaxClusters];   //[nClusters_sp]
Float_t         cluster_bdt_scores[MaxClusters][11];
Short_t         nJets02;
Short_t         nJets04;
Short_t         nJets06;
Short_t         nJets08;
Short_t         nTruthJets02;
Short_t         nTruthJets04;
Short_t         nTruthJets06;
Short_t         nTruthJets08;
TClonesArray    *jet_4mom02;
TClonesArray    *jet_4mom04;
TClonesArray    *jet_4mom06;
TClonesArray    *jet_4mom08;
TClonesArray    *truth_jet_4mom02;
TClonesArray    *truth_jet_4mom04;
TClonesArray    *truth_jet_4mom06;
TClonesArray    *truth_jet_4mom08;
Float_t         jet_emfrac02[MaxJets];   //[nJets]
Float_t         jet_emfrac04[MaxJets];   //[nJets]
Float_t         jet_emfrac06[MaxJets];   //[nJets]
Float_t         jet_emfrac08[MaxJets];   //[nJets]
Float_t         jet_ihfrac02[MaxJets];   //[nJets]
Float_t         jet_ihfrac04[MaxJets];   //[nJets]
Float_t         jet_ihfrac06[MaxJets];   //[nJets]
Float_t         jet_ihfrac08[MaxJets];   //[nJets]
Float_t         jet_ohfrac02[MaxJets];   //[nJets]
Float_t         jet_ohfrac04[MaxJets];   //[nJets]
Float_t         jet_ohfrac06[MaxJets];   //[nJets]
Float_t         jet_ohfrac08[MaxJets];   //[nJets]
Float_t         jet_time02[MaxJets];   //[nJets]
Float_t         jet_time04[MaxJets];   //[nJets]
Float_t         jet_time06[MaxJets];   //[nJets]
Float_t         jet_time08[MaxJets];   //[nJets]

// List of branches
TBranch        *b_RunNumber;   //!
TBranch        *b_vz;   //!
TBranch        *b_ScaledTriggerBit;   //!
TBranch        *b_LiveTriggerBit;   //!
TBranch        *b_Scaledowns;   //!
TBranch        *b_istriggeredsp;   //!
TBranch        *b_istriggeredns;   //!
TBranch        *b_mbd_nhits_south;   //!
TBranch        *b_mbd_nhits_north;   //!
TBranch        *b_mbd_time_south;   //!
TBranch        *b_mbd_time_north;   //!
TBranch        *b_nClusters_sp;   //!
TBranch        *b_nClusters_ns;   //!
TBranch        *b_nTruthClusters;   //!
TBranch        *b_cluster_4mom_sp;   //!
TBranch        *b_cluster_4mom_ns;   //!
TBranch        *b_truth_cluster_4mom;   //!
TBranch        *b_cluster_showershape_sp;   //!
TBranch        *b_cluster_showershape_ns;   //!
TBranch        *b_cluster_time_sp;   //!
TBranch        *b_cluster_time_ns;   //!
TBranch        *b_cluster_iso_e02_sp;   //!
TBranch        *b_cluster_iso_e04_sp;   //!
TBranch        *b_cluster_iso_e06_sp;   //!
TBranch        *b_cluster_iso_e08_sp;   //!
TBranch        *b_cluster_bdt_scores;   //!
TBranch        *b_nJets02;   //!
TBranch        *b_nJets04;   //!
TBranch        *b_nJets06;   //!
TBranch        *b_nJets08;   //!
TBranch        *b_nTruthJets02;   //!
TBranch        *b_nTruthJets04;   //!
TBranch        *b_nTruthJets06;   //!
TBranch        *b_nTruthJets08;   //!
TBranch        *b_jet_4mom02;   //!
TBranch        *b_jet_4mom04;   //!
TBranch        *b_jet_4mom06;   //!
TBranch        *b_jet_4mom08;   //!
TBranch        *b_truth_jet_4mom02;   //!
TBranch        *b_truth_jet_4mom04;   //!
TBranch        *b_truth_jet_4mom06;   //!
TBranch        *b_truth_jet_4mom08;   //!
TBranch        *b_jet_emfrac02;   //!
TBranch        *b_jet_emfrac04;   //!
TBranch        *b_jet_emfrac06;   //!
TBranch        *b_jet_emfrac08;   //!
TBranch        *b_jet_ihfrac02;   //!
TBranch        *b_jet_ihfrac04;   //!
TBranch        *b_jet_ihfrac06;   //!
TBranch        *b_jet_ihfrac08;   //!
TBranch        *b_jet_ohfrac02;   //!
TBranch        *b_jet_ohfrac04;   //!
TBranch        *b_jet_ohfrac06;   //!
TBranch        *b_jet_ohfrac08;   //!
TBranch        *b_jet_time02;   //!
TBranch        *b_jet_time04;   //!
TBranch        *b_jet_time06;   //!
TBranch        *b_jet_time08;   //!
void treesetup(TTree *tree)
{
   // Set object pointer
   cluster_4mom_sp = 0;
   cluster_4mom_ns = 0;
   truth_cluster_4mom = 0;
   jet_4mom02 = 0;
   jet_4mom04 = 0;
   jet_4mom06 = 0;
   jet_4mom08 = 0;
   truth_jet_4mom02 = 0;
   truth_jet_4mom04 = 0;
   truth_jet_4mom06 = 0;
   truth_jet_4mom08 = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;

   tree->SetBranchStatus("cluster_showershape_ns",0);

   tree->SetBranchAddress("RunNumber", &RunNumber, &b_RunNumber);
   tree->SetBranchAddress("vz", &vz, &b_vz);
   tree->SetBranchAddress("ScaledTriggerBit", ScaledTriggerBit, &b_ScaledTriggerBit);
   tree->SetBranchAddress("LiveTriggerBit", LiveTriggerBit, &b_LiveTriggerBit);
   tree->SetBranchAddress("Scaledowns", Scaledowns, &b_Scaledowns);
   tree->SetBranchAddress("istriggeredsp", &istriggeredsp, &b_istriggeredsp);
   tree->SetBranchAddress("istriggeredns", &istriggeredns, &b_istriggeredns);
   tree->SetBranchAddress("mbd_nhits_south", &mbd_nhits_south, &b_mbd_nhits_south);
   tree->SetBranchAddress("mbd_nhits_north", &mbd_nhits_north, &b_mbd_nhits_north);
   tree->SetBranchAddress("mbd_time_south", &mbd_time_south, &b_mbd_time_south);
   tree->SetBranchAddress("mbd_time_north", &mbd_time_north, &b_mbd_time_north);
   tree->SetBranchAddress("nClusters_sp", &nClusters_sp, &b_nClusters_sp);
   tree->SetBranchAddress("nClusters_ns", &nClusters_ns, &b_nClusters_ns);
   tree->SetBranchAddress("nTruthClusters", &nTruthClusters, &b_nTruthClusters);
   tree->SetBranchAddress("cluster_4mom_sp", &cluster_4mom_sp, &b_cluster_4mom_sp);
   tree->SetBranchAddress("cluster_4mom_ns", &cluster_4mom_ns, &b_cluster_4mom_ns);
   tree->SetBranchAddress("truth_cluster_4mom", &truth_cluster_4mom, &b_truth_cluster_4mom);
   tree->SetBranchAddress("cluster_showershape_sp", cluster_showershape_sp, &b_cluster_showershape_sp);
   tree->SetBranchAddress("cluster_showershape_ns", cluster_showershape_ns, &b_cluster_showershape_ns);
   tree->SetBranchAddress("cluster_time_sp", cluster_time_sp, &b_cluster_time_sp);
   tree->SetBranchAddress("cluster_time_ns", cluster_time_ns, &b_cluster_time_ns);
   tree->SetBranchAddress("cluster_iso_e02_sp", cluster_iso_e02_sp, &b_cluster_iso_e02_sp);
   tree->SetBranchAddress("cluster_iso_e04_sp", cluster_iso_e04_sp, &b_cluster_iso_e04_sp);
   tree->SetBranchAddress("cluster_iso_e06_sp", cluster_iso_e06_sp, &b_cluster_iso_e06_sp);
   tree->SetBranchAddress("cluster_iso_e08_sp", cluster_iso_e08_sp, &b_cluster_iso_e08_sp);
   tree->SetBranchAddress("cluster_bdt_scores", cluster_bdt_scores, &b_cluster_bdt_scores);
   tree->SetBranchAddress("nJets02", &nJets02, &b_nJets02);
   tree->SetBranchAddress("nJets04", &nJets04, &b_nJets04);
   tree->SetBranchAddress("nJets06", &nJets06, &b_nJets06);
   tree->SetBranchAddress("nJets08", &nJets08, &b_nJets08);
   tree->SetBranchAddress("nTruthJets02", &nTruthJets02, &b_nTruthJets02);
   tree->SetBranchAddress("nTruthJets04", &nTruthJets04, &b_nTruthJets04);
   tree->SetBranchAddress("nTruthJets06", &nTruthJets06, &b_nTruthJets06);
   tree->SetBranchAddress("nTruthJets08", &nTruthJets08, &b_nTruthJets08);
   tree->SetBranchAddress("jet_4mom02", &jet_4mom02, &b_jet_4mom02);
   tree->SetBranchAddress("jet_4mom04", &jet_4mom04, &b_jet_4mom04);
   tree->SetBranchAddress("jet_4mom06", &jet_4mom06, &b_jet_4mom06);
   tree->SetBranchAddress("jet_4mom08", &jet_4mom08, &b_jet_4mom08);
   tree->SetBranchAddress("truth_jet_4mom02", &truth_jet_4mom02, &b_truth_jet_4mom02);
   tree->SetBranchAddress("truth_jet_4mom04", &truth_jet_4mom04, &b_truth_jet_4mom04);
   tree->SetBranchAddress("truth_jet_4mom06", &truth_jet_4mom06, &b_truth_jet_4mom06);
   tree->SetBranchAddress("truth_jet_4mom08", &truth_jet_4mom08, &b_truth_jet_4mom08);
   tree->SetBranchAddress("jet_emfrac02", jet_emfrac02, &b_jet_emfrac02);
   tree->SetBranchAddress("jet_emfrac04", jet_emfrac04, &b_jet_emfrac04);
   tree->SetBranchAddress("jet_emfrac06", jet_emfrac06, &b_jet_emfrac06);
   tree->SetBranchAddress("jet_emfrac08", jet_emfrac08, &b_jet_emfrac08);
   tree->SetBranchAddress("jet_ihfrac02", jet_ihfrac02, &b_jet_ihfrac02);
   tree->SetBranchAddress("jet_ihfrac04", jet_ihfrac04, &b_jet_ihfrac04);
   tree->SetBranchAddress("jet_ihfrac06", jet_ihfrac06, &b_jet_ihfrac06);
   tree->SetBranchAddress("jet_ihfrac08", jet_ihfrac08, &b_jet_ihfrac08);
   tree->SetBranchAddress("jet_ohfrac02", jet_ohfrac02, &b_jet_ohfrac02);
   tree->SetBranchAddress("jet_ohfrac04", jet_ohfrac04, &b_jet_ohfrac04);
   tree->SetBranchAddress("jet_ohfrac06", jet_ohfrac06, &b_jet_ohfrac06);
   tree->SetBranchAddress("jet_ohfrac08", jet_ohfrac08, &b_jet_ohfrac08);
   tree->SetBranchAddress("jet_time02", jet_time02, &b_jet_time02);
   tree->SetBranchAddress("jet_time04", jet_time04, &b_jet_time04);
   tree->SetBranchAddress("jet_time06", jet_time06, &b_jet_time06);
   tree->SetBranchAddress("jet_time08", jet_time08, &b_jet_time08);
}
#endif
