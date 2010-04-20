#ifndef __ANOMALOUSNOISE_RBX_ORGANIZER_H__
#define __ANOMALOUSNOISE_RBX_ORGANIZER_H__

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/EcalObjects/interface/EcalChannelStatus.h"
#include "Geometry/CaloTopology/interface/CaloTowerConstituentsMap.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include <vector>
#include <set>
#include <map>

// forward declarations
class HBHERecHit;
class EcalRecHit;
class HcalChannelQuality;
class HcalSeverityLevelComputer;
class EcalSeverityLevelAlgo;

//////////////////////////////////////////////////////////////////////////////
//
// HitValidator
//
//////////////////////////////////////////////////////////////////////////////

// determines if a hit is valid or not
class HitValidator
{
 public:
  explicit HitValidator(const edm::ParameterSet&);
  virtual ~HitValidator();
  
  inline void setHcalChannelQuality(const HcalChannelQuality* q) { theHcalChStatus_=q; }
  inline void setEcalChannelStatus(const EcalChannelStatus* q) { theEcalChStatus_=q; }
  inline void setHcalSeverityLevelComputer(const HcalSeverityLevelComputer* q) { theHcalSevLvlComputer_=q; }
  inline void setEcalSeverityLevelAlgo(const EcalSeverityLevelAlgo* q) { theEcalSevLvlAlgo_=q; }
  inline void setEBRecHitCollection(const EcalRecHitCollection* q) { theEBRecHitCollection_=q; }
  inline void setEERecHitCollection(const EcalRecHitCollection* q) { theEERecHitCollection_=q; }


  bool validHit(const HBHERecHit&) const;
  bool validHit(const EcalRecHit&) const;

 private:

  double HBThreshold_;  // energy threshold for HB hits
  double HESThreshold_; // energy threshold for 5-degree (phi) HE hits
  double HEDThreshold_; // energy threshold for 10-degree (phi) HE hits
  double EBThreshold_;  // energy threshold for EB hits
  double EEThreshold_;  // energy threshold for EE hits

  uint32_t HcalAcceptSeverityLevel_; // severity level to accept HCAL hits
  uint32_t EcalAcceptSeverityLevel_; // severity level to accept ECAL hits
  bool UseHcalRecoveredHits_; // whether or not to use recovered HCAL hits
  bool UseEcalRecoveredHits_; // whether or not to use recovered HCAL hits
  
  // for checking the status of ECAL and HCAL channels stored in the DB 
  const HcalChannelQuality* theHcalChStatus_;
  const EcalChannelStatus* theEcalChStatus_;
  
  // calculator of severety level for ECAL and HCAL
  const HcalSeverityLevelComputer* theHcalSevLvlComputer_;
  const EcalSeverityLevelAlgo* theEcalSevLvlAlgo_;

  // needed to determine an ECAL channel's validity
  const EcalRecHitCollection* theEBRecHitCollection_;
  const EcalRecHitCollection* theEERecHitCollection_;

};


//////////////////////////////////////////////////////////////////////////////
//
// HcalHitOrganizer
//
//////////////////////////////////////////////////////////////////////////////

// organizes HCAL hits so that you can quickly identify their neighbors in eta-phi space
class HcalHitOrganizer
{
 public:
  HcalHitOrganizer(const HBHERecHitCollection& hcalcollection, const HitValidator& hitvalidator);
  virtual ~HcalHitOrganizer();

  // gets a vector of the neighboring hits
  void neighbors(const HBHERecHit* hit, std::vector<const HBHERecHit*>& neighbors);

  // same as above, but also requires that the neighbors are in a different RBX
  void neighborsDifferentRBX(const HBHERecHit* hit, std::vector<const HBHERecHit*>& neighbors);

  // same as above, but also requires that the neighbors are in a different HPD
  void neighborsDifferentHPD(const HBHERecHit* hit, std::vector<const HBHERecHit*>& neighbors);

  // same as above, but also requires that the neighbors are in a same HPD
  void neighborsSameHPD(const HBHERecHit* hit, std::vector<const HBHERecHit*>& neighbors);

  // finds a hit by eta/phi/depth coordinate
  // returns 0 if no hit with that coordinate can be found
  const HBHERecHit* findHit(int ieta, int iphi, int depth);

 private:

  struct HBHERecHitCompare {
    inline bool operator() (const HBHERecHit* h1, const HBHERecHit* h2) {
      if(h1->id().ieta()<h2->id().ieta()) return true;
      else if(h1->id().ieta()>h2->id().ieta()) return false;
      else if(h1->id().iphi()<h2->id().iphi()) return true;
      else if(h1->id().iphi()>h2->id().iphi()) return false;
      else if(h1->id().depth()<h2->id().depth()) return true;
      else if(h1->id().depth()>h2->id().depth()) return false;
      else return false;
    }
  };


  typedef std::set<const HBHERecHit*, HBHERecHitCompare> hitset_t;
  hitset_t hits_;

