#include <TFile.h>
#include <TTree.h>
#include <TTreeFormula.h>
#include <TH1.h>
#include <TH2.h>
#include <THStack.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TAxis.h>
#include <iostream>
#include <iostream>
#include <sstream>
#include <map>
#include <algorithm> 
#include </afs/cern.ch/work/i/ishvetso/GitHub/IvanShvetsov/CMS_stylistics/tdrstyle.C>
#include </afs/cern.ch/work/i/ishvetso/GitHub/IvanShvetsov/CMS_stylistics/CMS_lumi.cpp>

using namespace std;

struct range
{
  double low;
  double high;
  int NBins;
};

struct Var
{
  string VarName;
  range Range;
  Color_t color;
  void SetRange(double xlow, double xhigh, int NBins);
};

void Var::SetRange(double xlow, double xhigh, int NBinsX)
{
  range RangeX;
  RangeX.low = xlow;
  RangeX.high = xhigh;
  RangeX.NBins = NBinsX;
  Range = RangeX;
}


struct Sample
{
  string filename;
  string Processname; // name of the process
  
  Sample();
  void SetParameters(string Processname_);
  void SetFileNames(string filename_);  
};
Sample::Sample()
{}
void Sample::SetParameters( string Processname_)
{
  Processname = Processname_; 
}

void Sample::SetFileNames(string filename_)
{
  filename = filename_;
}


// draw a number of variables on the same canvas (to compare distributions)
void draw(vector<Var> Vars, Sample sample, string fileNamePrefix, string addCommentOnLegend, string selection, string OutDir)
{
  gStyle->SetCanvasColor(kWhite);
//gStyle->SetTitleFillColor(kWhite);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
//gPad->SetGrid();
  
  vector <TH1F*> hists;
  TFile file((sample.filename).c_str(), "READ");
  TLegend *leg = new TLegend(0.7,0.93,0.9,0.8);
  leg -> SetFillColor(kWhite);  
  vector <float> Buf;
    
  Buf.resize(Vars.size());
  
 for (unsigned int iVar = 0 ; iVar < Vars.size(); iVar ++)
 {
    TH1F *hist_tmp = new TH1F((Vars.at(iVar).VarName + "_tmp").c_str(), (Vars.at(iVar).VarName + "_tmp").c_str(), Vars.at(iVar).Range.NBins, Vars.at(iVar).Range.low, Vars.at(iVar).Range.high);
  //  hist_tmp->SetDirectory(0);// histogram would be deleted otherwise when file is closed
    ((TTree*)file.Get("ntupler/ElectronTree")) -> Project((Vars.at(iVar).VarName + "_tmp").c_str(), (Vars.at(iVar).VarName).c_str(), selection.c_str());
    hists.push_back(hist_tmp);
 }
  
 // 
 TCanvas *c1= new TCanvas("c1","canvas",1200,800);
  
 c1 -> cd();
 
 float hist_max = 0.0;
  for (unsigned iVar = 0; iVar < Vars.size(); iVar ++)
  {	   
   if ((hists.at(iVar) -> GetMaximum()) > hist_max )  hist_max = (hists.at(iVar) -> GetMaximum());
  }
  
  for (unsigned iVar = 0; iVar < Vars.size(); iVar ++)
  {	   
    hists.at(iVar) -> SetLineColor(Vars.at(iVar).color);
    hists.at(iVar) -> GetXaxis() -> SetTitle("GeV");
    hists.at(iVar) -> GetYaxis() -> SetRangeUser(0., 1.2*hist_max);
    hists.at(iVar) -> SetLineWidth(3.0);
    leg->AddEntry(hists.at(iVar), (Vars.at(iVar).VarName).c_str(),"l");
    hists.at(iVar) -> Draw("HISTE1SAME");
    leg -> SetHeader((sample.Processname + "  " + addCommentOnLegend).c_str());
  }
  
  leg -> Draw("SAME");   
  CMS_lumi( c1, 4, 0 );
  
  system(("mkdir -p " + OutDir ).c_str());
  c1 -> SaveAs(( OutDir + "/" + fileNamePrefix + "_" + sample.Processname + "_" + addCommentOnLegend  +  ".png").c_str());
  
  hists.clear();
  delete c1;
} 

