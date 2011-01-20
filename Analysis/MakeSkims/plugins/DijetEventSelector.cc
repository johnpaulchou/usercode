#include "Analysis/MakeSkims/plugins/DijetEventSelector.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include "TH1D.h"
#include "TTree.h"

using namespace pat;

DijetEventSelector::DijetEventSelector(edm::ParameterSet const& params) :
  edm::EDFilter(),
  HLTSrc_(params.getParameter<edm::InputTag>("HLTSrc")),
  vertexSrc_(params.getParameter<edm::InputTag>("vertexSrc")),
  minVertexNDOF_(params.getParameter<int>("minVertexNDOF")),
  maxVertexZ_(params.getParameter<double>("maxVertexZ")),
  maxVertexRho_(params.getParameter<double>("maxVertexRho")),
  doNoiseStep_(params.getParameter<bool>("doNoiseStep")),
  noiseResultSrc_(params.getParameter<edm::InputTag>("noiseResultSrc")),
  trackSrc_(params.getParameter<edm::InputTag>("trackSrc")),
  minHighQualityTrackFraction_(params.getParameter<double>("minHighQualityTrackFraction")),
  minNumTracksForMonsterEventCut_(params.getParameter<int>("minNumTracksForMonsterEventCut")),
  jetSrc_(params.getParameter<edm::InputTag>("jetSrc")),
  leadJetMinRawPt_(params.getParameter<double>("leadJetMinRawPt")),
  leadJetMinCorrPt_(params.getParameter<double>("leadJetMinCorrPt")),
  leadJetMaxAbsEta_(params.getParameter<double>("leadJetMaxAbsEta")),
  subLeadJetMinRawPt_(params.getParameter<double>("subLeadJetMinRawPt")),
  subLeadJetMinCorrPt_(params.getParameter<double>("subLeadJetMinCorrPt")),
  subLeadJetMaxAbsEta_(params.getParameter<double>("subLeadJetMaxAbsEta")),
  minDijetMass_(params.getParameter<double>("minDijetMass")),
  maxDijetDeta_(params.getParameter<double>("maxDijetDeta"))
{
  
  // setup file service
  edm::Service<TFileService> fs;

  // setup tree
  mytree_ = fs->make<TTree>("dijettree","Dijet Event Selector TTree");
  mytree_->Branch("nrun",&b_nrun,"nrun/I");
  mytree_->Branch("nlumi",&b_nlumi,"nlumi/I");
  mytree_->Branch("nevent",&b_nevent,"nevent/I");
  mytree_->Branch("eventSelectionBitSet",&b_eventSelectionBitSet,"eventSelectionBitSet/I");
  mytree_->Branch("triggerBitSet",&b_triggerBitSet,"triggerBitSet/I");
  mytree_->Branch("nGoodVertices",&b_nGoodVertices,"nGoodVertices/I");
  mytree_->Branch("vertexNDOF",b_vertexNDOF,"vertexNDOF[nGoodVertices]/I");
  mytree_->Branch("vertexZ",b_vertexZ,"vertexZ[nGoodVertices]/F");
  mytree_->Branch("vertexRho",b_vertexRho,"vertexRho[nGoodVertices]/F");
  mytree_->Branch("nTracks",&b_nTracks,"nTracks/I");
  mytree_->Branch("nHighPurityTracks",&b_nHighPurityTracks,"nHighPurityTracks/I");
  mytree_->Branch("nJets",&b_nJets,"nJets/I");
  mytree_->Branch("leadJet",&b_leadJet,"leadJet/I");
  mytree_->Branch("subLeadJet",&b_subLeadJet,"subLeadJet/I");
  mytree_->Branch("jetRawPt",b_jetRawPt,"jetRawPt[nJets]/F");
  mytree_->Branch("jetCorrPt",b_jetCorrPt,"jetCorrPt[nJets]/F");
  mytree_->Branch("jetEta",b_jetEta,"jetEta[nJets]/F");
  mytree_->Branch("dijetMass",&b_dijetMass,"dijetMass/F");
  mytree_->Branch("dijetDeta",&b_dijetDeta,"dijetDeta/F");

  // setup histograms
  hTriggerBitSetDefs = fs->make<TH1D>("hTriggerBitSetDefs","Trigger Bit Set Definitions",50,-0.5,49.5);
  hEventSelectionBitSetDefs = fs->make<TH1D>("hEventSelectionBitSetDefs","Event Selection Bit Set Definitions", 7,0.5,7.5);
  hEventSelectionBitSetDefs->GetXaxis()->SetBinLabel(1,"trigger");
  hEventSelectionBitSetDefs->GetXaxis()->SetBinLabel(2,"vertices");
  hEventSelectionBitSetDefs->GetXaxis()->SetBinLabel(3,"noise");
  hEventSelectionBitSetDefs->GetXaxis()->SetBinLabel(4,"monster events");
  hEventSelectionBitSetDefs->GetXaxis()->SetBinLabel(5,"leading jet");
  hEventSelectionBitSetDefs->GetXaxis()->SetBinLabel(6,"subleading jet");
  hEventSelectionBitSetDefs->GetXaxis()->SetBinLabel(7,"dijet");
}

