#pragma once
#include <string>
#include "Acquistabile.h"
// TODO: PUO' ESSERE SIA PARCHEGGIATO CHE A SECCO
enum class Attivita { NESSUNA, PARCHEGGIATO, IN_GIRO, INGRIPPATO, INVASATO, SEQUESTRATO, A_SECCO };
enum class TipoPezzo { NESSUNO, MARMITTA, CARBURATORE, PISTONE, FILTRO };

class Pezzo : public Acquistabile {
public:
	Pezzo()
		: Acquistabile{}, m_tipo{ TipoPezzo::NESSUNO }
	{}

	Pezzo(TipoPezzo tipo, std::string nome, std::string img, long long prezzo)
		: Acquistabile{ nome, "", img, prezzo, CategoriaOggetto::SCOOTER_PART}, m_tipo{tipo}
	{}

	TipoPezzo GetTipo() const { return m_tipo; }
private:	// TODO: COMPLETA
	TipoPezzo m_tipo;
};

class Scooter : public Acquistabile {
public:
	Scooter()
		: Acquistabile{}, m_fama{ 0 }, m_stato{ 0 }, m_attivita{Attivita::NESSUNA}, m_benza{0.f}, m_velocita{0}	// TODO: INIZiALIZZA PEZZI
	{}

	// TODO: CLASSE MARMITTA, CARBURATORE, CILINDRATA. FILTRO
	// TODO: COMPLETA CONSTRUCTOR
	Scooter(std::string nome, std::string img, long long prezzo, int fama, int stato, int velocita, float benza)
		: Acquistabile{nome, "", img, prezzo, CategoriaOggetto::SCOOTER},
		m_fama{fama}, m_stato{stato}, m_velocita{velocita}, m_benza{benza}, m_attivita{Attivita::IN_GIRO}
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

	void SetBenza(float litri) { if (litri > 0) m_benza = litri; }
	void IncBenza(float litri) {
		if (litri > 0) m_benza += litri;
	}
	void DecBenza(float litri) {
		if (litri > 0)
		{
			m_benza -= litri;
			if (m_benza < 0)
			{
				m_benza = 0;
			}
		}
	}

	void Azzera() { *this = Scooter{}; }
	void Ripara() { m_stato = 100; }
	void SetAttivita(Attivita a) { m_attivita = a; }
	
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

	Pezzo& GetMarmitta() { return m_marmitta; }
	Pezzo& GetCarburatore() { return m_carburatore; }
	Pezzo& GetCilindrata() { return m_cilindrata; }
	Pezzo& GetFiltro() { return m_filtro; }

private:
	int m_fama;
	int m_stato;
	int m_velocita;
	float m_benza;
	Attivita m_attivita;
	// PEZZI
	Pezzo m_marmitta;
	Pezzo m_carburatore;
	Pezzo m_cilindrata;
	Pezzo m_filtro;
};