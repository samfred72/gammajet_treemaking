#ifndef JET_H__
#define JET_H__

#include <vector>
#include <TLorentzVector.h>

class jet {
  private:
    TLorentzVector p;
    float emcalfrac;
    float ihcalfrac;
    float ohcalfrac;
    float time;
  public:
    jet();
    jet(float pt, float eta, float phi, float e, float emf, float ihf, float ohf, float t);
    jet(TLorentzVector mom, float emf, float ihf, float ohf, float t);
    ~jet();
    float Pt() { return p.Pt(); }
    float E() { return p.E(); }
    float Eta() { return p.Eta(); }
    float Phi() { return p.Phi(); }
    TLorentzVector fourmom() { return p; }
    float efrac() { return emcalfrac; }
    float ifrac() { return ihcalfrac; }
    float ofrac() { return ohcalfrac; }
    float T() { return time; }

    void set_jet(float pt, float eta, float phi, float e, float emf, float ihf, float ohf, float t);
    void set_jet(TLorentzVector mom, float emf, float ihf, float ohf, float t);
    void Clear();
};
    

#endif
