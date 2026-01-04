#pragma once
#include <random>
#include <vector>
#include "TabbyGuy.h"
#include "Chrono.h"
// Per attivare il debug
#define TABBY_DEBUG
constexpr float CAMBIO_EURO_LIRA = 1936.27;
constexpr int GIORNO_STIPENDIO = 27;

// TODO: Data del compleanno

// Sotto il cofano, i soldi di tabby e in generale il sistema monetario del gioco, viene gestito di base come euro, ma in base alla data sono messe a disposizione delle funzione per effettuare la conversione in lire, che fanno da "maschera"
enum class Valuta { LIRE, EURO };

enum class TipoEvento {
	NESSUNO, 
	INFO,		// Solo tasto OK
	SCELTA		// Tasti sì/no
};

struct EventoDati {
	EventoDati();
	EventoDati(TipoEvento tipo, int id, std::string titolo, std::string testo, std::string img);
	TipoEvento m_tipo = TipoEvento::NESSUNO;
	int m_idEvento = -1;
	std::string m_titolo;
	std::string m_testo;
	std::string m_immagine;	// Nome del file immagine
};

enum class TipoGiorno {
	NORMALE,        // Scuola aperta, Negozi aperti
	VACANZA_SCUOLA, // Scuola chiusa, Negozi aperti - Es. Estate
	FESTIVO         // Tutto chiuso - Es. Domenica o Natale
};

struct FestaFissa {
	int m_giorno;
	int m_mese;
	std::string m_nome;
	std::string m_messaggio;
};

class TabbyGame
{
public:
	TabbyGame();

	// GETTER FUNCTIONS
	Valuta GetValutaCorrente() const { return m_valutaCorrente; };
	TabbyGuy& GetTabbyGuy() { return m_tabbyGuy; };
	Chrono::Date& GetDate() { return m_date; };

	// LOGICA EVENTI
	// Restituisce true se c'era un evento (e lo mette in outEvento), false se la coda eventi è vuota
	bool PollEvento(EventoDati& outEvento);
	void ApplicaScelta(int idEvento, bool sceltaYes);
	// TODO: BOOL / CODICE DI INFORMAZIONE / EVENTO CON MESSAGGIO
	void AzioneStudia(int materiaIndex);
	void AzioneMinaccia(int materiaIndex);
	void AzioneCorrompi(int materiaIndex);

	// Stringa formattata (es. "1.000 L." o "5 €")
	std::string GetSoldiStr(long long valoreBase) const;

	// GENERATORE RANDOM
	int GenRandomInt(int min, int max);

private:
	TabbyGuy m_tabbyGuy;
	Chrono::Date m_date;
	Valuta m_valutaCorrente;
	int m_coolDownPestaggio;
	std::vector<EventoDati> m_codaEventi;
	TipoGiorno m_tipoGiorno;
	// IL MOTORE DI NUMERI CASUALI
	// Si chiama Mersenne Twister (mt19937), è veloce e affidabile
	std::mt19937 m_rng;

	// Evento
	void PushEvento(const EventoDati& e) { m_codaEventi.push_back(e); };

	// Funzioni di gestione giornaliera
	void NuovoGiorno();
	void AvanzaCalendario();
	void GestioneConsumi();     // Sigarette, telefono, abbonamento
	void GestioneRelazioni();   // Tipa, amici, famiglia
	void GestioneLavoro();      // Licenziamento, impegno
	void GestioneEconomia();    // Paghetta, stipendio
	void GestioneEventiCasuali(); // Il generatore di caos

	// LOGICA MONETARIA
	// Controlla se è il 2002...
	void CheckCambioValuta();
	// Prende il valore "grezzo" (base Euro) e lo converte in quello che l'utente deve vedere (Lire o Euro)
	long long ConvertiValuta(long long valoreBase) const;
};