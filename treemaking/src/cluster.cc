#include "cluster.h"
cluster::cluster() {
}
cluster::cluster(float pt, float eta, float phi, float e, std::vector<float> ss, float t) {
  set_cluster(pt,eta,phi,e,ss,t);
}
cluster::~cluster() {
}

void cluster::set_cluster(float pt, float eta, float phi, float e, std::vector<float> ss, float t) {
  p.SetPtEtaPhiE(pt,eta,phi,e);
  showershapes = ss;
  time = t;
}
void cluster::set_cluster(TLorentzVector mom, std::vector<float> ss, float t) {
  p = mom;
  showershapes = ss;
  time = t;
}
void cluster::Clear() {
  p.Clear();
  showershapes.clear();
  time = 0;
}
