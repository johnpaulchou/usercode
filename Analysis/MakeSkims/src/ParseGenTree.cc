#include "Analysis/MakeSkims/interface/ParseGenTree.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

int ParseGenTree::findUniqueDaughtersRecursiveCounter;

void ParseGenTree::findLeptonicDaughters(const reco::GenParticleCollection& genp, const std::set<int>& motherids, std::vector<const reco::GenParticle*>& daughters)
{
  // clear the daughters, just for good measure
  daughters.clear();

  // loop over gen particles
  for(reco::GenParticleCollection::const_iterator iGen=genp.begin(); iGen!=genp.end(); ++iGen) {
    const reco::GenParticle *mother = &(*iGen);

    // is this one of the mothers we are considering?
    std::set<int>::const_iterator foundIter = motherids.find(mother->pdgId());
    if(foundIter==motherids.end()) continue;

    // get all the unique daughters
    std::vector<const reco::GenParticle*> alldaughters;
    findUniqueDaughters(mother, alldaughters);

    // select those daughters which are leptons
    for(unsigned int i=0; i<alldaughters.size(); ++i) {
      int absid=abs(alldaughters[i]->pdgId());
      if(absid>=11 && absid<=16) daughters.push_back(alldaughters[i]);
    }
  }
  return;
}

void ParseGenTree::findChargedLeptonicDaughters(const reco::GenParticleCollection& genp, const std::set<int>& motherids, std::vector<const reco::GenParticle*>& daughters)
{
  // clear the daughters, just for good measure
  daughters.clear();

  // get all leptons
  std::vector<const reco::GenParticle*> allleptons;
  findLeptonicDaughters(genp, motherids, allleptons);

  // select only the charged leptons
  for(unsigned int i=0; i<allleptons.size(); ++i) {
    int absid=abs(allleptons[i]->pdgId());
    if(absid==11 || absid==13 || absid==15) daughters.push_back(allleptons[i]);
  }
  return;
}

void ParseGenTree::findLeptonicDaughtersParseTaus(const reco::GenParticleCollection& genp, const std::set<int>& motherids, std::vector<const reco::GenParticle*>& daughters)
{
  // clear the daughters, just for good measure
  daughters.clear();

  // loop over all the leptonic daughters
  std::vector<const reco::GenParticle*> allleptons;
  findLeptonicDaughters(genp, motherids, allleptons);
  for(unsigned int i=0; i<allleptons.size(); ++i) {

    // if a daughter is a tau, add the tau daughters instead
    if(abs(allleptons[i]->pdgId())==15) {
      std::vector<const reco::GenParticle*> taudaughters;
      findUniqueDaughters(allleptons[i], taudaughters);
      daughters.insert(daughters.end(), taudaughters.begin(), taudaughters.end());
    } else {
      daughters.push_back(allleptons[i]);
    }
  }
  return;
}

void ParseGenTree::findChargedLeptonicDaughtersParseTaus(const reco::GenParticleCollection& genp, const std::set<int>& motherids, std::vector<const reco::GenParticle*>& daughters)
{
  // clear the daughters, just for good measure
  daughters.clear();

  // get all leptons
  std::vector<const reco::GenParticle*> allleptons;
  findLeptonicDaughtersParseTaus(genp, motherids, allleptons);

  // select only the charged leptons
  for(unsigned int i=0; i<allleptons.size(); ++i) {
    int absid=abs(allleptons[i]->pdgId());
    if(absid==11 || absid==13 || absid==15) daughters.push_back(allleptons[i]);
  }
  return;
}

void ParseGenTree::findUniqueDaughters(const reco::GenParticle* mother, std::vector<const reco::GenParticle*>& daughters)
{
  // zero out the daughters, just in case
  daughters.clear();

  // zero out the counter
  findUniqueDaughtersRecursiveCounter=0;
  
  // now run the recursive version
  return findUniqueDaughtersRecursive(mother, daughters);
}


void ParseGenTree::findUniqueDaughtersRecursive(const reco::GenParticle* mother, std::vector<const reco::GenParticle*>& daughters)
{
  // since this is a recursive function, we should have a bail-out procedure
  if(findUniqueDaughtersRecursiveCounter++>20) {
    edm::LogWarning("RecursiveFailure") << "findUniqueDaughtersRecursive() called 20 times before bailing.";
    return;
  }

  // loop over all daughters
  for(unsigned int i=0; i<mother->numberOfDaughters(); i++) {
    const reco::GenParticle *daughter=dynamic_cast<const reco::GenParticle*>(mother->daughter(i));
    
    // don't want to cause a universe-ending paradox
    if(daughter==mother) continue;

    // descend if the mother and daughter have the same ID
    if(daughter->pdgId()==mother->pdgId())
      findUniqueDaughtersRecursive(daughter, daughters);

    // otherwise, add the daughter to the list
    else
      daughters.push_back(daughter);
  }
  return;
}
