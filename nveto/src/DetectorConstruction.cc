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
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"

#include "G4RunManager.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

DetectorConstruction::~DetectorConstruction(){}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....

void DetectorConstruction::DefineMaterials(){
    
    //** TODO: Insert the material definition here **//

    //***********//
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct(){
    
    //DefineMaterials();
    
    //** World **//
    G4Material* Air = G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
    
    G4double worldSize = 10. * CLHEP::meter;
    
    G4Box* worldSolid = new G4Box("World",
                                  worldSize/2.,
                                  worldSize/2.,
                                  worldSize/2.);
    
    G4LogicalVolume* worldLogic = new G4LogicalVolume(worldSolid,
                                                      Air,
                                                      "World");
    
    G4PVPlacement* worldPhysical = new G4PVPlacement(0,
                                                     G4ThreeVector(),
                                                     worldLogic,
                                                     "World",
                                                     0,
                                                     false,
                                                     0);
    //***********//
    G4Element* elC = G4NistManager::Instance()->FindOrBuildElement(6);
    G4Element* elH = G4NistManager::Instance()->FindOrBuildElement(1);
    
    G4int nC = 10; // between 10 and 16
    G4int ncomponents = 2;
    G4int natoms = 1;
    G4double density = 0.863 * g/cm3;
    G4Material* materialLAB = new G4Material("LAB", density, ncomponents=2);
    materialLAB->AddElement(elH, natoms=6+nC*2);
    materialLAB->AddElement(elC, natoms=6+nC);
    
    G4double nVetoDiameter =  10. * CLHEP::cm;
    G4double nVetoHeight   =  10. * CLHEP::cm;
    
    G4Tubs* nVetoSolid = new G4Tubs("nVetoSolid",
                                    0.,
                                    nVetoDiameter * 0.5,
                                    nVetoHeight   * 0.5,
                                    0.,
                                    360.*CLHEP::deg);
    
    nVetoLogical = new G4LogicalVolume(nVetoSolid,
                                       materialLAB,
                                       "nVetoLogical");
    
    
    new G4PVPlacement(0,
                      G4ThreeVector(0., 0., 0.),
                      nVetoLogical,
                      "nVetoPhysical",
                      worldLogic,
                      false,
                      0);
    
    //***********//

    return worldPhysical;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField(){
    
    //Sensitive Volume
	G4VSensitiveDetector* vDetector = new SensitiveDetector("det");
    G4SDManager::GetSDMpointer()->AddNewDetector(vDetector); 
	fSensitiveVolume = nVetoLogical;    			   
    fSensitiveVolume->SetSensitiveDetector(vDetector);
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo..
