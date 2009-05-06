import FWCore.ParameterSet.Config as cms

path            = cms.untracked.string('/QCDDiJetPt1800to2200/Summer08_IDEAL_V9_v1/GEN-SIM-RECO')
location        = cms.untracked.string('cmsdca2.fnal.gov')
numFiles        = cms.untracked.int32(14)
crossSection    = cms.untracked.double(1.381e-2)
crossSectionErr = cms.untracked.double(0.0)
numEvents       = cms.untracked.int32(22848)
minPthat        = cms.untracked.double(1800.)
maxPthat        = cms.untracked.double(2200.)

readFiles       = cms.untracked.vstring(
    '/store/mc/Summer08/QCDDiJetPt1800to2200/GEN-SIM-RECO/IDEAL_V9_v1/0000/1041A489-E3A4-DD11-87B4-0015C5E5B288.root',
    '/store/mc/Summer08/QCDDiJetPt1800to2200/GEN-SIM-RECO/IDEAL_V9_v1/0000/1854951A-F6A4-DD11-A1E2-001EC9ED5AF6.root',
    '/store/mc/Summer08/QCDDiJetPt1800to2200/GEN-SIM-RECO/IDEAL_V9_v1/0000/1E39E335-E8A4-DD11-BD9A-001D0967CFB3.root',
    '/store/mc/Summer08/QCDDiJetPt1800to2200/GEN-SIM-RECO/IDEAL_V9_v1/0000/20C16D18-ECA4-DD11-B050-001D0967D648.root',
    '/store/mc/Summer08/QCDDiJetPt1800to2200/GEN-SIM-RECO/IDEAL_V9_v1/0000/24D44119-05A5-DD11-92B7-0019B9F3FF70.root',
    '/store/mc/Summer08/QCDDiJetPt1800to2200/GEN-SIM-RECO/IDEAL_V9_v1/0000/309BEAF2-E6A4-DD11-A301-0015C5EC47A2.root',
    '/store/mc/Summer08/QCDDiJetPt1800to2200/GEN-SIM-RECO/IDEAL_V9_v1/0000/32E714B9-E4A4-DD11-A1DC-001EC9ED840F.root',
    '/store/mc/Summer08/QCDDiJetPt1800to2200/GEN-SIM-RECO/IDEAL_V9_v1/0000/54DF7AB3-F1A4-DD11-8648-001EC9ED8413.root',
    '/store/mc/Summer08/QCDDiJetPt1800to2200/GEN-SIM-RECO/IDEAL_V9_v1/0000/6CD46D8A-E3A4-DD11-81D8-0015C5E5B288.root',
    '/store/mc/Summer08/QCDDiJetPt1800to2200/GEN-SIM-RECO/IDEAL_V9_v1/0000/6E6728A6-FAA4-DD11-8FB8-001125756AF6.root',
    '/store/mc/Summer08/QCDDiJetPt1800to2200/GEN-SIM-RECO/IDEAL_V9_v1/0000/BEF6D438-EBA4-DD11-A9E3-001EC9ED88D8.root',
    '/store/mc/Summer08/QCDDiJetPt1800to2200/GEN-SIM-RECO/IDEAL_V9_v1/0000/C81E99FC-F4A4-DD11-BEE7-00145EDD7ACD.root',
    '/store/mc/Summer08/QCDDiJetPt1800to2200/GEN-SIM-RECO/IDEAL_V9_v1/0000/CA235FE8-F5A4-DD11-AB20-00145EDD7ACD.root',
    '/store/mc/Summer08/QCDDiJetPt1800to2200/GEN-SIM-RECO/IDEAL_V9_v1/0000/CCF9CD60-EBA4-DD11-8A29-001D0967DBE8.root'
    )
