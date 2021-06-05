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
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4tgbVolumeMgr.hh"
#include "G4tgrMessenger.hh"
#include "G4tgbMaterialMgr.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

#include "g4dRIChOptics.hh"

/*****************************************************************/

DetectorConstruction::DetectorConstruction()
{
  mDetectorDirectory = new G4UIdirectory("/detector/");
  
  mDetectorSelectCmd = new G4UIcmdWithAString("/detector/select", this);
  mDetectorSelectCmd->SetGuidance("Select detector");
  mDetectorSelectCmd->SetParameterName("select", false);
  mDetectorSelectCmd->SetCandidates("ideal drich");
  mDetectorSelectCmd->AvailableForStates(G4State_PreInit, G4State_Idle);

  mMagneticDirectory = new G4UIdirectory("/magnetic/");
  
  mMagneticFieldCmd = new G4UIcmdWithAString("/magnetic/field", this);
  mMagneticFieldCmd->SetGuidance("Select field in magnetic volume");
  mMagneticFieldCmd->SetParameterName("field", false);
  mMagneticFieldCmd->SetCandidates("map mapv2 proj zero");
  mMagneticFieldCmd->AvailableForStates(G4State_PreInit);

  mRadiatorDirectory = new G4UIdirectory("/radiator/");
  
  mRadiatorFieldCmd = new G4UIcmdWithAString("/radiator/field", this);
  mRadiatorFieldCmd->SetGuidance("Select field in radiator volume");
  mRadiatorFieldCmd->SetParameterName("field", false);
  mRadiatorFieldCmd->SetCandidates("map mapv2 proj zero");
  mRadiatorFieldCmd->AvailableForStates(G4State_PreInit);

  mRadiatorChromaCmd = new G4UIcmdWithAString("/radiator/chroma", this);
  mRadiatorChromaCmd->SetGuidance("Select chromaticity in radiator volume");
  mRadiatorChromaCmd->SetParameterName("field", false);
  mRadiatorChromaCmd->SetCandidates("real zero");
  mRadiatorChromaCmd->AvailableForStates(G4State_PreInit);

}

/*****************************************************************/

DetectorConstruction::~DetectorConstruction()
{
}

/*****************************************************************/

void
DetectorConstruction::SetNewValue(G4UIcommand *command, G4String value)
{
  if (command == mDetectorSelectCmd) {
    if (value.compare("ideal") == 0) mDetectorSelect = kDetectorSelectIdealRICH;
    if (value.compare("drich") == 0) mDetectorSelect = kDetectorSelectDualRICH;
  }

  if (command == mMagneticFieldCmd) {
    if (value.compare("map") == 0) mMagneticField = kMagneticFieldMap;
    if (value.compare("mapv2") == 0) mMagneticField = kMagneticFieldMapv2;
    if (value.compare("proj") == 0) mMagneticField = kMagneticFieldProj;
    if (value.compare("zero") == 0) mMagneticField = kMagneticFieldZero;
  }

  if (command == mRadiatorFieldCmd) {
    if (value.compare("map") == 0) mRadiatorField = kRadiatorFieldMap;
    if (value.compare("mapv2") == 0) mRadiatorField = kRadiatorFieldMapv2;
    if (value.compare("proj") == 0) mRadiatorField = kRadiatorFieldProj;
    if (value.compare("zero") == 0) mRadiatorField = kRadiatorFieldZero;
  }

  if (command == mRadiatorChromaCmd) {
    if (value.compare("real") == 0) mRadiatorChroma = kRadiatorChromaReal;
    if (value.compare("zero") == 0) mRadiatorChroma = kRadiatorChromaZero;
  }
}
  
/*****************************************************************/

