////////////////////////////////////////////////////////////////////////////////////////////////////
// 
// GetHLTriggers:
//     Lists all of the HLT triggers that were fired
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

#include <TH1F.h>
#include <TROOT.h>
#include <TFile.h>
#include <TSystem.h>

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/ChainEvent.h"
#include "FWCore/ParameterSet/interface/ProcessDesc.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "FWCore/PythonParameterSet/interface/PythonProcessDesc.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"

int main(int argc, char* argv[]) 
{
  using namespace std;
  using namespace edm;

  // load framework libraries
  gSystem->Load( "libFWCoreFWLite" );
  AutoLibraryLoader::enable();

  // get the list of files
  if ( argc < 2 ) {
    std::cout << "Usage : " << argv[0] << "[files...]" << std::endl;
    return 0;
  }
  vector<string> filenames;
  for(int i=1; i<argc; i++) {
    cout << "Processing file " << argv[i] << endl;
    filenames.push_back(argv[i]);
  }

  // trigger map
  map<string, int> trigmap;

  // loop over the events
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

    // get the HLT trigger info
    edm::Handle<TriggerResults> triggerResults_h;
    event.getByLabel(edm::InputTag("TriggerResults","","HLT"),triggerResults_h);
    if(!triggerResults_h.isValid()) continue;

    // get the trigger names
    const edm::TriggerNames &triggerNames = event.triggerNames(*triggerResults_h);
    vector<string> names=triggerNames.triggerNames();

    // loop over the triggers
    bool Jet15U=false, Jet15UNoise=false;
    for(unsigned int i=0; i<triggerResults_h->size(); i++) {
      if(triggerResults_h->accept(i)) {
	trigmap[names[i]]++;
	if(names[i]=="HLT_Jet15U") Jet15U=true;
	if(names[i]=="HLT_Jet15U_HcalNoiseFiltered") Jet15UNoise=true;
      }
    }
    if(Jet15U && !Jet15UNoise) {
      cout << "Jet15UNoise did not fire, but Jet15U did.\n"
	   << "file=" << ev.getTFile()->GetName()
	   << "; run=" << event.id().run()
	   << "; lumi=" << event.id().luminosityBlock()
	   << "; event=" << event.id().event() << endl;
    } else if(Jet15UNoise && !Jet15U) {
      cout << "WOAH!!!!   Jet15U did not fire, but Jet15UNoise did!!!\n"
	   << "file=" << ev.getTFile()->GetName()
	   << "; run=" << event.id().run()
	   << "; lumi=" << event.id().luminosityBlock()
	   << "; event=" << event.id().event() << endl;
    }
  }  

  // dump trigger results
  for(map<string, int>::const_iterator it=trigmap.begin(); it!=trigmap.end(); ++it) {
    cout << it->first << " " << it->second << endl;
  }

  // finished
  return 0;
}
