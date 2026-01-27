#include "../headers/commonUtility.h"
#include "../headers/Style_jaebeom.h"
#include "../headers/ana.cxx"

int colors[6] = {kRed, kBlue, kGreen-2, kMagenta, kOrange, kTeal};

void scale(TH1D * h) {
  double x_low_value = 0;
  double x_high_value = 100;

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

TH1D * getcombinedhist(vector<TFile*> files, vector<int> samples, string histname, bool isphoton) {
  if (files.size() != samples.size()) return nullptr;
  ana anaclone;
  vector<TH1D*> hists;
  for (int i = 0; i < files.size(); i++) {
    hists.push_back((TH1D*)files.at(i)->Get(histname.c_str()));
  }
  TH1D * thehist = anaclone.combineMC(hists,samples,isphoton);
  return thehist;
}

void drawMany(vector<TH1D*> hists, vector<string> labels) {
  if (hists.size() != labels.size()) return;
  if (hists.size() > 6) {cout << "too many hists!!"; return;}
  TLegend * l = new TLegend(.7,.45,.85,.7);
  TCanvas * c = new TCanvas(Form("c%s",labels.at(0).c_str()),"",700,700);
  gPad->SetLeftMargin(0.15);
  gPad->SetLogy();
  gPad->SetTicks(1,1);
  for (int i = 0; i < hists.size(); i++) {
    scale(hists.at(i));
    hists.at(i)->SetMarkerColor(colors[i]);
    hists.at(i)->SetMarkerStyle(20);
    hists.at(i)->SetMarkerSize(1);
    hists.at(i)->Draw("p same");
    l->AddEntry(hists.at(i),labels.at(i).c_str());
  }
  l->SetLineWidth(0);
  l->Draw();
}

void drawData(TFile * f, string histname, string label) {
  ana anaclone;
  TH1D * hist = (TH1D*)f->Get(histname.c_str());
  TLegend * l = new TLegend(.7,.45,.85,.7);
  TCanvas * c = new TCanvas(Form("c%s",histname.c_str()),"",700,700);
  gPad->SetLeftMargin(0.15);
  gPad->SetLogy();
  gPad->SetTicks(1,1);
  
  hist->SetMarkerColor(kBlack);
  hist->SetMarkerStyle(20);
  hist->SetMarkerSize(1);
  hist->GetYaxis()->SetTitle("Scaled counts");
  hist->GetXaxis()->SetTitle(label.c_str());
  hist->GetXaxis()->SetTitleOffset(1.2);
  hist->Draw("p same");
}

void drawMC(vector<TFile*> files, vector<int> samples, string histname, string label, bool isphoton) {
  if (files.size() != samples.size()) return;
  ana anaclone;
  vector<TH1D*> hists;
  for (int i = 0; i < files.size(); i++) {
    hists.push_back((TH1D*)files.at(i)->Get(histname.c_str()));
    hists.at(i)->SetName(Form("%s%i%i",histname.c_str(),i,(int)isphoton));
  }
  TH1D * thehist = anaclone.combineMC(hists,samples,isphoton);
  TLegend * l = new TLegend(.7,.45,.85,.7);
  TCanvas * c = new TCanvas(Form("c%s%i",histname.c_str(),(int)isphoton),"",700,700);
  gPad->SetLeftMargin(0.15);
  gPad->SetLogy();
  gPad->SetTicks(1,1);
  string trigger = (isphoton ? "Photon" : "Jet");
  
  thehist->SetMarkerColor(kBlack);
  thehist->SetMarkerStyle(20);
  thehist->SetMarkerSize(1);
  thehist->GetYaxis()->SetTitle("Scaled counts");
  thehist->GetXaxis()->SetTitle(label.c_str());
  thehist->GetXaxis()->SetTitleOffset(1.2);
  thehist->Draw("p same");
  l->AddEntry(thehist,"Sum");

  for (int i = 0; i < hists.size(); i++) {
    hists.at(i)->Scale(anaclone.scalemap[isphoton][samples.at(i)]);
    hists.at(i)->SetMarkerColor(colors[i]);
    hists.at(i)->SetMarkerStyle(24);
    hists.at(i)->SetMarkerSize(1);
    l->AddEntry(hists.at(i), Form("%s %i",trigger.c_str(),samples.at(i)));
    hists.at(i)->Draw("p same");
  }
  l->SetLineWidth(0);
  l->Draw();
}
    
void draw_pt() {
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
  
  float drawx = .55;
  float drawy = .85;
  float fontsize = 20;

  //TH1D * hdata = (TH1D*)f->Get("hclusterpt");
  //TH1D * hmcp = getcombinedhist({f05p,f10p,f20p},{5,10,20},"hclusterpt",1);
  //TH1D * hmcj = getcombinedhist({f05j,f10j,f20j,f30j,f50j,f70j},{5,10,20,30,50,70},"hclusterpt",0);
  //drawMany({hdata,hmcp,hmcj},{"Data","MC Photon","MC Jet"});
  //anaclone.drawAll({},{"|vz| < 60"},drawx,drawy,fontsize);

  //drawMC({f05p,f10p,f20p},{5,10,20},"hclusterpt","Leading p_{T}^{reco cluster}",1);
  //anaclone.drawAll({"MC Photon"},{"|vz| < 60"},drawx,drawy,fontsize); 
  drawMC({f05j,f10j,f20j,f30j,f50j,f70j},{5,10,20,30,50,70},"hclusterpt","Leading p_{T}^{reco cluster}",0);
  anaclone.drawAll({"MC Jet"},{"|vz| < 60"},drawx,drawy,fontsize); 


  //drawMC({f05j,f10j,f20j,f30j,f50j,f70j},{5,10,20,30,50,70},"htruthjetpt0",0);
  //anaclone.drawAll({"MC Jet"},{"jet R = 0.2","|vz| < 60"},drawx,drawy,fontsize); 
  //drawMC({f05j,f10j,f20j,f30j,f50j,f70j},{5,10,20,30,50,70},"hjetptprecut1","Max p_{T}^{reco jet}",0);
  //anaclone.drawAll({"MC Jet"},{"jet R = 0.4","|vz| < 60"},drawx,drawy,fontsize); 
  //drawMC({f05j,f10j,f20j,f30j,f50j,f70j},{5,10,20,30,50,70},"hjetpt1","Max p_{T}^{reco jet}",0);
  //anaclone.drawAll({"MC Jet"},{"jet R = 0.4","|vz| < 60"},drawx,drawy,fontsize); 
  //drawMC({f05j,f10j,f20j,f30j,f50j,f70j},{5,10,20,30,50,70},"htruthjetpt2",0);
  //anaclone.drawAll({"MC Jet"},{"jet R = 0.6","|vz| < 60"},drawx,drawy,fontsize); 
  //drawMC({f05j,f10j,f20j,f30j,f50j,f70j},{5,10,20,30,50,70},"htruthjetpt3",0);
  //anaclone.drawAll({"MC Jet"},{"jet R = 0.8","|vz| < 60"},drawx,drawy,fontsize); 
  
  //drawMC({f05p,f10p,f20p},{5,10,20},"htruthjetptprecut0",1);
  //anaclone.drawAll({"MC Photon"},{"jet R = 0.2","|vz| < 60"},drawx,drawy,fontsize); 
  //drawMC({f05p,f10p,f20p},{5,10,20},"htruthjetptprecut1","Leading p_{T}^{truth jet}",1);
  //anaclone.drawAll({"MC Photon"},{"jet R = 0.4","|vz| < 60","Pre-cluster cut"},drawx,drawy,fontsize); 
  //drawMC({f05p,f10p,f20p},{5,10,20},"htruthjetpt1","Leading p_{T}^{truth jet}",1);
  //anaclone.drawAll({"MC Photon"},{"jet R = 0.4","|vz| < 60","post-cluster cut","jet unmatched to photon"},drawx,drawy,fontsize); 
  //drawMC({f05p,f10p,f20p},{5,10,20},"htruthjetptprecut2",1);
  //anaclone.drawAll({"MC Photon"},{"jet R = 0.6","|vz| < 60"},drawx,drawy,fontsize); 
  //drawMC({f05p,f10p,f20p},{5,10,20},"htruthjetptprecut3",1);
  //anaclone.drawAll({"MC Photon"},{"jet R = 0.8","|vz| < 60"},drawx,drawy,fontsize); 
  
  //drawMC({f05p,f10p,f20p},{5,10,20},"htruthclusterptprecut","Leading p_{T}^{truth photon}",1);
  //anaclone.drawAll({"MC Photon"},{"|vz| < 60"},drawx,drawy,fontsize); 
  //drawMC({f05p,f10p,f20p},{5,10,20},"htruthjetptprecut1","Leading p_{T}^{truth jet}",1);
  //anaclone.drawAll({"MC Photon"},{"|vz| < 60"},drawx,drawy,fontsize); 
  //drawMC({f05p,f10p,f20p},{5,10,20},"htruthjetptprecutspec1","Leading p_{T}^{truth jet}",1);
  //anaclone.drawAll({"MC Photon"},{"|vz| < 60","Jet unmatched to photon"},drawx,drawy,fontsize); 
  //drawMC({f05p,f10p,f20p},{5,10,20},"htruthjetptprecutanti1","Leading p_{T}^{truth jet}",1);
  //anaclone.drawAll({"MC Photon"},{"|vz| < 60","Jet matched to photon"},drawx,drawy,fontsize); 
  //drawMC({f05j,f10j,f20j,f30j,f50j,f70j},{5,10,20,30,50,70},"htruthclusterptprecut",0);
  //anaclone.drawAll({"MC Jet"},{"|vz| < 60"},drawx,drawy,fontsize); 
  //TH2D * ha = (TH2D*)f05p->Get("htruthjetcluster");
  //TH2D * hb = (TH2D*)f10p->Get("htruthjetcluster");
  //TH2D * hc = (TH2D*)f20p->Get("htruthjetcluster");
  //TProfile * pa = ha->ProfileX();
  //pa->SetName("pa");
  //TProfile * pb = hb->ProfileX();
  //pb->SetName("pb");
  //TProfile * pc = hc->ProfileX();
  //pc->SetName("pc");
  //pa->SetLineColor(kRed);
  //pb->SetLineColor(kRed);
  //pc->SetLineColor(kRed);
  //pa->SetLineWidth(2);
  //pb->SetLineWidth(2);
  //pc->SetLineWidth(2);
  //TLine * l = new TLine(0,0,100,100);
  //TCanvas * ca = new TCanvas("ca","",700,700);
  //gPad->SetLogz();
  //gPad->SetTicks(1,1);
  //ha->Draw("colz");
  //pa->Draw("same");
  //l->Draw("same");
  //anaclone.drawAll({"MC Photon05"},{"|vz| < 60"},drawx,drawy,fontsize); 
  //TCanvas * cb = new TCanvas("cb","",700,700);
  //gPad->SetLogz();
  //gPad->SetTicks(1,1);
  //hb->Draw("colz");
  //pb->Draw("same");
  //l->Draw("same");
  //anaclone.drawAll({"MC Photon10"},{"|vz| < 60"},drawx,drawy,fontsize); 
  //TCanvas * cc = new TCanvas("cc","",700,700);
  //gPad->SetLogz();
  //gPad->SetTicks(1,1);
  //hc->Draw("colz");
  //pc->Draw("same");
  //l->Draw("same");
  //anaclone.drawAll({"MC Photon20"},{"|vz| < 60"},drawx,drawy,fontsize); 

}