G4VPhysicalVolume *
DetectorConstruction::ConstructIdealRICH() {
  
  /** materials **/
  G4NistManager *nist = G4NistManager::Instance();
  auto air_m = nist->FindOrBuildMaterial("G4_AIR");
  auto aluminum_m = ConstructMaterialAluminum();
  auto silicon_m = ConstructMaterialSilicon();
  auto c2f6_m = ConstructMaterialC2F6();
  
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
  /** RICH geometry **/
  auto radiusmin = 1.5 * m;
  auto radiusmax = 3.0 * m;
  auto thetamin = std::atan2(0.1, 1.5);
  auto thetamax = std::atan2(2.0, 3.0);
  auto thetadelta = thetamax - thetamin;
  
  /** radiator **/
  auto radiator_s = new G4Sphere("radiator_s",
				 radiusmin, radiusmax,
				 0., 2. * M_PI,
				 thetamin, thetadelta);
  auto radiator_lv = new G4LogicalVolume(radiator_s, c2f6_m, "radiator_lv");
  auto radiator_pv = new G4PVPlacement(nullptr,
				       G4ThreeVector(),
				       radiator_lv,
				       "radiator_pv",
				       magnetic_lv,
				       false,
				       0,
				       false);
  
  /** mirror **/
  auto mirror_s = new G4Sphere("mirror_s",
			       radiusmax - 100. * um, radiusmax,
			       0., 2. * M_PI,
			       thetamin, thetadelta);
  auto mirror_lv = new G4LogicalVolume(mirror_s, aluminum_m, "mirror_lv");
  auto mirror_pv = new G4PVPlacement(nullptr,
				     G4ThreeVector(),
				     mirror_lv,
				     "mirror_pv",
				     radiator_lv,
				     false,
				     0,
				     false);
  
  /** mirror surface **/
  auto mirror_os = ConstructOpticalSurfaceMirror("mirror_os");
  auto mirror_ls = new G4LogicalBorderSurface("mirror_ls",
					      radiator_pv,
					      mirror_pv,
					      mirror_os);

  /** sensor **/
  auto sensor_s = new G4Sphere("sensor_s",
			       radiusmin, radiusmin + 100. * um,
			       0., 2. * M_PI,
			       thetamin, thetadelta);
  auto sensor_lv = new G4LogicalVolume(sensor_s, silicon_m, "sensor_lv");
  auto sensor_pv = new G4PVPlacement(nullptr,
				     G4ThreeVector(),
				     sensor_lv,
				     "sensor_pv",
				     radiator_lv,
				     false,
				     0,
				     false);

  /** sensor surface **/
  auto sensor_os = ConstructOpticalSurfaceSensor("sensor_os");
  auto sensor_ls = new G4LogicalBorderSurface("sensor_ls",
					      sensor_pv,
					      radiator_pv,
					      sensor_os);
  
  
  /** phantom trackers **/
  auto tracker0_s = new G4Sphere("tracker0_s",
				 radiusmin - 100. * um, radiusmin,
				 0., 2. * M_PI,
				 thetamin, thetadelta);
  auto tracker0_lv = new G4LogicalVolume(tracker0_s, air_m, "tracker0_lv");
  auto tracker0_pv = new G4PVPlacement(nullptr,
				       G4ThreeVector(),
				       tracker0_lv,
				       "tracker0_pv",
				       magnetic_lv,
				       false,
				       0,
				       false);
  
  auto tracker1_s = new G4Sphere("tracker1_s",
				 0.5 * (radiusmin + radiusmax) - 50. * um, 0.5 * (radiusmin + radiusmax) + 50. * um,
				 0., 2. * M_PI,
				 thetamin, thetadelta);
  auto tracker1_lv = new G4LogicalVolume(tracker1_s, c2f6_m, "tracker1_lv");
  auto tracker1_pv = new G4PVPlacement(nullptr,
				       G4ThreeVector(),
				       tracker1_lv,
				       "tracker1_pv",
				       radiator_lv,
				       false,
				       0,
				       false);
  
  auto tracker2_s = new G4Sphere("tracker2_s",
				 radiusmax, radiusmax + 100. * um,
				 0., 2. * M_PI,
				 thetamin, thetadelta);
  auto tracker2_lv = new G4LogicalVolume(tracker2_s, air_m, "tracker2_lv");
  auto tracker2_pv = new G4PVPlacement(nullptr,
				       G4ThreeVector(),
				       tracker2_lv,
				       "tracker2_pv",
				       magnetic_lv,
				       false,
				       0,
				       false);

  return world_pv;
}

