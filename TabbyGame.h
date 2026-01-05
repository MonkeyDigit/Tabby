#pragma once
#include <random>
#include <vector>
#include "TabbyGuy.h"
#include "Chrono.h"
// Per attivare il debug
#define TABBY_DEBUG
constexpr float CAMBIO_EURO_LIRA = 1936.27;
constexpr int ATTESA_MAX = 7;

// TODO: Data del compleanno
// TODO: Calcola sesso
// TODO: Reset me con anagrafica random
// TODO: Init tabby con salvataggio
// TODO: Fine programma
// TODO: About
// TODO: Logo
// TODO: Spegnimi
// TODO: Configuration
// TODO: Personal info
// TODO: Famiglia
// TODO: Compagnia
// TODO: No money


// Sotto il cofano, i soldi di tabby e in generale il sistema monetario del gioco, viene gestito di base come euro, ma in base alla data sono messe a disposizione delle funzione per effettuare la conversione in lire, che fanno da "maschera"
enum class Valuta { LIRE, EURO };

enum class TipoMsg {
	INFO,		// Solo tasto OK
	SCELTA		// Tasti sì/no
};

enum class MsgAzione {
    NONE,
    // Scuola
    CORROMPI,
    // Compagnia
    GARA,
    // Lavoro
    LICENZIATI
};

struct Messaggio {
	Messaggio();
	Messaggio(TipoMsg tipo, MsgAzione id, std::string titolo, std::string testo, std::string img);
	TipoMsg m_tipo = TipoMsg::INFO;
    MsgAzione m_msgAzione = MsgAzione::NONE;
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
	bool PollMessaggi(Messaggio& outMsg);
	void ApplicaScelta(MsgAzione mgsAzione, bool sceltaYes);
	// TODO: BOOL / CODICE DI INFORMAZIONE / EVENTO CON MESSAGGIO
    // Scuola
    bool TriggerScuola();
    bool TriggerLavoro();
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
    const Ditta& ProponiDitta();
    bool AzioneCercaLavoro();   	// Restituisce true se possiamo procedere con la ricerca, false altrimenti (es. festivo)
    void AzioneLicenziati();
    void AzioneInformazioni();
    void AzioneLavora();
    void AzioneLeccaculo();
    void AzioneAumentoSalario();
    void AzioneSciopera();

    // Stringa formattata (es. "1.000 L." o "5 €")
    std::string GetSoldiStr(long long valoreBase) const;
	// GENERATORE RANDOM
	int GenRandomInt(int min, int max);
    // SCRITTURA LOG DI DEBUG
    void WriteLog(const std::string& messaggio);

private:
	TabbyGuy m_tabbyGuy;
	Chrono::Date m_date;
	Valuta m_valutaCorrente;
	int m_coolDownPestaggio;
    int m_attesa;
	std::vector<Messaggio> m_codaMsg;
	TipoGiorno m_tipoGiorno;
    long long m_costoCorruzione;
    int m_materiaIndex;
    int m_scooterAvversarioIndex;
	// IL MOTORE DI NUMERI CASUALI
	// Si chiama Mersenne Twister (mt19937), è veloce e affidabile
	std::mt19937 m_rng;

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

	// LOGICA MONETARIA
	// Prende il valore "grezzo" (base Euro) e lo converte in quello che l'utente deve vedere (Lire o Euro)
	long long ConvertiValuta(long long valoreBase) const;
};

