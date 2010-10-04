#include "Analysis/MakeSkims/plugins/MyPATElectronSelector.h"
#include "Analysis/DataFormats/interface/MyPATElectron.h"
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
    requireCaloDrivenSeed_(params.getParameter<bool>("requireCaloDrivenSeed") ),
    minEt_(params.getParameter<double>("minEt") ),
    minSuperClusterEt_(params.getParameter<double>("minSuperClusterEt") ),
    maxEta_(params.getParameter<double>("maxEta") ),
    maxSwissCross_(params.getParameter<double>("maxSwissCross") ),
    muonCleaningSrc_(params.getParameter<edm::InputTag>("muonCleaningSrc") ),
    muonCleaningIDs_(params.getParameter<std::vector<std::string> >("muonCleaningIDs") ),
    muonCleaningMinNumValidHits_(params.getParameter<unsigned int>("muonCleaningMinNumValidHits") ),
    muonCleaningMaxDr_(params.getParameter<double>("muonCleaningMaxDr") )
{
  produces< std::vector<pat::MyPATElectron> >();
}

bool MyPATElectronSelector::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // we are producing the following electrons
  std::auto_ptr< std::vector<pat::MyPATElectron> > electrons(new std::vector<pat::MyPATElectron>() );

  // the electrons we are going to select from
  edm::Handle< edm::View<pat::Electron> > h_electrons;
  iEvent.getByLabel(src_, h_electrons);

  // get the EB rechits so we can do spike cleaning
  edm::Handle<EcalRecHitCollection> h_ebrechits;
  iEvent.getByLabel(EBRecHitsSrc_, h_ebrechits);

  // get the tracks so we can do conversion rejection
  edm::Handle<reco::TrackCollection> tracks_h;
  iEvent.getByLabel(tracksSrc_, tracks_h);

  // the muons we use for a special cleaning
  edm::Handle< edm::View<pat::Muon> > h_muons;
  iEvent.getByLabel(muonCleaningSrc_, h_muons);

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
    // finished basic selection

    // create and fill a new MyPATElectron
    MyPATElectron myEle(ele);

    // conversion stuff
    ConversionFinder convFinder;
    myEle.convinfo_ = convFinder.getConversionInfo(ele, tracks_h, bfield);

    // i.p. selection
    std::pair<bool, Measurement1D> val=ipcalc.absoluteTransverseImpactParameter(*ele.gsfTrack());
    if(val.first) {
      myEle.pv2dip_=val.second.value();
      myEle.pv2dipsig_=val.second.value()/val.second.error();
    } else {
      myEle.pv2dip_=myEle.pv2dipsig_=999.;
    }
    myEle.deltaZ_=ipcalc.deltaZ(*ele.gsfTrack());

    electrons->push_back(myEle);
  }

  iEvent.put(electrons);

  return true;
}

DEFINE_FWK_MODULE(MyPATElectronSelector);