/*****************************************************************/

G4VPhysicalVolume *
DetectorConstruction::ConstructDualRICH() {

  /** materials **/
  G4NistManager *nist = G4NistManager::Instance();
  auto air_m = nist->FindOrBuildMaterial("G4_AIR");
  auto aluminum_m = ConstructMaterialAluminum();
  auto silicon_m = ConstructMaterialSilicon();
  auto c2f6_m = ConstructMaterialC2F6();//true, 1.00082);
  auto vacuum_m = nist->FindOrBuildMaterial("G4_Galactic");
  
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
  auto move = G4ThreeVector(0., 0., 2.5 * m);
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

  /** phantom tracker **/
  auto ciDRICHgasMat_m = G4tgbMaterialMgr::GetInstance()->FindBuiltG4Material("ciDRICHgasMat");
  auto ciDRICHpetal_lv = G4LogicalVolumeStore::GetInstance()->GetVolume("ciDRICHpetal");
  auto tracker1_s = new G4Cons("tracker1_s",
			       9.5 * cm, 210.5 * cm,
			       9.5 * cm, 210.5 * cm,
			       50. * um,
			       -30 * deg, 60 * deg);
  auto tracker1_lv = new G4LogicalVolume(tracker1_s, ciDRICHgasMat_m, "tracker1_lv");
  auto tracker1_pv = new G4PVPlacement(nullptr,
				       G4ThreeVector(),
				       tracker1_lv,
				       "tracker1_pv",
				       ciDRICHpetal_lv,
				       false,
				       0,
				       false);

  
  return world_pv;
}

/*****************************************************************/

G4VPhysicalVolume *
DetectorConstruction::Construct() {
  if (mDetectorSelect == kDetectorSelectIdealRICH)
    return ConstructIdealRICH();
  else if (mDetectorSelect == kDetectorSelectDualRICH)
    return ConstructDualRICH();
  return nullptr;
}

/*****************************************************************/

void
DetectorConstruction::ConstructSDandFieldIdealRICH()
{
  auto sensor_sd = new SensitiveDetector("sensor_sd");
  G4SDManager::GetSDMpointer()->AddNewDetector(sensor_sd);
  SetSensitiveDetector("sensor_lv", sensor_sd);

  auto tracker_sd = new SensitiveDetectorTrack("tracker_sd");
  G4SDManager::GetSDMpointer()->AddNewDetector(tracker_sd);
  SetSensitiveDetector("tracker1_lv", tracker_sd);

  auto magneticField = new MagneticField();
  auto fieldManager = new G4FieldManager();
  fieldManager->SetDetectorField(magneticField);
  fieldManager->CreateChordFinder(magneticField);

  auto magneticFieldv2 = new MagneticFieldv2();
  auto fieldManagerv2 = new G4FieldManager();
  fieldManagerv2->SetDetectorField(magneticFieldv2);
  fieldManagerv2->CreateChordFinder(magneticFieldv2);

  auto magneticFieldZero = new MagneticFieldZero();
  auto fieldManagerZero = new G4FieldManager();
  fieldManagerZero->SetDetectorField(magneticFieldZero);
  fieldManagerZero->CreateChordFinder(magneticFieldZero);

  auto magneticFieldProj = new MagneticFieldProj();
  auto fieldManagerProj = new G4FieldManager();
  fieldManagerProj->SetDetectorField(magneticFieldProj);
  fieldManagerProj->CreateChordFinder(magneticFieldProj);

  auto magnetic_lv = G4LogicalVolumeStore::GetInstance()->GetVolume("magnetic_lv");
  if (mMagneticField == kMagneticFieldMap) 
    magnetic_lv->SetFieldManager(fieldManager, false);
  else if (mMagneticField == kMagneticFieldMapv2) 
    magnetic_lv->SetFieldManager(fieldManagerv2, false);
  else if (mMagneticField = kMagneticFieldProj)
    magnetic_lv->SetFieldManager(fieldManagerProj, false);
  else if (mMagneticField = kMagneticFieldZero)
    magnetic_lv->SetFieldManager(fieldManagerZero, false);
  
  auto radiator_lv = G4LogicalVolumeStore::GetInstance()->GetVolume("radiator_lv");
  if (mRadiatorField == kRadiatorFieldMap)
    radiator_lv->SetFieldManager(fieldManager, true);
  else if (mRadiatorField == kRadiatorFieldMapv2)
    radiator_lv->SetFieldManager(fieldManagerv2, true);
  else if (mRadiatorField == kRadiatorFieldProj)
    radiator_lv->SetFieldManager(fieldManagerProj, true);
  else if (mRadiatorField == kRadiatorFieldZero)
    radiator_lv->SetFieldManager(fieldManagerZero, true);

}