//draw the difference between 2 variables (1 dim )
void draw_difference(string var1_, string var2_, Sample sample, int Nbins, double xmin, double xmax, string attribute, string outDirectory)
{
   system(("mkdir -p " + outDirectory).c_str());
  
  gStyle->SetCanvasColor(kWhite);
//gStyle->SetTitleFillColor(kWhite);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
//gPad->SetGrid();
  
  TH1F *hist = new TH1F((var1_ + " - " + var2_).c_str(), (var1_ + " - " + var2_).c_str(), Nbins, xmin, xmax);
  TFile file((sample.filename).c_str(), "READ");
  TLegend *leg = new TLegend(0.18,0.8,0.5,0.9);
  leg -> SetFillColor(kWhite);  
  TTree * tree = (TTree * )file.Get("ntupler/ElectronTree");
  
  float var1, var2;
  
  tree -> SetBranchAddress(var1_.c_str(), &var1);
  tree -> SetBranchAddress(var2_.c_str(), &var2);
  
  for (unsigned int iEntry = 0; iEntry < tree -> GetEntries(); iEntry ++  )
  {
    tree -> GetEntry(iEntry);
    hist -> Fill(var1 - var2);
  }
  
 // 
 TCanvas *c1= new TCanvas("c1","canvas",1200,800);
  
 c1 -> cd();

 hist -> SetLineColor(kBlue);
 hist -> GetXaxis() -> SetTitle("GeV");
 hist -> SetLineWidth(3.0);
 leg->AddEntry(hist, (var1_  + " - " + var2_).c_str(),"l");
 hist -> Draw("HISTE1SAME");
 leg -> SetHeader((sample.Processname).c_str());


 leg -> Draw("SAME");   
 CMS_lumi( c1, 4, 0 );
 c1 -> SaveAs((outDirectory + "/"  + attribute + "_" + sample.Processname + ".png").c_str());
  
 delete c1;
 delete hist;
} 

//draw difference versus some variable (2 dim)
void draw_difference2D(string var1_, string var2_, string var3_, Sample sample, int NXbins, double xmin, double xmax,int NYBins, double ymin, double ymax, string attribute, string outDirectory)
{
   system(("mkdir -p " + outDirectory).c_str());
  
  gStyle->SetCanvasColor(kWhite);
//gStyle->SetTitleFillColor(kWhite);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
//gPad->SetGrid();
  
  TH2F *hist = new TH2F((var1_ + " - " + var2_ + "_" + var3_).c_str(), (var1_ + " - " + var2_).c_str(), NXbins, xmin, xmax, NYBins, ymin, ymax);
  TFile file((sample.filename).c_str(), "READ");
  TLegend *leg = new TLegend(0.18,0.8,0.5,0.9);
  leg -> SetFillColor(kWhite);  
  TTree * tree = (TTree * )file.Get("ntupler/ElectronTree");
  
  float var1, var2, var3;
  
  tree -> SetBranchAddress(var1_.c_str(), &var1);
  tree -> SetBranchAddress(var2_.c_str(), &var2);
  tree -> SetBranchAddress(var3_.c_str(), &var3);
  
  for (unsigned int iEntry = 0; iEntry < tree -> GetEntries(); iEntry ++  )
  {
    tree -> GetEntry(iEntry);
    hist -> Fill(var1 - var2, var3);
  }
  
 // 
 TCanvas *c1= new TCanvas("c1","canvas",1200,800);
  
 c1 -> cd();

// hist -> SetLineColor(kBlue);
 hist -> GetXaxis() -> SetTitle("GeV");
 hist -> GetYaxis() -> SetTitle(var3_.c_str());
 gStyle->SetPalette(1);
 hist -> Draw("COLZ");

 CMS_lumi( c1, 4, 0 );
 c1 -> SaveAs((outDirectory + "/"  + attribute + "_" + var3_ + "_" + sample.Processname + ".png").c_str());
  
 delete c1;
 delete hist;
} 
