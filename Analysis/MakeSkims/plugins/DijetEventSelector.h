#ifndef __ANALYSIS_MAKESKIMS_DIJETEVENTSELECTOR_H__
#define __ANALYSIS_MAKESKIMS_DIJETEVENTSELECTOR_H__

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/EDFilter.h"

#include "DataFormats/PatCandidates/interface/Jet.h"

#include <string>

// forward declarations
class TTree;
class TH1D;

namespace pat {

  class DijetEventSelector : public edm::EDFilter {
  public:
    DijetEventSelector(edm::ParameterSet const& params);
    virtual ~DijetEventSelector() {}
    virtual void beginJob() {}
    virtual void endJob() {}
    
    virtual bool filter(edm::Event& iEvent, const edm::EventSetup& iSetup);

  protected:

    // parameters
    edm::InputTag HLTSrc_;
    edm::InputTag vertexSrc_;
    int minVertexNDOF_;
    double maxVertexZ_;
    double maxVertexRho_;
    bool doNoiseStep_;
    edm::InputTag noiseResultSrc_;
    edm::InputTag trackSrc_;
    double minHighQualityTrackFraction_;
    int minNumTracksForMonsterEventCut_;
    edm::InputTag jetSrc_;
    double leadJetMinRawPt_, leadJetMinCorrPt_, leadJetMaxAbsEta_;
    double subLeadJetMinRawPt_, subLeadJetMinCorrPt_, subLeadJetMaxAbsEta_;
    double minDijetMass_, maxDijetDeta_;

    std::map<std::string, int> jetTriggerMap;

    // Tree to be filled
    TTree* mytree_;
    int b_nrun;
    int b_nlumi;
    int b_nevent;
    int b_eventSelectionBitSet;
    int b_triggerBitSet;
    static const unsigned int maxGoodVertices_=50;
    int b_nGoodVertices;
    int b_vertexNDOF[maxGoodVertices_];
    float b_vertexZ[maxGoodVertices_];
    float b_vertexRho[maxGoodVertices_];
    int b_nTracks;
    int b_nHighPurityTracks;
    static const unsigned int maxJets_=500;
    int b_nJets;
    int b_leadJet;
    int b_subLeadJet;
    float b_jetRawPt[maxJets_];
    float b_jetCorrPt[maxJets_];
    float b_jetEta[maxJets_];
    float b_dijetMass;
    float b_dijetDeta;

    // Histograms to be filled
    TH1D* hTriggerBitSetDefs;
    TH1D* hEventSelectionBitSetDefs;

  };


}

#endif
