set(SOURCES
  DetectorConstruction.cc
  MagneticField.cc
  SensitiveDetector.cc
  PrimaryGeneratorAction.cc
  StackingAction.cc
  EventAction.cc
  RunAction.cc
  RootIO.cc
  )

set(HEADERS
  DetectorConstruction.hh
  MagneticField.hh
  SensitiveDetector.hh
  PrimaryGeneratorAction.hh
  StackingAction.hh
  EventAction.hh
  RunAction.hh
  RootIO.hh
  g4dRIChOptics.hh
  )

add_executable(${PROJECT_NAME} main.cc ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${ROOT_LIBRARIES} ${Geant4_LIBRARIES})
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
