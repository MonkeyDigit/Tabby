#pragma once
#include <string>
#include "Acquistabile.h"
constexpr int INGRIPPATO = -1;
constexpr int INVASATO = -2;

enum class Attivita { NESSUNA, PARCHEGGIATO, IN_GIRO, INGRIPPATO, INVASATO, SEQUESTRATO, A_SECCO };
enum class TipoPezzo { NESSUNO, MARMITTA, CARBURATORE, PISTONE, FILTRO };

class Pezzo : public Acquistabile {
public:
	Pezzo()
		: Acquistabile{}, m_tipo{ TipoPezzo::NESSUNO }, m_power{ 0 }
	{}

	Pezzo(TipoPezzo tipo, std::string nome, std::string img, long long prezzo, int power)
		: Acquistabile{ nome, "", img, prezzo, CategoriaOggetto::SCOOTER_PART}, m_tipo{tipo}, m_power{ power }
	{}

	TipoPezzo GetTipo() const { return m_tipo; }
	int GetPower() const { return m_power; }
private:
	TipoPezzo m_tipo;
	int m_power;		// Punti che servono a calcolare la velocità dello scooter
};

class Scooter : public Acquistabile {
public:
	Scooter()
		: Acquistabile{}, m_fama{ 0 }, m_stato{ 0 }, m_attivita{ Attivita::NESSUNA }, m_capSerbatoio{ 5.0f }, m_benza{ 0 }, m_velocita{ 0 },
		m_marmitta{}, m_carburatore{}, m_cilindrata{}, m_filtro{}
	{}

	Scooter(std::string nome, std::string img, long long prezzo, int fama, int capSerbatoio, Pezzo marmitta, Pezzo carburatore, Pezzo cilindrata, Pezzo filtro)
		: Acquistabile{nome, "", img, prezzo, CategoriaOggetto::SCOOTER},
		m_fama{ fama }, m_stato{ 100 }, m_velocita{ 0 }, m_capSerbatoio{ (float)capSerbatoio }, m_benza{ 0 },
		m_marmitta{ marmitta }, m_carburatore{ carburatore }, m_cilindrata{ cilindrata }, m_filtro{ filtro }, 
		m_attivita { Attivita::IN_GIRO }
	{
		CalcolaVelocita();
	}

	void IncStato(int punti) {
		if (punti > 0)
		{
			m_stato += punti;
			if (m_stato > 100) m_stato = 100;
		}
	}
	void DecStato(int punti) {
		if (punti > 0)
		{
			m_stato -= punti;
			if (m_stato < 0) m_stato = 0;
		}
	}

	void IncBenza(float litri) {
		if (litri > 0)
		{
			m_benza += litri;
			if (m_benza > m_capSerbatoio) m_benza = m_capSerbatoio;

			if (m_attivita == Attivita::A_SECCO)
			{
				m_attivita = Attivita::IN_GIRO;
				CalcolaVelocita();
			}
		}
	}
	void DecBenza(float litri) {
		if (litri > 0)
		{
			m_benza -= litri;
			if (m_benza <= 0)
			{
				m_benza = 0;
				m_attivita = Attivita::A_SECCO;
			}
		}
	}

	void Ripara() { m_stato = 100; }
	void SetAttivita(Attivita a) { m_attivita = a; }
	
	int GetStato() const { return m_stato; }
	int GetFama() const { return m_fama; }
	int GetVelocita() const { return m_velocita; }
	float GetBenza() const { return m_benza; }
	float GetCapSerbatoio() const { return m_capSerbatoio; }
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

	void InstallaPezzo(const Pezzo& nuovoPezzo) {
		switch (nuovoPezzo.GetTipo()) {
		case TipoPezzo::MARMITTA: m_marmitta = nuovoPezzo; break;
		case TipoPezzo::CARBURATORE: m_carburatore = nuovoPezzo; break;
		case TipoPezzo::PISTONE: m_cilindrata = nuovoPezzo; break;
		case TipoPezzo::FILTRO: m_filtro = nuovoPezzo; break;
		default: break;
		}
		// Ogni volta che cambi un pezzo, ricalcoli la velocità
		CalcolaVelocita();
	}

