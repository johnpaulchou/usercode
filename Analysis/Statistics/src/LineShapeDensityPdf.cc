#include "Analysis/Statistics/interface/LineShapeDensityPdf.h"
#include "Math/Interpolator.h"
#include "TH1F.h"

//Resonance Shape of any mass value using Interpolation technique 
//Author: Sertac Ozturk
// sertac@fnal.gov,  sertac.ozturk@cern.ch

// global variables -- shame on you, Sertac! ;)

double mass;

double y500[50]= {146, 172, 193, 193, 200, 271, 286, 331, 375, 395, 477, 543, 613, 629, 769, 853, 908, 1033, 1123, 1265, 1391, 1474, 1522, 1657, 1821, 1880, 2076, 2135, 2478, 2764, 3100, 3376, 3592, 3832, 3812, 3430, 3045, 2493, 1954, 1417, 1083, 764, 609, 439, 381, 311, 281, 251, 222, 192};


double y700[50] = {172, 195, 202, 247, 249, 334, 368, 403, 479, 522, 630, 652, 733, 827, 929, 1067, 1195, 1377, 1482, 1605, 1764, 1859, 2055, 2092, 2213, 2441, 2517, 2887, 3189, 3598, 4070, 4403, 4878, 5220, 4993, 4577, 3687, 3053, 2198, 1493, 1034, 734, 541, 444, 359, 314, 268, 238, 211, 185};

double y1200[50] = {168, 183, 233, 239, 280, 311, 334, 410, 460, 508, 581, 643, 723, 806, 947, 986, 1162, 1328, 1490, 1513, 1661, 1819, 1951, 1917, 2074, 2281, 2400, 2780, 2990, 3386, 3926, 4668, 5247, 5685, 5452, 4709, 3660, 2436, 1633, 958, 643, 470, 349, 284, 223, 242, 173, 157, 136, 97};


double y2000[50] = {147, 189, 199, 241, 248, 286, 328, 313, 390, 420, 427, 556, 638, 684, 746, 856, 965, 1095, 1233, 1296, 1437, 1489, 1543, 1677, 1654, 1802, 2033, 2172, 2487, 2857, 3265, 3973, 4542, 4996, 4770, 3921, 2751, 1601, 961, 563, 355, 239, 182, 145, 117, 88, 81, 64, 39, 35};

double y3500[50] = { 559, 546, 576, 633, 711, 693, 698, 767, 827, 813, 930, 951, 987, 1068, 1129, 1139, 1262, 1312, 1422, 1499, 1547, 1588, 1708, 1678, 1803, 1768, 1894, 2218, 2481, 2733, 3138, 3742, 4484, 4626, 4130, 2801, 1605, 735, 336, 198, 99, 81, 46, 34, 17, 14, 14, 7, 6, 4};


double bincenter[50]={
  0.31,  0.33,  0.35,  0.37,  0.39,  0.41,  0.43,  0.45,  0.47,  0.49,
  0.51,  0.53,  0.55,  0.57,  0.59,  0.61,  0.63,  0.65,  0.67,  0.69,
  0.71,  0.73,  0.75,  0.77,  0.79,  0.81,  0.83,  0.85,  0.87,  0.89,
  0.91,  0.93,  0.95,  0.97,  0.99,  1.01,  1.03,  1.05,  1.07,  1.09,
  1.11,  1.13,  1.15,  1.17,  1.19,  1.21,  1.23,  1.25,  1.27,  1.29};


std::vector<double> v;

double mqstar[5] = {500., 700., 1200., 2000., 3500.};

const int nMassBins = 103;
double massBoundaries[nMassBins+1] = {1, 3, 6, 10, 16, 23, 31, 40, 50, 61, 74, 88, 103, 119, 137, 156, 176, 197, 220, 244, 270, 296, 325,
				      354, 386, 419, 453, 489, 526, 565, 606, 649, 693, 740, 788, 838, 890, 944, 1000, 1058, 1118, 1181, 1246, 1313, 1383, 1455, 1530, 1607, 1687,
				      1770, 1856, 1945, 2037, 2132, 2231, 2332, 2438, 2546, 2659, 2775, 2895, 3019, 3147, 3279, 3416, 3558, 3704, 3854, 4010, 4171, 4337, 4509,
				      4686, 4869, 5058, 5253, 5455, 5663, 5877, 6099, 6328, 6564, 6808, 7060, 7320, 7589, 7866, 8152, 8447, 8752, 9067, 9391, 9726, 10072, 10430, 
				      10798, 11179, 11571, 11977, 12395, 12827, 13272, 13732, 14000};

