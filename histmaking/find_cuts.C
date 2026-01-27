#include "../headers/ana.cxx"

double get_counts(TH2D * h, float x, float y) {
  double count = 0;  
  int xlow; int xhigh;
  int ylow; int yhigh;
  
  if ( x < 0) { xlow = -x; xhigh = h->GetNbinsX(); }
  else {xlow = 1; xhigh = x; }
  
  if ( y < 0) { ylow = -y; yhigh = h->GetNbinsY(); }
  else {ylow = 1; yhigh = y; }
  
  for (int j = xlow; j < xhigh; j++) {
    for (int k = ylow; k < yhigh; k++) {
      count += h->GetBinContent(j,k);
    }
  }
  return count;
}

void find_cuts() {
  ana anaclone;
  TFile * f = TFile::Open("hists/hists.root");
  TFile * f05 = TFile::Open("MChists/histsJet5_unsmear.root");
  TFile * f10 = TFile::Open("MChists/histsJet10_unsmear.root");
  TFile * f20 = TFile::Open("MChists/histsJet20_unsmear.root");
  TFile * f30 = TFile::Open("MChists/histsJet30_unsmear.root");
  TFile * f50 = TFile::Open("MChists/histsJet50_unsmear.root");
  TFile * f70 = TFile::Open("MChists/histsJet70_unsmear.root");

  vector<TH2D*> mchists;
  TH2D * h[anaclone.nabcdbins];
  TGraph * g1 = new TGraph();
  TGraph * g2 = new TGraph();
  TGraph * g3 = new TGraph();
  TGraph * g4 = new TGraph();
  TCanvas * c = new TCanvas("c","",700,700);
  c->SaveAs("cuts.pdf[");
  vector<float> isobins;
  vector<float> bdtbins;
  for (int i = 7; i < 8; i++) {
    double counts = 0;
    double bestcount = 0;
    double bestj = 0;
    double bestk = 0;
    double bestA = 0;
    double bestB = 0;
    double bestC = 0;
    double bestD = 0;
    float bestscore = 999999999.9;
    mchists.clear();
    mchists.push_back((TH2D*)f05->Get(Form("hisobdt%i",i)));
    mchists.push_back((TH2D*)f10->Get(Form("hisobdt%i",i)));
    mchists.push_back((TH2D*)f20->Get(Form("hisobdt%i",i)));
    mchists.push_back((TH2D*)f30->Get(Form("hisobdt%i",i)));
    mchists.push_back((TH2D*)f50->Get(Form("hisobdt%i",i)));
    mchists.push_back((TH2D*)f70->Get(Form("hisobdt%i",i)));
    h[i] = anaclone.combineMC(mchists,{5,10,20,30,50,70},0);
    h[i] = (TH2D*)f->Get(Form("hisobdt%i",i)); 

    for (int j = 1; j < h[i]->GetNbinsX(); j++) {
      for (int k = 1; k < h[i]->GetNbinsY(); k++) {
        double countsA = get_counts(h[i], j,-k);
        double countsB = get_counts(h[i], j, k);
        double countsC = get_counts(h[i],-j, k);
        double countsD = get_counts(h[i],-j,-k);
        double sum = countsB + countsC + countsD;
        double diffs = abs(countsB - countsC) + abs(countsB - countsD) + abs(countsC - countsD);
        double ave = diffs/3.0;
        if (sum > 0) {
          double score = ave/sum;
          if (score < bestscore && countsA > sum/3.0 * 1.5) {
            bestscore = score;
            bestj = j;
            bestk = k;
            bestcount = counts;
            bestA = countsA;
            bestB = countsB;
            bestC = countsC;
            bestD = countsD;
          }
        }
        //cout << countsA << " " << countsB << " " << countsC << " " << countsD << endl;
     //   g1->AddPoint(counts,countsA);
     //   g2->AddPoint(counts,countsB);
     //   g3->AddPoint(counts,countsC);
     //   g4->AddPoint(counts,countsD);
        counts++;
      }
    }
    gPad->SetLogz();
    gPad->SetRightMargin(.15);
    TLine * lx = new TLine(h[i]->GetXaxis()->GetBinCenter(bestj),0,h[i]->GetXaxis()->GetBinCenter(bestj),1);
    TLine * ly = new TLine(-1,h[i]->GetYaxis()->GetBinCenter(bestk),20,h[i]->GetYaxis()->GetBinCenter(bestk));
    gStyle->SetOptStat(0);
    h[i]->Draw("colz");
    lx->SetLineColor(kRed);
    ly->SetLineColor(kRed);
    lx->Draw("same");
    ly->Draw("same");
    lx->SetLineWidth(2);
    ly->SetLineWidth(2);
    anaclone.drawAll({"data"},{Form("%i GeV < cluster p_{T} < %i GeV",i,i+1),"p_{T}^{jet} > 3 GeV","#Delta#phi > 7#pi/8"},.35,.6,25);
    c->SaveAs("cuts.pdf");

    isobins.push_back(h[i]->GetXaxis()->GetBinCenter(bestj));
    bdtbins.push_back(h[i]->GetYaxis()->GetBinCenter(bestk));
    cout << i << " " << h[i]->GetXaxis()->GetBinCenter(bestj) << " " << h[i]->GetYaxis()->GetBinCenter(bestk) << endl;
    cout << "For pT bin " << i << " with score " << bestscore << " at count " << bestcount << ":" << endl;
    cout << "iso cut: " << h[i]->GetXaxis()->GetBinCenter(bestj) << endl;
    cout << "bdt cut: " << h[i]->GetYaxis()->GetBinCenter(bestk) << endl;
    cout << "gives countA: " << bestA << " countB: " << bestB << " countC: " << bestC << " countD:" << bestD << endl; 
  }
  TGraph * giso = new TGraph();
  TGraph * gbdt = new TGraph();
  for (int i = 0; i < isobins.size(); i++) {
    cout << isobins.at(i) << ",";
    giso->AddPoint(7+i,isobins.at(i));
  }
  cout << endl;
  for (int i = 0; i < bdtbins.size(); i++) {
    cout << bdtbins.at(i) << ",";
    gbdt->AddPoint(7+i,bdtbins.at(i));
  }
  cout << endl;
  c->SaveAs("cuts.pdf]");
  //g1->SetMarkerColor(kRed);
  //g2->SetMarkerColor(kBlue);
  //g3->SetMarkerColor(kGreen);
  //g4->SetMarkerColor(kBlack);
  //g1->SetMarkerStyle(20);
  //g2->SetMarkerStyle(20);
  //g3->SetMarkerStyle(20);
  //g4->SetMarkerStyle(20);
  //g1->SetMarkerSize(1);
  //g2->SetMarkerSize(1);
  //g3->SetMarkerSize(1);
  //g4->SetMarkerSize(1);
  //g1->Draw("ap");
  //g2->Draw("p same");
  //g3->Draw("p same");
  //g4->Draw("p same");
  TCanvas * ciso = new TCanvas("ciso","",700,700);
  giso->SetMarkerColor(kBlue);
  giso->SetMarkerStyle(20);
  giso->SetMarkerSize(1);
  giso->GetXaxis()->SetTitle("Cluster p_{T}");
  giso->GetYaxis()->SetTitle("Cluster iso");
  giso->Draw("ap");
  TCanvas * cbdt = new TCanvas("cbdt","",700,700);
  gbdt->SetMarkerColor(kGreen);
  gbdt->SetMarkerStyle(20);
  gbdt->SetMarkerSize(1);
  gbdt->GetXaxis()->SetTitle("Cluster p_{T}");
  gbdt->GetYaxis()->SetTitle("Cluster bdt");
  gbdt->Draw("ap");
}
