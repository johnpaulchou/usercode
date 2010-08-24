#include "Analysis/MakeSkims/plugins/DileptonEventSelector.h"
#include "Analysis/MakeSkims/interface/ParseGenTree.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/PatCandidates/interface/TriggerPath.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
//#include "DataFormats/Math/interface/LorentzVector.h"

#include "TH1D.h"
#include "TTree.h"

using namespace pat;

DileptonEventSelector::DileptonEventSelector(edm::ParameterSet const& params)
  : edm::EDFilter(),
    genParticleSrc_(params.getParameter<edm::InputTag>("genParticleSrc")),
    minNumGenLeptons_(params.getParameter<int>("minNumGenLeptons")),
    motherPdgIds_(params.getParameter<std::vector<int> >("motherPdgIds")),
    requiredTriggers_(params.getParameter<std::vector<std::string> >("requiredTriggers")),
    vertexSrc_(params.getParameter<edm::InputTag>("vertexSrc")),
    minNumGoodVertices_(params.getParameter<int>("minNumGoodVertices")),
    noiseResultSrc_(params.getParameter<edm::InputTag>("noiseResultSrc")),
    trackSrc_(params.getParameter<edm::InputTag>("trackSrc")),
    minHighQualityTrackFraction_(params.getParameter<double>("minHighQualityTrackFraction")),
    minNumTracksForMonsterEventCut_(params.getParameter<int>("minNumTracksForMonsterEventCut")),
    patElectronSrc_(params.getParameter<edm::InputTag>("patElectronSrc")),
    patMuonSrc_(params.getParameter<edm::InputTag>("patMuonSrc")),
    minElectronEt_(params.getParameter<double>("minElectronEt")),
    minElectronSuperClusterEt_(params.getParameter<double>("minElectronSuperClusterEt")),
    minMuonPt_(params.getParameter<double>("minMuonPt")),
    excludeLepInvMass_(params.getParameter<std::vector<double> >("excludeLepInvMass")),
    excludeSameTypeLepInvMass_(params.getParameter<std::vector<double> >("excludeSameTypeLepInvMass"))
{

  // setup file service
  edm::Service<TFileService> fs;

  // setup tree
  mytree_ = fs->make<TTree>("dileptontree","Dilepton Event Selector TTree");
  mytree_->Branch("eventSelectionBitSet",&b_eventSelectionBitSet,"eventSelectionBitSet/I");
  mytree_->Branch("nGenLeps",&b_nGenLeps,"nGenLeps/I");
  mytree_->Branch("nGenTaus",&b_nGenTaus,"nGenTaus/I");
  mytree_->Branch("genLepPt",b_genLepPt,"genLepPt[nGenLeps]/F");
  mytree_->Branch("genLepEta",b_genLepEta,"genLepEta[nGenLeps]/F");
  mytree_->Branch("genLepPdgId",b_genLepPdgId,"genLepPdgId[nGenLeps]/I");
  mytree_->Branch("triggerBitSet",&b_triggerBitSet,"triggerBitSet/I");
  mytree_->Branch("nGoodVertices",&b_nGoodVertices,"nGoodVertices/I");
  mytree_->Branch("vertexNDOF",b_vertexNDOF,"vertexNDOF[nGoodVertices]/I");
  mytree_->Branch("vertexZ",b_vertexZ,"vertexZ[nGoodVertices]/F");
  mytree_->Branch("vertexRho",b_vertexRho,"vertexRho[nGoodVertices]/F");
  mytree_->Branch("nTracks",&b_nTracks,"nTracks/I");
  mytree_->Branch("nHighPurityTracks",&b_nHighPurityTracks,"nHighPurityTracks/I");
  mytree_->Branch("nRecoLeps",&b_nRecoLeps,"nRecoLeps/I");
  mytree_->Branch("recoLepPt",b_recoLepPt,"recoLepPt[nRecoLeps]/F");
  mytree_->Branch("recoLepEta",b_recoLepEta,"recoLepEta[nRecoLeps]/F");
  mytree_->Branch("recoLepPdgId",b_recoLepPdgId,"recoLepPdgId[nRecoLeps]/I");
  mytree_->Branch("foundRecoLepPair",&b_foundRecoLepPair,"foundRecoLepPair/I");
  mytree_->Branch("nExtraRecoLeps",&b_nExtraRecoLeps,"nExtraRecoLeps/I");
  mytree_->Branch("recoLepPairInvMass",&b_recoLepPairInvMass,"recoLepPairInvMass/F");


  // save the names of the cuts used for the event selection (for reference for b_eventSelectionBitSet)
  h_eventCutNames = fs->make<TH1D>("eventCutNames","Event Selection Cut Names",7, -0.5, 6.5);
  h_eventCutNames->GetXaxis()->SetBinLabel(1, "minNumGenLeptons");
  h_eventCutNames->GetXaxis()->SetBinLabel(2, "foundTrigger");
  h_eventCutNames->GetXaxis()->SetBinLabel(3, "minNumGoodVertices");
  h_eventCutNames->GetXaxis()->SetBinLabel(4, "HBHENoise");
  h_eventCutNames->GetXaxis()->SetBinLabel(5, "MonsterEvent");
  h_eventCutNames->GetXaxis()->SetBinLabel(6, "RecoLeptonPair");
  h_eventCutNames->GetXaxis()->SetBinLabel(7, "InvariantMass");


  // save the name of the triggers in a histogram (for reference for b_triggerBitSet)
  h_triggerNames = fs->make<TH1D>("triggerNames","Trigger Names",requiredTriggers_.size(), -0.5, requiredTriggers_.size()-0.5);
  for(unsigned int i=0; i<requiredTriggers_.size(); i++)
    h_triggerNames->GetXaxis()->SetBinLabel(i+1, requiredTriggers_[i].c_str());
  
  // sort the mother Ids
  for(unsigned int i=0; i<motherPdgIds_.size(); i++)
    motherPdgIdSet_.insert(motherPdgIds_[i]);
}


