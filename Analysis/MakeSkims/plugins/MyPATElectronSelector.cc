#include "Analysis/MakeSkims/plugins/MyPATElectronSelector.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "Analysis/MakeSkims/interface/ImpactParameterCalculator.h"

using namespace pat;

MyPATElectronSelector::MyPATElectronSelector(const edm::ParameterSet& params)
  : edm::EDFilter(),
    src_(params.getParameter<edm::InputTag>("src") ),
    vertexSrc_(params.getParameter<edm::InputTag>("vertexSrc") ),
    isolatedLabel_(params.getParameter<std::string>("isolatedLabel") ),
    nonisolatedLabel_(params.getParameter<std::string>("nonisolatedLabel") ),
    electronID_(params.getParameter<std::string>("electronID") ),
    minEt_(params.getParameter<double>("minEt") ),
    maxEta_(params.getParameter<double>("maxEta") ),
    maxNumLostHits_(params.getParameter<int>("maxNumLostHits") ),
    maxTransverseIP_(params.getParameter<double>("maxTransverseIP") ),
    maxIsolation_(params.getParameter<double>("maxIsolation") ),
    muonCleaningSrc_(params.getParameter<edm::InputTag>("muonCleaningSrc") ),
    muonCleaningIDs_(params.getParameter<std::vector<std::string> >("muonCleaningIDs") ),
    muonCleaningMinNumValidHits_(params.getParameter<unsigned int>("muonCleaningMinNumValidHits") ),
    muonCleaningMaxDr_(params.getParameter<double>("muonCleaningMaxDr") )
{
  produces< std::vector<pat::Electron> >(isolatedLabel_);
  produces< std::vector<pat::Electron> >(nonisolatedLabel_);
}

bool MyPATElectronSelector::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // we are producing the following electrons
  std::auto_ptr< std::vector<pat::Electron> > isolatedPatElectrons(new std::vector<pat::Electron>() );
  std::auto_ptr< std::vector<pat::Electron> > nonisolatedPatElectrons(new std::vector<pat::Electron>() );

  // the electrons we are going to select from
  edm::Handle< edm::View<pat::Electron> > h_electrons;
  iEvent.getByLabel(src_, h_electrons);
  if(!h_electrons.isValid()) {
    edm::LogWarning("DataNotFound") << "patElectrons with tag " << src_.encode() << " not found in the event.";
    return false;
  }

  // the muons we use for a special cleaning
  edm::Handle< edm::View<pat::Muon> > h_muons;
  iEvent.getByLabel(muonCleaningSrc_, h_muons);
  if(!h_muons.isValid()) {
    edm::LogWarning("DataNotFound") << "patMuons with tag " << muonCleaningSrc_.encode() << " not found in the event.";
    return false;
  }

  // setup some tools
  ImpactParameterCalculator ipcalc(iEvent, iSetup, vertexSrc_ );

  // loop over the electrons
  for(edm::View<pat::Electron>::const_iterator it=h_electrons->begin(); it!=h_electrons->end(); ++it) {
    pat::Electron ele=(*it);

    if(ele.electronID(electronID_)<0.5) continue;
    if(ele.et()<=minEt_) continue;
    if(fabs(ele.eta())>=maxEta_) continue;
    if(ele.gsfTrack()->trackerExpectedHitsInner().numberOfLostHits()>=maxNumLostHits_) continue;

    // i.p. selection
    std::pair<bool, Measurement1D> val=ipcalc.absoluteTransverseImpactParameter(*ele.gsfTrack());
    if(!val.first) continue;
    ele.setDB(val.second.value(), val.second.error());
    if(ele.dB()>=maxTransverseIP_) continue;

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

    // isolation selection
    double isolation = (ele.trackIso() + ele.ecalIso() + ele.hcalIso()) / ele.et();
    bool isolated = isolation<maxIsolation_;
    
    // this is a good electron
    if(isolated) isolatedPatElectrons->push_back(ele);
    else         nonisolatedPatElectrons->push_back(ele);
    
  }

  iEvent.put(isolatedPatElectrons, isolatedLabel_);
  iEvent.put(nonisolatedPatElectrons, nonisolatedLabel_);

  return true;
}

DEFINE_FWK_MODULE(MyPATElectronSelector);
