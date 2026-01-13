#pragma once
#include <random>
#include <vector>
#include "Utility.h"
#include "TabbyGuy.h"
#include "Disco.h"
#include "Chrono.h"
// Per attivare il debug
#define TABBY_DEBUG
constexpr double CAMBIO_EURO_LIRA = 1936.27;
constexpr int ATTESA_MAX = 7;
constexpr int PALESTRA_ABB_1 = 30;
constexpr int PALESTRA_ABB_6 = 145;
constexpr int PALESTRA_ABB_12 = 290;
constexpr int PREZZO_LAMPADA = 7;

// Sotto il cofano, i soldi di tabby e in generale il sistema monetario del gioco, viene gestito di base come euro, ma in base alla data sono messe a disposizione delle funzione per effettuare la conversione in lire, che fanno da "maschera"
enum class Valuta { LIRE, EURO };

enum class TipoMsg {
    SUONO,       // NON APPARE IL MESSAGGIO
    // Solo tasto OK
	INFO,       // Icona blu (i)
    AVVISO,     // Icona triangolo giallo (!)
    ERRORE,     // Icona Errore Rossa (X)
    SUCCESSO,   // Icona info
    // Tasti sì/no
	SCELTA,
    // FINESTRE SPECIALI
    METALLARO,
    MANOVALE,
    INCIDENTE_MURO,
    INCIDENTE_CAMION,
    PAGELLA,     // Così sappiamo di dover mostrare la finestra con la pagella
    DUE_DONNE
};

enum class Scelta {
    NONE,
    // Scuola
    CORROMPI,
    // Compagnia
    GARA,
    // Lavoro
    LICENZIATI,
    // Tipa
    LASCIA_TIPA,
    TIPA_MI_AMI,
    TIPA_INGRASSATA,
    TIPA_CI_PROVA,
    // Telefono
    VENDI_TEL,
    // Scooter
    RIPARA_SCOOTER,
    VENDI_SCOOTER
};

struct Messaggio {
	Messaggio();
	Messaggio(TipoMsg tipo, std::string titolo = "", std::string testo = "", Scelta id = Scelta::NONE);
	TipoMsg m_tipo = TipoMsg::INFO;
    Scelta m_msgAzione = Scelta::NONE;
	std::string m_titolo;
	std::string m_testo;
    int m_soundId = -1;
};

enum class TipoGiorno {
	NORMALE,        // Scuola aperta, Negozi aperti
	VACANZA_SCUOLA, // Scuola chiusa, Negozi aperti - Es. Estate
	FESTIVO         // Tutto chiuso - Es. Domenica o Natale
};

struct FestaFissa {
    FestaFissa() : m_giorno{ 1 }, m_mese{ 1 }, m_nome{}, m_messaggio{}
    {}
	int m_giorno;
	int m_mese;
	std::string m_nome;
	std::string m_messaggio;
};

struct Negozio {
    Negozio() : m_nome{}, m_merce{ CategoriaOggetto::GENERICO }, m_catalogo{}
    {}
    std::string m_nome;
    CategoriaOggetto m_merce;
    std::vector<Acquistabile*> m_catalogo;  // ATTENZIONE !!! Siccome Acquistabile è una classe base, è di vitale importanza utilizzare dei puntatori, perché se mettessi la variabile intera, nel momenti in cui inserisci acquistabili di diverso tipo, il vettore troncherebbe le informazioni aggiuntive della classe figlia
};

class TabbyGame
{
public:
	TabbyGame();

	// GETTER FUNCTIONS
	Valuta GetValutaCorrente() const { return m_valutaCorrente; };
	TabbyGuy& GetTabbyGuy() { return m_tabbyGuy; };
	Chrono::Date& GetDate() { return m_date; };
	Chrono::Date& GetScadenzaPal() { return m_scadenzaPal; };
    TipoGiorno GetTipoGiorno() const { return m_tipoGiorno; };
	// LOGICA EVENTI
	// Restituisce true se c'era un evento (e lo mette in outMsg), false se la coda eventi è vuota
	bool PollMessaggi(Messaggio& outMsg);
	void ApplicaScelta(Scelta mgsAzione, bool sceltaYes);
    // Scuola
    bool TriggerScuola();
	void AzioneStudia(int materiaIndex);
	void AzioneMinaccia(int materiaIndex);
	void AzioneCorrompi(int materiaIndex);
    // Compagnia
    void AzioneGara();
    void AzioneEsci();
    void AzioneChiama();
    // Famiglia
    void AzioneAumentoPaghetta();
    void AzioneSoldiExtra();
    void AzioneChiediSoldi();
    // Lavoro
    bool TriggerLavoro();
    const Ditta& ProponiDitta();
    void AzioneRifiutaProposta();
    const QuizScheda& AssegnaQuiz();
    void AzioneTerminaQuiz(const std::vector<int>& countRisposte, std::string nomeDitta);  // Controlla se il quiz è stato compilato correttamente (1 risposta per domanda)
    bool AzioneCercaLavoro();   	// Restituisce true se possiamo procedere con la ricerca, false altrimenti (es. festivo)
    void AzioneLicenziati();
    void AzioneLavora();
    void AzioneLeccaculo();
    void AzioneAumentoSalario();
    void AzioneSciopera();
    // Discoteca
    void AzionePagaDisco(int discoIndex);
    // Tipa
    Tipa GeneraTipa();
    void AzioneProvaci(const Tipa& tipa);
    void AzioneLasciaTipa();
    void AzioneEsciTipa();
    void AzioneTelefonaTipa();
    void AzioneVoglioEntrambe();
    void AzionePreferiscoNuova();
    // Negozi
    bool TriggerNegozio(CategoriaOggetto merce);
    void AzioneCompra(const Acquistabile& prod);
    // Palestra
    bool TriggerPalestra();
    bool PalestraAttiva() const { return m_scadenzaPal > m_date; }; // E' ancora attivo l'abbonamento della palestra?
    void AzioneVaiPalestra();
    void AzioneLampada();
    void AzioneAbbonamento(int mesi);
    // Telefono
    void AzioneVendiTelefono();
    void AzioneAttivaSim(int abbonIndex);
    void AzioneRicarica(long long taglio, std::string nomeOp);
    // Scooter
    void AzioneVendiScooter();
    void AzioneRiparaScooter();
    void AzioneUsaScooter();
    void AzioneFaiBenza();
    bool TriggerMeccanico();
    Scooter GeneraScooter();

