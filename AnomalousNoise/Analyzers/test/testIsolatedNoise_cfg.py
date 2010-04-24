import FWCore.ParameterSet.Config as cms

# define the dataset here
dataset = 2 # 1=CRAFT, 2=MinBiasData, 3=MinBiasMC. 4=ttbarMC

process = cms.Process("ANALYSIS")

process.load('Configuration/StandardSequences/Services_cff')
process.load('FWCore/MessageService/MessageLogger_cfi')
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('RecoLocalCalo.CaloTowersCreator.calotowermaker_cfi')

process.load('RecoJets.Configuration.CaloTowersES_cfi')
process.load('RecoLocalCalo.HcalRecAlgos.hcalRecAlgoESProd_cfi')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.Reconstruction_cff')

if dataset==1:
    process.GlobalTag.globaltag = 'CRAFT09_R_V10::All'
elif dataset==2:
    process.GlobalTag.globaltag = 'GR_R_35X_V7::All'
elif dataset==3:
    process.GlobalTag.globaltag = 'MC_3XY_V26::All'
elif dataset==4:
    process.GlobalTag.globaltag = 'MC_3XY_V26::All'

process.isolatednoise = cms.EDAnalyzer(
    'IsolatedNoise',

    # this way we use the same settings as calotowers
    hbheInput = process.calotowermaker.hbheInput,
    ecalInputs = process.calotowermaker.ecalInputs,
    trackInput = cms.InputTag('generalTracks'),
    HBThreshold = process.calotowermaker.HBThreshold,
    HESThreshold = process.calotowermaker.HESThreshold,
    HEDThreshold = process.calotowermaker.HEDThreshold,
    EBThreshold = process.calotowermaker.EBThreshold,
    EEThreshold = process.calotowermaker.EEThreshold,
    HcalAcceptSeverityLevel = process.calotowermaker.HcalAcceptSeverityLevel,
    EcalAcceptSeverityLevel = process.calotowermaker.EcalAcceptSeverityLevel,
    UseHcalRecoveredHits = process.calotowermaker.UseHcalRecoveredHits,
    UseEcalRecoveredHits = process.calotowermaker.UseEcalRecoveredHits,
    
    TrackAssociatorParameters = cms.PSet(
    muonMaxDistanceSigmaX = cms.double(0.0),
    muonMaxDistanceSigmaY = cms.double(0.0),
    CSCSegmentCollectionLabel = cms.InputTag("cscSegments"),
    dRHcal = cms.double(9999.0),
    dREcal = cms.double(9999.0),
    CaloTowerCollectionLabel = cms.InputTag("towerMaker"),
    useEcal = cms.bool(True),
    dREcalPreselection = cms.double(0.05),
    HORecHitCollectionLabel = cms.InputTag("horeco"),
    dRMuon = cms.double(9999.0),
    trajectoryUncertaintyTolerance = cms.double(-1.0),
    crossedEnergyType = cms.string('SinglePointAlongTrajectory'),
    muonMaxDistanceX = cms.double(5.0),
    muonMaxDistanceY = cms.double(5.0),
    useHO = cms.bool(False),
    accountForTrajectoryChangeCalo = cms.bool(True),
    DTRecSegment4DCollectionLabel = cms.InputTag("dt4DSegments"),
    EERecHitCollectionLabel = cms.InputTag("ecalRecHit",
                                           "EcalRecHitsEE"),
    dRHcalPreselection = cms.double(0.2),
    useMuon = cms.bool(False),
    useCalo = cms.bool(True),
    EBRecHitCollectionLabel = cms.InputTag("ecalRecHit",
                                           "EcalRecHitsEB"),
    dRMuonPreselection = cms.double(0.2),
    usePreshower = cms.bool(False),
    dRPreshowerPreselection = cms.double(0.2),
    truthMatch = cms.bool(False),
    HBHERecHitCollectionLabel = cms.InputTag("hbhereco"),
    useHcal = cms.bool(True)
    ),

    rootHistFileName = cms.string('out.root'),
    
)

if dataset==1:
    process.isolatednoise.rootHistFileName = cms.string('craft.root')
elif dataset==2:
    process.isolatednoise.rootHistFileName = cms.string('mbdata.root')
elif dataset==3:
    process.isolatednoise.rootHistFileName = cms.string('mbmc.root')
elif dataset==4:
    process.isolatednoise.rootHistFileName = cms.string('ttbarmc.root')

# Tone down the logging messages, MessageLogger!
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.options = cms.untracked.PSet(
    Rethrow = cms.untracked.vstring('ProductNotFound'),
    wantSummary = cms.untracked.bool(True)
    )

# run over files
readFiles = cms.untracked.vstring()
if dataset==1:
    readFiles.append('/store/data/CRAFT09/Calo/RECO/v1/000/112/237/08AEF410-D093-DE11-93B4-001D09F28F25.root')
