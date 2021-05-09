/// @author: Roberto Preghenella
/// @email: preghenella@bo.infn.it

#ifndef _MagneticField_h_
#define _MagneticField_h_

#include "G4MagneticField.hh"

/*****************************************************************/

class TGraph2D;

class MagneticField : public G4MagneticField
{

public:

  MagneticField();
  ~MagneticField() override;

  void GetFieldValue(const G4double Point[4], G4double *Bfield) const override;

private:

  TGraph2D *mBr = nullptr;
  TGraph2D *mBz = nullptr;
  
};

/*****************************************************************/

class MagneticFieldZero : public G4MagneticField
{

public:

  MagneticFieldZero() = default;
  void GetFieldValue(const G4double Point[4], G4double *Bfield) const override {
    Bfield[0] = 0.; Bfield[1] = 0.; Bfield[2] = 0.;
  };
  
};

#endif /** _MagneticField_h_ **/
