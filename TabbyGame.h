#pragma once
#include <random>
#include <vector>
#include "TabbyGuy.h"
#include "Disco.h"
#include "Chrono.h"
// Per attivare il debug
#define TABBY_DEBUG
constexpr float CAMBIO_EURO_LIRA = 1936.27;
constexpr int ATTESA_MAX = 7;

// Sotto il cofano, i soldi di tabby e in generale il sistema monetario del gioco, viene gestito di base come euro, ma in base alla data sono messe a disposizione delle funzione per effettuare la conversione in lire, che fanno da "maschera"
enum class Valuta { LIRE, EURO };

enum class TipoMsg {
	INFO,		// Solo tasto OK
	SCELTA,		// Tasti sì/no
    PAGELLA     // Così sappiamo di dover mostrare la finestra con la pagella
};

enum class MsgAzione {
    NONE,
    // Scuola
    CORROMPI,
    // Compagnia
    GARA,
    // Lavoro
    LICENZIATI,
    // TIPA
    LASCIA_TIPA,
    TIPA_MI_AMI,
    TIPA_INGRASSATA,
    TIPA_CI_PROVA
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
    // Negozi
    bool AzioneCompra(const Prodotto& prod);

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

// TODO: CAMBIA FIGOSITA'
static const std::vector<Negozio> negozi{
    {
        "Bau House", TipoProd::GIUBBOTTO,
        {
            {TipoProd::GIUBBOTTO, "Giubbotto Fatiscenza OG", "Giubbotto originale \"Fatiscenza\", elegante e raffinato, è adatto a tutte le occasioni, dalla serata con gli amici, alle feste più esclusive.", "", 175, 30},
            {TipoProd::GIUBBOTTO, "Giubbotto Fatiscenza NEW", "Giubbotto \"Fatiscenza\", nella sua nuova versione, per essere sempre in linea con i tempi che cambiano.", "", 190, 65},
            {TipoProd::GIUBBOTTO, "Giubbotto Ricercato", "Ecco qualcosa per i più ricercati!\nCome può mancare nell'armadio un vestito come questo ???\nCompratelo subito, non ve ne pentirete.", "", 150, 90}
        },
    },
    {
        "Blue Riders", TipoProd::GIUBBOTTO,
        {
            {TipoProd::GIUBBOTTO, "Giacca di pelle", "Giacca di pelle... cos'altro dire ?", "", 125, 40},
            {TipoProd::GIUBBOTTO, "Fatiscenza Green", "Giubbotto \"Fatiscenza\" Green Line*, per tutti quelli che possono permetterselo.", "", 190, 60},
            {TipoProd::GIUBBOTTO, "Fatiscenza White", "\"Fatiscenza White\" con una comoda imbottitura in piumino sintetico*, è adatto per tutte le stagioni.\n\n*Tutti i giubbotti della linea \"Fatiscenza\" sono fatti con materiali altamente infiammabili, da tenere lontano da fonti di calore....", "", 210, 75}
        }
    },
    {
        "Zoccolaro", TipoProd::PANTALONI,
        {
            {TipoProd::PANTALONI, "Pantaloni Gessati", "Pantaloni gessati stile anni '30, comunque sono tornati di moda ed ogni zarro che si rispetti doverebbe averne un paio.", "", 45, 45},
            {TipoProd::PANTALONI, "Pantaloni Firmati", "Pantaloni per il tempo libero firmati da una delle marche più prestigiose e costose di abbigliamento sportivo.", "", 85, 75},
            {TipoProd::PANTALONI, "Pantaloni Reciclati", "Pantaloni realizzati da uno dei più eccentrici e stravaganti stilisti del momento utilizzando fibra di bottiglie di plastica reciclate.", "", 125, 60},
            {TipoProd::PANTALONI, "Pantaloni Scacchiera", "Hai sempre sognato di essere una scacchiera ??? Eccoti accontentato ! Realizzati in 100% acrilico.", "", 95, 50}
        }
    },
    {
        "Footsmocker", TipoProd::SCARPE,
        {
            {TipoProd::SCARPE, "Scarpe Old Style", "Comode ed eleganti, dalla linea Old style, se avete un po' di soldi da buttare, dovete assolutemente comprarle.", "", 60, 50},
            {TipoProd::SCARPE, "Scarpe Scomode", "Il massimo della tendenza per i giovani d'oggi ! Queste scarpe, scomode e brutte, fanno sicuramente per voi !", "", 110, 70},
            {TipoProd::SCARPE, "Scarpe Rialzate", "Volete essere alla moda spendendo poco ??? Queste scarpe rialzate con mattoni forati sono quello che ci vuole per voi !", "", 45, 20}
        }
    },
    {
        "Footsmocker II", TipoProd::SCARPE,
        {   // TODO: VARIAZIONI ?
            {TipoProd::SCARPE, "Scarpe Design", "Stile e semplicità in una scarpa dal desing moderno, disponibile nei colori tenui e delicati o sgargianti e decisi.", "", 75, 80},
            {TipoProd::SCARPE, "Scarpe Prugna", "Come poteva mancare il massimo della tendenza per i giovani d'oggi nella nuova colorazione Prugna?", "", 120, 90},
            {TipoProd::SCARPE, "Scarpe Tendenza", "E per chi volesse osare ancora di più, ecco la nuova colorazione Tendenza, per non passare inosservati.", "", 90, 100}
        }
    },
    {
        "Bar Tabacchi", TipoProd::SIGARETTE,
        {
            {TipoProd::SIGARETTE, "Barclay", "Condensato: 5 Nicotina: 0.5", "", 6, 2},
            {TipoProd::SIGARETTE, "Camel", "Condensato: 8 Nicotina: 0.7", "", 6, 1},
            {TipoProd::SIGARETTE, "Davidoff Superior Lights", "These carefully selected tobaccos have been skillfully blended to assure your pleasure.\nCondensato: 7 Nicotina: 0.6", "", 6, 2},
            {TipoProd::SIGARETTE, "Davidoff Mildnes", "These carefully selected tobaccos have been skillfully blended to assure your pleasure.\nCondensato: 7 Nicotina: 0.6", "", 6, 2},
            {TipoProd::SIGARETTE, "Davidoff Classic", "These carefully selected tobaccos have been skillfully blended to assure your pleasure.\nCondensato: 13 Nicotina: 0.9", "", 6, 2},
            {TipoProd::SIGARETTE, "Diana Blu", "Specially mild\nCondensato: 9 Nicotina 0.7", "", 5, 1},
            {TipoProd::SIGARETTE, "Diana Rosse", "King size filter\nCondensato: 12 Nicotina 0.9", "", 5, 1},
            {TipoProd::SIGARETTE, "Dunhill Lights", "Condensato: 8 Nicotina 0.7", "", 6, 0},
            {TipoProd::SIGARETTE, "Merit", "Condensato: 7 Nicotina 0.5", "", 6, 0},
            {TipoProd::SIGARETTE, "Gauloises Blu", "Condensato: 14 Nicotina 1.0", "", 6, 0},
            {TipoProd::SIGARETTE, "Gauloises Rosse", "Condensato: 7 Nicotina 0.6", "", 6, 0},
            {TipoProd::SIGARETTE, "Unlucky Strike", "Unluckies are a blend of the finest american, turkish and other choice tobaccos. Their manufacture includes the unlucky strike process. It's toasted.\nCondensato: 13 Nicotina 1.0", "", 6, 1},
            {TipoProd::SIGARETTE, "Unlucky Strike Lights", "Unluckies are a blend of the finest american, turkish and other choice tobaccos. Their manufacture includes the unlucky strike process. It's toasted.\nCondensato: 9 Nicotina 0.7", "", 6, 1},
            {TipoProd::SIGARETTE, "Malborro Medium", "Condensato: 8 Nicotina 0.6", "", 6, 2},
            {TipoProd::SIGARETTE, "Malborro Rosse", "Condensato: 12 Nicotina 0.9", "", 6, 2},
            {TipoProd::SIGARETTE, "Malborro Lights", "Condensato: 8 Nicotina 0.6", "", 6, 2},
            {TipoProd::SIGARETTE, "NS Rosse", "Condensato: 11 Nicotina 1.0", "", 5, 0},
            {TipoProd::SIGARETTE, "NS Mild", "Condensato: 9 Nicotina 0.9", "", 5, 0},
            {TipoProd::SIGARETTE, "Poll Mon Blu", "Famous american cigarettes.\nWherever particular people congregat.\nCondensato: 9 Nicotina 0.7", "", 5, 1},
            {TipoProd::SIGARETTE, "Poll Mon Rosse", "Famous american cigarettes.\nWherever particular people congregat.\nCondensato: 12 Nicotina 0.9", "", 5, 1},
            {TipoProd::SIGARETTE, "Philip Morris", "Condensato: 12 Nicotina 1.0", "", 6, 2},
            {TipoProd::SIGARETTE, "Philip Morris Super Light", "Condensato: 4 Nicotina 0.4", "", 6, 2},
            {TipoProd::SIGARETTE, "Armadis", "Armadis contains an original blend of selected Burley, Virginia and Oriental tobaccos, whose natural curing processes guarantee its authentic flavour satisfaction.\nCondensato: 10 Nicotina 0.9", "", 5, 1},
            {TipoProd::SIGARETTE, "Winston", "Condensato: 11 Nicotina 0.9", "", 5, 0}
        }
    }
};

static const std::vector<std::string> frasiSigarette{
  "Nuoce gravemente alla salute",
  "Il fumo nuoce alle persone che vi circondano",
  "Ogni anno il tabagismo fa più vittime degli incidenti stradali",
  "Proteggete i bambini: non fate loro respirare il vostro fumo",
  "Il fumo nuoce alle persone che vi circondano",
  "Il fumo provoca malattie cardiovascolari",
  "Donne incinte: il fumo nuoce alla salute del vostro bambino",
  "Il fumo provoca il cancro"
};

static const std::vector<std::string> frasiSfighe{
    "Non ti caga nemmeno di striscio...",
    "Mi dispiace...insomma, ho già il ragazzo.. ",
    "Non vorrei offendere i tuoi sentimenti, ma mi fai proprio cagare...",
    "Ti amo troppo ma non potrei mai riuscire a stare con te...",
    "Sei la persona più spregevole che ho incontrato sulla faccia della terra...",
    "Meglio morire che mettermi con uno come te...",
    "Spero che tu stia scerzando... in caso contrario devo informati che il mio ragazzo è campione di lotta libera...",
    "Vorrei tanto mettermi con te, davvero... ora però devo prendermi una pausa per riflettere, spero che tu mi capisca...",
    "Sono già impegnata... se solo me lo avessi chiesto due giorni fa...",
    "Mi dispiace ma non credo che tu sia il mio tipo... Il mio amore è alto, biondo con gli occhi azzurri...",
    "No.",
    "Sono appena uscita da una storia molto triste ed ora non mi fido più di voi uomini.",
    "Voi uomini siete tutti stronzi... Chissà quante altre povere ragazze hai ingannato con il tuo sorrisetto di merda...",
    "Cioè zerella... non ci sto dentro...",
    "Mettermi con te ???? Ha,ha,ha !",
    "Nooooooo !",
    "Comincia a ritirati su i pantaloni...",
    "Forse mi hai preso per la ragazza sbagliata...",
    "Vai a farti fottere...",
    "O, scusa, mi sono dimenticata di dirti che sono impegnata..."
};

static const std::vector<std::string> nomiTipe{
"Marcella",
"Giovanna",
"Concetta",
"Elisa",
"Federica",
"Ilaria",
"Simona",
"Assunta",
"Maria",
"Elena",
"Margherita",
"Giorgia",
"Silvia",
"Teresa",
"Grazia",
"Elisabetta",
"Alessia",
"Chiara",
"Alessandra",
"Rosa"
};

static const std::vector<Disco> discoteche = {
        {
            "Splush",
            "Questo locale buio e spazioso si estende per circa 134.000 metri quadrati sotto la città. Di grande effetto con degli enormi tubi fognari che rallegrano l'ambiente.\nChiuso il Lunedì.",
            Chrono::WeekDay::monday,
            false,
            20, // Prezzo originale
            30,
            15,
            2
        },
        {
            "QB",
            "Spaziosa discoteca di tendenza ricavata da un'antica stalla. Odori e sapori tipici di una volta; si balla tra vagonate di letame e mangime. A mezzanotte casola e cotiche per tutti.\nChiuso il Giovedì",
            Chrono::WeekDay::thursday,
            false,
            15,
            0,
            7,
            1
        },
        {
            "Spoking",
            "Da sempre nome di grande richiamo per modelle, modaioli, giovani della Milano bene, tabbozzi e non. Per chi ama gli ambienti modaioli. A due passi dalla discarica di Cerro Maggiore.\nChiuso il Lunedì",
            Chrono::WeekDay::monday,
            false,
            16,
            0,
            8,
            1
        },
        {
            "Madame Close",
            "Elegante discobar e discoteca dove non mancano le sorprese. Ex bordello di periferio ora trasformato in un lussuoso bordello del centro storico.\nSelezione all'ingresso - Chiuso il Lunedì.",
            Chrono::WeekDay::monday,
            false,
            20,
            35,
            15,
            3
        },
        {
            "Number 0",
            "Che dire di questo storico locale? Meglio non dire niente...\nChiuso il Mercoledì.",
            Chrono::WeekDay::wednesday,
            false,
            15,
            0,
            6,
            2
        },
        {
            "Spuce",
            "Il massimo dell'economico per il tabbozzo che non deve chiedere mai (ai genitori i soldi per comprare le sigarette).\nChiuso il Martedì",
            Chrono::WeekDay::tuesday,
            false,
            10,
            0,
            5,
            2
        },
        {
            "Honeywood",
            "Locale di tendenza ispirato ad una acciaieria. All'interno tra altiforni e colate di acciao bollente, potete degustare ottimi piatti caraibici.\nChiuso il Lunedì",
            Chrono::WeekDay::monday,
            false,
            16,
            0,
            8,
            3
        },
        {
            "Strap Line",
            "Discoteca fuori porta, con ampie terrazze dove si può ballare all'aperto ed una splendida vista sulle montagne innevate del bassopiano siberiano orientale.\nAutostrada del Brennero uscita Novosibirsk - Chiuso la Domenica.",
            Chrono::WeekDay::sunday,
            true,
            20,
            0,
            9,
            2
        }
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
            "Lavoro??? Sto cercando di smettere..."
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
        "ENERGIA NUCLEARE - L'October Heavy Industries fornisce i seguenti servizi per l'industria nucleare: ",
        "Servizi di costruzione, equipaggiamento per il trattamento del combustibile, nocciolo dei reattori nucleari, fornitura di plutonio, inceneritori di rifiuti radioattivi, riprocessamento e trattamento delle acque.",
        // Offerta Lavoro
        {
            "L'October Heavy Industries cerca personale NON SPECIALIZZATO per la pulitura di noccioli dei reattori nucleari e per lo smaltimento dei rifiuti radioattivi. NON è RICHIESTO ALCUN TITOLO DI STUDIO.",
            "Sembra interessante...",
            "Troppo faticoso !"
        },
        ""
    },
    {"Arlond's Food Company",
        18500000000,    // Fatturato di 18.5 Miliardi di euro
        "Vevey, Svizzera",
        "ALIMENTARI - L'Arlond's Food Company distribuisce i seguenti prodotti: ",
        "Smorties(Dolci), Galac(Dolci), Toffe(Dolci), Vismaro(Salumi), Ethoprop(Pesticida), Scasso(Olio), Mera(Acqua), Sant'Ansemo(Acqua), Mare Fresko(Surgelati), Surgelami(Surgelati, DBCP(Vermicida), Maggio(Dadi), Rodo(Freni a disco)",
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

// Quiz
static std::vector<QuizScheda> schede = {
    {   "Sei ancora disoccupato ? Ah!ah!ah!...",
        "Sai, vorremmo assumerti subito ma, prima dovresti superare una piccola formalità , questo semplice test attitudinale sarà uno scherzo per una mente sveglia come la tua !",
        {
            { "A. Di che colore era il cavallo bianco di Napoleone ?", { "Verde", "Nero", "Rosso" } },
            { "B. In un lavoro in cantiere il vostro compagno viene travolto da una decina di travi provenienti dall'alto, ne deducete che...", { "Presto avrete bisogno di un nuovo compagno...", "Quel burlone del gruista avrà nuovamente fatto uno dei suoi scherzi.", "Dovete ammazzare subito il vostro compagno prima che soffra !" } },
            { "C. Vi rendete conto di quello che state facendo ?", { "Sì", "No", "Forse" } }
        }
    },
    {   "Test attitudinale Ministero Pubblica Distruzione",
        "Non sarai mica in difficoltà, non é da te riflettere su queste banalissime domande... auguri e buon lavoro (si fa per dire)",
        {
            { "A. Quale é il lasso di tempo che intercorre tra l'arresto di un mafioso e la sua scarcerazione ?", { "10 nano secondi", "-10 nano secondi (viene liberato prima dell'arresto effettivo)", "60 nano secondi" } },
            { "B. Quando i vostri genitori credono opportuno non elargire la paghetta settimanale, credete sia il caso di:", { "Prendere esempio da Gaetano Bresci.", "Non ho capito la domanda.", "Noooooooo! come farò adesso a mantenere il cellulare ?" } },
            { "C. E' vietato il sorpasso", { "in prossimità di centrali nucleari sovietiche.", "In tutte le strade percorse da camionisti infuriati e ubriachi.", "Banchettando sul cruscotto con lupini e mascarpone." } }
        }
    },
    {   "Questionario...",
        "Leggere attentamente le domande poi con la matita segnare la.....CATZ...!...porc !..AAaaaaahhhhhhhhhh !!!",
        {
            { "A. Indovinate, stronzoni, cosa é quell'oggetto tondo con un buco in mezzo, dal quale escono strani odori ?", { "Parigi dall'alto.", "Una marmitta catalitica !", "La mia faccia." } },
            { "B. Riteniamo che voi che leggete siete tutti dei disadattati e per di più senza lavoro... Ah dimenticavamo, anche un po' stronzi !", { "Lo andate a dire al budello di vostra madre nel casino !", "E io vi dico che siete degli sciocchini ! ...ECCO !", "Andatevela a stroncare nel culo va' !" } },
            { "C. Come ci si comporta per aumentare la produttività ?", { "Una partita a poker con gli amici in ufficio.", "Assenteismo.", "Ogni tanto si soddisfa sessualmente il direttore." } }
        }
    },
    {   "Nuovi quiz ministeriali",
        "Tracciate una retta A B, partite poi i suddetti punti per disegnare la secante, l'angolo formatosi dovrà essere di 32 gradi se fallite significa che non siete buoni a un cazzo !",
        {
            { "A. Quando si vuole preservare la democrazia bisogna:", { "Sciogliere il parlamento.", "Fare aereoplanini con la costituzione.", "Proclamarsi capo assoluto." } },
            { "B. Viene prima l'uovo o la gallina ?", { "L'uovo, sicuramente !", "La gallina perché frutto di un'evoluzione !", "Io" } },
            { "C. Quanto dista la nebulosa del granchio dalla stella del buco strappato ?", { "Tanto, tanto, tanto, tanto, tanto, tanto lontana !", "1.254.643.342 anni luce.", "Tanti metri quante le volte che è maiala !" } }
        }
    },
    {   "Pestasti lo sterco mezzo di strizzo",
        "Non é male questa pastasciutta, però quest'altra volta ci si rimette il parmigiano eh ?!",
        {
            { "A. Allo stadio sei in curva sud, in mezzo alla bolgia vedi una bella figona che balla e si dimena a ritmo di samba...", { "La fai a pezzi con un colpo di mortaio.", "Ti travesti da brasileiro e gli refili il tuo mango.", "Bah, non so ballare la samba." } },
            { "B. Che ruolo vorresti avere in una squadra di calcio ?", { "Attaccante, sono uno specialista nell'infilare le palle ovunque.", "Tutti ! Gliela faccio vedere io come si gioca a calcio.", "Portiere, sono bravo a prendere le palle di tutti." } },
            { "C. L'inquinamento atmosferico prodotto dai veicoli con motore a scoppio", { "Dipende dall'isolamento del reattore nucleare.", "Può essere incrementato dall'uso di camion senza marmitte.", "Dipende dal camionista." } }
        }
    },
    {   "Quiz attitudinale per pirloni senza lavoro",
        "Cortesemente, potrebbe compilare questo test in modo da comprovare la sua discutibile preparazione e mettere in condizioni noi della ditta a risparmiarci la sua orrenda puzza di zarro ?",
        {   // TODO: CAMBIA
            { "A. Quando ritornate a casa e trovate la vostra nonna che vi guarda con uno sguardo terrorizzato", { "La fai a pezzi con un colpo di fucile.", "La fate a pezzi con una granata di mortaio.", "Uhm...povera nonna !" } },
            { "B. Cosa significa: Le calcul de la gran mére d'Ada ?", { "Nel mezzo del cammin di nostra vita.", "I calcoli della nonna di Ada.", "Quei tegami delle vostre madri ?" } },
            { "C. Cosa può provocare una puzzetta in ufficio ?", { "Un'evaquazione immediata dello stabile.", "Dispersione del personale.", "Disapprovazione da parte del personale." } }
        }
    },
    {   "._..  .._  .._--__.. ._.._-- .__:.-_:::... ---_:.:::..:.--._",
        "Test attitudinale statale per portatori di handicap. Questo test é esclusivamente adibito all'uso dei non vedenti. I mutilati di guerra sono sempre i benaccetti.",
        {
            { "A. -_.-__..   ._  _.._..  ._._._.  .._..  _..._._...._  -", { "Ma state scherzando ?", "Non conosco il braille, non sono cieco.", "Sono un mutilato di guerra !" } },
            { "B. Come forse avrete capito ci siamo presi gioco di lei... Come reagite davanti un simile sopruso ?", { "Male, molto male !", "Ora vi faccio a pezzi con una motosega !", "I dipendenti devono essere sempre sottomessi." } },
            { "C. Da quanti e quali atomi é composta la molecola della benzina ?", { "2 atomi di potassio e mezzo.", "12 atomi di idrogeno e 24 di carbonio.", "8 atomi di idrogeno e 3 di potassio." } }
        }
    },
    {   "Provaci sarai fortunato",
        "Con questo test verificheremo la tempra di ogni aspirante operaio per vedere la sua validità e la sua devozione al lavoro nonche alla sua tenuta di strada...",
        {
            { "A. Come forse avete capito dalla didascalia sul fronte del documento crediamo opportuno domandarvi se possedete una Fìat Tempra.", { "Purltroppo No!", "No, ma se mi assumete potrò comprarla !", "Ehm...Um...err..certamente !" } },
            { "B. Vi é stato facile trovare altri lavori ?", { "Come rubare una mazza ferrata a un bambino !", "Come bere un bicchier d'acqua (e veleno) !", "Sì, sì, sì, sì, sì !" } },
            { "C. Vi trovate in una strada buia e desolata...", { "Speriamo non mi rubino il portafogli !", "La persona che trovo la faccio a pezzi !", "Devo tornare a casa, domani devo andare al lavoro." } }
        }
    },
    {   "Test lavorativo per disoccupati",
        "I nuovi QUIZ MINISTERIALI.\nIn vigore dal 15 settembre 1997",
        {
            { "A. Come si può trasmettere l'A.I.D.S. ?", { "In TV.", "Sulla Panda.", "Sulla Panda?" } },
            { "B. Il mod. 740 si compila seguendo...", { "Ordine e disciplina !", "Una vecchietta brandendo un'ascia !", "Le leggi n°345 cod.2 comma 3 paragrafo 7°" } },
            { "C. Come si provoca una combustione in camera stagna ?", { "A forza di rutti abominevoli.", "Con un pacchetto di fiammiferi.", "Con un pacchetto di dinamite ed armi assortite." } }
        }
    },
    {   "Test attitudinale",
        "Dal paterno terrazzo donde lucean le stelle illuminar con luci rubelle ci viene ora la rima in -azzo e si é sciupata tutta la poesia .",
        {
            { "A. Quando si esegue una colata di titanio ad elevata temperatura", { "Si verifichino bolle nella colata.", "Si rompe il catino rovesciando il titanio liquido sui lavoratori.", "Volino le bestemmie." } },
            { "B. Solitamente in gergo lavorativo si usa dire", { "Bestemmie varie", "Governo ladro", "Che voglia di lavorare che ho !" } },
            { "C. Con il sistema anti infortunistico sul lavoro... normalmente accade che", { "Tutti gli operai possono lavorare tranquilli", "Tutti gli operai crepano ugualmente per gli incidenti", "Tutti gli operai bestemmiano durante il lavoro." } }
        }
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
    "Non riesci a dire una parola durante l'interrogazione di {MATERIA}.",
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