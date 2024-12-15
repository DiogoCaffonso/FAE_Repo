#include <TFile.h>
#include <TTree.h>
#include <TTreeReader.h>
#include <TTreeReaderArray.h>
#include <TCanvas.h>
#include <TH1F.h>
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

    std::vector<double> massas_invariantes_com_corte;
    TH1F* hPT_antes = new TH1F("hPT_antes", "Distribuicao de pT antes do corte", 50, 0, 200);
    TH1F* hETA_antes = new TH1F("hETA_antes", "Distribuicao de eta antes do corte", 50, -3, 3);
    TH1F* hPT_depois = new TH1F("hPT_depois", "Distribuicao de pT depois do corte", 50, 0, 200);
    TH1F* hETA_depois = new TH1F("hETA_depois", "Distribuicao de eta depois do corte", 50, -3, 3);

    int eventos_totais = 0, eventos_filtrados = 0;

    for (const auto& dir : diretorios) {
        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            std::string file_path = entry.path();
            TFile file(file_path.c_str(), "READ");
            if (!file.IsOpen()) continue;

            TTreeReader reader("Events", &file);
            TTreeReaderArray<float> Electron_pt(reader, "Electron_pt");
            TTreeReaderArray<float> Electron_eta(reader, "Electron_eta");
            TTreeReaderArray<float> Electron_phi(reader, "Electron_phi");

            while (reader.Next()) {
                eventos_totais++;

                for (size_t i = 0; i < Electron_pt.GetSize(); ++i) {
                    hPT_antes->Fill(Electron_pt[i]);
                    hETA_antes->Fill(Electron_eta[i]);

                    if (Electron_pt[i] > 10 && std::abs(Electron_eta[i]) < 2.4) { // Cortes em pT > 10 e |eta| < 2.4
                        hPT_depois->Fill(Electron_pt[i]);
                        hETA_depois->Fill(Electron_eta[i]);
                        eventos_filtrados++;
                    }
                }
            }
        }
    }


    TCanvas* c1 = new TCanvas("c1", "Comparacoes", 800, 600);
    hPT_antes->SetLineColor(kRed);
    hPT_antes->Draw();
    hPT_depois->SetLineColor(kBlue);
    hPT_depois->Draw("SAME");
    c1->SaveAs("comparacao_PT.png");

    TCanvas* c2 = new TCanvas("c2", "Comparacao eta", 800, 600);
    hETA_antes->SetLineColor(kRed);
    hETA_antes->Draw();
    hETA_depois->SetLineColor(kBlue);
    hETA_depois->Draw("SAME");
    c2->SaveAs("comparacao_ETA.png");

    std::cout << "Numero de eventos totais: " << eventos_totais << std::endl;
    std::cout << "Numero de eventos apos cortes: " << eventos_filtrados << std::endl;

    delete hPT_antes;
    delete hPT_depois;
    delete hETA_antes;
    delete hETA_depois;
    delete c1;
    delete c2;
}

analise();
