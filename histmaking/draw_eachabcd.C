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

void draw_eachabcd() {
  int nrebin = 2000;
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
  

  TH1D * hratio     [anaclone.nPtBins][anaclone.nJetR][4];
  TH1D * hratiomc_p [anaclone.nPtBins][anaclone.nJetR][4];
  TH1D * hratiomc_j [anaclone.nPtBins][anaclone.nJetR][4];
  TH1D * Ohratio    [anaclone.nPtBins][anaclone.nJetR][4];
  TH1D * Ohratiomc_p[anaclone.nPtBins][anaclone.nJetR][4];
  TH1D * Ohratiomc_j[anaclone.nPtBins][anaclone.nJetR][4];

  const char * histname = "hratio";
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4 ; j++) {
      for (int k = 0; k < 4; k++) {
        // data
        hratio[i][j][k] = (TH1D*)f->Get(Form("%s_%i_%i_%i", histname,i,j,k));
        hratio[i][j][k]->Rebin(nrebin); 
        
        // Photon samples 
        TH1D * h05_p = (TH1D*)f05_p->Get(Form("%s_%i_%i_%i",histname,i,j,k));
        h05_p->SetName(Form("hratio_%i_%i_%i_05_p",i,j,k));
        TH1D * h10_p = (TH1D*)f10_p->Get(Form("%s_%i_%i_%i",histname,i,j,k));
        h10_p->SetName(Form("hratio_%i_%i_%i_10_p",i,j,k));
        TH1D * h20_p = (TH1D*)f20_p->Get(Form("%s_%i_%i_%i",histname,i,j,k));
        h20_p->SetName(Form("hratio_%i_%i_%i_20_p",i,j,k));

        h05_p->Rebin(nrebin);
        h10_p->Rebin(nrebin);
        h20_p->Rebin(nrebin);
        hratiomc_p[i][j][k] = anaclone.combineMC({h05_p,h10_p,h20_p},{5,10,20},1);
        hratiomc_p[i][j][k]->Rebin(nprebin);
        
        // Jet samples
        TH1D * h05_j = (TH1D*)f05_j->Get(Form("%s_%i_%i_%i",histname,i,j,k));
        h05_j->SetName(Form("hratio_%i_%i_%i_05_j",i,j,k));
        TH1D * h10_j = (TH1D*)f10_j->Get(Form("%s_%i_%i_%i",histname,i,j,k));
        h10_j->SetName(Form("hratio_%i_%i_%i_10_j",i,j,k));
        TH1D * h20_j = (TH1D*)f20_j->Get(Form("%s_%i_%i_%i",histname,i,j,k));
        h20_j->SetName(Form("hratio_%i_%i_%i_20_j",i,j,k));
        TH1D * h30_j = (TH1D*)f30_j->Get(Form("%s_%i_%i_%i",histname,i,j,k));
        h30_j->SetName(Form("hratio_%i_%i_%i_30_j",i,j,k));
        TH1D * h50_j = (TH1D*)f50_j->Get(Form("%s_%i_%i_%i",histname,i,j,k));
        h50_j->SetName(Form("hratio_%i_%i_%i_50_j",i,j,k));
        TH1D * h70_j = (TH1D*)f70_j->Get(Form("%s_%i_%i_%i",histname,i,j,k));
        h70_j->SetName(Form("hratio_%i_%i_%i_70_j",i,j,k));
        
        h05_j->Rebin(nrebin);
        h10_j->Rebin(nrebin);
        h20_j->Rebin(nrebin);
        h30_j->Rebin(nrebin);
        h50_j->Rebin(nrebin);
        h70_j->Rebin(nrebin);
        hratiomc_j[i][j][k] = anaclone.combineMC({h05_j,h10_j,h20_j,h30_j,h50_j,h70_j},{5,10,20,30,50,70},0);
        //hratiomc_j[i][j][k] = anaclone.combineMC({h05_j,h10_j,h20_j,h30_j},{5,10,20,30},0);
        hratiomc_j[i][j][k]->Rebin(njrebin);
        //hratiomc_j[i][j][k]->Draw();
        //return;
      
        Ohratio[i][j][k] = (TH1D*)hratio[i][j][k]->Clone();
        Ohratiomc_p[i][j][k] = (TH1D*)hratiomc_p[i][j][k]->Clone();
        Ohratiomc_j[i][j][k] = (TH1D*)hratiomc_j[i][j][k]->Clone();
      }
    }
  }

  int csize =  700;
  int i = 1; // 10 - 15 GeV
  int j = 2; // Jet R=0.6
  TCanvas * c = new TCanvas("c","",csize,csize);
  c->SaveAs("eachabcd.pdf[");
  string letter[4] = {"A","B","C","D"};
  float minjets[4] = {anaclone.minjete02,anaclone.minjete04,anaclone.minjete06,anaclone.minjete08};
  int colors[6] = {kSpring + 2, kBlue, kGreen + 3, kMagenta+1, kTeal, kSpring+2};
  for (int ia = 0; ia < 4; ia++) {
    hratio[i][j][ia]->GetXaxis()->SetTitle("p_{T,max}^{Jet}/p_{T,max}^{cluster}");
    hratio[i][j][ia]->GetXaxis()->SetTitleSize(0.04);
    hratio[i][j][ia]->GetXaxis()->SetTitleOffset(1.5);
    hratio[i][j][ia]->GetXaxis()->SetLabelSize(0.04);

    hratio[i][j][ia]->GetYaxis()->SetTitle("Arbitrary Units");
    hratio[i][j][ia]->GetYaxis()->SetTitleSize(0.04);
    hratio[i][j][ia]->GetYaxis()->SetTitleOffset(1.5);
    hratio[i][j][ia]->GetYaxis()->SetLabelSize(0.04);
    hratio[i][j][ia]->GetYaxis()->SetMaxDigits(3);
    hratio[i][j][ia]->GetYaxis()->SetDecimals(2);
    
    hratiomc_p[i][j][ia]->GetXaxis()->SetTitle("p_{T,max}^{Jet}/p_{T,max}^{cluster}");
    hratiomc_p[i][j][ia]->GetXaxis()->SetTitleSize(0.04);
    hratiomc_p[i][j][ia]->GetXaxis()->SetTitleOffset(1.5);
    hratiomc_p[i][j][ia]->GetXaxis()->SetLabelSize(0.04);

    hratiomc_p[i][j][ia]->GetYaxis()->SetTitle("Arbitrary Units");
    hratiomc_p[i][j][ia]->GetYaxis()->SetTitleSize(0.04);
    hratiomc_p[i][j][ia]->GetYaxis()->SetTitleOffset(1.5);
    hratiomc_p[i][j][ia]->GetYaxis()->SetLabelSize(0.04);
    hratiomc_p[i][j][ia]->GetYaxis()->SetMaxDigits(3);
    hratiomc_p[i][j][ia]->GetYaxis()->SetDecimals(2);
    
    hratiomc_j[i][j][ia]->GetXaxis()->SetTitle("p_{T,max}^{Jet}/p_{T,max}^{cluster}");
    hratiomc_j[i][j][ia]->GetXaxis()->SetTitleSize(0.04);
    hratiomc_j[i][j][ia]->GetXaxis()->SetTitleOffset(1.5);
    hratiomc_j[i][j][ia]->GetXaxis()->SetLabelSize(0.04);

    hratiomc_j[i][j][ia]->GetYaxis()->SetTitle("Arbitrary Units");
    hratiomc_j[i][j][ia]->GetYaxis()->SetTitleSize(0.04);
    hratiomc_j[i][j][ia]->GetYaxis()->SetTitleOffset(1.5);
    hratiomc_j[i][j][ia]->GetYaxis()->SetLabelSize(0.04);
    hratiomc_j[i][j][ia]->GetYaxis()->SetMaxDigits(3);
    hratiomc_j[i][j][ia]->GetYaxis()->SetDecimals(2);
  }
  int newc[4] = {kBlue,kRed,kGreen,kOrange};
  float drawx = .55;
  float drawy = .75;
  int fsize = 20;
  TLegend * l = new TLegend(.55,.45,.87,.65);
  l->SetLineWidth(0);
  const char * info[4] = {"Analysis region A","Analysis region B","Analysis region C","Analysis region D"};
  for (int ia = 0; ia < 4; ia++) {
    gPad->SetTicks(1,1);
    gPad->SetLeftMargin(.2);
    gPad->SetBottomMargin(.15);
    const char * text = (ia == 0 ? "hist":"hist same");
    hratio[i][j][ia]->SetLineColor(newc[ia]);
    hratio[i][j][ia]->Draw(text);
    l->AddEntry(hratio[i][j][ia],info[ia]);
  }
  l->Draw();
  anaclone.drawAll({"data"},{"Analysis cuts"},drawx,drawy,fsize);
  c->SaveAs("eachabcd.pdf");
  for (int ia = 0; ia < 4; ia++) {
    gPad->SetTicks(1,1);
    gPad->SetLeftMargin(.2);
    gPad->SetBottomMargin(.15);
    const char * text = (ia == 0 ? "hist":"hist same");
    hratiomc_p[i][j][ia]->SetLineColor(newc[ia]);
    hratiomc_p[i][j][ia]->Draw(text);
  }
  l->Draw();
  anaclone.drawAll({"MC Photon"},{"Analysis cuts"},drawx,drawy,fsize);
  c->SaveAs("eachabcd.pdf");
  for (int ia = 0; ia < 4; ia++) {
    gPad->SetTicks(1,1);
    gPad->SetLeftMargin(.2);
    gPad->SetBottomMargin(.15);
    const char * text = (ia == 0 ? "hist":"hist same");
    hratiomc_j[i][j][ia]->SetLineColor(newc[ia]);
    hratiomc_j[i][j][ia]->Draw(text);
  }
  l->Draw();
  anaclone.drawAll({"MC Jets"},{"Analysis cuts"},drawx,drawy,fsize);
  c->SaveAs("eachabcd.pdf");
  c->SaveAs("eachabcd.pdf]");

}
