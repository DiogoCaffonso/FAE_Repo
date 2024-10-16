#include <iostream>
#include <cmath>
#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TMath.h"
#include "Math/Integrator.h"
#include "TRandom.h"

void exercicio3() {

	TH1F *h1 = new TH1F("h1", "Gaussian Distribution", 50, 0, 10);

	TRandom *rand = new TRandom();
	for (int i = 0; i < 10000; i++) {
		h1->Fill(rand->Gaus(5, 2));  // Mean 5, Sigma 2
	}

	gStyle->SetOptStat("nemruoiks");
	gStyle->SetStatX(0.9);
	gStyle->SetStatY(0.9);
	gStyle->SetStatW(0.2);
	gStyle->SetStatH(0.3);

	TCanvas *c1 = new TCanvas("c1", "Gaussian Histogram", 800, 600);

	h1->Draw();

	c1->Update();

	TPaveStats *stats = (TPaveStats*)h1->GetListOfFunctions()->FindObject("stats");

	if (stats) {
		stats->SetName("mystats");

		double skewness = h1->GetSkewness();
		double kurtosis = h1->GetKurtosis();

		stats->AddText(Form("Skewness = %.3f", skewness));
		stats->AddText(Form("Kurtosis = %.3f", kurtosis));

		stats->SetX1NDC(0.7);
		stats->SetY1NDC(0.7);

		c1->Modified();
		c1->Update();
	}

	c1->SaveAs("gaussian_histogram.png");
}

