import FWCore.ParameterSet.Config as cms

# define the dataset here
dataset = 1 # 1=QCD no mix, 2 = QCD mix

process = cms.Process("ANALYSIS")

# Conditions
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = "MC_31X_V3::All"

process.load('RecoMET.METProducers.hcalnoiseinfoproducer_cfi')
process.hcalnoise.hcalNoiseRBXCollName = cms.string('hcalnoise')
process.hcalnoise.fillTracks = cms.bool(True)
process.hcalnoise.refillRefVectors = cms.bool(True)
process.hcalnoise.requirePedestals = cms.bool(False)

# analyzer
process.load('AnomalousNoise.Analyzers.dataefficiency_cfi')
if dataset == 1:
    process.dataefficiency.rootHistFilename = cms.string('dataeff_qcd_nomix.root')
elif dataset == 2:
    process.dataefficiency.rootHistFilename = cms.string('dataeff_qcd_mix.root')

# run over files
readFiles = cms.untracked.vstring()
process.source = cms.Source ("PoolSource",fileNames = readFiles )
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

if dataset == 1:
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/02654C50-828F-DE11-94BA-00145E55254F.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/04FA05C5-938B-DE11-9124-00163E1124E7.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/0A2CF794-828F-DE11-B934-00D0680BF986.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/16001CEA-908B-DE11-85EA-00163E11247F.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/20195B11-8A8B-DE11-BB9A-00167683191A.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/2855F244-778F-DE11-B90E-001D096750C6.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/2CA61F3D-928B-DE11-A703-00215E2212D2.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/489C7D6A-828F-DE11-AFF9-00D0680B8849.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/4C4BA9A3-A08B-DE11-8F71-00151724FF81.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/549AE432-7B8F-DE11-875A-00D0680BF8C8.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/58FA249F-748F-DE11-8128-00145E551463.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/64FFC6C0-808B-DE11-9191-00215E21DC72.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/70B3ECAB-838F-DE11-87EF-00145E55226A.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/72628873-838F-DE11-AB22-00D0680B8852.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/72AF2307-768F-DE11-BD4E-001D096750E9.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/72ED2086-758F-DE11-90FE-001D09675166.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/7406DD23-838F-DE11-B8D4-00D0680B8876.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/7A923EB3-828F-DE11-A1B4-00145E5523ED.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/7C793029-738F-DE11-B623-001D096750E9.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/86A36B87-828F-DE11-99C1-00D0680BF8C8.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/9ADB1BB6-A18B-DE11-B80C-001517255E7A.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/9C338626-818B-DE11-A1E4-001E0BC1EED6.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/A02804D7-7C8B-DE11-8CF7-00163E1124E8.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/B481E7A8-838F-DE11-B9EE-00D0680BF8C4.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/B6577D5E-838F-DE11-8743-00145E5522C7.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/C296629C-838F-DE11-9BBF-00145E5518E5.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/E26B0420-828F-DE11-959A-0022191F51AC.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/E851C86A-828F-DE11-B5F4-00145E552321.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/EEE52BB3-828F-DE11-990A-00D0680BF8C4.root')
    readFiles.append('/store/mc/Summer09/QCDDiJet_Pt0to15/GEN-SIM-RECO/MC_31X_V3-v1/0019/F43FAD80-838F-DE11-B548-0022191F5E95.root')
elif dataset == 2:
    readFiles.append('file:/uscmst1b_scratch/lpc1/lpcphys/wclarida/DataMixer/CMSSW_3_2_4/CRAFT_plus_QCD_DM_RECO_capIdSync_pt0t15.root')
    
# Tone down the logging messages, MessageLogger!
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 50
process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )

# schedule
process.p = cms.Path(process.hcalnoise+process.dataefficiency)

process.schedule = cms.Schedule(process.p)
