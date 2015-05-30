import FWCore.ParameterSet.Config as cms

process = cms.Process( "ElectronIsolation" )
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
    )

process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))
process.options.allowUnscheduled = cms.untracked.bool(False)

process.ElectronIsolation = cms.EDProducer("CITKPFIsolationSumProducer",
                                           srcToIsolate = cms.InputTag("slimmedElectrons"),
                                           srcForIsolationCone = cms.InputTag('packedPFCandidates'),
                                           isolationConeDefinitions = cms.VPSet(
                                               cms.PSet( isolationAlgo = cms.string('ElectronPFIsolationWithConeVeto'),
                                                         coneSize = cms.double(0.3),
                                                         VetoConeSizeEndcaps = cms.double(0.015),
                                                         VetoConeSizeBarrel = cms.double(0.0),
                                                         isolateAgainst = cms.string('h+'),
                                                         miniAODVertexCodes = cms.vuint32(2,3) ),
                                               cms.PSet( isolationAlgo = cms.string('ElectronPFIsolationWithConeVeto'),
                                                         coneSize = cms.double(0.3),
                                                         VetoConeSizeEndcaps = cms.double(0.0),
                                                         VetoConeSizeBarrel = cms.double(0.0),
                                                         isolateAgainst = cms.string('h0'),
                                                         miniAODVertexCodes = cms.vuint32(2,3) ),
                                               cms.PSet( isolationAlgo = cms.string('ElectronPFIsolationWithConeVeto'),
                                                         coneSize = cms.double(0.3),
                                                         VetoConeSizeEndcaps = cms.double(0.08),
                                                         VetoConeSizeBarrel = cms.double(0.0),
                                                         isolateAgainst = cms.string('gamma'),
                                                         miniAODVertexCodes = cms.vuint32(2,3) ),
                                               cms.PSet( isolationAlgo = cms.string('ElectronMiniIsolationWithConeVeto'),
                                                         coneSize = cms.double(0.2),
                                                         MinConeSize = cms.double(0.05),
                                                         ktScale = cms.double(10.),
                                                         VetoConeSizeEndcaps = cms.double(0.015),
                                                         VetoConeSizeBarrel = cms.double(0.0),
                                                         isolateAgainst = cms.string('h+'),
                                                         miniAODVertexCodes = cms.vuint32(2,3) ),
                                               cms.PSet( isolationAlgo = cms.string('ElectronMiniIsolationWithConeVeto'),
                                                         coneSize = cms.double(0.2),
                                                         MinConeSize = cms.double(0.05),
                                                         ktScale = cms.double(10.),
                                                         VetoConeSizeEndcaps = cms.double(0.0),
                                                         VetoConeSizeBarrel = cms.double(0.0),
                                                         isolateAgainst = cms.string('h0'),
                                                         miniAODVertexCodes = cms.vuint32(2,3) ),
                                               cms.PSet( isolationAlgo = cms.string('ElectronMiniIsolationWithConeVeto'),
                                                         coneSize = cms.double(0.2),
                                                         MinConeSize = cms.double(0.05),
                                                         ktScale = cms.double(10.),
                                                         VetoConeSizeEndcaps = cms.double(0.08),
                                                         VetoConeSizeBarrel = cms.double(0.0),
                                                         isolateAgainst = cms.string('gamma'),
                                                         miniAODVertexCodes = cms.vuint32(2,3) ),
                                               )
                                               )
process.ntupler = cms.EDAnalyzer('ElectronNtupler_CITK',
                                 pruned = cms.InputTag("prunedGenParticles"),
                                 pileup = cms.InputTag("addPileupInfo"),
                                 vertices = cms.InputTag("offlineSlimmedPrimaryVertices"),
                                 electrons = cms.InputTag("slimmedElectrons"),
                                 rho = cms.InputTag("fixedGridRhoFastjetAll"),
                                 #CITK
                                 ValueMaps_ChargedHadrons_src = cms.InputTag("ElectronIsolation", "h+-DR030-BarVeto000-EndVeto001"),
                                 ValueMaps_NeutralHadrons_src = cms.InputTag("ElectronIsolation", "h0-DR030-BarVeto000-EndVeto000"),
                                 ValueMaps_Photons_src = cms.InputTag("ElectronIsolation", "gamma-DR030-BarVeto000-EndVeto008"),
                                 #Mini
                                 ValueMaps_Mini_ChargedHadrons_src = cms.InputTag("ElectronIsolation", "h+-DR020-BarVeto000-EndVeto001-kt1000-Min005"),
                                 ValueMaps_Mini_NeutralHadrons_src = cms.InputTag("ElectronIsolation", "h0-DR020-BarVeto000-EndVeto000-kt1000-Min005"),
                                 ValueMaps_Mini_Photons_src = cms.InputTag("ElectronIsolation", "gamma-DR020-BarVeto000-EndVeto008-kt1000-Min005"),
                                 )

process.electrons = cms.Path(process.ElectronIsolation + process.ntupler)

#process.maxEvents.input = 1000
process.source = cms.Source("PoolSource",
                            secondaryFileNames = cms.untracked.vstring(),
                            #    fileNames = cms.untracked.vstring('file:////afs/cern.ch/work/i/ishvetso/RunII_preparation/Synchronization_March2015/miniAOD/RSGravitonToWW_kMpl01_M_1000_Tune4C_13TeV_pythia8_synch_exercise.root')
                            #                            fileNames = cms.untracked.vstring("root://cms-xrd-global.cern.ch//store/mc/Phys14DR/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/MINIAODSIM/PU20bx25_PHYS14_25_V1-v1/00000/00C90EFC-3074-E411-A845-002590DB9262.root")
                            fileNames = cms.untracked.vstring("file:/home/users/ald77/cmssw/CMSSW_7_3_3/src/Phys14DR_TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_MINIAODSIM_PU20bx25_PHYS14_25_V1-v1.root")
                            )

process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 10000

'''process.out = cms.OutputModule("PoolOutputModule",
fileName = cms.untracked.string('patTuple.root'),
outputCommands = cms.untracked.vstring('keep *')
)

process.outpath = cms.EndPath(process.out)
'''
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("tree.root")
                                   )
