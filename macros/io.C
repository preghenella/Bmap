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
  int     petal[kMaxHits];
} mHits;

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
  mTreeHits->SetBranchAddress("petal", &mHits.petal);

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
