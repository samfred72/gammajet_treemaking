#include "../headers/commonUtility.h"
#include "../headers/Style_jaebeom.h"
#include "../headers/ana.cxx"

void draw_emfrac() {
  ana anaclone;
  gStyle->SetOptStat(0);
  TFile * f = TFile::Open("hists/hists.root");
  float jetbins[4] = {7,10,15,30};

  TH1D * hratio[4][3];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3 ; j++) {
      hratio[i][j] = (TH1D*)f->Get(Form("hratio_emfrac_%i_%i",j,i));
    }
  }
  TCanvas * c = new TCanvas("c","",700*4,700);
  c->Divide(4,1,0,0);
  TLegend * l[4];
  int colors[3] = {kSpring + 2, kBlue, kRed - 2};
  string text[3] = {"jet emfrac > 0.9","0.6 < jet emfrac < 0.9", "jet emfrac < 0.6"};
  for (int i = 0; i < 4; i++) {
    l[i] = new TLegend(.34,.64,.6,.8);
    c->cd(i+1);
    gPad->SetTicks(1,1);
    for (int j = 0; j < 3; j++) {
      hratio[i][j]->GetXaxis()->SetTitle("E_{T,max}^{Jet}/E_{T}^{cluster}");
      cout << hratio[i][j]->GetEntries() << endl;
      hratio[i][j]->Scale(1.0/hratio[i][j]->Integral());
      if (j == 0) hratio[i][j]->GetYaxis()->SetRangeUser(0,hratio[i][j]->GetMaximum()*2);
      hratio[i][j]->SetLineColor(colors[j]);
      hratio[i][j]->SetLineWidth(2);
      hratio[i][j]->Draw("hist same");
      l[i]->AddEntry(hratio[i][j],text[j].c_str());
    }
    l[i]->SetLineWidth(0);
    l[i]->Draw();
    drawText("#bf{#it{sPHENIX}} Internal",0.6,0.81,1,22);
    drawText("run 47289-53864",0.6,0.76,1,22);
    drawText("|vz| < 30 cm",0.6,0.71,1,16);
    drawText("|#eta| < 0.7",0.6,0.68,1,16);
    drawText("#DeltaR > 0.4",0.6,0.65,1,16);
    drawText("#Delta#phi > 7#pi/8",0.6,0.62,1,16);
    drawText(Form("E_{jet} > %.2f GeV",anaclone.minjete),0.6,0.59,1,16);
    drawText("E_{iso} < 2 GeV",0.6,0.53,1,16);
  }
}
