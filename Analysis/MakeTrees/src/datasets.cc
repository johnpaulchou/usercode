#include "Analysis/MakeTrees/interface/datasets.h"

#include <iostream>
#include <fstream>
#include <cassert>

////////////////////////////////////////
//
// static member initialization
//
////////////////////////////////////////

datasets datasets::instance_;

////////////////////////////////////////
//
// dataset member definitions
//
////////////////////////////////////////


dataset::dataset()
{
  isMC=true;
  datasetId=-1;
  luminosity=luminosityError=0.0;
  numEvents=-1;
  xSection=xSectionError=0.0;

}

bool dataset::operator<(const dataset& d) const
{
  return name.compare(d.name)<0;
}

bool dataset::operator>(const dataset& d) const
{
  return name.compare(d.name)>0;
}

bool dataset::operator==(const dataset& d) const
{
  return name.compare(d.name)==0;
}

////////////////////////////////////////
//
// datasetContainer member functions
//
////////////////////////////////////////


const dataset& datasetContainer::findDataset(std::string datasetname)
{
  dataset dummy;
  dummy.name=datasetname;
  std::set<dataset>::const_iterator it=datasets.find(dummy);
  assert(it!=datasets.end());
  return (*it);
}

const dataset& datasetContainer::findDatasetByFile(std::string filename)
{
  for(std::set<dataset>::const_iterator it=datasets.begin(); it!=datasets.end(); ++it) {
    std::vector<std::string> files=it->getFiles();
    for(unsigned int i=0; i<files.size(); i++)
      if(files[i].compare(filename)==0) return (*it);
  }
  assert(0);
  return (*datasets.begin());
}

double datasetContainer::getTotalLuminosity(void) const
{
  double l=0.0;
  for(std::set<dataset>::const_iterator it=datasets.begin(); it!=datasets.end(); ++it) {
    l += it->getLuminosity();
  }
  return l;
}

double datasetContainer::getTotalLuminosityError(void) const
{
  double l=0.0;
  for(std::set<dataset>::const_iterator it=datasets.begin(); it!=datasets.end(); ++it) {
    l += it->getLuminosityError();
  }
  return l;
}

void datasetContainer::getFiles(std::vector<std::string>& files, int nsplits, int split) const
{
  files.clear();
  std::vector<std::string> tempfiles;
  for(std::set<dataset>::const_iterator it=datasets.begin(); it!=datasets.end(); ++it) {
    std::vector<std::string> moreFiles=it->getFiles();
    tempfiles.insert(tempfiles.begin(), moreFiles.begin(), moreFiles.end());
  }

  // now manipulate the files list
  int nfiles=static_cast<int>(tempfiles.size());
  assert(nfiles>=nsplits && split<nsplits);
  int first=(split*nfiles)/nsplits;
  int last = ((split+1)*nfiles)/nsplits;
  files.insert(files.begin(), &tempfiles[first], &tempfiles[last]);

  return;
}


////////////////////////////////////////
//
// datasets member functions
//
////////////////////////////////////////

bool datasets::findDataset(std::string name, dataset& d) const
{
  dataset dummy;
  dummy.name=name;
  std::set<dataset>::iterator it = theDatasets_.find(dummy);
  if(it==theDatasets_.end()) return false;
  else {
    d = (*it);
    return true;
  }
}

const dataset& datasets::findDataset(std::string name) const
{
  dataset dummy;
  dummy.name=name;
  std::set<dataset>::const_iterator it= theDatasets_.find(dummy);
  assert(it!=theDatasets_.end());
  return (*it);
}

void datasets::getDatasetContainer(std::vector<std::string> datasetNames, datasetContainer& cont) const
{
  cont.clear();
  for(unsigned int i=0; i<datasetNames.size(); i++)
    cont.insert(findDataset(datasetNames[i]));
  return;
}



void datasets::getFilelistFromFile(std::string filename, std::vector<std::string>& filelist) const
{
  filelist.clear();
  const int maxlen=1000;
  std::ifstream ifs;
  ifs.open(filename.c_str(), std::ifstream::in);
  int cnt=0;
  char s[maxlen];
  while(ifs) {
    ifs.getline(s, maxlen);
    if(ifs) {
      filelist.push_back(s);
      ++cnt;
    }
  }
  ifs.close();
  
  if(cnt<=0) {
    std::cout << "Warning: no files were found in " << filename << std::endl;
    assert(cnt>0);
  }

  return;
}

