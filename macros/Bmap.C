static const int kMaxHits = 1024;
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
} mHits;

static const int kMaxTracks = 1024;
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
} mTracks;

TProfile *
Bmap(const char *fname)
{

  auto fin = TFile::Open(fname);

  // connect hits tree
  auto thits = (TTree *)fin->Get("Hits");
  thits->SetBranchAddress("n",     &mHits.n);
  thits->SetBranchAddress("trkid", &mHits.trkid);
  thits->SetBranchAddress("t",     &mHits.t);
  thits->SetBranchAddress("x",     &mHits.x);
  thits->SetBranchAddress("y",     &mHits.y);
  thits->SetBranchAddress("z",     &mHits.z);
  thits->SetBranchAddress("e",     &mHits.e);
  thits->SetBranchAddress("px",    &mHits.px);
  thits->SetBranchAddress("py",    &mHits.py);
  thits->SetBranchAddress("pz",    &mHits.pz);

  // connect tracks tree
  auto ttracks = (TTree *)fin->Get("Tracks");
  ttracks->SetBranchAddress("n",      &mTracks.n);
  ttracks->SetBranchAddress("parent", &mTracks.parent);
  ttracks->SetBranchAddress("pdg",    &mTracks.pdg);
  ttracks->SetBranchAddress("vt",     &mTracks.vt);
  ttracks->SetBranchAddress("vx",     &mTracks.vx);
  ttracks->SetBranchAddress("vy",     &mTracks.vy);
  ttracks->SetBranchAddress("vz",     &mTracks.vz);
  ttracks->SetBranchAddress("e",      &mTracks.e);
  ttracks->SetBranchAddress("px",     &mTracks.px);
  ttracks->SetBranchAddress("py",     &mTracks.py);
  ttracks->SetBranchAddress("pz",     &mTracks.pz);

  auto hAngleEta = new TProfile("hAngleEta", ";#eta;#Delta#vartheta", 100, 0., 5.);
  
  // loop over entries
  TVector3 vin, vout;
  for (int iev = 0; iev < thits->GetEntries(); ++iev) {
    thits->GetEntry(iev);
    ttracks->GetEntry(iev);

    // must have two hits
    if (mHits.n != 2) continue;
    // from the same track
    if (mHits.trkid[0] != mHits.trkid[1]) continue;

    // get track info
    auto trkid = mHits.trkid[0];
    auto p = std::sqrt(mTracks.px[trkid] * mTracks.px[trkid] +
		       mTracks.py[trkid] * mTracks.py[trkid] +
		       mTracks.pz[trkid] * mTracks.pz[trkid]);
    auto eta = std::atanh(mTracks.pz[trkid] / p);

    // compute deviation angle
    vin.SetXYZ(mHits.px[0], mHits.py[0], mHits.pz[0]);
    vout.SetXYZ(mHits.px[1], mHits.py[1], mHits.pz[1]);
    auto angle = vin.Angle(vout);
    hAngleEta->Fill(eta, angle);
    
  }

  return hAngleEta;
  
}
