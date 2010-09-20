#include "Analysis/DataFormats/interface/MyPATMuon.h"

using namespace pat;

MyPATMuon::MyPATMuon() : Muon()
{
  pv2dip_=pv2dipsig_=0.0;
  deltaZ_=0.0;
}

MyPATMuon::MyPATMuon(const Muon& aMuon) : Muon(aMuon)
{
  pv2dip_=pv2dipsig_=0.0;
  deltaZ_=0.0;
}

MyPATMuon::~MyPATMuon() {}


////////////////////////////////////////////////////////////////////////
//
//
// TopDilepton Selection Criteria
//
//
////////////////////////////////////////////////////////////////////////

bool MyPATMuon::passTopDilepton(void) const
{
  return (passTopDileptonKinematics() &&
	  passTopDileptonVertex() &&
	  passTopDileptonIsolation() &&
	  passTopDileptonID());
}

bool MyPATMuon::passTopDileptonKinematics(void) const
{
  return (isGlobalMuon() &&
	  globalTrack()->pt()>20.0 &&
	  abs(globalTrack()->eta())<2.4);
}

bool MyPATMuon::passTopDileptonVertex(void) const
{
  return (fabs(pv2dImpactParam())<0.02 &&
	  fabs(deltaZ())<1.0);
}

bool MyPATMuon::passTopDileptonIsolation(void) const
{
  return (isGlobalMuon() &&
	  (trackIso() + ecalIso() + hcalIso()) / globalTrack()->pt()<0.15);
}

bool MyPATMuon::passTopDileptonID(void) const
{
  return (isGlobalMuon() &&
	  muonID("GlobalMuonPromptTight") &&
	  numberOfValidHits()>=10 &&
	  normChi2()>=10.0 &&
	  globalTrack()->hitPattern().numberOfValidMuonHits()>=1);
}