/*****************************************************************/

void
DetectorConstruction::ConstructSDandFieldDualRICH()
{
  auto ciDRICHpsst_sd = new SensitiveDetector("ciDRICHpsst_sd");
  G4SDManager::GetSDMpointer()->AddNewDetector(ciDRICHpsst_sd);
  SetSensitiveDetector("ciDRICHpsst", ciDRICHpsst_sd);

  auto tracker_sd = new SensitiveDetectorTrack("tracker_sd");
  G4SDManager::GetSDMpointer()->AddNewDetector(tracker_sd);
  SetSensitiveDetector("tracker1_lv", tracker_sd);

  auto magneticField = new MagneticField();
  auto fieldManager = new G4FieldManager();
  fieldManager->SetDetectorField(magneticField);
  fieldManager->CreateChordFinder(magneticField);

  auto magneticFieldZero = new MagneticFieldZero();
  auto fieldManagerZero = new G4FieldManager();
  fieldManagerZero->SetDetectorField(magneticFieldZero);
  fieldManagerZero->CreateChordFinder(magneticFieldZero);

  auto magneticFieldProj = new MagneticFieldProj();
  auto fieldManagerProj = new G4FieldManager();
  fieldManagerProj->SetDetectorField(magneticFieldProj);
  fieldManagerProj->CreateChordFinder(magneticFieldProj);

  auto magnetic_lv = G4LogicalVolumeStore::GetInstance()->GetVolume("magnetic_lv");
  if (mMagneticField == kMagneticFieldMap) 
    magnetic_lv->SetFieldManager(fieldManager, false);
  else if (mMagneticField = kMagneticFieldProj)
    magnetic_lv->SetFieldManager(fieldManagerProj, false);
  else if (mMagneticField = kMagneticFieldZero)
    magnetic_lv->SetFieldManager(fieldManagerZero, false);

  auto ciDRICHvessel_lv = G4LogicalVolumeStore::GetInstance()->GetVolume("ciDRICHvessel");
  if (mRadiatorField == kRadiatorFieldMap)
    ciDRICHvessel_lv->SetFieldManager(fieldManager, true);
  else if (mRadiatorField == kRadiatorFieldProj)
    ciDRICHvessel_lv->SetFieldManager(fieldManagerProj, true);
  else if (mRadiatorField == kRadiatorFieldZero)
    ciDRICHvessel_lv->SetFieldManager(fieldManagerZero, true);

}

/*****************************************************************/

void
DetectorConstruction::ConstructSDandField()
{
  if (mDetectorSelect == kDetectorSelectIdealRICH)
    return ConstructSDandFieldIdealRICH();
  else if (mDetectorSelect == kDetectorSelectDualRICH)
    return ConstructSDandFieldDualRICH();
}

/*****************************************************************/

G4Material *
DetectorConstruction::ConstructMaterialAluminum()
{

  /** element from nist **/
  G4NistManager *nist = G4NistManager::Instance();
  auto mat = nist->FindOrBuildMaterial("G4_Al");

  /** material properies table **/
  G4double energy[2] = { 1.2915 * eV, 5.166 * eV };
  G4double n[2] = { 1., 1. };
  
  G4MaterialPropertiesTable *mpt = new G4MaterialPropertiesTable(); 
  mpt->AddProperty("RINDEX", energy, n, 2);
  mat->SetMaterialPropertiesTable(mpt);

  return mat;
}

