/// @author: Roberto Preghenella
/// @email: preghenella@bo.infn.it

#include "MagneticField.hh"
#include "G4SystemOfUnits.hh"
#include "TGraph2D.h"

/*****************************************************************/

MagneticField::MagneticField()
{

  // Br vs. (r,z)
  mBr = new TGraph2D("EIC_Magnetic_Field_Map_2021_04_30_cart_coords_[mm]_[T].Bmap", "%*lg %lg %lg %*lg %lg %*lg");
  for (int i = 0; i < mBr->GetN(); ++i) {
    mBr->GetX()[i] *= mm;
    mBr->GetY()[i] *= mm;
    mBr->GetZ()[i] *= tesla;
  }
  
  // Bz vs. (r,z)
  mBz = new TGraph2D("EIC_Magnetic_Field_Map_2021_04_30_cart_coords_[mm]_[T].Bmap", "%*lg %lg %lg %*lg %*lg %lg");
  for (int i = 0; i < mBz->GetN(); ++i) {
    mBz->GetX()[i] *= mm;
    mBz->GetY()[i] *= mm;
    mBz->GetZ()[i] *= tesla;
  }

}

/*****************************************************************/

MagneticField::~MagneticField()
{
}

/*****************************************************************/

void
MagneticField::GetFieldValue(const G4double Point[4], G4double *Bfield) const
{
  auto x = Point[0];
  auto y = Point[1];
  auto z = Point[2];
  auto r = std::hypot(x, y);
  auto phi = std::atan2(y, x);

  auto br = mBr->Interpolate(r, z);
  auto bx = br * std::cos(phi);
  auto by = br * std::sin(phi);
  auto bz = mBz->Interpolate(r, z);

  //  std::cout << "in " << x << " " << y << " " << z << ": bfield = " << bx << " " << by << " " << bz << std::endl;
  
  Bfield[0] = bx;
  Bfield[1] = by;
  Bfield[2] = bz;
  
}

/*****************************************************************/
