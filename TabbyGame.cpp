#define _CRT_SECURE_NO_WARNINGS // Senza questa mi fotte la funzione localtime
#include <fstream>  // Gestione file
#include <ctime>    // Per l'orario nel log
#include <iomanip>  // Per formattare l'orario
#include "TabbyGame.h"

EventoDati::EventoDati() {}

EventoDati::EventoDati(TipoEvento tipo, IdEvento id, std::string titolo, std::string testo, std::string img)
    : m_tipo{ tipo }, m_idEvento{ id }, m_titolo{ titolo }, m_testo{ testo }, m_immagine{ img }
{}

TabbyGame::TabbyGame()	// Lunedì 16 settembre 1991
	: m_tabbyGuy{}, m_date{1991, 9, 16}, m_valutaCorrente{Valuta::LIRE}, m_coolDownPestaggio{ 5 }, m_tipoGiorno{ TipoGiorno::NORMALE },
        m_attesa{ATTESA_MAX}
{
    WriteLog("\n=== AVVIO TABBY - LOG SESSIONE ===");
    // Inizializzo il generatore randomico UNA VOLTA SOLA qui nel costruttore
    // 'rd' è un dispositivo hardware che restituisce un numero casuale vero per il seme
     std::random_device rd;
    // Inizializzo il motore (m_rng) con quel seme
    m_rng.seed(rd());
}

// Funzione helper per la formattazione dei soldi
static std::string formattaConPunti(long long numero)
{
    // Convertiamo in stringa grezza (es. "1000000")
    std::string s = std::to_string(numero);

    // Calcoliamo dove inserire i punti
    // Partiamo dalla fine, saltiamo 3 cifre, e mettiamo un punto.
    // Dobbiamo stare attenti a non mettere il punto se siamo all'inizio (es. non ".100")
    // e a gestire il segno meno se tabby va in rosso

    int len = s.length();
    int count = 0;

    // Partiamo dal fondo verso l'inizio
    for (int i = len - 1; i > 0; i--)
    {
        // Se il carattere è un numero (evitiamo il segno '-')
        if (isdigit(s[i])) {
            count++;

            // Ogni 3 cifre, se non siamo all'inizio e il carattere prima non è un '-', inseriamo il punto
            if (count == 3 && i > 0 && s[i - 1] != '-')
            {
                s.insert(i, "."); // Inserisce il punto nella posizione i
                count = 0;    // Resetta il count
            }
        }
    }

    return s;
}

// Funzione Helper: Cerca 'tag' dentro 'testo' e lo sostituisce con 'valore'
static std::string Sostituisci(std::string testo, const std::string& tag, const std::string& valore)
{
    size_t pos = testo.find(tag);
    if (pos != std::string::npos) {
        testo.replace(pos, tag.length(), valore);
    }
    return testo; // Ritorna la stringa modificata
}

int TabbyGame::GenRandomInt(int min, int max)
{
    // Creiamo una distribuzione uniforme tra min e max
    // Significa che ogni numero ha la stessa probabilità di uscire
    std::uniform_int_distribution<int> dist(min, max);
    // Chiediamo al motore di darci un numero secondo quella distribuzione
    return dist(m_rng);
}

void TabbyGame::NuovoGiorno()
{
    // Il tempo scorre inesorabilmente...
    AvanzaCalendario();

    // TODO: dec pestaggio??
    if (m_coolDownPestaggio > 0)
        m_coolDownPestaggio--;

    if (m_attesa > 0)
        m_attesa--;

    // Chiamata ai reparti
    GestioneConsumi();
    GestioneRelazioni();
    GestioneLavoro();
    GestioneEconomia();
    GestioneEventiCasuali();    // Il dado del destino...
}

