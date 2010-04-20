#ifndef HCALCOMPLICATEDRECALGO_H
#define HCALCOMPLICATEDRECALGO_H 1

#include "DataFormats/HcalDigi/interface/HBHEDataFrame.h"
#include "DataFormats/HcalDigi/interface/HFDataFrame.h"
#include "DataFormats/HcalDigi/interface/HODataFrame.h"
#include "DataFormats/HcalDigi/interface/ZDCDataFrame.h"
#include "DataFormats/HcalDigi/interface/HcalCalibDataFrame.h"
#include "DataFormats/HcalRecHit/interface/HBHERecHit.h"
#include "DataFormats/HcalRecHit/interface/HFRecHit.h"
#include "DataFormats/HcalRecHit/interface/HORecHit.h"
#include "DataFormats/HcalRecHit/interface/ZDCRecHit.h"
#include "DataFormats/HcalRecHit/interface/HcalCalibRecHit.h"
#include "CalibFormats/HcalObjects/interface/HcalCoder.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalPulseContainmentCorrection.h"
#include <memory>

/** \class HcalComplicatedRecAlgo

   This class reconstructs RecHits from Digis for HBHE, HF, and HO by addition
   of selected time samples, pedestal subtraction, and gain application. The
   time of the hit is reconstructed using a weighted peak bin calculation
   supplemented by precise time lookup table. A consumer of this class also
   has the option of correcting the reconstructed time for energy-dependent
   time slew associated with the QIE.
    
   $Date: 2010/01/21 14:27:43 $
   $Revision: 1.9 $
   \author J. Mans - Minnesota
*/

class HcalShape;
class CaloShapeIntegrator;

class HcalComplicatedRecAlgo {
public:
  /** Full featured constructor for HB/HE and HO (HPD-based detectors) */
  HcalComplicatedRecAlgo(int firstSample, int samplesToAdd, bool correctForTimeslew, 
		    bool correctForContainment, float fixedPhaseNs);
  ~HcalComplicatedRecAlgo();

  HBHERecHit reconstruct(const HBHEDataFrame& digi, const HcalCoder& coder, const HcalCalibrations& calibs) const;
private:
  int firstSample_, samplesToAdd_;
  bool correctForTimeslew_;
  std::auto_ptr<HcalPulseContainmentCorrection> pulseCorr_;
  HcalShape* theHcalShape;
  double theHcalShapePeakTime;
};

#endif
