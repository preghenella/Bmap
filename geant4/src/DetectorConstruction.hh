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

  G4VPhysicalVolume *ConstructIdealRICH();
  void ConstructSDandFieldIdealRICH();

  G4VPhysicalVolume *ConstructDualRICH();
  void ConstructSDandFieldDualRICH();
  
  G4Material *ConstructMaterialAluminum();
  G4Material *ConstructMaterialSilicon();
  G4Material *ConstructMaterialC2F6(bool isConstN = false, double constN = 1.00082);

  G4OpticalSurface *ConstructOpticalSurfaceMirror(G4String name);
  G4OpticalSurface *ConstructOpticalSurfaceSensor(G4String name);
  
  G4UIdirectory *mDetectorDirectory;
  G4UIcmdWithAString *mDetectorSelectCmd;
  enum EDetectorSelect_t {
    kDetectorSelectIdealRICH,
    kDetectorSelectDualRICH
  };
  EDetectorSelect_t mDetectorSelect = kDetectorSelectIdealRICH;
  
  G4UIdirectory *mMagneticDirectory;
  G4UIcmdWithAString *mMagneticFieldCmd;
  
  enum EMagneticField_t {
    kMagneticFieldMap,
    kMagneticFieldMapv2,
    kMagneticFieldProj,
    kMagneticFieldZero
  };
  EMagneticField_t mMagneticField = kMagneticFieldMap;
  
  G4UIdirectory *mRadiatorDirectory;
  G4UIcmdWithAString *mRadiatorFieldCmd;
  
  enum ERadiatorField_t {
    kRadiatorFieldMap,
    kRadiatorFieldMapv2,
    kRadiatorFieldProj,
    kRadiatorFieldZero
  };
  ERadiatorField_t mRadiatorField = kRadiatorFieldMap;
  
  G4UIcmdWithAString *mRadiatorChromaCmd;
  
  enum ERadiatorChroma_t {
    kRadiatorChromaReal,
    kRadiatorChromaZero
  };
  ERadiatorChroma_t mRadiatorChroma = kRadiatorChromaReal;
  
  
};

#endif /** _DetectorConstruction_h_ **/
