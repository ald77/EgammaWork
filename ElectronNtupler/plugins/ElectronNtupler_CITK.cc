// -*- C++ -*-
//
// Package:    ElectronWork/ElectronNtupler_CITK
// Class:      ElectronNtupler_CITK
// 
/**\class ElectronNtupler_CITK ElectronNtupler_CITK.cc ElectronWork/ElectronNtupler_CITK/plugins/ElectronNtupler_CITK.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Ilya Kravchenko
//         Created:  Thu, 10 Jul 2014 09:54:13 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"

#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/PatCandidates/interface/Electron.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "DataFormats/PatCandidates/interface/PackedGenParticle.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"

#include "TTree.h"
#include "Math/VectorUtil.h"


namespace reco {
  typedef edm::Ptr<reco::GsfElectron> GsfElectronPtr;
}
//
// class declaration
//

class ElectronNtupler_CITK : public edm::EDAnalyzer {
public:
  explicit ElectronNtupler_CITK(const edm::ParameterSet&);
  ~ElectronNtupler_CITK();
  
  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);
  
  enum ElectronMatchType {UNMATCHED = 0, 
			  TRUE_PROMPT_ELECTRON, 
			  TRUE_ELECTRON_FROM_TAU,
			  TRUE_NON_PROMPT_ELECTRON}; // The last does not include tau parents
  
private:
  virtual void beginJob() override;
  virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
  virtual void endJob() override;
  
  //virtual void beginRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void endRun(edm::Run const&, edm::EventSetup const&) override;
  //virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  //virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;
  
  // MC truth matching utilities
  // The function that uses algorith from Josh Bendavid with 
  // an explicit loop over gen particles. 
  int matchToTruth(const pat::Electron &el, const edm::Handle<edm::View<reco::GenParticle>>  &prunedGenParticles);
  // The function that uses the standard genParticle() matching for electrons.
  int matchToTruthAlternative(const pat::Electron &el);
  
  bool checkAncestor(const reco::Candidate *gen, int ancestorPid);
  void findFirstNonElectronMother(const reco::Candidate *particle, int &ancestorPID, int &ancestorStatus);
  void printAllZeroMothers(const reco::Candidate *particle);
  
  // ----------member data ---------------------------
  edm::EDGetTokenT<reco::VertexCollection> vtxToken_;
  edm::EDGetTokenT<edm::View<PileupSummaryInfo> > pileupToken_;
  edm::EDGetTokenT<edm::View<reco::Candidate>> electronToken_;
  edm::EDGetTokenT<edm::View<reco::GenParticle> > prunedGenToken_;
  edm::EDGetTokenT<edm::View<pat::PackedGenParticle> > packedGenToken_;
  edm::EDGetTokenT<double> rhoToken_;
  
  //CITK
  edm::EDGetTokenT<edm::ValueMap<float> > ValueMaps_ChargedHadrons_;
  edm::EDGetTokenT<edm::ValueMap<float> > ValueMaps_NeutralHadrons_;
  edm::EDGetTokenT<edm::ValueMap<float> > ValueMaps_Photons_;
  edm::EDGetTokenT<edm::ValueMap<float> > ValueMaps_Mini_ChargedHadrons_;
  edm::EDGetTokenT<edm::ValueMap<float> > ValueMaps_Mini_NeutralHadrons_;
  edm::EDGetTokenT<edm::ValueMap<float> > ValueMaps_Mini_Photons_;
  
  TTree *electronTree_;
  
  // Vars for pile-up
  Int_t nPUTrue_;    // true pile-up
  Int_t nPU_;        // generated pile-up
  Int_t nPV_;        // number of reconsrtucted primary vertices
  Float_t rho_;      // the rho variable

  // all electron variables
  Float_t pt_;
  Float_t etaSC_;
  Float_t dEtaIn_;
  Float_t dPhiIn_;
  Float_t hOverE_;
  // Float_t sigmaIetaIeta_;
  Float_t full5x5_sigmaIetaIeta_;
  Float_t relIsoWithEA_;
  Float_t relIsoWithDBeta_;
  Float_t ooEmooP_;
  Float_t d0_;
  Float_t dz_;
  Int_t   expectedMissingInnerHits_;
  
  Int_t classification;
  
  // I comment this because it is not accessible in AOD
  //Int_t   passConversionVeto_;     
  Int_t   isTrueElectron_;
  
  Float_t isoChargedHadrons_;
  Float_t isoNeutralHadrons_;
  Float_t isoPhotons_;
  Float_t isoChargedFromPU_;
  
  Float_t relisoChargedHadrons_;
  Float_t relisoNeutralHadrons_;
  Float_t relisoPhotons_;
  
  //CITK
  Float_t sumChargedHadronPt_CITK;
  Float_t sumNeutralHadronPt_CITK;
  Float_t sumPhotonPt_CITK;
  Float_t sumMiniChargedHadronPt_CITK;
  Float_t sumMiniNeutralHadronPt_CITK;
  Float_t sumMiniPhotonPt_CITK;
  
  //PUPPI
  Float_t sumChargedHadronPt_PUPPI;
  Float_t sumNeutralHadronPt_PUPPI;
  Float_t sumPhotonPt_PUPPI;
  
  //PUPPINoLeptons
  Float_t sumChargedHadronPt_PUPPI_NoLeptons;
  Float_t sumNeutralHadronPt_PUPPI_NoLeptons;
  Float_t sumPhotonPt_PUPPI_NoLeptons;
  
  Float_t reliso_PUPPI, reliso_PUPPI_NoLeptons;
  
  Float_t relisoChargedHadronPt_CITK;
  Float_t relisoNeutralHadronPt_CITK;
  Float_t relisoPhotonPt_CITK;
  Float_t relisoMiniChargedHadronPt_CITK;
  Float_t relisoMiniNeutralHadronPt_CITK;
  Float_t relisoMiniPhotonPt_CITK;
  
  bool isEB;
};

//
// constants, enums and typedefs
//

// Effective areas for electrons from Giovanni P. and Cristina
// distributed as private slides in Jan 2015, derived for PHYS14
namespace EffectiveAreas {
  const int nEtaBins = 5;
  const float etaBinLimits[nEtaBins+1] = {
    0.0, 0.8, 1.3, 2.0, 2.2, 2.5};
  const float effectiveAreaValues[nEtaBins] = {
    0.1013, 0.0988, 0.0572, 0.0842, 0.1530};
}

//
// static data member definitions
//

//
// constructors and destructor
//
ElectronNtupler_CITK::ElectronNtupler_CITK(const edm::ParameterSet& iConfig):
  vtxToken_(consumes<reco::VertexCollection>(iConfig.getParameter<edm::InputTag>("vertices"))),
  pileupToken_(consumes<edm::View<PileupSummaryInfo> >(iConfig.getParameter<edm::InputTag>("pileup"))),
  electronToken_(consumes<edm::View<reco::Candidate> >(iConfig.getParameter<edm::InputTag>("electrons"))),
  prunedGenToken_(consumes<edm::View<reco::GenParticle> >(iConfig.getParameter<edm::InputTag>("pruned"))),
  rhoToken_(consumes<double> (iConfig.getParameter<edm::InputTag>("rho"))),
  
  //CITK
  ValueMaps_ChargedHadrons_(consumes<edm::ValueMap<float> > (iConfig.getParameter<edm::InputTag>( "ValueMaps_ChargedHadrons_src" ) ) ),
  ValueMaps_NeutralHadrons_(consumes<edm::ValueMap<float> > (iConfig.getParameter<edm::InputTag>( "ValueMaps_NeutralHadrons_src" ) ) ),
  ValueMaps_Photons_(consumes<edm::ValueMap<float> > (iConfig.getParameter<edm::InputTag>( "ValueMaps_Photons_src" ) ) ),
  ValueMaps_Mini_ChargedHadrons_(consumes<edm::ValueMap<float> > (iConfig.getParameter<edm::InputTag>( "ValueMaps_Mini_ChargedHadrons_src" ) ) ),
  ValueMaps_Mini_NeutralHadrons_(consumes<edm::ValueMap<float> > (iConfig.getParameter<edm::InputTag>( "ValueMaps_Mini_NeutralHadrons_src" ) ) ),
  ValueMaps_Mini_Photons_(consumes<edm::ValueMap<float> > (iConfig.getParameter<edm::InputTag>( "ValueMaps_Mini_Photons_src" ) ) ){
  edm::Service<TFileService> fs;
  electronTree_ = fs->make<TTree> ("ElectronTree", "Electron data");
  
  electronTree_->Branch("nPV"        ,  &nPV_     , "nPV/I");
  electronTree_->Branch("nPU"        ,  &nPU_     , "nPU/I");
  electronTree_->Branch("nPUTrue"    ,  &nPUTrue_ , "nPUTrue/I");
  electronTree_->Branch("rho"        ,  &rho_ , "rho/F");

  electronTree_->Branch("pt"    ,  &pt_    , "pt/F");			    
  electronTree_->Branch("etaSC" ,  &etaSC_ , "etaSC/F");
  electronTree_->Branch("dEtaIn",  &dEtaIn_, "dEtaIn/F");
  electronTree_->Branch("dPhiIn",  &dPhiIn_, "dPhiIn/F");
  electronTree_->Branch("hOverE",  &hOverE_, "hOverE/F");
  // electronTree_->Branch("sigmaIetaIeta",         &sigmaIetaIeta_, "sigmaIetaIeta/F");
  electronTree_->Branch("full5x5_sigmaIetaIeta", &full5x5_sigmaIetaIeta_, "full5x5_sigmaIetaIeta/F");
 
  electronTree_->Branch("isoChargedFromPU"       , &isoChargedFromPU_);
  electronTree_->Branch("relIsoWithEA"           , &relIsoWithEA_, "relIsoWithEA/F");
  electronTree_->Branch("relIsoWithDBeta"      , &relIsoWithDBeta_, "relIsoWithDBeta/F");
  electronTree_->Branch("ooEmooP", &ooEmooP_, "ooEmooP/F");
  electronTree_->Branch("d0"     , &d0_,      "d0/F");
  electronTree_->Branch("dz"     , &dz_,      "dz/F");
  electronTree_->Branch("expectedMissingInnerHits", &expectedMissingInnerHits_, "expectedMissingInnerHits/I");
  
  electronTree_->Branch("classification"    , &classification,     "classification/I");
  
  //electronTree_->Branch("passConversionVeto", &passConversionVeto_, "passConversionVeto/I");
  electronTree_->Branch("isTrueElectron"    , &isTrueElectron_,     "isTrueElectron/I");
 
  electronTree_->Branch("isoChargedHadrons"      , &isoChargedHadrons_ , "isoChargedHadrons/F");
  electronTree_->Branch("isoNeutralHadrons"      , &isoNeutralHadrons_, "isoNeutralHadrons/F");
  electronTree_->Branch("isoPhotons"             , &isoPhotons_, "isoPhotons/F");
  
  electronTree_->Branch("relisoChargedHadrons"      , &relisoChargedHadrons_ , "relisoChargedHadrons/F");
  electronTree_->Branch("relisoNeutralHadrons"      , &relisoNeutralHadrons_, "relisoNeutralHadrons/F");
  electronTree_->Branch("relisoPhotons"             , &relisoPhotons_, "relisoPhotons/F");
  
  //CITK
  electronTree_ -> Branch("sumChargedHadronPt_CITK", &sumChargedHadronPt_CITK, "sumChargedHadronPt_CITK/F");
  electronTree_ -> Branch("sumNeutralHadronPt_CITK", &sumNeutralHadronPt_CITK, "sumNeutralHadronPt_CITK/F");
  electronTree_ -> Branch("sumPhotonPt_CITK", &sumPhotonPt_CITK, "sumPhotonPt_CITK/F");
  electronTree_ -> Branch("sumMiniChargedHadronPt_CITK", &sumMiniChargedHadronPt_CITK, "sumMiniChargedHadronPt_CITK/F");
  electronTree_ -> Branch("sumMiniNeutralHadronPt_CITK", &sumMiniNeutralHadronPt_CITK, "sumMiniNeutralHadronPt_CITK/F");
  electronTree_ -> Branch("sumMiniPhotonPt_CITK", &sumMiniPhotonPt_CITK, "sumMiniPhotonPt_CITK/F");
    
  electronTree_ -> Branch("relisoChargedHadronPt_CITK", &relisoChargedHadronPt_CITK, "relisoChargedHadronPt_CITK/F");
  electronTree_ -> Branch("relisoNeutralHadronPt_CITK", &relisoNeutralHadronPt_CITK, "relisoNeutralHadronPt_CITK/F");
  electronTree_ -> Branch("relisoPhotonPt_CITK", &relisoPhotonPt_CITK, "relisoPhotonPt_CITK/F");
  electronTree_ -> Branch("relisoMiniChargedHadronPt_CITK", &relisoMiniChargedHadronPt_CITK, "relisoMiniChargedHadronPt_CITK/F");
  electronTree_ -> Branch("relisoMiniNeutralHadronPt_CITK", &relisoMiniNeutralHadronPt_CITK, "relisoMiniNeutralHadronPt_CITK/F");
  electronTree_ -> Branch("relisoMiniPhotonPt_CITK", &relisoMiniPhotonPt_CITK, "relisoMiniPhotonPt_CITK/F");
  
  electronTree_ -> Branch("isEB", &isEB, "isEB/B");

}


ElectronNtupler_CITK::~ElectronNtupler_CITK()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
ElectronNtupler_CITK::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  using namespace std;
  using namespace edm;
  using namespace reco;
  
  // Pruned particles are the one containing "important" stuff
 
  Handle<edm::View<reco::GenParticle> > prunedGenParticles;
  iEvent.getByToken(prunedGenToken_,prunedGenParticles);


  // Get Pileup info
  Handle<edm::View<PileupSummaryInfo> > pileupHandle;
  iEvent.getByToken(pileupToken_, pileupHandle);
  for( auto & puInfoElement : *pileupHandle){
    if( puInfoElement.getBunchCrossing() == 0 ){
      nPU_    = puInfoElement.getPU_NumInteractions();
      nPUTrue_= puInfoElement.getTrueNumInteractions();
    }
  }

  // Get PV
  edm::Handle<reco::VertexCollection> vertices;
  iEvent.getByToken(vtxToken_, vertices);
  if (vertices->empty()) return; // skip the event if no PV found
  //const reco::Vertex &pv = vertices->front();
  
  nPV_ = vertices -> size();

  VertexCollection::const_iterator firstGoodVertex = vertices->end();
  int firstGoodVertexIdx = 0;
  for (VertexCollection::const_iterator vtx = vertices->begin(); 
       vtx != vertices->end(); ++vtx, ++firstGoodVertexIdx) {
    // The "good vertex" selection is borrowed from Giovanni Zevi Della Porta
    // Replace isFake() for miniAOD because it requires tracks and miniAOD vertices don't have tracks:
    // Vertex.h: bool isFake() const {return (chi2_==0 && ndof_==0 && tracks_.empty());}
    if (  /*!vtx->isFake() &&*/ 
	!(vtx->chi2()==0 && vtx->ndof()==0) 
	&&  vtx->ndof()>=4. && vtx->position().Rho()<=2.0
	&& fabs(vtx->position().Z())<=24.0) {
      firstGoodVertex = vtx;
      break;
    }
  }
  
  if ( firstGoodVertex==vertices->end() )
    return; // skip event if there are no good PVs
  
  
  edm::Handle< double > rhoH;
  iEvent.getByToken(rhoToken_,rhoH);
  rho_ = *rhoH;
  
  // Get electron collection
  Handle<edm::View<reco::Candidate> > electrons;
  iEvent.getByToken(electronToken_, electrons);
    
  //CITK
  Handle <edm::ValueMap <float> > ValueMaps_ChargedHadrons, ValueMaps_NeutralHadrons, ValueMaps_Photons;
  Handle <edm::ValueMap <float> > ValueMaps_Mini_ChargedHadrons, ValueMaps_Mini_NeutralHadrons, ValueMaps_Mini_Photons;
  Handle <edm::ValueMap <float> > ValueMaps_PUPPI_ChargedHadrons, ValueMaps_PUPPI_NeutralHadrons, ValueMaps_PUPPI_Photons;
  Handle <edm::ValueMap <float> > ValueMaps_PUPPI_NoLeptons_ChargedHadrons, ValueMaps_PUPPI_NoLeptons_NeutralHadrons, ValueMaps_PUPPI_NoLeptons_Photons;
  
  //CITK
  iEvent.getByToken( ValueMaps_ChargedHadrons_ , ValueMaps_ChargedHadrons);
  iEvent.getByToken( ValueMaps_NeutralHadrons_ , ValueMaps_NeutralHadrons);
  iEvent.getByToken( ValueMaps_Photons_ , ValueMaps_Photons);
  iEvent.getByToken( ValueMaps_Mini_ChargedHadrons_ , ValueMaps_Mini_ChargedHadrons);
  iEvent.getByToken( ValueMaps_Mini_NeutralHadrons_ , ValueMaps_Mini_NeutralHadrons);
  iEvent.getByToken( ValueMaps_Mini_Photons_ , ValueMaps_Mini_Photons);


  //
  // Loop over electrons
  //
  // printf("DEBUG: new event\n"); 
  for (unsigned int iElectron = 0; iElectron < electrons -> size(); iElectron++) {
    
    
    auto elePtr = electrons -> ptrAt(iElectron);
    reco::GsfElectronPtr eleGsfPtr(elePtr);
    // Kinematics
    pt_ = eleGsfPtr -> pt();
    
    // Keep only electrons above 10 GeV.
    // NOTE: miniAOD does not store some of the info for electrons <5 GeV at all!
    if( pt_ < 10 ) 
      continue;
    
    etaSC_ = eleGsfPtr -> superCluster()->eta();
    
    // ID and matching
    dEtaIn_ = eleGsfPtr -> deltaEtaSuperClusterTrackAtVtx();
    dPhiIn_ = eleGsfPtr -> deltaPhiSuperClusterTrackAtVtx();
    hOverE_ = eleGsfPtr -> hcalOverEcal();
    // sigmaIetaIeta_ = el.sigmaIetaIeta();
    full5x5_sigmaIetaIeta_ = eleGsfPtr -> full5x5_sigmaIetaIeta();
    // |1/E-1/p| = |1/E - EoverPinner/E| is computed below
    // The if protects against ecalEnergy == inf or zero (always
    // the case for electrons below 5 GeV in miniAOD)
    if( eleGsfPtr -> ecalEnergy() == 0 ){
      printf("Electron energy is zero!\n");
      ooEmooP_ = 1e30;
    }else if( !std::isfinite(eleGsfPtr -> ecalEnergy())){
      printf("Electron energy is not finite!\n");
      ooEmooP_ = 1e30;
    }else{
      ooEmooP_ = fabs(1.0/eleGsfPtr -> ecalEnergy() - eleGsfPtr -> eSuperClusterOverP()/eleGsfPtr -> ecalEnergy() );
    }
    
    // Isolation
    GsfElectron::PflowIsolationVariables pfIso = eleGsfPtr -> pfIsolationVariables();
    isoChargedHadrons_ = pfIso.sumChargedHadronPt ;
    isoNeutralHadrons_ = pfIso.sumNeutralHadronEt ;
    isoPhotons_        = pfIso.sumPhotonEt ;
    isoChargedFromPU_  = pfIso.sumPUPt ;
    
    relisoChargedHadrons_ = pfIso.sumChargedHadronPt/pt_;
    relisoNeutralHadrons_ = pfIso.sumNeutralHadronEt/pt_;
    relisoPhotons_        = pfIso.sumPhotonEt/pt_;
    // Compute isolation with effective area correction for PU
    // Find eta bin first. If eta>2.5, the last eta bin is used.
    int etaBin = 0; 
    while ( etaBin < EffectiveAreas::nEtaBins-1 
	    && abs(etaSC_) > EffectiveAreas::etaBinLimits[etaBin+1] )
      { ++etaBin; };
    double area = EffectiveAreas::effectiveAreaValues[etaBin];
    relIsoWithEA_ = ( pfIso.sumChargedHadronPt + max(0.0, pfIso.sumNeutralHadronEt + pfIso.sumPhotonEt - rho_ * area ) )/pt_;
    
    // Compute isolation with delta beta correction for PU
    float absiso = pfIso.sumChargedHadronPt + max(0.0 , pfIso.sumNeutralHadronEt + pfIso.sumPhotonEt - 0.5 * pfIso.sumPUPt );
    relIsoWithDBeta_ = absiso/pt_;
    
    // Impact parameter
    d0_ = (-1) * eleGsfPtr -> gsfTrack()->dxy(firstGoodVertex->position() );
    dz_ = eleGsfPtr -> gsfTrack()->dz( firstGoodVertex->position() );
    
    // Conversion rejection
    // pre-72X method below is commented out
    //expectedMissingInnerHits_ = el.gsfTrack()->trackerExpectedHitsInner().numberOfLostHits();
    // since 72X, the access of missing hits is this:
    expectedMissingInnerHits_ = eleGsfPtr -> gsfTrack()->hitPattern().numberOfHits(reco::HitPattern::MISSING_INNER_HITS);
    // I comment this because it is not accessible in AOD
    //passConversionVeto_ = elePatPtr -> passConversionVeto();
    
     classification = eleGsfPtr -> classification();
    // Match to generator level truth
    
    // 
    // Explicit loop over gen candidates method
    //
    isTrueElectron_ = matchToTruth( eleGsfPtr, prunedGenParticles);
    
     // I comment this because it is not accessible in AOD
    // isTrueElectronAlternative_ = matchToTruthAlternative( el );
    
    // For debug purposes, one can use this utility that prints 
    // the decay history, using standard matching in this case:
    //   printAllZeroMothers( el.genParticle() );
    
    //CITK
    sumChargedHadronPt_CITK =  (*ValueMaps_ChargedHadrons)[elePtr];
    sumNeutralHadronPt_CITK =  (*ValueMaps_NeutralHadrons)[elePtr];
    sumPhotonPt_CITK        =  (*ValueMaps_Photons)[elePtr];
    sumMiniChargedHadronPt_CITK =  (*ValueMaps_Mini_ChargedHadrons)[elePtr];
    sumMiniNeutralHadronPt_CITK =  (*ValueMaps_Mini_NeutralHadrons)[elePtr];
    sumMiniPhotonPt_CITK        =  (*ValueMaps_Mini_Photons)[elePtr];
  
    relisoChargedHadronPt_CITK = sumChargedHadronPt_CITK/pt_;
    relisoNeutralHadronPt_CITK = sumNeutralHadronPt_CITK/pt_;
    relisoPhotonPt_CITK = sumPhotonPt_CITK/pt_;
    relisoMiniChargedHadronPt_CITK = sumMiniChargedHadronPt_CITK/pt_;
    relisoMiniNeutralHadronPt_CITK = sumMiniNeutralHadronPt_CITK/pt_;
    relisoMiniPhotonPt_CITK = sumMiniPhotonPt_CITK/pt_;
    
    
    const reco::CaloClusterPtr& seed = eleGsfPtr -> superCluster()->seed();
    isEB = ( seed->seed().subdetId() == EcalBarrel );
         
    // Save this electron's info
    electronTree_->Fill();
  }
  
}


