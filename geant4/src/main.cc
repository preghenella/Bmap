#include "G4RunManager.hh"
#include "G4RunManagerFactory.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "PrimaryGeneratorAction.hh"
#include "FTFP_BERT.hh"
#include "DetectorConstruction.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "StackingAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "RootIO.hh"

int
main(int argc, char **argv)
{
  auto ui = new G4UIExecutive(argc, argv, "tcsh");

  auto run = new G4RunManager(); //G4RunManagerFactory::CreateRunManager();
  auto physics = new FTFP_BERT;
  auto detector = new DetectorConstruction();
  run->SetUserInitialization(detector);
  run->SetUserInitialization(physics);
  
  //  run->Initialize();

  auto action_generator = new PrimaryGeneratorAction();
  auto stacking_action = new StackingAction();
  auto action_run = new RunAction();
  auto action_event = new EventAction();

  run->SetUserAction(action_generator);
  run->SetUserAction(stacking_action);
  run->SetUserAction(action_run);
  run->SetUserAction(action_event);
  
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // start interative session
  if (argc == 1) {
    RootIO::Instance()->setInteractiveMode(true);
    auto ui = new G4UIExecutive(argc, argv, "Qt");
    //    auto ui = new G4UIExecutive(argc, argv, "tcsh");
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
