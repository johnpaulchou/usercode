#include "AnomalousNoise/Algorithms/interface/RBXOrganizer.h"
#include "RecoMET/METAlgorithms/interface/HcalHPDRBXMap.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "CondFormats/HcalObjects/interface/HcalChannelQuality.h"
#include "CondFormats/DataRecord/interface/HcalChannelQualityRcd.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputer.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputerRcd.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"

////////////////////////////////////////////////////////////
//
// ObjectValidator
//
////////////////////////////////////////////////////////////

ObjectValidator::ObjectValidator(const edm::ParameterSet& iConfig)
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

ObjectValidator::~ObjectValidator()
{
}

bool ObjectValidator::validHit(const HBHERecHit& hit) const
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

bool ObjectValidator::validHit(const EcalRecHit& hit) const
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

bool ObjectValidator::validTrack(const reco::Track& trk) const
{
  if(trk.pt()<0.7) return false;
  if(trk.pt()<1.0  && std::fabs(trk.momentum().eta())<1.479) return false;
  if(trk.numberOfValidHits()<8) return false;
  return true;
}

////////////////////////////////////////////////////////////
//
// PhysicsTowerOrganizer
//
////////////////////////////////////////////////////////////

PhysicsTowerOrganizer::PhysicsTowerOrganizer(const edm::Event& iEvent,
					     const edm::EventSetup& evSetup,
					     const HBHERecHitCollection& hbhehitcoll,
					     const EcalRecHitCollection& ebhitcoll,
					     const EcalRecHitCollection& eehitcoll,
					     const reco::TrackCollection& trackcoll,
					     const ObjectValidatorAbs& objectvalidator,
					     const CaloTowerConstituentsMap& ctcm,
					     TrackDetectorAssociator& trackAssociator,
					     const TrackAssociatorParameters& trackParameters)
{
  // get some geometries
  edm::ESHandle<CaloGeometry> pG;
  evSetup.get<CaloGeometryRecord>().get(pG);
  const CaloGeometry* geo = pG.product();
  const CaloSubdetectorGeometry* gEB = geo->getSubdetectorGeometry(DetId::Ecal,EcalBarrel);
  const CaloSubdetectorGeometry* gEE = geo->getSubdetectorGeometry(DetId::Ecal,EcalEndcap);

  // do the HCAL hits
  for(HBHERecHitCollection::const_iterator it=hbhehitcoll.begin(); it!=hbhehitcoll.end(); ++it) {
    const HBHERecHit* hit=&(*it);

    // check that the hit is valid
    if(!objectvalidator.validHit(*hit)) continue;

    // add the hit to the organizer
    CaloTowerDetId tid = ctcm.towerOf(hit->id());
    insert_(tid, hit);
  }

  // do the EB hits
  for(EcalRecHitCollection::const_iterator it=ebhitcoll.begin(); it!=ebhitcoll.end(); ++it) {
    const EcalRecHit* hit=&(*it);

    if(!objectvalidator.validHit(*hit)) continue;
    CaloTowerDetId tid = ctcm.towerOf(hit->id());
    insert_(tid, hit);
  }

  // do the EE hits
  for(EcalRecHitCollection::const_iterator it=eehitcoll.begin(); it!=eehitcoll.end(); ++it) {
    const EcalRecHit* hit=&(*it);

    if(!objectvalidator.validHit(*hit)) continue;
    CaloTowerDetId tid = ctcm.towerOf(hit->id());
    insert_(tid, hit);
  }
  
  // do the tracks
  for(reco::TrackCollection::const_iterator it=trackcoll.begin(); it!=trackcoll.end(); ++it) {
    const reco::Track* track=&(*it);
    
    if(!objectvalidator.validTrack(*track)) continue;
    

    const FreeTrajectoryState fts = TrackDetectorAssociator::getFreeTrajectoryState(evSetup, (*track));
    TrackDetMatchInfo info = trackAssociator.associate(iEvent, evSetup, fts, trackParameters);

    // doesn't make it to the ECAL
    if(info.trkGlobPosAtEcal.x()==0 && info.trkGlobPosAtEcal.y()==0 && info.trkGlobPosAtEcal.z()==0) continue;
    
    const GlobalPoint point(info.trkGlobPosAtEcal.x(), info.trkGlobPosAtEcal.y(), info.trkGlobPosAtEcal.z());

    if(std::fabs(point.eta())<1.479) {
      EBDetId cell = gEB->getClosestCell(point);
      CaloTowerDetId tid = ctcm.towerOf(cell);
      insert_(tid, track);
    } else {
      EEDetId cell = gEE->getClosestCell(point);
      CaloTowerDetId tid = ctcm.towerOf(cell);
      insert_(tid, track);
    }
  }
  return;
}

