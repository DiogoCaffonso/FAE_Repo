#include <iostream>
#include <cmath>
#include "TCanvas.h"
#include "TF1.h"
#include "TGraph.h"
#include "TAxis.h"
#include "TMath.h"
#include "Math/Integrator.h"
#include "TGraphErrors.h"
#include "TStyle.h"

void exercicio2() {
	std::ifstream dataFile("graphdata.txt");
	std::ifstream errorFile("graphdata_error.txt");

	const int nPoints = 10;
	double x[nPoints], y[nPoints];
	double ex[nPoints], ey[nPoints];

	for (int i = 0; i < nPoints; ++i) {
		dataFile >> x[i] >> y[i];
	}

	for (int i = 0; i < nPoints; ++i) {
		errorFile >> x[i] >> y[i] >> ex[i] >> ey[i];
	}

	dataFile.close();
	errorFile.close();

	TCanvas *c1 = new TCanvas("c1", "Graph of Points", 800, 600);
	TGraph *graph1 = new TGraph(nPoints, x, y);
	graph1->SetMarkerStyle(21);
	graph1->SetMarkerColor(kBlack);
	graph1->SetTitle("Graph of Points;X;Y");
	graph1->Draw("AP");
	c1->SaveAs("graph_points.png");

	TCanvas *c2 = new TCanvas("c2", "Graph with Line", 800, 600);
	TGraph *graph2 = new TGraph(nPoints, x, y);
	graph2->SetMarkerStyle(21);
	graph2->SetMarkerColor(kBlack);
	graph2->SetLineColor(kBlue);
	graph2->SetLineWidth(2);
	graph2->SetTitle("Graph with Line;X;Y");
	graph2->Draw("APL");
	c2->SaveAs("graph_with_line.png");

	TCanvas *c3 = new TCanvas("c3", "Graph with Errors", 800, 600);
	TGraphErrors *graph3 = new TGraphErrors(nPoints, x, y, ex, ey);
	graph3->SetMarkerStyle(21);
	graph3->SetMarkerColor(kBlack);
	graph3->SetLineColor(kRed);
	graph3->SetLineWidth(2);
	graph3->SetTitle("Graph with Errors;X;Y");
	graph3->Draw("AP");
	c3->SaveAs("graph_with_errors.png");
}

