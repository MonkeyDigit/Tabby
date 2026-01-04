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
    TipoGiorno GetTipoGiorno() const { return m_tipoGiorno; };
	// LOGICA EVENTI
	// Restituisce true se c'era un evento (e lo mette in outEvento), false se la coda eventi è vuota
	bool PollEvento(EventoDati& outEvento);
	void ApplicaScelta(int idEvento, bool sceltaYes);
	// TODO: BOOL / CODICE DI INFORMAZIONE / EVENTO CON MESSAGGIO
    // Scuola
    bool TriggerScuola();
	void AzioneStudia(int materiaIndex);
	void AzioneMinaccia(int materiaIndex);
	void AzioneCorrompi(int materiaIndex);
    // Compagnia
    void AzioneGara();
    void AzioneEsci();
    void AzioneChiama();

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


// ------------------ TESTI MESSAGGI POPUP VARIABILI ---------------------------------------------------------------------
const std::vector<FestaFissa> feste = {
    {1, 1, "Capodanno", "Oggi è Capodanno!"},
    {6, 1, "Epifania", "Tutte le feste si porta via..."},
    {25, 4, "Anniversario Liberazione", "Oggi mi sento liberato"},
    {1, 5, "Festa dei Lavoratori", "Nonostante nella tua vita tu non faccia nulla, oggi fai festa anche tu..."},
    {15, 8, "Ferragosto", "Tutti al mare!"},
    {1, 11, "Tutti i Santi", "Figata, oggi è vacanza..."},
    {7, 12, "Sant'Ambrogio", "Visto che siamo a Milano, oggi facciamo festa"},
    {8, 12, "Immacolata Concezione", "Oggi è festa"},
    {25, 12, "Natale", "Buon Natale !!!"},
    {26, 12, "Santo Stefano", "Buon Santo Stefano..."}
};

static const std::vector<std::string> frasiMetallari = {
    "Cerchi la rissa con un metallaro che passa in {LUOGO} e lui ti spacca tutte le ossa.",
    "Incontri un metallaro in {LUOGO}, lo minacci e lui ti spacca tutte le ossa.",
    "Insulti un gruppo di metallari che passano in {LUOGO} e uno di questi ti spacca le ossa.",
    "Mentre cammini per {LUOGO} cerchi la rissa con un metallaro e lui ti spacca tutte le ossa.",
    "Cerchi la rissa con un metallaro e lui ti spacca tutte le ossa."
};
static const std::vector<std::string> frasiFortuna = {
    "Mentre cammini per il quartiere con la tua compagnia, un piccione ti caga in testa...",
    "Cadi nel laghetto di Parco Sempione.",
    "Mentre cammini per il quartiere con la tua compagnia, un cane ti piscia su una gamba...",
    "Un attacco di diarrea ti costringe a restare a casa.",
    "Bevi due bicchieri di birra e vomiti per tutta la sera.",
    "Sei seduto al bar con i tuoi amici, passa una ragazza e fai dei commenti... Poi si gira e ti accorgi che è un uomo.",
    "Sei fermo al semaforo, un TIR ti passa vicino e solleva un ondata di fango.",
    "Ti perdi e non arrivi ad un appuntamento importante.",
    "La televisione si rompe e perdi l'ultima puntata di Tabby News.",
    "Qualcuno non apprezza il modo in cui sei vestito.",
    "Senza un preciso motivo, prendi per il culo un manovale che, essendo privo di senso dell'umorismo, ti fa a pezzi."
};
static const std::vector<std::string> frasiScuola = {
    "Senza una precisa ragione, picchi il prof di {MATERIA}",
    "Con lo scooter di un tuo amico, ti scontri contro la macchina del prof di {MATERIA}.",
    "Sputi dalla finestra della tua classe e centri il prof di {MATERIA}.",
    "Forse dovevi studiare di più {MATERIA}.",
    "Non riesci a dire una parola durante l' interrogazione di {MATERIA}.",
    "Offendi la moralità della madre del prof di {MATERIA}.",
    "Ti sei dimenticato del compito in classe di {MATERIA}.",
    "Non ti impegni abbastanza delle ore di {MATERIA}.",
    "Non ti metti di impegno a studiare {MATERIA}.",
    "Vai male in una interrogazione di {MATERIA}."
};

static const std::vector<std::string> vieStr = {
    "Via Lorenteggio",
    "Corso Vercelli",
    "Viale Papiniano",
    "Via Forze Armate",
    "Via Pontaccio",
    "Via Padova",
    "Via Porpora",
    "Corso Buenos Aires",
    "Viale Monza",
    "Via Leoncavallo",
    "Via Bagarotti",
    "Via Viterbo",
    "Via Fratelli Zoia",
    "Via dei Ciclamini",
    "Via Bassi",
    "Viale Monte Ceneri",
    "Viale Certosa",
    "Via Lamarmora",
    "Corso XII Marzo",
    "Viale dei Mille",
    "Via Goldoni",
    "Via Melchiorre Gioia",
    "Corso Lodi",
    "Viale Umbria",
    "Viale Puglie",
    "Via Tertulliano",
    "Viale Cassala",
    "Viale Liguria",
    "Viale Beatrice D'Este",
    "Viale Filippetti",
    "Viale Toscana",
    "Viale Famagosta",
    "Via La Spezia",
    "Via Giambellino",
    "Via Inganni",
    "Via Savona",
    "Viale Lomellina",
    "Via Galileo Galilei",
    "Viale Faenza",
    "Viale Jenner",
    "Via Fiuggi",
    "Via Imbonati",
    "Via Pio II",
    "Viale Tunisia",
    "Via Pergolesi",
    "Via Sammartini",
    "Via Primaticcio",
    "Via Archimede",
    "Corso Venezia",
    "Via Karl Marx"
};