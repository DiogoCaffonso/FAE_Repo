#include <iostream>
#include <cmath>
#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TMath.h"
#include "Math/Integrator.h"
#include "TRandom.h"

void exercicio4() {

	TCanvas *c1 = new TCanvas("c1", "Total Momentum Distribution", 800, 600);
	TFile *file = TFile::Open("tree.root");
	TTree *tree = (TTree*)file->Get("tree1");
	TH1F *h1 = new TH1F("h1", "Total Momentum Distribution", 50, 0, 1000);
	TH1F *h2 = new TH1F("h2", "Cut Momentum Distribution", 100, 100, 200);
	float px, py, pz, e;
	Int_t noE = 1e+5;

	tree->SetBranchAddress("px", &px);
	tree->SetBranchAddress("py", &py);
	tree->SetBranchAddress("pz", &pz);
	tree->SetBranchAddress("ebeam", &e);

	for (Int_t i=0; i<noE; i++){
		tree->GetEntry(i);
		h1->Fill(e);
	}

	float e_mean = h1->GetMean();
	for (Int_t i=0; i<noE; i++){
		tree->GetEntry(i);
		if (e > e_mean + 0.2 || e < e_mean - 0.2){
			float p = sqrt(px*px + py*py + pz*pz);
			h2->Fill(p);
		}	
	}

	h2->Draw();
	c1->SaveAs("total_momentum_distribution.png");

}

