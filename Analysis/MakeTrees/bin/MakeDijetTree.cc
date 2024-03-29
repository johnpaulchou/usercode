////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// MakeDijetTree:
//     Make a tree for the dijet analyses
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////


#include <memory>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>

#include <TTree.h>
#include <TH1F.h>
#include <TROOT.h>
#include <TFile.h>
#include <TSystem.h>
#include <TString.h>

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"

#include "Analysis/DataFormats/interface/MyPATMuon.h"
#include "Analysis/MakeTrees/interface/datasets.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

//const std::string jetcoll="selectedPatJetsAK7Calo";
const std::string jetcoll="selectedPatJetsAK7PF";
const std::string metcoll="patMETs";
const std::string muoncoll="selectedPatMuons";
const double jetptcut=30.0;
const double dijetmasscut=500.0;
const double dijetdetacut=2.0;

enum ANATYPES { ANA_DATA=0, ANA_JETMC, NANATYPES };

int main(int argc, char* argv[])
{

  ////////////////////////////////////////////////////////////////////////////////
  // basic setup
  ////////////////////////////////////////////////////////////////////////////////

  using namespace std;
  using namespace edm;

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  // get analysis to work on
  if ( argc < 2 ) {
    std::cout << "Usage: " << argv[0] << " anatype [nsplits] [split]" << std::endl;
    return 0;
  }
  int anatype=std::atoi(argv[1]);
  if(anatype<-1 || anatype>=NANATYPES) {
    std::cout << "Invalid anatype" << std::endl;
    return 0;
  }

  int nsplits=1, split=0;
  if(anatype>=0) {
    if(argc>=2) nsplits=std::atoi(argv[2]);
    if(argc>=3) split=std::atoi(argv[3]);
  }

  ////////////////////////////////////////////////////////////////////////////////
  // setup the tree and histograms
  ////////////////////////////////////////////////////////////////////////////////

  TString rootfn;
  if(anatype==-1) rootfn="temp.root";
  if(anatype==ANA_DATA) rootfn="data";
  if(anatype==ANA_JETMC) rootfn="jetMC";
  if(anatype>=0) rootfn+="_"+TString(argv[2])+"_"+TString(argv[3])+".root";
  TFile* rootfile = new TFile(rootfn, "RECREATE");
  rootfile->cd();

  TTree* mytree = new TTree("dijettree","dijettree");
  int b_nrun, b_nlumi, b_nevent, b_datasetID;
  const int maxstrsize=100;
  char b_datasetName[maxstrsize];
  double b_lumi, b_lumiErr;
  double b_eventWeight;
  int b_nJets;
  const int maxjets=100;
  double b_jetRawPt[maxjets], b_jetCorrPt[maxjets], b_jetEta[maxjets], b_jetPhi[maxjets], b_jetEMF[maxjets], b_jetIDEMF[maxjets], b_jetFHPD[maxjets];
  int b_jetN90Hits[maxjets], b_jetNTrkVtx[maxjets], b_jetPassJetID[maxjets], b_jetFlavor[maxjets];
  double b_jetSecVtxTagLooseDisc[maxjets], b_jetTrkCntTagLooseDisc[maxjets], b_jetSecVtxTagTightDisc[maxjets], b_jetTrkCntTagTightDisc[maxjets];
  double b_dijetMass, b_dijetDeta, b_dijetDphi;
  double b_met, b_sumEt, b_metSig;
  int b_nMuons;
  const int maxmuons=100;
  double b_muonPt[maxmuons], b_muonEta[maxmuons], b_muonPhi[maxmuons], b_muonDeltaZ[maxmuons], b_muonD0[maxmuons], b_muonNormChi2[maxmuons];
  int b_muonJetMatch[maxmuons], b_muonNValidHits[maxmuons], b_muonNValidMuonHits[maxmuons];

  mytree->Branch("nrun",&b_nrun,"nrun/I");
  mytree->Branch("nlumi",&b_nlumi,"nlumi/I");
  mytree->Branch("nevent",&b_nevent,"nevent/I");
  mytree->Branch("datasetID",&b_datasetID,"datasetID/I");
  mytree->Branch("datasetName",b_datasetName,"datasetName[100]/C"); // note that this magic number is put in by hand
  mytree->Branch("eventWeight",&b_eventWeight,"eventWeight/D");
  mytree->Branch("lumi",&b_lumi,"lumi/D");
  mytree->Branch("lumiErr",&b_lumiErr,"lumiErr/D");
  mytree->Branch("nJets",&b_nJets,"nJets/I");
  mytree->Branch("jetRawPt",b_jetRawPt,"jetRawPt[nJets]/D");
  mytree->Branch("jetCorrPt",b_jetCorrPt,"jetCorrPt[nJets]/D");
  mytree->Branch("jetEta",b_jetEta,"jetEta[nJets]/D");
  mytree->Branch("jetPhi",b_jetPhi,"jetPhi[nJets]/D");
  mytree->Branch("jetEMF",b_jetEMF,"jetEMF[nJets]/D");
  mytree->Branch("jetIDEMF",b_jetIDEMF,"jetIDEMF[nJets]/D");
  mytree->Branch("jetFHPD",b_jetFHPD,"jetFHPD[nJets]/D");
  mytree->Branch("jetN90Hits",b_jetN90Hits,"jetN90Hits[nJets]/I");
  mytree->Branch("jetNTrkVtx",b_jetNTrkVtx,"jetNTrkVtx[nJets]/I");
  mytree->Branch("jetPassJetID",b_jetPassJetID,"jetPassJetID[nJets]/I");
  mytree->Branch("jetFlavor",b_jetFlavor,"jetFlavor[nJets]/I");
  mytree->Branch("jetSecVtxTagLooseDisc",b_jetSecVtxTagLooseDisc,"jetSecVtxTagLooseDisc[nJets]/D");
  mytree->Branch("jetTrkCntTagLooseDisc",b_jetTrkCntTagLooseDisc,"jetTrkCntTagLooseDisc[nJets]/D");
  mytree->Branch("jetSecVtxTagTightDisc",b_jetSecVtxTagTightDisc,"jetSecVtxTagTightDisc[nJets]/D");
  mytree->Branch("jetTrkCntTagTightDisc",b_jetTrkCntTagTightDisc,"jetTrkCntTagTightDisc[nJets]/D");
  mytree->Branch("dijetMass",&b_dijetMass,"dijetMass/D");
  mytree->Branch("dijetDeta",&b_dijetDeta,"dijetDeta/D");
  mytree->Branch("dijetDphi",&b_dijetDphi,"dijetDphi/D");
  mytree->Branch("met",&b_met,"met/D");
  mytree->Branch("sumEt",&b_sumEt,"sumEt/D");
  mytree->Branch("metSig",&b_metSig,"metSig/D");
  mytree->Branch("nMuons",&b_nMuons,"nMuons/I");
  mytree->Branch("muonPt",b_muonPt,"muonPt[nMuons]/D");
  mytree->Branch("muonEta",b_muonEta,"muonEta[nMuons]/D");
  mytree->Branch("muonPhi",b_muonPhi,"muonPhi[nMuons]/D");
  mytree->Branch("muonDeltaZ",b_muonDeltaZ,"muonDeltaZ[nMuons]/D");
  mytree->Branch("muonD0",b_muonD0,"muonD0[nMuons]/D");
  mytree->Branch("muonJetMatch",b_muonJetMatch,"muonJetMatch[nMuons]/I");
  mytree->Branch("muonNValidHits",b_muonNValidHits,"muonNValidHits[nMuons]/I");
  mytree->Branch("muonNValidMuonHits",b_muonNValidMuonHits,"muonNValidMuonHits[nMuons]/I");
  mytree->Branch("muonNormChi2",b_muonNormChi2,"muonNormChi2[nMuons]/D");

  ////////////////////////////////////////////////////////////////////////////////
  // get the datasets we want to run over (indexed by anatype)
  ////////////////////////////////////////////////////////////////////////////////

  std::vector<std::string> datasetnames[NANATYPES];
  datasetnames[ANA_DATA].push_back("JetMETTauNov4thReRecoDijetSelection385p3");
  datasetnames[ANA_DATA].push_back("JetMETNov4thReRecoDijetSelection385p3");
  datasetnames[ANA_DATA].push_back("JetNov4thReRecoDijetSelection385p3");

  datasetnames[ANA_JETMC].push_back("QCDPt80to120Pythia6Fall10DijetSelection385p3");
  datasetnames[ANA_JETMC].push_back("QCDPt120to170Pythia6Fall10DijetSelection385p3");
  datasetnames[ANA_JETMC].push_back("QCDPt170to300Pythia6Fall10DijetSelection385p3");
  datasetnames[ANA_JETMC].push_back("QCDPt300to470Pythia6Fall10DijetSelection385p3");
  datasetnames[ANA_JETMC].push_back("QCDPt470to600Pythia6Fall10DijetSelection385p3");
  datasetnames[ANA_JETMC].push_back("QCDPt600to800Pythia6Fall10DijetSelection385p3");
  datasetnames[ANA_JETMC].push_back("QCDPt800to1000Pythia6Fall10DijetSelection385p3");
  datasetnames[ANA_JETMC].push_back("QCDPt1000to1400Pythia6Fall10DijetSelection385p3");
  datasetnames[ANA_JETMC].push_back("QCDPt1400to1800Pythia6Fall10DijetSelection385p3");
  datasetnames[ANA_JETMC].push_back("QCDPt1800Pythia6Fall10DijetSelection385p3");

  // get the relevant dataset containers
  datasetContainer datasetConts[NANATYPES];
  for(int i=0; i<NANATYPES; i++)
    datasets::getDatasets().getDatasetContainer(datasetnames[i], datasetConts[i]);

  // get the files we are going to run over (this is determined by the anatype)
  std::vector<std::string> filenames;
  if(anatype>=0) datasetConts[anatype].getFiles(filenames, nsplits, split);
  else {
    for(int j=2; j<argc; j++) {
      filenames.push_back(argv[j]);
    }
  }
  std::cout << "We are going to run over " << filenames.size() << " files for anatype=" << anatype << std::endl;
  for(unsigned int i=0; i<filenames.size(); i++) {
    std::cout << filenames[i] << std::endl;
  }

  // get the luminosity of the data
  b_lumi=datasetConts[ANA_DATA].getTotalLuminosity();
  b_lumiErr=datasetConts[ANA_DATA].getTotalLuminosityError();
  std::cout << "luminosity = " << b_lumi << " +/- " << b_lumiErr << std::endl;

  ////////////////////////////////////////////////////////////////////////////////
  // loop over the events
  ////////////////////////////////////////////////////////////////////////////////

  cout << "Starting loop." << endl;
  unsigned int iEvent=0;
  fwlite::ChainEvent ev(filenames);
  int numevents=ev.size();
  for(ev.toBegin(); !ev.atEnd(); ++ev, ++iEvent){
    edm::EventBase const & event = ev;

    // simple event counter
    if(iEvent%1000==0){
      std::cout << "processing event " << iEvent << " out of " << numevents << std::endl;
    }

    b_nrun=ev.id().run();
    b_nlumi=ev.luminosityBlock();
    b_nevent=ev.id().event();

    // determine the event weight by looking up the dataset through the filename
    b_eventWeight=1.0;
    b_datasetID=-1;
    if(anatype>=0) {
      std::string theFileName=ev.getTFile()->GetName();
      const dataset theDataset=datasetConts[anatype].findDatasetByFile(theFileName);
      if(theDataset.getIsMC()) b_eventWeight = b_lumi*theDataset.getXSection()/theDataset.getNumEvents();
      b_datasetID=theDataset.getDatasetId();
      strcpy(b_datasetName, theDataset.getName().substr(0,maxstrsize-1).c_str());
    }
    
    // get the MET
    edm::Handle<std::vector<pat::MET> > met_h;
    event.getByLabel(edm::InputTag(metcoll), met_h);
    if(!met_h.isValid()) {
      std::cout << "Could not find met collection" << std::endl;
      continue;
    }
    
    b_met=met_h->front().pt();
    b_sumEt=met_h->front().sumEt();
    b_metSig=met_h->front().mEtSig();

    // get the jets
    edm::Handle<std::vector<pat::Jet> > jets_h;
    event.getByLabel(edm::InputTag(jetcoll), jets_h);
    if(!jets_h.isValid()) {
      std::cout << "Could not find jet collection" << std::endl;
      continue;
    }

    // fill the tree, first
    b_nJets=0;
    for(unsigned int i=0; i<jets_h->size(); i++) {
      const pat::Jet *jet=&jets_h->at(i);
      double corrpt = jet->pt();
      if(corrpt<jetptcut) continue;

      b_jetRawPt[b_nJets] = jet->correctedP4("Uncorrected").Pt();
      b_jetCorrPt[b_nJets] = corrpt;
      b_jetEta[b_nJets] = jet->eta();
      b_jetPhi[b_nJets] = jet->phi();
      if(jet->isCaloJet()) {
	b_jetEMF[b_nJets] = jet->emEnergyFraction();
	b_jetIDEMF[b_nJets] = jet->jetID().restrictedEMF;
	b_jetFHPD[b_nJets] = jet->jetID().fHPD;
	b_jetN90Hits[b_nJets] = jet->jetID().n90Hits;
      }
      b_jetNTrkVtx[b_nJets] = jet->associatedTracks().size();
      b_jetPassJetID[b_nJets] = (jet->isCaloJet() && b_jetIDEMF[b_nJets]>0.01 && b_jetFHPD[b_nJets]<0.98 && b_jetN90Hits[b_nJets]>1);
      b_jetFlavor[b_nJets] = jet->partonFlavour();

      b_jetSecVtxTagLooseDisc[b_nJets] = jet->bDiscriminator("simpleSecondaryVertexHighEffBJetTags");
      b_jetTrkCntTagLooseDisc[b_nJets] = jet->bDiscriminator("trackCountingHighEffBJetTags");
      b_jetSecVtxTagTightDisc[b_nJets] = jet->bDiscriminator("simpleSecondaryVertexHighPurBJetTags");
      b_jetTrkCntTagTightDisc[b_nJets] = jet->bDiscriminator("trackCountingHighPurBJetTags");

      ++b_nJets;
    }

    // require two jets
    if(b_nJets<2) continue;

    ////////////////////////////////////////////
    // dijet selection
    ////////////////////////////////////////////
    
    math::XYZTLorentzVector p1=jets_h->at(0).p4();
    math::XYZTLorentzVector p2=jets_h->at(1).p4();
    math::XYZTLorentzVector p=p1+p2;
    b_dijetMass=p.mass();
    b_dijetDeta=p1.Eta()-p2.Eta();
    b_dijetDphi=fabs(p1.Phi()-p2.Phi());
    if(b_dijetDphi>3.1415927) b_dijetDphi=6.29-b_dijetDphi;

    if(b_dijetMass<dijetmasscut) continue;
    if(fabs(b_dijetDeta)>dijetdetacut) continue;

    ////////////////////////////////////////////
    // muon selection
    ////////////////////////////////////////////
    
    // get the muons
    edm::Handle<std::vector<pat::MyPATMuon> > muons_h;
    event.getByLabel(edm::InputTag(muoncoll), muons_h);
    if(!muons_h.isValid()) {
      std::cout << "Could not find muon collection" << std::endl;
      continue;
    }

    b_nMuons=0;
    for(unsigned int i=0; i<muons_h->size(); i++) {
      const pat::MyPATMuon *mu=&muons_h->at(i);
      if(!mu->isGlobalMuon()) continue;
      //      if(!mu->muonID("GlobalMuonPromptTight")) continue;
      double pt=mu->globalTrack()->pt();
      double eta=mu->globalTrack()->eta();
      double phi =mu->globalTrack()->phi();
      if(pt<6.0) continue;
      //      if(fabs(mu->pv2dImpactParam())>0.02) continue;
      //      if(fabs(mu->deltaZ())>1.0) continue;

      double dR1=deltaR(eta, phi, b_jetEta[0], b_jetPhi[0]);
      double dR2=deltaR(eta, phi, b_jetEta[1], b_jetPhi[1]);

      b_muonPt[b_nMuons] = pt;
      b_muonEta[b_nMuons] = mu->globalTrack()->eta();
      b_muonPhi[b_nMuons] = mu->globalTrack()->phi();
      b_muonDeltaZ[b_nMuons] = mu->deltaZ();
      b_muonD0[b_nMuons] = mu->pv2dImpactParam();
      b_muonNormChi2[b_nMuons] = mu->normChi2();
      b_muonNValidHits[b_nMuons] = mu->numberOfValidHits();
      b_muonNValidMuonHits[b_nMuons] = mu->globalTrack()->hitPattern().numberOfValidMuonHits();

      if(dR1<0.7) b_muonJetMatch[b_nMuons]=0;
      else if(dR2<0.7) b_muonJetMatch[b_nMuons]=1;
      else b_muonJetMatch[b_nMuons]=-1;

      ++b_nMuons;
    }
    
    
    mytree->Fill();
    
  }
  
  rootfile->cd();
  mytree->Write();
  rootfile->Close();
  
  return 0;
}
  
