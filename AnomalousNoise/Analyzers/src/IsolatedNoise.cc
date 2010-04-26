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

#include "RecoMET/METAlgorithms/interface/HcalHPDRBXMap.h"

// root stuff
#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"
#include "TMath.h"

IsolatedNoise::IsolatedNoise(const edm::ParameterSet& iConfig) :
  hbheLabel_(iConfig.getParameter<edm::InputTag>("hbheInput")),
  ecalLabels_(iConfig.getParameter<std::vector<edm::InputTag> >("ecalInputs")),
  trackLabel_(iConfig.getParameter<edm::InputTag>("trackInput")),
  rootHistFileName_(iConfig.getParameter<std::string>("rootHistFileName")),
  objvalidator_(iConfig),
  trackAssociator_()
{
  trackAssociator_.useDefaultPropagator();
  edm::ParameterSet parameters = iConfig.getParameter<edm::ParameterSet>("TrackAssociatorParameters");
  trackParameters_.loadParameters( parameters );
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

  // get the tracks
  edm::Handle<reco::TrackCollection> tracks_h;
  iEvent.getByLabel(trackLabel_, tracks_h);
  if(!tracks_h.isValid()) {
    throw edm::Exception(edm::errors::ProductNotFound)
      << " could not find TrackCollection named " << trackLabel_ << ".\n";
    return;
  }

  // set the status maps and severity level computers for the hit validator
  objvalidator_.setHcalChannelQuality(dbHcalChStatus);
  objvalidator_.setEcalChannelStatus(dbEcalChStatus);
  objvalidator_.setHcalSeverityLevelComputer(hcalSevLvlComputer);
  objvalidator_.setEcalSeverityLevelAlgo(ecalSevLvlAlgo);
  objvalidator_.setEBRecHitCollection(&(*ebhits_h));
  objvalidator_.setEERecHitCollection(&(*eehits_h));

  // organizer the hits
  PhysicsTowerOrganizer pto(iEvent, evSetup, *hbhehits_h, *ebhits_h, *eehits_h, *tracks_h, objvalidator_, *(ctcm.product()), trackAssociator_, trackParameters_);
  HBHEHitMapOrganizer organizer(*hbhehits_h, objvalidator_, pto);

  // get the rbxs, hpds, dihits, and monohits above 10 GeV
  std::vector<HBHEHitMap> rbxs;
  std::vector<HBHEHitMap> hpds;
  std::vector<HBHEHitMap> dihits;
  std::vector<HBHEHitMap> monohits;
  organizer.getRBXs(rbxs, 10.0);
  organizer.getHPDs(hpds, 10.0);
  organizer.getDiHits(dihits, 10.0);
  organizer.getMonoHits(monohits, 10.0);

  std::set<const HBHERecHit*> noisehits;

  // setup the tree
  foundnoise_=0;
  noisee_=0;
  nnoisehits_=0;
  noisetype_=0;

  nrbxs_=static_cast<int>(rbxs.size());
  for(int i=0; i<static_cast<int>(rbxs.size()); i++) {
    rbxnhits_[i]=rbxs[i].nHits();
    rbxe_[i]=rbxs[i].hitEnergy();
    rbxtrkfide_[i]=rbxs[i].hitEnergyTrackFiducial();
    rbxisolnhcalhits_[i]=rbxs[i].nHcalHitsSameTowers()+rbxs[i].nHcalHitsNeighborTowers();
    rbxisolnecalhits_[i]=rbxs[i].nEcalHitsSameTowers();
    rbxisolntrks_[i]=rbxs[i].nTracksSameTowers()+rbxs[i].nTracksNeighborTowers();
    rbxisolhcale_[i]=rbxs[i].hcalEnergySameTowers()+rbxs[i].hcalEnergyNeighborTowers();
    rbxisolecale_[i]=rbxs[i].ecalEnergySameTowers();
    rbxisoltrke_[i]=rbxs[i].trackEnergySameTowers()+rbxs[i].trackEnergyNeighborTowers();

    if(rbxe_[i]>10.0 && rbxnhits_[i]>=10 && rbxtrkfide_[i]>5.0 && rbxisolhcale_[i]/rbxe_[i]<0.05 && rbxisolecale_[i]/rbxe_[i]<0.05 && rbxisoltrke_[i]/rbxtrkfide_[i]<0.05) {
      foundnoise_=1;
      noisee_+=rbxe_[i];
      nnoisehits_+=rbxnhits_[i];
      noisetype_ |= 0x1;
      for(HBHEHitMap::hitmap_const_iterator it=rbxs[i].beginHits(); it!=rbxs[i].endHits(); ++it)
	noisehits.insert(it->first);
    }
  }

  nhpds_=static_cast<int>(hpds.size());
  for(int i=0; i<static_cast<int>(hpds.size()); i++) {
    hpdnhits_[i]=hpds[i].nHits();
    hpde_[i]=hpds[i].hitEnergy();
    hpdtrkfide_[i]=hpds[i].hitEnergyTrackFiducial();
    hpdisolnhcalhits_[i]=hpds[i].nHcalHitsSameTowers()+hpds[i].nHcalHitsNeighborTowers();
    hpdisolnecalhits_[i]=hpds[i].nEcalHitsSameTowers();
    hpdisolntrks_[i]=hpds[i].nTracksSameTowers()+hpds[i].nTracksNeighborTowers();
    hpdisolhcale_[i]=hpds[i].hcalEnergySameTowers()+hpds[i].hcalEnergyNeighborTowers();
    hpdisolecale_[i]=hpds[i].ecalEnergySameTowers();
    hpdisoltrke_[i]=hpds[i].trackEnergySameTowers()+hpds[i].trackEnergyNeighborTowers();

    if(hpde_[i]>15.0 && hpdnhits_[i]>=3 && hpdtrkfide_[i]>8.0 && hpdisolhcale_[i]/hpde_[i]<0.05 && hpdisolecale_[i]/hpde_[i]<0.05 && hpdisoltrke_[i]/hpdtrkfide_[i]<0.05) {
      foundnoise_=1;
      noisee_+=hpde_[i];
      nnoisehits_+=hpdnhits_[i];
      noisetype_ |= 0x2;
      for(HBHEHitMap::hitmap_const_iterator it=hpds[i].beginHits(); it!=hpds[i].endHits(); ++it)
	noisehits.insert(it->first);

    }
  }

  ndis_=static_cast<int>(dihits.size());
  for(int i=0; i<static_cast<int>(dihits.size()); i++) {
    dinhits_[i]=dihits[i].nHits();
    die_[i]=dihits[i].hitEnergy();
    ditrkfide_[i]=dihits[i].hitEnergyTrackFiducial();
    diisolnhcalhits_[i]=dihits[i].nHcalHitsSameTowers()+dihits[i].nHcalHitsNeighborTowers();
    diisolnecalhits_[i]=dihits[i].nEcalHitsSameTowers();
    diisolntrks_[i]=dihits[i].nTracksSameTowers()+dihits[i].nTracksNeighborTowers();
    diisolhcale_[i]=dihits[i].hcalEnergySameTowers()+dihits[i].hcalEnergyNeighborTowers();
    diisolecale_[i]=dihits[i].ecalEnergySameTowers();
    diisoltrke_[i]=dihits[i].trackEnergySameTowers()+dihits[i].trackEnergyNeighborTowers();

    if((die_[i]>20.0 && ditrkfide_[i]>19.9 && diisolhcale_[i]/die_[i]<0.05 && diisolecale_[i]/die_[i]<0.05 && diisoltrke_[i]/ditrkfide_[i]<0.05) ||
       (die_[i]>20.0 && diisolhcale_[i]/die_[i]<0.01 && diisolecale_[i]/die_[i]<0.01)) {
      foundnoise_=1;
      noisee_+=die_[i];
      nnoisehits_+=dinhits_[i];
      noisetype_ |= 0x4;
      for(HBHEHitMap::hitmap_const_iterator it=dihits[i].beginHits(); it!=dihits[i].endHits(); ++it)
	noisehits.insert(it->first);

    }
  }

  nmonos_=static_cast<int>(monohits.size());
  for(int i=0; i<static_cast<int>(monohits.size()); i++) {
    mononhits_[i]=monohits[i].nHits();
    monoe_[i]=monohits[i].hitEnergy();
    monotrkfide_[i]=monohits[i].hitEnergyTrackFiducial();
    monoisolnhcalhits_[i]=monohits[i].nHcalHitsSameTowers()+monohits[i].nHcalHitsNeighborTowers();
    monoisolnecalhits_[i]=monohits[i].nEcalHitsSameTowers();
    monoisolntrks_[i]=monohits[i].nTracksSameTowers()+monohits[i].nTracksNeighborTowers();
    monoisolhcale_[i]=monohits[i].hcalEnergySameTowers()+monohits[i].hcalEnergyNeighborTowers();
    monoisolecale_[i]=monohits[i].ecalEnergySameTowers();
    monoisoltrke_[i]=monohits[i].trackEnergySameTowers()+monohits[i].trackEnergyNeighborTowers();

    if((monoe_[i]>25.0 && monotrkfide_[i]>24.9 && monoisolhcale_[i]/monoe_[i]<0.05 && monoisolecale_[i]/monoe_[i]<0.05 && monoisoltrke_[i]/monotrkfide_[i]<0.05) ||
       (monoe_[i]>25.0 && monoisolhcale_[i]/monoe_[i]<0.01 && monoisolecale_[i]/monoe_[i]<0.01)) {
      foundnoise_=1;
      noisee_+=monoe_[i];
      nnoisehits_+=mononhits_[i];
      noisetype_ |= 0x8;
      for(HBHEHitMap::hitmap_const_iterator it=monohits[i].beginHits(); it!=monohits[i].endHits(); ++it)
	noisehits.insert(it->first);

    }
  }


  // print the results
  if(noisehits.size()>0) {
    std::cout << "Dump of the noisy hits (" << iEvent.run() << ":" << iEvent.id().event() << ")" << std::endl;
    for(std::set<const HBHERecHit*>::const_iterator it=noisehits.begin(); it!=noisehits.end(); ++it) {
      std::cout << (**it) << std::endl;
    }
  }
  /*
  if(rbxs.size()>0 || hpds.size()>0 || dihits.size()>0 || monohits.size()>0) {
    std::cout << "------------------------------------" << std::endl;
    std::cout << "RBXs:" << std::endl;
    DumpHBHEHitMap(rbxs);
    std::cout << "\nHPDs:" << std::endl;
    DumpHBHEHitMap(hpds);
    std::cout << "\nDiHits:" << std::endl;
    DumpHBHEHitMap(dihits);
    std::cout << "\nMonoHits:" << std::endl;
    DumpHBHEHitMap(monohits);
    std::cout << "------------------------------------" << std::endl;
    }*/
  
  tree_->Fill();

  delete ecalSevLvlAlgo;

  return;  
}


