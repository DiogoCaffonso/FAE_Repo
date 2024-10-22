#include <RooRealVar.h>
#include <RooCBShape.h>
#include <RooDataSet.h>
#include <RooPlot.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TPaveText.h>

void exercicio1() {
	RooRealVar x("x", "x", -10, 10);
	RooRealVar mean("mean", "Mean", 0, -10, 10);
	RooRealVar sigma("sigma", "Sigma", 1, 0.1, 5);
	RooRealVar alpha("alpha", "Alpha", 1.5, 0.1, 5);
	RooRealVar n("n", "n", 2, 0.1, 10);

	RooCBShape crystalBall("crystalBall", "Crystal Ball PDF", x, mean, sigma, alpha, n);

	RooDataSet* data = crystalBall.generate(x, 10000);

	crystalBall.fitTo(*data);

	RooPlot* xframe = x.frame();
	data->plotOn(xframe);
	crystalBall.plotOn(xframe);

	TCanvas* c1 = new TCanvas("c1", "Ajuste Crystal Ball", 800, 600);
	xframe->Draw();

	gStyle->SetOptStat(1111);
	gStyle->SetOptFit(1111);

	TPaveText* paramsText = new TPaveText(0.6, 0.7, 0.9, 0.9, "NDC");
	paramsText->AddText(Form("Mean ajustado = %.3f", mean.getVal()));
	paramsText->AddText(Form("Sigma ajustado = %.3f", sigma.getVal()));
	paramsText->AddText(Form("Alpha ajustado = %.3f", alpha.getVal()));
	paramsText->AddText(Form("n ajustado = %.3f", n.getVal()));
	paramsText->SetFillColor(0);
	paramsText->SetTextSize(0.03);
	paramsText->Draw();

	c1->SaveAs("exercicio1.png");
}
