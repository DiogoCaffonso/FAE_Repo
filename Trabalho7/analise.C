#include <iostream>
#include <fstream>
#include <vector>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLorentzVector.h>

void analise() {
	std::string fileList = "/afs/cern.ch/user/d/dgomessa/Temporary/FAE_Repo/Trabalho7/data.txt";
	TChain chain("Events");

	std::ifstream inputFile(fileList);
	if (!inputFile.is_open()) {
		std::cerr << "Erro: não foi possível abrir o arquivo " << fileList << std::endl;
		return;
	}

	std::string line;
	while (std::getline(inputFile, line)) {
		if (!line.empty()) {
			chain.Add(line.c_str());
		}
	}
	inputFile.close();

	TH1F* h_pT_before = new TH1F("h_pT_before", "Distribuicao de pT antes dos cortes; pT [GeV/c]; Eventos", 50, 0, 100);
	TH1F* h_eta_before = new TH1F("h_eta_before", "Distribuicao de eta antes dos cortes; #eta; Eventos", 50, -3, 3);
	TH1F* h_pT_after = new TH1F("h_pT_after", "Distribuicao de pT depois dos cortes; pT [GeV/c]; Eventos", 50, 0, 100);
	TH1F* h_eta_after = new TH1F("h_eta_after", "Distribuicao de eta depois dos cortes; #eta; Eventos", 50, -3, 3);

	float pt, eta;
	chain.SetBranchAddress("Muon_pt", &pt);
	chain.SetBranchAddress("Muon_eta", &eta);
	
	
	Long64_t nEntries = chain.GetEntries();

	for (Long64_t i = 0; i < nEntries; ++i) {
		chain.GetEntry(i);
		h_pT_before->Fill(pt);
		h_eta_before->Fill(eta);
		if (pt > 20.0 && fabs(eta) < 2.4) {
			h_pT_after->Fill(pt);
			h_eta_after->Fill(eta);
		}
	}

	TCanvas* c1 = new TCanvas("c1", "pT antes e depois dos cortes", 800, 600);
	h_pT_before->SetLineColor(kBlue);
        h_pT_after->SetLineColor(kRed);
	h_pT_before->Draw();
	h_pT_after->Draw("SAME");
	c1->BuildLegend();
	c1->SaveAs("pT_comparacao.png");

	TCanvas* c2 = new TCanvas("c2", "Eta antes e depois dos cortes", 800, 600);
	h_eta_before->SetLineColor(kBlue);
	h_eta_after->SetLineColor(kRed);
	h_eta_before->Draw();
	h_eta_after->Draw("SAME");
	c2->BuildLegend();
	c2->SaveAs("eta_comparacao.png");

	delete h_pT_before;
	delete h_eta_before;
        delete h_pT_after;
	delete h_eta_after;
	delete c1;
	delete c2;
}


