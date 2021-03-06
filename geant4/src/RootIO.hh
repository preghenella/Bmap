/// @author: Roberto Preghenella
/// @email: preghenella@bo.infn.it

#ifndef _RootIO_h_
#define _RootIO_h_

#include "G4UImessenger.hh"

#include <string>

class TFile;
class TTree;
class G4Run;
class G4Event;
class G4Step;
class G4StepPoint;
class G4Track;

class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithAString;

class RootIO : public G4UImessenger
{
  
public:

  static RootIO *Instance() {
    if (!mInstance) mInstance = new RootIO();
    return mInstance;
  };

  void InitMessenger();
  void SetNewValue(G4UIcommand *command, G4String value);

  void BeginOfRunAction(const G4Run *aRun);
  void EndOfRunAction(const G4Run *aRun);
  void BeginOfEventAction(const G4Event *aEvent);
  void EndOfEventAction(const G4Event *aEvent);

  void setInteractiveMode(bool val) { mInteractive = val; };
  void Open(std::string filename);
  void Close();

  void ResetHits();
  void FillHits();
  void AddHit(const G4Track *aTrack, const G4StepPoint *aPoint);

  void ResetTracks();
  void FillTracks();
  void AddTrack(const G4Track *aTrack);

private:
  
  RootIO() = default;

  static RootIO *mInstance;
  std::string mFilePrefix = "Bmap";
  bool mInteractive = false;
  TFile *mFile = nullptr;

  G4UIdirectory *mDirectory;
  G4UIcmdWithAString *mFileNameCmd;
  
  TTree *mTreeHits = nullptr;
  static const int kMaxHits = 1048576;
  struct Hits_t {
    int    n;
    int    trkid[kMaxHits];
    float  t[kMaxHits];
    float  x[kMaxHits];
    float  y[kMaxHits];
    float  z[kMaxHits];
    float  e[kMaxHits];
    float  px[kMaxHits];
    float  py[kMaxHits];
    float  pz[kMaxHits];
    int    petal[kMaxHits];
  } mHits; //!

  TTree *mTreeTracks = nullptr;
  static const int kMaxTracks = 1048576;
  struct Tracks_t {
    int    n;
    int    parent[kMaxTracks];
    int    pdg[kMaxTracks];
    float  vt[kMaxTracks];
    float  vx[kMaxTracks];
    float  vy[kMaxTracks];
    float  vz[kMaxTracks];
    float  e[kMaxTracks];
    float  px[kMaxTracks];
    float  py[kMaxTracks];
    float  pz[kMaxTracks];
  } mTracks; //!

};

#endif /** _RootIO_h_ **/
