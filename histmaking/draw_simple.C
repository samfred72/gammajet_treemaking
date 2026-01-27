#include "../headers/commonUtility.h"
#include "../headers/Style_jaebeom.h"
#include "../headers/ana.cxx"

void scale(TH1D * h) {
  double x_low_value = 0.5;
  double x_high_value = 2;

  // Get the X-axis object
  TAxis *xaxis = h->GetXaxis();

  // Find the corresponding bin numbers
  Int_t bin_low = xaxis->FindBin(x_low_value);
  Int_t bin_high = xaxis->FindBin(x_high_value);

  // Sum the bin contents within the range
  double entries_in_range = 0;
  for (Int_t bin = bin_low; bin <= bin_high; ++bin) {
    entries_in_range += h->GetBinContent(bin);
  }
  h->Scale(1.0/entries_in_range);
}

void draw_simple() {
  int nrebin = 1;
  ana anaclone;
  gStyle->SetOptStat(0);
  TFile * f = TFile::Open("hists_simple/hists.root");
  TFile * f05_s = TFile::Open(Form("MChists_simple/hists%s_smear.root","Photon5"));
  TFile * f10_s = TFile::Open(Form("MChists_simple/hists%s_smear.root","Photon10"));
  TFile * f20_s = TFile::Open(Form("MChists_simple/hists%s_smear.root","Photon20"));
  TFile * f05_u = TFile::Open(Form("MChists_simple/hists%s_unsmear.root","Photon5"));
  TFile * f10_u = TFile::Open(Form("MChists_simple/hists%s_unsmear.root","Photon10"));
  TFile * f20_u = TFile::Open(Form("MChists_simple/hists%s_unsmear.root","Photon20"));


  TH1D * hratio;
  TH1D * hratiomc_s;
  TH1D * hratiomc_u;
  // data
  hratio = (TH1D*)f->Get(Form("hratio"));

  // smeared MC
  TH1D * h05_s = (TH1D*)f05_s->Get(Form("hratio"));
  h05_s->SetName(Form("hratio_05_s"));
  TH1D * h10_s = (TH1D*)f10_s->Get(Form("hratio"));
  h10_s->SetName(Form("hratio_10_s"));
  TH1D * h20_s = (TH1D*)f20_s->Get(Form("hratio"));
  h20_s->SetName(Form("hratio_20_s"));
  h05_s->Scale(146359.3);
  h10_s->Scale(6944.675);
  h20_s->Scale(130.4461);
  hratiomc_s = h05_s;
  hratiomc_s->Add(h10_s);
  hratiomc_s->Add(h20_s);
  hratiomc_s->Rebin(nrebin);

  // unsmeared MC
  TH1D * h05_u = (TH1D*)f05_u->Get(Form("hratio"));
  h05_u->SetName(Form("hratio_05_u"));
  TH1D * h10_u = (TH1D*)f10_u->Get(Form("hratio"));
  h10_u->SetName(Form("hratio_10_u"));
  TH1D * h20_u = (TH1D*)f20_u->Get(Form("hratio"));
  h20_u->SetName(Form("hratio_20_u"));
  h05_u->Scale(146359.3);
  h10_u->Scale(6944.675);
  h20_u->Scale(130.4461);
  hratiomc_u = h05_u;
  hratiomc_u->Add(h10_u);
  hratiomc_u->Add(h20_u);
  hratiomc_u->Rebin(nrebin);

  int csize =  700;
  int colors[6] = {kSpring + 2, kBlue, kGreen + 3, kBlue + 4, kTeal, kMagenta+1};
  string text[2] = {"No iso cut",""};
  float minjets[4] = {anaclone.minjete02,anaclone.minjete04,anaclone.minjete06,anaclone.minjete08};
  float etas[4] = {.9,.7,.5,.4};
  TCanvas * c2 = new TCanvas("c2","",csize,csize);
  TLegend * l2[1];
  int index = 0;
  gPad->SetTicks(1,1);
  l2[index] = new TLegend(.14,.8,.6,.9);
  hratio->GetXaxis()->SetTitle("E_{T,max}^{Jet}/E_{T,max}^{cluster}");
  scale(hratio);// ->Scale(1.0/hratio->GetEntries());
  hratio->GetYaxis()->SetRangeUser(0.00001,hratio->GetMaximum()*2);
  hratio->SetLineColor(colors[1]);
  hratio->SetLineWidth(2);
  hratio->Draw("hist same");

  scale(hratiomc_s);//->Scale(1.0/hratiomc_s->Integral()/(float)nrebin);//,"width");
  hratiomc_s->SetLineColor(colors[1+2]);
  hratiomc_s->Draw("hist same");

  scale(hratiomc_u);//->Scale(1.0/hratiomc_u->Integral()/(float)nrebin);//,"width");
  hratiomc_u->SetLineColor(colors[1+4]);
  hratiomc_u->Draw("hist same");

  l2[index]->AddEntry(hratio,(text[1]+" data").c_str());
  l2[index]->AddEntry(hratiomc_u,(text[1]+" MC reco unsmeared").c_str());
  l2[index]->AddEntry(hratiomc_s,(text[1]+" MC reco smeared").c_str());

  l2[index]->SetLineWidth(0);
  l2[index]->Draw();
  TLine * line = new TLine(3.0/10.0,0,3.0/10.0,1);
  line->SetLineStyle(8);
  line->Draw();
  drawText("#bf{#it{sPHENIX}} Internal",0.6,0.86,1,22);
  drawText("run 47289-53864",0.6,0.81,1,22);
  drawText(Form("MC run28"),0.6,0.76,1,22);
  drawText("|vz| < 30 cm",0.6,0.71,1,16);
  drawText(Form("|#eta|_{jet} < %.1f, |#eta|_{cluster} < 1",.7),0.6,0.68,1,16);
  drawText("#Delta#phi > 3#pi/4",0.6,0.65,1,16);
  drawText(Form("E_{jet} > %.2f GeV",3.0),0.6,0.62,1,16);
  float lowjet = 10;
  float highjet = 35;
  drawText(Form("%.1f GeV < E_{cluster} < %.1f GeV",lowjet,highjet),0.6,0.59,1,16);
  drawText("E_{iso} < 2 GeV",0.6,0.56,1,16);
  //drawText("0 < #DeltaT_{cluster,mbd} < 4",0.6,0.53,1,16);
  c2->SaveAs("iso.pdf");
}
