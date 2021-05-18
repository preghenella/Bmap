#include "SensitiveDetector.hh"
#include "G4Step.hh"

#include "RootIO.hh"

/*****************************************************************/

SensitiveDetector::SensitiveDetector(const G4String &name) :
  G4VSensitiveDetector(name)
{
  collectionName.insert("collection");
}

/*****************************************************************/

SensitiveDetector::~SensitiveDetector()
{
}

/*****************************************************************/

void
SensitiveDetector::Initialize(G4HCofThisEvent *hce)
{
}

/*****************************************************************/

void
SensitiveDetector::EndOfEvent(G4HCofThisEvent *hce)
{
}

/*****************************************************************/

G4bool
SensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{  
  if (!aStep->IsFirstStepInVolume()) return true;
  auto track = aStep->GetTrack();
  auto point = aStep->GetPreStepPoint();
  RootIO::Instance()->AddHit(track, point);  
  return true;
}

/*****************************************************************/