// ------------ method called once each job just before starting event loop  ------------
void 
ElectronNtupler_CITK::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
ElectronNtupler_CITK::endJob() 
{
}

// ------------ method called when starting to processes a run  ------------
/*
void 
ElectronNtupler_CITK::beginRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a run  ------------
/*
void 
ElectronNtupler_CITK::endRun(edm::Run const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when starting to processes a luminosity block  ------------
/*
void 
ElectronNtupler_CITK::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method called when ending the processing of a luminosity block  ------------
/*
void 
ElectronNtupler_CITK::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
}
*/

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ElectronNtupler_CITK::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

bool ElectronNtupler_CITK::checkAncestor(const reco::Candidate *gen, int ancestorPid){

  // General sanity check
  if( gen == 0 ){
    printf("ElectronNtupler_CITK::checkAncestor: ERROR null particle is passed in, ignore it.\n");
    return false;
  }

  // If this is true, we found our target ancestor
  if( abs( gen->pdgId() ) == ancestorPid )
    return true;

  // Go deeper and check all mothers
  for(size_t i=0;i< gen->numberOfMothers();i++) {
    if ( checkAncestor( gen->mother(i), ancestorPid) )
      return true;
  }
  
  return false;
}

// The function that uses algorith from Josh Bendavid with 
// an explicit loop over gen particles. 
int ElectronNtupler_CITK::matchToTruth(const pat::Electron &el, 
				  const edm::Handle<edm::View<reco::GenParticle>> &prunedGenParticles){

  // 
  // Explicit loop and geometric matching method (advised by Josh Bendavid)
  //

  // Find the closest status 1 gen electron to the reco electron
  double dR = 999;
  const reco::Candidate *closestElectron = 0;
  for(size_t i=0; i<prunedGenParticles->size();i++){
    const reco::Candidate *particle = &(*prunedGenParticles)[i];
    // Drop everything that is not electron or not status 1
    if( abs(particle->pdgId()) != 11 || particle->status() != 1 )
      continue;
    //
    double dRtmp = ROOT::Math::VectorUtil::DeltaR( el.p4(), particle->p4() );
    if( dRtmp < dR ){
      dR = dRtmp;
      closestElectron = particle;
    }
  }
  // See if the closest electron (if it exists) is close enough.
  // If not, no match found.
  if( !(closestElectron != 0 && dR < 0.1) ) {
    return UNMATCHED;
  }

  // 
  int ancestorPID = -999; 
  int ancestorStatus = -999;
  findFirstNonElectronMother(closestElectron, ancestorPID, ancestorStatus);

  if( ancestorPID == -999 && ancestorStatus == -999 ){
    // No non-electron parent??? This should never happen.
    // Complain.
    printf("ElectronNtupler_CITK: ERROR! Electron does not apper to have a non-electron parent\n");
    return UNMATCHED;
  }
  
  if( abs(ancestorPID) > 50 && ancestorStatus == 2 )
    return TRUE_NON_PROMPT_ELECTRON;

  if( abs(ancestorPID) == 15 && ancestorStatus == 2 )
    return TRUE_ELECTRON_FROM_TAU;

  // What remains is true prompt electrons
  return TRUE_PROMPT_ELECTRON;
}