bool DileptonEventSelector::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  b_eventSelectionBitSet=0;

  ////////////////////////////////////////////
  // do the generator lepton identification
  ////////////////////////////////////////////

  b_nGenLeps=0;
  b_nGenTaus=0;
  if(!iEvent.isRealData()) {

    // get the gen particle collection
    edm::Handle<reco::GenParticleCollection > genp_h;
    iEvent.getByLabel(genParticleSrc_, genp_h);

    if(!(genp_h.isValid())) {
      edm::LogError("DataNotFound") << "genParticles not found";
      return false;
    }

    // count the tauonic daughters of the mothers
    std::vector<const reco::GenParticle*> taus;
    ParseGenTree::findChargedLeptonicDaughters(*genp_h, motherPdgIdSet_, taus);

    // fill the tree
    for(unsigned int i=0; i<taus.size() && i<maxGenTaus_; i++) {
      if(abs(taus[i]->pdgId())==15) {
	b_genTauPt[i]=taus[i]->pt();
	b_genTauEta[i]=taus[i]->eta();
	b_genTauPdgId[i]=taus[i]->pdgId();
	++b_nGenTaus;
      }
    }


    // find the leptonic daughters of the mothers (parse the taus, if need be)
    std::vector<const reco::GenParticle*> daughters;
    ParseGenTree::findChargedLeptonicDaughtersParseTaus(*genp_h, motherPdgIdSet_, daughters);

    // fill the tree
    for(unsigned int i=0; i<daughters.size() && i<maxGenLeps_; i++) {
      b_genLepPt[i]=daughters[i]->pt();
      b_genLepEta[i]=daughters[i]->eta();
      b_genLepPdgId[i]=daughters[i]->pdgId();
      ++b_nGenLeps;
    }

    if(b_nGenLeps<minNumGenLeptons_) b_eventSelectionBitSet |= 0x1;
  }

  ////////////////////////////////////////////
  // trigger selection
  ////////////////////////////////////////////

  // get the trigger
  edm::Handle<TriggerPathCollection> tp_h;
  iEvent.getByType(tp_h);
  if(!tp_h.isValid()) {
    edm::LogError("DataNotFound") << "patTriggerPath not found";
    return false;
  }

  // loop over the triggers and find a match
  b_triggerBitSet=0;
  for(TriggerPathCollection::const_iterator it=tp_h->begin(); it!=tp_h->end(); ++it) {
    const TriggerPath *path = &(*it);
    for(unsigned int i=0; i<requiredTriggers_.size(); i++)
      if(path->name()==requiredTriggers_[i] && path->wasAccept())
	b_triggerBitSet |= (0x1 << i);
  }
  if(b_triggerBitSet==0) b_eventSelectionBitSet |= 0x2;

  ////////////////////////////////////////////
  // primary vertex selection
  ////////////////////////////////////////////

  b_nGoodVertices=0;
  edm::Handle< std::vector<reco::Vertex> > vertices_h;
  iEvent.getByLabel(vertexSrc_, vertices_h);
  if(vertices_h.isValid()) {
    for(unsigned int i=0; i<vertices_h->size() && i<maxGoodVertices_; i++) {
      ++b_nGoodVertices;
      b_vertexNDOF[i] = vertices_h->at(i).ndof();
      b_vertexZ[i] = vertices_h->at(i).z();
      b_vertexRho[i] = vertices_h->at(i).position().rho();
    }
  }

  if(b_nGoodVertices<minNumGoodVertices_) b_eventSelectionBitSet |= 0x4;

  ////////////////////////////////////////////
  // noise filter selection
  ////////////////////////////////////////////

  edm::Handle<bool> noiseresult_h;
  iEvent.getByLabel(noiseResultSrc_, noiseresult_h);
  if(!noiseresult_h.isValid()) {
    edm::LogError("DataNotFound") << "noise result not found";
    return false;
  }
  if(*noiseresult_h==false) b_eventSelectionBitSet |= 0x8;


  ////////////////////////////////////////////
  // monster event selection
  ////////////////////////////////////////////

  edm::Handle<reco::TrackCollection> tracks_h;
  iEvent.getByLabel(trackSrc_,tracks_h);
  if(!tracks_h.isValid()) {
    edm::LogError("DataNotFound") << "track collection not found";
    return false;
  }
  b_nTracks=tracks_h->size();
  b_nHighPurityTracks=0;
  for(unsigned int i=0; i<tracks_h->size(); i++)
    if(tracks_h->at(i).quality(reco::TrackBase::qualityByName("highPurity")))
      ++b_nHighPurityTracks;
  double fraction=b_nHighPurityTracks>0 ? static_cast<double>(b_nTracks)/b_nHighPurityTracks : 0.0;
  if(b_nTracks>=minNumTracksForMonsterEventCut_ && fraction<minHighQualityTrackFraction_)
    b_eventSelectionBitSet |= 0x10;


  ////////////////////////////////////////////
  // lepton selection
  ////////////////////////////////////////////

  edm::Handle<std::vector<pat::Electron> > eles_h;
  edm::Handle<std::vector<pat::Muon> > muos_h;
  iEvent.getByLabel(patElectronSrc_, eles_h);
  iEvent.getByLabel(patMuonSrc_, muos_h);
  if(!eles_h.isValid()) {
    edm::LogError("DataNotFound") << "pat electrons not found";
    return false;
  }
  if(!muos_h.isValid()) {
    edm::LogError("DataNotFound") << "pat muons not found";
    return false;
  }

  // select the leptons and order them by pt
  b_nRecoLeps=0;
  std::set<lepton_t> leptons;
  for(std::vector<pat::Electron>::const_iterator it=eles_h->begin(); it!=eles_h->end(); ++it) {
    b_recoLepPt[b_nRecoLeps] = it->et();
    b_recoLepEta[b_nRecoLeps] = it->eta();
    b_recoLepPdgId[b_nRecoLeps] = it->charge()<0 ? 11 : -11;
    if(b_nRecoLeps<static_cast<int>(maxRecoLeps_)) ++b_nRecoLeps;

    if(it->et()<minElectronEt_) continue;
    if(it->superCluster()->energy()/cosh(it->superCluster()->eta())<minElectronSuperClusterEt_) continue;
    leptons.insert(lepton_t(&(*it)));
  }
  for(std::vector<pat::Muon>::const_iterator it=muos_h->begin(); it!=muos_h->end(); ++it) {
    b_recoLepPt[b_nRecoLeps] = it->pt();
    b_recoLepEta[b_nRecoLeps] = it->eta();
    b_recoLepPdgId[b_nRecoLeps] = it->charge()<0 ? 13 : -13;
    if(b_nRecoLeps<static_cast<int>(maxRecoLeps_)) ++b_nRecoLeps;

    if(it->pt()<minMuonPt_) continue;
    leptons.insert(lepton_t(&(*it)));
  }

  // leptons are now ordered; count them and pick the highest pt opposite-signed pair
  b_foundRecoLepPair=0;
  b_nExtraRecoLeps=0;
  lepton_t lep1, lep2;
  for(std::set<lepton_t>::const_iterator it=leptons.begin(); it!=leptons.end(); ++it) {
    if(it==leptons.begin()) {
      lep1=(*it);
    } else if(it->q_ * lep1.q_<0 && b_foundRecoLepPair==0) { // opposite-charge
      b_foundRecoLepPair=1;
      lep2=(*it);
    } else {
      ++b_nExtraRecoLeps;
    }
  }
  if(b_foundRecoLepPair!=1) b_eventSelectionBitSet |= 0x20;

  ////////////////////////////////////////////
  // invariant mass selection
  ////////////////////////////////////////////

  b_recoLepPairInvMass=-999.0;
  if(lep1.q_!=0 && lep2.q_!=0) {
    math::XYZTLorentzVector p1=lep1.e_ ? lep1.e_->p4() : lep1.m_->p4();
    math::XYZTLorentzVector p2=lep2.e_ ? lep2.e_->p4() : lep2.m_->p4();
    p1+=p2;
    b_recoLepPairInvMass=p1.mass();

    // exclude invariant masses
    for(unsigned int i=0; i<excludeLepInvMass_.size(); i+=2) {
      if(i+1>=excludeLepInvMass_.size()) break;
      if(b_recoLepPairInvMass>excludeLepInvMass_[i] && b_recoLepPairInvMass<excludeLepInvMass_[i+1])
	b_eventSelectionBitSet |= 0x40;
    }

    // exclude invariant masses of the same type
    if((lep1.e_ && lep2.e_) || (lep1.m_ && lep2.m_)) {
      for(unsigned int i=0; i<excludeSameTypeLepInvMass_.size(); i+=2) {
	if(i+1>=excludeSameTypeLepInvMass_.size()) break;
	if(b_recoLepPairInvMass>excludeSameTypeLepInvMass_[i] && b_recoLepPairInvMass<excludeSameTypeLepInvMass_[i+1])
	  b_eventSelectionBitSet |= 0x40;
      }
    }
  }


  // fill the tree regardless of the outcome
  mytree_->Fill();

  return b_eventSelectionBitSet==0;
}

DEFINE_FWK_MODULE(DileptonEventSelector);
