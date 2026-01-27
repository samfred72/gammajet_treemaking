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

void draw_all(vector<vector<vector<TH1D*>>> hratio, vector<vector<TH1D*>> hists, string info, int ihist) {
  int csize = 700;
  float drawx = 0.80;
  float drawy = 0.92;
  float fontsize = 60;
  int njrebin = 1;
  int nprebin = 1;
  int offset = 0;
  int npt = hratio.size(); 
  int njet = hratio[0].size();
  int colors[6] = {kBlue, kMagenta+1, kSpring+2};
  float minjets[4] = {anaclone.minjete02,anaclone.minjete04,anaclone.minjete06,anaclone.minjete08};
  vector<string> t1 = {"#bf{Analysis region A}:","#bf{Analysis region B}:","#bf{Analysis region C}:","#bf{Analysis region D}:", "Combined ABCD"};
  vector<string> t2 = {"E_{iso} < 2 GeV","E_{iso} > 2 GeV","E_{iso} < 2 GeV","E_{iso} > 2 GeV",""};
  vector<string> t3 = {"bdt score > 0.8","bdt score > 0.8","No bdt cut", "No bdt cut",""};

  TCanvas * c = new TCanvas(Form("c%i",ihist),"",csize*njet+200,csize*npt);
  TPad * p = new TPad("p","",0,0,1,1);
  p->SetLeftMargin(0.25);
  p->SetBottomMargin(0.25);
  p->Divide(njet+1,npt,0,0);
  p->Draw();
  anaclone.drawAll(
      {
      info.c_str()
      },
      {
      "Analysis Cuts",
      },
      drawx, drawy, 60, 1
      );

  for (int ia = 0; ia < npt; ia++) {
    for (int j = 0; j < njet+1; j++) {
      int index = ia*(njet+1) + j+1;
      if (index % 5 == 0) continue;

      p->cd(index);
      if ((index-1) % 4 == 0) gPad->SetRightMargin(0.01);
      gPad->SetTicks(1,1);
      if ((index-1) % 4 == 0) hratio[ia][j][0]->GetXaxis()->SetTitle("p_{T,max}^{Jet}/p_{T,max}^{cluster}");
      else hratio[ia][j][0]->GetXaxis()->SetTitle("");
      hratio[ia][j][0]->GetXaxis()->SetTitleSize(0.10);
      hratio[ia][j][0]->GetXaxis()->SetLabelSize(0.08);
      if (index == njet*(npt-1)) hratio[ia][j][0]->GetXaxis()->SetLabelSize(0.07);
      hratio[ia][j][0]->GetXaxis()->SetNdivisions(405,false);
      hratio[ia][j][0]->GetXaxis()->ChangeLabel(-1,-1,0);
      if (index == njet*(npt-1)) hratio[ia][j][0]->GetYaxis()->SetTitle("Arbitrary Units");
      else hratio[ia][j][0]->GetYaxis()->SetTitle("");
      hratio[ia][j][0]->GetYaxis()->SetTitleSize(0.10);
      hratio[ia][j][0]->GetYaxis()->SetLabelSize(0.08);
      if (index == njet*(npt-1)) hratio[ia][j][0]->GetYaxis()->SetLabelSize(0.07);
      hratio[ia][j][0]->GetYaxis()->SetLabelOffset(0.04);
      hratio[ia][j][0]->GetYaxis()->SetMaxDigits(3);
      hratio[ia][j][0]->GetYaxis()->SetDecimals(2);
      scale(hratio[ia][j][0]);
      hratio[ia][j][0]->Scale(1.0/(float)nprebin);
      hratio[ia][j][0]->GetYaxis()->SetRangeUser(0,hratio[ia][j][0]->GetMaximum()*2);
      hratio[ia][j][0]->SetLineColor(colors[ihist]);
      hratio[ia][j][0]->Draw("hist same");

      float lowjet = anaclone.abcdbins[ia];
      float highjet = anaclone.abcdbins[ia+1];
      float drawx = 0.15;
      if ((index-1)%5 == 0) drawx = 0.35;
      drawText(Form("%.0f GeV < p_{T}^{cluster} < %.0f GeV",lowjet,highjet),drawx,0.85,1,42);
      drawText(Form("Jet R=0.%i",j*2+2),drawx,0.75,1,42);
      TLine * line = new TLine(minjets[j]/lowjet,0,minjets[j]/lowjet,1);
      line->SetLineStyle(8);
      line->Draw();

      //TF1 * func2 = new TF1(Form("func2%i%i",ia,j),"gaus",(int)(minjets[j]/lowjet/0.04) * 0.04,2);
      //TF1 * func2 = new TF1(Form("func2%i%i",ia,j),"gaus",(int)(minjets[j]/lowjet/0.04 + 1)*0.04,2);
      TF1 * func2 = new TF1(Form("func2%i%i",ia,j),"gaus",minjets[j]/lowjet,2);
      hratio[ia][j][0]->Fit(func2,"RQI0");
      func2->Draw("same");
      if (func2->GetParameter(1) < 100 && func2->GetParameter(1) > 0) {
        hists[ihist][j]->SetBinContent(ia+1,func2->GetParameter(1));
        hists[ihist][j]->SetBinError(ia+1,func2->GetParError(1));
      }
    }
  }
  c->SaveAs(Form("pdfs/xjfits%i.pdf",ihist));
  return;
}

