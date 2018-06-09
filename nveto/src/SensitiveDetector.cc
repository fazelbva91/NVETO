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
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#include "SensitiveDetector.hh"
#include "SensitiveDetectorHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4Navigator.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

SensitiveDetector::SensitiveDetector(G4String name):G4VSensitiveDetector(name)
{
    G4String HCname;
    collectionName.insert(HCname="collection");
    fHCID = -1;
    
    G4cout << "### SensitiveDetector instantiated ###" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

SensitiveDetector::~SensitiveDetector() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void SensitiveDetector::Initialize(G4HCofThisEvent* HCE)
{
    fHitsCollection = new SensitiveDetectorHitsCollection(SensitiveDetectorName, collectionName[0]);
    if(fHCID<0) {fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection);}
    HCE->AddHitsCollection(fHCID, fHitsCollection);    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
    G4Track* vTrack = aStep->GetTrack();
    
    G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
    
    G4TouchableHandle theTouchable = preStepPoint->GetTouchableHandle();
    
    G4ThreeVector worldPosition = preStepPoint->GetPosition();
    G4ThreeVector localPosition = theTouchable->GetHistory()->GetTopTransform().TransformPoint(worldPosition);
    
    G4ThreeVector mom = preStepPoint->GetMomentumDirection()*preStepPoint->GetKineticEnergy();
    
    G4int vType = -1;
    G4String partName = vTrack->GetDefinition()->GetParticleName();
    if (partName == "gamma") vType = 0;
    if (partName == "e-") vType = 1;
    if (partName == "proton") vType = 2;
   	
   	SensitiveDetectorHit* aHit = new SensitiveDetectorHit();

	aHit->SetType(vType);
    aHit->SetMom(mom);
    aHit->SetPos(localPosition);
	aHit->SetEnergy(aStep->GetTotalEnergyDeposit());
	aHit->SetTime(preStepPoint->GetGlobalTime());    
    aHit->SetTrackID(vTrack->GetTrackID());
    aHit->SetTrackIDP(vTrack->GetParentID());

    fHitsCollection->insert(aHit);
    
    return true;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void SensitiveDetector::EndOfEvent(G4HCofThisEvent*) {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