PhysicsTower* PhysicsTowerOrganizer::findTower(const CaloTowerDetId& id)
{
  // create dummy PhysicsTower
  PhysicsTower dummy;

  // correct for the merging of the |ieta|=28-29 towers
  if(id.ietaAbs()==29) dummy.id = CaloTowerDetId((id.ietaAbs()-1)*id.zside(), id.iphi());
  else dummy.id=id;

  // search on the dummy
  std::set<PhysicsTower, towercmp>::iterator it=towers_.find(dummy);
  
  if(it==towers_.end()) return 0;

  // for whatever reason, I can't get a non-const out of the find method
  PhysicsTower &twr = const_cast<PhysicsTower&>(*it);
  return &twr;
}

const PhysicsTower* PhysicsTowerOrganizer::findTower(const CaloTowerDetId& id) const
{
  // create dummy PhysicsTower
  PhysicsTower dummy;

  // correct for the merging of the |ieta|=28-29 towers
  if(id.ietaAbs()==29) dummy.id = CaloTowerDetId((id.ietaAbs()-1)*id.zside(), id.iphi());
  else dummy.id=id;

  // search on the dummy
  std::set<PhysicsTower, towercmp>::iterator it=towers_.find(dummy);
  
  if(it==towers_.end()) return 0;
  return &(*it);
}

const PhysicsTower* PhysicsTowerOrganizer::findTower(int ieta, int iphi) const
{
  CaloTowerDetId tid(ieta, iphi);
  return findTower(tid);
}

PhysicsTower* PhysicsTowerOrganizer::findTower(int ieta, int iphi)
{
  CaloTowerDetId tid(ieta, iphi);
  return findTower(tid);
}

