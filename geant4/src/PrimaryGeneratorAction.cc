/// @author: Roberto Preghenella
/// @email: preghenella@bo.infn.it

#include "PrimaryGeneratorAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4ParticleGun.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Event.hh"

/*****************************************************************/

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
  mGeneratorDirectory = new G4UIdirectory("/generator/");
  
  mGeneratorSelectCmd = new G4UIcmdWithAString("/generator/select", this);
  mGeneratorSelectCmd->SetGuidance("Select event generator");
  mGeneratorSelectCmd->SetParameterName("select", false);
  mGeneratorSelectCmd->SetCandidates("gun gps");
  mGeneratorSelectCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

/*****************************************************************/

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete mParticleSource;
}

/*****************************************************************/

void
PrimaryGeneratorAction::SetNewValue(G4UIcommand *command, G4String value)
{
  if (command == mGeneratorSelectCmd) {
    if (value.compare("gun") == 0) mParticleSource = new G4ParticleGun();
    if (value.compare("gps") == 0) mParticleSource = new G4GeneralParticleSource();
  }
}
  
/*****************************************************************/

void
PrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
  mParticleSource->GeneratePrimaryVertex(event);
}

/*****************************************************************/

