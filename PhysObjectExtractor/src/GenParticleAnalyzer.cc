// -*- C++ -*-
//
// Package:    GenParticleAnalyzer
// Class:      GenParticleAnalyzer
//

// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

//classes to extract GenParticle information
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

//classes to save data
#include "TTree.h"
#include "TFile.h"
#include<vector>

//
// class declaration
//

class GenParticleAnalyzer : public edm::EDAnalyzer {
   public:
      explicit GenParticleAnalyzer(const edm::ParameterSet&);
      ~GenParticleAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      virtual void beginRun(edm::Run const&, edm::EventSetup const&);
      virtual void endRun(edm::Run const&, edm::EventSetup const&);
      virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
      virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) override;


      // ----------member data ---------------------------

      TTree *mtree;
      int nGenPart;
      std::vector<float> GenPart_pt;
      std::vector<float> GenPart_eta;
      std::vector<float> GenPart_mass;
      std::vector<int> GenPart_pdgId;
      std::vector<float> GenPart_phi;
      std::vector<int> GenPart_status;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//

GenParticleAnalyzer::GenParticleAnalyzer(const edm::ParameterSet& iConfig)

{
//now do what ever initialization is needed
	edm::Service<TFileService> fs;
	mtree = fs->make<TTree>("Events", "Events");

    mtree->Branch("nGenPart",&nGenPart);
    mtree->GetBranch("nGenPart")->SetTitle("number of interesting generator particles");
	mtree->Branch("GenPart_pt",&GenPart_pt);
    mtree->GetBranch("GenPart_pt")->SetTitle("generator particle transverse momentum");
    mtree->Branch("GenPart_eta",&GenPart_eta);
    mtree->GetBranch("GenPart_eta")->SetTitle("generator particle pseudorapidity");
    mtree->Branch("GenPart_mass",&GenPart_mass);
    mtree->GetBranch("GenPart_mass")->SetTitle("generator particle mass");
    mtree->Branch("GenPart_pdgId",&GenPart_pdgId);
    mtree->GetBranch("GenPart_pdgId")->SetTitle("generator particle PDG id");
    mtree->Branch("GenPart_phi",&GenPart_phi);
    mtree->GetBranch("GenPart_phi")->SetTitle("generator particle azimuthal angle of momentum vector");
    mtree->Branch("GenPart_status",&GenPart_status);
    mtree->GetBranch("GenPart_status")->SetTitle("generator particle status. 1=stable");
}

GenParticleAnalyzer::~GenParticleAnalyzer()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)
}


//
// member functions
//

// ------------ method called for each event  ------------
void
GenParticleAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
   using namespace std;

   Handle<reco::GenParticleCollection> gens;
   iEvent.getByLabel("genParticles", gens);

  //nGenPart=gens->size();
  if(gens.isValid())
  {
        for (reco::GenParticleCollection::const_iterator itGenPart=gens->begin(); itGenPart!=gens->end(); ++itGenPart)
        {
                GenPart_pt.push_back(itGenPart->pt());
                GenPart_eta.push_back(itGenPart->eta());
                GenPart_mass.push_back(itGenPart->mass());
                GenPart_pdgId.push_back(itGenPart->pdgId());
                GenPart_phi.push_back(itGenPart->phi());
                GenPart_status.push_back(itGenPart->status());
        }
  }
	
  mtree->Fill();
  return;

}

// ------------ method called once each job just before starting event loop  ------------
void
GenParticleAnalyzer::beginJob()
{}

// ------------ method called once each job just after ending the event loop  ------------
void
GenParticleAnalyzer::endJob()
{}

// ------------ method called when starting to processes a run  ------------
void
GenParticleAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{}

// ------------ method called when ending the processing of a run  ------------
void
GenParticleAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{}
// ------------ method called when starting to processes a luminosity block  ------------
void
GenParticleAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{}

// ------------ method called when ending the processing of a luminosity block  ------------
void
GenParticleAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
GenParticleAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
 //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(GenParticleAnalyzer);