void TabbyGame::AvanzaCalendario()
{
    m_date.AddDay(1);
    m_tipoGiorno = TipoGiorno::NORMALE;
    
    // Messaggio anno bisestile
    if (Chrono::leapYear(m_date.GetYear()) &&
        m_date.GetMonth() == Chrono::Month::feb &&
        m_date.GetDay() == 29)
    {
        EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Anno Bisesto", "Anno bisesto, anno funesto...", "" };
        PushEvento(ev);
    }

    // Cambio di valuta: dopo il 2002 scatta l'euro
    if (m_valutaCorrente == Valuta::LIRE && m_date.GetYear() >= 2002)
    {
        m_valutaCorrente = Valuta::EURO;
        // TODO: Immagine silvio
        EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "L'italia è il paese che amo <3", "Oggi entra in vigore l'Euro €.\n Grazie di cuore, Silvio !!!", "" };
        PushEvento(ev);
    }

    // TODO: abbronzatura

    // ---------------> S T I P E N D I O <---------------

    if (m_tabbyGuy.GetImpegno() > 0)
    {
        m_tabbyGuy.IncGiorniLavoro(1);

        if (m_date.GetDay() == 27)
        {
            long stipendietto{};  // Stipendio calcolato secondo i giorni effettivi di lavoro

            if (m_tabbyGuy.GetGiorniLavoro() > 29)
                stipendietto = m_tabbyGuy.GetStipendio();
            else
                stipendietto = m_tabbyGuy.GetStipendio() * (long)m_tabbyGuy.GetGiorniLavoro() / 30;

            m_tabbyGuy.ResetGiorniLavoro();

            // EVENTO STIPENDIO
            // TODO: SESSO?
            std::string msg =  "Visto che sei stato un bravo dipendente sottomesso, ora ti arriva il tuo misero stipendio di " + GetSoldiStr(stipendietto);
            EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Stipendio !", msg, ""};
            PushEvento(ev);

            m_tabbyGuy.GuadagnaSoldi(stipendietto);

            WriteLog("Calendario: Stipendio (" + GetSoldiStr(stipendietto) + ")");
        }

    }

    // ---------------> P A L E S T R A <---------------
    if (m_date == m_tabbyGuy.GetScadenzaGym())
    {
        EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Pagah", "E' appena scaduto il tuo abbonamento della palestra...", "" };
        PushEvento(ev);
        // TODO: RESETTA DATA SCADENZA
        WriteLog("Calendario: E' scaduto l'abbonamento alla palestra");
    }

    switch (m_date.GetMonth())
    {
    case Chrono::Month::jan:    // Gennaio
        if (m_date.GetDay() < 7)
        {
            m_tipoGiorno = TipoGiorno::VACANZA_SCUOLA;
            // TODO: tipa vestita da babbo natale
        }
        break;

    case Chrono::Month::jun:    // Giugno
        if (m_date.GetDay() == 15)
        {
            // TODO: Sta roba va sistemata
            EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Ultimo giorno di scuola", "Da domani iniziano le vacanze estive !", "" };
            PushEvento(ev);
        }
        
        if (m_date.GetDay() > 15)
        {
            if (m_date.GetDay() == 22)
            {
                // TODO: DIALOG pagella
                // TODO: Qua va fatta una roba speciale
            }

            m_tipoGiorno = TipoGiorno::VACANZA_SCUOLA;
        }
        break;

    case Chrono::Month::jul:    // Luglio
    case Chrono::Month::aug:    // Agosto
        m_tipoGiorno = TipoGiorno::VACANZA_SCUOLA;
        // TODO: Tipa al mare
        break;

    case Chrono::Month::sep:    // Settembre

        if (m_date.GetDay() < 15)
            m_tipoGiorno = TipoGiorno::VACANZA_SCUOLA;

        if (m_date.GetDay() == 15)
        {
            EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Primo giorno di scuola", "Questa mattina devi tornare a scuola...", "" };
            PushEvento(ev);
            // Azzera le materie
            m_tabbyGuy.GetScuola().Reset();
        }
        break;

    case Chrono::Month::dec:    // Dicembre
        if (m_date.GetDay() > 22)
        {
            m_tipoGiorno = TipoGiorno::VACANZA_SCUOLA;
            // TODO: Roba tipa

            if (m_date.GetDay() == 25)
            {   // TODO: VESTITI NATALIZI
                if (m_tabbyGuy.GetPantaloni() == 19 && m_tabbyGuy.GetGiubotto() == 19)
                {
                    EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Natale...", "Con il tuo vestito da Babbo Natale riesci a stupire tutti...", "" };
                    PushEvento(ev);
                    m_tabbyGuy.IncFama(20);
                }
            }
            else if (m_date.GetDay() == 28 && m_tabbyGuy.GetPantaloni() == 19 && m_tabbyGuy.GetGiubotto() == 19)
            {
                EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Natale...", "Natale è già passato... Togliti quel dannato vestito...", "" };
                PushEvento(ev);
                m_tabbyGuy.DecFama(5);
            }
        }
    }

    // Domeniche e festività varie
    if (m_date.GetWeekDay() == Chrono::WeekDay::sunday)
        m_tipoGiorno = TipoGiorno::FESTIVO;

    // Cicla eventi delle feste fisse
    for (const auto& festa : feste)
    {
        if (m_date.GetMonth() == (Chrono::Month)festa.m_mese && m_date.GetDay() == festa.m_giorno)
        {
            m_tipoGiorno = TipoGiorno::FESTIVO;
            // Genera l'evento popup
            EventoDati ev;
            ev.m_tipo = TipoEvento::INFO;
            ev.m_titolo = festa.m_nome;
            ev.m_testo = festa.m_messaggio;
            PushEvento(ev);
        }
    }

    // DEBUG LOG
    std::string msg = "Calendario: " + 
        m_date.GetWeekDayStr() + " " + 
        std::to_string(m_date.GetDay()) + " " + 
        m_date.GetMonthStr() + " " + 
        std::to_string(m_date.GetYear()) + " " +
        GetSoldiStr(m_tabbyGuy.GetSoldi());
    WriteLog(msg);
}

