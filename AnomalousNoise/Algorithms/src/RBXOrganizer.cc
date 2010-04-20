#include "AnomalousNoise/Algorithms/interface/RBXOrganizer.h"
#include "RecoMET/METAlgorithms/interface/HcalHPDRBXMap.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"

#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "CondFormats/HcalObjects/interface/HcalChannelQuality.h"
#include "CondFormats/DataRecord/interface/HcalChannelQualityRcd.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputer.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputerRcd.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"

////////////////////////////////////////////////////////////
//
// HitValidator
//
////////////////////////////////////////////////////////////

HitValidator::HitValidator(const edm::ParameterSet& iConfig)
{
  HBThreshold_  = iConfig.getParameter<double>("HBThreshold");
  HESThreshold_ = iConfig.getParameter<double>("HESThreshold");
  HEDThreshold_ = iConfig.getParameter<double>("HEDThreshold");
  EBThreshold_  = iConfig.getParameter<double>("EBThreshold");
  EEThreshold_  = iConfig.getParameter<double>("EEThreshold");

  HcalAcceptSeverityLevel_ = iConfig.getParameter<uint32_t>("HcalAcceptSeverityLevel");
  EcalAcceptSeverityLevel_ = iConfig.getParameter<uint32_t>("EcalAcceptSeverityLevel");
  UseHcalRecoveredHits_ = iConfig.getParameter<bool>("UseHcalRecoveredHits");
  UseEcalRecoveredHits_ = iConfig.getParameter<bool>("UseEcalRecoveredHits");

  theHcalChStatus_=0;
  theEcalChStatus_=0;
  theHcalSevLvlComputer_=0;
  theEcalSevLvlAlgo_=0;
  theEBRecHitCollection_=0;
  theEERecHitCollection_=0;

  return;
}

HitValidator::~HitValidator()
{
}

bool HitValidator::validHit(const HBHERecHit& hit) const
{
  assert(theHcalSevLvlComputer_!=0 && theHcalChStatus_!=0);

  // require the hit to pass a certain energy threshold
  if(hit.id().subdet()==HcalBarrel && hit.energy()<HBThreshold_) return false;
  else if(hit.id().subdet()==HcalEndcap && hit.id().ietaAbs()<=20 && hit.energy()<HESThreshold_) return false;
  else if(hit.id().subdet()==HcalEndcap && hit.id().ietaAbs()>20 && hit.energy()<HEDThreshold_) return false;

  // determine if the hit is good, bad, or recovered
  const DetId id = hit.detid();
  const uint32_t recHitFlag = hit.flags();
  const uint32_t dbStatusFlag = theHcalChStatus_->getValues(id)->getValue();
  int severityLevel = theHcalSevLvlComputer_->getSeverityLevel(id, recHitFlag, dbStatusFlag);
  bool isRecovered  = theHcalSevLvlComputer_->recoveredRecHit(id, recHitFlag);

  if(severityLevel==0) return true;
  if(isRecovered) return UseHcalRecoveredHits_;
  if(severityLevel>static_cast<int>(HcalAcceptSeverityLevel_)) return false;
  else return true;
}

bool HitValidator::validHit(const EcalRecHit& hit) const
{
  assert(theEcalSevLvlAlgo_!=0 && theEcalChStatus_!=0 && theEBRecHitCollection_!=0 && theEERecHitCollection_!=0);

  // require the hit to pass a certain energy threshold
  const DetId id = hit.detid();
  if(id.subdetId()==EcalBarrel && hit.energy()<EBThreshold_) return false;
  else if(id.subdetId()==EcalEndcap && hit.energy()<EEThreshold_) return false;

  // determine if the hit is good, bad, or recovered
  int severityLevel = 999;
  if     (id.subdetId() == EcalBarrel) severityLevel = theEcalSevLvlAlgo_->severityLevel(id, *theEBRecHitCollection_, *theEcalChStatus_);
  else if(id.subdetId() == EcalEndcap) severityLevel = theEcalSevLvlAlgo_->severityLevel(id, *theEERecHitCollection_, *theEcalChStatus_);
  else return false;
  
  if(severityLevel == EcalSeverityLevelAlgo::kGood) return true;
  if(severityLevel == EcalSeverityLevelAlgo::kRecovered) return UseEcalRecoveredHits_;
  if(severityLevel > static_cast<int>(EcalAcceptSeverityLevel_)) return false;
  else return true;
}


