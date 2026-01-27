#ifndef ana_h
#define ana_h

#include <TROOT.h>
#include "TClonesArray.h"
#include <TLorentzVector.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <vector>
#include <map>
#include "/sphenix/user/samfred/projects/gammajet/headers/Style_jaebeom.h"
#include "/sphenix/user/samfred/projects/gammajet/headers/commonUtility.h"
#include <TEfficiency.h>
struct jet_object {
  float e = 0;
  float et = 0;
  float pt = 0;
  float eta = 0;
  float phi = 0;
  float t = 0;
  float emfrac = 0;
  float ihfrac = 0;
  float ohfrac = 0;
};
struct pho_object {
  float e = 0;
  float et = 0;
  float pt = 0;
  float eta = 0;
  float phi = 0;
  float t = 0;
  float iso3 = 0;
  float iso4 = 0;
  float bdt = 0;
};
jet_object make_jet(TLorentzVector jet, float efrac, float ifrac, float ofrac, float t) {
  jet_object j;
  j.e = jet.E();
  j.et = jet.E()/TMath::CosH(jet.Eta());
  j.pt = jet.Pt();
  j.eta = jet.Eta();
  j.phi = jet.Phi();
  j.emfrac = efrac;
  j.ihfrac = ifrac;
  j.ohfrac = ofrac;
  j.t = t;
  return j;
}
pho_object make_pho(TLorentzVector pho, float i3, float i4, float t, float bdt) {
  pho_object p;
  p.e = pho.E();
  p.et = pho.E()/TMath::CosH(pho.Eta());
  p.pt = pho.Pt();
  p.eta = pho.Eta();
  p.phi = pho.Phi();
  p.iso3 = i3;
  p.iso4 = i4;
  p.bdt = bdt;
  p.t = t;
  return p;
}


class ana {
  public :
    ana();
    ~ana();

    virtual Bool_t                        PassEtaCut(float eta, float vz); 
    virtual Double_t                      GetShiftedEta(float _vz, float _eta);
    virtual Float_t                       deltaR(float eta1, float eta2, float phi1, float phi2);
    virtual Float_t                       deltaR(TLorentzVector pho1, TLorentzVector pho2);
    virtual Float_t                       deltaR(pho_object obj1, jet_object obj2);
    virtual Float_t                       deltaR(jet_object obj1, pho_object obj2);
    virtual Float_t                       deltaR(jet_object obj1, jet_object obj2);
    virtual Float_t                       deltaR(pho_object obj1, pho_object obj2);
    virtual TH1D *                        combineMC(vector<TH1D*> hists, vector<int> samples, bool isphoton);
    virtual TH2D *                        combineMC(vector<TH2D*> hists, vector<int> samples, bool isphoton);
    virtual void                          drawAll(vector<string> samples, vector<string> features, float drawx, float drawy, int fontsize, bool isbig);
    virtual float                         getPurity(float low, float high);
    virtual vector<vector<vector<TH1D*>>> collect_hists(vector<TFile*> files, vector<int> samples, const char * histname, int ptbins, int jbins, bool isphoton);

    float sPHENIX_posx = 0.6;
    float sPHENIX_posy = 0.85;
    float posy_diff = 0.05;

    static const int nDims=7;
    static const int NHIST = nDims*nDims;
    const int gridSize = 7;
    const double vzcut = 60; 
    const double oppnum = 7;
    const double oppden = 8;
    const double oppcut = oppnum*M_PI/oppden;
    const double tcut = 4;
    const double tlowcut = 0;
    const double thighcut = 4;
    const double drcut = 0.4;
    const double radius = 93; 
    const double etacut = 1.1;
    double etamin = -etacut;
    double etamax = etacut;
    const double minclustere = 7;
    const float minjete02 =3;// 6.6;
    const float minjete04 =3;// 7.6;
    const float minjete06 =3;// 8;
    const float minjete08 =3;// 12;

    static const int nPtBins = 6;
    static constexpr double ptBins[nPtBins+1] = {10,11,12,13,15,19,30};
    static const int nabcdbins = 20;
    static constexpr double abcdbins[nabcdbins+1] = {10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30};
    virtual Int_t findPtBin(double value);
    virtual Int_t findabcdBin(double value);
    static const int nJetR = 4;
    static constexpr double JetRs[nJetR] = {0.2, 0.4, 0.6, 0.8};
    
    map<bool,map<int,double>> scalemap = {
      {0,{{5,1.369e+08},{10,3.997e+06},{15,4.073e+05},{20,6.218e+04},{30,2.502e+03},{50,7.2695},{70,1.034e-02}}},
      {1,{{5,146359.3},{10,6944.675},{20,130.4461}}}
    };


  private:
};

#endif