double binwidth[nMassBins+1] = {2, 3, 4, 6, 7, 8, 9, 10, 11, 13, 14, 15, 16, 18, 19, 20, 21, 23, 24, 26, 26, 29, 29, 32, 33, 34, 36, 37, 39, 41, 43, 
				44, 47, 48, 50, 52, 54, 56, 58, 60, 63, 65, 67, 70, 72, 75, 77, 80, 83, 86, 89, 92, 95, 99, 101, 106, 108, 113, 116, 120, 124, 128, 132, 137, 142, 146,
				150, 156, 161, 166, 172, 177, 183, 189, 195, 202, 208, 214, 222, 229, 236, 244, 252, 260, 269, 277, 286, 295, 305, 315, 324, 335, 346, 358, 368, 381, 
				392, 406, 418, 432, 445, 460, 268};

double massnew[nMassBins+1];
double FoundQstarBinnedProb[nMassBins+1];

const unsigned int n = 50;
double x[n];
double y[n];
double f[n];


void LineShapeDensity_pdf(double mass){
  setArray(mass);
}


void setArray (double mass){ 
  for(int ind=0;ind<nMassBins; ind++){
    double bin_width = -1;		 
    double peak_low = 0.31;
    double peak_high = 1.27;
    double mjj = (massBoundaries[ind]+massBoundaries[ind+1])/2;
    //	  cout<<ind<<"\t "<<mjj<<"\t "<<peak_low*mass<<endl;
    //if (mjj<0.3*mass || mjj>1.3*mass) {
    if (mjj<peak_low*mass || mjj>peak_high*mass) {
      FoundQstarBinnedProb[ind] = 0.;
      // Create a vector and fill it with variable x bins according to entered mass value
    }
    else {
	for(int i=0;i<103; i++){
	  massnew[i] = massBoundaries[i]/mass;
	  if (massBoundaries[i]<=mjj && massBoundaries[i+1]>=mjj) 
	    bin_width = binwidth[i];
	  if (massnew[i]<=1.32 && massnew[i]>=0.28){ // x distribution is from peak_low to peak_high
	    //if (massnew[i]<=peak_high && massnew[i]>=peak_low){ // x distribution is from peak_low to peak_high
	    v.push_back(massnew[i]);
	  }
	}
	
        const unsigned int s = v.size();
	
	// Definition of arrays with variable bins 
	double bins[1000]; // variable bins for x distribution
	double m[1000]; // variable mass bins for dijet mass distribution
	double dndm[1000];
	double prob[1000];
	
	
	// Fill the bin values to array
	for(unsigned int j=0; j<v.size(); j++){
	  bins[j] = v[j];
	  m[j] = v[j] * mass;
	}
	
	// Definition of histograms
	TH1F* h_new2 = new TH1F("h_new2","with variale bins2",s-1 , m); // Histogram for dijet mass distribution with variable mass bins
	
	if(mass == mqstar[0]){ // If mass = 0.5 TeV
	  for(unsigned int k=0; k<n; k++){
	    x[k] = bincenter[k];
	    y[k] = y500[k];
	  }
	}
	
	if(mass == mqstar[1]){ // If mass = 0.7 TeV
	  for(unsigned int k=0; k<n; k++){
	    x[k] = bincenter[k];
	    y[k] = y700[k];}
	}
	
	if(mass == mqstar[2]){ // If mass = 1.2 TeV
	  for(unsigned int k=0; k<n; k++){
	    x[k] = bincenter[k];
	    y[k] = y1200[k];}
	}
	
	if(mass == mqstar[3]){ // If mass = 2 TeV
	  for(unsigned int k=0; k<n; k++){
	    x[k] = bincenter[k];
	    y[k] = y2000[k];}
	}
	
	if(mass == mqstar[4]){ // If mass = 3.5 TeV
	  for(unsigned int k=0; k<n; k++){
	    x[k] = bincenter[k];
	    y[k] = y3500[k];}
	}
	
	
	// Calculate and create new x distribution with fixed bins at entered mass value which is between 0.5 TeV and 0.7 TeV
	if((mass > mqstar[0] && mass < mqstar[1]) || mass < mqstar[0]){ 
	  for(unsigned int k=0; k<n; k++){
	    double bincont1 = y500[k];
	    double bincont2 = y700[k];
	    double Mjj1 = mqstar[0];
	    double Mjj2 = mqstar[1];
	    double Mq = mass;
	    double norm = (Mq - Mjj1)/(Mjj2 - Mjj1);
	    double eff = bincont2 - bincont1;
	    double neff = eff * norm;
	    double mqstar1 = bincont1 + neff;
	    y[k] = mqstar1;
	    x[k] = bincenter[k];}
	}
	
	// Calculate and create new x distribution with fixed bins at entered mass value which is between 0.7 TeV and 1.2 TeV. 
	if(mass > mqstar[1] && mass < mqstar[2]){
	  for(unsigned int k=0; k<n; k++){
	    double bincont1 = y700[k];
	    double bincont2 = y1200[k];
	    double Mjj1 = mqstar[1];
	    double Mjj2 = mqstar[2];
	    double Mq = mass;
	    double norm = (Mq - Mjj1)/(Mjj2 - Mjj1);
	    double eff = bincont2 - bincont1;
	    double neff = eff * norm;
	    double mqstar1 = bincont1 + neff;
	    y[k] = mqstar1;
	    x[k] = bincenter[k];}
	}
	
	// Calculate and create new x distribution with fixed bins at entered mass value which is between 1.2 TeV and 2 TeV. 
	if(mass > mqstar[2] && mass < mqstar[3]){
	  for(unsigned int k=0; k<n; k++){
	    double bincont1 = y1200[k];
	    double bincont2 = y2000[k];
	    double Mjj1 = mqstar[2];
	    double Mjj2 = mqstar[3];
	    double Mq = mass;
	    double norm = (Mq - Mjj1)/(Mjj2 - Mjj1);
	    double eff = bincont2 - bincont1;
	    double neff = eff * norm;
	    double mqstar1 = bincont1 + neff;
	    y[k] = mqstar1;
	    x[k] = bincenter[k];}
	}
	
	// Calculate and create new x distribution with fixed bins at entered mass value which is between 2 TeV and 3.5 TeV. 
	if((mass > mqstar[3] && mass < mqstar[4]) || mass > mqstar[4]){
	  for(unsigned int k=0; k<n; k++){
	    double bincont1 = y2000[k];
	    double bincont2 = y3500[k];
	    double Mjj1 = mqstar[3];
	    double Mjj2 = mqstar[4];
	    double Mq = mass;
	    double norm = (Mq - Mjj1)/(Mjj2 - Mjj1);
	    double eff = bincont2 - bincont1;
	    double neff = eff * norm;
	    double mqstar1 = bincont1 + neff;
	    y[k] = mqstar1;
	    x[k] = bincenter[k];}
	}
	
	// Interpolation tecnique is used to convert distribution in x to distribution in dijet mass with variable bins
	ROOT::Math::Interpolator inter(n, ROOT::Math::Interpolation::kCSPLINE); // If CSPLINE is not valid, please use kCSPLINE
	inter.SetData(n, x, y);
	
	// Generate the new distribution in x with variable bins
	for(unsigned int t=0; t<n; t++)
	  f[t] = inter.Eval(x[t]);
	
	//  int bin_number = h_new->GetNbinsX();
	for(unsigned int z=0; z<s; z++){
	  double bin = 0;
	  if(v[z]>peak_low && v[z]<peak_high && v[z+1]>peak_low && v[z+1]<peak_high)
	    bin = inter.Integ(v[z], v[z+1]); 
	  
	  dndm[z] = bin;
	  h_new2->SetBinContent(z+1,bin); // histogram in dijet mass distribution with variable bins
	}
	
	v.clear();
	
	double integ = h_new2->Integral();
	h_new2->Delete();
	
	for(unsigned int p=0; p<s; p++){
	  double nor = dndm[p] / integ;
	  prob[p] = nor;
	  if(m[p]<mjj && m[p+1]>=mjj){
	    FoundQstarBinnedProb[ind] = prob[p] / bin_width;	
	    ///cout << "bin_width=" << bin_width << endl;
	  }
	}
    }
    //	 cout<<ind<<"\t mjj: "<<mjj<<"\t prob: "<<FoundQstarBinnedProb[ind]<<endl;
  }	
}


// This gets the qstar binned probability faster than the original rountine
double FastQstarBinnedProb(double mjj )
{
  int bin=0;
  int binFound=0;
  for(int i=0;i<nMassBins && binFound==0; i++){
    if(massBoundaries[i+1]>mjj){
      binFound=1;
      bin=i;
    }
  }
  return FoundQstarBinnedProb[bin];
}

