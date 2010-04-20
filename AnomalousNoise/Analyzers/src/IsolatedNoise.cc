//
// IsoaltedNoise.cc
//
//    description: Measure the rate of isolated HCAL noise
//
//    author: J.P. Chou, Brown
//
//

#include "AnomalousNoise/Analyzers/interface/IsolatedNoise.h"

#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "CondFormats/DataRecord/interface/EcalChannelStatusRcd.h"
#include "CondFormats/HcalObjects/interface/HcalChannelQuality.h"
#include "CondFormats/DataRecord/interface/HcalChannelQualityRcd.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputer.h"
#include "RecoLocalCalo/HcalRecAlgos/interface/HcalSeverityLevelComputerRcd.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"

// root stuff
#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"
#include "TMath.h"

IsolatedNoise::IsolatedNoise(const edm::ParameterSet& iConfig) :
  hbheLabel_(iConfig.getParameter<edm::InputTag>("hbheInput")),
  ecalLabels_(iConfig.getParameter<std::vector<edm::InputTag> >("ecalInputs")),
  hitvalidator_(iConfig)
{
}

IsolatedNoise::~IsolatedNoise()
{
}


// ------------ method called to for each event  ------------
void
IsolatedNoise::analyze(const edm::Event& iEvent, const edm::EventSetup& evSetup)
{
  // get the ECAL channel status map
  edm::ESHandle<EcalChannelStatus> ecalChStatus;
  evSetup.get<EcalChannelStatusRcd>().get( ecalChStatus );
  const EcalChannelStatus* dbEcalChStatus = ecalChStatus.product();

  // get the HCAL channel status map
  edm::ESHandle<HcalChannelQuality> hcalChStatus;    
  evSetup.get<HcalChannelQualityRcd>().get( hcalChStatus );
  const HcalChannelQuality* dbHcalChStatus = hcalChStatus.product();

  // get the severity level computers
  edm::ESHandle<HcalSeverityLevelComputer> hcalSevLvlComputerHndl;
  evSetup.get<HcalSeverityLevelComputerRcd>().get(hcalSevLvlComputerHndl);
  const HcalSeverityLevelComputer* hcalSevLvlComputer = hcalSevLvlComputerHndl.product();
  const EcalSeverityLevelAlgo* ecalSevLvlAlgo = new EcalSeverityLevelAlgo();

  // get the calotower mappings
  edm::ESHandle<CaloTowerConstituentsMap> ctcm;
  evSetup.get<IdealGeometryRecord>().get(ctcm);
  

  // get the HB/HE hits
  edm::Handle<HBHERecHitCollection> hbhehits_h;
  iEvent.getByLabel(hbheLabel_, hbhehits_h);
  if(!hbhehits_h.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find HBHERecHitCollection named " << hbheLabel_ << ".\n";
    return;
  }

  // get the ECAL hits
  edm::Handle<EcalRecHitCollection> ebhits_h;
  edm::Handle<EcalRecHitCollection> eehits_h;
  for(std::vector<edm::InputTag>::const_iterator i=ecalLabels_.begin(); i!=ecalLabels_.end(); i++) {
    edm::Handle<EcalRecHitCollection> ec_h;
    iEvent.getByLabel(*i,ec_h);
    if(ec_h.isValid() && ec_h->size()>0) {
      if((ec_h->begin()->detid()).subdetId() == EcalBarrel) ebhits_h=ec_h;
      if((ec_h->begin()->detid()).subdetId() == EcalEndcap) eehits_h=ec_h;
    }
  }
  if(!ebhits_h.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find EcalRecHitCollection for the Barrel.\n";
    return;
  }
  if(!eehits_h.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find EcalRecHitCollection for the Endcap.\n";
    return;
  }


  // set the status maps and severity level computers for the hit validator
  hitvalidator_.setHcalChannelQuality(dbHcalChStatus);
  hitvalidator_.setEcalChannelStatus(dbEcalChStatus);
  hitvalidator_.setHcalSeverityLevelComputer(hcalSevLvlComputer);
  hitvalidator_.setEcalSeverityLevelAlgo(ecalSevLvlAlgo);
  hitvalidator_.setEBRecHitCollection(&(*ebhits_h));
  hitvalidator_.setEERecHitCollection(&(*eehits_h));

  // organizer the hits
  RBXOrganizer organizer(*hbhehits_h, *ebhits_h, *eehits_h, hitvalidator_, *(ctcm.product()));

  // get the rbxs, hpds, dihits, and monohits
  std::vector<HitCollectionInfo> rbxs;
  std::vector<HitCollectionInfo> hpds;
  std::vector<HitCollectionInfo> dihits;
  std::vector<HitCollectionInfo> monohits;
  
  organizer.getRBXs(rbxs, 10.0);
  organizer.getHPDs(hpds, 10.0);
  organizer.getDiHits(dihits, 10.0);
  organizer.getMonoHits(monohits, 10.0);

  // print the results
  if(rbxs.size()>0 || hpds.size()>0 || dihits.size()>0 || monohits.size()>0) {
    std::cout << "------------------------------------" << std::endl;
    std::cout << "RBXs:" << std::endl;
    DumpHitCollectionInfo(rbxs);
    std::cout << "\nHPDs:" << std::endl;
    DumpHitCollectionInfo(hpds);
    std::cout << "\nDiHits:" << std::endl;
    DumpHitCollectionInfo(dihits);
    std::cout << "\nMonoHits:" << std::endl;
    DumpHitCollectionInfo(monohits);
    std::cout << "------------------------------------" << std::endl;
  }

  delete ecalSevLvlAlgo;

  return;  
}


void IsolatedNoise::DumpHitCollectionInfo(std::vector<HitCollectionInfo>& i) const
{
  for(std::vector<HitCollectionInfo>::const_iterator it=i.begin(); it!=i.end(); ++it) {
    std::cout << "hit energy=" << it->hitEnergy()
	      << "; neighbor energy=" << it->neighborEnergy()
	      << "; ecal energy=" << it->ecalEnergy()
	      << "; # hits=" << it->nHits()
	      << "; # neighbors=" << it->nNeighbors()
	      << "; # ecal hits=" << it->nEcalHits() << std::endl;
    std::cout << "Hcal Hits:" << std::endl;
    for(std::set<const HBHERecHit*>::const_iterator hit=it->hcalHitsBegin(); hit!=it->hcalHitsEnd(); ++hit) {
      const HBHERecHit* thehit=&(**hit);
      std::cout << (*thehit) << std::endl;
    }
  }
  return;
}

void IsolatedNoise::beginJob()
{
}

void IsolatedNoise::endJob()
{
}

//define this as a plug-in
DEFINE_FWK_MODULE(IsolatedNoise);

