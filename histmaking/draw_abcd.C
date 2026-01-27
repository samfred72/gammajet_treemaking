#include "../headers/commonUtility.h"
#include "../headers/Style_jaebeom.h"
#include "../headers/ana.cxx"
ana anaclone;

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

void draw_all(TCanvas * c, vector<vector<vector<TH1D*>>> hratio, vector<vector<vector<TH1D*>>> hratiomc_p, vector<vector<vector<TH1D*>>> hratiomc_j, int ia) {
  float drawx = 0.80;
  float drawy = 0.92;
  float fontsize = 60;
  int njrebin = 1;
  int nprebin = 1;
  int offset = 0;
  int colors[6] = {kSpring + 2, kBlue, kGreen + 3, kMagenta+1, kTeal, kSpring+2};
  float minjets[4] = {anaclone.minjete02,anaclone.minjete04,anaclone.minjete06,anaclone.minjete08};
  vector<string> t1 = {"#bf{Analysis region A}:","#bf{Analysis region B}:","#bf{Analysis region C}:","#bf{Analysis region D}:", "Combined ABCD"};
  vector<string> t2 = {"E_{iso} < 2 GeV","E_{iso} > 2 GeV","E_{iso} < 2 GeV","E_{iso} > 2 GeV",""};
  vector<string> t3 = {"bdt score > 0.8","bdt score > 0.8","No bdt cut", "No bdt cut",""};

  TPad * p = new TPad("p","",0,0,1,1);
  p->SetLeftMargin(0.25);
  p->SetBottomMargin(0.25);
  p->Divide(anaclone.nJetR+1,anaclone.nPtBins,0,0);
  p->Draw();

  for (int i = 0; i < anaclone.nPtBins; i++) {
    for (int j = 0; j < anaclone.nJetR; j++) {
      int index = i*anaclone.nJetR + j + 1 + offset;
      if (index % (anaclone.nJetR+1) == 0) {index++; offset++;}
      p->cd(index);
      if ((index + 1) % (anaclone.nJetR+1) == 0) gPad->SetRightMargin(0.01);
      gPad->SetTicks(1,1);
      if (index == (anaclone.nPtBins*(anaclone.nJetR+1) - 1)) hratio[i][j][ia]->GetXaxis()->SetTitle("p_{T,max}^{Jet}/p_{T,max}^{cluster}");
      else hratio[i][j][ia]->GetXaxis()->SetTitle("");
      hratio[i][j][ia]->GetXaxis()->SetTitleSize(0.10);
      hratio[i][j][ia]->GetXaxis()->SetLabelSize(0.08);
      if (index == (anaclone.nJetR+1)*(anaclone.nPtBins-1)+1) hratio[i][j][ia]->GetXaxis()->SetLabelSize(0.07);
      hratio[i][j][ia]->GetXaxis()->SetNdivisions(405,false);
      hratio[i][j][ia]->GetXaxis()->ChangeLabel(-1,-1,0);
      if (index == 1) hratio[i][j][ia]->GetYaxis()->SetTitle("Arbitrary Units");
      else hratio[i][j][ia]->GetYaxis()->SetTitle("");
      hratio[i][j][ia]->GetYaxis()->SetTitleSize(0.10);
      hratio[i][j][ia]->GetYaxis()->SetLabelSize(0.08);
      if (index == (anaclone.nJetR+1)*(anaclone.nPtBins-1)+1) hratio[i][j][ia]->GetYaxis()->SetLabelSize(0.07);
      hratio[i][j][ia]->GetYaxis()->SetLabelOffset(0.04);
      hratio[i][j][ia]->GetYaxis()->SetMaxDigits(3);
      hratio[i][j][ia]->GetYaxis()->SetDecimals(2);
      scale(hratio[i][j][ia]);
      hratio[i][j][ia]->GetYaxis()->SetRangeUser(0,hratio[i][j][ia]->GetMaximum()*2);
      hratio[i][j][ia]->SetLineColor(colors[1]);
      hratio[i][j][ia]->SetLineWidth(2);
      hratio[i][j][ia]->Draw("hist same");

      scale(hratiomc_p[i][j][ia]);
      hratiomc_p[i][j][ia]->Scale(1.0/(float)nprebin);
      hratiomc_p[i][j][ia]->SetLineColor(colors[3]);
      hratiomc_p[i][j][ia]->Draw("hist same");

      scale(hratiomc_j[i][j][ia]);
      hratiomc_j[i][j][ia]->Scale(1.0/(float)njrebin);
      hratiomc_j[i][j][ia]->SetLineColor(colors[5]);
      hratiomc_j[i][j][ia]->Draw("hist same");

      float lowjet = anaclone.ptBins[i];
      float highjet = anaclone.ptBins[i+1];
      float drawx = 0.15;
      if ((index - 1) % 5 == 0) drawx = 0.35;
      drawText(Form("%.0f GeV < p_{T}^{cluster} < %.0f GeV",lowjet,highjet),drawx,0.85,1,42);
      drawText(Form("Jet R=0.%i",j*2+2),drawx,0.75,1,42);
      TLine * line = new TLine(minjets[j]/lowjet,0,minjets[j]/lowjet,1);
      line->SetLineStyle(8);
      line->Draw();
    }
  }
  p->cd();
  anaclone.drawAll({
      "run 47289-53864",
      "MC run28 Jet",
      "MC run28 Photon"},
      {
      Form("|vz| < %.0f cm",anaclone.vzcut),
      Form("|#eta|_{jet} < %.1f - R",anaclone.etacut),
      Form("|#eta|_{cluster} < %.1f",anaclone.etacut),
      Form("#Delta#phi > %.0f#pi/%.0f",anaclone.oppnum,anaclone.oppden),
      Form("p_{T}^{jet} > %.0f GeV",anaclone.minjete02),
      Form("|#DeltaT|_{jet,cluster} < %.0f",anaclone.tcut),
      t1[ia].c_str(), t2[ia].c_str(), t3[ia].c_str()},
      drawx,drawy,fontsize,1);
  TLegend * l2 = new TLegend(drawx,drawy-.7,0.99,drawy-.55);
  l2->SetLineWidth(0);
  l2->AddEntry(hratio[0][0][0],("data"));
  l2->AddEntry(hratiomc_p[0][0][0],("MC Photon reco"));
  l2->AddEntry(hratiomc_j[0][0][0],("MC Jet reco"));
  l2->Draw();
  return;
}

