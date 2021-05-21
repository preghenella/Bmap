#include "io.C"
#include "invraytrac.C"

TH1 *
ideal_ring(const char *fname, const char *fnameout)
{
  io(fname);

  TVector3 E(0., 0., 0.);
  TVector3 P(0., 0., 0.);
  TVector3 C(0., 0., 0.);  
  TVector3 T(0., 0., 0.);  
  double R = 300.;

  auto hRMS = new TProfile2D("hRMS", ";#eta;p (GeV/c)", 51, -0.05, 5.05, 1001, -0.05, 100.05, "S");
  auto hAngle = new TH1F("hAngle", ";#vartheta_{Ch} (rad);", 1000, 0., 0.1);
  
  /** loop over events **/
  auto nev = mTreeHits->GetEntries();
  for (int iev = 0; iev < nev; ++iev) {
    mTreeHits->GetEntry(iev);
    mTreeTracks->GetEntry(iev);

    /** loop over hits to find tracking handle 
	set emission point and direction **/
    for (int ihit = 0; ihit < mHits.n; ++ihit) {
      if (mHits.trkid[ihit] != 0) continue;
      E.SetXYZ(mHits.x[ihit], mHits.y[ihit], mHits.z[ihit]);
      P.SetXYZ(mHits.px[ihit], mHits.py[ihit], mHits.pz[ihit]);
      break;
    }

    T.SetXYZ(mTracks.px[0], mTracks.py[0], mTracks.pz[0]);
    
    /** loop over photon hits **/
    for (int ihit = 0; ihit < mHits.n; ++ihit) {

      auto trkid = mHits.trkid[ihit];
      if (trkid == 0) continue;
      if (mTracks.parent[trkid] != 0) continue;

      TVector3 D(mHits.x[ihit], mHits.y[ihit], mHits.z[ihit]);
      auto S = invraytrac(E, D, C, R);
      S = S - E;
      auto angle = P.Angle(S);
      
      hAngle->Fill(angle);
      hRMS->Fill(T.Eta(), T.Mag(), angle);
      
    } /** end of loop over hits **/

  } /** end of loop over events **/

  auto g = new TGraph2D;
  auto npt = 0;
  for (int i = 0; i < hRMS->GetNbinsX(); ++i) {
    for (int j = 0; j < hRMS->GetNbinsY(); ++j) {
      if (hRMS->GetBinError(i + 1, j + 1) <= 0.) continue;
      auto eta = hRMS->GetXaxis()->GetBinCenter(i + 1);
      auto p = hRMS->GetYaxis()->GetBinCenter(j + 1);
      auto val = hRMS->GetBinError(i + 1, j + 1);
      g->SetPoint(npt, eta, p, val);
      npt++;
    }
  }
  
  auto fout = TFile::Open(fnameout, "RECREATE");
  g->Write("gRMS");
  fout->Close();
  
  return hAngle;
}