void PhysicsTowerOrganizer::findNeighbors(const CaloTowerDetId& id, std::set<const PhysicsTower*>& neighbors) const
{
  std::vector<CaloTowerDetId> ids;
  // get the neighbor with higher iphi
  if(id.ietaAbs()<=20) {
    if(id.iphi()==72) ids.push_back(CaloTowerDetId(id.ieta(), 1));
    else              ids.push_back(CaloTowerDetId(id.ieta(), id.iphi()+1));
  } else {
    if(id.iphi()==71) ids.push_back(CaloTowerDetId(id.ieta(), 1));
    else              ids.push_back(CaloTowerDetId(id.ieta(), id.iphi()+2));
  }

  // get the neighbor with the lower iphi
  if(id.ietaAbs()<=20) {
    if(id.iphi()==1) ids.push_back(CaloTowerDetId(id.ieta(), 72));
    else             ids.push_back(CaloTowerDetId(id.ieta(), id.iphi()-1));
  } else {
    if(id.iphi()==1) ids.push_back(CaloTowerDetId(id.ieta(), 71));
    else             ids.push_back(CaloTowerDetId(id.ieta(), id.iphi()-2));
  }

  // get the neighbor with the higher ietaAbs
  if(id.ietaAbs()==20 && (id.iphi()%2)==0)
    ids.push_back(CaloTowerDetId((id.ietaAbs()+1)*id.zside(), id.iphi()-1));
  else
    ids.push_back(CaloTowerDetId((id.ietaAbs()+1)*id.zside(), id.iphi()));

  // get the neighbor(s) with the lower ietaAbs
  if(id.ietaAbs()==21) {
    ids.push_back(CaloTowerDetId((id.ietaAbs()-1)*id.zside(), id.iphi()));
    ids.push_back(CaloTowerDetId((id.ietaAbs()-1)*id.zside(), id.iphi()+1));
  } else if(id.ietaAbs()==1) {
    ids.push_back(CaloTowerDetId(-id.ieta(), id.iphi()));
  } else {
    ids.push_back(CaloTowerDetId((id.ietaAbs()-1)*id.zside(), id.iphi()));
  }
  
  // get the neighbor with higher ieta and higher iphi
  if(id.ietaAbs()<=19 || (id.ietaAbs()==20 && (id.iphi()%2)==0)) {
    if(id.iphi()==72) ids.push_back(CaloTowerDetId((id.ietaAbs()+1)*id.zside(), 1));
    else              ids.push_back(CaloTowerDetId((id.ietaAbs()+1)*id.zside(), id.iphi()+1));
  } else if(id.ietaAbs()>=21) {
    if(id.iphi()==71) ids.push_back(CaloTowerDetId((id.ietaAbs()+1)*id.zside(), 1));
    else              ids.push_back(CaloTowerDetId((id.ietaAbs()+1)*id.zside(), id.iphi()+2));
  }

  // get the neighbor with higher ieta and lower iphi
  if(id.ietaAbs()<=19) {
    if(id.iphi()==1) ids.push_back(CaloTowerDetId((id.ietaAbs()+1)*id.zside(), 72));
    else             ids.push_back(CaloTowerDetId((id.ietaAbs()+1)*id.zside(), id.iphi()-1));
  } else if(id.ietaAbs()>=21 || (id.ietaAbs()==20 && (id.iphi()%2)==1)) {
    if(id.iphi()==1) ids.push_back(CaloTowerDetId((id.ietaAbs()+1)*id.zside(), 71));
    else             ids.push_back(CaloTowerDetId((id.ietaAbs()+1)*id.zside(), id.iphi()-2));
  }

  // get the neighbor with lower ieta and higher iphi
  if(id.ietaAbs()==1) {
    if(id.iphi()==72) ids.push_back(CaloTowerDetId(-id.ieta(), 1));
    else              ids.push_back(CaloTowerDetId(-id.ieta(), id.iphi()+1));
  } else if(id.ietaAbs()<=20) {
    if(id.iphi()==72) ids.push_back(CaloTowerDetId((id.ietaAbs()-1)*id.zside(), 1));
    else              ids.push_back(CaloTowerDetId((id.ietaAbs()-1)*id.zside(), id.iphi()+1));
  } else if(id.ietaAbs()>=21) {
    if(id.iphi()==71) ids.push_back(CaloTowerDetId((id.ietaAbs()-1)*id.zside(), 1));
    else              ids.push_back(CaloTowerDetId((id.ietaAbs()-1)*id.zside(), id.iphi()+2));
  }

  // get the neighbor with lower ieta and lower iphi
  if(id.ietaAbs()==1) {
    if(id.iphi()==1) ids.push_back(CaloTowerDetId(-id.ieta(), 72));
    else             ids.push_back(CaloTowerDetId(-id.ieta(), id.iphi()-1));
  } else if(id.ietaAbs()<=20) {
    if(id.iphi()==1) ids.push_back(CaloTowerDetId((id.ietaAbs()-1)*id.zside(), 72));
    else             ids.push_back(CaloTowerDetId((id.ietaAbs()-1)*id.zside(), id.iphi()-1));
  } else if(id.ietaAbs()>=22) {
    if(id.iphi()==1) ids.push_back(CaloTowerDetId((id.ietaAbs()-1)*id.zside(), 71));
    else             ids.push_back(CaloTowerDetId((id.ietaAbs()-1)*id.zside(), id.iphi()-2));
  } else if(id.ietaAbs()==21) {
    if(id.iphi()==1) ids.push_back(CaloTowerDetId((id.ietaAbs()-1)*id.zside(), 72));
    else             ids.push_back(CaloTowerDetId((id.ietaAbs()-1)*id.zside(), id.iphi()-1));
  }
  
  // clear neighbors
  neighbors.clear();

  // find the neighbors and add them to the eponymous set
  for(std::vector<CaloTowerDetId>::const_iterator it=ids.begin(); it!=ids.end(); ++it) {
    const PhysicsTower* twr=findTower(*it);
    if(twr) neighbors.insert(twr);
  }

  return;
}

void PhysicsTowerOrganizer::findNeighbors(const PhysicsTower* twr, std::set<const PhysicsTower*>& neighbors) const
{
  findNeighbors(twr->id, neighbors);
  return;
}

void PhysicsTowerOrganizer::findNeighbors(int ieta, int iphi, std::set<const PhysicsTower*>& neighbors) const
{
  findNeighbors(CaloTowerDetId(ieta, iphi), neighbors);
  return;
}

