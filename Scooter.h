#pragma once
#include <string>
#include "Acquistabile.h"

enum class Attivita { NESSUNA, PARCHEGGIATO, IN_GIRO, INGRIPPATO, INVASATO, SEQUESTRATO, A_SECCO };

class Scooter : public Acquistabile {
public:
	Scooter()
		: Acquistabile{} 
	{ Azzera(); }

	// TODO: CLASSE MARMITTA, CARBURATORE, CILINDRATA. FILTRO
	// TODO: COMPLETA CONSTRUCTOR
	Scooter(std::string nome, std::string desc, std::string img, long long prezzo, int fama, int stato, int velocita, float benza)
		: Acquistabile{nome, desc, img, prezzo, CategoriaOggetto::SCOOTER },
		m_fama{fama}, m_stato{stato}, m_velocita{velocita}, m_benza{benza}
	{}

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

	void IncBenza(int litri) {
		if (litri > 0) m_benza += litri;
	}
	void DecBenza(int litri) {
		if (litri > 0)
		{
			m_benza -= litri;
			if (m_benza < 0) m_benza = 0;
		}
	}

	void Azzera()
	{
		// TODO: QUESTO FUNZIONA?
		Acquistabile();
		m_stato = 0;
		m_velocita = 0;
		m_fama = 0;
		m_prezzo = 0;
		m_attivita = Attivita::NESSUNA;
	}
	
	int GetStato() const { return m_stato; }
	int GetFama() const { return m_fama; }
	int GetVelocita() const { return m_velocita; }
	float GetBenza() const { return m_benza; }
	Attivita GetAttivita() const { return m_attivita; }

	std::string GetAttivitaStr(bool tolower) const
	{
		std::string s{};
		switch (m_attivita)
		{
		case Attivita::NESSUNA:
			s = "Nessuna";
			break;
		case Attivita::PARCHEGGIATO:
			s = "Parcheggiato";
			break;
		case Attivita::IN_GIRO:
			s = "In giro";
			break;
		case Attivita::INGRIPPATO:
			s = "Ingrippato";
			break;
		case Attivita::INVASATO:
			s = "Invasato";
			break;
		case Attivita::SEQUESTRATO:
			s = "Sequestrato";
			break;
		case Attivita::A_SECCO:
			s = "A secco";
		}

		if (tolower)
			s[0] = std::tolower(s[0]);

		return s;
	}


private:
	int m_fama;
	int m_stato;
	int m_velocita;
	float m_benza;
	Attivita m_attivita;
};