void TabbyGame::GestioneConsumi()
{
    // Sigarette
    if (m_tabbyGuy.GetSizze() > 0)
    {
        m_tabbyGuy.DecSizze(1);

        if (m_tabbyGuy.GetSizze() == 0)
        {
            EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Sei senza sigarette !", "Apri il tuo pacchetto di sigarette e lo trovi disperatamente vuoto...", "" };
            PushEvento(ev);
            if (m_tabbyGuy.GetRep() > 10)
                m_tabbyGuy.DecRep(3);
        }
        else if (m_tabbyGuy.GetSizze() < 3)
        {
            EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Sigarette...", "Ti accorgi che stai per finire le tue sizze", "" };
            PushEvento(ev);
        }
    }

    // Abbonamento
    if (m_tabbyGuy.GetOperatore().GetCredito() > 0 && m_tabbyGuy.GetTelefono().GetStato() > -1)
    {
        m_tabbyGuy.GetOperatore().DecCredito(1);
        if (m_tabbyGuy.GetFama() < 55)
            m_tabbyGuy.IncFama(1);

        if (m_tabbyGuy.GetOperatore().GetCredito() == 0)
        {
            EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Telefonino", "Cerchi di telefonare e ti accorgi di aver finito i soldi a tua disposizione...", "" };
            PushEvento(ev);
        }
        else if (m_tabbyGuy.GetOperatore().GetCredito() < 3)
        {
            EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Telefonino", "Ti accorgi che stai per finire la ricarica del tuo telefonino", "" };
            PushEvento(ev);
        }
    }

    // Telefono
    if (m_tabbyGuy.GetTelefono().GetStato() == 1)
    {
        // Cellulare morente
        m_tabbyGuy.GetTelefono().DecStato(1);

        EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Telefonino", "Dopo una vita di duro lavoro, a furia di prendere botte, il tuo cellulare si spacca...", "" };
        PushEvento(ev);
    }
}

void TabbyGame::GestioneRelazioni()
{
    int rnd{};
    // Rapporti Tipa
    if (m_tabbyGuy.GetRapporti() > 3)
    {
        rnd = GenRandomInt(0, 4) - 3;
        if (rnd > 0)
            m_tabbyGuy.DecRapporti(1);
    }

    if (m_tabbyGuy.GetRapporti() > 0)
    {
        if (m_tabbyGuy.GetRapporti() < 98)
        {
            int rand_max = (m_tabbyGuy.GetRapporti() + m_tabbyGuy.GetFortuna() + m_tabbyGuy.GetFama()) * 3 + 1;
            rnd = GenRandomInt(1, rand_max);

            if (rnd < 11)
            {
                // Da 1 a 10, la donna ti molla...
                // TODO: Play sound

                m_tabbyGuy.SetRapporti(0);

                // TODO: Dialog la tipa ti molla - sesso m f
                // TODO: Messaggio speciale
                EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "La tipa ti molla...", "[messaggio]", "" };
                PushEvento(ev);

                m_tabbyGuy.DecRep(11 - rnd);    // Quelle con numero più basso, sono peggiori...

            }
        }
    }
}

void TabbyGame::GestioneLavoro()
{
    int rnd{};
    // Lavoro
    if (m_tabbyGuy.GetImpegno() > 3)
    {
        rnd = GenRandomInt(0, 6) - 3;
        if (rnd > 0)
            m_tabbyGuy.DecImpegno(1);
    }

    if (m_tabbyGuy.isAssunto())
    {
        rnd = GenRandomInt(0, (m_tabbyGuy.GetImpegno() * 2 + m_tabbyGuy.GetFortuna() * 3) - 1);

        if (rnd < 2)
        {
            // Perdi il lavoro
            m_tabbyGuy.ResetLavoro();

            // TODO: Dialog licenziato + suono
            EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Perdi il lavoro...", "Un bel giorno ti svegli e scopri di essere stato licenziato", "" };
            PushEvento(ev);

        }
    }
}

