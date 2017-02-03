#include <iostream>
#include <cmath>
#include <TGraph2D.h>
#include <TMath.h>
#include <TFile.h>
#include <TString.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TTree.h>
using namespace std;

Int_t nBinTot = 28*400;
Double_t binAverages[28*400];
Double_t theta[28*400];
Double_t binEnergy[28*400];
Int_t at_binx = 0;

 // Double_t *binAverages = new Double_t[28*400];
 // Double_t *theta = new Double_t[28*400];
 // Double_t *binEnergy = new Double_t[28*400];


void ReadFitArray(char *infile) {

  TString *filename = new TString(infile);
  TFile *f1 = new TFile(filename->Data());
  TTree *t1 = (TTree*)f1->Get("Test");

  filename->ReplaceAll("../lead_theta","");
  filename->ReplaceAll("_phi0.root","");
  int theta_bin = filename->Atoi();

  double theta_v = double(theta_bin)/180.*TMath::Pi();
  printf("Analyzing theta_bin=%d theta=%.3f\n",theta_bin,theta_v);
  Double_t kineticEnergy, energyLoss, ThetaInit, PhiInit;

  t1->SetBranchAddress("kineticEnergy", &kineticEnergy);
  t1->SetBranchAddress("energyLoss", &energyLoss);
  t1->SetBranchAddress("ThetaInit", &ThetaInit);
  t1->SetBranchAddress("PhiInit", &PhiInit);

//TH2F *h1 = new TH2F("h1", "Energy Loss Theta Dependence", 500.0, 0.0, 3.15, 1000.0, 0.0, 100.0);

//TH2F *h2 = new TH2F("h2", "Energy Loss Phi Dependence", 500.0, 0.0, 1.58,
//1000.0, 0.0, 100.0);

//TH3F *h3 = new TH3F("h3", "Energy Loss Angular Dependence", 500.0, -1.58, 1.58, 500.0, 0.0, 3.15, 1000.0, 0.0, 100.0);

  TH2F *h1 = new TH2F("h1", "Energy Loss Correction", 400.0, 0.0, 400.0, 1000.0, 0.0, 100.0);

  Int_t nEntries = (Int_t)t1->GetEntries();

  for (Int_t i=0; i<nEntries; i++) {

    t1->GetEntry(i);
    if (energyLoss > 1.0) {
        //h1->Fill(ThetaInit, energyLoss);
	//h3->Fill(PhiInit, ThetaInit, energyLoss);
	h1->Fill(kineticEnergy, energyLoss);
    }

    //h2->Fill(PhiInit, energyLoss);

  }
  
//h1->Draw();

  h1->FitSlicesY();
  TH1D *h1_1 = (TH1D*)gDirectory->Get("h1_1");
  TH1D *h1_chi2 = (TH1D*)gDirectory->Get("h1_chi2");
  TH1D *h1_2 = (TH1D*)gDirectory->Get("h1_2");

  printf("got slice in Y \n");
 

 

  Int_t nBinsX = 400; // number of energy bins
  //  Int_t at_binx = (theta_bin-22)/2 *400;
  cout << nBinsX << " " << h1_1->GetNbinsX() << " At bin" << at_binx << endl;
 //TTree *outTree = new TTree("averages", "Averaged energy loss bin");
 //outTree->Branch("binAverages", &binAverages, "binAverages[nBinsX]");
 //outTree->Branch("theta", &theta, "theta[nBinsX]");
 //outTree->Branch("binEnergy", &binEnergy, "binEnergy[nBinsX]");

 for (Int_t i=0; i<nBinsX; i++) {
   if (h1_1->GetBinContent(i) < 50.&& h1_1->GetBinContent(i) > 0.0 && h1_2->GetBinError(i) < 0.15 && h1_2->GetBinError(i) > 0.0 )    {
     binAverages[at_binx] = h1_1->GetBinContent(i);
     theta[at_binx] = theta_v;
     binEnergy[at_binx] = double(i);
     printf("at bin=%d Chisquare ok fit=%.3e energy=%.1f Errorsigma=%.3f\n",at_binx,h1_chi2->GetBinContent(i),binEnergy[at_binx],h1_2->GetBinError(i));
     at_binx++;
   }
   // else  if ( (h1_1->GetBinContent(i) > 50. || h1_1->GetBinContent(i) < 0.0) &&  i>0 && i<(nBinsX - 1) ) {
   //   if (h1_1->GetBinContent(i-1)  < 50.&& h1_1->GetBinContent(i-1)  > 0.0 ) 
   //   binAverages[i+at_binx] = (h1_1->GetBinContent(i-1) +  h1_1->GetBinContent(i+1)) / 2; 
   // }  
   else {
     // binAverages[i+at_binx] = 0.0;
     printf("Chisquare wrong fit=%.3e Errorsigma=%.3f \n",h1_chi2->GetBinContent(i),h1_2->GetBinError(i));
   }
   // theta[i+at_binx] = theta_v;
   // binEnergy[i+at_binx] = double(i);
  // cout << binAverages[i] << " " <<theta[i] << " " << binEnergy[i] << endl;
  // outTree->Fill();
 }
 printf("finished filling array\n");
}

 void Define2dGraph() {

 TFile *outFile = new TFile("24degAveraged.root", "RECREATE");
 outFile->cd();

 
 TGraph2D *tg1 = new TGraph2D(at_binx,theta,binEnergy,binAverages);

 // for (Int_t i=0; i<nBinsX; i++) {
 //   // cout << binEnergy[i] << ".0" << ", ";
 //   cout << binAverages[i] << ", ";
 // }

 //TGraph2D *tg1 = new TGraph2D(nBinsX, binEnergy, theta, binAverages);
 //tg1->Draw("LINE");
 //cout << "TEST: "  << tg1->Interpolate(0.383, 200.0) <<endl;
 tg1->Write("ELOSS");
 outFile->Close();

}


void Run_all(){
  char name[100];
  for (int i=0;i<28;i++) {
    sprintf(name,"../lead_theta%d_phi0.root",22+i*2);
    printf("Reading %s\n",name);
    ReadFitArray(name);
  }
  Define2dGraph();
}
