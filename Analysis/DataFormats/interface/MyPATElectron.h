#ifndef __MY_PAT_ELECTRON_H__
#define __MY_PAT_ELECTRON_H__

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "RecoEgamma/EgammaTools/interface/ConversionInfo.h"

namespace pat {

  class MyPATElectron : public Electron {

    friend class MyPATElectronSelector;

  public:
    MyPATElectron();
    MyPATElectron(const Electron& anElectron);
    virtual ~MyPATElectron();

    /// required reimplementation of the Candidate's clone method
    virtual MyPATElectron* clone() const { return new MyPATElectron(*this); }

    // Selection criteria
    bool passTopDilepton(void) const;
    bool passTopDileptonKinematics(void) const;
    bool passTopDileptonConversion(void) const;
    bool passTopDileptonVertex(void) const;
    bool passTopDileptonID(void) const;
    bool passTopDileptonIsolation(void) const;

    // quick access
    inline double superClusterE(void) const { return superCluster()->energy(); }
    inline double superClusterEta(void) const { return superCluster()->eta(); }
    inline double superClusterEt(void) const { return superClusterE()/cosh(superClusterEta()); }
    
    inline int numberOfLostHits(void) const { return gsfTrack()->trackerExpectedHitsInner().numberOfLostHits(); }
    inline ConversionInfo conversionInfo(void) const { return convinfo_; }
    inline double conversionSep(void) const { return convinfo_.dist(); }
    inline double conversionDcotth(void) const { return convinfo_.dcot(); }
    inline double conversionRadius(void) const { return convinfo_.radiusOfConversion(); }
    
    // stored information
    inline double pv2dImpactParam(void) const { return pv2dip_; }
    inline double pv2dImpactParamSig(void) const { return pv2dipsig_; }
    inline double deltaZ(void) const { return deltaZ_; }

  private:
    mutable ConversionInfo convinfo_; // mutable, because someone upstream needs to make accessors const

    double pv2dip_, pv2dipsig_;
    double deltaZ_;

  };

}


#endif
