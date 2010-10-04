#include "Analysis/MakeSkims/plugins/MyPATMuonSelector.h"
#include "Analysis/DataFormats/interface/MyPATMuon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "Analysis/MakeSkims/interface/ImpactParameterCalculator.h"

using namespace pat;

MyPATMuonSelector::MyPATMuonSelector(edm::ParameterSet const& params)
  : edm::EDFilter(),
    src_(params.getParameter<edm::InputTag>("src") ),
    vertexSrc_(params.getParameter<edm::InputTag>("vertexSrc") ),
    minPt_(params.getParameter<double>("minPt") ),
    maxEta_(params.getParameter<double>("maxEta") )
{
  produces< std::vector<pat::MyPATMuon> >();
}

bool MyPATMuonSelector::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // we are producing the following muons
  std::auto_ptr< std::vector<pat::MyPATMuon> > muons(new std::vector<pat::MyPATMuon>() );

  // the muons we are going to select from
  edm::Handle< edm::View<pat::Muon> > h_muons;
  iEvent.getByLabel(src_, h_muons);

  // setup some tools
  ImpactParameterCalculator ipcalc(iEvent, iSetup, vertexSrc_ );

  // loop over the muons
  for(edm::View<pat::Muon>::const_iterator it=h_muons->begin(); it!=h_muons->end(); ++it) {
    pat::Muon mu=(*it);

    // basic selection
    if(!(mu.muonID("AllGlobalMuons") || mu.muonID("AllTrackerMuons"))) continue;
    if(mu.globalTrack().isNonnull() && mu.globalTrack()->pt()<minPt_) continue;
    if(mu.globalTrack().isNull() && mu.track()->pt()<minPt_) continue;
    if(mu.globalTrack().isNonnull() && fabs(mu.globalTrack()->eta())>=maxEta_) continue;
    if(mu.globalTrack().isNull() && fabs(mu.track()->eta())>=maxEta_) continue;


    // create and fill a new MyPATMuon
    MyPATMuon myMu(mu);

    // i.p. selection
    std::pair<bool, Measurement1D> val=ipcalc.absoluteTransverseImpactParameter(*mu.innerTrack());
    if(val.first) {
      myMu.pv2dip_=val.second.value();
      myMu.pv2dipsig_=val.second.value()/val.second.error();
    } else {
      myMu.pv2dip_=myMu.pv2dipsig_=999.;
    }
    myMu.deltaZ_=ipcalc.deltaZ(*mu.innerTrack());

    muons->push_back(myMu);
  }
  
  iEvent.put(muons);

  return true;
  
}

DEFINE_FWK_MODULE(MyPATMuonSelector);


