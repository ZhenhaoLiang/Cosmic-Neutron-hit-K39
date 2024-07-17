/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include <G4Tubs.hh>
#include <G4RotationMatrix.hh>
#include <G4ThreeVector.hh>
#include <G4UnionSolid.hh>
#include <G4SubtractionSolid.hh>
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include <G4VisAttributes.hh>

#define pi 3.14159265359

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction()
{
  DefineMaterial();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::DefineMaterial()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  Air = nist->FindOrBuildMaterial("G4_AIR");
  Water = nist->FindOrBuildMaterial("G4_WATER");
  //material ============================================
  //G4Element *Al = new G4Element("Aluminium", "Al", 13., 26.982 * g / mole);
  //G4Element *H = new G4Element("Hydrogen", "H", 1., 1.0079 * g / mole);
  G4Element *C = new G4Element("Carbon", "C", 6., 12.011 * g / mole);
  G4Element *K = nist->FindOrBuildElement("K");
  G4Element *O = new G4Element("Oxygen", "O", 8., 15.999 * g / mole);
  //==== K2CO3 ====

  K2CO3 = new G4Material("K2CO3", 2.428 * g / cm3, 3, kStateLiquid);
  K2CO3->AddElement(C, 1);
  K2CO3->AddElement(K, 2);
  K2CO3->AddElement(O, 3);

  G4cout << "K2CO3 : density " <<  K2CO3->GetDensity()/(g / cm3) << " , "
         << "NbOfKAtomsPerVolume " << K2CO3->GetTotNbOfAtomsPerVolume()/(1. / cm3) /3.0<< G4endl;
}
G4VPhysicalVolume* DetectorConstruction::Construct()
{  
  
  // K box
  //
  G4double bok_sizeXY = 0.1*m;
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;
 
  // World
  G4double world_sizeXYZ = 1.2*bok_sizeXY;
  
  G4Box* solidWorld =    
    new G4Box("World",                       //its name
       0.5*world_sizeXYZ, 0.5*world_sizeXYZ, 0.5*world_sizeXYZ);     //its size
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        Air,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking   
  // solidKBox
  // 
   
  G4Box* solidKBox =    
    new G4Box("solidKBox",                    //its name
        0.5*bok_sizeXY, 0.5*bok_sizeXY, 0.5*0.85*cm); //its size
      
  G4LogicalVolume* logicKBox =                         
    new G4LogicalVolume(solidKBox,            //its solid
                        Water,             //its material
                        "solidKBox");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0,0),         //at (0,0,0)
                    logicKBox,                //its logical volume
                    "solidKBox",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking  
                    /*
  //Source
  G4Box* SourceBox =    
    new G4Box("SourceBox",                    //its name
        0.5*bok_sizeXY, 0.5*0.1*cm, 0.5*bok_sizeXY); //its size
      
  G4LogicalVolume* logicSourceBox =                         
    new G4LogicalVolume(SourceBox,            //its solid
                        Air,             //its material
                        "SourceBox");         //its name
               
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0.5*bok_sizeXY + 0.05*cm,0),         //at (0,0,0)
                    logicSourceBox,                //its logical volume
                    "SourceBox",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking          */  
  //Detector===============================================================  
  G4Box* solidDetector =    
    new G4Box("solidDetector",                    //its name
        0.5*bok_sizeXY, 0.5*cm, 0.5*bok_sizeXY); //its size
  G4LogicalVolume* logicDetector =                         
    new G4LogicalVolume(solidDetector,         //its solid
                        Air,          //its material
                        "Detector");           //its name
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(0,0.5*bok_sizeXY + 0.25*cm,0),   //at position
                    logicDetector,             //its logical volume
                    "Detector",                //its name
                    logicWorld,                //its mother volume  is contanier
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
