import FWCore.ParameterSet.Config as cms

path            = cms.untracked.string('/QCDDiJetPt2600to3000/Summer08_IDEAL_V9_v1/GEN-SIM-RECO')
location        = cms.untracked.string('cmsdca2.fnal.gov')
numFiles        = cms.untracked.int32(15)
crossSection    = cms.untracked.double(1.140e-4)
crossSectionErr = cms.untracked.double(0.0)
numEvents       = cms.untracked.int32(22800)
minPthat        = cms.untracked.double(2600.)
maxPthat        = cms.untracked.double(3000.)

readFiles       = cms.untracked.vstring(
    '/store/mc/Summer08/QCDDiJetPt2600to3000/GEN-SIM-RECO/IDEAL_V9_v1/0000/029E54E5-F1A4-DD11-AD31-00145EDD781B.root',
    '/store/mc/Summer08/QCDDiJetPt2600to3000/GEN-SIM-RECO/IDEAL_V9_v1/0000/06676C24-F6A4-DD11-8E7C-001EC9ED8413.root',
    '/store/mc/Summer08/QCDDiJetPt2600to3000/GEN-SIM-RECO/IDEAL_V9_v1/0000/22C33E3E-77A6-DD11-B236-0015C5E5B335.root',
    '/store/mc/Summer08/QCDDiJetPt2600to3000/GEN-SIM-RECO/IDEAL_V9_v1/0000/4E7FF588-F3A4-DD11-9D60-00145EDD778D.root',
    '/store/mc/Summer08/QCDDiJetPt2600to3000/GEN-SIM-RECO/IDEAL_V9_v1/0000/56E81892-F1A4-DD11-B19D-00145EDD72F1.root',
    '/store/mc/Summer08/QCDDiJetPt2600to3000/GEN-SIM-RECO/IDEAL_V9_v1/0000/60C50B3B-F6A4-DD11-BACF-00145EDD7511.root',
    '/store/mc/Summer08/QCDDiJetPt2600to3000/GEN-SIM-RECO/IDEAL_V9_v1/0000/6C89E816-F0A4-DD11-82B1-00145EFC59CC.root',
    '/store/mc/Summer08/QCDDiJetPt2600to3000/GEN-SIM-RECO/IDEAL_V9_v1/0000/764C4C7B-ECA4-DD11-A4F5-0019B9E4FE29.root',
    '/store/mc/Summer08/QCDDiJetPt2600to3000/GEN-SIM-RECO/IDEAL_V9_v1/0000/8649A394-F1A4-DD11-9623-001EC9E14A75.root',
    '/store/mc/Summer08/QCDDiJetPt2600to3000/GEN-SIM-RECO/IDEAL_V9_v1/0000/944882F1-F8A4-DD11-B964-001EC9ED4FB2.root',
    '/store/mc/Summer08/QCDDiJetPt2600to3000/GEN-SIM-RECO/IDEAL_V9_v1/0000/9CC1E7F1-F8A4-DD11-BA39-001EC9ED4FC2.root',
    '/store/mc/Summer08/QCDDiJetPt2600to3000/GEN-SIM-RECO/IDEAL_V9_v1/0000/A85C1DF2-F8A4-DD11-B09E-001EC9ED948B.root',
    '/store/mc/Summer08/QCDDiJetPt2600to3000/GEN-SIM-RECO/IDEAL_V9_v1/0000/B88CEF15-F0A4-DD11-A2BA-00145EDD76E9.root',
    '/store/mc/Summer08/QCDDiJetPt2600to3000/GEN-SIM-RECO/IDEAL_V9_v1/0000/BE7918C8-F9A4-DD11-8D49-00145E6D496A.root',
    '/store/mc/Summer08/QCDDiJetPt2600to3000/GEN-SIM-RECO/IDEAL_V9_v1/0000/C08A3C9F-7BA6-DD11-BCD1-001D096908D3.root'
)
