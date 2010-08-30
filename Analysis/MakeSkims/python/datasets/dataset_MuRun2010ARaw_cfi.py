import FWCore.ParameterSet.Config as cms

## production release = none

dataset = "/Mu/Run2010A-v1/RAW"

## global tag
globaltag = cms.string('GR_R_38X_V9::All')

## is MC
isMC = False

## read files
readFiles = cms.untracked.vstring()
readFiles.append('/store/data/Run2010A/Mu/RAW/v1/000/136/066/0CC92E9E-5766-DF11-8897-001617E30D40.root')
readFiles.append('/store/data/Run2010A/Mu/RAW/v1/000/136/066/366F7AA3-5E66-DF11-9F20-0030487C7828.root')
readFiles.append('/store/data/Run2010A/Mu/RAW/v1/000/136/066/48AF4995-5066-DF11-ACEE-0030487CD77E.root')
readFiles.append('/store/data/Run2010A/Mu/RAW/v1/000/136/066/66596ADC-4866-DF11-8840-000423D94A20.root')
readFiles.append('/store/data/Run2010A/Mu/RAW/v1/000/136/066/BA8C68E0-6966-DF11-9366-0030487C8CBE.root')
readFiles.append('/store/data/Run2010A/Mu/RAW/v1/000/136/066/C0B29590-4266-DF11-9599-000423D33970.root')
readFiles.append('/store/data/Run2010A/Mu/RAW/v1/000/136/066/D6E50538-3C66-DF11-831B-000423D99896.root')
readFiles.append('/store/data/Run2010A/Mu/RAW/v1/000/136/066/E21D28E7-3566-DF11-BCD5-0030487CD7E0.root')

## out files
outfile = cms.untracked.string('MuRun2010ARaw.root')

## good run list
from Analysis.MakeSkims.datasets.json_July15thReReco_cfi import *

## do other stuff
def doOtherStuff(process):
    pass


