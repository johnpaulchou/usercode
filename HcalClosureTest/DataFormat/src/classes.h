#include "HcalClosureTest/DataFormats/interface/SingleParticleCluster.h"
#include "DataFormats/Common/interface/Wrapper.h"

namespace {
  struct dictionary {
    
    SingleParticleCluster dummy1;
    SingleParticleClusterCollection dummy2;
    edm::Wrapper<SingleParticleClusterCollection> dummy3;
  };
}
