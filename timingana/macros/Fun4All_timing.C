// Jet-timing study: same calo calibration and jet reconstruction as
// treemaking/macros/Fun4All_macro.C (photon, isolation and topo-cluster modules dropped),
// one radius only, with TimingAna storing the leading jet's tower-level timing.
//
// timecalib = true adds Dading Chen's tower-time calibration as a sidecar after the standard
// Process_Calo_Calib (caloreco/CaloTowerTimeCalibration, already in the release; talks at
// indico.bnl.gov/event/33579 and /34156). It writes TOWERINFO_CALIB_TIMING_<det> with the
// standard energies and corrected times, which TimingAna then reads. Payloads exist only for
// his 284 calibrated runs (ppRun2024_MB_triggered_ihcal_time_stat_4000_cuts.txt).
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllUtils.h>
#include <phool/recoConsts.h>
#include <ffamodules/CDBInterface.h>

#include <GlobalVariables.C>
#include <Calo_Calib.C>
#include <mbd/MbdReco.h>
#include <globalvertex/GlobalVertexReco.h>

#include <jetbase/JetReco.h>
#include <jetbase/TowerJetInput.h>
#include <jetbase/JetCalib.h>
#include <jetbackground/FastJetAlgoSub.h>
#include <jetbackground/RetowerCEMC.h>

#include <timingana/TimingAna.h>
#include <caloreco/CaloTowerTimeCalibration.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libmbd.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libglobalvertex.so)
R__LOAD_LIBRARY(libjetbase.so)
R__LOAD_LIBRARY(libjetbackground.so)
R__LOAD_LIBRARY(libtimingana.so)
#endif

void Fun4All_timing(const char *dst = "DST_JETCALO_run2pp_ana521_2025p007_v001-00047289-00000.root",
                    const char *outfile = "timing.root", float radius = 0.4, int nevents = 0,
                    bool timecalib = false)
{
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);
  recoConsts *rc = recoConsts::instance();
  CDBInterface::instance()->Verbosity(1);

  std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(dst);
  int runnumber = runseg.first;
  std::cout << "DST " << dst << " -> run " << runnumber << " segment " << runseg.second << std::endl;

  rc->set_StringFlag("CDB_GLOBALTAG", "ProdA_2024");
  rc->set_uint64Flag("TIMESTAMP", runnumber);

  se->registerSubsystem(new MbdReco());
  se->registerSubsystem(new GlobalVertexReco());

  Process_Calo_Calib();

  if (timecalib)
  {
    // His run-by-run payloads are not in the ProdA_2024 global tag: pass each file explicitly.
    const std::string dir = "/sphenix/user/dading/my_tower_time_calibrations/macro/timing_cdb";
    auto addTimeCalib = [&](CaloTowerDefs::DetectorSystem det, const char *detname)
    {
      std::string payload = dir + "/" + detname + "_timeCalib_run" + std::to_string(runnumber) + ".root";
      if (gSystem->AccessPathName(payload.c_str()))
      {
        std::cerr << "No tower-time payload for run " << runnumber << ": " << payload << std::endl;
        gSystem->Exit(1);
      }
      CaloTowerTimeCalibration *tc = new CaloTowerTimeCalibration(std::string("TimeCalib_") + detname);
      tc->set_detector_type(det);
      tc->set_directURL_timeCorrection(payload);
      se->registerSubsystem(tc);
    };
    addTimeCalib(CaloTowerDefs::CEMC, "CEMC");
    addTimeCalib(CaloTowerDefs::HCALIN, "HCALIN");
    addTimeCalib(CaloTowerDefs::HCALOUT, "HCALOUT");
  }

  std::string prefix = "TOWERINFO_CALIB";
  RetowerCEMC *retower = new RetowerCEMC();
  retower->set_towerinfo(true);
  retower->set_frac_cut(0.5);
  retower->set_towerNodePrefix(prefix);
  se->registerSubsystem(retower);

  std::string jetnode = Form("AntiKt_unsubtracted_r0%i", (int) std::lround(radius * 10));
  JetReco *jetreco = new JetReco();
  jetreco->add_input(new TowerJetInput(Jet::CEMC_TOWERINFO_RETOWER, prefix));
  jetreco->add_input(new TowerJetInput(Jet::HCALIN_TOWERINFO, prefix));
  jetreco->add_input(new TowerJetInput(Jet::HCALOUT_TOWERINFO, prefix));
  jetreco->add_algo(new FastJetAlgoSub(Jet::ANTIKT, radius), jetnode);
  jetreco->set_algo_node("ANTIKT");
  jetreco->set_input_node("TOWER");
  se->registerSubsystem(jetreco);

  JetCalib *jetcalib = new JetCalib(Form("JetCalib0%i", (int) std::lround(radius * 10)));
  jetcalib->set_InputNode(jetnode);
  jetcalib->set_OutputNode(jetnode + "_calib");
  jetcalib->set_JetRadius(radius);
  jetcalib->set_ApplyResidualCalib(true);
  se->registerSubsystem(jetcalib);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DST_TOWERS");
  in->AddFile(dst);
  se->registerInputManager(in);

  TimingAna *ta = new TimingAna("TimingAna", outfile);
  ta->SetRunNumber(runnumber);
  ta->SetJetRadius(radius);
  if (timecalib) ta->SetTimeNodePrefix("TOWERINFO_CALIB_TIMING");
  se->registerSubsystem(ta);

  se->run(nevents);
  se->End();
  std::cout << "Written to " << outfile << std::endl;
  delete se;
}
