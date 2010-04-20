#include "AnomalousNoise/Algorithms/interface/HcalComplicatedRecAlgo.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CalibCalorimetry/HcalAlgos/interface/HcalTimeSlew.h"
#include "SimCalorimetry/HcalSimAlgos/interface/HcalShape.h"
#include <algorithm> // for "max"
#include <math.h>


static double MaximumFractionalError = 0.0005; // 0.05% error allowed from this source

HcalComplicatedRecAlgo::HcalComplicatedRecAlgo(int firstSample, int samplesToAdd, bool correctForTimeslew, bool correctForPulse, float phaseNS) : 
  firstSample_(firstSample), 
  samplesToAdd_(samplesToAdd), 
  correctForTimeslew_(correctForTimeslew) {
  if (correctForPulse) 
    pulseCorr_=std::auto_ptr<HcalPulseContainmentCorrection>(new HcalPulseContainmentCorrection(samplesToAdd_,phaseNS,MaximumFractionalError));
  theHcalShape = new HcalShape();

  double theHcalShapePeak=0.0;
  theHcalShapePeakTime=0.0;
  for(double time=0.5; time<256; time+=1.0) {
    double ampl = (*theHcalShape)(time);
    if(ampl > theHcalShapePeak) {
      theHcalShapePeak = ampl;
      theHcalShapePeakTime = time;
    }
  }
}

HcalComplicatedRecAlgo::~HcalComplicatedRecAlgo()
{
  delete theHcalShape;
}

///Timeshift correction for HPDs based on the position of the peak ADC measurement.
///  Allows for an accurate determination of the relative phase of the pulse shape from
///  the HPD.  Calculated based on a weighted sum of the -1,0,+1 samples relative to the peak
///  as follows:  wpksamp = (0*sample[0] + 1*sample[1] + 2*sample[2]) / (sample[0] + sample[1] + sample[2])
///  where sample[1] is the maximum ADC sample value.
static float timeshift_ns_hbheho(float wpksamp);

namespace HcalComplicatedRecAlgoImpl {
  template<class Digi, class RecHit>
  inline RecHit reco(const Digi& digi, const HcalCoder& coder, const HcalCalibrations& calibs, 
		     int ifirst, int n, bool slewCorrect, const HcalPulseContainmentCorrection* corr, HcalTimeSlew::BiasSetting slewFlavor, HcalShape* theHcalShape, double theHcalShapePeakTime) {
    CaloSamples tool;
    coder.adc2fC(digi,tool);

    double ampl=0; int maxI = -1; double maxA = -1e10; float ta=0;
    double fc_ampl=0;
    for (int i=ifirst; i<tool.size() && i<n+ifirst; i++) {
      int capid=digi[i].capid();
      ta = (tool[i]-calibs.pedestal(capid)); // pedestal subtraction
      fc_ampl+=ta;
      ta*= calibs.respcorrgain(capid) ; // fC --> GeV
      ampl+=ta;
      if(ta>maxA){
	maxA=ta;
	maxI=i;
      }
    }

    float time=-9999;
    ////Cannot calculate time value with max ADC sample at first or last position in window....
    if(maxI==0 || maxI==(tool.size()-1)) {      
      LogDebug("HCAL Pulse") << "HcalComplicatedRecAlgo::reconstruct :" 
					       << " Invalid max amplitude position, " 
					       << " max Amplitude: "<< maxI
					       << " first: "<<ifirst
					       << " last: "<<(tool.size()-1)
					       << std::endl;
    } else {
      maxA=fabs(maxA);
      int capid=digi[maxI-1].capid();
      float t0 = fabs((tool[maxI-1]-calibs.pedestal(capid))*calibs.respcorrgain(capid) );
      capid=digi[maxI+1].capid();
      float t2 = fabs((tool[maxI+1]-calibs.pedestal(capid))*calibs.respcorrgain(capid) );    
      float wpksamp = (t0 + maxA + t2);
      if (wpksamp!=0) wpksamp=(maxA + 2.0*t2) / wpksamp; 
      time = (maxI - digi.presamples())*25.0 + timeshift_ns_hbheho(wpksamp);

      if (corr!=0) {
	// Apply phase-based amplitude correction:
	ampl *= corr->getCorrection(fc_ampl);
	//      std::cout << fc_ampl << " --> " << corr->getCorrection(fc_ampl) << std::endl;
      }

    
      if (slewCorrect) time-=HcalTimeSlew::delay(std::max(1.0,fc_ampl),slewFlavor);
    }

    time=time-calibs.timecorr(); // time calibration

    double data[100];
    double dataerror[100];
    for(int i=0; i<tool.size(); i++) {
      int capid=digi[i].capid();
      double fC = (tool[i]-calibs.pedestal(capid)); // pedestal subtraction
      double pes = fC/0.3305; // fC --> photo-electrons (should do this more correctly)
      double gain = calibs.respcorrgain(capid); // fC --> GeV
      double GeV = fC*gain;
      
      double peserror = sqrt(fabs(pes)+7.4); // photostatistics added in quadrature with (estimated) pedestal width; this should be updated JPC & JMSJ
      double GeVerror = peserror*0.3305*gain;
      
      data[i]=GeV;
      dataerror[i]=GeVerror;
    }

    // calculate the integrated HCAL shape scaled to the amplitude and shifted to match peak times
    double theory[100];
    double tfull = time+calibs.timecorr()+25.0*digi.presamples();
    double tp=theHcalShapePeakTime;
    double step=0.5;
    for (int i=0; i<tool.size(); i++) {
      theory[i]=0.0;
      if(tfull-tp>(i+1)*25.0) continue;
      for(int nsoffset=0; nsoffset<25; nsoffset++) {
	double nstotal=25.0*i+static_cast<double>(nsoffset);
	if(tfull-tp>nstotal) continue;
	theory[i]+=(*theHcalShape)(step)*ampl;
	step+=1.0;
      }
    }

    

    return RecHit(digi.id(),ampl,time);    
  }
}

