#ifndef __MY_PAT_PHOTON_H__
#define __MY_PAT_PHOTON_H__

#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"

namespace pat {
  
  class MyPATPhoton : public Photon {

    friend class MyPATPhotonSelector;

  public:
    MyPATPhoton() {}
    MyPATPhoton(const Photon& aPhoton);
    virtual ~MyPATPhoton() {}

    /// required reimplementation of the Candidate's clone method
    virtual MyPATPhoton* clone() const { return new MyPATPhoton(*this); }

    // Selection criteria
    bool passADDTightIsolation(void) const { return true; }
    bool passADDLooseIsolation(void) const { return true; }
    bool passADDKinematics(void) const { return true; }
    bool passADDID(void) const { return true; }
    bool isADDTight(void) const { return true; }
    bool isADDLoose(void) const { return true; }

    // stored information
    inline bool validVertex(void) const { return validVertex_; }

  private:
    bool validVertex_;
    std::vector<reco::Conversion> conversions_;
    
  };

}

#endif