/*****************************************************************/

G4Material *
DetectorConstruction::ConstructMaterialSilicon()
{

  /** element from nist **/
  G4NistManager *nist = G4NistManager::Instance();
  auto mat = nist->FindOrBuildMaterial("G4_Si");

  /** material properies table **/
  G4double energy[2] = { 1.2915 * eV, 5.166 * eV };
  G4double n[2] = { 1., 1. };
  G4double abs[2] = {1. * um, 1. * um };
  
  G4MaterialPropertiesTable *mpt = new G4MaterialPropertiesTable(); 
  mpt->AddProperty("RINDEX", energy, n, 2);
  mpt->AddProperty("ABSLENGTH", energy, abs, 2);
  mat->SetMaterialPropertiesTable(mpt);

  return mat;
}

/*****************************************************************/

G4Material *
DetectorConstruction::ConstructMaterialC2F6(bool isConstN, double constN)
{

  if (mRadiatorChroma == kRadiatorChromaZero) {
    isConstN = true;
    constN = 1.00083;
  }

  /** elements and material **/
  G4Element *C = new G4Element("C2F6_Carbon", "C", 6, 12.011 * g/mole);
  G4Element *F = new G4Element("C2F6_Fluorine", "F", 9, 18.998 * g/mole);
  G4Material *mat = new G4Material("C2F6", 5.734 * mg/cm3, 2);
  mat->AddElement(C, 2);
  mat->AddElement(F, 6);

  /** material properties table **/
  G4double energy[10] = { 1.2915 * eV, 1.4089 * eV, 1.5498 * eV, 1.722 * eV, 1.9373 * eV,
			  2.214 * eV, 2.583 * eV, 3.0996 * eV, 3.8745 * eV, 5.166 * eV };
  
  G4double n[10] = { 1.00081514, 1.000815863, 1.000816816, 1.000818108, 1.00081992,
		     1.000822578, 1.000826707, 1.000833647, 1.000846733, 1.000876456 };
  
  G4double abs[10] = { 10. * m, 10. * m, 10. * m, 10. * m, 10. * m,
		       10. * m, 10. * m, 10. * m, 10. * m, 10. * m };
  
  if (isConstN) for (int i = 0; i < 10; ++i) n[i] = constN;
  
  G4MaterialPropertiesTable *mpt = new G4MaterialPropertiesTable(); 
  mpt->AddProperty("RINDEX", energy, n, 10)->SetSpline(true);
  mpt->AddProperty("ABSLENGTH", energy, abs, 10)->SetSpline(true);
  mat->SetMaterialPropertiesTable(mpt);
  
  return mat;
}

/*****************************************************************/

G4OpticalSurface *
DetectorConstruction::ConstructOpticalSurfaceMirror(G4String name)
{
  G4double energy[2] = { 1.2915 * eV, 5.166 * eV };
  G4double reflectivity[2] = {1., 1.};
  auto mpt = new G4MaterialPropertiesTable();
  mpt->AddProperty("REFLECTIVITY", energy, reflectivity, 2);

  auto os = new G4OpticalSurface(name);
  os->SetType(dielectric_metal);
  os->SetFinish(polished);
  os->SetModel(glisur);
  os->SetMaterialPropertiesTable(mpt);

  return os;
}

/*****************************************************************/

G4OpticalSurface *
DetectorConstruction::ConstructOpticalSurfaceSensor(G4String name)
{  
  G4double energy[2] = { 1.2915 * eV, 5.166 * eV };
  G4double reflectivity[2] = {0., 0.};
  G4double efficiency[2] = {1., 1.};
  auto mpt = new G4MaterialPropertiesTable();
  mpt->AddProperty("EFFICIENCY", energy, efficiency, 2);

  auto os = new G4OpticalSurface(name);
  os->SetType(dielectric_metal);
  os->SetFinish(polished);
  os->SetModel(glisur);
  os->SetMaterialPropertiesTable(mpt);

  return os;
}

/*****************************************************************/
