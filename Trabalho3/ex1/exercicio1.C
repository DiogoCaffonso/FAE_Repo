#include <iostream>
#include <cmath>
#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TMath.h"
#include "Math/Integrator.h"

double myFunc(double *x, double *par) {
    double p0 = par[0];
    double p1 = par[1];
    return p0 * TMath::Sin(p1 * x[0]) / x[0];
}

void exercicio1() {
	double p0 = 1.0;
	double p1 = 2.0;
	double x = 1.0;

	TCanvas *c1 = new TCanvas("c1", "Function Plot", 800, 600);
	TF1 *f1 = new TF1("f1", myFunc, 0.01, 10, 2);
	f1->SetParameters(p0,p1);
	f1->SetLineColor(kBlue);
	f1->Draw();
	c1->SaveAs("function_plot.png");

	double func_value = f1->Eval(x);
	std::cout << "Function value at x = 1: " << func_value << std::endl;

	double dx = 1e-6;
	double derivada = (f1->Eval(x + dx) - f1->Eval(x - dx)) / (2 * dx);
	std::cout << "Derivative at x = 1: " << derivada << std::endl;

	double integral = f1->Integral(0, 3);
	std::cout << "Integral from 0 to 3: " << integral << std::endl;
}