	// Calcola la velocità dello scooter, secondo il tipo di marmitta, carburatore, ecc...
	void CalcolaVelocita() {
		// INGRIPPATO:	se monti un carburatore piccolo su un cilindro troppo grosso, il motore non riceve abbastanza miscela/olio, si surriscalda e si grippa
		// INVASATO:	Il motore non riesce a bruciare: se monti un carburatore troppo grande su un cilindro piccolo, entra troppa benzina che il motore non riesce a bruciare. Il motorino si "invasa" (si ingolfa/allaga pesantemente) e non parte
		// TABELLA COMBINAZIONI CARBURATORE + CILINDRATA
		static const int TABLE_SPEED[6][6] = {
//  Cilindrata:   <=50      <=70      <=90			  <=120           <=150           >150           // Carburatore
				{ 65,       70,       INGRIPPATO,     INGRIPPATO,     INGRIPPATO,     INGRIPPATO },  // <= 12/10
				{ 70,       80,       95,             INGRIPPATO,     INGRIPPATO,     INGRIPPATO },  // <= 16/16
				{ INVASATO, 90,       100,            115,            INGRIPPATO,     INGRIPPATO },  // <= 19/19
				{ INVASATO, INVASATO, 110,            125,            135,            INGRIPPATO },  // <= 20/20
				{ INVASATO, INVASATO, INVASATO,       130,            150,            INGRIPPATO },  // <= 24/24
				{ INVASATO, INVASATO, INVASATO,       INVASATO,       INVASATO,       250        }   // > 24/24
		};

		// 1. CONVERSIONE AL VOLO: Valori Reali -> Indici Tabella (0-5)
		// Sfrutto un trick ricorsivo dell'operatore ternario perché sono troppo intelligente...
		int valCarb = m_carburatore.GetPower(); // es. 12, 19, 21...
		int idxCarb = (valCarb <= 12) ? 0 :
			(valCarb <= 16) ? 1 :
			(valCarb <= 19) ? 2 :
			(valCarb <= 21) ? 3 :
			(valCarb <= 24) ? 4 : 5;

		int valCil = m_cilindrata.GetPower(); // es. 50, 70, 90...
		int idxCil = (valCil <= 50) ? 0 :
			(valCil <= 70) ? 1 :
			(valCil <= 90) ? 2 :
			(valCil <= 120) ? 3 :
			(valCil <= 150) ? 4 : 5;

		// 2. LOOKUP
		int baseSpeed = TABLE_SPEED[idxCarb][idxCil];

		// 3. CHECK ERRORI (Ingrippato/Invasato)
		if (baseSpeed == INGRIPPATO) { m_attivita = Attivita::INGRIPPATO; m_velocita = 0; return; }
		if (baseSpeed == INVASATO) { m_attivita = Attivita::INVASATO;   m_velocita = 0; return; }

		// Reset stato se risolto
		if (m_attivita == Attivita::INGRIPPATO || m_attivita == Attivita::INVASATO) m_attivita = Attivita::PARCHEGGIATO;

		// 4. SOMMA BONUS
		m_velocita = baseSpeed + m_marmitta.GetPower() + m_filtro.GetPower();
	}

	void SetFama(int fama) {
		m_fama = fama;
		if (m_fama < 0) m_fama = 0;
		else if (m_fama > 100) m_fama = 100;
	}

	void SetBenza(int benza) {
		m_benza = benza;
		if (m_benza < 0) m_benza = 0;
		else if (m_benza > m_capSerbatoio) m_benza = m_capSerbatoio;
	}

	void SetCapSerbatoio(int litri) {
		m_capSerbatoio = litri;
		if (m_capSerbatoio < 0) m_capSerbatoio = 0;
	}

	const Pezzo& GetMarmitta() const { return m_marmitta; }
	const Pezzo& GetCarburatore() const { return m_carburatore; }
	const Pezzo& GetCilindrata() const { return m_cilindrata; }
	const Pezzo& GetFiltro() const { return m_filtro; }

private:
	int m_fama;
	int m_stato;
	int m_velocita;
	float m_capSerbatoio;
	float m_benza;
	Attivita m_attivita;
	// PEZZI
	Pezzo m_marmitta;		// + 0, 5, 10, 15
	Pezzo m_filtro;			// + 0, 5, 10, 15, 20
	Pezzo m_carburatore;	
	Pezzo m_cilindrata;
};