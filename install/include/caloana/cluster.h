#ifndef CLUSTER_H__
#define CLUSTER_H__

#include <vector>
#include <TLorentzVector.h>

class cluster {
  private:
    TLorentzVector p;
    std::vector<float> showershapes;
    float time;
  public:
    cluster();
    cluster(float pt, float eta, float phi, float e, std::vector<float> ss, float t);
    ~cluster();
    float Pt() { return p.Pt(); }
    float E() { return p.E(); }
    float Eta() { return p.Eta(); }
    float Phi() { return p.Phi(); }
    TLorentzVector fourmom() { return p; }
    float e1t() { return showershapes[0]; }
    float e2t() { return showershapes[1]; }
    float e3t() { return showershapes[2]; }
    float e4t() { return showershapes[3]; }
    float clustereta() { return showershapes[4]; }
    float clusterphi() { return showershapes[5]; }
    float T() { return time; }

    void set_cluster(float pt, float eta, float phi, float e, std::vector<float> ss, float t);
    void set_cluster(TLorentzVector mom, std::vector<float> ss, float t);
    void Clear();
};
    

#endif
