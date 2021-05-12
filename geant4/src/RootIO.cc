#include "RootIO.hh"
#include "G4Step.hh"
#include "G4Run.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4VTouchable.hh"
#include "G4VPhysicalVolume.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

/*****************************************************************/

RootIO *RootIO::mInstance = nullptr;

/*****************************************************************/

void
RootIO::InitMessenger()
{
  mDirectory = new G4UIdirectory("/io/");

  mFileNameCmd = new G4UIcmdWithAString("/io/prefix", this);
  mFileNameCmd->SetGuidance("Output file prefix.");
  mFileNameCmd->SetParameterName("prefix", false);
  mFileNameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

/*****************************************************************/
  
void
RootIO::SetNewValue(G4UIcommand *command, G4String value)
{
  if (command == mFileNameCmd)
    mFilePrefix = value;
}

/*****************************************************************/
  
void
RootIO::BeginOfRunAction(const G4Run *aRun)
{
  if (mInteractive) return;
  std::string filename = Form("%s.%03d.root", mFilePrefix.c_str(), aRun->GetRunID());
  Open(filename);
}

/*****************************************************************/

void
RootIO::EndOfRunAction(const G4Run *aRun)
{
  if (mInteractive) return;
  Close();
}

/*****************************************************************/

void
RootIO::BeginOfEventAction(const G4Event *aEvent)
{
  ResetHits();
  ResetTracks();
}

/*****************************************************************/

void
RootIO::EndOfEventAction(const G4Event *aEvent)
{
  if (mInteractive) return;
  FillHits();
  FillTracks();
}

/*****************************************************************/

void
RootIO::Open(std::string filename)
{
  mFile = TFile::Open(filename.c_str(), "RECREATE");
  
  mTreeHits = new TTree("Hits", "RootIO tree");
  mTreeHits->Branch("n"      , &mHits.n      , "n/I");
  mTreeHits->Branch("trkid"  , &mHits.trkid  , "trkid[n]/I");
  mTreeHits->Branch("t"      , &mHits.t      , "t[n]/F");
  mTreeHits->Branch("x"      , &mHits.x      , "x[n]/F");
  mTreeHits->Branch("y"      , &mHits.y      , "y[n]/F");
  mTreeHits->Branch("z"      , &mHits.z      , "z[n]/F");
  mTreeHits->Branch("e"      , &mHits.e      , "e[n]/F");
  mTreeHits->Branch("px"     , &mHits.px     , "px[n]/F");
  mTreeHits->Branch("py"     , &mHits.py     , "py[n]/F");
  mTreeHits->Branch("pz"     , &mHits.pz     , "pz[n]/F");

  mTreeTracks = new TTree("Tracks", "RootIO tree");
  mTreeTracks->Branch("n"        , &mTracks.n        , "n/I");
  mTreeTracks->Branch("parent"   , &mTracks.parent   , "parent[n]/I");
  mTreeTracks->Branch("pdg"      , &mTracks.pdg      , "pdg[n]/I");
  mTreeTracks->Branch("vt"       , &mTracks.vt       , "vt[n]/F");
  mTreeTracks->Branch("vx"       , &mTracks.vx       , "vx[n]/F");
  mTreeTracks->Branch("vy"       , &mTracks.vy       , "vy[n]/F");
  mTreeTracks->Branch("vz"       , &mTracks.vz       , "vz[n]/F");
  mTreeTracks->Branch("e"        , &mTracks.e        , "e[n]/F");
  mTreeTracks->Branch("px"       , &mTracks.px       , "px[n]/F");
  mTreeTracks->Branch("py"       , &mTracks.py       , "py[n]/F");
  mTreeTracks->Branch("pz"       , &mTracks.pz       , "pz[n]/F");

}

/*****************************************************************/

void
RootIO::Close()
{
  mFile->cd();
  mTreeHits->Write();
  mTreeTracks->Write();
  mFile->Close();
}

/*****************************************************************/

void
RootIO::ResetHits()
{
  mHits.n = 0;
}

/*****************************************************************/

void
RootIO::ResetTracks()
{
  mTracks.n = 0;
}

/*****************************************************************/

void
RootIO::FillHits()
{
  mTreeHits->Fill();
}

/*****************************************************************/

void
RootIO::FillTracks()
{
  mTreeTracks->Fill();
}

/*****************************************************************/

void
RootIO::AddHit(const G4Track *aTrack, const G4StepPoint *aPoint)
{
  mHits.trkid[mHits.n]  = aTrack->GetTrackID() - 1;
  mHits.t[mHits.n]      = aPoint->GetGlobalTime()    / ns;
  mHits.x[mHits.n]      = aPoint->GetPosition().x()  / cm;
  mHits.y[mHits.n]      = aPoint->GetPosition().y()  / cm;
  mHits.z[mHits.n]      = aPoint->GetPosition().z()  / cm;
  mHits.e[mHits.n]      = aPoint->GetTotalEnergy()   / GeV;
  mHits.px[mHits.n]     = aPoint->GetMomentum().x()  / GeV;
  mHits.py[mHits.n]     = aPoint->GetMomentum().y()  / GeV;
  mHits.pz[mHits.n]     = aPoint->GetMomentum().z()  / GeV;
  mHits.n++;
}

/*****************************************************************/

void
RootIO::AddTrack(const G4Track *aTrack)
{
  auto id = aTrack->GetTrackID() - 1;
  if (mTracks.n != id) {
    std::cout << "--- oh dear, this can lead to hard times later: " << mTracks.n << " " << aTrack->GetTrackID() << std::endl;
  }
  mTracks.parent[id]   = aTrack->GetParentID() - 1;
  mTracks.pdg[id]      = aTrack->GetParticleDefinition()->GetPDGEncoding();
  mTracks.vt[id]       = aTrack->GetGlobalTime()    / ns;
  mTracks.vx[id]       = aTrack->GetPosition().x()  / cm;
  mTracks.vy[id]       = aTrack->GetPosition().y()  / cm;
  mTracks.vz[id]       = aTrack->GetPosition().z()  / cm;
  mTracks.e[id]        = aTrack->GetTotalEnergy()   / GeV;
  mTracks.px[id]       = aTrack->GetMomentum().x()  / GeV;
  mTracks.py[id]       = aTrack->GetMomentum().y()  / GeV;
  mTracks.pz[id]       = aTrack->GetMomentum().z()  / GeV;
  mTracks.n++;
}

/*****************************************************************/

void
RootIO::ProcessHits(const G4Step *aStep)
{
  bool first = aStep->IsFirstStepInVolume();
  bool last = aStep->IsLastStepInVolume();
  auto track = aStep->GetTrack();

  if (first) {
    auto point = aStep->GetPreStepPoint();
    AddHit(track, point);
  }
  if (last) {
    auto point = aStep->GetPostStepPoint();
    AddHit(track, point);
  }
  
}

/*****************************************************************/