void TabbyGame::GestioneEconomia()
{
    // Paghetta
    if (m_date.GetWeekDay() == Chrono::WeekDay::saturday)
    {
        // Il Sabato c'è la paghetta...
        if (m_tabbyGuy.GetStudio() >= 45)
        {
            m_tabbyGuy.GuadagnaSoldi(m_tabbyGuy.GetPaghetta());
            // DEBUG LOG
            WriteLog("GestioneEconomia: Paghetta (" + GetSoldiStr(m_tabbyGuy.GetPaghetta()) + ")");

            if (m_tabbyGuy.GetStudio() >= 80)
            {
                m_tabbyGuy.GuadagnaSoldi(m_tabbyGuy.GetPaghetta());
                // DEBUG LOG
                WriteLog("GestioneEconomia: Paghetta doppia !!!");


                // TODO: Dialog eventi paghetta doppia + suono
                EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Paghetta settimanale", "Visto che vai bene a scuola, ti diamo il doppio della paghetta...", "" };
                PushEvento(ev);
            }
        }
        else
        {
            m_tabbyGuy.GuadagnaSoldi(m_tabbyGuy.GetPaghetta() * 0.5f);
            // DEBUG LOG
            WriteLog("GestioneEconomia: Metà paghetta (" + GetSoldiStr(m_tabbyGuy.GetPaghetta()*0.5f) + ")");


            EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Paghetta settimanale", "Finché non andrai bene a scuola, ti daremo solo metà della paghetta...", "" };
            PushEvento(ev);
        }
    }
}

