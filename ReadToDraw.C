#include <iostream>
#include <cmath>
using namespace std;

void ReadToDraw() {

TFile *f1 = new TFile("100MeV_1million.root");
TTree *t1 = (TTree*)f1->Get("Test;1");

Double_t energyLoss, ThetaInit, PhiInit);

t1->SetBranchAddress("energyLoss", &energyLoss);
t1->SetBranchAddress("ThetaInit", &ThetaInit);
t1->SetBranchAddress("PhiInit", &PhiInit);

TH2F *h1 = new TH2F("h1", "Energy Loss Theta Dependence", 500.0, 0.0, 3.15, 100000.0, 0.0, 100.0);

TH2F *h2 = new TH2F("h2", "Energy Loss Phi Dependence", 500.0, 0.0, 1.58,
100000.0, 0.0, 100.0);

TH3F *h3 = new TH3F("h3", "Energy Loss Angular Dependence", 500.0, -1.58, 1.58, 500.0, 0.0, 3.15, 1000.0, 0.0, 100.0);

Int_t nEntries = (Int_t)t1->GetEntries();

for (Int_t i=0; i<nEntries; i++) {

    t1->GetEntry(i);
    if (energyLoss > 6.5) {
        h1->Fill(ThetaInit, energyLoss);
	//h3->Fill(PhiInit, ThetaInit, energyLoss);
    }

    h2->Fill(PhiInit, energyLoss);

}

//h1->Draw();

}
