#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "MagneticField.hh"
#include "G4FieldManager.hh"

/*****************************************************************/

DetectorConstruction::DetectorConstruction()
{
}

/*****************************************************************/

DetectorConstruction::~DetectorConstruction()
{
}

/*****************************************************************/

void
DetectorConstruction::SetNewValue(G4UIcommand *command, G4String value)
{
}
  
/*****************************************************************/

G4VPhysicalVolume *
DetectorConstruction::Construct() {

  /** materials **/
  G4NistManager *nist = G4NistManager::Instance();
  auto air = nist->FindOrBuildMaterial("G4_AIR");
  
  /** world **/
  auto world_s = new G4Box("world_s", 4. * m, 4. * m, 6. * m);
  auto world_lv = new G4LogicalVolume(world_s, air, "world_lv");
  auto world_pv = new G4PVPlacement(0,                // no rotation
				    G4ThreeVector(),  // at (0,0,0)
				    world_lv,         // its logical volume
				    "world_pv",       // its name
				    0,                // its mother  volume
				    false,            // no boolean operations
				    0,                // copy number
				    false);           // checking overlaps    

  /** magnetic **/
  auto magnetic_s = new G4Tubs("magnetic_s", 0., 4. * m, 6. * m, 0., 2. * M_PI);
  auto magnetic_lv = new G4LogicalVolume(magnetic_s, air, "magnetic_lv");
  auto magnetic_pv = new G4PVPlacement(nullptr,
				       G4ThreeVector(),
				       magnetic_lv,
				       "magnetic_pv",
				       world_lv,
				       false,
				       0,
				       false);

#if 0
  /** radiator **/
  auto radiator_s = new G4Cons("radiator_s",
			       0.1 * m, 1.0 * m,
			       0.1 * m, 2.0 * m,
			       0.75 * m,
			       0., 2. * M_PI);
  auto radiator_lv = new G4LogicalVolume(radiator_s, air, "radiator_lv");
  auto radiator_pv = new G4PVPlacement(nullptr,
				       G4ThreeVector(0., 0., (1.5 + 0.75) * m),
				       radiator_lv,
				       "radiator_pv",
				       magnetic_lv,
				       false,
				       0,
				       false);
#endif

  auto thetamin = std::atan2(0.1, 1.5);
  auto thetamax = std::atan2(2.0, 3.0);
  
  /** radiator **/
  auto radiator_s = new G4Sphere("radiator_s",
				 1.5 * m, 3.0 * m,
				 0., 2. * M_PI,
				 thetamin, thetamax - thetamin);
  auto radiator_lv = new G4LogicalVolume(radiator_s, air, "radiator_lv");
  auto radiator_pv = new G4PVPlacement(nullptr,
				       G4ThreeVector(),
				       radiator_lv,
				       "radiator_pv",
				       magnetic_lv,
				       false,
				       0,
				       false);
  
  mMagneticLogical = magnetic_lv;
  mRadiatorLogical = radiator_lv;
  return world_pv;

}

/*****************************************************************/

void
DetectorConstruction::ConstructSDandField()
{
  auto radiator_sd = new SensitiveDetector("radiator_sd");
  G4SDManager::GetSDMpointer()->AddNewDetector(radiator_sd);
  SetSensitiveDetector("radiator_lv", radiator_sd);

  auto magneticField = new MagneticField();
  auto fieldManager = new G4FieldManager();
  fieldManager->SetDetectorField(magneticField);
  fieldManager->CreateChordFinder(magneticField);

  auto magneticFieldZero = new MagneticFieldZero();
  auto fieldManagerZero = new G4FieldManager();
  fieldManagerZero->SetDetectorField(magneticFieldZero);
  fieldManagerZero->CreateChordFinder(magneticFieldZero);

  mMagneticLogical->SetFieldManager(fieldManagerZero, false);
  mRadiatorLogical->SetFieldManager(fieldManager, false);
}

/*****************************************************************/