void TabbyGame::GestioneEventiCasuali()
{
    // Eventi casuali
    int caso = GenRandomInt(0, (100 + m_tabbyGuy.GetFortuna() * 2) - 1);
    // DEBUG LOG
    WriteLog("GestioneEventiCasuali: Evento casuale n. " + std::to_string(caso));


    if (caso < 51)
    {
        // METALLONI E MANOVALI
        if (caso <= 10)
        {
            // TODO: FEMMINA

            m_tabbyGuy.DecRep(caso);
            int rndFrase = GenRandomInt(0, frasiMetallari.size() - 1);
            int rndVia = GenRandomInt(0, vieStr.size() - 1);

            EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Vieni pestato", Sostituisci(frasiMetallari[rndFrase],"{LUOGO}", vieStr[rndVia]), ""};
            PushEvento(ev);
            // DEBUG LOG
            WriteLog("GestioneEventiCasuali: Metallaro n. " + std::to_string(rndFrase));

            m_coolDownPestaggio = 5;
        }
        else if (11 <= caso && caso <= 20)   // SCOOTER
        {
            if (m_tabbyGuy.GetScooter().GetStato() != -1 && m_tabbyGuy.GetScooter().GetAttivita() == Attivita::IN_GIRO)
            {
                if (m_tabbyGuy.GetTelefono().GetStato() > -1)
                {
                    // A furia di prendere botte, il cellulare si spacca...
                    m_tabbyGuy.GetTelefono().DecStato(GenRandomInt(1, 8));
                }

                if (caso < 17)
                {
                    // TODO: messaggio camionista
                    m_tabbyGuy.GetScooter().DecStato(35);
                    EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Camionista bastardo", "[messaggio]", "" };
                    PushEvento(ev);
                    // DEBUG LOG
                    WriteLog("GestioneEventiCasuali: Scooter - Camionista...");

                }
                else
                {
                    // TODO: messaggio muro
                    m_tabbyGuy.GetScooter().DecStato(20);
                    EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Muro bastardo", "[messaggio]", "" };
                    PushEvento(ev);
                    // DEBUG LOG
                    WriteLog("GestioneEventiCasuali: Scooter - Muro...");
                }

                m_tabbyGuy.DecRep(2);

                if (m_tabbyGuy.GetScooter().GetStato() <= 0)
                {
                    m_tabbyGuy.GetScooter().Reset();
                    EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Scooter Distrutto", "Quando ti rialzi ti accorgi che il tuo scooter è ormai ridotto a un ammasso di rottami", "" };
                    PushEvento(ev);
                    // DEBUG LOG
                    WriteLog("GestioneEventiCasuali: Lo scooter si è completamente distrutto...");
                }
            }
        }
        else if (21 <= caso && caso <= 30)
        {
            if (caso <= 27)
                m_tabbyGuy.DecFama(1);

            if (caso <= 25)
                m_tabbyGuy.DecFama(1);

            if (caso <= 23)
                m_tabbyGuy.DecFama(5);

            m_tabbyGuy.DecFama(2);

            int rndFrase = GenRandomInt(0, frasiFortuna.size() - 1);
            EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Sei fortunato...", frasiFortuna[rndFrase], ""};
            PushEvento(ev);
            // DEBUG LOG
            WriteLog("GestioneEventiCasuali: Evento riguardante la figosità...");
        }
        else if (31 <= caso && caso <= 40)   // Skuola
        {
            // Durante i giorni di vacanza non ci sono eventi riguardanti la scuola
            if (m_tipoGiorno == TipoGiorno::NORMALE)
            {
                int rndMat = GenRandomInt(0, m_tabbyGuy.GetScuola().m_materie.size() - 1);
                int rndFrase = GenRandomInt(0, frasiScuola.size() - 1);
                Materia& mat = m_tabbyGuy.GetScuola().m_materie[rndMat];
                EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Scuola", Sostituisci(frasiScuola[rndFrase],"{MATERIA}",mat.GetNome()), ""};
                PushEvento(ev);

                mat.DecVoto(2);
                m_tabbyGuy.CalcolaStudio();
                // DEBUG LOG
                WriteLog("GestioneEventiCasuali: Evento riguardante la scuola");
            }
        }
        else if (caso == 41 || caso == 42)  // Tipa - una tipa ci prova
        {
            if (m_tabbyGuy.GetFama() > 35)   // Fama < 35 = nessuna speranza...
            {
                // TODO: Sesso m f

                // TODO: Dialog una tipa ci prova
                EventoDati ev{ TipoEvento::SCELTA, IdEvento::BASE, "Qualcuno ti caga...", "Una tipa, di nome [nome] (Figosità [fama]), ci prova con te...\nCi stai ???", "" };
                PushEvento(ev);
            }

            // TODO: if yes
            /*
                if ((figTemp >= 79) && (Rapporti < 1) && (sesso == 'M'))
                { // Se non hai gia' una tipa e rifiuti una figona...
                    MessageBox(hInstance,
                               "Appena vengono a sapere che non ti vuoi mettere insieme ad una figona come quella, i tuoi amici ti prendono a scarpate.",
                               "Idiota...", MB_OK | MB_ICONSTOP);
                    Reputazione -= 4;
                    if (Reputazione < 0)
                        Reputazione = 0;
                }
                break;
            */

            // Controlla che tu non abbia già una tipa...
            if (m_tabbyGuy.GetRapporti() > 0)
            {
                // TODO: Dialog due donne
                EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Due donne", "", "" };
                PushEvento(ev);
            }
            else   // Bravo, non hai una tipa...
            {
                // DEBUG LOG
                WriteLog("GestioneEventiCasuali: Una tipa/o ci prova...");

                // TODO: m_tabbyGuy.SetRapporti(45 + GenRandomInt(0, 14));
                // TODO: Figtipa
                /*
                sprintf(Nometipa, "%s", nomeTemp);
                FigTipa = figTemp;
                Rapporti = 45 + random(15);
                Fama += FigTipa / 10;
                if (Fama > 100)
                    Fama = 100;
                Reputazione += FigTipa / 13;
                if (Reputazione > 100)
                    Reputazione = 100;
                */
            }
        }
        else if (caso == 43)    // Domande inutili
        {
            // TODO: COMPLETARE
            EventoDati ev{ TipoEvento::SCELTA, IdEvento::BASE, "Domande inutili della tipa...", "Mi ami ???", "" };
            PushEvento(ev);
            /*
            i = MessageBox(hInstance,
                                   "Mi ami ???",
                                   "Domande inutili della Tipa...", MB_YESNO | MB_ICONQUESTION);
                    if (i != IDYES)
                    {
                        MessageBox(hInstance,
                                   "Sei sempre il solito stronzo.. non capisco perche' resto ancora con uno come cosi'...",
                                   "Risposta sbagliata...", MB_OK | MB_ICONSTOP);
                        Rapporti -= 45;
                        if (Rapporti < 5)
                            Rapporti = 5;
                    }
            */
            // DEBUG LOG
            WriteLog("GestioneEventiCasuali: Domande inutili della tipa...");
        }
        else if (caso == 44)
        {
            // TODO: COMPLETARE
            EventoDati ev{ TipoEvento::SCELTA, IdEvento::BASE, "Domande inutili della tipa...", "Ma sono ingrassata ???", "" };
            PushEvento(ev);
            /*
            i = MessageBox(hInstance,
                                   "Ma sono ingrassata ???",
                                   "Domande inutili della Tipa...", MB_YESNO | MB_ICONQUESTION);
                    if (i != IDNO)
                    {
                        MessageBox(hInstance,
                                   "Sei un bastardo, non capisci mai i miei problemi...",
                                   "Risposta sbagliata...", MB_OK | MB_ICONSTOP);
                        Rapporti -= 20;
                        if (Rapporti < 5)
                            Rapporti = 5;
                    }
            */
            // DEBUG LOG
            WriteLog("GestioneEventiCasuali: Domande inutili della tipa...");
        }
        else if (45 <= caso && caso <= 48)
        {
            // TODO: Qua non c'è nulla
        }
        else if (caso == 49 || caso == 50)  // Vari ed eventuali
        {
            if (m_tabbyGuy.GetTelefono().GetStato() > -1)
            {
                m_tabbyGuy.GetTelefono().DecStato(GenRandomInt(1, 8));

                EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Telefonino", "Il telefonino ti cade di tasca e vola per terra...", "" };
                PushEvento(ev);
                // DEBUG LOG
                WriteLog("GestioneEventiCasuali: Telefonino - Cade...");

            }
        }
    }
}

