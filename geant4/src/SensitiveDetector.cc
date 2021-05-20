#include "SensitiveDetector.hh"
#include "G4Step.hh"
#include "G4Track.hh"

#include "RootIO.hh"

/*****************************************************************/

G4bool
SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{  
  if (!aStep->IsFirstStepInVolume()) return true;
  auto track = aStep->GetTrack();
  if (track->GetParentID() == 0) return true;
  auto point = aStep->GetPreStepPoint();
  RootIO::Instance()->AddHit(track, point);  
  return true;
}

/*****************************************************************/

G4bool
SensitiveDetectorTrack::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{  
  if (!aStep->IsFirstStepInVolume()) return true;
  auto track = aStep->GetTrack();
  if (track->GetParentID() != 0) return true;
  auto point = aStep->GetPreStepPoint();
  RootIO::Instance()->AddHit(track, point);  
  return true;
}

/*****************************************************************/