TH1D * combine_hists(TH1D * A, TH1D * B, TH1D * C, TH1D * D, int bin) {
  TH1D * h = (TH1D*)A->Clone();
  h->Clear();
  float p = anaclone.getPurity(anaclone.ptBins[bin],anaclone.ptBins[bin+1]);
  h->Add(A,C,1/p,-(1-p)/p);
  return h;
}

void draw_axj() {
  int nrebin = 4000;
  int njrebin = 1;
  int nprebin = 1;
  ana anaclone;
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

  vector<vector<TH1D*>> hists(3,vector<TH1D*>(anaclone.nJetR)); // 3 for data, and 2 MCs
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < anaclone.nJetR; j++) {
      hists[i][j] = new TH1D(Form("axj_%i_%i",i,j),";p_{T}^{cluster};<x_{J#gamma}>",anaclone.nPtBins,anaclone.ptBins);
    }
  }
  draw_all(hratio,hists,"data",0);
  draw_all(hratiomc_p,hists,"MC Photon",1);
  draw_all(hratiomc_j,hists,"MC Jet",2);
  
  int csize =  700;
  string info[3] = {"Data","MC Photon","MC Jet"};
  int colors[3] = {kBlue, kMagenta+1, kSpring+2};
  
  TCanvas * cxj = new TCanvas("cxj","",1000,700);
  cxj->SaveAs("pdfs/axj.pdf[");
  for (int i = 0; i < anaclone.nJetR; i++) {
    TPad * p1 = new TPad("p1","",0,.4,1,1);
    TPad * p2 = new TPad("p2","",0,0,1,0.4);
    p1->Draw();
    p2->Draw();
    p1->SetBottomMargin(0);
    p2->SetTopMargin(0);
    p1->cd();
    gPad->SetTicks(1,1);
    TLegend * lxj = new TLegend(.15,.65,.4,.85);
    for (int j = 0; j < 2; j++) {
      hists[j][i]->GetYaxis()->SetRangeUser(0,2);
      hists[j][i]->SetMarkerColor(colors[j]);
      hists[j][i]->SetLineColor(colors[j]);
      hists[j][i]->SetMarkerSize(1);
      hists[j][i]->SetMarkerStyle(20);
      if (j == 0) hists[j][i]->Draw();
      else hists[j][i]->Draw("same");
      lxj->AddEntry(hists[j][i],info[j].c_str());
      lxj->SetLineWidth(0);
      lxj->Draw();
    }
    anaclone.drawAll(
        {"run 47289-53864",
        "MC run28 Photon",
        "MC run28 Jet"},
        {"Analysis cuts",
        Form("Jet R=%0.1f",anaclone.JetRs[i]) }, 
        .5, .8, 16);

    p2->cd();
    gPad->SetTicks(1,1);
    TH1D * dummy = (TH1D*)hists[0][i]->Clone();
    dummy->SetName(Form("d%i",i));
    dummy->Reset("ICES");
    dummy->GetYaxis()->SetTitle("Ratio data/MC");
    dummy->GetYaxis()->SetRangeUser(.5,1.5);
    dummy->Draw();
    TH1D * hdivide = (TH1D*)hists[0][i]->Clone();
    hdivide->SetName(Form("h%i",i));
    hdivide->Divide(hists[0][i],hists[1][i]);
    hdivide->SetMarkerColor(kBlack);
    hdivide->SetMarkerStyle(20);
    hdivide->SetMarkerSize(1);
    hdivide->Draw("same");
    drawLine(10,1,30,1); 

    cxj->SaveAs("pdfs/axj.pdf");
    cxj->Clear();
  }
  cxj->SaveAs("pdfs/axj.pdf]");
}