bool TabbyGame::PollEvento(EventoDati& outEvento)
{
    if (m_codaEventi.empty())
        return false;   // Nessun evento

    // Viene COPIATO il primo evento del vettore
    outEvento = m_codaEventi.front();
    // Viene rimosso dalla coda
    m_codaEventi.erase(m_codaEventi.begin());

    return true;    // C'è un evento da processare
}

void TabbyGame::ApplicaScelta(IdEvento idEvento, bool sceltaYes)
{
    switch (idEvento)
    {
    case IdEvento::CORROMPI:
        if (sceltaYes)
        {
            if (m_tabbyGuy.SpendiSoldi(m_costoCorruzione))
            {
                // DEBUG LOG
                WriteLog("AzioneCorrompi: Corrompi un professore per " + GetSoldiStr(m_costoCorruzione));
                m_tabbyGuy.GetScuola().m_materie[m_materiaIndex].SetVoto(10);
                m_tabbyGuy.CalcolaStudio();
            }
            else
            {
                m_tabbyGuy.GetScuola().m_materie[m_materiaIndex].DecVoto(2);
                m_tabbyGuy.CalcolaStudio();
                EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Errore critico", "Cosa ??? Prima cerchi di corrompermi, poi si scopre che non hai abbastanza soldi !!!", "" };
                PushEvento(ev);
            }
        }

        NuovoGiorno();
        
        break;
    case IdEvento::GARA:
        if (sceltaYes)
        {
            /*
            if ((scooter[m_scooterAvversarioIndex].GetVelocita() + 80 + GenRandomInt(0, 39)) > (m_tabbyGuy.GetScooter().GetVelocita() + m_tabbyGuy.GetScooter().GetStato() + m_tabbyGuy.GetFortuna())
            {
                // Gara persa
                if(m_tabbyGuy.GetRep() > 80)
                    m_tabbyGuy.DecRep(3);
                if(m_tabbyGuy.GetRep() > 10)
                    m_tabbyGuy.DecRep(2);

                EventoDati ev{TipoEvento::INFO, IdEvento::BASE, "Hai perso, coglione", "Dopo pochi metri si vede l'inferiorità del tuo scooter...", "" };
                PushEvento(ev);
            }
            else
            {
                // Gara vinta
                m_tabbyGuy.IncRep(10);
            
                EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Hai vinto", "Con il tuo scooter, bruci l'avversario in partenza...", "" };
                PushEvento(ev);
            }
            */
        }
        else
        {
            if (m_tabbyGuy.GetRep() > 80)    // Se non accetti la sfida, perdi reputazione...
                m_tabbyGuy.DecRep(3);
            if (m_tabbyGuy.GetRep() > 10)
                m_tabbyGuy.DecRep(2);
        }
        m_tabbyGuy.GetScooter().DecBenzina(5);
        // TODO: CalocolaVelocita

        NuovoGiorno();
        break;
    }
    
}

bool TabbyGame::TriggerScuola()
{
    if (m_tipoGiorno == TipoGiorno::NORMALE)
        return true;

    EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Scuola", "Non puoi andare a scuola in un giorno di vacanza!", "" };
    PushEvento(ev);

    return false;
}

bool TabbyGame::TriggerLavoro()
{
    if (m_tipoGiorno != TipoGiorno::FESTIVO)
        return true;

    EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Lavoro", "Arrivato davanti ai cancelli della ditta, li trovi irrimediabilmente chiusi...", "" };
    PushEvento(ev);

    return false;
}

