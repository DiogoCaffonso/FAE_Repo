#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TF1.h>
#include <TMath.h>
#include <iostream>
#include <vector>
#include <filesystem>

double calcular_massa_invariante(const std::vector<float>& pt, const std::vector<float>& eta, const std::vector<float>& phi) {
    if (pt.size() == 2) {
        return sqrt(2 * pt[0] * pt[1] * (TMath::CosH(eta[0] - eta[1]) - TMath::Cos(phi[0] - phi[1])));
    }
    return -1.0;
}

void analise() {
    std::vector<std::string> diretorios = {
        "/opendata/eos/opendata/cms/Run2016G/DoubleEG/NANOAOD/UL2016_MiniAODv2_NanoAODv9-v1/100000",
        "/opendata/eos/opendata/cms/Run2016G/DoubleEG/NANOAOD/UL2016_MiniAODv2_NanoAODv9-v1/1010000",
        "/opendata/eos/opendata/cms/Run2016G/DoubleEG/NANOAOD/UL2016_MiniAODv2_NanoAODv9-v1/250000"
    };

    std::vector<double> e_massas_invariantes, m_massas_invariantes, t_massas_invariantes;

    for (const auto& dir : diretorios) {
        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            std::string file_path = entry.path();
            TFile file(file_path.c_str(), "READ");
            if (!file.IsOpen()) continue;

            TTreeReader reader("Events", &file);
            TTreeReaderArray<float> Electron_pt(reader, "Electron_pt");
            TTreeReaderArray<float> Electron_eta(reader, "Electron_eta");
            TTreeReaderArray<float> Electron_phi(reader, "Electron_phi");
            TTreeReaderArray<float> Muon_pt(reader, "Muon_pt");
            TTreeReaderArray<float> Muon_eta(reader, "Muon_eta");
            TTreeReaderArray<float> Muon_phi(reader, "Muon_phi");
            TTreeReaderArray<float> Tau_pt(reader, "Tau_pt");
            TTreeReaderArray<float> Tau_eta(reader, "Tau_eta");
            TTreeReaderArray<float> Tau_phi(reader, "Tau_phi");

            while (reader.Next()) {
    		for (size_t i = 0; i < Electron_pt.GetSize(); ++i) {
		    for (size_t j = i + 1; j < Electron_pt.GetSize(); ++j) {
			if (Electron_pt[i] > 2 && Electron_pt[j] > 2 && abs(Electron_eta[i]) > 2.4 && abs(Electron_eta[j]) > 2.4){
			    e_massas_invariantes.push_back(
   	                    calcular_massa_invariante(
        	            {Electron_pt[i], Electron_pt[j]},
                     	    {Electron_eta[i], Electron_eta[j]},
   	                    {Electron_phi[i], Electron_phi[j]}
            	            )
		            );
			}
		    }
	        }
		for (size_t i = 0; i < Muon_pt.GetSize(); ++i) {
     		    for (size_t j = i + 1; j < Muon_pt.GetSize(); ++j) {
			if (Muon_pt[i] > 2 && Muon_pt[j] > 2 && abs(Muon_eta[i]) > 2.4 && abs(Muon_eta[j]) > 2.4){
		            m_massas_invariantes.push_back(
                  	    calcular_massa_invariante(
 	                    {Muon_pt[i], Muon_pt[j]},
          	            {Muon_eta[i], Muon_eta[j]},
                 	    {Muon_phi[i], Muon_phi[j]}
	                    )
		            );
			}
		    }
		}
		for (size_t i = 0; i < Tau_pt.GetSize(); ++i) {
		    for (size_t j = i + 1; j < Tau_pt.GetSize(); ++j) {
			if (Tau_pt[i] > 2 && Tau_pt[j] > 2 && abs(Tau_eta[i]) > 2.4 && abs(Tau_eta[j]) > 2.4){
	        	    t_massas_invariantes.push_back(
	   	    	    calcular_massa_invariante(
	                    {Tau_pt[i], Tau_pt[j]},
        	            {Tau_eta[i], Tau_eta[j]},
                	    {Tau_phi[i], Tau_phi[j]}
                	    )
		            );
			}
		    }
		}
            }
        }
    }

    TCanvas* canvas = new TCanvas("canvas", "Distribuições de Massas Invariantes", 800, 600);
    TH1F* hEletron = new TH1F("hEletron", "", 50, 0, 200);
    TH1F* hMuon = new TH1F("hMuon", "", 50, 0, 200);
    TH1F* hTau = new TH1F("hTau", "", 50, 0, 200);

    for (const auto& massa : e_massas_invariantes) if (massa >= 0) hEletron->Fill(massa);
    for (const auto& massa : m_massas_invariantes) if (massa >= 0) hMuon->Fill(massa);
    for (const auto& massa : t_massas_invariantes) if (massa >= 0) hTau->Fill(massa);

    TF1* gaussEletron = new TF1("gaussEletron", "[0]*exp(-0.5*((x-[1])/[2])^2) + [3]", 80, 100);
    gaussEletron->SetParameters(2000, 90, 15, 1000);
    gaussEletron->SetLineColor(0);
    hEletron->Fit(gaussEletron, "R");
    double massaEletron = gaussEletron->GetParameter(1);
    double erroEletron = gaussEletron->GetParError(1);
    std::cout << "Massa do sinal (Eletron): " << massaEletron << " GeV/c^2" << std::endl;
    std::cout << "Incerteza estatística (Eletron): " << erroEletron << " GeV/c^2" << std::endl;

    TF1* gaussTau = new TF1("gaussTau", "[0]*exp(-0.5*((x-[1])/[2])^2) + [3]", 80, 100);
    gaussTau->SetParameters(800, 90, 15, 200);
    gaussTau->SetLineColor(0);
    hTau->Fit(gaussTau, "R");
    double massaTau = gaussTau->GetParameter(1);
    double erroTau = gaussTau->GetParError(1);
    std::cout << "Massa do sinal (Tau): " << massaTau << " GeV/c^2" << std::endl;
    std::cout << "Incerteza estatística (Tau): " << erroTau << " GeV/c^2" << std::endl;

    hEletron->SetLineColor(kBlue);
    hEletron->SetStats(0);
    hEletron->GetXaxis()->SetTitle("e_mass (GeV/c^{2})");
    hEletron->GetYaxis()->SetTitle("Eventos");
    hEletron->Draw();
    canvas->SaveAs("e_massa_invariante.png");

    hMuon->SetLineColor(kBlue);
    hMuon->SetStats(0);
    hMuon->GetXaxis()->SetTitle("#mu_mass (GeV/c^{2})");
    hMuon->GetYaxis()->SetTitle("Eventos");
    hMuon->Draw();
    canvas->SaveAs("m_massa_invariante.png");

    hTau->SetLineColor(kBlue);
    hTau->SetStats(0);
    hTau->GetXaxis()->SetTitle("#tau_mass (GeV/c^{2})");
    hTau->GetYaxis()->SetTitle("Eventos");
    hTau->Draw();
    canvas->SaveAs("t_massa_invariante.png");

    delete hEletron;
    delete hMuon;
    delete hTau;
    delete canvas;
}

analise();
