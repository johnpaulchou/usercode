#ifndef __ANALYSIS_MAKESKIMS_PARSEGENTREE_H__
#define __ANALYSIS_MAKESKIMS_PARSEGENTREE_H__

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include <vector>
#include <set>

class ParseGenTree
{
 public:

  // find all leptonic daughters, given a list of id's
  static void findLeptonicDaughters(const reco::GenParticleCollection& genp, const std::set<int>& motherids, std::vector<const reco::GenParticle*>& daughters);

  // find all charged leptonic daughters, given a list of id's
  static void findChargedLeptonicDaughters(const reco::GenParticleCollection& genp, const std::set<int>& motherids, std::vector<const reco::GenParticle*>& daughters);

  // find all leptonic daughters, given a list of id's
  // if a daughter is a tau, look at all of the tau's daughters, too
  static void findLeptonicDaughtersParseTaus(const reco::GenParticleCollection& genp, const std::set<int>& motherids, std::vector<const reco::GenParticle*>& daughters);

  // find all charged leptonic daughters, given a list of id's
  // if a daughter is a tau, look at all of the tau's daughters, too
  static void findChargedLeptonicDaughtersParseTaus(const reco::GenParticleCollection& genp, const std::set<int>& motherids, std::vector<const reco::GenParticle*>& daughters);

  // find all the daughters of a particle that do not have the same ID as the mother particle
  static void findUniqueDaughters(const reco::GenParticle* mother, std::vector<const reco::GenParticle*>& daughters);

 private:
  static void findUniqueDaughtersRecursive(const reco::GenParticle* mother, std::vector<const reco::GenParticle*>& daughters);
  static int findUniqueDaughtersRecursiveCounter;

};

#endif
