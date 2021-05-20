void swap(TGraph2D *g)
{
  double x, y, z;
  for (int i = 0; i < g->GetN(); ++i) {
    g->GetPoint(i, x, y, z);
    g->SetPoint(i, y, x, z);
  }
}

void
read_Bmap()
{

  auto gy = new TGraph2D("EIC_Magnetic_Field_Map_2021_04_30_cart_coords_[mm]_[T].Bmap", "%*lg %lg %lg %*lg %lg %*lg");
  gy->SetTitle("Br (T);z (mm);R (mm)");
  auto gz = new TGraph2D("EIC_Magnetic_Field_Map_2021_04_30_cart_coords_[mm]_[T].Bmap", "%*lg %lg %lg %*lg %*lg %lg");
  gz->SetTitle("Bz (T);z (mm);R (mm)");

  swap(gy);
  swap(gz);

  auto c = new TCanvas("c", "c", 800, 800);
  c->Divide(1, 2);
  c->cd(1);
  gz->SetNpx(500);
  gz->SetNpy(500);
  gz->Draw("colz");
  c->cd(2);
  gy->SetNpx(500);
  gy->SetNpy(500);
  gy->Draw("colz");
}

void
read_Bmap_radial_coords()
{

  auto gr = new TGraph2D("EIC_Magnetic_Field_Map_2021_05_07_radial_coords_[cm]_[T].120000.lines.Bmap", "%lg %lg %lg %*lg");
  gr->SetTitle("Br (T);z (mm);R (mm)");
  auto gz = new TGraph2D("EIC_Magnetic_Field_Map_2021_05_07_radial_coords_[cm]_[T].120000.lines.Bmap", "%lg %lg %*lg %lg");
  gz->SetTitle("Bz (T);z (mm);R (mm)");

  swap(gr);
  swap(gz);

  auto c = new TCanvas("c", "c", 800, 800);
  c->Divide(1, 2);
  c->cd(1);
  gz->SetNpx(500);
  gz->SetNpy(500);
  gz->Draw("colz");
  c->cd(2);
  gr->SetNpx(500);
  gr->SetNpy(500);
  gr->Draw("colz");
}
