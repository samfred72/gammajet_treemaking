#include "../headers/commonUtility.h"
#include "../headers/Style_jaebeom.h"
#include "../headers/ana.cxx"

void draw_time() {
  ana anaclone;
  gStyle->SetOptStat(0);
  TFile * f = TFile::Open("hists/hists.root");
 
  TH1D * h[4];
  for (int i = 0; i < 4; i++) {
    h[i] = (TH1D*)f->Get(Form("hctminusjt%i",i));
  }

  TCanvas * c = new TCanvas("c","",700,700);
  TLegend * l = new TLegend(.34,.64,.6,.8);
  l->SetLineWidth(0);
  int colors[4] = { kBlue, kBlack, kRed, kGreen};
  string text[4] = {"Jet R=0.2", "Jet R=0.4", "Jet R=0.6", "Jet R=0.8"};
  gPad->SetTicks(1,1);
  for (int i = 0; i < 4; i++) {
    h[i]->Scale(1.0/h[i]->GetEntries());
    if (i == 0) h[i]->GetYaxis()->SetRangeUser(0,h[i]->GetMaximum()*1.5);
    h[i]->SetLineColor(colors[i]);
    h[i]->SetLineWidth(2);
    h[i]->Draw("hist same");
    l->AddEntry(h[i],text[i].c_str());
  }
  l->Draw();
  drawText("#bf{#it{sPHENIX}} Internal",0.6,0.81,1,22);
  drawText("pp #sqrt{s}=200 Gev",0.6,0.76,1,22);
  drawText(Form("|vz| < %.0f cm",anaclone.vzcut),0.6,0.71,1,16);
  drawText(Form("E_{jet} > %.0f GeV",anaclone.minjete04),0.6,0.66,1,16);
  drawText(Form("#Delta#phi > %.0f#pi/%.0f",anaclone.oppnum,anaclone.oppden),0.6,0.61,1,16);
}