void PhysicsTowerOrganizer::insert_(CaloTowerDetId& id, const HBHERecHit* hit)
{
  PhysicsTower* twr=findTower(id);
  if(twr==0) {
    PhysicsTower dummy;
    if(id.ietaAbs()==29)
      dummy.id = CaloTowerDetId((id.ietaAbs()-1)*id.zside(), id.iphi());
    else
      dummy.id = id;
    dummy.hcalhits.insert(hit);
    towers_.insert(dummy);
  } else {
    twr->hcalhits.insert(hit);
  }
  return;
}

void PhysicsTowerOrganizer::insert_(CaloTowerDetId& id, const EcalRecHit* hit)
{
  PhysicsTower* twr=findTower(id);
  if(twr==0) {
    PhysicsTower dummy;
    if(id.ietaAbs()==29)
      dummy.id = CaloTowerDetId((id.ietaAbs()-1)*id.zside(), id.iphi());
    else
      dummy.id = id;
    dummy.ecalhits.insert(hit);
    towers_.insert(dummy);
  } else {
    twr->ecalhits.insert(hit);
  }
  return;
}

void PhysicsTowerOrganizer::insert_(CaloTowerDetId& id, const reco::Track* track)
{
  PhysicsTower* twr=findTower(id);
  if(twr==0) {
    PhysicsTower dummy;
    if(id.ietaAbs()==29)
      dummy.id = CaloTowerDetId((id.ietaAbs()-1)*id.zside(), id.iphi());
    else
      dummy.id = id;
    dummy.tracks.insert(track);
    towers_.insert(dummy);
  } else {
    twr->tracks.insert(track);
  }
  return;
}


////////////////////////////////////////////////////////////
//
// HBHEHitMap
//
////////////////////////////////////////////////////////////

HBHEHitMap::HBHEHitMap()
{
  hitEnergy_=hitEnergyTrkFid_=-999.;
  nHits_=-999;
  hcalEnergySameTowers_=ecalEnergySameTowers_=trackEnergySameTowers_=-999.;
  nHcalHitsSameTowers_=nEcalHitsSameTowers_=nTracksSameTowers_=-999;
  hcalEnergyNeighborTowers_=ecalEnergyNeighborTowers_=trackEnergyNeighborTowers_=-999.;
  nHcalHitsNeighborTowers_=nEcalHitsNeighborTowers_=nTracksNeighborTowers_=-999;

}

double HBHEHitMap::hitEnergy(void) const
{
  if(hitEnergy_<-900) calcHits_();
  return hitEnergy_;
}

int HBHEHitMap::nHits(void) const
{
  if(nHits_<-900) calcHits_();
  return nHits_;
}

double HBHEHitMap::hitEnergyTrackFiducial(void) const
{
  if(hitEnergyTrkFid_<-900) calcHits_();
  return hitEnergyTrkFid_;
}


double HBHEHitMap::hcalEnergySameTowers(void) const
{
  if(hcalEnergySameTowers_<-900) calcHcalSameTowers_();
  return hcalEnergySameTowers_;
}

int HBHEHitMap::nHcalHitsSameTowers(void) const
{
  if(nHcalHitsSameTowers_<-900) calcHcalSameTowers_();
  return nHcalHitsSameTowers_;
}

double HBHEHitMap::ecalEnergySameTowers(void) const
{
  if(ecalEnergySameTowers_<-900) calcEcalSameTowers_();
  return ecalEnergySameTowers_;
}

int HBHEHitMap::nEcalHitsSameTowers(void) const
{
  if(nEcalHitsSameTowers_<-900) calcEcalSameTowers_();
  return nEcalHitsSameTowers_;
}

double HBHEHitMap::trackEnergySameTowers(void) const
{
  if(trackEnergySameTowers_<-900) calcTracksSameTowers_();
  return trackEnergySameTowers_;
}

int HBHEHitMap::nTracksSameTowers(void) const
{
  if(nTracksSameTowers_<-900) calcTracksSameTowers_();
  return nTracksSameTowers_;
}

void HBHEHitMap::hcalHitsSameTowers(std::set<const HBHERecHit*>& v) const
{
  v.clear();
  for(hitmap_const_iterator it1=beginHits(); it1!=endHits(); ++it1) {
    for(std::set<const HBHERecHit*>::const_iterator it2=it1->second->hcalhits.begin(); it2!=it1->second->hcalhits.end(); ++it2) {
      const HBHERecHit* hit=(*it2);
      // if the hit in the tower is already in the hitmap, don't include it
      if(findHit(hit)==endHits()) v.insert(hit);
    }
  }
  return;
}