// TODO: SPOSTARE STA ROBA DA QUALCHE PARTE?
static const std::vector<Ditta> ditte{
    {"Magneti Budelli",
        775800000, // 775.8 Milioni di euro
        "Sesto San Giovanni",
        "INDUSTRIA PESANTE - La Magneti Budelli produce su richiesta oggetti metallici, tra cui: ",
        "Magneti, dadi, bulloni, ferodi, turbine, aerei, patatine, attrezi da lavoro, parti meccaniche, missili, cannoni, dadi per il brodo, viti, chiavi, brugole, lime, carri armati, montacarichi, muletti, etc...",
        // Offerta Lavoro
        {
            "Si desidera incontrare giovani, età massima 30 anni, con una precedente esperienza nel settore, conoscenza della lingua inglese e dimestichezza nell'uso del PC. Trattamenti economici commisurati alle effettive capacità.",
            "Mi presento !",
            "Lascio perdere..."
        },
        ""
    },
    {"Acciaierie Diamine",
        -1, // Fatturato sconosciuto
        "Sconosciuta",
        "ACCIAIERIE E FERIERE - Produzioni su vasta scala di: ",
        "Tubi saldati, tondini, lamiere, trafilati, laminati, banda stagnata.",
        // Offerta Lavoro
        {
            "Cercasi n.6 addetti alla manutenzione delle apparecchiature, n 4. ballerine di danza classica con esperienza decennale, n.20 operai non specializzati da sfruttare e retribuire male, n.1 figli di puttana per quadri dirigenziali.",
            "Potrei provare...",
            "Non ho molta voglia di lavorare..."
        },
        ""
    },
    {"TESTMEC",
        -1, // Fatturato sconosciuto
        "Livorno, Italia",
        "MACCHINE DA LAVORO - Tra i prodotti più conosciuti della TESTMEC ci sono: ",
        "Motoseghe, motoseghe cingolate, muletti, schiaccia sassi, camion da cava, martelli pneumatici, lance termiche, armature, scavatrici, profilattici, hard disk.",
        // Offerta Lavoro
        {
            "Azienda leader nella produzione di motoseghe, cerca giovani di età non superiore ad anni n. 22, fini, educati e di gradevole aspetto per la vendita porta a porta di motoseghe cingolate da cava e da cantiere.",
            "Basta fare il disokkupato !",
            "Lavoro??? Sto' cercando di smettere..."
        },
        ""
    },
    {"T.I.B.B.",
        14250000000, // 14.25 Miliardi di euro
        "Sconosciuta",
        "MATERIALE ROTABILE O MOBILE - Produzione materiali pesanti od ad alta tecnologia:",
        "Treni, locomotori, pezzi per mezzi pesanti, motori per navi, turbine, reattori nucleari, centrali idroelettriche, motori a curvatura per navi stellari, mezzi cingolati.",
        // Offerta Lavoro
        {
            "TIBB cerca: 10 Apprendisti, 1 Tecnico specializzato riparazione materiale rotabile disponibili trasferte settimanali; inglese o tedesco saranno requisiti preferenziali. 4 collaboratori/collaboaratrici età massima anni 30, esperti in vendita, automuniti.",
            "Voglio trovare un posto !",
            "Che palle lavorare..."
        },
        ""
    },
    {"October Heavy Industries",
        830000000000000, // 83.4 Miliardi di euro
        "Mosca, Russia",
        "ENERGIA NUCLEARE - l' October Heavy Industries fornisce i seguenti servizi per l'industria nucleare: ",
        "Servizi di costruzione, equipaggiamento per il trattamento del combustibile, nocciolo dei reattori nucleari, fornitura di plutonio, inceneritori di rifiuti radioattivi, riprocessamento e trattamento delle acque.",
        // Offerta Lavoro
        {
            "L' October Heavy Industries cerca personale NON SPECIALIZZATO per la pulitura di noccioli dei reattori nucleari e per lo smaltimento dei rifiuti radioattivi. NON è RICHIESTO ALCUN TITOLO DI STUDIO.",
            "Sembra interessante...",
            "Troppo faticoso !"
        },
        ""
    },
    {"Arlond's Food Company",
        18500000000,    // Fatturato di 18.5 Miliardi di euro
        "Vevey, Svizzera",
        "ALIMENTARI - L'Arlond's Food Company distribuisce i seguenti prodotti: ",
        "Smorties(Dolci), Galac(Dolci), Toffe(Dolci), Vismaro(Salumi), Ethoprop(Pesticida), Scasso(Olio), Mera(Acqua), Sant' Ansemo(Acqua), Mare Fresko(Surgelati), Surgelami(Surgelati, DBCP(Vermicida), Maggio(Dadi), Rodo(Freni a disco)",
        // Offerta Lavoro
        {
            "Cerchiamo giovani dinamici e fantasiosi da inserire nella nostra catena di produzione dei pesticidi, un ramo emergente e gratificante che offre interessanti possibilità di guadagno.",
            "Figata !",
            "Torno a casa a dormire..."
        },
        ""
    },
    {"Computer Discarica",
        790180000, // 790.18 Milioni di euro
        "Venegono Superiore",
        "Ecco finalmente l'unica discarica abusiva dove potrete buttare tutti i vostri rottami informatici e riacquistarli al doppio del prezzo...",
        "",
        // Offerta Lavoro
        {
            "Ecco finalmente l'unica discarica abusiva dove potrete buttare tutti i vostri rottami informatici e riaquistarli al doppio del prezzo... Cercasi personale defic..ehm con una graaande passione per l'informatica, bella o brutta presenza, perfetta padronanza del sistema ZX Spectrum, CPM, MS-DOS ver 1.0 o inferiore, padronanza della lingua di mucca, cerchiamo giovani elastici, vitali, stronzi, scazzati basta che non siano tabbozzi....",
            "Tabbozzo?? Cos'è???",
            "Ma io sono un po' tabboz..."
        },
        ""
    },
    {"RICOPIO",
        -1, // Fatturato sconosciuto
        "Tokyo, Japan",
        "La RICOPIO, è la prima azienda al mondo che vanta innumerevoli copie illegali di libri, CD, cassette, persone, oggetti, progetti, brevetti, ecc...",
        "",
        // Offerta Lavoro
        {
            "La RICOPIO, è la prima azienda al mondo che vanta innumerevoli copie illegali di libri, CD, cassette, persone, oggetti, progetti, brevetti, ecc... Cercasi personale disinvolto, di piccola presenza, da assumersi come copertura.",
            "Potrei provare...",
            "Piccola presenza ??? No, grazie !"
        },
        ""
    }
};


// ------------------ TESTI MESSAGGI POPUP VARIABILI ---------------------------------------------------------------------
static const std::vector<FestaFissa> feste = {
    {1, 1, "Capodanno", "Oggi è Capodanno!"},
    {6, 1, "Epifania", "Tutte le feste si porta via..."},
    {25, 4, "Anniversario Liberazione", "La vera festa della liberazione ci sarà quando ci saremo liberati di tutti gli stronzi di questo mondo..."},
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
    "Cerchi la rissa con un metallaro e lui ti spacca tutte le ossa.",
    "Senza un preciso motivo, prendi per il culo un manovale che, essendo privo di senso dell'umorismo, ti fa a pezzi."
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