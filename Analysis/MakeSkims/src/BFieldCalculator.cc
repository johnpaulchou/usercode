#include "Analysis/MakeSkims/interface/BFieldCalculator.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/Scalers/interface/DcsStatus.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include <iostream>

double BFieldCalculator::calculate(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  if(iEvent.isRealData()) {
    edm::Handle<DcsStatusCollection> dcsHandle;
    iEvent.getByLabel("scalersRawToDigi", dcsHandle);
    if(!dcsHandle.isValid() || dcsHandle->size()==0) return 3.8;

    // scale factor = 3.801/18166.0 which are average values taken over a stable two week period
    double currentToBFieldScaleFactor = 2.09237036221512717e-04;
    double current = (*dcsHandle)[0].magnetCurrent();
    return current*currentToBFieldScaleFactor;
  } else {
    edm::ESHandle<MagneticField> magneticField;
    iSetup.get<IdealMagneticFieldRecord>().get(magneticField);
    return magneticField->inTesla(GlobalPoint(0.,0.,0.)).z();
  }
}
