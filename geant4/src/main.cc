#include "G4RunManager.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4GenericPhysicsList.hh"
#include "FTFP_BERT.hh"
#include "G4OpticalPhysics.hh"
#include "DetectorConstruction.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "PrimaryGeneratorAction.hh"
#include "StackingAction.hh"
#include "SteppingAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "RootIO.hh"

int
main(int argc, char **argv)
{
  auto ui = new G4UIExecutive(argc, argv, "tcsh");

  auto run = new G4RunManager;
  auto physics = new FTFP_BERT;
  physics->RegisterPhysics(new G4OpticalPhysics());
  auto detector = new DetectorConstruction();
  run->SetUserInitialization(detector);
  run->SetUserInitialization(physics);
  
  //  run->Initialize();

  auto generator_action = new PrimaryGeneratorAction();
  auto stacking_action = new StackingAction();
  auto stepping_action = new SteppingAction();
  auto run_action = new RunAction();
  auto event_action = new EventAction();

  run->SetUserAction(generator_action);
  run->SetUserAction(stacking_action);
  //  run->SetUserAction(stepping_action);
  run->SetUserAction(run_action);
  run->SetUserAction(event_action);
  
  // initialize RootIO messenger
  RootIO::Instance()->InitMessenger();

  // start interative session
  if (argc == 1) {
    RootIO::Instance()->setInteractiveMode(true);
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    auto ui = new G4UIExecutive(argc, argv, "Qt");
    ui->SessionStart();
    delete ui;  
    return 0;
  }

  auto uiManager = G4UImanager::GetUIpointer();
  std::string command = "/control/execute ";
  std::string fileName = argv[1];
  uiManager->ApplyCommand(command + fileName);
  
  return 0;
}
