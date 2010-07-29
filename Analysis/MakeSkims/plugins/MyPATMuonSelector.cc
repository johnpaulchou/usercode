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
    muonID_(params.getParameter<std::string>("muonID") ),
    minPt_(params.getParameter<double>("minPt") ),
    maxEta_(params.getParameter<double>("maxEta") ),
    minNumValidHits_(params.getParameter<unsigned int>("minNumValidHits") ),
    maxNormChi2_(params.getParameter<double>("maxNormChi2") ),
    maxIsolation_(params.getParameter<double>("maxIsolation") ),
    maxTransverseIP_(params.getParameter<double>("maxTransverseIP") )
{
  produces< std::vector<pat::Muon> >(isolatedLabel_);
  produces< std::vector<pat::Muon> >(nonisolatedLabel_);
}

bool MyPATMuonSelector::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // we are producing the following muons
  std::auto_ptr< std::vector<pat::Muon> > isolatedPatMuons(new std::vector<pat::Muon>() );
  std::auto_ptr< std::vector<pat::Muon> > nonisolatedPatMuons(new std::vector<pat::Muon>() );

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

    // select
    if(!mu.muonID(muonID_)) continue;
    std::cout << mu.pt() << " " << mu.globalTrack()->pt() << std::endl;
    if(mu.pt()<=minPt_) continue;
    if(fabs(mu.eta())>=maxEta_) continue;
    if(mu.numberOfValidHits()<minNumValidHits_) continue;
    if(mu.normChi2()>=maxNormChi2_) continue;

    // i.p. significance selection
    std::pair<bool, Measurement1D> val=ipcalc.absoluteTransverseImpactParameter(*mu.innerTrack());
    if(!val.first) continue;
    mu.setDB(val.second.value(), val.second.error());
    if(mu.dB()>maxTransverseIP_) continue;

    // isolation selection
    double isolation = (mu.trackIso() + mu.ecalIso() + mu.hcalIso()) / mu.pt();
    bool isolated = isolation<maxIsolation_;
    
    // this is a good muon
    if(isolated) isolatedPatMuons->push_back(mu);
    else         nonisolatedPatMuons->push_back(mu);
  }
  
  iEvent.put(isolatedPatMuons, isolatedLabel_);
  iEvent.put(nonisolatedPatMuons, nonisolatedLabel_);

  return true;
  
}

DEFINE_FWK_MODULE(MyPATMuonSelector);


