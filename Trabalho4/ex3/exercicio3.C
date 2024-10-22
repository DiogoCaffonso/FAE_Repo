#include <iostream>
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "RooCBShape.h"
#include "RooPolynomial.h"
#include "RooAddPdf.h"
#include "RooPlot.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"

void exercicio3() {
	TFile *file = TFile::Open("/home/cms-opendata/FAE_Repo/Trabalho4/ex3/DataSet_lowstat.root");
	RooDataSet *data = dynamic_cast<RooDataSet*>(file->Get("data"));

	RooRealVar mass("mass", "Massa", 2.8, 3.5);
	RooRealVar mean("mean", "Média", 3.096916, 3.0, 3.2);
	RooRealVar sigma("sigma", "Desvio Padrão", 0.05, 0.01, 0.1);
	RooRealVar alpha("alpha", "Alpha", 1.5);
	RooRealVar n("n", "n", 0.5, 0, 5);
	RooCBShape signal("signal", "Sinal J/ψ", mass, mean, sigma, alpha, n);
    
	RooRealVar a0("a0", "a0", 0, -10, 10);
	RooRealVar a1("a1", "a1", 1000, 0, 10000);
	RooPolynomial background("background", "Fundo", mass, RooArgList(a0, a1));
    
	RooRealVar nsig("nsig", "Número de eventos sinal", 1000, 0, 10000);
	RooRealVar nbkg("nbkg", "Número de eventos fundo", 1000, 0, 10000);
	RooAddPdf model("model", "Modelo Sinal + Fundo", RooArgList(signal, background), RooArgList(nsig, nbkg));
    
	model.fitTo(*data);
	RooPlot *frame = mass.frame();
	frame->SetXTitle("Massa (GeV/c^{2})");

	data->plotOn(frame);
	model.plotOn(frame);
	model.paramOn(frame);

	double chi2 = frame->chiSquare();
	int nParams = model.getParameters(*data)->getSize();
	int nPoints = data->numEntries();
	int ndf = nPoints - nParams;

	std::cout << "Chi2/ndf: " << chi2 / ndf << std::endl;

	TCanvas *c1 = new TCanvas("c1", "Ajuste da ressonância J/ψ", 800, 600);
	frame->Draw();

	c1->SaveAs("exercicio3.png");
}
