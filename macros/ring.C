#include "io.C"
#include "invraytrac.C"

TH1 *
ring(const char *fname)
{
  io(fname);

  TVector3 E(0., 0., 0.);
  TVector3 P(0., 0., 0.);
  TVector3 C(0., 0., 0.);  
  double R = 300.;

  auto hHits = new TH2F("hHits", "", 3000, -150., 150., 3000, -150., 150.);
  auto hAngle = new TH1F("hAngle", ";#vartheta_{Ch} (rad);", 1000, 0., 0.1);
  auto hAngleP = new TH2F("hAngleP", ";p (GeV/c);#vartheta_{Ch} (rad)", 1000, 0., 100., 500, 0., 0.05);
  auto hAngleEta = new TH2F("hAngleEta", ";#eta;#vartheta_{Ch} (rad)", 400, 1., 4., 500, 0., 0.05); 
  auto hAnglePhi = new TH2F("hAnglePhi", "", 1000, -M_PI, M_PI, 1000, 0., 0.1);
 
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

    /** loop over photon hits **/
    for (int ihit = 0; ihit < mHits.n; ++ihit) {

      auto trkid = mHits.trkid[ihit];
      if (trkid == 0) continue;
      if (mTracks.parent[trkid] != 0) continue;
      
      hHits->Fill(mHits.x[ihit], mHits.y[ihit]);
      
      TVector3 D(mHits.x[ihit], mHits.y[ihit], mHits.z[ihit]);
      auto S = invraytrac(E, D, C, R);
      S = S - E;
      auto angle = P.Angle(S);

      hAngle->Fill(angle);
      hAngleP->Fill(P.Mag(), angle);
      hAngleEta->Fill(P.Eta(), angle);
      hAnglePhi->Fill(P.Phi(), angle);
	
    } /** end of loop over hits **/

  } /** end of loop over events **/

  //  hHits->Draw("colz");
  
  return hAngleEta;
}
