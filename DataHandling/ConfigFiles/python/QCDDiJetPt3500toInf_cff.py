import FWCore.ParameterSet.Config as cms

path            = cms.untracked.string('/QCDDiJetPt3500toInf/Summer08_IDEAL_V9_v1/GEN-SIM-RECO')
location        = cms.untracked.string('cmsdca2.fnal.gov')
numFiles        = cms.untracked.int32(22)
crossSection    = cms.untracked.double(1.815e-7)
crossSectionErr = cms.untracked.double(0.0)
numEvents       = cms.untracked.int32(34320)
minPthat        = cms.untracked.double(3500.)
maxPthat        = cms.untracked.double(999999999.)

readFiles       = cms.untracked.vstring(
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/1AB84C77-7AA6-DD11-841F-00145EDD7567.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/26103E7A-9FA6-DD11-A733-00093D14572B.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/2EB60FF8-7EA6-DD11-8BE5-001D09690A04.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/3E9CF471-7AA6-DD11-95DC-00145EDD787D.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/46143700-7FA6-DD11-93CF-001D0967DEFE.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/4890E672-7AA6-DD11-9FE6-00145EDD7311.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/4A320FD7-7BA6-DD11-B041-001D0967D1BB.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/4E684E25-8CA6-DD11-AD07-001D0967D670.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/4EA65084-88A6-DD11-BE61-001D0968E1F8.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/5412FA71-7AA6-DD11-B45B-00145EDD7AD1.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/747BB271-7AA6-DD11-B3FC-00145EDD778D.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/802B83E5-7EA6-DD11-BDBE-001D0967D080.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/827C9707-7DA6-DD11-817F-001EC9ED5F1B.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/924CC262-A8A6-DD11-A84E-001EC9ED8F2B.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/BCD56477-7AA6-DD11-91F6-00145EDD78BB.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/CA0F3672-7AA6-DD11-9BDA-00145EFC5AA8.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/EA2FFDD4-9CA6-DD11-B011-001D0967D224.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/EA456277-7AA6-DD11-AAB0-00145EDD75D5.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/EA9B5772-7AA6-DD11-B2D3-00145EDD7715.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/EE7664B6-81A6-DD11-8FE5-001EC9ED4F7A.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/F0E0EF0B-99A6-DD11-B5EF-001D0967D5F8.root',
    '/store/mc/Summer08/QCDDiJetPt3500toInf/GEN-SIM-RECO/IDEAL_V9_v1/0000/FC775F77-7AA6-DD11-A895-00145EDD73ED.root'
    )
