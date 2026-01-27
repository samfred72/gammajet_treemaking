#include "jet.h"
jet::jet() {
}
jet::jet(float pt, float eta, float phi, float e, float emf, float ihf, float ohf, float t) {
  set_jet(pt,eta,phi,e,emf,ihf,ohf,t);
}
jet::jet(TLorentzVector mom, float emf, float ihf, float ohf, float t) {
  set_jet(mom,emf,ihf,ohf,t);
}
jet::~jet() {
}
void jet::set_jet(float pt, float eta, float phi, float e, float emf, float ihf, float ohf, float t) {
  p.SetPtEtaPhiE(pt,eta,phi,e);
  emcalfrac = emf;
  ihcalfrac = ihf;
  ohcalfrac = ohf;
  time = t;
}
void jet::set_jet(TLorentzVector mom, float emf, float ihf, float ohf, float t) {
  p = mom;
  emcalfrac = emf;
  ihcalfrac = ihf;
  ohcalfrac = ohf;
  time = t;
}
void jet::Clear() {
  p.Clear();
  emcalfrac = 0;
  ihcalfrac = 0;
  ohcalfrac = 0;
  time = 0;
}
