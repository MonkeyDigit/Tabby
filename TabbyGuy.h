#pragma once
#include <string>
#include "Scooter.h"
#include "Tipa.h"
#include "Chrono.h"
#include "Scuola.h"

struct CartaIdentita {
	std::string m_nome;
	std::string m_cognome;
	Chrono::Date m_dataNascita;
	std::string m_attoNascita;
	std::string m_luogoNascita;
	std::string m_provinciaNascita;
	std::string m_cittadinanza;
	std::string m_residenza;
	std::string m_indirizzo;
	std::string m_statoCivile;
	std::string m_professione;
};

class TabbyGuy {
public:
	TabbyGuy();		// Constructor

	// Restituisce true se l'acquisto avviene con successo
	bool SpendiSoldi(int importo);
	void GuadagnaSoldi(int importo);

	CartaIdentita* GetID() { return &m_identita; };
	long long GetSoldi() const { return m_soldi; };
	int GetRep() const { return m_rep; };
	int GetFigo() const { return m_figo; };
	int GetStudio() const { return m_studio; };
	Scuola* GetScuola() { return &m_scuola; };
	Scooter* GetScooter() { return &m_scooter; };
	Tipa* GetTipa() { return &m_tipa; };
	int GetPaghetta() const { return m_paghetta; };
	int GetGiubotto() const { return m_giubotto; };
	int GetPantaloni() const { return m_pantaloni; };
	int GetScarpe() const { return m_scarpe; };
	int GetSesso() const { return m_sesso; };


private:
	CartaIdentita m_identita;
	long long m_soldi;			// intero a 64 bit (max 9 miliardi DI miliardi): il tabbozzo è rikko...
	int m_rep;
	int m_figo;
	int m_studio;
	Scuola m_scuola;
	// Qua potevo usare dei puntatori inizializzando lo scooter e la tipa a NULL, ma non voglio fare un puttanaio con destructor e magagne dei puntatori
	Scooter m_scooter;
	bool m_haScooter;
	Tipa m_tipa;
	bool m_haTipa;
	int m_paghetta;
	int m_testa;	// abbronzatura
	int m_giubotto;
	int m_pantaloni;
	int m_scarpe;
	int m_sesso;	// TODO: FAI MACRO
};