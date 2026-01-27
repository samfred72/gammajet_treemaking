#include "../headers/commonUtility.h"
#include "../headers/Style_jaebeom.h"
#include "../headers/ana.cxx"

void scale(TH1D * h) {
  double x_low_value = 0;
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

void draw_insitu() {
  int nrebin = 4000;
  ana anaclone;
  gStyle->SetOptStat(0);
  TFile * f = TFile::Open("hists/hists.root");
  TFile * f05_s = TFile::Open(Form("MChists/hists%s_smear.root","Photon5"));
  TFile * f10_s = TFile::Open(Form("MChists/hists%s_smear.root","Photon10"));
  TFile * f20_s = TFile::Open(Form("MChists/hists%s_smear.root","Photon20"));
  TFile * f05_u = TFile::Open(Form("MChists/hists%s_unsmear.root","Photon5"));
  TFile * f10_u = TFile::Open(Form("MChists/hists%s_unsmear.root","Photon10"));
  TFile * f20_u = TFile::Open(Form("MChists/hists%s_unsmear.root","Photon20"));
  

  vector<vector<vector<vector<float>>>> x(3,vector<vector<vector<float>>>(4,vector<vector<float>>(2,vector<float>())));
  vector<vector<vector<vector<float>>>> y(3,vector<vector<vector<float>>>(4,vector<vector<float>>(2,vector<float>())));
  TH1D * hratio[3][4][2];
  TH1D * hratiomc_s[3][4][2];
  TH1D * hratiomc_u[3][4][2];
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 4 ; j++) {
      for (int k = 0; k < 2; k++) {
        // data
        hratio[i][j][k] = (TH1D*)f->Get(Form("hratio_%i_%i_%i",i,j,k));
        for (int l = 1; l < hratio[i][j][k]->GetNbinsX(); l++) {
          x[i][j][k].push_back(hratio[i][j][k]->GetBinCenter(l));
          y[i][j][k].push_back(hratio[i][j][k]->GetBinContent(l));
        }
        hratio[i][j][k]->Sumw2();
        scale(hratio[i][j][k]);
        
        // smeared MC
        TH1D * h05_s = (TH1D*)f05_s->Get(Form("hratio_%i_%i_%i",i,j,k));
        h05_s->SetName(Form("hratio_%i_%i_%i_05_s",i,j,k));
        TH1D * h10_s = (TH1D*)f10_s->Get(Form("hratio_%i_%i_%i",i,j,k));
        h10_s->SetName(Form("hratio_%i_%i_%i_10_s",i,j,k));
        TH1D * h20_s = (TH1D*)f20_s->Get(Form("hratio_%i_%i_%i",i,j,k));
        h20_s->SetName(Form("hratio_%i_%i_%i_20_s",i,j,k));
        h05_s->Sumw2();
        h10_s->Sumw2();
        h20_s->Sumw2();
        h05_s->Scale(146359.3);
        h10_s->Scale(6944.675);
        h20_s->Scale(130.4461);
        hratiomc_s[i][j][k] = h05_s;
        hratiomc_s[i][j][k]->Add(h10_s);
        hratiomc_s[i][j][k]->Add(h20_s);
        hratiomc_s[i][j][k]->Rebin(nrebin);
        scale(hratiomc_s[i][j][k]);
        //hratiomc_s[i][j][k]->Scale(1.0/nrebin);
        
        // unsmeared MC
        TH1D * h05_u = (TH1D*)f05_u->Get(Form("hratio_%i_%i_%i",i,j,k));
        h05_u->SetName(Form("hratio_%i_%i_%i_05_u",i,j,k));
        TH1D * h10_u = (TH1D*)f10_u->Get(Form("hratio_%i_%i_%i",i,j,k));
        h10_u->SetName(Form("hratio_%i_%i_%i_10_u",i,j,k));
        TH1D * h20_u = (TH1D*)f20_u->Get(Form("hratio_%i_%i_%i",i,j,k));
        h20_u->SetName(Form("hratio_%i_%i_%i_20_u",i,j,k));
        h05_u->Sumw2();
        h10_u->Sumw2();
        h20_u->Sumw2();
        h05_u->Scale(146359.3);
        h10_u->Scale(6944.675);
        h20_u->Scale(130.4461);
        hratiomc_u[i][j][k] = h05_u;
        hratiomc_u[i][j][k]->Add(h10_u);
        hratiomc_u[i][j][k]->Add(h20_u);
        hratiomc_u[i][j][k]->Rebin(nrebin);
        scale(hratiomc_u[i][j][k]);// ->Scale(1.0/hratio[i][j][k]->GetEntries());
        //hratiomc_u[i][j][k]->Scale(1.0/nrebin);
      }
    }
  }

  TCanvas * cmany[4];
  TCanvas * cchisq = new TCanvas("cchisq","",700*4,700*2);
  cchisq->Divide(4,2,0,0);
  float bestchisq[4] = {100000,100000,100000,100000};
  float bestchisqndf[4] = {100000,100000,100000,100000};
  float shift[4];
  TH1D * hbest[4];
  TH1D * hbestrat[4];
  TH1D * hchisq[4];
  TH1D * hchisqndf[4];
  int nbins = hratio[0][0][0]->GetNbinsX();
  float shiftval = 0.0001;
  int shifts = (int)(0.1/shiftval);
  TH1D * hbase = new TH1D("h","",100000/nrebin,0,2);
  for (int ih = 0; ih < 4; ih++) {
    cmany[ih] = new TCanvas(Form("cmany%i",ih),"",700,800);
    cmany[ih]->Divide(1,2,0,0);
    //cmany[ih]->SaveAs(Form("cmany%i.pdf[",ih));
    hchisq[ih] = new TH1D(Form("hchisq%i",ih),";shift val;chisq",      2*shifts,-0.1,.1);
    hchisqndf[ih] = new TH1D(Form("hchisqndf%i",ih),";shift val;chisq",2*shifts,-0.1,.1);
    for (int s = -shifts; s < shifts; s++) {
      TH1D * htest = (TH1D*)hbase->Clone();
      TH1D * hMC = (TH1D*)hratiomc_u[2][ih][1]->Clone();
      htest->Reset();
      for (int j = 0; j < x[2][ih][1].size(); j++) {
        htest->Fill(x[2][ih][1].at(j)-s*shiftval, y[2][ih][1].at(j));
      }
      scale(htest);

      cmany[ih]->cd(1);
      //htest->GetYaxis()->SetRangeUser(0,.1);
      htest->SetMarkerColor(kBlack);
      htest->SetMarkerSize(1);
      htest->SetMarkerStyle(20);
      hMC->SetLineColor(kRed);
      htest->Draw("pe");
      hMC->Draw("same");
      cmany[ih]->cd(2);
      TH1D * hrat = (TH1D*)htest->Clone();
      hrat->GetYaxis()->SetRangeUser(0,4);
      hrat->Divide(hMC);
      hrat->SetMarkerStyle(20);
      hrat->SetMarkerSize(1);
      hrat->Draw();
      //cmany[ih]->SaveAs(Form("cmany%i.pdf",ih));
      //float chisq = htest->Chi2Test(hMC, "CHI2 UW");
      //float chisqndf = htest->Chi2Test(hMC, "CHI2/NDF UW");
      float chisq = 0;
      float chisqndf = 0;
      for (int j = 0; j < hrat->GetNbinsX(); j++) {
        float val = (1-hrat->GetBinContent(j));
        if (val == 1) continue;
        float err = hrat->GetBinError(j);
        if (err > 0) chisq += val*val/err/err;
      }
      chisqndf = chisq/hrat->GetNbinsX();

      hchisq[ih]->Fill(s*shiftval,chisq);
      hchisqndf[ih]->Fill(s*shiftval,chisqndf);

      if (chisq < bestchisq[ih]) {
        cout << ih << " found best! " << chisq << " at " << -s*shiftval << endl;
        bestchisq[ih] = chisq;
        bestchisqndf[ih] = chisqndf;
        hbest[ih] = (TH1D*)htest->Clone();
        hbest[ih]->SetName(Form("hbest%i",ih));
        hbestrat[ih] = hrat;
        shift[ih] = -s*shiftval;
      }
      delete htest;
    }
    cchisq->cd(ih+1);
    //gPad->SetLogy();
    hchisq[ih]->GetYaxis()->SetRangeUser(bestchisq[ih]-1,bestchisq[ih]+100);
    hchisq[ih]->Draw("hist");
    cchisq->cd(ih+1+4);
    //gPad->SetLogy();
    hchisqndf[ih]->GetYaxis()->SetRangeUser(bestchisqndf[ih]-1,bestchisqndf[ih]+10);
    hchisqndf[ih]->Draw("hist");
    //cmany[ih]->SaveAs(Form("cmany%i.pdf]",ih));
  }
  cchisq->SaveAs("cchisq.pdf");
 




  // Now draw the final product
  int csize =  700;
  int colors[6] = {kSpring + 2, kBlue+2, kGreen + 3, kBlue + 4, kTeal, kMagenta+1};
  string text[2] = {"No iso cut",""};
  float minjets[4] = {anaclone.minjete02,anaclone.minjete04,anaclone.minjete06,anaclone.minjete08};
  float etas[4] = {.9,.7,.5,.4};
  TCanvas * c2 = new TCanvas("c2","",csize*4+200,csize*3);
  c2->Divide(5,2,0,0);
  gPad->SetBottomMargin(.15);
  gPad->SetRightMargin(.15);
  float fontsize = .08;
  for (int j = 0; j < 4; j++) {
    int index = j+1;
    c2->cd(index);
    gPad->SetTicks(1,1);
    hbest[j]->GetXaxis()->SetTitle("E_{T,max}^{Jet}/E_{T,max}^{cluster}");
    hbest[j]->GetYaxis()->SetTitle("Normalized Counts");
    hbest[j]->GetYaxis()->SetLabelSize(fontsize);
    hbest[j]->GetYaxis()->SetTitle("Normalized Counts");
    hbest[j]->SetLineColor(colors[1]);
    hbest[j]->SetLineWidth(2);
    hbest[j]->Draw("hist same");

    hratiomc_u[2][j][1]->SetLineColor(colors[1+4]);
    hratiomc_u[2][j][1]->Draw("hist same");

    float lowjet = anaclone.ptBins[2];
    float highjet = anaclone.ptBins[2+1];
    drawText(Form("Jet R=0.%i",j*2+2),0.45,0.85,1,52);
    drawText(Form("Shift = %.04f",shift[j]),0.45,0.80,1,52);
    TLine * line = new TLine(minjets[j]/lowjet,0,minjets[j]/lowjet,1);
    line->SetLineStyle(8);
    line->Draw();
  }
  
  for (int i = 0; i < 4; i++) {
    c2->cd(6+i);
    hbestrat[i]->GetXaxis()->SetTitle("E_{T,max}^{Jet}/E_{T,max}^{cluster}");
    hbestrat[i]->GetXaxis()->SetLabelSize(fontsize);
    hbestrat[i]->GetYaxis()->SetRangeUser(0.5,2);
    hbestrat[i]->GetYaxis()->SetTitle("Ratio");
    hbestrat[i]->GetYaxis()->SetLabelSize(fontsize);
    hbestrat[i]->SetMarkerStyle(20);
    hbestrat[i]->SetMarkerColor(kBlack);
    hbestrat[i]->SetLineColor(kMagenta+1);
    hbestrat[i]->SetMarkerSize(1);
    hbestrat[i]->Draw("p");
    TLine * l = new TLine(0,1,2,1);
    l->SetLineStyle(8);
    l->Draw(); 
    drawText(Form("#chi^{2} = %.05f",bestchisq[i]),.15,.9,1,52);
    drawText(Form("#chi^{2}/NDF = %.05f",bestchisqndf[i]),.15,.85,1,52);
  }
  
  c2->cd();
  float drawx = 0.80;
  float drawy = 0.92;
  float ydiff = 0.04;
  drawText("#bf{#it{sPHENIX}} Internal"   ,drawx,drawy-0.00-ydiff*0, 1,75);
  drawText("run 47289-53864"              ,drawx,drawy-0.05-ydiff*0, 1,65);
  drawText("MC run28 Jet10"               ,drawx,drawy-0.05-ydiff*1, 1,65);
  drawText("MC run28 Photon5"             ,drawx,drawy-0.05-ydiff*2, 1,65);
  drawText("|vz| < 30 cm"                 ,drawx,drawy-0.05-ydiff*3, 1,60);
  drawText("|#eta|_{jet} < 1.1 - R"       ,drawx,drawy-0.05-ydiff*4, 1,60);
  drawText("|#eta|_{cluster} < 1"         ,drawx,drawy-0.05-ydiff*5, 1,60);
  drawText("#Delta#phi > 7#pi/8"          ,drawx,drawy-0.05-ydiff*6, 1,60);
  drawText("E_{jet} > 3 GeV"              ,drawx,drawy-0.05-ydiff*7, 1,60);
  drawText("0 < #DeltaT_{mbd-cluster} < 4",drawx,drawy-0.05-ydiff*8, 1,60);
  drawText("#bf{Analysis region A}:"      ,drawx,drawy-0.05-ydiff*9, 1,60);
  drawText("E_{iso} < 2 GeV"              ,drawx,drawy-0.05-ydiff*10,1,60);
  drawText("bdt score > 0.8"              ,drawx,drawy-0.05-ydiff*11,1,60);

  c2->cd(10);
  gPad->SetRightMargin(0);
  TLegend * l2 = new TLegend(.05,.5,1,.85);
  //TLegend * l2 = new TLegend(0,1,0,1);
  l2->SetLineWidth(0);
  l2->AddEntry(hratio[0][0][1],("data"));
  l2->AddEntry(hratiomc_u[0][0][1],("MC reco"));
  l2->Draw();
  
  c2->SaveAs("insitu.pdf");

}
