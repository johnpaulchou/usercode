import FWCore.ParameterSet.Config as cms

process = cms.Process("RECO")

# define the dataset here
dataset = 1 # 1=R68288, 2=R109049, 3=QCDpt3000

# Conditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
if dataset == 1:
    process.GlobalTag.globaltag = "CRAFT0831X_V1::All"
elif dataset == 2:
    process.GlobalTag.globaltag = "CRAFT0831X_V1::All"
elif dataset == 3:
    process.GlobalTag.globaltag = "MC_31X_V3::All"

process.prefer("GlobalTag")

# Magnetic field
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

#Geometry
process.load("Configuration.StandardSequences.Geometry_cff")

#raw-to-digi
if dataset <= 2:
    process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
    process.RawToDigi = cms.Sequence(process.gctDigis+process.gtDigis+process.gtEvmDigis+process.siPixelDigis+process.siStripDigis+process.ecalDigis+process.ecalPreshowerDigis+process.hcalDigis)
elif dataset ==3:
    process.load('Configuration.StandardSequences.RawToDigi_cff')

#reco for cosmics
process.load('EventFilter.L1GlobalTriggerRawToDigi.l1GtRecord_cfi')
process.load('RecoLocalCalo.Configuration.RecoLocalCalo_cff')
process.load('RecoLocalTracker.Configuration.RecoLocalTracker_Cosmics_cff')
process.load('RecoTracker.Configuration.RecoTrackerP5_cff')
process.load('RecoVertex.BeamSpotProducer.BeamSpot_cff')
process.load('RecoTracker.Configuration.RecoTrackerBHM_cff')
process.load('RecoVertex.Configuration.RecoVertexCosmicTracks_cff')
process.load('RecoJets.Configuration.RecoCaloTowersGR_cff')
process.load('RecoJets.Configuration.RecoJetsGR_cff')
process.load('RecoMET.METProducers.HTMET_cfi')
process.load('RecoMET.METProducers.CaloMET_cfi')
process.load('RecoMET.METProducers.hcalnoiseinfoproducer_cfi')
process.metreco = cms.Sequence(process.metNoHF)
process.trackerCosmics = cms.Sequence(process.offlineBeamSpot*process.trackerlocalreco*process.tracksP5)
process.reconstructionCosmics = cms.Sequence(process.l1GtRecord*process.trackerCosmics*process.calolocalreco*process.vertexrecoCosmics*process.recoCaloTowersGR*process.recoJetsGR*process.hcalnoise)

# set special threshold for ECAL energy, since Run 68288 has a hot tower
if dataset<=2:
    process.towerMaker.EBThreshold = cms.double(9999999.)
    process.towerMaker.EEThreshold = cms.double(9999999.)

# set the pulse shape and hit multiplicity parameters for the Hcal rechit reconstruction (these aren't correct <= 3.2.X)
process.hbhereco.flagParameters = cms.PSet(nominalPedestal=cms.double(3.0), #fC
                                           hitEnergyMinimum=cms.double(1.0), #GeV
                                           hitMultiplicityThreshold=cms.int32(17),
                                           pulseShapeParameterSets = cms.VPSet(
    cms.PSet(pulseShapeParameters=cms.vdouble(   0.0, 100.0, -50.0, 0.0, -15.0, 0.15)),
    cms.PSet(pulseShapeParameters=cms.vdouble( 100.0, 2.0e3, -50.0, 0.0,  -5.0, 0.05)),
    cms.PSet(pulseShapeParameters=cms.vdouble( 2.0e3, 1.0e6, -50.0, 0.0,  95.0,  0.0)),
    cms.PSet(pulseShapeParameters=cms.vdouble(-1.0e6, 1.0e6,  45.0, 0.1, 1.0e6,  0.0))
    ))

process.hcalnoise.dropRefVectors = cms.bool(False)
process.hcalnoise.minRBXEnergy = cms.double(10)
process.hcalnoise.minHPDEnergy = cms.double(10)
process.hcalnoise.minRecHitEnergy = cms.double(1.5)
process.hcalnoise.fillTracks = cms.bool(False)
process.hcalnoise.fillJets = cms.bool(False)

