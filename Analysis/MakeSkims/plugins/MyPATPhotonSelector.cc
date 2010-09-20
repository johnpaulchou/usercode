#include "Analysis/MakeSkims/plugins/MyPATPhotonSelector.h"
#include "Analysis/MakeSkims/interface/BFieldCalculator.h"
#include "Analysis/MakeSkims/interface/ImpactParameterCalculator.h"
#include "Analysis/DataFormats/interface/MyPATPhoton.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/Math/interface/Point3D.h"

#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"

using namespace pat;

MyPATPhotonSelector::MyPATPhotonSelector(const edm::ParameterSet& params)
  : edm::EDFilter(),
    src_(params.getParameter<edm::InputTag>("src") ),
    vertexSrc_(params.getParameter<edm::InputTag>("vertexSrc") ),
    EBRecHitsSrc_(params.getParameter<edm::InputTag>("EBRecHitsSrc") ),
    vetoPixelSeed_(params.getParameter<bool>("vetoPixelSeed") ),
    minEt_(params.getParameter<double>("minEt") ),
    maxEta_(params.getParameter<double>("maxEta") ),
    maxSwissCross_(params.getParameter<double>("maxSwissCross") ),
    muonCleaningSrc_(params.getParameter<edm::InputTag>("muonCleaningSrc") ),
    muonCleaningIDs_(params.getParameter<std::vector<std::string> >("muonCleaningIDs") ),
    muonCleaningMinNumValidHits_(params.getParameter<unsigned int>("muonCleaningMinNumValidHits") ),
    muonCleaningMaxDr_(params.getParameter<double>("muonCleaningMaxDr") ),
    embedConversions_(params.getParameter<bool>("embedConversions") )
{
  produces< std::vector<pat::MyPATPhoton> >();
}

bool MyPATPhotonSelector::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  // we are producing the following photons
  std::auto_ptr< std::vector<pat::MyPATPhoton> > photons(new std::vector<pat::MyPATPhoton>() );

  // the photons we are going to select from
  edm::Handle< edm::View<pat::Photon> > h_photons;
  iEvent.getByLabel(src_, h_photons);
  if(!h_photons.isValid()) {
    edm::LogWarning("DataNotFound") << "patPhotons with tag " << src_.encode() << " not found in the event.";
    return false;
  }

  // get the vertex
  edm::Handle<std::vector<reco::Vertex> > h_vertices;
  iEvent.getByLabel(vertexSrc_, h_vertices);
  reco::Vertex::Point vertex = reco::Vertex::Point(0,0,0);
  bool validVertex=false;
  if(h_vertices.isValid() && h_vertices->size()>0) {
    vertex=h_vertices->front().position();
    validVertex=true;
  }

  // get the EB rechits so we can do spike cleaning
  edm::Handle<EcalRecHitCollection> h_ebrechits;
  iEvent.getByLabel(EBRecHitsSrc_, h_ebrechits);

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

  // loop over the photons
  for(edm::View<pat::Photon>::const_iterator it=h_photons->begin(); it!=h_photons->end(); ++it) {
    pat::Photon pho=(*it);

    // first set the vertex before calculating any energies
    pho.setVertex(vertex);
    
    // basic selection
    if(vetoPixelSeed_ && pho.hasPixelSeed()) continue;
    if(pho.p4().Pt()<minEt_) continue;
    if(fabs(pho.p4().eta())>maxEta_) continue;
    if(!pho.isEB() && !pho.isEE()) continue;

    // do swiss-cross cleaning
    const DetId seedId=pho.superCluster()->seed()->seed();
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
      double dR = deltaR(pho.eta(), pho.phi(), muptr->eta(), muptr->phi());
      if(dR<muonCleaningMaxDr_) {
	foundMuon=true;
	break;
      }
    }
    if(foundMuon) continue;
    // finished basic selection

    // create and fill a new MyPATPhoton
    MyPATPhoton myPho(pho);
    myPho.validVertex_=validVertex;

    // embed conversions
    if(embedConversions_) {
      reco::ConversionRefVector convs=pho.photonCore()->conversions();
      for(reco::ConversionRefVector::const_iterator it=convs.begin(); it!=convs.end(); ++it) {
	myPho.conversions_.push_back(**it);
      }
    }

    photons->push_back(myPho);
  }

  iEvent.put(photons);
  return true;
}


DEFINE_FWK_MODULE(MyPATPhotonSelector);
