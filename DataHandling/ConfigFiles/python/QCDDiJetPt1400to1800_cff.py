import FWCore.ParameterSet.Config as cms

path            = cms.untracked.string('/QCDDiJetPt1400to1800/Summer08_IDEAL_V9_v1/GEN-SIM-RECO')
location        = cms.untracked.string('cmsdca2.fnal.gov')
numFiles        = cms.untracked.int32(17)
crossSection    = cms.untracked.double(1.569e-1)
crossSectionErr = cms.untracked.double(0.0)
numEvents       = cms.untracked.int32(27744)
minPthat        = cms.untracked.double(1400.)
maxPthat        = cms.untracked.double(1800.)

readFiles       = cms.untracked.vstring(
    '/store/mc/Summer08/QCDDiJetPt1400to1800/GEN-SIM-RECO/IDEAL_V9_v1/0000/06BBF68F-E3A4-DD11-BC62-001EC9ED4F9E.root',
    '/store/mc/Summer08/QCDDiJetPt1400to1800/GEN-SIM-RECO/IDEAL_V9_v1/0000/0AB45A18-FAA4-DD11-B090-0019B9E7BADF.root',
    '/store/mc/Summer08/QCDDiJetPt1400to1800/GEN-SIM-RECO/IDEAL_V9_v1/0000/1C15E197-F9A4-DD11-A955-00145EFBDD5C.root',
    '/store/mc/Summer08/QCDDiJetPt1400to1800/GEN-SIM-RECO/IDEAL_V9_v1/0000/30D2C874-EBA4-DD11-8FFD-001EC9ED88D8.root',
    '/store/mc/Summer08/QCDDiJetPt1400to1800/GEN-SIM-RECO/IDEAL_V9_v1/0000/3876D174-EBA4-DD11-AFC6-001EC9ED4F92.root',
    '/store/mc/Summer08/QCDDiJetPt1400to1800/GEN-SIM-RECO/IDEAL_V9_v1/0000/4CE75B40-F2A4-DD11-AAEE-001D0967DA49.root',
    '/store/mc/Summer08/QCDDiJetPt1400to1800/GEN-SIM-RECO/IDEAL_V9_v1/0000/60919256-F6A4-DD11-94FB-00145EFC54D8.root',
    '/store/mc/Summer08/QCDDiJetPt1400to1800/GEN-SIM-RECO/IDEAL_V9_v1/0000/62ECFE74-EBA4-DD11-8501-001EC9ED4FC2.root',
    '/store/mc/Summer08/QCDDiJetPt1400to1800/GEN-SIM-RECO/IDEAL_V9_v1/0000/74CBCF2E-F7A4-DD11-A76C-0030487C11A0.root',
    '/store/mc/Summer08/QCDDiJetPt1400to1800/GEN-SIM-RECO/IDEAL_V9_v1/0000/8E789DF7-F3A4-DD11-94D0-00145EFBD314.root',
    '/store/mc/Summer08/QCDDiJetPt1400to1800/GEN-SIM-RECO/IDEAL_V9_v1/0000/94EB6859-F8A4-DD11-A3BF-00145EDD7881.root',
    '/store/mc/Summer08/QCDDiJetPt1400to1800/GEN-SIM-RECO/IDEAL_V9_v1/0000/BCF8A520-FBA4-DD11-81CD-0019B9F40547.root',
    '/store/mc/Summer08/QCDDiJetPt1400to1800/GEN-SIM-RECO/IDEAL_V9_v1/0000/CA8F1790-E3A4-DD11-9E01-001EC9ED840F.root',
    '/store/mc/Summer08/QCDDiJetPt1400to1800/GEN-SIM-RECO/IDEAL_V9_v1/0000/D45C5A55-F0A4-DD11-8518-0015C5EC47A2.root',
    '/store/mc/Summer08/QCDDiJetPt1400to1800/GEN-SIM-RECO/IDEAL_V9_v1/0000/D8F32C74-EBA4-DD11-A684-001EC9ED5ADA.root',
    '/store/mc/Summer08/QCDDiJetPt1400to1800/GEN-SIM-RECO/IDEAL_V9_v1/0000/F8228539-F7A4-DD11-A2FB-0030487C1174.root',
    '/store/mc/Summer08/QCDDiJetPt1400to1800/GEN-SIM-RECO/IDEAL_V9_v1/0000/FC5390F7-F3A4-DD11-A122-00145EFBD314.root'
    )