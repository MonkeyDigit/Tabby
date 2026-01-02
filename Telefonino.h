#pragma once
#include <string>

class OperatoreTel {
public:
	int GetCredito() const { return m_credito; };
	bool AbbonamentoAttivo() const { return m_abbonamento; };
	int GetFama() const { return m_fama; };
	int GetPrezzo() const { return m_prezzo; };

	void IncCredito(int punti);
	void DecCredito(int punti);
private:
	std::string m_nome;
	bool m_abbonamento;	// 0 = Ricarica, 1 = Abbonamento
	int m_dualOnly;		// Dual band only?
	int m_credito;
	int m_fama;
	int m_prezzo;
};

class Telefono
{
public:
	std::string GetNome() const { return m_nome; };
	int GetPrezzo() const { return m_prezzo; };
	int GetFama() const { return m_fama; };
	int GetStato() const { return m_stato; };

	void IncStato(int punti);
	void DecStato(int punti);
private:
	std::string m_nome;
	int m_prezzo;
	int m_dual;	// Dual band?
	int m_fama;	// figosità
	int m_stato;
};

