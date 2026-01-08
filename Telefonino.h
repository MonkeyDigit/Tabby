#pragma once
#include <string>
#include <vector>

class Abbonamento {
public:
	Abbonamento() {};
	Abbonamento(std::string nome, long long attivazione, const std::vector<long long>& ricariche, std::string icon);
	std::string GetNome() const { return m_nome; }
	long long GetCostoAttivazione() const { return m_attivazione; }
	const std::vector<long long>& GetRicariche() const { return m_ricariche; }
private:
	std::string m_nome;
	long long m_attivazione;
	std::vector<long long> m_ricariche;
	std::string m_icon;
};

class Telefono
{
public:
	Telefono() {};
	Telefono(std::string nome, std::string desc, long long prezzo, int fama, int stato, long long credito, const Abbonamento& abb, std::string icon);
	std::string GetNome() const { return m_nome; }
	std::string GetDesc() const { return m_desc; }
	long long GetPrezzo() const { return m_prezzo; }
	int GetFama() const { return m_fama; }
	int GetStato() const { return m_stato; }
	long long GetCredito() const { return m_credito; }
	Abbonamento& GetAbbonamento() { return m_abb; }

	void SetAbbonamento(const Abbonamento& abb) { m_abb = abb; }
	void IncStato(int punti);
	void DecStato(int punti);

	void IncCredito(int punti);
	void DecCredito(int punti);

	bool HaSim() const { return !m_abb.GetNome().empty(); }
private:
	std::string m_nome;
	std::string m_desc;
	long long m_prezzo;
	int m_fama;	// figosità
	int m_stato;
	long long m_credito;
	Abbonamento m_abb;
	std::string m_icon;
};