import FWCore.ParameterSet.Config as cms

path            = cms.untracked.string('/QCDDiJetPt3000to3500/Summer08_IDEAL_V9_v1/GEN-SIM-RECO')
location        = cms.untracked.string('cmsdca2.fnal.gov')
numFiles        = cms.untracked.int32(14)
crossSection    = cms.untracked.double(8.432e-6)
crossSectionErr = cms.untracked.double(0.0)
numEvents       = cms.untracked.int32(20880)
minPthat        = cms.untracked.double(3000.)
maxPthat        = cms.untracked.double(3500.)

readFiles       = cms.untracked.vstring(
    '/store/mc/Summer08/QCDDiJetPt3000to3500/GEN-SIM-RECO/IDEAL_V9_v1/0000/062F9704-ECA4-DD11-9D1D-0015C5E673CC.root',
    '/store/mc/Summer08/QCDDiJetPt3000to3500/GEN-SIM-RECO/IDEAL_V9_v1/0000/20E9FF0A-F9A4-DD11-A588-00145EDD79F1.root',
    '/store/mc/Summer08/QCDDiJetPt3000to3500/GEN-SIM-RECO/IDEAL_V9_v1/0000/3EF6B697-EEA4-DD11-97E6-0015C5E5B335.root',
    '/store/mc/Summer08/QCDDiJetPt3000to3500/GEN-SIM-RECO/IDEAL_V9_v1/0000/4045934E-FBA4-DD11-B840-001A645C0C8E.root',
    '/store/mc/Summer08/QCDDiJetPt3000to3500/GEN-SIM-RECO/IDEAL_V9_v1/0000/5CB57778-E3A4-DD11-A46B-003048C26CB6.root',
    '/store/mc/Summer08/QCDDiJetPt3000to3500/GEN-SIM-RECO/IDEAL_V9_v1/0000/6AF05143-F0A4-DD11-824D-0015C5E5B335.root',
    '/store/mc/Summer08/QCDDiJetPt3000to3500/GEN-SIM-RECO/IDEAL_V9_v1/0000/74FFEA69-7AA6-DD11-80D0-00145EDD7399.root',
    '/store/mc/Summer08/QCDDiJetPt3000to3500/GEN-SIM-RECO/IDEAL_V9_v1/0000/78DECA04-ECA4-DD11-815A-0015C5E5B22E.root',
    '/store/mc/Summer08/QCDDiJetPt3000to3500/GEN-SIM-RECO/IDEAL_V9_v1/0000/804D7CE4-FCA4-DD11-8946-001D09645AEC.root',
    '/store/mc/Summer08/QCDDiJetPt3000to3500/GEN-SIM-RECO/IDEAL_V9_v1/0000/8829DEE9-FCA4-DD11-8347-003048C26CB6.root',
    '/store/mc/Summer08/QCDDiJetPt3000to3500/GEN-SIM-RECO/IDEAL_V9_v1/0000/A02518D4-F1A4-DD11-918A-003048C26CB6.root',
    '/store/mc/Summer08/QCDDiJetPt3000to3500/GEN-SIM-RECO/IDEAL_V9_v1/0000/BE7CFC67-FAA4-DD11-A6F8-003048C26CB6.root',
    '/store/mc/Summer08/QCDDiJetPt3000to3500/GEN-SIM-RECO/IDEAL_V9_v1/0000/C24A7B67-FAA4-DD11-8303-001EC9ED5AF6.root',
    '/store/mc/Summer08/QCDDiJetPt3000to3500/GEN-SIM-RECO/IDEAL_V9_v1/0000/C6F7C8C7-F0A4-DD11-AB6B-003048C26CB6.root'
    )
