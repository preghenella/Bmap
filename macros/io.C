TTree *mTreeHits = nullptr;
static const int kMaxHits = 1048576;
struct Hits_t {
  int     n;
  int     trkid[kMaxHits];
  double  t[kMaxHits];
  double  x[kMaxHits];
  double  y[kMaxHits];
  double  z[kMaxHits];
  double  e[kMaxHits];
  double  px[kMaxHits];
  double  py[kMaxHits];
  double  pz[kMaxHits];
} mHits;

TTree *mTreeTracks = nullptr;
static const int kMaxTracks = 1048576;
struct Tracks_t {
  int     n;
  int     parent[kMaxTracks];
  int     pdg[kMaxTracks];
  double  vt[kMaxTracks];
  double  vx[kMaxTracks];
  double  vy[kMaxTracks];
  double  vz[kMaxTracks];
  double  e[kMaxTracks];
  double  px[kMaxTracks];
  double  py[kMaxTracks];
  double  pz[kMaxTracks];
} mTracks;

void
io(const char *fname)
{

  auto fin = TFile::Open(fname);

  // connect hits tree
  mTreeHits = (TTree *)fin->Get("Hits");
  mTreeHits->SetBranchAddress("n",     &mHits.n);
  mTreeHits->SetBranchAddress("trkid", &mHits.trkid);
  mTreeHits->SetBranchAddress("t",     &mHits.t);
  mTreeHits->SetBranchAddress("x",     &mHits.x);
  mTreeHits->SetBranchAddress("y",     &mHits.y);
  mTreeHits->SetBranchAddress("z",     &mHits.z);
  mTreeHits->SetBranchAddress("e",     &mHits.e);
  mTreeHits->SetBranchAddress("px",    &mHits.px);
  mTreeHits->SetBranchAddress("py",    &mHits.py);
  mTreeHits->SetBranchAddress("pz",    &mHits.pz);

  // connect tracks tree
  mTreeTracks = (TTree *)fin->Get("Tracks");
  mTreeTracks->SetBranchAddress("n",      &mTracks.n);
  mTreeTracks->SetBranchAddress("parent", &mTracks.parent);
  mTreeTracks->SetBranchAddress("pdg",    &mTracks.pdg);
  mTreeTracks->SetBranchAddress("vt",     &mTracks.vt);
  mTreeTracks->SetBranchAddress("vx",     &mTracks.vx);
  mTreeTracks->SetBranchAddress("vy",     &mTracks.vy);
  mTreeTracks->SetBranchAddress("vz",     &mTracks.vz);
  mTreeTracks->SetBranchAddress("e",      &mTracks.e);
  mTreeTracks->SetBranchAddress("px",     &mTracks.px);
  mTreeTracks->SetBranchAddress("py",     &mTracks.py);
  mTreeTracks->SetBranchAddress("pz",     &mTracks.pz);

}
