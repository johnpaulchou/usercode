#include "Analysis/DataFormats/interface/MyPATMuon.h"
#include "Analysis/DataFormats/interface/MyPATElectron.h"
#include "Analysis/DataFormats/interface/MyPATPhoton.h"
#include "RecoEgamma/EgammaTools/interface/ConversionInfo.h"

#include "DataFormats/Common/interface/Wrapper.h"
#include <vector>

namespace {
  struct dictionary {
    
    ConversionInfo ci;
    edm::Wrapper<ConversionInfo> w_ci;

    pat::MyPATMuon myp_m;
    std::vector<pat::MyPATMuon> v_myp_m;
    edm::Wrapper<pat::MyPATMuon> w_myp_m;
    edm::Wrapper<std::vector<pat::MyPATMuon> > w_v_myp_m;
    
    pat::MyPATElectron myp_e;
    std::vector<pat::MyPATElectron> v_myp_e;
    edm::Wrapper<pat::MyPATElectron> w_myp_e;
    edm::Wrapper<std::vector<pat::MyPATElectron> > w_v_myp_e;
    
    pat::MyPATPhoton myp_p;
    std::vector<pat::MyPATPhoton> v_myp_p;
    edm::Wrapper<pat::MyPATPhoton> w_myp_p;
    edm::Wrapper<std::vector<pat::MyPATPhoton> > w_v_myp_p;
    
  };
}