////////////////////////////////////////////////////////////
//
// HcalHitOrganizer
//
////////////////////////////////////////////////////////////

HcalHitOrganizer::HcalHitOrganizer(const HBHERecHitCollection& collection, const HitValidator& hitvalidator)
{
  for(HBHERecHitCollection::const_iterator it=collection.begin(); it!=collection.end(); ++it) {
    const HBHERecHit* hit=&(*it);
    if(hitvalidator.validHit(*hit))
      hits_.insert(hit);
  }
}

HcalHitOrganizer::~HcalHitOrganizer()
{
}

const HBHERecHit* HcalHitOrganizer::findHit(int ieta, int iphi, int depth)
{
  // create a temporary HBHERecHit to search for (the fact that we have chosen the barrel by default doesn't matter)
  HBHERecHit hit(HcalDetId(HcalBarrel, ieta, iphi, depth), 0.0, 0.0);

  // search for the hit
  hitset_t::iterator it=hits_.find(&hit);

  // returns 0 if nothing is found
  if(it==hits_.end()) return 0;
  else return (*it);
}

void HcalHitOrganizer::neighbors(const HBHERecHit* hit, std::vector<const HBHERecHit*>& neighbors)
{
  neighbors_(hit, neighbors, 2);
  return;
}

void HcalHitOrganizer::neighborsDifferentRBX(const HBHERecHit* hit, std::vector<const HBHERecHit*>& neighbors)
{
  neighbors_(hit, neighbors, 0);
  return;
}

void HcalHitOrganizer::neighborsDifferentHPD(const HBHERecHit* hit, std::vector<const HBHERecHit*>& neighbors)
{
  neighbors_(hit, neighbors, 1);
  return;
}

void HcalHitOrganizer::neighborsSameHPD(const HBHERecHit* hit, std::vector<const HBHERecHit*>& neighbors)
{
  neighbors_(hit, neighbors, 3);
  return;
}


void HcalHitOrganizer::neighbors_(const HBHERecHit* hit, std::vector<const HBHERecHit*>& neighbors, int relative)
{
  assert(relative>=0 && relative<=3);
  
  // clear neighbors
  neighbors.clear();
  
  // add neighbors one unit up in ieta
  int neweta=hit->id().ieta()+1;
  int newetaAbs=std::abs(neweta);
  addHit_(hit, findHit(neweta, hit->id().iphi(), 1), neighbors, relative);
  if(newetaAbs>=15) addHit_(hit, findHit(neweta, hit->id().iphi(), 2), neighbors, relative);
  if(newetaAbs==16 || newetaAbs>=27) addHit_(hit, findHit(neweta, hit->id().iphi(), 3), neighbors, relative);

  // add neighbors one unit down in ieta
  neweta=hit->id().ieta()-1;
  newetaAbs=std::abs(neweta);
  addHit_(hit, findHit(neweta, hit->id().iphi(), 1), neighbors, relative);
  if(newetaAbs>=15) addHit_(hit, findHit(neweta, hit->id().iphi(), 2), neighbors, relative);
  if(newetaAbs==16 || newetaAbs>=27) addHit_(hit, findHit(neweta, hit->id().iphi(), 3), neighbors, relative);
  
  // add neighbors one unit up in iphi
  int phidiff = hit->id().ieta()<=20 ? 1 : 2;
  neweta=hit->id().ieta();
  newetaAbs=hit->id().ietaAbs();
  addHit_(hit, findHit(neweta, hit->id().iphi()+phidiff, 1), neighbors, relative);
  if(newetaAbs>=15) addHit_(hit, findHit(neweta, hit->id().iphi()+phidiff, 2), neighbors, relative);
  if(newetaAbs==16 || newetaAbs>=27) addHit_(hit, findHit(neweta, hit->id().iphi()+phidiff, 3), neighbors, relative);

  // add neighbors one unit down in iphi
  addHit_(hit, findHit(neweta, hit->id().iphi()-phidiff, 1), neighbors, relative);
  if(newetaAbs>=15) addHit_(hit, findHit(neweta, hit->id().iphi()-phidiff, 2), neighbors, relative);
  if(newetaAbs==16 || newetaAbs>=27) addHit_(hit, findHit(neweta, hit->id().iphi()-phidiff, 3), neighbors, relative);

  // special case for hit with ietaAbs==28 and depth==3
  // consider neighbors in ietaAbs==29, too
  if(hit->id().ietaAbs()==28 && hit->id().depth()==3) {
    neweta=hit->id().ieta()>0 ? 29 : -29;
    addHit_(hit, findHit(neweta, hit->id().iphi()+2, 1), neighbors, relative);
    addHit_(hit, findHit(neweta, hit->id().iphi()+2, 2), neighbors, relative);
    addHit_(hit, findHit(neweta, hit->id().iphi()-2, 1), neighbors, relative);
    addHit_(hit, findHit(neweta, hit->id().iphi()-2, 2), neighbors, relative);
  }


  return;
}