void IsolatedNoise::DumpHBHEHitMap(std::vector<HBHEHitMap>& i) const
{
  for(std::vector<HBHEHitMap>::const_iterator it=i.begin(); it!=i.end(); ++it) {
    std::cout << "hit energy=" << it->hitEnergy()
	      << "; # hits=" << it->nHits()
	      << "; hcal energy same=" << it->hcalEnergySameTowers()
	      << "; ecal energy same=" << it->ecalEnergySameTowers()
	      << "; track energy same=" << it->trackEnergySameTowers()
	      << "; neighbor hcal energy=" << it->hcalEnergyNeighborTowers() << std::endl;
    std::cout << "hits:" << std::endl;
    for(HBHEHitMap::hitmap_const_iterator it2=it->beginHits(); it2!=it->endHits(); ++it2) {
      const HBHERecHit *hit=it2->first;
      std::cout << "RBX #=" << HcalHPDRBXMap::indexRBX(hit->id())
		<< "; HPD #=" << HcalHPDRBXMap::indexHPD(hit->id())
		<< "; " << (*hit) << std::endl;
    }
    std::cout << std::endl;
  }
  return;
}

void IsolatedNoise::beginJob()
{
  rootfile_ = new TFile(rootHistFileName_.c_str(), "RECREATE");
  tree_ = new TTree("tree","tree");

  tree_->Branch("foundnoise",&foundnoise_,"foundnoise/I");
  tree_->Branch("noisee",&noisee_,"noisee/F");
  tree_->Branch("nnoisehits",&nnoisehits_,"nnoisehits/I");
  tree_->Branch("noisetype",&noisetype_,"noisetype/I");

  tree_->Branch("nrbxs",&nrbxs_,"nrbxs/I");
  tree_->Branch("rbxnhits",rbxnhits_,"rbxnhits[nrbxs]/I");
  tree_->Branch("rbxe",rbxe_,"rbxe[nrbxs]/F");
  tree_->Branch("rbxtrkfide",rbxtrkfide_,"rbxtrkfide[nrbxs]/F");
  tree_->Branch("rbxisolnhcalhits",rbxisolnhcalhits_,"rbxisolnhcalhits[nrbxs]/I");
  tree_->Branch("rbxisolnecalhits",rbxisolnecalhits_,"rbxisolnecalhits[nrbxs]/I");
  tree_->Branch("rbxisolntrks",rbxisolntrks_,"rbxisolntrks[nrbxs]/I");
  tree_->Branch("rbxisolhcale",rbxisolhcale_,"rbxisolhcale[nrbxs]/F");
  tree_->Branch("rbxisolecale",rbxisolecale_,"rbxisolecale[nrbxs]/F");
  tree_->Branch("rbxisoltrke",rbxisoltrke_,"rbxisoltrke[nrbxs]/F");

  tree_->Branch("nhpds",&nhpds_,"nhpds/I");
  tree_->Branch("hpdnhits",hpdnhits_,"hpdnhits[nhpds]/I");
  tree_->Branch("hpde",hpde_,"hpde[nhpds]/F");
  tree_->Branch("hpdtrkfide",hpdtrkfide_,"hpdtrkfide[nhpds]/F");
  tree_->Branch("hpdisolnhcalhits",hpdisolnhcalhits_,"hpdisolnhcalhits[nhpds]/I");
  tree_->Branch("hpdisolnecalhits",hpdisolnecalhits_,"hpdisolnecalhits[nhpds]/I");
  tree_->Branch("hpdisolntrks",hpdisolntrks_,"hpdisolntrks[nhpds]/I");
  tree_->Branch("hpdisolhcale",hpdisolhcale_,"hpdisolhcale[nhpds]/F");
  tree_->Branch("hpdisolecale",hpdisolecale_,"hpdisolecale[nhpds]/F");
  tree_->Branch("hpdisoltrke",hpdisoltrke_,"hpdisoltrke[nhpds]/F");

  tree_->Branch("ndis",&ndis_,"ndis/I");
  tree_->Branch("dinhits",dinhits_,"dinhits[ndis]/I");
  tree_->Branch("die",die_,"die[ndis]/F");
  tree_->Branch("ditrkfide",ditrkfide_,"ditrkfide[ndis]/F");
  tree_->Branch("diisolnhcalhits",diisolnhcalhits_,"diisolnhcalhits[ndis]/I");
  tree_->Branch("diisolnecalhits",diisolnecalhits_,"diisolnecalhits[ndis]/I");
  tree_->Branch("diisolntrks",diisolntrks_,"diisolntrks[ndis]/I");
  tree_->Branch("diisolhcale",diisolhcale_,"diisolhcale[ndis]/F");
  tree_->Branch("diisolecale",diisolecale_,"diisolecale[ndis]/F");
  tree_->Branch("diisoltrke",diisoltrke_,"diisoltrke[ndis]/F");

  tree_->Branch("nmonos",&nmonos_,"nmonos/I");
  tree_->Branch("mononhits",mononhits_,"mononhits[nmonos]/I");
  tree_->Branch("monoe",monoe_,"monoe[nmonos]/F");
  tree_->Branch("monotrkfide",monotrkfide_,"monotrkfide[nmonos]/F");
  tree_->Branch("monoisolnhcalhits",monoisolnhcalhits_,"monoisolnhcalhits[nmonos]/I");
  tree_->Branch("monoisolnecalhits",monoisolnecalhits_,"monoisolnecalhits[nmonos]/I");
  tree_->Branch("monoisolntrks",monoisolntrks_,"monoisolntrks[nmonos]/I");
  tree_->Branch("monoisolhcale",monoisolhcale_,"monoisolhcale[nmonos]/F");
  tree_->Branch("monoisolecale",monoisolecale_,"monoisolecale[nmonos]/F");
  tree_->Branch("monoisoltrke",monoisoltrke_,"monoisoltrke[nmonos]/F");
}

void IsolatedNoise::endJob()
{
  rootfile_->cd();
  tree_->Write();
  rootfile_->Close();
}

//define this as a plug-in
DEFINE_FWK_MODULE(IsolatedNoise);

