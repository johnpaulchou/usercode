import FWCore.ParameterSet.Config as cms

generator = cms.EDFilter("Pythia6GeneratorFilter",
                         crossSection = cms.untracked.double(-1),
                         maxEventsToPrint = cms.untracked.int32(0),
                         pythiaPylistVerbosity = cms.untracked.int32(0),
                         filterEfficiency = cms.untracked.double(1.0),
                         pythiaHepMCVerbosity = cms.untracked.bool(False),
                         comEnergy = cms.double(7000.0),
                         ExternalDecays = cms.PSet( Tauola = cms.untracked.PSet( UseTauolaPolarization = cms.bool(True),
                                                                                 InputCards = cms.PSet( mdtau = cms.int32(0),
                                                                                                        pjak2 = cms.int32(0),
                                                                                                        pjak1 = cms.int32(0) )
                                                                                 ),
                                                    parameterSets = cms.vstring('Tauola')
                                                    ),
                         PythiaParameters = cms.PSet( pythiaUESettings = cms.vstring( 'MSTJ(11)=3      ! Choice of the fragmentation function', 
                                                                                      'MSTJ(22)=2      ! Decay those unstable particles', 
                                                                                      'PARJ(71)=10 .   ! for which ctau  10 mm', 
                                                                                      'MSTP(2)=1       ! which order running alphaS', 
                                                                                      'MSTP(33)=0      ! no K factors in hard cross sections', 
                                                                                      'MSTP(51)=10042  ! structure function chosen (external PDF CTEQ6L1)', 
                                                                                      'MSTP(52)=2      ! work with LHAPDF', 
                                                                                      'MSTP(81)=1      ! multiple parton interactions 1 is Pythia default', 
                                                                                      'MSTP(82)=4      ! Defines the multi-parton model', 
                                                                                      'MSTU(21)=1      ! Check on possible errors during program execution', 
                                                                                      'PARP(82)=1.8387 ! pt cutoff for multiparton interactions', 
                                                                                      'PARP(89)=1960.  ! sqrts for which PARP82 is set', 
                                                                                      'PARP(83)=0.5    ! Multiple interactions: matter distrbn parameter', 
                                                                                      'PARP(84)=0.4    ! Multiple interactions: matter distribution parameter', 
                                                                                      'PARP(90)=0.16   ! Multiple interactions: rescaling power', 
                                                                                      'PARP(67)=2.5    ! amount of initial-state radiation', 
                                                                                      'PARP(85)=1.0    ! gluon prod. mechanism in MI', 
                                                                                      'PARP(86)=1.0    ! gluon prod. mechanism in MI', 
                                                                                      'PARP(62)=1.25   ! ', 
                                                                                      'PARP(64)=0.2    ! ', 
                                                                                      'MSTP(91)=1      !', 
                                                                                      'PARP(91)=2.1    ! kt distribution', 
                                                                                      'PARP(93)=15.0   ! ' ),

                                                      SLHAParameters = cms.vstring('SLHAFILE = Analysis/Generator/data/taustar.out'),
                                                      
                                                      processParameters = cms.vstring( 'MSEL=0                ! custom process',
                                                                                       'MSUB(169)=1           ! q+qbar->e*^+- e^-+',
                                                                                       'KFPR(169,1)=15        ! replace the standard electron with a standard tau',
                                                                                       'KFPR(169,2)=4000015   ! replace the excited electron with an excited tau',
                                                                                       'IMSS(21)=33           ! look up parameters in the SLHA file',
                                                                                       'IMSS(22)=33           !',
                                                                                       'RTCM(41)=10000.0      ! Lambda (compositeness scale)',
                                                                                       'RTCM(43)=1.0          ! SU(2) coupling',
                                                                                       'RTCM(44)=1.0          ! U(1) coupling',
                                                                                       'RTCM(45)=1.0          ! SU(3) coupling',
                                                                                       'PMAS(4000015,1)=400.0   ! leptoquark mass',
                                                                                       ),

                                                      parameterSets = cms.vstring( 'pythiaUESettings', 
                                                                                   'SLHAParameters',
                                                                                   'processParameters')
                                                      )
                         )

configurationMetadata = cms.untracked.PSet(version = cms.untracked.string('$Revision: 1.0 $'),
                                           name = cms.untracked.string('$Source: $'),
                                           annotation = cms.untracked.string('PYTHIA6-TAUSTAR at 7TeV')
                                           )
