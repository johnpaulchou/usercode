//
// SingleParticleCluster.cc
//
//    description: container class for a NxN cluster of calotowers
//                 seeded by a single generated particle
//

#include "HcalClosureTest/DataFormats/interface/SingleParticleCluster.h"
#include <TMath.h>

SingleParticleCluster::SingleParticleCluster()
{
}

SingleParticleCluster::~SingleParticleCluster()
{
}

const edm::Ref<reco::GenParticleCollection>&
SingleParticleCluster::genParticle(void) const
{
  return genp_;
}

const edm::RefVector<CaloTowerCollection>&
SingleParticleCluster::caloTowers(void) const
{
  return calotowers_;
}

double SingleParticleCluster::EoverP(void) const
{
  double p=genp_->p();
  return p==0 ? -999. : clstrP()/p;
}

double SingleParticleCluster::deltaR(void) const
{
  double deta=genp_->eta() - clstrP4().eta();
  double dphi=fabs(genp_->phi() - clstrP4().phi());
  if(dphi > TMath::Pi()) dphi = 2.0*TMath::Pi() - dphi;
  return TMath::Sqrt(deta*deta+dphi*dphi);
}

math::PtEtaPhiMLorentzVector SingleParticleCluster::clstrP4(void) const
{
  math::PtEtaPhiMLorentzVector vec;
  for(edm::RefVector<CaloTowerCollection>::const_iterator it=calotowers_.begin();
      it!=calotowers_.end(); ++it) {
    vec += (*it)->p4();
  }
  return vec;
}

double SingleParticleCluster::clstrEmEnergy(void) const
{
  double tot=0.0;
  for(edm::RefVector<CaloTowerCollection>::const_iterator it=calotowers_.begin();
      it!=calotowers_.end(); ++it) {
    tot += (*it)->emEnergy();
  }
  return tot;
}

double SingleParticleCluster::clstrHadEnergy(void) const
{
  double tot=0.0;
  for(edm::RefVector<CaloTowerCollection>::const_iterator it=calotowers_.begin();
      it!=calotowers_.end(); ++it) {
    tot += (*it)->hadEnergy();
  }
  return tot;
}

double SingleParticleCluster::clstrOuterEnergy(void) const
{
  double tot=0.0;
  for(edm::RefVector<CaloTowerCollection>::const_iterator it=calotowers_.begin();
      it!=calotowers_.end(); ++it) {
    tot += (*it)->outerEnergy();
  }
  return tot;
}
