/// @author: Roberto Preghenella
/// @email: preghenella@bo.infn.it

#ifndef _DetectorConstruction_h_
#define _DetectorConstruction_h_

#include "G4VUserDetectorConstruction.hh"
#include "G4UImessenger.hh"

class G4String;
class G4LogicalVolume;

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
  
};

#endif /** _DetectorConstruction_h_ **/
