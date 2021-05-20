#ifndef _SensitiveDetector_h_
#define _SensitiveDetector_h_

#include "G4VSensitiveDetector.hh"

class SensitiveDetector : public G4VSensitiveDetector {
  
public:
  
  SensitiveDetector(const G4String &name) : G4VSensitiveDetector(name) {};
  ~SensitiveDetector() {};
  
protected:

  G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) override;
  
};

class SensitiveDetectorTrack : public G4VSensitiveDetector {
  
public:
  
  SensitiveDetectorTrack(const G4String &name) : G4VSensitiveDetector(name) {};
  ~SensitiveDetectorTrack() {};
  
protected:

  G4bool ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) override;
  
};

#endif /** _SensitiveDetector_h_ **/
