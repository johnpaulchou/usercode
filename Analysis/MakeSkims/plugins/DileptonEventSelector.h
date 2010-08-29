#ifndef __ANALYSIS_MAKESKIMS_DILEPTONEVENTSELECTOR_H__
#define __ANALYSIS_MAKESKIMS_DILEPTONEVENTSELECTOR_H__

#include "FWCore/Framework/interface/EDFilter.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"

#include <string>

// forward declarations
class TTree;
class TH1D;

namespace pat {

  class DileptonEventSelector : public edm::EDFilter {
  public:
    DileptonEventSelector(edm::ParameterSet const& params);
    virtual ~DileptonEventSelector() {}
    virtual void beginJob() {}
    virtual void endJob() {}

    virtual bool filter(edm::Event& iEvent, const edm::EventSetup& iSetup);

  protected:

    // parameters
    edm::InputTag genParticleSrc_;
    int minNumGenLeptons_;
    std::vector<int> motherPdgIds_;
    std::set<int> motherPdgIdSet_;
    std::vector<std::string> requiredTriggers_;
    edm::InputTag vertexSrc_;
    int minNumGoodVertices_;
    bool doNoiseStep_;
    edm::InputTag noiseResultSrc_;
    edm::InputTag trackSrc_;
    double minHighQualityTrackFraction_;
    int minNumTracksForMonsterEventCut_;
    edm::InputTag patElectronSrc_;
    edm::InputTag patMuonSrc_;
    double minElectronEt_;
    double minElectronSuperClusterEt_;
    double minMuonPt_;
    std::vector<double> excludeLepInvMass_;
    std::vector<double> excludeSameTypeLepInvMass_;
    
    // Tree and histograms to be filled
    TTree* mytree_;
    int b_eventSelectionBitSet;
    static const unsigned int maxGenLeps_=100;
    int b_nGenLeps;
    float b_genLepPt[maxGenLeps_];
    float b_genLepEta[maxGenLeps_];
    int b_genLepPdgId[maxGenLeps_];
    static const unsigned int maxGenTaus_=100;
    int b_nGenTaus;
    float b_genTauPt[maxGenTaus_];
    float b_genTauEta[maxGenTaus_];
    int b_genTauPdgId[maxGenTaus_];
    int b_triggerBitSet;
    static const unsigned int maxGoodVertices_=50;
    int b_nGoodVertices;
    int b_vertexNDOF[maxGoodVertices_];
    float b_vertexZ[maxGoodVertices_];
    float b_vertexRho[maxGoodVertices_];
    int b_nTracks;
    int b_nHighPurityTracks;
    static const unsigned int maxRecoLeps_=200;
    int b_nRecoLeps;
    float b_recoLepPt[maxRecoLeps_];
    float b_recoLepEta[maxRecoLeps_];
    int b_recoLepPdgId[maxRecoLeps_];
    int b_foundRecoLepPair;
    int b_nExtraRecoLeps;
    float b_recoLepPairInvMass;

    TH1D* h_triggerNames;
    TH1D* h_eventCutNames;

    struct lepton_t {
      lepton_t() {
	pt_=-999;
	q_=0;
	e_=0;
	m_=0;
      }
      lepton_t(const Electron* e) {
	pt_=e->et();
	q_=e->charge()>0 ? 1 : -1;
	e_=e;
	m_=0;
      }
      lepton_t(const Muon* m) {
	pt_=m->pt();
	q_=m->charge()>0 ? 1 : -1;
	e_=0;
	m_=m;
      }
      ~lepton_t() {}
      inline bool isEle(void) const { return e_!=0; }
      inline bool isMuo(void) const { return m_!=0; }
      double pt_;
      const Electron* e_;
      const Muon* m_;
      int q_;

      bool operator<(const lepton_t& l) const {
	return pt_>l.pt_;
      }
    };


  };
}

#endif
