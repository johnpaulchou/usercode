#include "Analysis/MakeTrees/interface/datasets.h"

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
  char s[maxlen];
  while(ifs) {
    ifs.getline(s, maxlen);
    if(ifs) filelist.push_back(s);
  }
  ifs.close();
  
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
  d.name="JetMETPromptRecoV4DijetSelection370p4";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/JetMETPromptRecoV4DijetSelection370p4.list", d.files);
  d.isMC=false;
  d.luminosity=2.649980731;
  d.luminosityError=d.luminosity*0.11;
  d.numEvents=0;
  d.xSection=0.0;
  d.xSectionError=0.0;
  specialInsert(d);


  d.name="JetMETTauPromptRecoV4DijetSelection370p4";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/JetMETTauPromptRecoV4DijetSelection370p4.list", d.files);
  d.isMC=false;
  d.luminosity=0.100376564;
  d.luminosityError=d.luminosity*0.11;
  d.numEvents=0;
  d.xSection=0.0;
  d.xSectionError=0.0;
  specialInsert(d);

  d.name="JetMETTauJuly15thReRecoDijetSelection370p4";
  getFilelistFromFile("/uscms/home/johnpaul/datasets/JetMETTauJuly15thReRecoDijetSelection370p4.list", d.files);
  d.isMC=false;
  d.luminosity=0.066199297;
  d.luminosityError=d.luminosity*0.11;
  d.numEvents=0;
  d.xSection=0.0;
  d.xSectionError=0.0;
  specialInsert(d);

}

