/// @author: Roberto Preghenella
/// @email: preghenella@bo.infn.it

#ifndef _DetectorConstruction_h_
#define _DetectorConstruction_h_

#include "G4VUserDetectorConstruction.hh"
#include "G4UImessenger.hh"

class G4String;
class G4LogicalVolume;
class G4Material;
class G4OpticalSurface;

class G4UIdirectory;
class G4UIcmdWithAString;

class DetectorConstruction : public G4VUserDetectorConstruction,
			     public G4UImessenger
{
  
public:
  
  DetectorConstruction();
  ~DetectorConstruction() override;
  
  G4VPhysicalVolume *Construct() override;
  void ConstructSDandField() override;
  
protected:

  void SetNewValue(G4UIcommand *command, G4String value);

  G4LogicalVolume *mMagneticLogical = nullptr;
  G4LogicalVolume *mRadiatorLogical = nullptr;

  G4Material *ConstructMaterialAluminum();
  G4Material *ConstructMaterialSilicon();
  G4Material *ConstructMaterialC2F6(bool isConstN = false, double constN = 1.00082);

  G4OpticalSurface *ConstructOpticalSurfaceMirror(G4String name);
  G4OpticalSurface *ConstructOpticalSurfaceSensor(G4String name);
  
  G4UIdirectory *mRadiatorDirectory;
  G4UIcmdWithAString *mRadiatorFieldCmd;
  
  enum ERadiatorField_t {
    kRadiatorFieldMap,
    kRadiatorFieldZero
  };
  ERadiatorField_t mRadiatorField = kRadiatorFieldMap;
  
  
};

#endif /** _DetectorConstruction_h_ **/
