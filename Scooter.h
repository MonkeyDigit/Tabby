#pragma once
#include <string>

enum class Attivita { NESSUNA, PARCHEGGIATO, IN_GIRO };

class Scooter {
public:
	Scooter();

	// MODIFICHE
	void IncStato(int punti);
	void DecStato(int punti);
	void IncBenzina(int litri);
	void DecBenzina(int litri);
	// TODO: salute piena o -1??
	void Reset();
	
	std::string GetNome() const { return m_nome; };
	int GetVelocita() const { return m_velocita; };
	int GetCilindrata() const { return m_cilindrata; };
	int GetEfficienza() const { return m_efficienza; };
	float GetBenza() const { return m_benzina; };
	int GetStato() const { return m_stato; };
	int GetFama() const { return m_fama; };
	int GetMarmitta() const { return m_marmitta; };
	int GetCarburatore() const { return m_carburatore; };
	int GetFiltro() const { return m_filtro; };
	Attivita GetAttivita() const { return m_attivita; };
	std::string GetAttivitaStr(bool tolower) const;
	int GetPrezzo() const { return m_prezzo; };

private:
	std::string m_nome;
	int m_velocita;
	int m_cilindrata;
	int m_efficienza;
	float m_benzina;
	int m_stato;
	int m_fama;
	int m_marmitta;
	int m_carburatore;
	int m_filtro;
	Attivita m_attivita;
	int m_prezzo;
};