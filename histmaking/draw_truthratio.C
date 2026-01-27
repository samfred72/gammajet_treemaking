#include "../headers/commonUtility.h"
#include "../headers/Style_jaebeom.h"
#include "../headers/ana.cxx"

int colors[6] = {kRed, kBlue, kGreen-2, kMagenta, kOrange, kTeal};
void drawMC(vector<TFile*> files, vector<int> samples, string histname, string label, bool isphoton) {
  if (files.size() != samples.size()) return;
  ana anaclone;
  vector<TH2D*> hists;
  for (int i = 0; i < files.size(); i++) {
    hists.push_back(((TH2D*)files.at(i)->Get(histname.c_str())));
  }
  TH2D * thehist = new TH2D();//anaclone.combineMC(hists,samples,isphoton);
  TLegend * l = new TLegend(.7,.45,.85,.7);
  TCanvas * c = new TCanvas(Form("c%s",histname.c_str()),"",700,700);
  gPad->SetLeftMargin(0.15);
  //gPad->SetLogy();
  gPad->SetTicks(1,1);
  string trigger = (isphoton ? "Photon" : "Jet");
  
  thehist->SetMarkerColor(kBlack);
  thehist->SetMarkerStyle(20);
  thehist->SetMarkerSize(1);
  thehist->GetYaxis()->SetTitle("p_{T}^{matched jet}/p_{T}^{truth photon}");
  if (label != "") thehist->GetXaxis()->SetTitle(label.c_str());
  thehist->GetXaxis()->SetTitleOffset(1.2);
  //thehist->Draw("p same");
  //l->AddEntry(thehist,"Sum");

  TProfile * h[6];
  for (int i = 0; i < hists.size(); i++) {
    h[i] = hists.at(i)->ProfileX();
    h[i]->SetName(Form("blah%i",i));
    h[i]->GetYaxis()->SetTitle("p_{T}^{matched jet}/p_{T}^{truth photon}");
    h[i]->SetMarkerColor(colors[i]);
    h[i]->SetMarkerStyle(20);
    h[i]->SetMarkerSize(1);
    l->AddEntry(h[i], Form("%s %i",trigger.c_str(),samples.at(i)));
    h[i]->Draw("p same");
  }
  cout << h[0]->GetBinContent(3) << " " << h[1]->GetBinContent(3) << " " << h[2]->GetBinContent(3) << endl;
  l->SetLineWidth(0);
  l->Draw();
}
    
void draw_truthratio() {
  ana anaclone;
  gStyle->SetOptStat(0);
  TFile * f = TFile::Open("hists/hists.root");
  TFile * f05p = TFile::Open("MChists/histsPhoton5_unsmear.root");
  TFile * f10p = TFile::Open("MChists/histsPhoton10_unsmear.root");
  TFile * f20p = TFile::Open("MChists/histsPhoton20_unsmear.root");
  
  float drawx = .55;
  float drawy = .85;
  float fontsize = 20;
  
  drawMC({f05p,f10p,f20p},{5,10,20},"htruthratio","",1);
  anaclone.drawAll({"MC Photon"},{"|vz| < 60"},drawx,drawy,fontsize); 

}