void HcalHitOrganizer::addHit_(const HBHERecHit* hit, const HBHERecHit* neighbor, std::vector<const HBHERecHit*>& neighbors, int relative)
{
  if(hit==0 || neighbor==0) return;
  if((relative==0 && HcalHPDRBXMap::indexRBX(hit->id())!=HcalHPDRBXMap::indexRBX(neighbor->id())) ||
     (relative==1 && HcalHPDRBXMap::indexHPD(hit->id())!=HcalHPDRBXMap::indexHPD(neighbor->id())) ||
     (relative==3 && HcalHPDRBXMap::indexHPD(hit->id())==HcalHPDRBXMap::indexHPD(neighbor->id())) ||
     (relative==2)) {
    neighbors.push_back(neighbor);
  }
  return;
}


////////////////////////////////////////////////////////////
//
// EcalHitOrganizer
//
////////////////////////////////////////////////////////////

EcalHitOrganizer::EcalHitOrganizer(const EcalRecHitCollection& ebcollection,
				   const EcalRecHitCollection& eecollection,
				   const HitValidator& hitvalidator,
				   const CaloTowerConstituentsMap& ctcm)
{
  for(EcalRecHitCollection::const_iterator it=ebcollection.begin(); it!=ebcollection.end(); ++it) {
    const EcalRecHit* hit=&(*it);
    if(!hitvalidator.validHit(*hit)) continue;

    // get the eta-phi coordinate and add it to the multimap
    CaloTowerDetId tid = ctcm.towerOf(hit->id());
    etaphi_t etaphi(tid.ieta(), tid.iphi());
    hits_.insert(std::pair<etaphi_t, const EcalRecHit*>(etaphi, hit));
  }
  return;
}

EcalHitOrganizer::~EcalHitOrganizer() {}

void EcalHitOrganizer::findHits(int ieta, int iphi, std::vector<const EcalRecHit*>& v) const
{
  v.clear();

  // find every rechit with these coordinates
  hitmap_t::const_iterator itlo, ithi;
  etaphi_t etaphi(ieta, iphi);
  itlo=hits_.lower_bound(etaphi);
  ithi=hits_.upper_bound(etaphi);

  for(hitmap_t::const_iterator it=itlo; it!=ithi; ++it) {
    v.push_back(it->second);
  }
  return;
}

////////////////////////////////////////////////////////////
//
// HitCollectionInfo
//
////////////////////////////////////////////////////////////

HitCollectionInfo::HitCollectionInfo()
{
  hitenergy_=neighborenergy_=ecalenergy_=-999.;
  nhits_=nneighbors_=necalhits_=-999;
}

double HitCollectionInfo::hitEnergy(double threshold) const
{
  if(hitenergy_<-900) {
    double energy=0;
    for(std::set<const HBHERecHit*>::const_iterator it=hcalhits_.begin(); it!=hcalhits_.end(); ++it) {
      const HBHERecHit *hit=(*it);
      double e = hit->energy();
      if(e<threshold) continue;
      energy += e;
    }
    hitenergy_=energy;
  }
  return hitenergy_;
}

double HitCollectionInfo::neighborEnergy(double threshold) const
{
  if(neighborenergy_<-900) {
    double energy=0;
    for(std::set<const HBHERecHit*>::const_iterator it=hcalneighbors_.begin(); it!=hcalneighbors_.end(); ++it) {
      const HBHERecHit *hit=(*it);
      double e = hit->energy();
      if(e<threshold) continue;
      energy += e;
    }
    neighborenergy_=energy;
  }
  return neighborenergy_;
}