bool DijetEventSelector::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  ////////////////////////////////////////////
  // setup stuff
  ////////////////////////////////////////////

  b_eventSelectionBitSet=0;
  b_nrun=iEvent.id().run();
  b_nlumi=iEvent.luminosityBlock();
  b_nevent=iEvent.id().event();

  ////////////////////////////////////////////
  // trigger selection
  ////////////////////////////////////////////

  b_triggerBitSet=0;
  if(iEvent.isRealData()) {
    edm::Handle<edm::TriggerResults> triggerResults_h;
    iEvent.getByLabel(edm::InputTag(HLTSrc_), triggerResults_h);
    
    // get the jet trigger names
    const edm::TriggerNames &triggerNames = iEvent.triggerNames(*triggerResults_h);
    std::vector<std::string> names=triggerNames.triggerNames();
    
    bool foundJet50U=false, foundJet100U=false, foundJet140Uv1=false, foundJet140Uv3=false;
    for(unsigned int i=0; i<names.size(); i++) {
      
      // this is a jet trigger that was fired
      if(triggerResults_h->accept(i) &&
	 (names[i].find("Jet")!=std::string::npos)) {
	
	// if a jet trigger has not been used before, add it to the map in order of its addition
	if(jetTriggerMap.count(names[i])==0) {
	  unsigned int bit=jetTriggerMap.size();
	  jetTriggerMap[names[i]]=bit;
	  hTriggerBitSetDefs->GetXaxis()->SetBinLabel(bit+1, names[i].c_str());
	}
	
	// mark this trigger as fired
	b_triggerBitSet |= (1 << jetTriggerMap[names[i]]);
	if(names[i]=="HLT_Jet50U") foundJet50U=true;
	if(names[i]=="HLT_Jet100U") foundJet100U=true;
	if(names[i]=="HLT_Jet140U_v1") foundJet140Uv1=true;
	if(names[i]=="HLT_Jet140U_v3") foundJet140Uv3=true;
      }
    }
    if(b_nrun<=144114 && !foundJet50U) b_eventSelectionBitSet |= 0x1;
    if(b_nrun>144114 && b_nrun<=147116 && !foundJet100U) b_eventSelectionBitSet |= 0x1;
    if(b_nrun>147116 && b_nrun<=148058 && !foundJet140Uv1) b_eventSelectionBitSet |= 0x1;
    if(b_nrun>148058 && !foundJet140Uv3) b_eventSelectionBitSet |= 0x1;
  }

  ////////////////////////////////////////////
  // primary vertex selection
  ////////////////////////////////////////////

  b_nGoodVertices=0;
  edm::Handle< std::vector<reco::Vertex> > vertices_h;
  iEvent.getByLabel(vertexSrc_, vertices_h);
  for(unsigned int i=0; i<vertices_h->size() && i<maxGoodVertices_; i++) {
    ++b_nGoodVertices;
    b_vertexNDOF[i] = vertices_h->at(i).ndof();
    b_vertexZ[i] = vertices_h->at(i).z();
    b_vertexRho[i] = vertices_h->at(i).position().rho();
  }
  if(b_nGoodVertices<1 ||
     b_vertexNDOF[0]<minVertexNDOF_ ||
     fabs(b_vertexZ[0])>maxVertexZ_ ||
     fabs(b_vertexRho[0])>maxVertexRho_)  b_eventSelectionBitSet |= 0x2;

  ////////////////////////////////////////////
  // noise filter selection
  ////////////////////////////////////////////

  if(doNoiseStep_) {
    edm::Handle<bool> noiseresult_h;
    iEvent.getByLabel(noiseResultSrc_, noiseresult_h);
    if(*noiseresult_h==false) b_eventSelectionBitSet |= 0x4;
  }

  ////////////////////////////////////////////
  // monster event selection
  ////////////////////////////////////////////

  edm::Handle<reco::TrackCollection> tracks_h;
  iEvent.getByLabel(trackSrc_,tracks_h);
  b_nTracks=tracks_h->size();
  b_nHighPurityTracks=0;
  for(unsigned int i=0; i<tracks_h->size(); i++)
    if(tracks_h->at(i).quality(reco::TrackBase::qualityByName("highPurity")))
      ++b_nHighPurityTracks;
  double fraction=b_nHighPurityTracks>0 ? static_cast<double>(b_nTracks)/b_nHighPurityTracks : 0.0;
  if(b_nTracks>=minNumTracksForMonsterEventCut_ && fraction<minHighQualityTrackFraction_)
    b_eventSelectionBitSet |= 0x8;

  ////////////////////////////////////////////
  // jet selection
  ////////////////////////////////////////////

  edm::Handle<std::vector<pat::Jet> > jets_h;
  iEvent.getByLabel(jetSrc_, jets_h);

  // fill the tree, first
  b_nJets=jets_h->size();
  for(unsigned int i=0; i<jets_h->size(); i++) {
    const Jet *jet=&jets_h->at(i);
    b_jetRawPt[i] = jet->correctedP4(JetCorrFactors::Raw).Pt();
    b_jetCorrPt[i] = jet->correctedP4(JetCorrFactors::L3).Pt();
    b_jetEta[i] = jet->correctedP4(JetCorrFactors::L3).Eta();
  }

  // find the leading jet
  b_leadJet=-1;
  for(int i=0; i<b_nJets; i++) {
    if(b_jetRawPt[i]<leadJetMinRawPt_) continue;
    if(b_jetCorrPt[i]<leadJetMinCorrPt_) continue;
    if(fabs(b_jetEta[i])>leadJetMaxAbsEta_) continue;
    if(b_leadJet<0 || b_jetCorrPt[i]>b_jetCorrPt[b_leadJet])
      b_leadJet=i;
  }

  // find the subleading jet
  b_subLeadJet=-1;
  for(int i=0; i<b_nJets; i++) {
    if(i==b_leadJet) continue;
    if(b_jetRawPt[i]<subLeadJetMinRawPt_) continue;
    if(b_jetCorrPt[i]<subLeadJetMinCorrPt_) continue;
    if(fabs(b_jetEta[i])>subLeadJetMaxAbsEta_) continue;
    if(b_subLeadJet<0 || b_jetCorrPt[i]>b_jetCorrPt[b_subLeadJet])
      b_subLeadJet=i;
  }

  if(b_leadJet<0) b_eventSelectionBitSet |= 0x10;
  if(b_subLeadJet<0) b_eventSelectionBitSet |= 0x20;
  
  ////////////////////////////////////////////
  // dijet selection
  ////////////////////////////////////////////

  b_dijetMass=-999.0;
  b_dijetDeta=999.0;
  if(b_leadJet>=0 && b_subLeadJet>=0) {
    math::XYZTLorentzVector p1=jets_h->at(b_leadJet).correctedP4(JetCorrFactors::L3);
    math::XYZTLorentzVector p2=jets_h->at(b_subLeadJet).correctedP4(JetCorrFactors::L3);
    math::XYZTLorentzVector p=p1+p2;
    b_dijetMass=p.mass();
    b_dijetDeta=fabs(p1.Eta()-p2.Eta());
  }

  if(b_dijetMass<minDijetMass_ || b_dijetDeta>maxDijetDeta_) b_eventSelectionBitSet |= 0x40;


  ////////////////////////////////////////////
  // we're done
  ////////////////////////////////////////////

  mytree_->Fill();
  return (b_eventSelectionBitSet==0);
}  

DEFINE_FWK_MODULE(DijetEventSelector);