void HBHEHitMap::ecalHitsSameTowers(std::set<const EcalRecHit*>& v) const
{
  v.clear();
  for(hitmap_const_iterator it1=beginHits(); it1!=endHits(); ++it1) {
    v.insert(it1->second->ecalhits.begin(), it1->second->ecalhits.end());
  }
  return;
}

void HBHEHitMap::tracksSameTowers(std::set<const reco::Track*>& v) const
{
  v.clear();
  for(hitmap_const_iterator it1=beginHits(); it1!=endHits(); ++it1) {
    v.insert(it1->second->tracks.begin(), it1->second->tracks.end());
  }
  return;
}

double HBHEHitMap::hcalEnergyNeighborTowers(void) const
{
  if(hcalEnergyNeighborTowers_<-900) calcHcalNeighborTowers_();
  return hcalEnergyNeighborTowers_;
}

int HBHEHitMap::nHcalHitsNeighborTowers(void) const
{
  if(nHcalHitsNeighborTowers_<-900) calcHcalNeighborTowers_();
  return nHcalHitsNeighborTowers_;
}

double HBHEHitMap::ecalEnergyNeighborTowers(void) const
{
  if(ecalEnergyNeighborTowers_<-900) calcEcalNeighborTowers_();
  return ecalEnergyNeighborTowers_;
}

int HBHEHitMap::nEcalHitsNeighborTowers(void) const
{
  if(nEcalHitsNeighborTowers_<-900) calcEcalNeighborTowers_();
  return nEcalHitsNeighborTowers_;
}

double HBHEHitMap::trackEnergyNeighborTowers(void) const
{
  if(trackEnergyNeighborTowers_<-900) calcTracksNeighborTowers_();
  return trackEnergyNeighborTowers_;
}

int HBHEHitMap::nTracksNeighborTowers(void) const
{
  if(nTracksNeighborTowers_<-900) calcTracksNeighborTowers_();
  return nTracksNeighborTowers_;
}

void HBHEHitMap::hcalHitsNeighborTowers(std::set<const HBHERecHit*>& v) const
{
  v.clear();
  for(neighbor_const_iterator it1=beginNeighbors(); it1!=endNeighbors(); ++it1) {
    const PhysicsTower* twr=(*it1);
    v.insert(twr->hcalhits.begin(), twr->hcalhits.end());
  }
  return;
}

void HBHEHitMap::ecalHitsNeighborTowers(std::set<const EcalRecHit*>& v) const
{
  v.clear();
  for(neighbor_const_iterator it1=beginNeighbors(); it1!=endNeighbors(); ++it1) {
    const PhysicsTower* twr=(*it1);
    v.insert(twr->ecalhits.begin(), twr->ecalhits.end());
  }

  return;
}

void HBHEHitMap::tracksNeighborTowers(std::set<const reco::Track*>& v) const
{
  v.clear();
  for(neighbor_const_iterator it1=beginNeighbors(); it1!=endNeighbors(); ++it1) {
    const PhysicsTower* twr=(*it1);
    v.insert(twr->tracks.begin(), twr->tracks.end());
  }
  return;
}

void HBHEHitMap::byTowers(std::vector<twrinfo>& v) const
{
  assert(0);
}

void HBHEHitMap::insert(const HBHERecHit* hit, const PhysicsTower* twr, std::set<const PhysicsTower*>& neighbors)
{
  hits_[hit]=twr;
  neighbors_.insert(neighbors.begin(), neighbors.end());

  // make sure none of the neighbors are also are part of the hitmap
  for(hitmap_const_iterator it=beginHits(); it!=endHits(); ++it) {
    const PhysicsTower* t=it->second;
    neighbor_const_iterator find=findNeighbor(t);

    // if a hit is also a neighbor, remove the neighbor
    if(find!=endNeighbors()) neighbors_.erase(find);
  }
  return;
}


void HBHEHitMap::calcHits_(void) const
{
  hitEnergy_=0;
  nHits_=0;
  hitEnergyTrkFid_=0;
  for(hitmap_const_iterator it=hits_.begin(); it!=hits_.end(); ++it) {
    const HBHERecHit* hit=it->first;
    if(hit->id().ietaAbs()<=24) hitEnergyTrkFid_+=hit->energy();
    hitEnergy_+=hit->energy();
    ++nHits_;
  }
  return;
}

