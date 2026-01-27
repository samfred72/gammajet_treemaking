#include "../headers/commonUtility.h"
#include "../headers/Style_jaebeom.h"
#include "../headers/ana.cxx"

int colors[6] = {kRed, kBlue, kGreen-2, kMagenta, kOrange, kTeal};

void drawdata(TFile * f, string histname) {
  ana anaclone;
  TH1D * h = (TH1D*)f->Get(histname.c_str());
  TLegend * l = new TLegend(.15,.5,.5,.7);
  TCanvas * c = new TCanvas(Form("cdata%s",histname.c_str()),"",700,700);
  
  h->SetMarkerColor(kBlack);
  h->SetMarkerStyle(20);
  h->SetMarkerSize(1);
  h->Rebin(2);
  h->GetXaxis()->SetTitle("p_{T}^{cluster}/p_{T}^{jet}");
  h->Draw("p same");
  l->AddEntry(h,"MBD + Photon > 5 GeV");
  l->SetLineWidth(0);
  l->Draw();
}

void drawMC(vector<TFile*> files, vector<int> samples, string histname, bool isphoton) {
  if (files.size() != samples.size()) return;
  ana anaclone;
  vector<TH1D*> hists;
  for (int i = 0; i < files.size(); i++) {
    hists.push_back((TH1D*)files.at(i)->Get(histname.c_str()));
  }
  TH1D * thehist = anaclone.combineMC(hists,samples,isphoton);
  TLegend * l = new TLegend(.15,.25,.4,.6);
  TCanvas * c = new TCanvas(Form("c%s%i",histname.c_str(),(int)isphoton),"",700,700);
  string trigger = (isphoton ? "Photon" : "Jet");
  
  thehist->SetMarkerColor(kBlack);
  thehist->SetMarkerStyle(20);
  thehist->SetMarkerSize(1);
  thehist->Rebin(2);
  thehist->GetXaxis()->SetTitle("p_{T}^{cluster}/p_{T}^{jet}");
  thehist->Draw("p same");
  l->AddEntry(thehist,"Sum");

  for (int i = 0; i < hists.size(); i++) {
    hists.at(i)->Scale(anaclone.scalemap[isphoton][samples.at(i)]);
    hists.at(i)->SetMarkerColor(colors[i]);
    hists.at(i)->SetMarkerStyle(20);
    hists.at(i)->SetMarkerSize(1);
    hists.at(i)->Rebin(2);
    l->AddEntry(hists.at(i), Form("%s %i",trigger.c_str(),samples.at(i)));
    hists.at(i)->Draw("p same");
  }
  l->SetLineWidth(0);
  l->Draw();
}
    
void draw_Z() {
  ana anaclone;
  gStyle->SetOptStat(0);
  TFile * f = TFile::Open("hists/hists.root");
  TFile * f05p = TFile::Open("MChists/histsPhoton5_unsmear.root");
  TFile * f10p = TFile::Open("MChists/histsPhoton10_unsmear.root");
  TFile * f20p = TFile::Open("MChists/histsPhoton20_unsmear.root");
  TFile * f05j = TFile::Open("MChists/histsJet5_unsmear.root");
  TFile * f10j = TFile::Open("MChists/histsJet10_unsmear.root");
  TFile * f20j = TFile::Open("MChists/histsJet20_unsmear.root");
  TFile * f30j = TFile::Open("MChists/histsJet30_unsmear.root");
  TFile * f50j = TFile::Open("MChists/histsJet50_unsmear.root");
  TFile * f70j = TFile::Open("MChists/histsJet70_unsmear.root");
  
  float drawx = .15;
  float drawy = .85;
  float fontsize = 20;
  const char * hist = "frag";
  

  drawdata(f,"hfrag");
  anaclone.drawAll({"Run 24 pp"},{"jet R = 0.4","|vz| < 60","analysis cuts"},drawx,drawy,fontsize); 
  drawdata(f,"hfragiso");
  anaclone.drawAll({"Run 24 pp"},{"jet R = 0.4","|vz| < 60","analysis cuts","isolation cut"},drawx,drawy,fontsize); 

  drawMC({f05j,f10j,f20j,f30j,f50j,f70j},{5,10,20,30,50,70},"hfrag",0);
  anaclone.drawAll({"MC Jet"},{"jet R = 0.4","|vz| < 60","analysis cuts"},drawx,drawy,fontsize); 
  drawMC({f05j,f10j,f20j,f30j,f50j,f70j},{5,10,20,30,50,70},"hfragiso",0);
  anaclone.drawAll({"MC Jet"},{"jet R = 0.4","|vz| < 60","analysis cuts","isolation cut"},drawx,drawy,fontsize); 
  
  drawMC({f05p,f10p,f20p},{5,10,20},"hfrag",1);
  anaclone.drawAll({"MC Photon"},{"jet R = 0.4","|vz| < 60","analysis cuts"},drawx,drawy,fontsize); 
  drawMC({f05p,f10p,f20p},{5,10,20},"hfragiso",1);
  anaclone.drawAll({"MC Photon"},{"jet R = 0.4","|vz| < 60","analysis cuts","isolation cut"},drawx,drawy,fontsize); 

}
