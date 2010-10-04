#ifndef __ANALYSIS_MAKETREES_DATASETS_H__
#define __ANALYSIS_MAKETREES_DATASETS_H__

#include <string>
#include <vector>
#include <set>

// basic data structure

class dataset {
  friend class datasets;

 public:
  dataset();
  virtual ~dataset() {}

  inline std::string getName(void) const { return name; }
  inline const std::vector<std::string>& getFiles(void) const { return files; }
  inline bool getIsMC(void) const { return isMC; }
  inline bool getIsRealData(void) const { return !isMC; }
  inline int getDatasetId(void) const { return datasetId; }
  inline double getLuminosity(void) const { return luminosity; }
  inline double getLuminosityError(void) const { return luminosityError; }
  inline int getNumEvents(void) const { return numEvents; }
  inline double getXSection(void) const { return xSection; }
  inline double getXSectionError(void) const { return xSectionError; }

  bool operator<(const dataset& d) const;
  bool operator>(const dataset& d) const;
  bool operator==(const dataset& d) const;

 public:
  std::string name;
  std::vector<std::string> files;
  bool isMC;
  int datasetId;
  double luminosity;
  double luminosityError;
  int numEvents;
  double xSection;
  double xSectionError;

};


// container class for multiple datasets
class datasetContainer {
 public:
  datasetContainer() {}
  ~datasetContainer() {}

  const dataset& findDataset(std::string datasetname);
  const dataset& findDatasetByFile(std::string filename);

  double getTotalLuminosity(void) const;
  double getTotalLuminosityError(void) const;

  void getFiles(std::vector<std::string>& files, int nsplits=1, int split=0) const;

  void insert(const dataset& d) { datasets.insert(d); return; }
  void clear(void) { datasets.clear(); return; }
  std::set<dataset>::const_iterator begin(void) const { return datasets.begin(); }
  std::set<dataset>::const_iterator end(void) const { return datasets.end(); }
  std::set<dataset>::iterator begin(void) { return datasets.begin(); }
  std::set<dataset>::iterator end(void) { return datasets.end(); }

 private:
  std::set<dataset> datasets;

};


// Singleton pattern

class datasets {
 public:

  static datasets& getDatasets() { return instance_; }

  void getDatasetContainer(std::vector<std::string> datasetNames, datasetContainer& cont) const;
  bool findDataset(std::string datasetName, dataset& d) const;
  const dataset& findDataset(std::string datasetName) const;

 private:
  // self-reference
  static datasets instance_;

  // constructors/destructors
  datasets();
  ~datasets() {}
  datasets(const datasets &);             // intentionally undefined
  datasets & operator=(const datasets &); // intentionally undefined

  // helper functions
  void getFilelistFromFile(std::string filename, std::vector<std::string>& filelist) const;
  void specialInsert(dataset& d);

  // actual container
  std::set<dataset> theDatasets_;
};


#endif