double HitCollectionInfo::ecalEnergy(double threshold) const
{
  if(ecalenergy_<-900) {
    double energy=0;
    for(std::set<const EcalRecHit*>::const_iterator it=ecalneighbors_.begin(); it!=ecalneighbors_.end(); ++it) {
      const EcalRecHit *hit=(*it);
      double e = hit->energy();
      if(e<threshold) continue;
      energy += e;
    }
    ecalenergy_=energy;
  }
  return ecalenergy_;
}

int HitCollectionInfo::nHits(double threshold) const
{
  if(nhits_<-900) {
    int cntr=0;
    for(std::set<const HBHERecHit*>::const_iterator it=hcalhits_.begin(); it!=hcalhits_.end(); ++it) {
      const HBHERecHit *hit=(*it);
      double e = hit->energy();
      if(e<threshold) continue;
      ++cntr;
    }
    nhits_=cntr;
  }
  return nhits_;
}

int HitCollectionInfo::nNeighbors(double threshold) const
{
  if(nneighbors_<-900) {
    int cntr=0;
    for(std::set<const HBHERecHit*>::const_iterator it=hcalneighbors_.begin(); it!=hcalneighbors_.end(); ++it) {
      const HBHERecHit *hit=(*it);
      double e = hit->energy();
      if(e<threshold) continue;
      ++cntr;
    }
    nneighbors_=cntr;
  }
  return nneighbors_;
}

int HitCollectionInfo::nEcalHits(double threshold) const
{
  if(necalhits_<-900) {
    int cntr=0;
    for(std::set<const EcalRecHit*>::const_iterator it=ecalneighbors_.begin(); it!=ecalneighbors_.end(); ++it) {
      const EcalRecHit *hit=(*it);
      double e = hit->energy();
      if(e<threshold) continue;
      ++cntr;
    }
    necalhits_=cntr;
  }
  return necalhits_;
}


////////////////////////////////////////////////////////////
//
// RBXOrganizer
//
////////////////////////////////////////////////////////////