void draw_one(TCanvas * c, vector<vector<vector<TH1D*>>> hratio, vector<vector<vector<TH1D*>>> hratiomc_p, vector<vector<vector<TH1D*>>> hratiomc_j, int ipt, int ir, int ia) {
  float drawx = 0.55;
  float drawy = 0.85;
  int i = ipt;
  int j = ir;
  vector<string> t1 = {"#bf{Analysis region A}:","#bf{Analysis region B}:","#bf{Analysis region C}:","#bf{Analysis region D}:", "Combined ABCD"};
  float minjets[4] = {anaclone.minjete02,anaclone.minjete04,anaclone.minjete06,anaclone.minjete08};
  int colors[6] = {kSpring + 2, kBlue, kGreen + 3, kMagenta+1, kTeal, kSpring+2};
  gPad->SetTicks(1,1);
  gPad->SetLeftMargin(.2);
  gPad->SetBottomMargin(.15);
  hratio[i][j][ia]->GetXaxis()->SetTitle("p_{T,max}^{Jet}/p_{T,max}^{cluster}");
  hratio[i][j][ia]->GetXaxis()->SetTitleSize(0.04);
  hratio[i][j][ia]->GetXaxis()->SetTitleOffset(1.5);
  hratio[i][j][ia]->GetXaxis()->SetLabelSize(0.04);

  hratio[i][j][ia]->GetYaxis()->SetTitle("Arbitrary Units");
  hratio[i][j][ia]->GetYaxis()->SetTitleSize(0.04);
  hratio[i][j][ia]->GetYaxis()->SetTitleOffset(2);
  hratio[i][j][ia]->GetYaxis()->SetLabelSize(0.04);
  hratio[i][j][ia]->GetYaxis()->SetMaxDigits(3);
  hratio[i][j][ia]->GetYaxis()->SetDecimals(2);
  hratio[i][j][ia]->Draw("hist e");

  hratiomc_p[i][j][ia]->Draw("hist e same");

  hratiomc_j[i][j][ia]->Draw("hist e same");

  float lowjet = anaclone.ptBins[i];
  float highjet = anaclone.ptBins[i+1];
  TLine * line = new TLine(minjets[j]/lowjet,0,minjets[j]/lowjet,hratio[i][j][ia]->GetMaximum());
  line->SetLineStyle(8);
  line->Draw();
  TLegend * l = new TLegend(.25,.65,.55,.87);
  l->SetLineWidth(0);
  l->SetFillStyle(0);
  l->AddEntry(hratio[i][j][ia],("data"));
  l->AddEntry(hratiomc_p[i][j][ia],("MC Photon reco"));
  l->AddEntry(hratiomc_j[i][j][ia],("MC Jet reco"));
  l->Draw();
  anaclone.drawAll({
      "run 47289-53864",
      "MC run28 Jet",
      "MC run28 Photon"},
      {Form("%0.0f GeV < p_{T}^{cluster} < %0.0f GeV",anaclone.ptBins[i],anaclone.ptBins[i+1]),
      "p_{T}^{jet} > 3 GeV", 
      Form("Jet R=%0.1f",anaclone.JetRs[j]), 
      "Analysis cuts",
      t1[ia].c_str()},
    drawx,drawy,20);
  return;
}


