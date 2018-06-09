//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4MTRunManager.hh"
#include "G4ScoringManager.hh"
#include "G4UImanager.hh"

#include "UserActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "RunAction.hh"

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "QGSP_BERT.hh"

#include "Randomize.hh" 
#include <sys/time.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
	struct timeval start,end;
    gettimeofday(&start,NULL);

    //Construct the default run manager
#ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    G4cout << "### MT MODE ON ###" << G4endl;
#else
    G4RunManager* runManager = new G4RunManager;
    G4cout << "### MT MODE OFF ###" << G4endl;
#endif

	//Activate UI-command base scorer
	G4ScoringManager * scManager = G4ScoringManager::GetScoringManager();
	scManager->SetVerboseLevel(0);
	
	//Choose the Random Engine 
	G4Random::setTheEngine(new CLHEP::RanecuEngine);
	time_t systime = time(NULL);
    G4int seed = (long)(systime*G4UniformRand());
    G4Random::setTheSeed(seed);
	  
	//Set mandatory initialization classes
	runManager->SetUserInitialization(new PhysicsList);
	runManager->SetUserInitialization(new DetectorConstruction);
	
#ifdef G4MULTITHREADED
    runManager->SetUserInitialization(new UserActionInitialization());
#else
  	PrimaryGeneratorAction* beam = new PrimaryGeneratorAction();
  	runManager->SetUserAction(beam);

	RunAction* runact = new RunAction();
	runManager->SetUserAction(runact);

	EventAction* evact = new EventAction();
	runManager->SetUserAction(evact);
#endif

	//Get the pointer to the User Interface manager
	G4UImanager* UImanager = G4UImanager::GetUIpointer();  

	if(argc!=1) {
		//Batch mode
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command+fileName);
	}

	else {
		//Define visualization and UI terminal for interactive mode
#ifdef G4VIS_USE
		G4VisManager* visManager = new G4VisExecutive;
		visManager->Initialize();
#endif
		
#ifdef G4UI_USE
		G4UIExecutive* ui = new G4UIExecutive(argc,argv);
		UImanager->ApplyCommand("/control/execute init_vis.mac");
		ui->SessionStart();
		delete ui;
#endif
		
#ifdef G4VIS_USE
		delete visManager;
#endif
	}

    //Job termination
    delete runManager;
    
    std::cout << std::endl;
    std::cout << ">> Simulation ended!" << std::endl;

    gettimeofday(&end,NULL);
    std::cout << "Elapsed time [s] = " << ((end.tv_sec - start.tv_sec)*1000000 + (end.tv_usec - start.tv_usec))/1.E6 << std::endl << std::endl;
    
    return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