void datasets::specialInsert(dataset& d)
{
  // make sure there is no duplicate entries
  assert(theDatasets_.find(d)==theDatasets_.end());

  // specify the datasetID before insertion
  d.datasetId=theDatasets_.size();

  // insert
  theDatasets_.insert(d);
  return;
}


////////////////////////////////////////
//
// datasets definition
//
/////////////////////////////////////////

datasets::datasets()
{
  dataset d;

  d.name="JetMETSep17thReRecoDijetSelection384p2";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/JetMETSep17thReRecoDijetSelection384p2.list", d.files);
  d.isMC=false;
  d.luminosity=2.889134263;
  d.luminosityError=d.luminosity*0.11;
  d.numEvents=0;
  d.xSection=0.0;
  d.xSectionError=0.0;
  specialInsert(d);

  d.name="JetMETTauSep17thReRecoDijetSelection384p2";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/JetMETTauSep17thReRecoDijetSelection384p2.list", d.files);
  d.isMC=false;
  d.luminosity=0.172082870;
  d.luminosityError=d.luminosity*0.11;
  d.numEvents=0;
  d.xSection=0.0;
  d.xSectionError=0.0;
  specialInsert(d);

  d.name="JetRun2010BPromptRecoV2DijetSelection384p2";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/JetRun2010BPromptRecoV2DijetSelection384p2.list", d.files);
  d.isMC=false;
  d.luminosity=3.567454396;
  d.luminosityError=d.luminosity*0.11;
  d.numEvents=0;
  d.xSection=0.0;
  d.xSectionError=0.0;
  specialInsert(d);

  d.name="JetMETTauNov4thReRecoDijetSelection385p3";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/JetMETTauNov4thReRecoDijetSelection385p3.list", d.files);
  d.isMC=false;
  d.luminosity=0.167488688;
  d.luminosityError=d.luminosity*0.11;
  d.numEvents=0;
  d.xSection=0.0;
  d.xSectionError=0.0;
  specialInsert(d);

  d.name="JetMETNov4thReRecoDijetSelection385p3";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/JetMETNov4thReRecoDijetSelection385p3.list", d.files);
  d.isMC=false;
  d.luminosity=2.895797321;
  d.luminosityError=d.luminosity*0.11;
  d.numEvents=0;
  d.xSection=0.0;
  d.xSectionError=0.0;
  specialInsert(d);

  d.name="JetNov4thReRecoDijetSelection385p3";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/JetNov4thReRecoDijetSelection385p3.list", d.files);
  d.isMC=false;
  d.luminosity=30.687634908;
  d.luminosityError=d.luminosity*0.11;
  d.numEvents=0;
  d.xSection=0.0;
  d.xSectionError=0.0;
  specialInsert(d);

  d.name="QCDPt30to50Pythia8Fall10DijetSelection384p2";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt30to50Pythia8Fall10DijetSelection384p2.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=1100000;
  d.xSection=50329230.0;
  d.xSectionError=0;
  specialInsert(d);

  d.name="QCDPt50to80Pythia8Fall10DijetSelection384p2";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt50to80Pythia8Fall10DijetSelection384p2.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=1091306;
  d.xSection=6026504.0;
  d.xSectionError=0;
  specialInsert(d);

  d.name="QCDPt80to120Pythia8Fall10DijetSelection384p2";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt80to120Pythia8Fall10DijetSelection384p2.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=1094036;
  d.xSection=751043.9;
  d.xSectionError=0;
  specialInsert(d);

  d.name="QCDPt120to170Pythia8Fall10DijetSelection384p2";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt120to170Pythia8Fall10DijetSelection384p2.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=880000;
  d.xSection=111930.2;
  d.xSectionError=0;
  specialInsert(d);

  d.name="QCDPt170to300Pythia8Fall10DijetSelection384p2";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt170to300Pythia8Fall10DijetSelection384p2.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=880000;
  d.xSection=24085.04;
  d.xSectionError=0;
  specialInsert(d);

  d.name="QCDPt300to470Pythia8Fall10DijetSelection384p2";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt300to470Pythia8Fall10DijetSelection384p2.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=547370;
  d.xSection=1201.385;
  d.xSectionError=0;
  specialInsert(d);

  d.name="QCDPt470to600Pythia8Fall10DijetSelection384p2";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt470to600Pythia8Fall10DijetSelection384p2.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=544917;
  d.xSection=74.15958;
  d.xSectionError=0;
  specialInsert(d);

  d.name="QCDPt600to800Pythia8Fall10DijetSelection384p2";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt600to800Pythia8Fall10DijetSelection384p2.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=543140;
  d.xSection=16.71798;
  d.xSectionError=0;
  specialInsert(d);

  d.name="QCDPt800to1000Pythia8Fall10DijetSelection384p2";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt800to1000Pythia8Fall10DijetSelection384p2.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=436652;
  d.xSection=2.002449;
  d.xSectionError=0;
  specialInsert(d);


  d.name="QCDPt1000to1400Pythia8Fall10DijetSelection384p2";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt1000to1400Pythia8Fall10DijetSelection384p2.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=387112;
  d.xSection=0.3625033;
  d.xSectionError=0;
  specialInsert(d);

  d.name="QCDPt1400to1800Pythia8Fall10DijetSelection384p2";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt1400to1800Pythia8Fall10DijetSelection384p2.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=210496;
  d.xSection=0.01179865;
  d.xSectionError=0;
  specialInsert(d);

  d.name="QCDPt1800Pythia8Fall10DijetSelection384p2";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt1800Pythia8Fall10DijetSelection384p2.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=218065;
  d.xSection=0.0003802342;
  d.xSectionError=0;
  specialInsert(d);

  d.name="QCDPt80to120Pythia6Fall10DijetSelection385p3";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt80to120Pythia6Fall10DijetSelection385p3.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=3208299;
  d.xSection=784265.2;
  d.xSectionError=0;
  specialInsert(d);

  d.name="QCDPt120to170Pythia6Fall10DijetSelection385p3";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt120to170Pythia6Fall10DijetSelection385p3.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=3045200;
  d.xSection=115133.5;
  d.xSectionError=0;
  specialInsert(d);

  d.name="QCDPt170to300Pythia6Fall10DijetSelection385p3";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt170to300Pythia6Fall10DijetSelection385p3.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=3220080;
  d.xSection=24262.83;
  d.xSectionError=0;
  specialInsert(d);

  d.name="QCDPt300to470Pythia6Fall10DijetSelection385p3";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt300to470Pythia6Fall10DijetSelection385p3.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=3171240;
  d.xSection=1168.494;
  d.xSectionError=0;
  specialInsert(d);

  d.name="QCDPt470to600Pythia6Fall10DijetSelection385p3";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt470to600Pythia6Fall10DijetSelection385p3.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=2019732;
  d.xSection=70.22421;
  d.xSectionError=0;
  specialInsert(d);

  d.name="QCDPt600to800Pythia6Fall10DijetSelection385p3";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt600to800Pythia6Fall10DijetSelection385p3.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=1979055;
  d.xSection=15.55374;
  d.xSectionError=0;
  specialInsert(d);

  d.name="QCDPt800to1000Pythia6Fall10DijetSelection385p3";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt800to1000Pythia6Fall10DijetSelection385p3.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=2084404;
  d.xSection=1.843688;
  d.xSectionError=0;
  specialInsert(d);

  d.name="QCDPt1000to1400Pythia6Fall10DijetSelection385p3";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt1000to1400Pythia6Fall10DijetSelection385p3.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=150000;
  d.xSection=0.3321052;
  d.xSectionError=0;
  specialInsert(d);

  d.name="QCDPt1400to1800Pythia6Fall10DijetSelection385p3";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt1400to1800Pythia6Fall10DijetSelection385p3.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=100000;
  d.xSection=0.01087214;
  d.xSectionError=0;
  specialInsert(d);

  d.name="QCDPt1800Pythia6Fall10DijetSelection385p3";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/QCDPt1800Pythia6Fall10DijetSelection385p3.list", d.files);
  d.isMC=true;
  d.luminosity=0;
  d.luminosityError=0;
  d.numEvents=100000;
  d.xSection=0.000357463;
  d.xSectionError=0;
  specialInsert(d);

}

