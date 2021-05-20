/// @author: Roberto Preghenella
/// @email: preghenella@bo.infn.it

#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "RootIO.hh"

/******************************************************************************/

void
SteppingAction::UserSteppingAction(const G4Step *aStep)
{

  auto track = aStep->GetTrack();

  /** only primary particles **/
  if (track->GetParentID() != 0) return;

  auto point = aStep->GetPreStepPoint();
  RootIO::Instance()->AddHit(track, point);  
  
}

/******************************************************************************/