RBXOrganizer::RBXOrganizer(const HBHERecHitCollection& hbhehitcollection,
			   const EcalRecHitCollection& ebhitcollection,
			   const EcalRecHitCollection& eehitcollection,
			   const HitValidator& validator,
			   const CaloTowerConstituentsMap& ctcm)
  : hcalhitorganizer_(hbhehitcollection, validator),
    ecalhitorganizer_(ebhitcollection, eehitcollection, validator, ctcm)
{ 

  // loop over the hits first
  for(HBHERecHitCollection::const_iterator it=hbhehitcollection.begin(); it!=hbhehitcollection.end(); ++it) {
    const HBHERecHit* hit=&(*it);
    if(!validator.validHit(*hit)) continue;

    //
    // get the ECAL hits, first
    //

    std::vector<const EcalRecHit*> ecalhits;
    ecalhitorganizer_.findHits(hit->id().ieta(), hit->id().iphi(), ecalhits);
    
    // do something special if ietaAbs()==28 and depth==3
    if(hit->id().ietaAbs()==28 && hit->id().depth()==3) {

      // get the ecal hits in tower +/- 29, too
      std::vector<const EcalRecHit*> extraecalhits;
      ecalhitorganizer_.findHits(hit->id().ieta()==28 ? 29 : -29, hit->id().iphi(), extraecalhits);

      // append the hits to the original list
      ecalhits.insert(ecalhits.begin(), extraecalhits.begin(), extraecalhits.end());
    }

    //
    // handle the RBXs
    //

    // find out which rbx this hit belongs to
    int rbxidnum = HcalHPDRBXMap::indexRBX(hit->id());

    // add the hit to the rbx
    rbxs_[rbxidnum].hcalhits_.insert(hit);

    // get the hit's neighbors in a different RBX
    std::vector<const HBHERecHit*> rbxneighbors;
    hcalhitorganizer_.neighborsDifferentRBX(hit, rbxneighbors);
    
    // add the neighboring hits to the RBX
    for(std::vector<const HBHERecHit*>::const_iterator nit=rbxneighbors.begin(); nit!=rbxneighbors.end(); ++nit) {
      rbxs_[rbxidnum].hcalneighbors_.insert(*nit);
    }

    // add the ecal hits to the RBX
    rbxs_[rbxidnum].ecalneighbors_.insert(ecalhits.begin(), ecalhits.end());

    //
    // handle the HPDs
    //

    // find out which hpd this hit belonds to
    int hpdidnum = HcalHPDRBXMap::indexHPD(hit->id());

    // add the hit to the hpd
    hpds_[hpdidnum].hcalhits_.insert(hit);

    // get the hit's neighbors in a different HPD
    std::vector<const HBHERecHit*> hpdneighbors;
    hcalhitorganizer_.neighborsDifferentHPD(hit, hpdneighbors);
    
    // add the neighboring hits to the HPD
    for(std::vector<const HBHERecHit*>::const_iterator nit=hpdneighbors.begin(); nit!=hpdneighbors.end(); ++nit) {
      hpds_[hpdidnum].hcalneighbors_.insert(*nit);
    }

    // add the ecal hits to the HPD
    hpds_[hpdidnum].ecalneighbors_.insert(ecalhits.begin(), ecalhits.end());

    //
    // handle the dihits
    //
    
    // get the hit's neighbors in the same HPD
    std::vector<const HBHERecHit*> samehpdneighbors;
    hcalhitorganizer_.neighborsSameHPD(hit, samehpdneighbors);
    
    // if the hit has only one neighbor in the same HPD, see how many neighbors the neighbor has in the same HPD
    if(samehpdneighbors.size()==1) {
      std::vector<const HBHERecHit*> neighborsneighbors;
      hcalhitorganizer_.neighborsSameHPD(samehpdneighbors[0], neighborsneighbors);

      // if the neighbor only has one neighbor in the same HPD, then we call this a "dihit"
      // in order not to double-count, only consider the case when the hit has a higher energy than its neighbor
      if(neighborsneighbors.size()==1 && hit->energy()>samehpdneighbors[0]->energy()) {

	// create the dihit collection
	HitCollectionInfo dihit;
	dihit.hcalhits_.insert(hit);
	dihit.hcalhits_.insert(samehpdneighbors[0]);
	for(std::vector<const HBHERecHit*>::const_iterator nit=hpdneighbors.begin(); nit!=hpdneighbors.end(); ++nit) {
	  dihit.hcalneighbors_.insert(*nit);
	}
	
	// get the neighbor's neighbors in a different HPD
	std::vector<const HBHERecHit*> neighborshpdneighbors;
	hcalhitorganizer_.neighborsDifferentHPD(samehpdneighbors[0], neighborshpdneighbors);
	for(std::vector<const HBHERecHit*>::const_iterator nit=neighborshpdneighbors.begin(); nit!=neighborshpdneighbors.end(); ++nit) {
	  dihit.hcalneighbors_.insert(*nit);
	}

	// add the ecal hits to the dihit
	dihit.ecalneighbors_.insert(ecalhits.begin(), ecalhits.end());

	// get the neighbor's ecal hits, too
	std::vector<const EcalRecHit*> extraecalhits;
	ecalhitorganizer_.findHits(samehpdneighbors[0]->id().ieta(), samehpdneighbors[0]->id().iphi(), extraecalhits);
	dihit.ecalneighbors_.insert(extraecalhits.begin(), extraecalhits.end());


	// add the dihit to the vector
	dihits_.push_back(dihit);
      }
    }

    //
    // handle the mono hits
    //

    if(samehpdneighbors.size()==0) {
      // create the hit collection
      HitCollectionInfo monohit;
      monohit.hcalhits_.insert(hit);
      for(std::vector<const HBHERecHit*>::const_iterator nit=hpdneighbors.begin(); nit!=hpdneighbors.end(); ++nit) {
	monohit.hcalneighbors_.insert(*nit);
      }

      // add the ecal hits to the monohit
      monohit.ecalneighbors_.insert(ecalhits.begin(), ecalhits.end());
      
      // add the monohit to the vector
      monohits_.push_back(monohit);
    }

  } // finished looping over HBHERecHits
}

RBXOrganizer::~RBXOrganizer()
{

}

void RBXOrganizer::getRBXs(std::vector<HitCollectionInfo>& rbxs, double rbxenergy) const
{
  for(std::map<int, HitCollectionInfo>::const_iterator it=rbxs_.begin(); it!=rbxs_.end(); ++it) {
    const HitCollectionInfo *info=&(it->second);
    if(info->hitEnergy()>rbxenergy) rbxs.push_back(*info);
  }
  return;
}

