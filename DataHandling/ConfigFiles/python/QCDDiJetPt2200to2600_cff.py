import FWCore.ParameterSet.Config as cms

path            = cms.untracked.string('/QCDDiJetPt2200to2600/Summer08_IDEAL_V9_v1/GEN-SIM-RECO')
location        = cms.untracked.string('cmsdca2.fnal.gov')
numFiles        = cms.untracked.int32(15)
crossSection    = cms.untracked.double(1.29608e-3)
crossSectionErr = cms.untracked.double(0.0)
numEvents       = cms.untracked.int32(22560)
minPthat        = cms.untracked.double(2200.)
maxPthat        = cms.untracked.double(2600.)

readFiles       = cms.untracked.vstring(
        '/store/mc/Summer08/QCDDiJetPt2200to2600/GEN-SIM-RECO/IDEAL_V9_v1/0000/069C44C7-FAA4-DD11-8143-001125756BAC.root',
        '/store/mc/Summer08/QCDDiJetPt2200to2600/GEN-SIM-RECO/IDEAL_V9_v1/0000/2664EDA8-E7A4-DD11-8977-001EC9ED8413.root',
        '/store/mc/Summer08/QCDDiJetPt2200to2600/GEN-SIM-RECO/IDEAL_V9_v1/0000/2ABD1F25-F5A4-DD11-BAB1-00145EDD795D.root',
        '/store/mc/Summer08/QCDDiJetPt2200to2600/GEN-SIM-RECO/IDEAL_V9_v1/0000/3E39EDCD-E4A4-DD11-99D9-00188B7431B9.root',
        '/store/mc/Summer08/QCDDiJetPt2200to2600/GEN-SIM-RECO/IDEAL_V9_v1/0000/5020FC85-E3A4-DD11-A823-001EC9ED840F.root',
        '/store/mc/Summer08/QCDDiJetPt2200to2600/GEN-SIM-RECO/IDEAL_V9_v1/0000/5059D1FB-EBA4-DD11-8C26-001EC9ED4FB2.root',
        '/store/mc/Summer08/QCDDiJetPt2200to2600/GEN-SIM-RECO/IDEAL_V9_v1/0000/5A4AAEF6-F0A4-DD11-A391-00145EDD73ED.root',
        '/store/mc/Summer08/QCDDiJetPt2200to2600/GEN-SIM-RECO/IDEAL_V9_v1/0000/9EFDA7F6-F0A4-DD11-847F-00145EDD7411.root',
        '/store/mc/Summer08/QCDDiJetPt2200to2600/GEN-SIM-RECO/IDEAL_V9_v1/0000/AE9B6569-E0A4-DD11-90AD-00093D1456B0.root',
        '/store/mc/Summer08/QCDDiJetPt2200to2600/GEN-SIM-RECO/IDEAL_V9_v1/0000/B2C58FF6-F0A4-DD11-BA65-00145EDD75C5.root',
        '/store/mc/Summer08/QCDDiJetPt2200to2600/GEN-SIM-RECO/IDEAL_V9_v1/0000/C6B235FF-EBA4-DD11-8CE5-001EC9ED8F2B.root',
        '/store/mc/Summer08/QCDDiJetPt2200to2600/GEN-SIM-RECO/IDEAL_V9_v1/0000/E04D7E45-FDA4-DD11-83DC-001EC9ED4A02.root',
        '/store/mc/Summer08/QCDDiJetPt2200to2600/GEN-SIM-RECO/IDEAL_V9_v1/0000/E0D67D92-F5A4-DD11-9F27-0015C5E5B335.root',
        '/store/mc/Summer08/QCDDiJetPt2200to2600/GEN-SIM-RECO/IDEAL_V9_v1/0000/F2EB6635-E8A4-DD11-9238-001D0967DA3A.root',
        '/store/mc/Summer08/QCDDiJetPt2200to2600/GEN-SIM-RECO/IDEAL_V9_v1/0000/F6FE400E-F1A4-DD11-A567-00145EDD79F5.root'
)
