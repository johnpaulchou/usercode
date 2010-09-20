#ifndef __MY_PAT_MUON_H__
#define __MY_PAT_MUON_H__

#include "DataFormats/PatCandidates/interface/Muon.h"

namespace pat {

  class MyPATMuon : public Muon {
    
    friend class MyPATMuonSelector;

  public:
    MyPATMuon();
    MyPATMuon(const Muon& aMuon);
    virtual ~MyPATMuon();

    /// required reimplementation of the Candidate's clone method
    virtual MyPATMuon* clone() const { return new MyPATMuon(*this); }
    
    // Selection criteria
    bool passTopDilepton(void) const;
    bool passTopDileptonKinematics(void) const;
    bool passTopDileptonVertex(void) const;
    bool passTopDileptonIsolation(void) const;
    bool passTopDileptonID(void) const;

    // stored information
    inline double pv2dImpactParam(void) const { return pv2dip_; }
    inline double pv2dImpactParamSig(void) const { return pv2dipsig_; }
    inline double deltaZ(void) const { return deltaZ_; }

  private:
    double pv2dip_, pv2dipsig_;
    double deltaZ_;
  };

}

#endif
