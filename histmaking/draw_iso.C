#include "../headers/commonUtility.h"
#include "../headers/Style_jaebeom.h"
#include "../headers/ana.cxx"

void draw_iso() {
  ana anaclone;
  gStyle->SetOptStat(0);
  TFile * f = TFile::Open("hists/hists.root");
  float jetbins[4] = {7,10,15,30};
  
  TH1D * hratio[3][4][2];
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4 ; j++) {
      for (int k = 0; k < 2; k++) {
        hratio[i][j][k] = (TH1D*)f->Get(Form("hratio_%i_%i_%i",i,j,k));
      }
    }
  }
  TCanvas * c = new TCanvas("c","",700*4,700*3);
  c->Divide(4,3,0,0);
  TLegend * l[12];
  int colors[2] = {kSpring + 2, kBlue};
  string text[2] = {"No iso cut","iso cut"};
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4; j++) {
      int index = i*4 + j;
      c->cd(index+1);
      gPad->SetTicks(1,1);
      l[index] = new TLegend(.34,.64,.6,.8);
      for (int k = 0; k < 2; k++) {
        hratio[i][j][k]->GetXaxis()->SetTitle("E_{T,max}^{Jet}/E_{T}^{cluster}");
        hratio[i][j][k]->Scale(1.0/hratio[i][j][k]->GetEntries());
        if (k == 0) hratio[i][j][k]->GetYaxis()->SetRangeUser(0,hratio[i][j][k]->GetMaximum()*2);
        hratio[i][j][k]->SetLineColor(colors[k]);
        hratio[i][j][k]->SetLineWidth(2);
        hratio[i][j][k]->Draw("hist same");
        l[index]->AddEntry(hratio[i][j][k],text[k].c_str());
      }
      l[index]->SetLineWidth(0);
      l[index]->Draw();
      drawText("#bf{#it{sPHENIX}} Internal",0.6,0.81,1,22);
      drawText("run 47289-53864",0.6,0.76,1,22);
      drawText("|vz| < 30 cm",0.6,0.71,1,16);
      drawText("|#eta| < 0.7",0.6,0.68,1,16);
      drawText("#DeltaR > 0.4",0.6,0.65,1,16);
      drawText("#Delta#phi > 7#pi/8",0.6,0.62,1,16);
      drawText(Form("E_{jet} > %.2f GeV",anaclone.minjete),0.6,0.59,1,16);
      float lowjet = anaclone.jetBins[i];
      float highjet = anaclone.jetBins[i+1];
      drawText(Form("%.1f GeV < E_{cluster} < %.1f GeV",lowjet,highjet),0.6,0.56,1,16);
      drawText("E_{iso} < 2 GeV",0.6,0.53,1,16);
    }
  }
}
