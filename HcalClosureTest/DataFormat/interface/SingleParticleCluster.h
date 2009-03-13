#ifndef __SINGLE_PARTICLE_CLUSTER_H__
#define __SINGLE_PARTICLE_CLUSTER_H__

//
// SingleParticleCluster.h
//
//    description: Object which uses a generated particle
//                 to seed a NxN tower cluster
//

#include "DataFormats/CaloTowers/interface/CaloTower.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Common/interface/RefVector.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/RefProd.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <vector>

//
// forward declaration and typedefs
//

class SingleParticleCluster;
typedef std::vector<SingleParticleCluster> SingleParticleClusterCollection;


//
// class definition
//

class SingleParticleCluster {

  friend class SingleParticleClusterProducer;

 public:
  // constructors/destructor
  SingleParticleCluster();
  virtual ~SingleParticleCluster();

  // accessors
  const edm::Ref<reco::GenParticleCollection>& genParticle(void) const;
  const edm::RefVector<CaloTowerCollection>& caloTowers(void) const;

  // NxN cluster
  inline static int N(void) { return NUMTOWERS; }

  // cluster energy over particle momentum
  double EoverP(void) const;

  // delta-R between cluster and particle
  double deltaR(void) const;

  // number of towers used in the cluster
  int numUsedTowers(void) const { return caloTowers().size(); }

  // energy weighted cluster quantities
  math::PtEtaPhiMLorentzVector clstrP4(void) const;
  double clstrP(void) const { return clstrP4().e(); }
  double clstrEt(void) const { return clstrP4().pt(); }
  double clstrEmEnergy(void) const;
  double clstrHadEnergy(void) const;
  double clstrOuterEnergy(void) const;
  //  double clstrEmEt(void) const;
  //  double clstrHadEt(void) const;
  //  double clstrOuterEt(void) const;

 private:

  const static int NUMTOWERS = 3;

  edm::Ref<reco::GenParticleCollection> genp_;
  edm::RefVector<CaloTowerCollection> calotowers_;

};

#endif