void ElectronNtupler_CITK::findFirstNonElectronMother(const reco::Candidate *particle,
						 int &ancestorPID, int &ancestorStatus){

  if( particle == 0 ){
    printf("ElectronNtupler_CITK: ERROR! null candidate pointer, this should never happen\n");
    return;
  }

  // Is this the first non-electron parent? If yes, return, otherwise
  // go deeper into recursion
  if( abs(particle->pdgId()) == 11 ){
    findFirstNonElectronMother(particle->mother(0), ancestorPID, ancestorStatus);
  }else{
    ancestorPID = particle->pdgId();
    ancestorStatus = particle->status();
  }

  return;
}

// The function that uses the standard genParticle() matching for electrons.
int ElectronNtupler_CITK::matchToTruthAlternative(const pat::Electron &el){

     //
     // genParticle method
     //
     int result = UNMATCHED;

     const reco::GenParticle * gen = el.genParticle();
     if( gen != 0 ){
       int pid = gen->pdgId();
       int status = gen->status();
       bool isFromZ   = checkAncestor(gen, 23);
       bool isFromW   = checkAncestor(gen, 24);
       bool isFromTau = checkAncestor(gen, 15);
       // Check if it is a true prompt electron
       if( abs( pid ) == 11 // this is electron
	   && (status == 1 || status == 22 || status == 23 ) // NOTE: Pythia8 status here 22/23 (for Pythia6 would be 3)
	   && (isFromZ || isFromW ) && !isFromTau // comes from Z or W+-, but not from tau
	   )
	 {
	   result = TRUE_PROMPT_ELECTRON;
	 } else if ( abs( pid ) == 11 
		     && (status == 1 || status == 22 || status == 23 ) 
		     && (isFromTau ) 
		     ) 
	 {
	   // This is a true electron, but it comes from tau
	   result = TRUE_ELECTRON_FROM_TAU;
	 } else if ( abs( pid ) == 11 )
	 {
	   // This is a true electron, but it comes from something else
	   const reco::Candidate *mom = el.mother(0);
	   int momPid = -999;
	   if ( mom != 0 )
	     momPid = mom->pdgId();
	   printf("pid= %d  status= %d isFromZ= %d isFromW= %d  isFromTau= %d  momPid= %d\n", 
		  pid,  status, isFromZ, isFromW, isFromTau, momPid);
	   result = TRUE_NON_PROMPT_ELECTRON;
	 } else {
	 printf("The reco electron has a truth match with pid= %d\n", pid);
       }
     }

     return result;
}

void ElectronNtupler_CITK::printAllZeroMothers(const reco::Candidate *particle){
  
  if( particle == 0 ){
    printf("ElectronNtupler_CITK::printAllZeroMothers: reached the top of the decay tree\n");
    return;
  }
  
  printf("ElectronNtupler_CITK::printAllZeroMothers: ancestor ID= %d, status= %d\n",
	 particle->pdgId(), particle->status() );

  printAllZeroMothers( particle->mother(0) );

  return;
}

//define this as a plug-in
DEFINE_FWK_MODULE(ElectronNtupler_CITK);
