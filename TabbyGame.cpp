#define _CRT_SECURE_NO_WARNINGS // Senza questa mi fotte la funzione localtime
#include <fstream>  // Gestione file
#include <ctime>    // Per l'orario nel log
#include <iomanip>  // Per formattare l'orario
#include "TabbyGame.h"

Messaggio::Messaggio() {}

Messaggio::Messaggio(TipoMsg tipo, MsgAzione id, std::string titolo, std::string testo, std::string img)
    : m_tipo{ tipo }, m_msgAzione{ id }, m_titolo{ titolo }, m_testo{ testo }, m_immagine{ img }
{}

TabbyGame::TabbyGame()	// Lunedì 16 settembre 1991
	: m_tabbyGuy{}, m_date{1991, 9, 16}, 
    m_valutaCorrente{Valuta::LIRE}, 
    m_coolDownPestaggio{ 5 }, 
    m_tipoGiorno{ TipoGiorno::NORMALE },
    m_attesa{ATTESA_MAX}
{
    WriteLog(" =======|| AVVIO TABBY - LOG SESSIONE ||======= ");
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
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Anno Bisesto", "Anno bisesto, anno funesto...", "" };
        PushMessaggio(msg);
    }

    // Cambio di valuta: dopo il 2002 scatta l'euro
    if (m_valutaCorrente == Valuta::LIRE && m_date.GetYear() >= 2002)
    {
        m_valutaCorrente = Valuta::EURO;
        // TODO: Immagine silvio
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "L'italia è il paese che amo <3", "Oggi entra in vigore l'Euro €.\n Grazie di cuore, Silvio !!!", "" };
        PushMessaggio(msg);
    }

    // TODO: abbronzatura

    // ---------------> S T I P E N D I O <---------------

    if (m_tabbyGuy.GetCarriera().GetImpegno() > 0)
    {
        m_tabbyGuy.GetCarriera().Lavora();

        if (m_date.GetDay() == 27)
        {
            long stipendietto{};  // Stipendio calcolato secondo i giorni effettivi di lavoro

            if (m_tabbyGuy.GetCarriera().GetGiorniLavorati() > 29)
                stipendietto = m_tabbyGuy.GetCarriera().GetStipendio();
            else
                stipendietto = m_tabbyGuy.GetCarriera().GetStipendio() * (long)m_tabbyGuy.GetCarriera().GetGiorniLavorati() / 30;

            m_tabbyGuy.GetCarriera().ResetGiorni();

            // EVENTO STIPENDIO
            // TODO: SESSO?
            std::string testo =  "Visto che sei stato un bravo dipendente sottomesso, ora ti arriva il tuo misero stipendio di " + GetSoldiStr(stipendietto);
            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Stipendio !", testo, ""};
            PushMessaggio(msg);

            m_tabbyGuy.GuadagnaSoldi(stipendietto);

            WriteLog("Calendario: Stipendio (" + GetSoldiStr(stipendietto) + ")");
        }

    }

    // ---------------> P A L E S T R A <---------------
    if (m_date == m_tabbyGuy.GetScadenzaGym())
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Pagah", "E' appena scaduto il tuo abbonamento della palestra...", "" };
        PushMessaggio(msg);
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
            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Ultimo giorno di scuola", "Da domani iniziano le vacanze estive !", "" };
            PushMessaggio(msg);
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
            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Primo giorno di scuola", "Questa mattina devi tornare a scuola...", "" };
            PushMessaggio(msg);
            // Azzera le materie
            m_tabbyGuy.GetScuola().Reset();
        }
        break;

    case Chrono::Month::dec:    // Dicembre
        if (m_date.GetDay() > 22)
        {
            m_tipoGiorno = TipoGiorno::VACANZA_SCUOLA;
            // TODO: Roba tipa

            // TODO: SISTEMA STA ROBA
            /*
            if (m_date.GetDay() == 25)
            {   // TODO: VESTITI NATALIZI
                if (m_tabbyGuy.GetPantaloni() == 19 && m_tabbyGuy.GetGiubotto() == 19)
                {
                    Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Natale...", "Con il tuo vestito da Babbo Natale riesci a stupire tutti...", "" };
                    PushMessaggio(msg);
                    m_tabbyGuy.IncFama(20);
                }
            }
            else if (m_date.GetDay() == 28 && m_tabbyGuy.GetPantaloni() == 19 && m_tabbyGuy.GetGiubotto() == 19)
            {
                Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Natale...", "Natale è già passato... Togliti quel dannato vestito...", "" };
                PushMessaggio(msg);
                m_tabbyGuy.DecFama(5);
            }
            */
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
            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, festa.m_nome, festa.m_messaggio, "" };
            PushMessaggio(msg);
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
            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Sei senza sigarette !", "Apri il tuo pacchetto di sigarette e lo trovi disperatamente vuoto...", "" };
            PushMessaggio(msg);
            if (m_tabbyGuy.GetRep() > 10)
                m_tabbyGuy.DecRep(3);
        }
        else if (m_tabbyGuy.GetSizze() < 3)
        {
            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Sigarette...", "Ti accorgi che stai per finire le tue sizze", "" };
            PushMessaggio(msg);
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
            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Telefonino", "Cerchi di telefonare e ti accorgi di aver finito i soldi a tua disposizione...", "" };
            PushMessaggio(msg);
        }
        else if (m_tabbyGuy.GetOperatore().GetCredito() < 3)
        {
            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Telefonino", "Ti accorgi che stai per finire la ricarica del tuo telefonino", "" };
            PushMessaggio(msg);
        }
    }

    // Telefono
    if (m_tabbyGuy.GetTelefono().GetStato() == 1)
    {
        // Cellulare morente
        m_tabbyGuy.GetTelefono().DecStato(1);

        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Telefonino", "Dopo una vita di duro lavoro, a furia di prendere botte, il tuo cellulare si spacca...", "" };
        PushMessaggio(msg);
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
                Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "La tipa ti molla...", "[messaggio]", "" };
                PushMessaggio(msg);

                m_tabbyGuy.DecRep(11 - rnd);    // Quelle con numero più basso, sono peggiori...

            }
        }
    }
}