  // helper function which implements the neighbors algorithm
  // if relative==0, get all neighbors in a different RBX
  // if relative==1, get all neighbors in a different HPD
  // if relative==2, get all neighbors
  // if relative==3, get all neighbors in the same HPD
  void neighbors_(const HBHERecHit*, std::vector<const HBHERecHit*>&, int relative);
  void addHit_(const HBHERecHit*, const HBHERecHit*, std::vector<const HBHERecHit*>&, int relative);

};

//////////////////////////////////////////////////////////////////////////////
//
// EcalHitOrganizer
//
//////////////////////////////////////////////////////////////////////////////

// organizes ECAL hits so that you can quickly identify their neighbors in eta-phi space
class EcalHitOrganizer
{
 public:
  EcalHitOrganizer(const EcalRecHitCollection& ebcollection,
		   const EcalRecHitCollection& eecollection,
		   const HitValidator& hitvalidator,
		   const CaloTowerConstituentsMap& ctcm);
  virtual ~EcalHitOrganizer();

  // finds ecal hits by eta/phi coordinate
  void findHits(int ieta, int iphi, std::vector<const EcalRecHit*>& v) const;

 private:
  
  class etaphi_t : public std::pair<int, int> {
  public:
    etaphi_t(int ieta, int iphi) { first=ieta; second=iphi; }
    ~etaphi_t() {}
    inline int ieta(void) { return first; }
    inline int iphi(void) { return second; }
    inline void ieta(int i) { first=i; }
    inline void iphi(int i) { second=i; }
  };
  typedef std::multimap<etaphi_t, const EcalRecHit*> hitmap_t;
  hitmap_t hits_;
};


//////////////////////////////////////////////////////////////////////////////
//
// HitCollectionInfo
//
//////////////////////////////////////////////////////////////////////////////

// a collection of hits, and their hcal neighbors, ecal neighbors, and tracks
class HitCollectionInfo {

  // allow RBXOrganizer to fill the private members
  friend class RBXOrganizer;

public:
  HitCollectionInfo();
  virtual ~HitCollectionInfo() {}

  // total energy in the hits themselves, their neighbors, and the ecal in front of them
  double hitEnergy(double threshold=0.0) const;
  double neighborEnergy(double threshold=0.0) const;
  double ecalEnergy(double threshold=0.0) const;

  // total number of hits found
  int nHits(double threshold=0.0) const;
  int nNeighbors(double threshold=0.0) const;
  int nEcalHits(double threshold=0.0) const;

  // used to loop over the hcal hits
  inline std::set<const HBHERecHit*>::const_iterator hcalHitsBegin() const { return hcalhits_.begin(); }
  inline std::set<const HBHERecHit*>::const_iterator hcalHitsEnd() const { return hcalhits_.end(); }

  // used to loop over the neighbors
  inline std::set<const HBHERecHit*>::const_iterator hcalNeighborsBegin() const { return hcalneighbors_.begin(); }
  inline std::set<const HBHERecHit*>::const_iterator hcalNeighborsEnd() const { return hcalneighbors_.end(); }

  // used to loop over the ecal hits
  inline std::set<const EcalRecHit*>::const_iterator ecalNeighborsBegin() const { return ecalneighbors_.begin(); }
  inline std::set<const EcalRecHit*>::const_iterator ecalNeighborsEnd() const { return ecalneighbors_.end(); }

 private:

  std::set<const HBHERecHit*> hcalhits_;
  std::set<const HBHERecHit*> hcalneighbors_;
  std::set<const EcalRecHit*> ecalneighbors_;

  mutable double hitenergy_;
  mutable double neighborenergy_;
  mutable double ecalenergy_;
  mutable int nhits_;
  mutable int nneighbors_;
  mutable int necalhits_;

};

//////////////////////////////////////////////////////////////////////////////
//
// RBXOrganizer
//
//////////////////////////////////////////////////////////////////////////////

// organizers hits according to RBXs, HPDs, dihits, and monohits
class RBXOrganizer
{
public:
  RBXOrganizer(const HBHERecHitCollection& hbhehitcollection,
	       const EcalRecHitCollection& ebhitcollection,
	       const EcalRecHitCollection& eehitcollection,
	       const HitValidator& validator,
	       const CaloTowerConstituentsMap& ctcm);

  virtual ~RBXOrganizer();  

  void getRBXs(std::vector<HitCollectionInfo>& rbxs, double rbxenergy) const;
  void getHPDs(std::vector<HitCollectionInfo>& hpds, double hpdenergy) const;
  void getDiHits(std::vector<HitCollectionInfo>& dihits, double dihitenergy) const;
  void getMonoHits(std::vector<HitCollectionInfo>& monohits, double monohitenergy) const;

  void dump(void) const;

 private:
  std::map<int, HitCollectionInfo> rbxs_;
  std::map<int, HitCollectionInfo> hpds_;

  std::vector<HitCollectionInfo> dihits_;
  std::vector<HitCollectionInfo> monohits_;

  HcalHitOrganizer hcalhitorganizer_;
  EcalHitOrganizer ecalhitorganizer_;


};



#endif
