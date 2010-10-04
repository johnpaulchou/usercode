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

#include "Analysis/MakeTrees/interface/datasets.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/MET.h"

const pat::JetCorrFactors::CorrStep JetCorrLevel=pat::JetCorrFactors::L3;
const std::string jetcoll="selectedPatJetsAK7Calo";
const std::string metcoll="patMETs";
const std::string trkcntcoll="trackCountingHighEffBJetTags";
const std::string secvtxcoll="simpleSecondaryVertexHighEffBJetTags";

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
    std::cout << "    anatype==0: data" << std::endl;
    return 0;
  }
  const int numanatypes=1;
  int anatype=std::atoi(argv[1]);
  if(anatype<0 || anatype>=numanatypes) {
    std::cout << "Invalid anatype" << std::endl;
    return 0;
  }

  int nsplits=1, split=0;
  if(argc>=2) nsplits=std::atoi(argv[2]);
  if(argc>=3) split=std::atoi(argv[3]);

  ////////////////////////////////////////////////////////////////////////////////
  // setup the tree and histograms
  ////////////////////////////////////////////////////////////////////////////////

  TString rootfn;
  if(anatype==0) rootfn="data";
  rootfn+="_"+TString(argv[2])+"_"+TString(argv[3])+".root";
  TFile* rootfile = new TFile(rootfn, "RECREATE");
  rootfile->cd();

  TTree* mytree = new TTree("dijettree","dijettree");
  int b_nrun, b_nlumi, b_nevent;
  int b_leadJet, b_subLeadJet, b_nJets;
  const int maxjets=100;
  double b_jetRawPt[maxjets], b_jetCorrPt[maxjets], b_jetEta[maxjets], b_jetPhi[maxjets], b_jetEMF[maxjets], b_jetIDEMF[maxjets], b_jetFHPD[maxjets];
  int b_jetN90Hits[maxjets], b_jetNTrkVtx[maxjets], b_jetPassJetID[maxjets];
  double b_jetSecVtxTag[maxjets], b_jetTrkCntTag[maxjets];
  double b_dijetMass, b_dijetDeta, b_dijetDphi;
  double b_met, b_sumEt, b_metSig;
  int b_passSelection;

  mytree->Branch("nrun",&b_nrun,"nrun/I");
  mytree->Branch("nlumi",&b_nlumi,"nlumi/I");
  mytree->Branch("nevent",&b_nevent,"nevent/I");
  mytree->Branch("nJets",&b_nJets,"nJets/I");
  mytree->Branch("leadJet",&b_leadJet,"leadJet/I");
  mytree->Branch("subLeadJet",&b_subLeadJet,"subLeadJet/I");
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
  mytree->Branch("jetSecVtxTag",b_jetSecVtxTag,"jetSecVtxTag[nJets]/D");
  mytree->Branch("jetTrkCntTag",b_jetTrkCntTag,"jetTrkCntTag[nJets]/D");
  mytree->Branch("dijetMass",&b_dijetMass,"dijetMass/D");
  mytree->Branch("dijetDeta",&b_dijetDeta,"dijetDeta/D");
  mytree->Branch("dijetDphi",&b_dijetDphi,"dijetDphi/D");
  mytree->Branch("met",&b_met,"met/D");
  mytree->Branch("sumEt",&b_sumEt,"sumEt/D");
  mytree->Branch("metSig",&b_metSig,"metSig/D");
  mytree->Branch("passSelection",&b_passSelection,"passSelection/I");


  TH1D* hLumi=new TH1D("hLumi","Luminosity",1,-0.5,0.5);


  ////////////////////////////////////////////////////////////////////////////////
  // get the datasets we want to run over (indexed by anatype)
  ////////////////////////////////////////////////////////////////////////////////

  std::vector<std::string> datasetnames[numanatypes];
  datasetnames[0].push_back("JetMETPromptRecoV4DijetSelection370p4");
  datasetnames[0].push_back("JetMETTauPromptRecoV4DijetSelection370p4");
  datasetnames[0].push_back("JetMETTauJuly15thReRecoDijetSelection370p4");

  // get the relevant dataset containers
  datasetContainer datasetConts[numanatypes];
  for(int i=0; i<numanatypes; i++)
    datasets::getDatasets().getDatasetContainer(datasetnames[i], datasetConts[i]);

  // get the files we are going to run over (this is determined by the anatype)
  std::vector<std::string> filenames;
  datasetConts[anatype].getFiles(filenames, nsplits, split);
  std::cout << "We are going to run over " << filenames.size() << " files for anatype=" << anatype << std::endl;
  for(unsigned int i=0; i<filenames.size(); i++) {
    std::cout << filenames[i] << std::endl;
  }

  // get the luminosity of the data
  double lumi=datasetConts[0].getTotalLuminosity();
  double lumie=datasetConts[0].getTotalLuminosityError();
  hLumi->SetBinContent(1, lumi);
  hLumi->SetBinError(1, lumie);

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
    b_nJets=jets_h->size();
    for(unsigned int i=0; i<jets_h->size(); i++) {
      const pat::Jet *jet=&jets_h->at(i);
      b_jetRawPt[i] = jet->correctedP4(pat::JetCorrFactors::Raw).Pt();
      b_jetCorrPt[i] = jet->correctedP4(JetCorrLevel).Pt();
      b_jetEta[i] = jet->correctedP4(JetCorrLevel).Eta();
      b_jetPhi[i] = jet->correctedP4(JetCorrLevel).Phi();
      b_jetEMF[i] = jet->emEnergyFraction();
      b_jetIDEMF[i] = jet->jetID().restrictedEMF;
      b_jetFHPD[i] = jet->jetID().fHPD;
      b_jetN90Hits[i] = jet->jetID().n90Hits;
      b_jetNTrkVtx[i] = jet->associatedTracks().size();
      b_jetPassJetID[i] = (b_jetIDEMF[i]>0.01 && b_jetFHPD[i]<0.98 && b_jetN90Hits[i]>1);
      b_jetSecVtxTag[i] = jet->bDiscriminator(secvtxcoll);
      b_jetTrkCntTag[i] = jet->bDiscriminator(trkcntcoll);
    }

    // find the leading jet
    b_leadJet=-1;
    for(int i=0; i<b_nJets; i++) {
      if(fabs(b_jetEta[i])>2.5) continue;
      if(b_leadJet<0 || b_jetCorrPt[i]>b_jetCorrPt[b_leadJet])
	b_leadJet=i;
    }
    if(b_leadJet<0) continue;
    
    // find the subleading jet
    b_subLeadJet=-1;
    for(int i=0; i<b_nJets; i++) {
      if(i==b_leadJet) continue;
      if(fabs(b_jetEta[i])>2.5) continue;
      if(b_subLeadJet<0 || b_jetCorrPt[i]>b_jetCorrPt[b_subLeadJet])
	b_subLeadJet=i;
    }
    if(b_subLeadJet<0) continue;

    ////////////////////////////////////////////
    // dijet selection
    ////////////////////////////////////////////
    
    math::XYZTLorentzVector p1=jets_h->at(b_leadJet).correctedP4(JetCorrLevel);
    math::XYZTLorentzVector p2=jets_h->at(b_subLeadJet).correctedP4(JetCorrLevel);
    math::XYZTLorentzVector p=p1+p2;
    b_dijetMass=p.mass();
    b_dijetDeta=p1.Eta()-p2.Eta();
    b_dijetDphi=fabs(p1.Phi()-p2.Phi());
    if(b_dijetDphi>3.1415927) b_dijetDphi=6.29-b_dijetDphi;

    if(b_dijetMass<200.0) continue;
    
    b_passSelection = (b_dijetMass>354. && b_jetPassJetID[b_leadJet]==1 && b_jetPassJetID[b_subLeadJet]==1);

    mytree->Fill();
    
  }

  rootfile->cd();
  mytree->Write();
  hLumi->Write();
  rootfile->Close();

  return 0;
}
