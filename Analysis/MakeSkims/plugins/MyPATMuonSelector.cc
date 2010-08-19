#include "Analysis/MakeSkims/plugins/MyPATMuonSelector.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "Analysis/MakeSkims/interface/ImpactParameterCalculator.h"

using namespace pat;

MyPATMuonSelector::MyPATMuonSelector(edm::ParameterSet const& params)
  : edm::EDFilter(),
    src_(params.getParameter<edm::InputTag>("src") ),
    vertexSrc_(params.getParameter<edm::InputTag>("vertexSrc") ),
    isolatedLabel_(params.getParameter<std::string>("isolatedLabel") ),
    nonisolatedLabel_(params.getParameter<std::string>("nonisolatedLabel") ),
    looseLabel_(params.getParameter<std::string>("looseLabel") ),
    muonID_(params.getParameter<std::string>("muonID") ),
    minPt_(params.getParameter<double>("minPt") ),
    maxEta_(params.getParameter<double>("maxEta") ),
    minNumValidHits_(params.getParameter<unsigned int>("minNumValidHits") ),
    minNumValidMuonHits_(params.getParameter<int>("minNumValidMuonHits") ),
    maxNormChi2_(params.getParameter<double>("maxNormChi2") ),
    maxIsolation_(params.getParameter<double>("maxIsolation") ),
    maxTransverseIP_(params.getParameter<double>("maxTransverseIP") ),
    maxDeltaZ_(params.getParameter<double>("maxDeltaZ") )
{
  produces< std::vector<pat::Muon> >(isolatedLabel_);
  produces< std::vector<pat::Muon> >(nonisolatedLabel_);
  produces< std::vector<pat::Muon> >(looseLabel_);
}

bool MyPATMuonSelector::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // we are producing the following muons
  std::auto_ptr< std::vector<pat::Muon> > isolatedPatMuons(new std::vector<pat::Muon>() );
  std::auto_ptr< std::vector<pat::Muon> > nonisolatedPatMuons(new std::vector<pat::Muon>() );
  std::auto_ptr< std::vector<pat::Muon> > loosePatMuons(new std::vector<pat::Muon>() );

  // the muons we are going to select from
  edm::Handle< edm::View<pat::Muon> > h_muons;
  iEvent.getByLabel(src_, h_muons);
  if(!h_muons.isValid()) {
    edm::LogWarning("DataNotFound") << "patMuons with tag " << src_.encode() << " not found in the event.";
    return false;
  }

  // setup some tools
  ImpactParameterCalculator ipcalc(iEvent, iSetup, vertexSrc_ );

  // loop over the muons
  for(edm::View<pat::Muon>::const_iterator it=h_muons->begin(); it!=h_muons->end(); ++it) {
    pat::Muon mu=(*it);

    // loose selection
    if(!(mu.muonID("AllGlobalMuons") || mu.muonID("AllTrackerMuons"))) continue;
    if(mu.globalTrack().isNonnull() && mu.globalTrack()->pt()<minPt_) continue;
    if(mu.globalTrack().isNull() && mu.track()->pt()<minPt_) continue;
    if(mu.globalTrack().isNonnull() && fabs(mu.globalTrack()->eta())>=maxEta_) continue;
    if(mu.globalTrack().isNull() && fabs(mu.track()->eta())>=maxEta_) continue;

    bool isTight=true;
    // tight selection
    if(!mu.isGlobalMuon()) isTight=false;
    if(!mu.muonID(muonID_)) isTight=false;
    if(mu.numberOfValidHits()<minNumValidHits_) isTight=false;
    if(mu.globalTrack().isNull() || mu.normChi2()>=maxNormChi2_) isTight=false;
    if(mu.globalTrack().isNull() || mu.globalTrack()->hitPattern().numberOfValidMuonHits()<minNumValidMuonHits_) isTight=false;

    // i.p. selection
    std::pair<bool, Measurement1D> val=ipcalc.absoluteTransverseImpactParameter(*mu.innerTrack());
    if(val.first) mu.setDB(val.second.value(), val.second.error());
    else          mu.setDB(999., 999.);
    if(mu.dB()>=maxTransverseIP_) isTight=false;
    if(fabs(ipcalc.deltaZ(*mu.innerTrack()))>=maxDeltaZ_) isTight=false;
    
    // isolation selection
    double isolation = (mu.trackIso() + mu.ecalIso() + mu.hcalIso()) / mu.pt();
    bool isolated = isolation<maxIsolation_;
    
    // this is a good muon
    if(isolated && isTight) isolatedPatMuons->push_back(mu);
    else if(isTight)        nonisolatedPatMuons->push_back(mu);
    else                    loosePatMuons->push_back(mu);
  }
  
  iEvent.put(isolatedPatMuons, isolatedLabel_);
  iEvent.put(nonisolatedPatMuons, nonisolatedLabel_);
  iEvent.put(loosePatMuons, looseLabel_);

  return true;
  
}

DEFINE_FWK_MODULE(MyPATMuonSelector);