void HBHEHitMap::calcHcalSameTowers_(void) const
{
  hcalEnergySameTowers_=0;
  nHcalHitsSameTowers_=0;
  std::set<const HBHERecHit*> v;
  hcalHitsSameTowers(v);
  for(std::set<const HBHERecHit*>::const_iterator it=v.begin(); it!=v.end(); ++it) {
    const HBHERecHit* hit=(*it);
    hcalEnergySameTowers_+=hit->energy();
    ++nHcalHitsSameTowers_;
  }
  return;
}

void HBHEHitMap::calcEcalSameTowers_(void) const
{
  ecalEnergySameTowers_=0;
  nEcalHitsSameTowers_=0;
  std::set<const EcalRecHit*> v;
  ecalHitsSameTowers(v);
  for(std::set<const EcalRecHit*>::const_iterator it=v.begin(); it!=v.end(); ++it) {
    const EcalRecHit* hit=(*it);
    ecalEnergySameTowers_+=hit->energy();
    ++nEcalHitsSameTowers_;
  }
  return;
}

void HBHEHitMap::calcTracksSameTowers_(void) const
{
  trackEnergySameTowers_=0;
  nTracksSameTowers_=0;
  std::set<const reco::Track*> v;
  tracksSameTowers(v);
  for(std::set<const reco::Track*>::const_iterator it=v.begin(); it!=v.end(); ++it) {
    const reco::Track* trk=(*it);
    trackEnergySameTowers_+=trk->p();
    ++nTracksSameTowers_;
  }
  return;
}

void HBHEHitMap::calcHcalNeighborTowers_(void) const
{
  hcalEnergyNeighborTowers_=0;
  nHcalHitsNeighborTowers_=0;
  std::set<const HBHERecHit*> v;
  hcalHitsNeighborTowers(v);
  for(std::set<const HBHERecHit*>::const_iterator it=v.begin(); it!=v.end(); ++it) {
    const HBHERecHit* hit=(*it);
    hcalEnergyNeighborTowers_+=hit->energy();
    ++nHcalHitsNeighborTowers_;
  }
  return;
}

void HBHEHitMap::calcEcalNeighborTowers_(void) const
{
  ecalEnergyNeighborTowers_=0;
  nEcalHitsNeighborTowers_=0;
  std::set<const EcalRecHit*> v;
  ecalHitsNeighborTowers(v);
  for(std::set<const EcalRecHit*>::const_iterator it=v.begin(); it!=v.end(); ++it) {
    const EcalRecHit* hit=(*it);
    ecalEnergyNeighborTowers_+=hit->energy();
    ++nEcalHitsNeighborTowers_;
  }
  return;
}

void HBHEHitMap::calcTracksNeighborTowers_(void) const
{
  trackEnergyNeighborTowers_=0;
  nTracksNeighborTowers_=0;
  std::set<const reco::Track*> v;
  tracksNeighborTowers(v);
  for(std::set<const reco::Track*>::const_iterator it=v.begin(); it!=v.end(); ++it) {
    const reco::Track* trk=(*it);
    trackEnergyNeighborTowers_+=trk->p();
    ++nTracksNeighborTowers_;
  }
  return;
}

////////////////////////////////////////////////////////////
//
// HBHEHitMapOrganizer
//
////////////////////////////////////////////////////////////