HBHERecHit HcalComplicatedRecAlgo::reconstruct(const HBHEDataFrame& digi, const HcalCoder& coder, const HcalCalibrations& calibs) const {
  return HcalComplicatedRecAlgoImpl::reco<HBHEDataFrame,HBHERecHit>(digi,coder,calibs,
								    firstSample_,samplesToAdd_,correctForTimeslew_,
								    pulseCorr_.get(),
								    HcalTimeSlew::Medium, theHcalShape, theHcalShapePeakTime);
}

// timeshift implementation

static const float wpksamp0_hbheho = 0.680178;
static const float scale_hbheho    = 0.819786;
static const int   num_bins_hbheho = 50;

static const float actual_ns_hbheho[num_bins_hbheho] = {
 0.00000, // 0.000-0.020
 0.41750, // 0.020-0.040
 0.81500, // 0.040-0.060
 1.21000, // 0.060-0.080
 1.59500, // 0.080-0.100
 1.97250, // 0.100-0.120
 2.34750, // 0.120-0.140
 2.71250, // 0.140-0.160
 3.07500, // 0.160-0.180
 3.43500, // 0.180-0.200
 3.79000, // 0.200-0.220
 4.14250, // 0.220-0.240
 4.49250, // 0.240-0.260
 4.84250, // 0.260-0.280
 5.19000, // 0.280-0.300
 5.53750, // 0.300-0.320
 5.89000, // 0.320-0.340
 6.23750, // 0.340-0.360
 6.59250, // 0.360-0.380
 6.95250, // 0.380-0.400
 7.31000, // 0.400-0.420
 7.68000, // 0.420-0.440
 8.05500, // 0.440-0.460
 8.43000, // 0.460-0.480
 8.83250, // 0.480-0.500
 9.23250, // 0.500-0.520
 9.65500, // 0.520-0.540
10.09500, // 0.540-0.560
10.54750, // 0.560-0.580
11.04500, // 0.580-0.600
11.55750, // 0.600-0.620
12.13000, // 0.620-0.640
12.74500, // 0.640-0.660
13.41250, // 0.660-0.680
14.18500, // 0.680-0.700
15.02750, // 0.700-0.720
15.92250, // 0.720-0.740
16.82500, // 0.740-0.760
17.70000, // 0.760-0.780
18.52500, // 0.780-0.800
19.28750, // 0.800-0.820
19.99750, // 0.820-0.840
20.67250, // 0.840-0.860
21.31250, // 0.860-0.880
21.90750, // 0.880-0.900
22.48750, // 0.900-0.920
23.02750, // 0.920-0.940
23.55250, // 0.940-0.960
24.05000, // 0.960-0.980
24.53750, // 0.980-1.000
};

float timeshift_ns_hbheho(float wpksamp) {
  int index=(int)(0.5+num_bins_hbheho*(wpksamp-wpksamp0_hbheho)/scale_hbheho);
  
  if      (index <    0)             return actual_ns_hbheho[0];
  else if (index >= num_bins_hbheho) return actual_ns_hbheho[num_bins_hbheho-1];
  
  return actual_ns_hbheho[index];
}

//double function(double amplitude, double timeoffset, double pedestal) {
//  
//
//}
