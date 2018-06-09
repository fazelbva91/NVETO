#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4VUserPhysicsList.hh"
#include "G4ProcessManager.hh"
#include "G4VPhysicsConstructor.hh"

#include "G4Electron.hh"
#include "G4Proton.hh"
#include "G4Neutron.hh"
#include "G4Gamma.hh"

#include "G4EmStandardPhysics_option4.hh"
#include "G4HadronPhysicsQGS_BIC.hh"

#include "G4HadronCaptureProcess.hh"
#include "G4NeutronHPCaptureData.hh"
#include "G4NeutronHPCapture.hh"


class PhysicsList: public G4VUserPhysicsList {
public:
  PhysicsList();
  ~PhysicsList();
  
  void ConstructParticle() {
  		G4Electron::ElectronDefinition();
    	G4Proton::ProtonDefinition();
    	G4Neutron::NeutronDefinition();
   		G4Gamma::GammaDefinition();
  }
       
  void ConstructProcss() {
       //EM physics						   
       G4VPhysicsConstructor* emList = new G4EmStandardPhysics_option4();
       emList->ConstructProcess();
       
       //Inlastic Physics for neutrons
       G4VPhysicsConstructor* nList = new G4HadronPhysicsQGS_BIC();
       nList->ConstructProcess();
       
     	//Neutron capture
  		GetParticleIterator()->reset();
  		while((*GetParticleIterator())()) {
      		G4ParticleDefinition* particle = GetParticleIterator()->value();
      		G4ProcessManager*     pmanager = particle->GetProcessManager();
      		G4String              pname    = particle->GetParticleName();
     	
		 	if (pname == "neutron") {
		   		G4HadronCaptureProcess* the_neutron_capture_process = new G4HadronCaptureProcess();
		   		G4NeutronHPCaptureData* the_HP_capture_data = new G4NeutronHPCaptureData();
		   		the_neutron_capture_process->AddDataSet(the_HP_capture_data);
		   		G4NeutronHPCapture* the_neutron_capture_model = new G4NeutronHPCapture();
		   		the_neutron_capture_process->RegisterMe(the_neutron_capture_model);
		   		//the_neutron_capture_process->AddEmModel(0, the_neutron_capture_model);
		   		pmanager->AddDiscreteProcess(the_neutron_capture_process);
		   	}  
		}  
  }
  
  //void SetParticleCuts(G4double cut, const G4String& particleName, G4Region* region) {
  void SetParticleCuts(G4double cut, const G4String& particleName) {
	if (cut<0.0) {
    	#ifdef G4VERBOSE
		if (verboseLevel >0){
    		G4cout << "G4VUserPhysicsList::SetParticleCuts: negative cut values"
                	<< "  :" << cut/CLHEP::mm << "[mm]"
                	<< " for "<< particleName << G4endl;
    	}
       #endif
       
       return;
   	}
  }

};

