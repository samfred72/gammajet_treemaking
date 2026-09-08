#define ana_cxx
#include "ana.h"
#include "/sphenix/user/samfred/projects/gammajet/headers/commonUtility.h"
ana::ana() {
}

ana::~ana() {
  // destructor implementation
}


Bool_t ana::PassEtaCut(float eta, float vz = 0)
{
  float loweta = GetShiftedEta(vz,etamin);
  float higheta = GetShiftedEta(vz,etamax);
  if (eta < etamin || eta > etamax) return false;
  else return true;
}

Double_t ana::GetShiftedEta(float _vz, float _eta)
{
  double theta = 2*atan(exp(-_eta));
  double z = radius / tan(theta);
  double zshifted = z - _vz;
  double thetashifted = atan2(radius,zshifted);
  double etashifted = -log(tan(thetashifted/2.0));
  return etashifted;
}

Float_t ana::deltaR(TLorentzVector pho1, TLorentzVector pho2)
{
  float phi1 = pho1.Phi();
  float phi2 = pho2.Phi();
  float eta1 = pho1.Eta();
  float eta2 = pho2.Eta();
  float dphi = (fabs(phi1-phi2) > M_PI) ? 2*M_PI - fabs(phi1-phi2) : fabs(phi1-phi2);
  float dR = sqrt((eta2-eta1) * (eta2-eta1) + dphi*dphi);
  return dR;
}
Float_t ana::deltaR(pho_object obj1, jet_object obj2)
{
  float phi1 = obj1.phi;
  float phi2 = obj2.phi;
  float eta1 = obj1.eta;
  float eta2 = obj2.eta;
  float dphi = (fabs(phi1-phi2) > M_PI) ? 2*M_PI - fabs(phi1-phi2) : fabs(phi1-phi2);
  float dR = sqrt((eta2-eta1) * (eta2-eta1) + dphi*dphi);
  return dR;
}
Float_t ana::deltaR(jet_object obj1, pho_object obj2)
{
  float phi1 = obj1.phi;
  float phi2 = obj2.phi;
  float eta1 = obj1.eta;
  float eta2 = obj2.eta;
  float dphi = (fabs(phi1-phi2) > M_PI) ? 2*M_PI - fabs(phi1-phi2) : fabs(phi1-phi2);
  float dR = sqrt((eta2-eta1) * (eta2-eta1) + dphi*dphi);
  return dR;
}
Float_t ana::deltaR(pho_object obj1, pho_object obj2)
{
  float phi1 = obj1.phi;
  float phi2 = obj2.phi;
  float eta1 = obj1.eta;
  float eta2 = obj2.eta;
  float dphi = (fabs(phi1-phi2) > M_PI) ? 2*M_PI - fabs(phi1-phi2) : fabs(phi1-phi2);
  float dR = sqrt((eta2-eta1) * (eta2-eta1) + dphi*dphi);
  return dR;
}
Float_t ana::deltaR(jet_object obj1, jet_object obj2)
{
  float phi1 = obj1.phi;
  float phi2 = obj2.phi;
  float eta1 = obj1.eta;
  float eta2 = obj2.eta;
  float dphi = (fabs(phi1-phi2) > M_PI) ? 2*M_PI - fabs(phi1-phi2) : fabs(phi1-phi2);
  float dR = sqrt((eta2-eta1) * (eta2-eta1) + dphi*dphi);
  return dR;
}

Float_t ana::deltaR(float eta1, float eta2, float phi1, float phi2)
{
  float dphi = (fabs(phi1-phi2) > M_PI) ? 2*M_PI - fabs(phi1-phi2) : fabs(phi1-phi2);
  float dR = sqrt((eta2-eta1) * (eta2-eta1) + dphi*dphi);
  return dR;
}

Int_t ana::findPtBin(double value)
{
  for (int i = 0; i < nPtBins; ++i) {
    if (value >= ptBins[i] && value < ptBins[i + 1]) {
      return  i;
    }
  }
  return -1;
}

Int_t ana::findabcdBin(double value)
{
  for (int i = 0; i < nabcdbins; ++i) {
    if (value >= abcdbins[i] && value < abcdbins[i + 1]) {
      return  i;
    }
  }
  if (value > abcdbins[nabcdbins-1]) return nabcdbins-1;
  else return -1;
}

TH2D * ana::combineMC(vector<TH2D*> hists, vector<int> samples, bool isphoton) {
  if (hists.size() != samples.size()) {
    cout << "Incompatible sizes!" << endl;
    return nullptr;
  }
  if (hists.size() == 0) return nullptr;
  TH2D * thehist = (TH2D*)hists.at(0)->Clone();
  thehist->Reset("ICES");
  for (int i = 0; i < hists.size(); i++) {
    TH1D * hist = (TH1D*)hists.at(i)->Clone();
    hist->SetName(Form("h%i",i));
    hist->Scale(scalemap[isphoton][samples.at(i)]);
    thehist->Add(hist);
  }
  return thehist;
}

