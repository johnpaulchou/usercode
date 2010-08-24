#include "Analysis/MakeSkims/plugins/MyPATElectronSelector.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"

#include "RecoEgamma/EgammaTools/interface/ConversionFinder.h"

#include "Analysis/MakeSkims/interface/BFieldCalculator.h"
#include "Analysis/MakeSkims/interface/ImpactParameterCalculator.h"

using namespace pat;

MyPATElectronSelector::MyPATElectronSelector(const edm::ParameterSet& params)
  : edm::EDFilter(),
    src_(params.getParameter<edm::InputTag>("src") ),
    vertexSrc_(params.getParameter<edm::InputTag>("vertexSrc") ),
    tracksSrc_(params.getParameter<edm::InputTag>("tracksSrc") ),
    EBRecHitsSrc_(params.getParameter<edm::InputTag>("EBRecHitsSrc") ),
    isolatedLabel_(params.getParameter<std::string>("isolatedLabel") ),
    nonisolatedLabel_(params.getParameter<std::string>("nonisolatedLabel") ),
    looseLabel_(params.getParameter<std::string>("looseLabel") ),
    requireCaloDrivenSeed_(params.getParameter<bool>("requireCaloDrivenSeed") ),
    minEt_(params.getParameter<double>("minEt") ),
    minSuperClusterEt_(params.getParameter<double>("minSuperClusterEt") ),
    maxEta_(params.getParameter<double>("maxEta") ),
    maxSuperClusterEta_(params.getParameter<double>("maxSuperClusterEta") ),
    maxNumLostHits_(params.getParameter<int>("maxNumLostHits") ),
    maxTransverseIP_(params.getParameter<double>("maxTransverseIP") ),
    maxDeltaZ_(params.getParameter<double>("maxDeltaZ") ),
    maxSwissCross_(params.getParameter<double>("maxSwissCross") ),
    conversionMinSep_(params.getParameter<double>("conversionMinSep") ),
    conversionMinDcot_(params.getParameter<double>("conversionMinDcot") ),
    maxSihihEB_(params.getParameter<double>("maxSihihEB") ),
    maxDphiEB_(params.getParameter<double>("maxDphiEB") ),
    maxDetaEB_(params.getParameter<double>("maxDetaEB") ),
    maxHoeEB_(params.getParameter<double>("maxHoeEB") ),
    maxSihihEE_(params.getParameter<double>("maxSihihEE") ),
    maxDphiEE_(params.getParameter<double>("maxDphiEE") ),
    maxDetaEE_(params.getParameter<double>("maxDetaEE") ),
    maxHoeEE_(params.getParameter<double>("maxHoeEE") ),
    maxIsolation_(params.getParameter<double>("maxIsolation") ),
    muonCleaningSrc_(params.getParameter<edm::InputTag>("muonCleaningSrc") ),
    muonCleaningIDs_(params.getParameter<std::vector<std::string> >("muonCleaningIDs") ),
    muonCleaningMinNumValidHits_(params.getParameter<unsigned int>("muonCleaningMinNumValidHits") ),
    muonCleaningMaxDr_(params.getParameter<double>("muonCleaningMaxDr") )
{
  produces< std::vector<pat::Electron> >(isolatedLabel_);
  produces< std::vector<pat::Electron> >(nonisolatedLabel_);
  produces< std::vector<pat::Electron> >(looseLabel_);
}