HBHEHitMapOrganizer::HBHEHitMapOrganizer(const HBHERecHitCollection& hbhehitcoll,
					 const ObjectValidatorAbs& objvalidator,
					 const PhysicsTowerOrganizer& pto)
{

  // loop over the hits
  for(HBHERecHitCollection::const_iterator it=hbhehitcoll.begin(); it!=hbhehitcoll.end(); ++it) {
    const HBHERecHit *hit=&(*it);
    if(!objvalidator.validHit(*hit)) continue;

    // get the Physics Tower and the neighbors
    const PhysicsTower* tower=pto.findTower(hit->id().ieta(), hit->id().iphi());

    std::set<const PhysicsTower*> neighbors;
    pto.findNeighbors(hit->id().ieta(), hit->id().iphi(), neighbors);

    // organize the RBXs
    int rbxidnum = HcalHPDRBXMap::indexRBX(hit->id());
    rbxs_[rbxidnum].insert(hit, tower, neighbors);

    // organize the HPDs
    int hpdidnum = HcalHPDRBXMap::indexHPD(hit->id());
    hpds_[hpdidnum].insert(hit, tower, neighbors);


    // organize the dihits
    std::vector<const HBHERecHit*> hpdneighbors;
    getHPDNeighbors(hit, hpdneighbors, pto);

    if(hpdneighbors.size()==1) {
      std::vector<const HBHERecHit*> hpdneighborsneighbors;
      getHPDNeighbors(hpdneighbors[0], hpdneighborsneighbors, pto);
      
      if(hpdneighborsneighbors.size()==1 && hpdneighborsneighbors[0]==hit && hit->energy()>hpdneighbors[0]->energy()) {
	// we've found two hits who are neighbors in the same HPD, but who have no other
	// neighbors (in the same HPD) in common.  In order not to double-count, we
	// require that the first hit has more energy

	const PhysicsTower* tower2=pto.findTower(hpdneighbors[0]->id().ieta(), hpdneighbors[0]->id().iphi());
	std::set<const PhysicsTower*> neighbors2;
	pto.findNeighbors(hpdneighbors[0]->id().ieta(), hpdneighbors[0]->id().iphi(), neighbors2);
	
	HBHEHitMap dihit;
	dihit.insert(hit, tower, neighbors);
	dihit.insert(hpdneighbors[0], tower2, neighbors2);
	dihits_.push_back(dihit);
      }
    } else if(hpdneighbors.size()==0) {

      // organize the monohits
      HBHEHitMap monohit;
      monohit.insert(hit, tower, neighbors);
      monohits_.push_back(monohit);
      }
    
  } // finished looping over HBHERecHits
  return;
}

void HBHEHitMapOrganizer::getRBXs(std::vector<HBHEHitMap>& v, double energy) const
{
  for(std::map<int, HBHEHitMap>::const_iterator it=rbxs_.begin(); it!=rbxs_.end(); ++it) {
    const HBHEHitMap &map=it->second;
    if(map.hitEnergy()>energy) v.push_back(map);
  }
  return;
}

void HBHEHitMapOrganizer::getHPDs(std::vector<HBHEHitMap>& v, double energy) const
{
  for(std::map<int, HBHEHitMap>::const_iterator it=hpds_.begin(); it!=hpds_.end(); ++it) {
    const HBHEHitMap &map=it->second;
    if(map.hitEnergy()>energy) v.push_back(map);
  }
  return;
}

void HBHEHitMapOrganizer::getDiHits(std::vector<HBHEHitMap>& v, double energy) const
{
  for(std::vector<HBHEHitMap>::const_iterator it=dihits_.begin(); it!=dihits_.end(); ++it) {
    if(it->hitEnergy()>energy) v.push_back(*it);
  }
  return;
}

void HBHEHitMapOrganizer::getMonoHits(std::vector<HBHEHitMap>& v, double energy) const
{
  for(std::vector<HBHEHitMap>::const_iterator it=monohits_.begin(); it!=monohits_.end(); ++it) {
    if(it->hitEnergy()>energy) v.push_back(*it);
  }
  return;
}



void HBHEHitMapOrganizer::getHPDNeighbors(const HBHERecHit* hit, std::vector<const HBHERecHit*>& neighbors, const PhysicsTowerOrganizer& pto)
{
  std::set<const PhysicsTower*> temp;
  pto.findNeighbors(hit->id().ieta(), hit->id().iphi(), temp);

  // make sure to include the same tower that the hit is in
  temp.insert(pto.findTower(hit->id().ieta(), hit->id().iphi()));

  // loop over the rechits in the temp neighbors
  for(std::set<const PhysicsTower*>::const_iterator it1=temp.begin(); it1!=temp.end(); ++it1) {
    for(std::set<const HBHERecHit*>::const_iterator it2=(*it1)->hcalhits.begin(); it2!=(*it1)->hcalhits.end(); ++it2) {
      const HBHERecHit* hit2(*it2);
      if(hit!=hit2 && HcalHPDRBXMap::indexHPD(hit->id())==HcalHPDRBXMap::indexHPD(hit2->id())) {
	neighbors.push_back(hit2);
      }
    }
  }
  return;
}