TH1D * ana::combineMC(vector<TH1D*> hists, vector<int> samples, bool isphoton) {
  if (hists.size() != samples.size()) {
    cout << "Incompatible sizes!" << endl;
    return nullptr;
  }
  if (hists.size() == 0) return nullptr;
  TH1D * thehist = (TH1D*)hists.at(0)->Clone();
  thehist->Reset("ICES");
  for (int i = 0; i < hists.size(); i++) {
    TH1D * hist = (TH1D*)hists.at(i)->Clone();
    hist->SetName(Form("h%i",i));
    hist->Scale(scalemap[isphoton][samples.at(i)]);
    thehist->Add(hist);
  }
  return thehist;
}

void ana::drawAll(vector<string> samples, vector<string> features, float drawx, float drawy, int fontsize, bool isbig = 0) {
  float titlescale = (isbig ? 1.25 : 1.5);
  float subtitlescale = 1.25;
  float ydiff = (isbig ? fontsize * 0.00067 : fontsize * 0.002);
  drawText("#bf{#it{sPHENIX}} Internal",drawx,drawy,1,(int)fontsize*titlescale);
  for (int i = 0; i < samples.size(); i++) {
    drawText(samples.at(i).c_str(),drawx,drawy-ydiff*subtitlescale*(i+1),1,(int)fontsize*subtitlescale);
  }
  for (int i = 0; i < features.size(); i++) {
    drawText(features.at(i).c_str(),drawx,drawy-ydiff*subtitlescale*samples.size()-ydiff*(i+1),1,fontsize);
  }
}

float ana::getPurity(float low, float high) {
  TFile * f = TFile::Open("/sphenix/user/shuhangli/ppg12/efficiencytool/results/Photon_final_bdt_base_v3E_2.root");
  TF1 * func = (TF1*)f->Get("f_purity_fit");
  float val = func->Integral(low,high)/(high-low);
  return val;
}

vector<vector<vector<vector<TH1D*>>>> ana::collect_hists(vector<TFile*> files, vector<int> samples, const char * histname, int ptbins, int jbins, bool isphoton = 0) {
  vector<vector<vector<vector<TH1D*>>>> hists(ptbins, vector<vector<vector<TH1D*>>>(jbins, vector<vector<TH1D*>>(nIsoBdtBins, vector<TH1D*>(4))));
  if (files.size() == 0) {
    cout << "Collecting hists requires at least one file!" << endl;
    return hists;
  }
  
  int nrebin = 4000;
  int njrebin = 1;
  int nprebin = 1;

  if (samples.size() == 0) { // its data!
    for (int i = 0; i < ptbins; i++) {
      for (int j = 0; j < jbins; j++) {
        for (int l = 0; l < nIsoBdtBins; l++) {
          for (int k = 0; k < 4; k++) {
            hists[i][j][l][k] = (TH1D*)files[0]->Get(Form("%s_%i_%i_%i_%i", histname,i,j,l,k));
            hists[i][j][l][k]->Rebin(nrebin); 
          }
        }
      }
    }
  }
  else {
    int rebin = (isphoton ? nprebin : njrebin);
    for (int i = 0; i < ptbins; i++) {
      for (int j = 0; j < jbins; j++) {
        for (int l = 0; l < nIsoBdtBins; l++) {
          for (int k = 0; k < 4; k++) {
            // Photon samples 
            vector<TH1D*> mchists;
            for (int ifile = 0; ifile < files.size(); ifile++) {
              mchists.push_back((TH1D*)files[ifile]->Get(Form("%s_%i_%i_%i_%i",histname,i,j,l,k)));
              mchists[ifile]->SetName(Form("hratio_%i_%i_%i_%i_%i",i,j,k,ifile,isphoton));
              mchists[ifile]->Rebin(nrebin);
            }
            hists[i][j][l][k] = combineMC(mchists,samples,isphoton);
            hists[i][j][l][k]->Rebin(rebin);
          }
        }
      }
    }
  }
  return hists;
}

void ana::drawEvent(TClonesArray * photons, TClonesArray * jets, int npho, int njet, int i) {
  TCanvas * c = new TCanvas(Form("c%i",i),"",400,900);

  jet_object maxjet;
  for (int i = 0; i < njet; i++) {
    TLorentzVector jet = *(TLorentzVector*)jets->At(i);
    jet_object obj = make_jet(jet,0,0,0,0);
    TMarker* jet1 = new TMarker(obj.eta,obj.phi, 24);
    jet1->SetMarkerSize(14);
    jet1->SetMarkerColor(kBlack);
    jet1->Draw();
    if (obj.pt > maxjet.pt) maxjet = obj;
  }
  TMarker* jet2 = new TMarker(maxjet.eta,maxjet.phi, 20);
  jet2->SetMarkerSize(14);
  jet2->SetMarkerColorAlpha(kRed,.8);
  jet2->Draw();

  pho_object maxpho;
  for (int i = 0; i < npho; i++) {
    TLorentzVector pho = *(TLorentzVector*)photons->At(i);
    pho_object obj = make_pho(pho,0,0,0,0);
    TMarker* star1 = new TMarker(obj.eta,obj.phi, 29);
    star1->SetMarkerSize(1.5);
    star1->SetMarkerColor(kBlue);
    star1->Draw();
    if (obj.pt > maxpho.pt) maxpho = obj;
  }
  TMarker* star2 = new TMarker(maxpho.eta,maxpho.phi, 29);
  star2->SetMarkerSize(1.4);
  star2->SetMarkerColor(kGreen);
  star2->Draw();
}
