#pragma once
#include <random>
#include <vector>
#include "TabbyGuy.h"
#include "Chrono.h"
// Per attivare il debug
#define TABBY_DEBUG
constexpr float CAMBIO_EURO_LIRA = 1936.27;

/* TODO:
giorno vacanza
anno bisesto
data scadenza palestra
data del compleanno
attesa prima di riavere soldi
fortuna
numeroditta
impegno
giorni di lavoro -> serve a calcolare lo stipendio solo per il primo mese...
stipendio
tempo trascorso dal pestaggio

*/

// Sotto il cofano, i soldi di tabby e in generale il sistema monetario del gioco, viene gestito di base come euro, ma in base alla data sono messe a disposizione delle funzione per effettuare la conversione in lire, che fanno da "maschera"
enum class Valuta { LIRE, EURO };

enum class TipoEvento {
	NESSUNO, 
	INFO,		// Solo tasto OK
	SCELTA		// Tasti sì/no
};

struct EventoDati {
	EventoDati() {}	// Constructor
	TipoEvento tipo = TipoEvento::NESSUNO;
	int idEvento = -1;
	std::string titolo;
	std::string testo;
	std::string immagine;	// Nome del file immagine
};

class TabbyGame
{
public:
	TabbyGame();

	TabbyGuy& GetTabbyGuy() { return m_tabbyGuy; };
	Chrono::Date& GetDate() { return m_date; };
	Valuta GetValutaCorrente() const { return m_valutaCorrente; };
	int GenRandomInt(int min, int max);

	// TODO: fai direttamente una funzione di gestione del tempo, in cui dentro metti cambio valuta
	// TODO: implementa idevento
	bool CheckVacanza();
	// Ci sono eventi in coda da processare?
	bool NuoviEventi() const { return (m_eventQueue.size() > 0 ? true : false); };
	void ApplicaScelta(int idEvento, bool sceltaYes);

	// LOGICA MONETARIA
	// Controlla se è il 2002...
	void CheckCambioValuta();
	// Prende il valore "grezzo" (base Euro) e lo converte in quello che l'utente deve vedere (Lire o Euro)
	long long ConvertiValuta(long long valoreBase) const;
	// Stringa formattata (es. "1.000 L." o "5 €")
	std::string GetSoldiStr(long long valoreBase) const;
	void ProssimoGiorno();

	// Gli eventi sono pubblici
	std::vector<EventoDati> m_eventQueue;

private:
	TabbyGuy m_tabbyGuy;
	Chrono::Date m_date;
	Valuta m_valutaCorrente;
	int m_CoolDownPestaggio;


	// IL MOTORE DI NUMERI CASUALI
	// Si chiama Mersenne Twister (mt19937), è veloce e affidabile
	std::mt19937 m_rng;
};