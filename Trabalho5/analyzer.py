import uproot
import numpy as np
import matplotlib.pyplot as plt

file = uproot.open("root://eospublic.cern.ch//eos/opendata/cms/mc/RunIISummer20UL16NanoAODv9/ZZTo4L_TuneCP5_13TeV_powheg_pythia8/NANOAODSIM/106X_mcRun2_asymptotic_v17-v1/2430000/051E9D22-4F30-8E49-8477-644E65768282.root")
tree = file["Events"]

ePt = tree["Electron_pt"].array()
eEta = tree["Electron_eta"].array()
ePhi = tree["Electron_phi"].array()

muPt = tree["Muon_pt"].array()
muEta = tree["Muon_eta"].array()
muPhi = tree["Muon_phi"].array()

tPt = tree["Tau_pt"].array()
tEta = tree["Tau_eta"].array()
tPhi = tree["Tau_phi"].array()

def calcular_massa_invariante(pt1, pt2, eta1, eta2, phi1, phi2):
	return np.sqrt(2 * pt1 * pt2 * (np.cosh(eta1 - eta2) - np.cos(phi1 - phi2)))

e_massas_invariantes = []
m_massas_invariantes = []
t_massas_invariantes = []

for i in range(len(ePt)):
	if len(ePt[i]) < 2:
		continue
	indices = np.argsort(ePt[i])[::-1][:2]
	ePt1, ePt2 = ePt[i][indices[0]], ePt[i][indices[1]]
	eEta1, eEta2 = eEta[i][indices[0]], eEta[i][indices[1]]
	ePhi1, ePhi2 = ePhi[i][indices[0]], ePhi[i][indices[1]]
	e_massa_invariante = calcular_massa_invariante(ePt1, ePt2, eEta1, eEta2, ePhi1, ePhi2)
	e_massas_invariantes.append(e_massa_invariante)

for i in range(len(muPt)):
	if len(muPt[i]) < 2:
		continue
	indices = np.argsort(muPt[i])[::-1][:2]
	muPt1, muPt2 = muPt[i][indices[0]], muPt[i][indices[1]]
	muEta1, muEta2 = muEta[i][indices[0]], muEta[i][indices[1]]
	muPhi1, muPhi2 = muPhi[i][indices[0]], muPhi[i][indices[1]]
	m_massa_invariante = calcular_massa_invariante(muPt1, muPt2, muEta1, muEta2, muPhi1, muPhi2)
	m_massas_invariantes.append(m_massa_invariante)

for i in range(len(tPt)):
	if len(tPt[i]) < 2:
		continue
	indices = np.argsort(tPt[i])[::-1][:2]
	tPt1, tPt2 = tPt[i][indices[0]], tPt[i][indices[1]]
	tEta1, tEta2 = tEta[i][indices[0]], tEta[i][indices[1]]
	tPhi1, tPhi2 = tPhi[i][indices[0]], tPhi[i][indices[1]]
	t_massa_invariante = calcular_massa_invariante(tPt1, tPt2, tEta1, tEta2, tPhi1, tPhi2)
	t_massas_invariantes.append(t_massa_invariante)



e_massas_invariantes = np.array(e_massas_invariantes)
m_massas_invariantes = np.array(m_massas_invariantes)
t_massas_invariantes = np.array(t_massas_invariantes)

plt.hist(e_massas_invariantes, bins=50, color='blue', alpha=0.7)
plt.xlabel("Massa Invariante do Eletron (GeV/$c^{2}$)")
plt.ylabel("Eventos")
plt.title("Distribuição da Massa Invariante dos Dois Eletrons de Maior $p_T$")
plt.savefig("e_massa_invariante.png")
plt.show()

plt.hist(m_massas_invariantes, bins=50, color='blue', alpha=0.7)
plt.xlabel("Massa Invariante do Muon (GeV/$c^{2}$)")
plt.ylabel("Eventos")
plt.title("Distribuição da Massa Invariante dos Dois Muons de Maior $p_T$")
plt.savefig("m_massa_invariante.png")
plt.show()

plt.hist(t_massas_invariantes, bins=50, color='blue', alpha=0.7)
plt.xlabel("Massa Invariante do Tau (GeV/$c^{2}$)")
plt.ylabel("Eventos")
plt.title("Distribuição da Massa Invariante dos DoiTaus de Maior $p_T$")
plt.savefig("t_massa_invariante.png")
plt.show()

