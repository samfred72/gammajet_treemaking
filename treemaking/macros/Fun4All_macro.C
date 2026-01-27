#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fstream>
#include <filesystem>
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <phool/recoConsts.h>
#include <fun4all/Fun4AllNoSyncDstInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllUtils.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>

#include <G4_Global.C>
#include <GlobalVariables.C>
#include <mbd/MbdReco.h>
#include <zdcinfo/ZdcReco.h>
#include <globalvertex/GlobalVertexReco.h>
#include <caloreco/CaloTowerBuilder.h>
#include <caloreco/CaloWaveformProcessing.h>
//
//#include <calotowerbuilder/CaloTowerBuilder.h>
//
#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>
#include <ffamodules/CDBInterface.h>

//#include <caloana/ClusterIso.h>
#include <caloreco/CaloTowerBuilder.h>
#include <caloreco/CaloTowerCalib.h>
#include <caloreco/CaloTowerStatus.h>
#include <caloreco/CaloWaveformProcessing.h>
#include <caloreco/DeadHotMapLoader.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <caloreco/RawClusterDeadHotMask.h>
#include <caloreco/RawClusterPositionCorrection.h>
#include <caloreco/TowerInfoDeadHotMask.h>
#include <caloreco/PhotonClusterBuilder.h>
#include <clusteriso/ClusterIso.h>

#include <jetbase/JetReco.h>
#include <jetbase/TowerJetInput.h>
#include <jetbase/FastJetAlgo.h>
#include <jetbackground/CopyAndSubtractJets.h>
#include <jetbackground/DetermineTowerBackground.h>
#include <jetbackground/FastJetAlgoSub.h>
#include <jetbackground/RetowerCEMC.h>
#include <jetbackground/SubtractTowers.h>
#include <jetbackground/SubtractTowersCS.h>

// #include <runtowerinfo/RunTowerInfo.h>
#include <caloana/CaloAna.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <mbd/MbdPmtContainer.h>
#include <mbd/MbdPmtContainerV1.h>
#include <globalvertex/MbdVertexMap.h>
#include <globalvertex/GlobalVertexMap.h>
#include <globalvertex/GlobalVertexReco.h>
#include <mbd/MbdReco.h>
#include <phool/getClass.h>
#include <phool/PHCompositeNode.h>
#include <g4centrality/PHG4CentralityReco.h>

#include <centrality/CentralityReco.h>
#include <calotrigger/MinimumBiasClassifier.h>

#include "HIJetReco.C"
#include <Calo_Calib.C>

#include <sstream>
#include <fstream>
#include <string>
#include <TSQLServer.h>
#include <TSQLResult.h>
#include <TSQLRow.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libcaloana.so)
R__LOAD_LIBRARY(libmbd.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libg4vertex.so)
R__LOAD_LIBRARY(libglobalvertex.so)
R__LOAD_LIBRARY(libzdcinfo.so)
R__LOAD_LIBRARY(libjetbase.so)
R__LOAD_LIBRARY(libg4jets.so)
R__LOAD_LIBRARY(libjetbackground.so)
R__LOAD_LIBRARY(libclusteriso.so)
R__LOAD_LIBRARY(libg4centrality.so)
R__LOAD_LIBRARY(libcentrality.so)
R__LOAD_LIBRARY(libcalotrigger.so)
R__LOAD_LIBRARY(libFROG.so)


#endif

void get_scaledowns(int runnumber, int scaledowns[])
{

  TSQLServer *db = TSQLServer::Connect("pgsql://sphnxdaqdbreplica:5432/daq","phnxro","");
  if (db) {
    printf("Server info: %s\n", db->ServerInfo());
  }
  else {
    printf("bad\n");
  }
  TSQLRow *row;
  TSQLResult *res;
  TString cmd = "";
  for (int is = 0; is < 64; is++) {
    const char * sql = Form("select scaledown%02d from gl1_scaledown where runnumber = %d;", is, runnumber);
    res = db->Query(sql);
    int nrows = res->GetRowCount();
    int nfields = res->GetFieldCount();
    for (int i = 0; i < nrows; i++) {
      row = res->Next();
      for (int j = 0; j < nfields; j++) {
        scaledowns[is] = stoi(row->GetField(j));
      }
      delete row;
    }
    delete res;
  }
  delete db;
}


