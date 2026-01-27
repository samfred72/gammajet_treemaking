#include "../headers/commonUtility.h"
#include "../headers/Style_jaebeom.h"
#include "../headers/ana.cxx"

void draw_deltaphi() {
  ana anaclone;
  gStyle->SetOptStat(0);
  TFile * f = TFile::Open("hists/hists.root");
 
  TH1D * h[4];
  TH1D * h2[4];
  for (int i = 0; i < 4; i++) {
    h[i] = (TH1D*)f->Get(Form("hdeltaphiprecut%i",i));
    h2[i] = (TH1D*)f->Get(Form("hdeltaphi%i",i));
  }

  TCanvas * c = new TCanvas("c","",700,700);
  TLegend * l = new TLegend(.30,.64,.55,.8);
  l->SetLineWidth(0);
  int colors[anaclone.nPtBins] = { kBlue, kBlack, kRed};
  gPad->SetTicks(1,1);
  for (int i = 0; i < anaclone.nPtBins; i++) {
    h[i]->Scale(1.0/h[i]->GetEntries());
    if (i == 0) h[i]->GetYaxis()->SetRangeUser(0,h[i]->GetMaximum()*1.5);
    h[i]->SetLineColor(colors[i]);
    h[i]->SetLineWidth(2);
    h[i]->Draw("hist same");
    l->AddEntry(h[i],Form("Cluster %.1f < pT < %.0f",anaclone.ptBins[i],anaclone.ptBins[i+1]));
  }
  l->Draw();
  drawText("#bf{#it{sPHENIX}} Internal",0.6,0.81,1,22);
  drawText("pp #sqrt{s}=200 Gev",0.6,0.76,1,22);
  drawText(Form("|vz| < %.0f cm",anaclone.vzcut),0.6,0.71,1,16);
  drawText(Form("E_{jet} > %.0f GeV",anaclone.minjete04),0.6,0.66,1,16);
  //drawText(Form("#Delta#phi > %.0f#pi/%.0f",anaclone.oppnum,anaclone.oppden),0.6,0.61,1,16);
  
  TCanvas * c2 = new TCanvas("c2","",700,700);
  TLegend * l2 = new TLegend(.30,.64,.55,.8);
  l2->SetLineWidth(0);
  gPad->SetTicks(1,1);
  for (int i = 0; i < anaclone.nPtBins; i++) {
    h2[i]->Scale(1.0/h2[i]->GetEntries());
    if (i == 0) h2[i]->GetYaxis()->SetRangeUser(0,h2[i]->GetMaximum()*2);
    h2[i]->SetLineColor(colors[i]);
    h2[i]->SetLineWidth(2);
    h2[i]->Draw("hist same");
    l2->AddEntry(h2[i],Form("Cluster %.1f < pT < %.0f",anaclone.ptBins[i],anaclone.ptBins[i+1]));
  }
  l2->Draw();
  drawText("#bf{#it{sPHENIX}} Internal",0.6,0.81,1,22);
  drawText("pp #sqrt{s}=200 Gev",0.6,0.76,1,22);
  drawText(Form("|vz| < %.0f cm",anaclone.vzcut),0.6,0.71,1,16);
  drawText(Form("E_{jet} > %.0f GeV",anaclone.minjete04),0.6,0.66,1,16);
  drawText(Form("#DeltaR > %.0f",anaclone.drcut),0.6,0.61,1,16);
  drawText("|#eta|_{jet} < 1.1 - R"       ,0.6,0.56,1,16);
  drawText("|#eta|_{cluster} < 1"         ,0.6,0.51,1,16);
  drawText("0 < #DeltaT_{mbd-cluster} < 4",0.6,0.46,1,16);
  drawText("E_{iso} < 2 GeV"              ,0.6,0.41,1,16);
  drawText("bdt score > 0.8"              ,0.6,0.36,1,16);
}