bool MyPATElectronSelector::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // we are producing the following electrons
  std::auto_ptr< std::vector<pat::Electron> > isolatedPatElectrons(new std::vector<pat::Electron>() );
  std::auto_ptr< std::vector<pat::Electron> > nonisolatedPatElectrons(new std::vector<pat::Electron>() );
  std::auto_ptr< std::vector<pat::Electron> > loosePatElectrons(new std::vector<pat::Electron>() );

  // the electrons we are going to select from
  edm::Handle< edm::View<pat::Electron> > h_electrons;
  iEvent.getByLabel(src_, h_electrons);
  if(!h_electrons.isValid()) {
    edm::LogWarning("DataNotFound") << "patElectrons with tag " << src_.encode() << " not found in the event.";
    return false;
  }

  // get the EB rechits so we can do spike cleaning
  edm::Handle<EcalRecHitCollection> h_ebrechits;
  iEvent.getByLabel(EBRecHitsSrc_, h_ebrechits);

  // get the tracks so we can do conversion rejection
  edm::Handle<reco::TrackCollection> tracks_h;
  iEvent.getByLabel(tracksSrc_, tracks_h);

  // the muons we use for a special cleaning
  edm::Handle< edm::View<pat::Muon> > h_muons;
  iEvent.getByLabel(muonCleaningSrc_, h_muons);
  if(!h_muons.isValid()) {
    edm::LogWarning("DataNotFound") << "patMuons with tag " << muonCleaningSrc_.encode() << " not found in the event.";
    return false;
  }

  // setup some tools
  ImpactParameterCalculator ipcalc(iEvent, iSetup, vertexSrc_ );
  EcalSeverityLevelAlgo ecalseverity;

  // calculate the b-field
  double bfield=BFieldCalculator::calculate(iEvent, iSetup);

  // loop over the electrons
  for(edm::View<pat::Electron>::const_iterator it=h_electrons->begin(); it!=h_electrons->end(); ++it) {
    pat::Electron ele=(*it);

    // et calculation
    double eleet = ele.p4().Pt();

    // basic selection
    if(requireCaloDrivenSeed_ && !ele.ecalDrivenSeed()) continue;
    if(eleet<minEt_) continue;
    if(ele.superCluster()->energy()/cosh(ele.superCluster()->eta())<minSuperClusterEt_) continue;
    if(fabs(ele.eta())>=maxEta_) continue;
    if(fabs(ele.superCluster()->eta())>=maxSuperClusterEta_) continue;

    // conversion rejection
    if(ele.gsfTrack()->trackerExpectedHitsInner().numberOfLostHits()>=maxNumLostHits_) continue;
    ConversionFinder convFinder;
    ConversionInfo convInfo = convFinder.getConversionInfo(ele, tracks_h, bfield);
    if(convFinder.isFromConversion(conversionMinSep_, conversionMinDcot_)) continue;

    // do swiss-cross cleaning
    const DetId seedId=ele.superCluster()->seed()->seed();
    double swissCross = ecalseverity.swissCross(seedId, *h_ebrechits);
    if(swissCross>=maxSwissCross_) continue;

    // muon cleaning - loop over the muons
    bool foundMuon=false;
    for(edm::View<pat::Muon>::const_iterator muon_it=h_muons->begin(); muon_it!=h_muons->end(); ++muon_it) {
      const pat::Muon *muptr=&(*muon_it);
      bool goodMuon=false;
      for(unsigned int i=0; i<muonCleaningIDs_.size(); ++i) {
	if(muptr->muonID(muonCleaningIDs_[i])) {
	  goodMuon=true;
	  break;
	}
      }
      if(!goodMuon) continue;
      if(muptr->numberOfValidHits()<muonCleaningMinNumValidHits_) continue;
      double dR = deltaR(ele.eta(), ele.phi(), muptr->eta(), muptr->phi());
      if(dR<muonCleaningMaxDr_) {
	foundMuon=true;
	break;
      }
    }
    if(foundMuon) continue;
    // finished loose selection

    //    bool isTight=true;
    int isTight=0;

    // i.p. selection
    std::pair<bool, Measurement1D> val=ipcalc.absoluteTransverseImpactParameter(*ele.gsfTrack());
    if(val.first) ele.setDB(val.second.value(), val.second.error());
    else          ele.setDB(999., 999.);
    if(ele.dB()>=maxTransverseIP_) isTight |= 0x1;
    if(fabs(ipcalc.deltaZ(*ele.gsfTrack()))>=maxDeltaZ_) isTight |=0x2;

    // do electron ID
    double sihih   = ele.sigmaIetaIeta();
    double dphi    = ele.deltaPhiSuperClusterTrackAtVtx();
    double deta    = ele.deltaEtaSuperClusterTrackAtVtx();
    double HoE     = ele.hadronicOverEm();

    // do EB cuts
    if(ele.isEB()) {
      if(sihih >= maxSihihEB_) isTight |= 0x4;
      if(fabs(dphi) >= maxDphiEB_) isTight |= 0x8;
      if(fabs(deta) >= maxDetaEB_) isTight |= 0x10;
      if(HoE >= maxHoeEB_) isTight |= 0x20;

    // do EE cuts
    } else {
      if(sihih >= maxSihihEE_) isTight |= 0x4;
      if(fabs(dphi) >= maxDphiEE_) isTight |= 0x8;
      if(fabs(deta) >= maxDetaEE_) isTight |= 0x10;
      if(HoE >= maxHoeEE_) isTight |= 0x20;
    }

    // isolation selection
    double trackIso = ele.dr03TkSumPt()/eleet;
    double ecalIso = ele.dr03EcalRecHitSumEt()/eleet;
    double hcalIso = ele.dr03HcalTowerSumEt()/eleet; 
    double EBIso = (trackIso + std::max(0., ecalIso-1.0) + hcalIso) / std::max(20., eleet);
    double EEIso = (trackIso + ecalIso + hcalIso) / std::max(20., eleet);
    bool isolated = ele.isEB() ? EBIso<maxIsolation_ : EEIso<maxIsolation_;
    
    // this is a good electron
    if(isolated && isTight==0) isolatedPatElectrons->push_back(ele);
    else if(isTight==0)        nonisolatedPatElectrons->push_back(ele);
    else                       loosePatElectrons->push_back(ele);
  }

  iEvent.put(isolatedPatElectrons, isolatedLabel_);
  iEvent.put(nonisolatedPatElectrons, nonisolatedLabel_);
  iEvent.put(loosePatElectrons, looseLabel_);

  return true;
}

DEFINE_FWK_MODULE(MyPATElectronSelector);
