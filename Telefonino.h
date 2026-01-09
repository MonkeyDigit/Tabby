#pragma once
#include <string>
#include <vector>
#include "Acquistabile.h"

class Abbonamento {
public:
	Abbonamento() : m_nome{ "" }, m_img{ "" }, m_attivazione { 0 }
	{}

	Abbonamento(std::string nome, std::string img, long long attivazione, const std::vector<long long>& ricariche)
		: m_nome{ nome }, m_img{ img }, m_attivazione { attivazione }, m_ricariche{ ricariche }
	{}

	std::string GetNome() const { return m_nome; }
	std::string GetImgStr() const { return m_img; }
	long long GetCostoAttivazione() const { return m_attivazione; }
	const std::vector<long long>& GetRicariche() const { return m_ricariche; }
private:
	std::string m_nome;
	std::string m_img;
	long long m_attivazione;
	std::vector<long long> m_ricariche;
};

class Telefono : public Acquistabile {
public:
	Telefono() : Acquistabile{}, m_fama{ 0 }, m_stato{ 0 }, m_credito{ 0 }, m_abb{}
	{}

	Telefono(std::string nome, std::string desc, std::string img, long long prezzo, int fama, int stato, long long credito, const Abbonamento& abb)
		: Acquistabile{ nome, desc, img, prezzo, CategoriaOggetto::TELEFONO },
		m_fama{fama}, m_stato{stato}, m_credito{credito}, m_abb{abb}
	{}

	int GetFama() const { return m_fama; }
	int GetStato() const { return m_stato; }
	long long GetCredito() const { return m_credito; }
	Abbonamento& GetAbbonamento() { return m_abb; }

	void IncStato(int punti) {
		if (punti > 0) m_stato += punti;
	}
	void DecStato(int punti) {
		if (punti > 0)
		{
			m_stato -= punti;
			if (m_stato < 0) m_stato = 0;
		}
	}

	void IncCredito(int punti) {
		if (punti > 0) m_credito += punti;
	}
	void DecCredito(int punti) {
		if (punti > 0)
		{
			m_credito -= punti;
			if (m_credito < 0) m_credito = 0;
		}
	}

	bool HaSim() const { return !m_abb.GetNome().empty(); }

	void Azzera() {
		*this = Telefono{};
	}

private:
	int m_fama;
	int m_stato;
	long long m_credito;
	Abbonamento m_abb;
};