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
#include "G4tgbVolumeMgr.hh"
#include "G4tgrMessenger.hh"
#include "G4tgbMaterialMgr.hh"
#include "G4LogicalVolumeStore.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

#include "g4dRIChOptics.hh"

/*****************************************************************/

DetectorConstruction::DetectorConstruction()
{
  mRadiatorDirectory = new G4UIdirectory("/radiator/");
  
  mRadiatorFieldCmd = new G4UIcmdWithAString("/radiator/field", this);
  mRadiatorFieldCmd->SetGuidance("Select field in radiator volume");
  mRadiatorFieldCmd->SetParameterName("field", false);
  mRadiatorFieldCmd->SetCandidates("map zero");
  mRadiatorFieldCmd->AvailableForStates(G4State_PreInit);

}

/*****************************************************************/

DetectorConstruction::~DetectorConstruction()
{
}

/*****************************************************************/

void
DetectorConstruction::SetNewValue(G4UIcommand *command, G4String value)
{
  if (command == mRadiatorFieldCmd) {
    if (value.compare("map") == 0) mRadiatorField = kRadiatorFieldMap;
    if (value.compare("zero") == 0) mRadiatorField = kRadiatorFieldZero;
  }
}
  
/*****************************************************************/

G4VPhysicalVolume *
DetectorConstruction::Construct() {

  /** materials **/
  G4NistManager *nist = G4NistManager::Instance();
  auto vacuum_m = nist->FindOrBuildMaterial("G4_Galactic");
  auto air_m = nist->FindOrBuildMaterial("G4_AIR");
  
  
  /** world **/
  auto world_s = new G4Box("world_s", 4. * m, 4. * m, 6. * m);
  auto world_lv = new G4LogicalVolume(world_s, air_m, "world_lv");
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
  auto magnetic_lv = new G4LogicalVolume(magnetic_s, air_m, "magnetic_lv");
  auto magnetic_pv = new G4PVPlacement(nullptr,
				       G4ThreeVector(),
				       magnetic_lv,
				       "magnetic_pv",
				       world_lv,
				       false,
				       0,
				       false);
  
  /** text geometry **/
  auto volmgr = G4tgbVolumeMgr::GetInstance();
  volmgr->AddTextFile("drich-g4model.txt");
  auto text_pv = volmgr->ReadAndConstructDetector();
  auto tran = text_pv->GetTranslation();
  auto move = G4ThreeVector(0., 0., 2. * m);
  tran += move;
  text_pv->SetTranslation(tran);
  magnetic_lv->AddDaughter(text_pv);

  //  auto aeroPO = new g4dRIChAerogel("ciDRICHaerogelMat"); // (optical) model parameters
  //  aeroPO->setOpticalParams(3); // mode=3: use experimental data  

  //  auto acryPO = new g4dRIChFilter("ciDRICHfilterMat"); // (optical) model parameters
  //  acryPO->setOpticalParams(300. * nm);
  
  auto gasPO = new g4dRIChGas("ciDRICHgasMat");
  gasPO->setOpticalParams();
  
  auto photoSensor = new g4dRIChPhotosensor("ciDRICHpsst"); 
  photoSensor->setOpticalParams("ciDRICH");

  auto mirror = new g4dRIChMirror("ciDRICHmirror"); 
  mirror->setOpticalParams("ciDRICH");
  
  return world_pv;
}

/*****************************************************************/

void
DetectorConstruction::ConstructSDandField()
{
  auto ciDRICHvessel_sd = new SensitiveDetector("ciDRICHvessel_sd");
  G4SDManager::GetSDMpointer()->AddNewDetector(ciDRICHvessel_sd);
  SetSensitiveDetector("ciDRICHvessel", ciDRICHvessel_sd);

  auto magneticField = new MagneticField();
  auto fieldManager = new G4FieldManager();
  fieldManager->SetDetectorField(magneticField);
  fieldManager->CreateChordFinder(magneticField);

  auto magneticFieldZero = new MagneticFieldZero();
  auto fieldManagerZero = new G4FieldManager();
  fieldManagerZero->SetDetectorField(magneticFieldZero);
  fieldManagerZero->CreateChordFinder(magneticFieldZero);

  auto magnetic_lv = G4LogicalVolumeStore::GetInstance()->GetVolume("magnetic_lv");
  magnetic_lv->SetFieldManager(fieldManager, false);

  auto ciDRICHvessel_lv = G4LogicalVolumeStore::GetInstance()->GetVolume("ciDRICHvessel");
  if (mRadiatorField == kRadiatorFieldMap)
    ciDRICHvessel_lv->SetFieldManager(fieldManager, true);
  else if (mRadiatorField == kRadiatorFieldZero)
    ciDRICHvessel_lv->SetFieldManager(fieldManagerZero, true);

}