# rate analyzer
process.load('AnomalousNoise.Analyzers.noiserates_cfi')
if dataset==1:
    process.noiserates.findTrigger = cms.bool(True)
    process.noiserates.trigName = cms.string('L1_SingleJet10')
    process.noiserates.rootHistFilename = cms.string('R68288.root')
elif dataset==2:
    process.noiserates.findTrigger = cms.bool(False)
    process.noiserates.rootHistFilename = cms.string('R109049.root')
elif dataset==3:
    process.noiserates.findTrigger = cms.bool(False)
    process.noiserates.rootHistFilename = cms.string('QCDpt3000.root')

# run over files
readFiles = cms.untracked.vstring()
if dataset==1:
    readFiles.append('/store/data/Commissioning08/Calo/RAW/v1/000/068/288/FE92951A-3CA8-DD11-8981-001D09F23A4D.root')
#    readFiles.append('/store/data/Commissioning08/Calo/RAW/v1/000/068/288/FCEA0CB3-3FA8-DD11-82E6-001D09F28D4A.root')
#    readFiles.append('/store/data/Commissioning08/Calo/RAW/v1/000/068/288/24CF332E-37A8-DD11-89F8-000423D98750.root')
#    readFiles.append('/store/data/Commissioning08/Calo/RAW/v1/000/068/288/2600FE7E-57A8-DD11-9650-001D09F290BF.root')
#    readFiles.append('/store/data/Commissioning08/Calo/RAW/v1/000/068/288/260CBA03-2EA8-DD11-8A8A-000423D98B28.root')
#    readFiles.append('/store/data/Commissioning08/Calo/RAW/v1/000/068/288/26755C42-3EA8-DD11-9649-00304879FBB2.root')
elif dataset==2:
    readFiles.append('/store/data/CRAFT09/Calo/RAW/v1/000/109/049/002E7792-257C-DE11-8719-001D09F2426D.root')
elif dataset==3:
    readFiles.append('/store/mc/Summer09/QCD_Pt3000/GEN-SIM-RAW/MC_31X_V3-v1/0007/003C3B4C-6380-DE11-B9A3-00E0813008AA.root')
    readFiles.append('/store/mc/Summer09/QCD_Pt3000/GEN-SIM-RAW/MC_31X_V3-v1/0007/0417F486-6380-DE11-B697-00188B7AC621.root')
    readFiles.append('/store/mc/Summer09/QCD_Pt3000/GEN-SIM-RAW/MC_31X_V3-v1/0007/06244359-6080-DE11-B527-00E081300892.root')
    readFiles.append('/store/mc/Summer09/QCD_Pt3000/GEN-SIM-RAW/MC_31X_V3-v1/0007/12D2596A-6580-DE11-8F94-00E0813000C2.root')

process.source = cms.Source ("PoolSource",fileNames = readFiles )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

# Tone down the logging messages, MessageLogger!
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 50
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# output
process.output = cms.OutputModule(
    "PoolOutputModule",
    outputCommands = cms.untracked.vstring('drop *',
#                                           'keep HBHEDataFramesSorted_*_*_*',
                                           'keep HBHERecHitsSorted_*_*_*',
                                           'keep CaloTowersSorted_*_*_*',
                                           'keep recoHcalNoiseRBXs_*_*_*',
                                           'keep HcalNoiseSummary_*_*_*',
                                           'keep recoCaloMETs_*_*_*', 
                                           'keep recoCaloJets_*_*_*',
                                           'keep L1GlobalTriggerRecord_*_*_*'
                                           ),
    fileName = cms.untracked.string('out.root')
    )


# schedule
process.digi = cms.Path(process.RawToDigi)
process.reco = cms.Path(process.reconstructionCosmics+process.metreco)
process.ana = cms.Path(process.noiserates)
process.e = cms.EndPath(process.output)

#process.schedule = cms.Schedule(process.digi,process.reco,process.ana,process.e)
process.schedule = cms.Schedule(process.digi,process.reco,process.ana)
