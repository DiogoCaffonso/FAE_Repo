#include <iostream>
#include <cmath>
#include "TH1F.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TRandom.h"

void exercicio2() {

	int nEvents = 1500;
	double lambdaTrue = 1.0;
	double x[nEvents];

	for (int i = 0; i < nEvents; i++) {
		x[i] = -log(1 - gRandom->Uniform()) / lambdaTrue;
	}

	TH1F *hist = new TH1F("hist", "Histograma de eventos simulados", 50, 0, 10);
	for (int i = 0; i < nEvents; i++) {
		hist->Fill(x[i]);
	}

	TF1 *fitFunction = new TF1("fitFunction", "[0] * exp(-[1] * x)", 0, 10);
	fitFunction->SetParameters(nEvents, 1.0);
	fitFunction->SetParLimits(1, 0.1, 2);
	hist->Fit("fitFunction", "R");

	double lambdaAdjusted = fitFunction->GetParameter(1);
	double totalYield = fitFunction->Integral(0, 10);

	TCanvas *c1 = new TCanvas("c1", "Ajuste da função exponencial", 800, 600);
	hist->Draw();
	fitFunction->Draw("same");
	std::cout << "Valor ajustado para lambda: " << lambdaAdjusted << std::endl;

	c1->SaveAs("exercicio2.png");
}