void TabbyGame::AzioneStudia(int materiaIndex)
{
    Materia& mat = m_tabbyGuy.GetScuola().m_materie[materiaIndex];
    if (mat.GetVoto() >= 10)
        return;

    if (m_tabbyGuy.GetRep() > 10) // Studiare costa fatica...(oltre che reputazione e fama)
        m_tabbyGuy.DecRep(5);
    if (m_tabbyGuy.GetFama() > 5)
        m_tabbyGuy.DecFama(1);

    mat.IncVoto(1);
    m_tabbyGuy.CalcolaStudio();
    NuovoGiorno();
}

void TabbyGame::AzioneMinaccia(int materiaIndex)
{
    Materia& mat = m_tabbyGuy.GetScuola().m_materie[materiaIndex];

    // TODO: Sesso??
    if (m_tabbyGuy.GetRep() >= 30 || GenRandomInt(1, 10) == 1)
    {
        mat.IncVoto(2);
    }
    else
    {
        m_tabbyGuy.DecRep(2);
        mat.DecVoto(1);

        // TODO: SUONO
        EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Bella figura", "Cosa ??? Credi di farmi paura piccolo pezzettino di letame vestito da zarro...\nDeve ancora nascere chi può minacciarmi...", "" };
        PushEvento(ev);
    }
    // IMPORTANTE: NEGLI EVENTI NULLI VA SEMPRE FATTO NUOVOGIORNO, MENTRE QUELLI A SCELTA CONVIENE IN APPLICASCELTA PER RISPETTARE L'ORDINE DI INVOCAZIONE EVENTI
    m_tabbyGuy.CalcolaStudio();
    NuovoGiorno();
}

void TabbyGame::AzioneCorrompi(int materiaIndex)
{
    m_materiaIndex = materiaIndex;
    // Per corrompere un professore va da 75€ a 150€
    m_costoCorruzione = GenRandomInt(75,150);
    EventoDati ev{ TipoEvento::SCELTA, IdEvento::CORROMPI, "Corrompi i professori", "Ma... forse per " + GetSoldiStr(m_costoCorruzione) + " potrei dimenticare i tuoi ultimi compiti in classe...", "" };
    PushEvento(ev);
}

// TODO: SISTEMA QUESTE FUNZIONI
void TabbyGame::AzioneGara()
{
    if (m_tabbyGuy.GetScooter().GetStato() <= 0)
    {
        EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Ma che sei scemo ???", "Con quale scooter vorresti gareggiare, visto che non lo possiedi ?", "" };
        PushEvento(ev);
    }
    else if (m_tabbyGuy.GetScooter().GetAttivita() != Attivita::IN_GIRO)
    {
        EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Svegliati...", "Purtroppo non puoi gareggiare visto che il tuo scooter è " + m_tabbyGuy.GetScooter().GetAttivitaStr(true) + ".", "" };
        PushEvento(ev);
    }
    else
    {
        // TODO: IL RANDOM VA FATTO SUL NUMERO DI SCOOTER IMPLEMENTATI
        /*
        int rnd = GenRandomInt(1, scootdim);
        m_scooterAvversarioIndex = rnd;
        // TODO: suono
        EventoDati ev{ TipoEvento::SCELTA, IdEvento::GARA, "Gareggia con lo scooter", "Accetti la sfida con un tabbozzo che ha un ", scooter[rnd].getnome() + " ?", "" };
        PushEvento(ev);
        if (m_tabbyGuy.GetScooter().GetStato() > 30)
            m_tabbyGuy.GetScooter().DecStato(GenRandomInt(0, 1));
        */
    }
}

void TabbyGame::AzioneEsci()
{
    // Uscendo con la propria compagnia si può arrivare solamente a reputazione = 57
    if (m_tabbyGuy.GetRep() < 57)
        m_tabbyGuy.IncRep(1);
    if (m_tabbyGuy.GetRep() < 37)    // Se la reputazione è bassa, sale più in fretta
        m_tabbyGuy.IncRep(1);
    if (m_tabbyGuy.GetRep() < 12)
        m_tabbyGuy.IncRep(1);
    NuovoGiorno();
}