void Fun4All_macro(const char* infile="/sphenix/user/samfred/projects/filelists/goldenruns_pp24/queue_53300_v0000.list", bool test=true)
{

    //=====================
    // Filename management
    //=====================

    string outdir = "/sphenix/tg/tg01/jets/samfred/gammajet_calojet";
    void * dirf = gSystem->OpenDirectory(outdir.c_str());
    if(dirf) gSystem->FreeDirectory(dirf);
    else {gSystem->mkdir(outdir.c_str(), kTRUE);}

    std::cout << "infile : " << infile <<std::endl;
    std::ifstream infilestream(infile);
    
    if (!infilestream.is_open()) {
      std::cerr << "Error opening file: " << infile << std::endl;
      return;
    }

    std::string firstfilename;
    if (infilestream >> firstfilename) {
      std::cout << "First filename: " << firstfilename << std::endl;
    } else {
      std::cerr << "Error reading first filename or file is empty!" << std::endl;
      return;
    }
    infilestream.clear();
    infilestream.seekg(0, std::ios::beg); 

    std::filesystem::path p(infile);
    std::string fname = p.filename().stem().string();
    string outfile = "";
    if (test) {
      outfile = "/sphenix/user/samfred/projects/gammajet/treemaking/macros/testtree.root";
    }
    else {
      outfile = Form("%s/outtree_%s.root",outdir.c_str(),fname.c_str());
    }

    Fun4AllServer *se = Fun4AllServer::instance();
    int verbosity = 0;

    se->Verbosity(verbosity);
    recoConsts *rc = recoConsts::instance();

    pair<int, int> runseg = Fun4AllUtils::GetRunSegment(firstfilename.c_str());
    int runnumber = runseg.first;
    int segment = runseg.second;

    //=====================
    // conditions DB flags
    //=====================

    // global tag
    rc->set_StringFlag("CDB_GLOBALTAG","ProdA_2024"); 
    rc->set_uint64Flag("TIMESTAMP",runnumber);
    // scaledowns
    int m_scaledowns[64];
    get_scaledowns(runnumber,m_scaledowns);
    

    //=====================
    // Global reco
    //=====================
    MbdReco *mbdreco = new MbdReco();
    se->registerSubsystem(mbdreco);
    GlobalVertexReco *gvertex = new GlobalVertexReco();
    se->registerSubsystem(gvertex);
    
    //====================
    // Calo Calib
    //====================
    Process_Calo_Calib();
    //Mother cluster
    std::string emc_prof = getenv("CALIBRATIONROOT");
    emc_prof += "/EmcProfile/CEMCprof_Thresh30MeV.root";
    RawClusterBuilderTemplate *ClusterBuilderMother = new RawClusterBuilderTemplate("EmcRawClusterBuilderTemplateMother");
    ClusterBuilderMother->Detector("CEMC");
    ClusterBuilderMother->set_threshold_energy(0.070);  // for when using basic calibration
    ClusterBuilderMother->LoadProfile(emc_prof);
    ClusterBuilderMother->set_UseTowerInfo(1);  // to use towerinfo objects rather than old RawTower
    ClusterBuilderMother->setOutputClusterNodeName("CEMC_CLUSTERINFO_MOTHER");
    ClusterBuilderMother->setSubclusterSplitting(false);
    ClusterBuilderMother->Verbosity(1);
    se->registerSubsystem(ClusterBuilderMother);

    //====================
    // Photon reco
    //====================
    PhotonClusterBuilder *photon0 = new PhotonClusterBuilder();
    PhotonClusterBuilder *photon1 = new PhotonClusterBuilder();
    PhotonClusterBuilder *photon2 = new PhotonClusterBuilder();
    PhotonClusterBuilder *photon3 = new PhotonClusterBuilder();
    PhotonClusterBuilder *photon4 = new PhotonClusterBuilder();
    PhotonClusterBuilder *photon5 = new PhotonClusterBuilder();
    PhotonClusterBuilder *photon6 = new PhotonClusterBuilder();
    PhotonClusterBuilder *photon7 = new PhotonClusterBuilder();
    PhotonClusterBuilder *photon8 = new PhotonClusterBuilder();
    PhotonClusterBuilder *photon9 = new PhotonClusterBuilder();
    PhotonClusterBuilder *photon10 = new PhotonClusterBuilder();
    PhotonClusterBuilder * photon[11] = {photon0,photon1,photon2,photon3,photon4,photon5,photon6,photon7,photon8,photon9,photon10};

    string paths[11] = {
      "/sphenix/user/shuhangli/ppg12/FunWithxgboost/binned_models/model_base_single_tmva.root",
      "/sphenix/user/shuhangli/ppg12/FunWithxgboost/binned_models/model_base_v0_single_tmva.root",
      "/sphenix/user/shuhangli/ppg12/FunWithxgboost/binned_models/model_base_v1_single_tmva.root",
      "/sphenix/user/shuhangli/ppg12/FunWithxgboost/binned_models/model_base_v2_single_tmva.root",
      "/sphenix/user/shuhangli/ppg12/FunWithxgboost/binned_models/model_base_v3_single_tmva.root",
      "/sphenix/user/shuhangli/ppg12/FunWithxgboost/binned_models/model_base_E_single_tmva.root",
      "/sphenix/user/shuhangli/ppg12/FunWithxgboost/binned_models/model_base_v0E_single_tmva.root",
      "/sphenix/user/shuhangli/ppg12/FunWithxgboost/binned_models/model_base_v1E_single_tmva.root",
      "/sphenix/user/shuhangli/ppg12/FunWithxgboost/binned_models/model_base_v2E_single_tmva.root",
      "/sphenix/user/shuhangli/ppg12/FunWithxgboost/binned_models/model_base_v3E_single_tmva.root",
      "/sphenix/user/shuhangli/ppg12/FunWithxgboost/binned_models/model_insitu_E_single_tmva.root",
    };
    vector<vector<string>> features = {
      {"vertex_z","cluster_eta","e11_over_e33","et1","et2","et3","et4"},
      {"vertex_z","cluster_eta","e11_over_e33","et2","et3","et4"},
      {"weta_cogx","vertex_z","cluster_eta","e11_over_e33","et1","et2","et3","et4"},
      {"weta_cogx","wphi_cogx","vertex_z","cluster_eta","e11_over_e33","et1","et2","et3","et4"},
      {"weta_cogx","wphi_cogx","vertex_z","cluster_eta","e11_over_e33","et1","et2","et3","et4","e32_over_e35"},
      {"ET","vertex_z","cluster_eta","e11_over_e33","et1","et2","et3","et4"},
      {"ET","vertex_z","cluster_eta","e11_over_e33","et2","et3","et4"},
      {"ET","weta_cogx","vertex_z","cluster_eta","e11_over_e33","et1","et2","et3","et4"},
      {"ET","weta_cogx","wphi_cogx","vertex_z","cluster_eta","e11_over_e33","et1","et2","et3","et4"},
      {"ET","weta_cogx","wphi_cogx","vertex_z","cluster_eta","e11_over_e33","et1","et2","et3","et4","e32_over_e35"},
      {"ET","weta_cogx","wphi_cogx","vertex_z","cluster_eta","e11_over_e33","et1","et2","et3","et4","e32_over_e35","w32","w52","w72","e11_over_e22","e11_over_e13","e11_over_e15","e11_over_e17","e11_over_e31","e11_over_e51","e11_over_e71","e22_over_e33","e22_over_e35","e22_over_e37","e22_over_e53"}
    };
    for (int i = 0; i < 11; i++) {
      photon[i]->set_output_photon_node(Form("PHOTONCLUSTER_CEMC%i",i));
      photon[i]->set_do_bdt(true);
      photon[i]->set_bdt_model_file(paths[i].c_str());
      photon[i]->set_bdt_feature_list(features[i]);
      photon[i]->set_ET_threshold(5.0);
      se->registerSubsystem(photon[i]);
    }

    //====================
    // Cluster Iso
    //====================
    ClusterIso *cliso2 = new ClusterIso("ClusterIso2",1,2,false,true);
    ClusterIso *cliso4 = new ClusterIso("ClusterIso4",1,4,false,true);
    cliso2->set_cluster_node_name("PHOTONCLUSTER_CEMC0");
    cliso4->set_cluster_node_name("PHOTONCLUSTER_CEMC0");
    cliso2->set_use_towerinfo(true);
    cliso4->set_use_towerinfo(true);
    se->registerSubsystem( cliso2 );
    se->registerSubsystem( cliso4 );

    //====================
    // Jet reco
    //====================
    std::vector<float> doUnsubJet_radius = {0.2,0.4,0.6,0.8};

    // retowering
    std::string jetreco_input_prefix = "TOWERINFO_CALIB";
    RetowerCEMC *_retowerCEMC;
    _retowerCEMC = new RetowerCEMC();
    _retowerCEMC->Verbosity(verbosity);
    _retowerCEMC->set_towerinfo(true);
    _retowerCEMC->set_frac_cut(0.5); //fraction of retower that must be masked to mask the full retower
    _retowerCEMC->set_towerNodePrefix(jetreco_input_prefix);
    se->registerSubsystem(_retowerCEMC);

    // Jet reco
    JetReco *_jetRecoUnsub = new JetReco();
    _jetRecoUnsub->add_input(new TowerJetInput(Jet::CEMC_TOWERINFO_RETOWER, jetreco_input_prefix));
    _jetRecoUnsub->add_input(new TowerJetInput(Jet::HCALIN_TOWERINFO, jetreco_input_prefix));
    _jetRecoUnsub->add_input(new TowerJetInput(Jet::HCALOUT_TOWERINFO, jetreco_input_prefix));
    for (int ir = 0; ir < doUnsubJet_radius.size(); ++ir) {
      _jetRecoUnsub->add_algo(new FastJetAlgoSub(Jet::ANTIKT, doUnsubJet_radius[ir]), "AntiKt_unsubtracted_r0" + std::to_string((int)(10*doUnsubJet_radius[ir])));
    }
    _jetRecoUnsub->set_algo_node("ANTIKT");
    _jetRecoUnsub->set_input_node("TOWER");
    _jetRecoUnsub->Verbosity(verbosity);
    se->registerSubsystem(_jetRecoUnsub);
    

    //======================
    // File inputs
    //======================

    Fun4AllInputManager *intower = new Fun4AllDstInputManager("DST_TOWERS");
    std::string infilename;
    while(infilestream >> infilename){
      intower->AddFile(infilename.c_str());
      std::cout << "input file : " << infilename.c_str() << std::endl;
    }
    se->registerInputManager(intower);

    
    //======================
    // Calo Ana
    //======================
     
    CaloAna *ca = new CaloAna("caloana",outfile.c_str());
    ca->SetMbdZVtxCut(999);
    ca->SetRunNumber(runnumber);
    ca->SetScaledowns(m_scaledowns);
    se->registerSubsystem(ca);

    std::cout << "now run..." << std::endl;
    if (test) {
      se->run(1000);
    }
    else {
      se->run();
    }
    se->End();
    std::cout << "ok done.. " << std::endl;
    std::cout << "Written to " << outfile << std::endl;

    delete se;
}