    // Stringa formattata (es. "1.000 L." o "5 €")
    std::string GetSoldiStr(long long valoreBase) const;
	// GENERATORE RANDOM
	int GenRandomInt(int min, int max);
    // SCRITTURA LOG DI DEBUG
    void WriteLog(const std::string& messaggio);
    // SOUND
    void PlaySound(int id);
    void SetSoundActive(bool active) { m_soundActive = active; }

    const Tipa& GetTipaNuova() const { return m_tipaNuova; }
    const Negozio& GetTelefonia() const { return m_telefonia; }
    const Negozio& GetConcessionario() const { return m_concessionario; }
    const Negozio& GetMeccanico() const { return m_meccanico; }
    std::string GetMetallaroImageStr() { return ("metallaro" + std::to_string(GenRandomInt(1, 5)) + ".png"); }
    std::vector<Abbonamento>& GetAbbonamenti() { return m_abbonamenti; }
    std::vector<Negozio>& GetNegozi() { return m_negozi; }
    std::vector<Disco>& GetDiscoteche() { return m_discoteche; }
    std::vector<Ditta>& GetDitte() { return m_ditte; }
    std::vector<QuizScheda>& GetSchedeQuiz() { return m_schede; }
    std::vector<FestaFissa>& GetFeste() { return m_feste; }

private:
	TabbyGuy m_tabbyGuy;
	Chrono::Date m_date;
    Chrono::Date m_scadenzaPal;
	Valuta m_valutaCorrente;
	int m_coolDownPestaggio;
	int m_coolDownPelle;
    int m_paloCounter;
    int m_attesa;
	std::vector<Messaggio> m_codaMsg;
	TipoGiorno m_tipoGiorno;
    long long m_costoCorruzione;
    long long m_costoRiparazione;
    long long m_offertaTel;
    long long m_offertaScooter;
    Tipa m_tipaNuova;
    Scooter m_scooterRnd;
    int m_materiaIndex;
    bool m_soundActive;
	// IL MOTORE DI NUMERI CASUALI
	// Si chiama Mersenne Twister (mt19937), è veloce e affidabile
	std::mt19937 m_rng;


    // CONTENUTI DI GIOCO
    std::vector<Abbonamento> m_abbonamenti;
    Negozio m_telefonia;
    Negozio m_concessionario;
    Negozio m_meccanico;
    // TODO: CAMBIA FIGOSITA'
    std::vector<Negozio> m_negozi;
    std::vector<Disco> m_discoteche;
    std::vector<Ditta> m_ditte;
    std::vector<QuizScheda> m_schede;  // Quiz

    // ------------------ TESTI MESSAGGI POPUP VARIABILI ---------------------------------------------------------------------
    std::vector<FestaFissa> m_feste;
    std::vector<std::string> m_nomiTipe;
    std::vector<std::string> m_frasiSeparazione;
    std::vector<std::string> m_frasiPalo;
    std::vector<std::string> m_frasiPalestra;
    std::vector<std::string> m_frasiSigarette;
    std::vector<std::string> m_frasiMetallari;
    std::vector<std::string> m_frasiManovali;
    std::vector<std::string> m_frasiCamionista;
    std::vector<std::string> m_frasiMuro;
    std::vector<std::string> m_frasiFortuna;
    std::vector<std::string> m_frasiScuola;
    std::vector<std::string> m_vieStr;

    void CaricaStringhe();
    void CaricaAbbonamenti();
    void CaricaDiscoteche();
    void CaricaDitte();
    void CaricaFeste();
    void CaricaNegozi();
    void CaricaQuiz();

	// Evento
	void PushMessaggio(const Messaggio& e) { m_codaMsg.push_back(e); };

	// Funzioni di gestione giornaliera
	void NuovoGiorno();
	void AvanzaCalendario();
	void GestioneConsumi();     // Sigarette, telefono, abbonamento
	void GestioneRelazioni();   // Tipa, amici, famiglia
	void GestioneLavoro();      // Licenziamento, impegno
	void GestioneEconomia();    // Paghetta, stipendio
	void GestioneEventiCasuali(); // Il generatore di caos
    // Stato dello scooter (a secco, invasato, ingrippato)
    void CheckScooter();

	// LOGICA MONETARIA
	// Prende il valore "grezzo" (base Euro) e lo converte in quello che l'utente deve vedere (Lire o Euro)
	long long ConvertiValuta(long long valoreBase) const;
};

// TODO: IMPOSTA LA FAMA DEI CELL

// TODO: CAMBIA FIGOSITA' VESTITI
// TODO: VARIAZIONI FOOTSMOCKER?