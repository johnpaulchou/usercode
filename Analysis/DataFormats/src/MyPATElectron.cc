#include "Analysis/DataFormats/interface/MyPATElectron.h"

using namespace pat;

MyPATElectron::MyPATElectron() : Electron()
{
  pv2dip_=pv2dipsig_=0.0;
  deltaZ_=0.0;
}

MyPATElectron::MyPATElectron(const Electron& anElectron) : Electron(anElectron)
{
  pv2dip_=pv2dipsig_=0.0;
  deltaZ_=0.0;
}

MyPATElectron::~MyPATElectron() {}

////////////////////////////////////////////////////////////////////////
//
//
// TopDilepton Selection Criteria
//
//
////////////////////////////////////////////////////////////////////////

bool MyPATElectron::passTopDilepton(void) const
{
  return (passTopDileptonKinematics() &&
	  passTopDileptonConversion() &&
	  passTopDileptonVertex() && 
	  passTopDileptonID() && 
	  passTopDileptonIsolation());
}

bool MyPATElectron::passTopDileptonKinematics(void) const
{
  return (p4().Pt()>=20.0 &&
	  superClusterEt()>=10.0 &&
	  fabs(superClusterEta())<2.4);
}

bool MyPATElectron::passTopDileptonConversion(void) const
{
  if(fabs(conversionSep())<0.02 && fabs(conversionDcotth()<0.02)) return false;
  return numberOfLostHits()<2;
}

bool MyPATElectron::passTopDileptonVertex(void) const
{
  return (fabs(deltaZ())<1.0 &&
	  fabs(pv2dImpactParam())<0.04);
}

bool MyPATElectron::passTopDileptonID(void) const
{
  if(isEB())
    return (sigmaIetaIeta()<0.01 &&
	    fabs(deltaPhiSuperClusterTrackAtVtx())<0.8 &&
	    fabs(deltaEtaSuperClusterTrackAtVtx())<0.007 &&
	    hadronicOverEm()<0.12);
  else if(isEE())
    return (sigmaIetaIeta()<0.03 &&
	    fabs(deltaPhiSuperClusterTrackAtVtx())<0.7 &&
	    hadronicOverEm()<0.05);
  else
    return false;
}

bool MyPATElectron::passTopDileptonIsolation(void) const
{
  double eleet=p4().Pt();
  double trackIso = dr03TkSumPt()/eleet;
  double ecalIso = dr03EcalRecHitSumEt()/eleet;
  double hcalIso = dr03HcalTowerSumEt()/eleet; 
  double EBIso = (trackIso + std::max(0., ecalIso-1.0) + hcalIso) / std::max(20., eleet);
  double EEIso = (trackIso + ecalIso + hcalIso) / std::max(20., eleet);
  return isEB() ? EBIso<0.15 : EEIso<0.15;
}