elif dataset==2:
    readFiles.append('/store/data/Commissioning10/MinimumBias/RECO/Apr20ReReco-v1/0160/00064D6A-8D4C-DF11-B7C4-002618943829.root')
    readFiles.append('/store/data/Commissioning10/MinimumBias/RECO/Apr20ReReco-v1/0160/0065C473-8F4C-DF11-A129-003048679080.root')
    readFiles.append('/store/data/Commissioning10/MinimumBias/RECO/Apr20ReReco-v1/0160/007CE907-924C-DF11-9AE8-003048679070.root')
    readFiles.append('/store/data/Commissioning10/MinimumBias/RECO/Apr20ReReco-v1/0160/009407B5-904C-DF11-AC3B-003048678FD6.root')
    readFiles.append('/store/data/Commissioning10/MinimumBias/RECO/Apr20ReReco-v1/0160/02316438-944C-DF11-AA66-003048679084.root')
    readFiles.append('/store/data/Commissioning10/MinimumBias/RECO/Apr20ReReco-v1/0160/0271A66B-8E4C-DF11-B834-002354EF3BE3.root')
    readFiles.append('/store/data/Commissioning10/MinimumBias/RECO/Apr20ReReco-v1/0160/027378F8-914C-DF11-A396-003048678ED4.root')
    readFiles.append('/store/data/Commissioning10/MinimumBias/RECO/Apr20ReReco-v1/0160/02984D49-974C-DF11-8AF4-002618943962.root')
    readFiles.append('/store/data/Commissioning10/MinimumBias/RECO/Apr20ReReco-v1/0160/02AED3F3-914C-DF11-9EB1-002618943866.root')
    readFiles.append('/store/data/Commissioning10/MinimumBias/RECO/Apr20ReReco-v1/0160/02BB8573-8F4C-DF11-9A99-00304867C1BC.root')
elif dataset==3:
    readFiles.append('/store/mc/Spring10/MinBias/RECO/START3X_V26_S09_preproduction-v1/0129/002DA025-523E-DF11-A30B-002618943874.root')
    readFiles.append('/store/mc/Spring10/MinBias/RECO/START3X_V26_S09_preproduction-v1/0129/00F2D5C8-4F3E-DF11-88F8-00261894388B.root')
    readFiles.append('/store/mc/Spring10/MinBias/RECO/START3X_V26_S09_preproduction-v1/0129/02A4912E-523E-DF11-9768-0026189437F2.root')
    readFiles.append('/store/mc/Spring10/MinBias/RECO/START3X_V26_S09_preproduction-v1/0129/045B9171-513E-DF11-AA5B-0026189437FE.root')
    readFiles.append('/store/mc/Spring10/MinBias/RECO/START3X_V26_S09_preproduction-v1/0129/04C22804-523E-DF11-971E-003048678D86.root')
    readFiles.append('/store/mc/Spring10/MinBias/RECO/START3X_V26_S09_preproduction-v1/0129/04C7576A-533E-DF11-B767-003048D15E14.root')
    readFiles.append('/store/mc/Spring10/MinBias/RECO/START3X_V26_S09_preproduction-v1/0129/08229480-513E-DF11-9AD0-00261894394D.root')
    readFiles.append('/store/mc/Spring10/MinBias/RECO/START3X_V26_S09_preproduction-v1/0129/08326122-553E-DF11-A81E-003048678E94.root')
    readFiles.append('/store/mc/Spring10/MinBias/RECO/START3X_V26_S09_preproduction-v1/0129/0842FA77-513E-DF11-955F-00304867BFF2.root')
    readFiles.append('/store/mc/Spring10/MinBias/RECO/START3X_V26_S09_preproduction-v1/0129/089516F6-4F3E-DF11-AC14-002618943902.root')
    readFiles.append('/store/mc/Spring10/MinBias/RECO/START3X_V26_S09_preproduction-v1/0129/0A05BF10-523E-DF11-98F9-002618943975.root')
    readFiles.append('/store/mc/Spring10/MinBias/RECO/START3X_V26_S09_preproduction-v1/0129/0A4489B2-513E-DF11-A4BC-00248C0BE018.root')
elif dataset==4:
    readFiles.append('/store/mc/Spring10/TTbar-mcatnlo/GEN-SIM-RECO/START3X_V26_S09-v1/0004/0002A744-9F46-DF11-99CC-002618943974.root')
    readFiles.append('/store/mc/Spring10/TTbar-mcatnlo/GEN-SIM-RECO/START3X_V26_S09-v1/0004/0049BFE0-9E46-DF11-B46B-002618943884.root')
    readFiles.append('/store/mc/Spring10/TTbar-mcatnlo/GEN-SIM-RECO/START3X_V26_S09-v1/0004/0493254F-9A46-DF11-B830-002618943944.root')
    readFiles.append('/store/mc/Spring10/TTbar-mcatnlo/GEN-SIM-RECO/START3X_V26_S09-v1/0004/04BA3612-9F46-DF11-A88F-00261894392B.root')
    readFiles.append('/store/mc/Spring10/TTbar-mcatnlo/GEN-SIM-RECO/START3X_V26_S09-v1/0004/08443B43-9F46-DF11-8A12-0026189438D9.root')
    readFiles.append('/store/mc/Spring10/TTbar-mcatnlo/GEN-SIM-RECO/START3X_V26_S09-v1/0004/0893244C-9A46-DF11-BC3F-002618943862.root')
    readFiles.append('/store/mc/Spring10/TTbar-mcatnlo/GEN-SIM-RECO/START3X_V26_S09-v1/0004/08AC6953-A346-DF11-AD22-0026189438C4.root')
    readFiles.append('/store/mc/Spring10/TTbar-mcatnlo/GEN-SIM-RECO/START3X_V26_S09-v1/0004/0A9AF64D-9A46-DF11-9271-00261894392C.root')

process.source = cms.Source ("PoolSource",fileNames = readFiles )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(5000) )

process.p = cms.Path(process.isolatednoise)