void TabbyGame::AzioneChiama()
{
    if (m_tabbyGuy.GetRep() < 16)
    {
        EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Sfigato", "Con la scarsa reputazione che hai, tutti trovano qualcosa di meglio da fare piuttosto che venire.", "" };
        PushEvento(ev);
    }
    else if (m_coolDownPestaggio > 0)    // TODO: Ragiona su sta roba
    {
        if (GenRandomInt(0, 1) == 1)
        {
            if (m_tabbyGuy.GetRep() < 80)
                m_tabbyGuy.IncRep(3);
            EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Chiama la Compagnia", "Dopo aver visto i tuoi amici, chi ti ha picchiato non si farà più vedere in giro per un bel pezzo...", "" };
            PushEvento(ev);
        }
        else
        {
            if (m_tabbyGuy.GetRep() < 95)
                m_tabbyGuy.IncRep(5);
            EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Chiama la Compagnia", "Anche i tuoi amici, al gran completo, vengono sacagnati di botte da chi ti aveva picchiato, accorgendosi così che non sei solo tu ad essere una chiavica, ma lo sono anche loro...", "" };
            PushEvento(ev);
        }
        NuovoGiorno();
    }
    else
    {
        EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Chiama la Compagnia (perchè ?)", "Visto che non c'è nessuno da minacciare, tutti se ne vanno avviliti...", "" };
        PushEvento(ev);
    }
}

void TabbyGame::AzioneAumentoPaghetta()
{
    if (m_tabbyGuy.GetStudio() > 40)
    {
        if (((m_tabbyGuy.GetStudio() - m_tabbyGuy.GetPaghetta() + m_tabbyGuy.GetFortuna()) > (100 + GenRandomInt(0, 50)) && (m_tabbyGuy.GetPaghetta() < 50)))
        {
            EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Aumento Paghetta !", "Va bene... ti daremo " + GetSoldiStr(5) + " di paghetta in più...", "" };
            PushEvento(ev);
            m_tabbyGuy.IncPaghetta(5);
            // TODO: RICORDA DI RESETTARE
        }
        else
        {
            EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Errore irrecuperabile", "Vedi di scordartelo... Dovrà passare molto tempo prima che ti venga aumentata la paghetta...", "" };
            PushEvento(ev);
        }

        NuovoGiorno();
    }
    else
    {
        EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Errore irrecuperabile", "Quando andrai meglio a scuola, forse...", "" };
        PushEvento(ev);
    }
}

void TabbyGame::AzioneSoldiExtra()
{
    if (m_tabbyGuy.GetStudio() >= 40)
    {
        if (m_attesa == 0)
        {
            m_attesa = ATTESA_MAX;
            m_tabbyGuy.GuadagnaSoldi(10);

            // DEBUG LOG
            WriteLog("AzioneSoldiExtra: Soldi extra (" + GetSoldiStr(10) + ")");
        }
        else
        {
            EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Non te li diamo, viziato", "Ma insomma ! Non puoi continuamente chiedere soldi ! Aspetta ancora qualche giorno. Fai qualche cosa di economico nel frattempo...", "" };
            PushEvento(ev);
        }

        NuovoGiorno();
    }
    else
    {
        EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Errore irrecuperabile",
            "Quando andrai meglio a scuola potrai tornare a chiederci dei soldi, non ora. "
            "\nMa non lo sai che per la tua vita è importante studiare, e dovresti impegnarti "
            "di più, perchè quando ti impegni i risultati si vedono, solo che sei svogliato "
            "e non fai mai nulla, mi ricordo che quando ero giovane io era tutta un altra cosa..."
            "allora sì che i giovani studiavano...", "" };
        PushEvento(ev);
    }
}

void TabbyGame::AzioneChiediSoldi()
{
    EventoDati ev{ TipoEvento::INFO, IdEvento::BASE, "Errore irrecuperabile", "Non pensarci neanche lontanamente...", "" };
    PushEvento(ev);
    NuovoGiorno();
}

long long TabbyGame::ConvertiValuta(long long valoreBase) const
{
	if (m_valutaCorrente == Valuta::EURO)
		return valoreBase;

	return (long long)(valoreBase * CAMBIO_EURO_LIRA);
}

std::string TabbyGame::GetSoldiStr(long long valoreBase) const
{
	if (m_valutaCorrente == Valuta::EURO)
		return (formattaConPunti(valoreBase) + " €");

	// Altrimenti è la lira...
	long long valoreConvertito = ConvertiValuta(valoreBase);
	return (formattaConPunti(valoreConvertito) + " L.");
}

void TabbyGame::WriteLog(const std::string& messaggio)
{
#ifdef TABBY_DEBUG
    // Apre il file "tabby.log" in modalità append, e se non esiste lo crea
    std::ofstream logFile("tabby.log", std::ios::app);

    if (logFile.is_open())
    {
        // Ottengo l'orario corrente per fare il figo
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);

        // [ORARIO] Messaggio
        logFile << "[" << std::put_time(&tm, "%H:%M:%S") << "] "
            << messaggio << std::endl;

        // Chiudiamo il file (importante per salvare subito)
        logFile.close();
    }
#endif
}