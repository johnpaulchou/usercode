#ifndef __DIJET_HELPER_H__
#define __DIJET_HELPER_H__

#include <string>

class RooDataHist;
class RooDataSet;
class RooAbsPdf;
class RooRealVar;
class RooAbsData;
class RooFitResult;
class TH1D;

namespace DijetHelper {

  RooDataHist* makeBinnedData(std::string filename, std::string objectname, RooRealVar* var, int nbins, double *binsx);
  RooDataHist* makeBinnedData(std::string filename, std::string objectname, RooRealVar* var, int nbins, double xlo, double xhi);
  RooDataHist* makeBinnedData(std::string filename, std::string objectname, RooRealVar* var, TH1D* hist);

  RooDataHist* makeBinnedPseudoData(RooAbsPdf* pdf, int nevents, std::string objectname, RooRealVar* var, int nbins, double *binsx);
  RooDataHist* makeBinnedPseudoData(RooAbsPdf* pdf, int nevents, std::string objectname, RooRealVar* var, int nbins, double xlo, double xhi);
  RooDataHist* makeBinnedPseudoData(RooAbsPdf* pdf, int nevents, std::string objectname, RooRealVar* var, TH1D* hist);

  RooDataSet* makeUnbinnedData(std::string filename, std::string objectname, RooRealVar* var);

  double calcPDF1DIntegral(RooAbsPdf* pdf, RooRealVar* var, double min, double max);

  RooFitResult* doFit(std::string name, RooAbsPdf* pdf, RooAbsData* data, RooRealVar* var, double count, int nbins, double *binsx);
  RooFitResult* doFit(std::string name, RooAbsPdf* pdf, RooAbsData* data, RooRealVar* var, double count, int nbins, double xlo, double xhi);

  double getParFinalValue(RooFitResult* fit, std::string parname);

  void printVal(const RooRealVar& var);
  double calcPoissonError(double mu);

  double getMinMaxDeviations(RooAbsPdf* pdf, RooAbsPdf* pdfb, RooAbsPdf* pdfc, RooRealVar* var, double normalization, int nbins, double *binsx);

}


#endif