void TabbyGame::GestioneLavoro()
{
    int rnd{};
    // Lavoro
    if (m_tabbyGuy.GetCarriera().GetImpegno() > 3)
    {
        rnd = GenRandomInt(0, 6) - 3;
        if (rnd > 0)
            m_tabbyGuy.GetCarriera().DecImpegno(1);
    }

    if (m_tabbyGuy.HaUnLavoro())
    {
        rnd = GenRandomInt(0, (m_tabbyGuy.GetCarriera().GetImpegno() * 2 + m_tabbyGuy.GetFortuna() * 3) - 1);

        if (rnd < 2)
        {
            // Perdi il lavoro
            m_tabbyGuy.Licenziati();

            // TODO: Dialog licenziato + suono
            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Perdi il lavoro...", "Un bel giorno ti svegli e scopri di essere stato licenziato", "" };
            PushMessaggio(msg);

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
                Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Paghetta settimanale", "Visto che vai bene a scuola, ti diamo il doppio della paghetta...", "" };
                PushMessaggio(msg);
            }
        }
        else
        {
            m_tabbyGuy.GuadagnaSoldi(m_tabbyGuy.GetPaghetta() * 0.5f);
            // DEBUG LOG
            WriteLog("GestioneEconomia: Metà paghetta (" + GetSoldiStr(m_tabbyGuy.GetPaghetta()*0.5f) + ")");


            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Paghetta settimanale", "Finché non andrai bene a scuola, ti daremo solo metà della paghetta...", "" };
            PushMessaggio(msg);
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

            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Vieni pestato", Sostituisci(frasiMetallari[rndFrase],"{LUOGO}", vieStr[rndVia]), ""};
            PushMessaggio(msg);
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
                    Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Camionista bastardo", "[messaggio]", "" };
                    PushMessaggio(msg);
                    // DEBUG LOG
                    WriteLog("GestioneEventiCasuali: Scooter - Camionista...");

                }
                else
                {
                    // TODO: messaggio muro
                    m_tabbyGuy.GetScooter().DecStato(20);
                    Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Muro bastardo", "[messaggio]", "" };
                    PushMessaggio(msg);
                    // DEBUG LOG
                    WriteLog("GestioneEventiCasuali: Scooter - Muro...");
                }

                m_tabbyGuy.DecRep(2);

                if (m_tabbyGuy.GetScooter().GetStato() <= 0)
                {
                    m_tabbyGuy.GetScooter().Reset();
                    Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Scooter Distrutto", "Quando ti rialzi ti accorgi che il tuo scooter è ormai ridotto a un ammasso di rottami", "" };
                    PushMessaggio(msg);
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
            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Sei fortunato...", frasiFortuna[rndFrase], ""};
            PushMessaggio(msg);
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
                Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Scuola", Sostituisci(frasiScuola[rndFrase],"{MATERIA}",mat.GetNome()), ""};
                PushMessaggio(msg);

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
                Tipa tipa = GeneraTipa();
                // TODO: Dialog una tipa ci prova
                Messaggio msg{ TipoMsg::SCELTA, MsgAzione::TIPA_CI_PROVA, "Qualcuno ti caga...", "Una tipa, di nome "+tipa.GetNome()+" (Figosità"+std::to_string(tipa.GetFama())+"), ci prova con te...\nCi stai ???", "" };
                PushMessaggio(msg);
            }

            // TODO: if yes
            /*
                if ((figTemp >= 79) && (Rapporti < 1) && (sesso == 'M'))
                { // Se non hai già una tipa e rifiuti una figona...
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
                Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Due donne", "", "" };
                PushMessaggio(msg);
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
            if (m_tabbyGuy.HaTipa())
            {
                Messaggio msg{ TipoMsg::SCELTA, MsgAzione::TIPA_MI_AMI, "Domande inutili della tipa...", "Mi ami ???", "" };
                PushMessaggio(msg);

            }
            /*
            i = MessageBox(hInstance,
                                   "Mi ami ???",
                                   "Domande inutili della Tipa...", MB_YESNO | MB_ICONQUESTION);
                    if (i != IDYES)
                    {
                        MessageBox(hInstance,
                                   "Sei sempre il solito stronzo.. non capisco perchè resto ancora con uno come cosi'...",
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
            if (m_tabbyGuy.HaTipa())
            {
                Messaggio msg{ TipoMsg::SCELTA, MsgAzione::TIPA_INGRASSATA, "Domande inutili della tipa...", "Ma sono ingrassata ???", "" };
                PushMessaggio(msg);
            }
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

                Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Telefonino", "Il telefonino ti cade di tasca e vola per terra...", "" };
                PushMessaggio(msg);
                // DEBUG LOG
                WriteLog("GestioneEventiCasuali: Telefonino - Cade...");

            }
        }
    }
}

bool TabbyGame::PollMessaggi(Messaggio& outEvento)
{
    if (m_codaMsg.empty())
        return false;   // Nessun evento

    // Viene COPIATO il primo evento del vettore
    outEvento = m_codaMsg.front();
    // Viene rimosso dalla coda
    m_codaMsg.erase(m_codaMsg.begin());

    return true;    // C'è un evento da processare
}

void TabbyGame::ApplicaScelta(MsgAzione msgAzione, bool sceltaYes)
{
    switch (msgAzione)
    {
    case MsgAzione::CORROMPI:
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
                Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Errore critico", "Cosa ??? Prima cerchi di corrompermi, poi si scopre che non hai abbastanza soldi !!!", "" };
                PushMessaggio(msg);
            }
        }
        NuovoGiorno();
        break;

    case MsgAzione::GARA:
        if (sceltaYes)  // TODO: COMPLETA
        {
            /*
            if ((scooter[m_scooterAvversarioIndex].GetVelocita() + 80 + GenRandomInt(0, 39)) > (m_tabbyGuy.GetScooter().GetVelocita() + m_tabbyGuy.GetScooter().GetStato() + m_tabbyGuy.GetFortuna())
            {
                // Gara persa
                if(m_tabbyGuy.GetRep() > 80)
                    m_tabbyGuy.DecRep(3);
                if(m_tabbyGuy.GetRep() > 10)
                    m_tabbyGuy.DecRep(2);

                EventoDati ev{TipoEvento::INFO, msgAzione::BASE, "Hai perso, coglione", "Dopo pochi metri si vede l'inferiorità del tuo scooter...", "" };
                PushEvento(ev);
            }
            else
            {
                // Gara vinta
                m_tabbyGuy.IncRep(10);
            
                EventoDati ev{ TipoEvento::INFO, msgAzione::BASE, "Hai vinto", "Con il tuo scooter, bruci l'avversario in partenza...", "" };
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

    case MsgAzione::LICENZIATI:

        if (sceltaYes)
        {
            m_tabbyGuy.Licenziati();
            NuovoGiorno();
        }
        break;

    case MsgAzione::LASCIA_TIPA:
        if (sceltaYes)
        {
            // TODO: SUONO
            // TODO: SESSO
            if (m_tabbyGuy.GetTipa().GetFama() >= 79)
            {
                Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Coglione...", "Appena vengono a sapere quello che hai fatto, i tuoi amici ti prendono a scarpate.\nQualcuno più furbo di te, va a consolarla...", "" };
                PushMessaggio(msg);

                m_tabbyGuy.DecRep(8);
            }
            else if (m_tabbyGuy.GetTipa().GetFama() <= 40)
            {
                m_tabbyGuy.IncRep(4);
            }

            m_tabbyGuy.LasciaTipa();
            NuovoGiorno();
        }
        break;
    }
    
}

bool TabbyGame::TriggerScuola()
{
    if (m_tipoGiorno == TipoGiorno::NORMALE)
        return true;

    Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Scuola", "Non puoi andare a scuola in un giorno di vacanza!", "" };
    PushMessaggio(msg);

    return false;
}

bool TabbyGame::TriggerLavoro()
{
    if (m_tipoGiorno != TipoGiorno::FESTIVO)
        return true;

    Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Lavoro", "Arrivato davanti ai cancelli della ditta, li trovi irrimediabilmente chiusi...", "" };
    PushMessaggio(msg);

    return false;
}

void TabbyGame::AzioneStudia(int materiaIndex)
{
    // Doppio controllo
    if (!TriggerScuola())
        return;

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
    // Doppio controllo
    if (!TriggerScuola())
        return;

    Materia& mat = m_tabbyGuy.GetScuola().m_materie[materiaIndex];

    // TODO: Sesso??
    if (m_tabbyGuy.GetRep() >= 30 || GenRandomInt(1, 10) == 1)
        mat.IncVoto(2);
    else
    {
        m_tabbyGuy.DecRep(2);
        mat.DecVoto(1);

        // TODO: SUONO
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Bella figura", "Cosa ??? Credi di farmi paura piccolo pezzettino di letame vestito da zarro...\nDeve ancora nascere chi può minacciarmi...", "" };
        PushMessaggio(msg);
    }
    // IMPORTANTE: NEGLI EVENTI NULLI VA SEMPRE FATTO NUOVOGIORNO, MENTRE QUELLI A SCELTA CONVIENE IN APPLICASCELTA PER RISPETTARE L'ORDINE DI INVOCAZIONE EVENTI
    m_tabbyGuy.CalcolaStudio();
    NuovoGiorno();
}

void TabbyGame::AzioneCorrompi(int materiaIndex)
{
    // Doppio controllo
    if (!TriggerScuola())
        return;

    m_materiaIndex = materiaIndex;
    // Per corrompere un professore va da 75€ a 150€
    m_costoCorruzione = GenRandomInt(75,150);
    Messaggio msg{ TipoMsg::SCELTA, MsgAzione::CORROMPI, "Corrompi i professori", "Ma... forse per " + GetSoldiStr(m_costoCorruzione) + " potrei dimenticare i tuoi ultimi compiti in classe...", "" };
    PushMessaggio(msg);
}

// TODO: SISTEMA QUESTE FUNZIONI
void TabbyGame::AzioneGara()
{
    if (m_tabbyGuy.GetScooter().GetStato() <= 0)
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Ma che sei scemo ???", "Con quale scooter vorresti gareggiare, visto che non lo possiedi ?", "" };
        PushMessaggio(msg);
    }
    else if (m_tabbyGuy.GetScooter().GetAttivita() != Attivita::IN_GIRO)
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Svegliati...", "Purtroppo non puoi gareggiare visto che il tuo scooter è " + m_tabbyGuy.GetScooter().GetAttivitaStr(true) + ".", "" };
        PushMessaggio(msg);
    }
    else
    {
        // TODO: IL RANDOM VA FATTO SUL NUMERO DI SCOOTER IMPLEMENTATI
        /*
        int rnd = GenRandomInt(1, scootdim);
        m_scooterAvversarioIndex = rnd;
        // TODO: suono
        EventoDati ev{ TipoEvento::SCELTA, msgAzione::GARA, "Gareggia con lo scooter", "Accetti la sfida con un tabbozzo che ha un ", scooter[rnd].getnome() + " ?", "" };
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
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Sfigato", "Con la scarsa reputazione che hai, tutti trovano qualcosa di meglio da fare piuttosto che venire.", "" };
        PushMessaggio(msg);
    }
    else if (m_coolDownPestaggio > 0)    // TODO: Ragiona su sta roba
    {
        if (GenRandomInt(0, 1) == 1)
        {
            if (m_tabbyGuy.GetRep() < 80)
                m_tabbyGuy.IncRep(3);
            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Chiama la Compagnia", "Dopo aver visto i tuoi amici, chi ti ha picchiato non si farà più vedere in giro per un bel pezzo...", "" };
            PushMessaggio(msg);
        }
        else
        {
            if (m_tabbyGuy.GetRep() < 95)
                m_tabbyGuy.IncRep(5);
            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Chiama la Compagnia", "Anche i tuoi amici, al gran completo, vengono sacagnati di botte da chi ti aveva picchiato, accorgendosi così che non sei solo tu ad essere una chiavica, ma lo sono anche loro...", "" };
            PushMessaggio(msg);
        }
        NuovoGiorno();
    }
    else
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Chiama la Compagnia (perchè ?)", "Visto che non c'è nessuno da minacciare, tutti se ne vanno avviliti...", "" };
        PushMessaggio(msg);
    }
}

void TabbyGame::AzioneAumentoPaghetta()
{
    if (m_tabbyGuy.GetStudio() > 40)
    {
        if (((m_tabbyGuy.GetStudio() - m_tabbyGuy.GetPaghetta() + m_tabbyGuy.GetFortuna()) > (100 + GenRandomInt(0, 50)) && (m_tabbyGuy.GetPaghetta() < 50)))
        {
            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Aumento Paghetta !", "Va bene... ti daremo " + GetSoldiStr(5) + " di paghetta in più...", "" };
            PushMessaggio(msg);
            m_tabbyGuy.IncPaghetta(5);
            // TODO: RICORDA DI RESETTARE
        }
        else
        {
            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Errore irrecuperabile", "Vedi di scordartelo... Dovrà passare molto tempo prima che ti venga aumentata la paghetta...", "" };
            PushMessaggio(msg);
        }

        NuovoGiorno();
    }
    else
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Errore irrecuperabile", "Quando andrai meglio a scuola, forse...", "" };
        PushMessaggio(msg);
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
            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Non te li diamo, viziato", "Ma insomma ! Non puoi continuamente chiedere soldi ! Aspetta ancora qualche giorno. Fai qualche cosa di economico nel frattempo...", "" };
            PushMessaggio(msg);
        }

        NuovoGiorno();
    }
    else
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Errore irrecuperabile",
            "Quando andrai meglio a scuola potrai tornare a chiederci dei soldi, non ora. "
            "\nMa non lo sai che per la tua vita è importante studiare, e dovresti impegnarti "
            "di più, perchè quando ti impegni i risultati si vedono, solo che sei svogliato "
            "e non fai mai nulla, mi ricordo che quando ero giovane io era tutta un altra cosa..."
            "allora sì che i giovani studiavano...", "" };
        PushMessaggio(msg);
    }
}

void TabbyGame::AzioneChiediSoldi()
{
    Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Errore irrecuperabile", "Non pensarci neanche lontanamente...", "" };
    PushMessaggio(msg);
    NuovoGiorno();
}

// TODO: POSSIBILE CASINO DI DISTRUZIONE DELLA VARIABILE?
const Ditta& TabbyGame::ProponiDitta()
{
    int indiceLavoro = GenRandomInt(0, ditte.size() - 1);
    return ditte[indiceLavoro];
}

void TabbyGame::AzioneRifiutaProposta()
{
    Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Levati dai coglioni", "Allora vai a farti fottere...", "" };
    PushMessaggio(msg);
}

const QuizScheda& TabbyGame::AssegnaQuiz()
{
    int rnd = GenRandomInt(0, schede.size() - 1);
    return schede[rnd];
}

Tipa TabbyGame::GeneraTipa()
{
    int fama = GenRandomInt(30, 100);
    std::string nome = nomiTipe[GenRandomInt(0, nomiTipe.size() - 1)];
    return Tipa{ nome, fama };
}

void TabbyGame::AzioneProvaci(const Tipa& tipa)
{
    if ((tipa.GetFama() * 2 + GenRandomInt(0, 49)) <= (m_tabbyGuy.GetFama() + m_tabbyGuy.GetRep() + GenRandomInt(0, 29)))
    {
        // E' andata bene...
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "E' andata bene !", "Con il tuo fascino nascosto da tabbozzo, seduci la tipa e ti ci metti insieme.", "" };
        PushMessaggio(msg);
        // ...Ma comunque controlla che tu non abbia già una tipa
        if (m_tabbyGuy.HaTipa())
        {
            // TODO: DUE DONNE
        }
        else
        {   // Bravo, non hai una tipa
            m_tabbyGuy.SetTipa(tipa);
            m_tabbyGuy.SetRapporti(GenRandomInt(30, 45));
            m_tabbyGuy.IncFama(tipa.GetFama() * 0.1f);
        }
    }
    else
    {
        // Fai cagare...
        // TODO: SUONO
        // TODO: LOG 2 di picche
        m_tabbyGuy.DecRep(2);
        m_tabbyGuy.DecFama(2);

        // TODO: CARICA MESSAGGIO SPECIALE TIPA
        int rnd = GenRandomInt(0, frasiSfighe.size() - 1);
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Due di picche", frasiSfighe[rnd], "" };
        PushMessaggio(msg);
        // TODO: IMPLEMENTA LISTA FRASI
    }

    NuovoGiorno();
}

void TabbyGame::AzioneTerminaQuiz(const std::vector<int>& countRisposte, std::string nomeDitta)
{
    bool errore = false;
    for (int count : countRisposte)
    {
        if (count != 1) { // Deve essere esattamente 1 risposta
            errore = true;
            break;
        }
    }

    if (errore)
    {
        Messaggio msg{
            TipoMsg::INFO,
            MsgAzione::NONE,
            "Sei un po' stupido...",
            "Mi spieghi perchè dovremmo assumere qualcuno che non è neanche in grado di mettere delle crocette su un foglio ???",
            ""
        };
        PushMessaggio(msg);
    }
    else if (m_tabbyGuy.GetRep() + m_tabbyGuy.GetFortuna() + GenRandomInt(1, 80) > GenRandomInt(1, 200))
    {
        m_tabbyGuy.GetCarriera().SetImpegno(10 + GenRandomInt(0, 20));
        m_tabbyGuy.GetCarriera().SetGiorniLavorati(1);
        m_tabbyGuy.GetCarriera().SetStipendio(520 + GenRandomInt(0, 6) * 60);
        m_tabbyGuy.GetCarriera().SetDitta(nomeDitta);

        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Ora sei sfruttato !", "SEI STATO ASSUNTO ! Ora sei un felice dipendente della " + nomeDitta + " !", ""};
        PushMessaggio(msg);
    }
    else
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Sei inutile", "Mi dispiace ragazzo, ma non sei riuscito a superare il test... Ora puoi anche portare la tua brutta faccia fuori dal mio ufficio, prima che ti faccia buttare fuori a calci... Grazie e arrivederci...", "" };
        PushMessaggio(msg);

        if (m_tabbyGuy.GetRep() > 10)
            m_tabbyGuy.DecRep(2);

        NuovoGiorno();
    }
}

bool TabbyGame::AzioneCercaLavoro()
{
    if (!TriggerLavoro())
        return false;

    if (m_tabbyGuy.HaUnLavoro())
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Non ti permettere", "Forse non ti ricordi che hai già un lavoro...", "" };
        PushMessaggio(msg);
        return false;
    }

    return true;
}

void TabbyGame::AzioneLicenziati()
{
    if (!m_tabbyGuy.HaUnLavoro())
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Disabile", "Forse non ti ricordi che sei disokkupato...", "" };
        PushMessaggio(msg);
    }
    else
    {
        if (!TriggerLavoro())
            return;
        
        Messaggio msg{ TipoMsg::SCELTA, MsgAzione::LICENZIATI, "Licenziati", "Sei proprio sicuro di voler dare le dimissioni dalla " + m_tabbyGuy.GetCarriera().GetNomeDitta() + " ?", ""};
        PushMessaggio(msg);
    }
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

void TabbyGame::AzioneLavora()
{
    if (!m_tabbyGuy.HaUnLavoro())
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Disabile", "Forse non ti ricordi che sei disokkupato...", "" };
        PushMessaggio(msg);
    }
    else
    {
        if (!TriggerLavoro())
            return;

        if (m_tabbyGuy.GetCarriera().GetImpegno() < 85)
            m_tabbyGuy.GetCarriera().IncImpegno(1);

        // TODO: SUONO
        NuovoGiorno();
        // TODO: AGGIORNA LAVORO
    }
}

void TabbyGame::AzioneLeccaculo()
{
    if (!m_tabbyGuy.HaUnLavoro())
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Disabile", "Forse non ti ricordi che sei disokkupato...", "" };
        PushMessaggio(msg);
    }
    else
    {
        if (!TriggerLavoro())
            return;
        // TODO: PLAY SOUND + SESSO
        if (m_tabbyGuy.GetRep() > 20)    // Facendo il leccaculo perdi reputazione e fama...
            m_tabbyGuy.DecRep(1);

        if (m_tabbyGuy.GetCarriera().GetImpegno() < 99) // Perchè non ti impegnerai mai abbastanza...
            m_tabbyGuy.GetCarriera().IncImpegno(1);

        if (GenRandomInt(1, m_tabbyGuy.GetFortuna() + 3) == 1)
            NuovoGiorno();

        // TODO: AGGIORNA LAVORO
    }
}

void TabbyGame::AzioneAumentoSalario()
{
    if (!m_tabbyGuy.HaUnLavoro())
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Disabile", "Forse non ti ricordi che sei disokkupato...", "" };
        PushMessaggio(msg);
    }
    else
    {
        if (!TriggerLavoro())
            return;
        
        if (m_tabbyGuy.GetCarriera().GetImpegno() > 90)
            {
                if (m_tabbyGuy.GetFortuna() > (GenRandomInt(0, 49)))
                {
                    Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Aumento salario", "Forse per questa volta potremmo darti qualcosina in più...", "" };
                    PushMessaggio(msg);
                    m_tabbyGuy.GetCarriera().IncStipendio(GenRandomInt(1, 2) * 50);
                    m_tabbyGuy.GetCarriera().DecImpegno(30);
                }
                else
                {
                    // TODO: SESSO

                    Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Come osi...", "Vedi di scordartelo, bastardo...", "" };
                    PushMessaggio(msg);
                    m_tabbyGuy.GetCarriera().DecImpegno(20);
                }

                NuovoGiorno();
            }
            else
            {
                Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Sei impazzito ???", "Che cosa vorresti ??? SCORDATELO !!!", "" };
                PushMessaggio(msg);
            }
    }
}

void TabbyGame::AzioneSciopera()
{
    if (!m_tabbyGuy.HaUnLavoro())
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Disabile", "Forse non ti ricordi che sei disokkupato...", "" };
        PushMessaggio(msg);
    }
    else
    {
        if (!TriggerLavoro())
            return;

        // TODO: PLAY SOUND

        if (m_tabbyGuy.GetRep() < 85)
            m_tabbyGuy.IncRep(10);

        if (m_tabbyGuy.GetCarriera().GetImpegno() > 19)
            m_tabbyGuy.GetCarriera().DecImpegno(15);

        NuovoGiorno();
        // TODO: AGGIORNA LAVORO
    }
}

void TabbyGame::AzionePagaDisco(int discoIndex)
{
    const Disco& disco = discoteche[discoIndex];
    if (m_date.GetWeekDay() == disco.m_giornoChiuso)
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Giorno di chiusura", "Un cartello recita che oggi è il giorno di chiusura settimanale...", "" };
        PushMessaggio(msg);
        return;
    }

    if (disco.m_fuoriPorta && m_tabbyGuy.GetScooter().GetAttivita() != Attivita::IN_GIRO)
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Discoteca fuori porta", "Senza lo scooter non puoi andare nelle discoteche fuori porta...", "" };
        PushMessaggio(msg);
        return;
    }

    // TODO: SESSO
    if (m_tabbyGuy.GetFama() < disco.m_reqFama)
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Selezione all'ingresso", "Mi dispiace signore, conciato così, qui non può entrare...\nVenga vestito meglio la prossima volta, signore.", "" };
        PushMessaggio(msg);
        return;
    }

    // TODO: PREZZO RIDOTTO SE FEMMINA

    if (!m_tabbyGuy.SpendiSoldi(disco.m_prezzoIngresso))
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Bella figura di merda...", "Appena entraro ti accorgi di non avere abbastanza soldi per pagare il biglietto.\nUn energumeno buttafuori ti deposita gentilmente in un cassonetto della spazzatura poco distante dalla discoteca.", "" };
        PushMessaggio(msg);
    }
    else
    {
        // FINALMENTE VAI IN DISCO
        // TODO: SUONO
        WriteLog("AzionePagaDisco: Paga " + GetSoldiStr(disco.m_prezzoIngresso));
        m_tabbyGuy.IncFama(disco.m_incFama);
        m_tabbyGuy.IncRep(disco.m_incRep);
    }
    
    NuovoGiorno();
}

void TabbyGame::AzioneLasciaTipa()
{
    if (!m_tabbyGuy.HaTipa())
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Hai le allucinazioni ???", "Scusa, che ragazza avresti intenzione di lasciare ???", "" };
        PushMessaggio(msg);
        return;
    }

    // TODO: IMPLEMENTA SCELTA
    Messaggio msg{ TipoMsg::SCELTA, MsgAzione::LASCIA_TIPA, "Lascia tipa", "Sei proprio sicuro di voler lasciare "+m_tabbyGuy.GetTipa().GetNome()+" ?", "" };
    PushMessaggio(msg);
}

void TabbyGame::AzioneEsciTipa()
{
    // TODO: SESSO
    if (!m_tabbyGuy.HaTipa())
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Non sei molto intelligente...", "Scusa, con che tipa vorresti uscire ???", "" };
        PushMessaggio(msg);
        return;
    }

    if (!m_tabbyGuy.HaScooter())
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Compra lo scooter", "Finché non comprerai lo scooter, non usciremo più insieme...", "" };
        PushMessaggio(msg);
        return;
    }

    if (m_tabbyGuy.GetScooter().GetAttivita() != Attivita::IN_GIRO)
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Risistema lo scooter", "Finché il tuo scooter resterà "+m_tabbyGuy.GetScooter().GetAttivitaStr(true) + " non potremo uscire insieme...", ""};
        PushMessaggio(msg);
        return;
    }

    if (m_tabbyGuy.GetScooter().GetStato() <= 35)
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Ripara lo scooter", "Finché non riparerai lo scooter, non usciremo più insieme...", "" };
        PushMessaggio(msg);
        return;
    }

    // FINALMENTE ESCI CON LA TIPA
    if (!m_tabbyGuy.SpendiSoldi(10))
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Sei povero", "Se mi vuoi portare fuori, cerca di avere almeno un po' di soldi...", "" };
        PushMessaggio(msg);
        return;
    }

    WriteLog("AzioneEsciTipa: Esci con la tipa " + GetSoldiStr(10));

    m_tabbyGuy.IncRapporti(5);
    if (m_tabbyGuy.GetTipa().GetFama() > m_tabbyGuy.GetFama())
        m_tabbyGuy.IncFama(1);

    m_tabbyGuy.GetScooter().DecBenzina(3);
    // TODO: CALCOLA VELOCITA
}

void TabbyGame::AzioneTelefonaTipa()
{
    if (!m_tabbyGuy.HaTipa())
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Non sei molto intelligente...", "Scusa, che ragazza vorresti chiamare ???", "" };
        PushMessaggio(msg);
        return;
    }

    if (!m_tabbyGuy.HaTelefono() && m_tabbyGuy.GetSoldi() <= 5 && m_tabbyGuy.GetOperatore().GetCredito() < 2)
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Non toccare quel telefono...", "\"Se ti azzardi a fare anche una singola telefonata, ti spezzo le gambe\", disse tuo padre con un'accetta in mano...", "" };
        PushMessaggio(msg);
        return;
    }

    // TODO: PLAY SUONO
    // TODO: LOGICA STRANA
    if (m_tabbyGuy.HaTelefono() && m_tabbyGuy.GetOperatore().GetCredito() <= 2)
        m_tabbyGuy.GetOperatore().DecCredito(2);
    else
        m_tabbyGuy.SpendiSoldi(5);

    WriteLog("AzioneTelefonaTipa: Telefona alla tipa " + GetSoldiStr(5));

    if (m_tabbyGuy.GetRapporti() <= 60)
        m_tabbyGuy.IncRapporti(1);
}

bool TabbyGame::TriggerNegozio(TipoProd merce)
{
    if (m_tipoGiorno == TipoGiorno::FESTIVO)
    {
        std::string str{};
        if (merce == TipoProd::SIGARETTE)
            str = "Rimani fisso a guardare la saracinesca del tabaccaio irrimediabilmente chiusa...";
        else
            str = "Oh, tipo... i negozi sono chiusi di festa...";

        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Torna a casa", str, "" };
        PushMessaggio(msg);
        return false;
    }

    return true;
}

// TODO: PASSARE NEGOZIO ??
void TabbyGame::AzioneCompra(const Prodotto& prod)
{
    // TODO: IMPLEMENTA

    if (!m_tabbyGuy.SpendiSoldi(prod.GetPrezzo()))
    {
        std::string str{};
        switch (prod.GetTipoProd())
        {
        case TipoProd::GIUBBOTTO:
        case TipoProd::PANTALONI:
        case TipoProd::SCARPE:
            str = "Con cosa avresti intenzione di pagare, stronzetto ??? Caramelle ??? ";
            break;
        case TipoProd::SIGARETTE:
            str = "\"Vai fuori dal mio locale, brutto pezzente !\", esclama il tabaccaio con un'AK-47 in mano...";
            break;
        }

        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Non hai abbastanza soldi...", str, "" };
        PushMessaggio(msg);

        return;
    }

    switch (prod.GetTipoProd())
    {
    case TipoProd::GIUBBOTTO:
        m_tabbyGuy.SetGiubbotto(prod);
        m_tabbyGuy.IncFama(prod.GetFama());
        break;
    case TipoProd::PANTALONI:
        m_tabbyGuy.SetPantaloni(prod);
        m_tabbyGuy.IncFama(prod.GetFama());
        break;
    case TipoProd::SCARPE:
        m_tabbyGuy.SetScarpe(prod);
        m_tabbyGuy.IncFama(prod.GetFama());
        break;
    case TipoProd::SIGARETTE:
    {
        m_tabbyGuy.IncSizze(20);
        m_tabbyGuy.IncFama(prod.GetFama());
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "ART. 46 L. 29/12/1990 N.428", frasiSigarette[GenRandomInt(0,frasiSigarette.size())], "" };
        PushMessaggio(msg);
        break;
    }
    }

    WriteLog("AzioneCompra: acquista " + prod.GetNome() + " per " + GetSoldiStr(prod.GetPrezzo()));
    NuovoGiorno();
}