void RBXOrganizer::getHPDs(std::vector<HitCollectionInfo>& hpds, double hpdenergy) const
{
  for(std::map<int, HitCollectionInfo>::const_iterator it=hpds_.begin(); it!=hpds_.end(); ++it) {
    const HitCollectionInfo *info=&(it->second);
    if(info->hitEnergy()>hpdenergy) hpds.push_back(*info);
  }
  return;
}

void RBXOrganizer::getDiHits(std::vector<HitCollectionInfo>& dihits, double dihitenergy) const
{
  for(std::vector<HitCollectionInfo>::const_iterator it=dihits_.begin(); it!=dihits_.end(); ++it) {
    const HitCollectionInfo *info=&(*it);
    if(info->hitEnergy()>dihitenergy) dihits.push_back(*info);
  }
  return;
}

void RBXOrganizer::getMonoHits(std::vector<HitCollectionInfo>& monohits, double monohitenergy) const
{
  for(std::vector<HitCollectionInfo>::const_iterator it=monohits_.begin(); it!=monohits_.end(); ++it) {
    const HitCollectionInfo *info=&(*it);
    if(info->hitEnergy()>monohitenergy) monohits.push_back(*info);
  }
  return;
}

void RBXOrganizer::dump(void) const
{
  std::cout << "------------------------------------------" << std::endl;

  for(std::map<int, HitCollectionInfo>::const_iterator it=rbxs_.begin(); it!=rbxs_.end(); ++it) {
    std::cout << "Hits in RBX " << it->first << ":" << std::endl;
    for(std::set<const HBHERecHit*>::const_iterator it2=it->second.hcalhits_.begin(); it2!=it->second.hcalhits_.end(); ++it2) {
      std::cout << (**it2) << std::endl;
    }
    std::cout << "Neighboring hits in the RBX " << it->first << ":" << std::endl;
    for(std::set<const HBHERecHit*>::const_iterator it2=it->second.hcalneighbors_.begin(); it2!=it->second.hcalneighbors_.end(); ++it2) {
      std::cout << (**it2) << std::endl;
    }
  }
  std::cout << std::endl;

  for(std::map<int, HitCollectionInfo>::const_iterator it=hpds_.begin(); it!=hpds_.end(); ++it) {
    std::cout << "Hits in HPD " << it->first << ":" << std::endl;
    for(std::set<const HBHERecHit*>::const_iterator it2=it->second.hcalhits_.begin(); it2!=it->second.hcalhits_.end(); ++it2) {
      std::cout << (**it2) << std::endl;
    }
    std::cout << "Neighboring hits in the HPD " << it->first << ":" << std::endl;
    for(std::set<const HBHERecHit*>::const_iterator it2=it->second.hcalneighbors_.begin(); it2!=it->second.hcalneighbors_.end(); ++it2) {
      std::cout << (**it2) << std::endl;
    }
  }
  std::cout << std::endl;

  for(std::vector<HitCollectionInfo>::const_iterator it=dihits_.begin(); it!=dihits_.end(); ++it) {
    std::cout << "Dihits" << std::endl;
    for(std::set<const HBHERecHit*>::const_iterator it2=it->hcalhits_.begin(); it2!=it->hcalhits_.end(); ++it2) {
      std::cout << (**it2) << std::endl;
    }
    std::cout << "Neighboring hits to the dihits" << std::endl;
    for(std::set<const HBHERecHit*>::const_iterator it2=it->hcalneighbors_.begin(); it2!=it->hcalneighbors_.end(); ++it2) {
      std::cout << (**it2) << std::endl;
    }
  }
  std::cout << std::endl;


  for(std::vector<HitCollectionInfo>::const_iterator it=monohits_.begin(); it!=monohits_.end(); ++it) {
    std::cout << "Monohits" << std::endl;
    for(std::set<const HBHERecHit*>::const_iterator it2=it->hcalhits_.begin(); it2!=it->hcalhits_.end(); ++it2) {
      std::cout << (**it2) << std::endl;
    }
    std::cout << "Neighboring hits to the monohits" << std::endl;
    for(std::set<const HBHERecHit*>::const_iterator it2=it->hcalneighbors_.begin(); it2!=it->hcalneighbors_.end(); ++it2) {
      std::cout << (**it2) << std::endl;
    }
  }
  std::cout << "------------------------------------------" << std::endl;

  return;
}
