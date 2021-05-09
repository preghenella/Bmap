#ifndef _SensitiveDetector_h_
#define _SensitiveDetector_h_

#include "G4VSensitiveDetector.hh"

class SensitiveDetector : public G4VSensitiveDetector {
  
public:
  
  SensitiveDetector(const G4String &name);
  ~SensitiveDetector() override;

  void Initialize(G4HCofThisEvent *hce) override;
  void EndOfEvent(G4HCofThisEvent *hce) override;
  
protected:

  G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) override;
  
};

#endif /** _SensitiveDetector_h_ **/