TH1D * combine_hists(TH1D * A, TH1D * B, TH1D * C, TH1D * D, int bin) {
  TH1D * h = (TH1D*)A->Clone();
  h->Clear();
  float p = anaclone.getPurity(anaclone.ptBins[bin],anaclone.ptBins[bin+1]);
  cout << "Purity for bins " << anaclone.ptBins[bin] << "-" << anaclone.ptBins[bin+1] << " = " << p << endl;
  h->Add(A,C,1/p,-(1-p)/p);
  return h;
}

void draw_abcd() {
  gStyle->SetOptStat(0);
  TFile * f = TFile::Open("hists/hists.root");
  TFile * f05_p = TFile::Open(Form("MChists/hists%s_unsmear.root","Photon5"));
  TFile * f10_p = TFile::Open(Form("MChists/hists%s_unsmear.root","Photon10"));
  TFile * f20_p = TFile::Open(Form("MChists/hists%s_unsmear.root","Photon20"));
  TFile * f05_j = TFile::Open(Form("MChists/hists%s_unsmear.root","Jet5"));
  TFile * f10_j = TFile::Open(Form("MChists/hists%s_unsmear.root","Jet10"));
  TFile * f20_j = TFile::Open(Form("MChists/hists%s_unsmear.root","Jet20"));
  TFile * f30_j = TFile::Open(Form("MChists/hists%s_unsmear.root","Jet30"));
  TFile * f50_j = TFile::Open(Form("MChists/hists%s_unsmear.root","Jet50"));
  TFile * f70_j = TFile::Open(Form("MChists/hists%s_unsmear.root","Jet70"));

  vector<vector<vector<TH1D*>>> hratio     = anaclone.collect_hists({f},{},"hratio",anaclone.nPtBins,anaclone.nJetR);
  vector<vector<vector<TH1D*>>> hratiomc_p = anaclone.collect_hists({f05_p,f10_p,f20_p},{5,10,20},"hratio",anaclone.nPtBins,anaclone.nJetR);
  vector<vector<vector<TH1D*>>> hratiomc_j = anaclone.collect_hists({f05_j,f10_j,f20_j,f30_j,f50_j,f70_j},{5,10,20,30,50,70},"hratio",anaclone.nPtBins,anaclone.nJetR);

  for (int i = 0; i < anaclone.nPtBins; i++) {
    for (int j = 0; j < anaclone.nJetR ; j++) {
      hratio[i][j].push_back(combine_hists(hratio[i][j][0],hratio[i][j][1],hratio[i][j][2],hratio[i][j][3],i));
      hratiomc_p[i][j].push_back(combine_hists(hratiomc_p[i][j][0],hratiomc_p[i][j][1],hratiomc_p[i][j][2],hratiomc_p[i][j][3],i));
      hratiomc_j[i][j].push_back(combine_hists(hratiomc_j[i][j][0],hratiomc_j[i][j][1],hratiomc_j[i][j][2],hratiomc_j[i][j][3],i));
    }
  }
  

  int csize =  700;
  TCanvas * c = new TCanvas("c","",csize*4+200,csize*3);
  c->SaveAs("pdfs/abcd.pdf[");
  // 5 for A,B,C,D, and the combined one
  for (int ia = 0; ia < 5; ia++) {
    c->Clear();
    draw_all(c, hratio,hratiomc_p,hratiomc_j,ia);
    c->SaveAs("pdfs/abcd.pdf");
  }
  c->SaveAs("pdfs/abcd.pdf]");

  int i = 3; // pt 13-15 GeV
  int j = 2; // R = 0.6
  TCanvas * cone = new TCanvas("cone","",csize,csize); 
  cone->SaveAs("pdfs/oneabcd.pdf[");
  for (int ia = 0; ia < 5; ia++) {
    cone->Clear();
    draw_one(cone, hratio,hratiomc_p,hratiomc_j,i,j,ia);
    cone->SaveAs("pdfs/oneabcd.pdf");
  }
  cone->SaveAs("pdfs/oneabcd.pdf]");


  return;
}
