#define _CRT_SECURE_NO_WARNINGS // Senza questa mi fotte la funzione localtime
#include <fstream>              // Gestione file
#include <ctime>                // Per l'orario nel log
#include <iomanip>              // Per formattare l'orario
#include <iostream>             // Per cerr (errori)
#include <sstream>
#include "TabbyGame.h"

Messaggio::Messaggio() {}

Messaggio::Messaggio(TipoMsg tipo, MsgAzione id, std::string titolo, std::string testo, std::string img)
    : m_tipo{ tipo }, m_msgAzione{ id }, m_titolo{ titolo }, m_testo{ testo }, m_immagine{ img }
{}

TabbyGame::TabbyGame()	// Lunedì 16 settembre 1991
	: m_tabbyGuy{}, m_date{1991, 9, 16}, 
    m_valutaCorrente{Valuta::EURO}, 
    m_coolDownPestaggio{ 5 }, 
    m_tipoGiorno{ TipoGiorno::NORMALE },
    m_attesa{ATTESA_MAX}
{
    CaricaStringhe();
    CaricaAbbonamenti();
    CaricaDiscoteche();
    CaricaDitte();
    CaricaFeste();
    CaricaNegozi();
    CaricaQuiz();

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

    // Ogni 7 giorni cala l'abbronzatura
    if (m_tabbyGuy.GetPelle() != Pelle::ABBR_NO)
    {
        m_coolDownPelle--;
        if (m_coolDownPelle < 0)
        {
            m_tabbyGuy.Sbianca();
            m_coolDownPelle = 7;
        }
    }

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
    if (m_date == m_scadenzaPal)
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
    for (const auto& festa : m_feste)
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
    if (m_tabbyGuy.HaTelefono() && m_tabbyGuy.GetTelefono().GetCredito() > 0)
    {
        m_tabbyGuy.GetTelefono().DecCredito(1);
        if (m_tabbyGuy.GetFama() < 55)
            m_tabbyGuy.IncFama(1);

        if (m_tabbyGuy.GetTelefono().GetCredito() == 0)
        {
            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Telefonino", "Cerchi di telefonare e ti accorgi di aver finito i soldi a tua disposizione...", "" };
            PushMessaggio(msg);
        }
        else if (m_tabbyGuy.GetTelefono().GetCredito() < 3)
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
        // TODO: AZZERA TELEFONO

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

    if (m_tabbyGuy.HaTipa())
    {
        if (m_tabbyGuy.GetRapporti() < 98)
        {
            int rand_max = (m_tabbyGuy.GetRapporti() + m_tabbyGuy.GetFortuna() + m_tabbyGuy.GetFama()) * 3 + 1;
            rnd = GenRandomInt(1, rand_max);

            if (rnd < 11)
            {
                // Da 1 a 10, la donna ti molla...
                // TODO: Play sound

                m_tabbyGuy.LasciaTipa();

                // TODO: Dialog la tipa ti molla - sesso m f
                rnd = GenRandomInt(0, m_frasiSeparazione.size() - 1);
                Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "La tipa ti molla...", m_frasiSeparazione[rnd], ""};
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
            int rndFrase = GenRandomInt(0, m_frasiMetallari.size() - 1);
            int rndVia = GenRandomInt(0, m_vieStr.size() - 1);

            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Vieni pestato", Sostituisci(m_frasiMetallari[rndFrase],"{LUOGO}", m_vieStr[rndVia]), ""};
            PushMessaggio(msg);
            // DEBUG LOG
            WriteLog("GestioneEventiCasuali: Metallaro n. " + std::to_string(rndFrase));

            m_coolDownPestaggio = 5;
        }
        else if (11 <= caso && caso <= 20)   // SCOOTER
        {
            if (m_tabbyGuy.HaScooter() && m_tabbyGuy.GetScooter().GetAttivita() == Attivita::IN_GIRO)
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
                    m_tabbyGuy.GetScooter().Azzera();
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

            int rndFrase = GenRandomInt(0, m_frasiFortuna.size() - 1);
            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Sei fortunato...", m_frasiFortuna[rndFrase], ""};
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
                int rndFrase = GenRandomInt(0, m_frasiScuola.size() - 1);
                Materia& mat = m_tabbyGuy.GetScuola().m_materie[rndMat];
                Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Scuola", Sostituisci(m_frasiScuola[rndFrase],"{MATERIA}",mat.GetNome()), ""};
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
                Messaggio msg{ TipoMsg::SCELTA, MsgAzione::TIPA_CI_PROVA, "Qualcuno ti caga...", "Una tipa, di nome "+tipa.GetNome()+" (Figosità "+std::to_string(tipa.GetFama())+"/100), ci prova con te...\nCi stai ???", "" };
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
        m_tabbyGuy.GetScooter().DecBenza(5);
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
    case MsgAzione::VENDI_TEL:
        if (sceltaYes)
        {
            // Trasferiamo sim e credito
            Abbonamento abb = m_tabbyGuy.GetTelefono().GetAbbonamento();
            long long cred = m_tabbyGuy.GetTelefono().GetCredito();

            m_tabbyGuy.GetTelefono().Azzera();

            m_tabbyGuy.GetTelefono().IncCredito(cred);
            m_tabbyGuy.GetTelefono().GetAbbonamento() = abb;
            m_tabbyGuy.GuadagnaSoldi(m_offertaTel);
        }
        else
        {
            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Telefonino", "Allora vai a farti fottere, pirletta !", "" };
            PushMessaggio(msg);
        }
        break;

    case MsgAzione::RIPARA_SCOOTER:
        if (sceltaYes)
        {
            if (m_tabbyGuy.SpendiSoldi(m_costoRiparazione))
            {
                // TODO: SUONO
                m_tabbyGuy.GetScooter().Ripara();
                WriteLog("AzioneRiparaScooter: Paga riparazione " + GetSoldiStr(m_costoRiparazione));
            }
            else
            {
                Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Non hai abbastanza soldi...", "L'enorme meccanico ti afferra con una sola mano, ti riempie di pugni, e non esita a scaraventare te e il tuo motorino merdoso fuori dall'officina.", "" };
                PushMessaggio(msg);
            }
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
    int indiceLavoro = GenRandomInt(0, m_ditte.size() - 1);
    return m_ditte[indiceLavoro];
}

void TabbyGame::AzioneRifiutaProposta()
{
    Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Levati dai coglioni", "Allora vai a farti fottere...", "" };
    PushMessaggio(msg);
}

const QuizScheda& TabbyGame::AssegnaQuiz()
{
    int rnd = GenRandomInt(0, m_schede.size() - 1);
    return m_schede[rnd];
}

Tipa TabbyGame::GeneraTipa()
{
    int fama = GenRandomInt(30, 100);
    std::string nome = m_nomiTipe[GenRandomInt(0, m_nomiTipe.size() - 1)];
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
        int rnd = GenRandomInt(0, m_frasiSfighe.size() - 1);
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Due di picche", m_frasiSfighe[rnd], "" };
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

// Funzione helper locale per rimuovere spazi o caratteri di a capo residui (es. \r su Windows)
static void trimString(std::string& s) {
    while (!s.empty() && (s.back() == '\r' || s.back() == '\n' || s.back() == ' ')) {
        s.pop_back();
    }
}

void TabbyGame::CaricaStringhe()
{
    std::ifstream file("dati/strings.txt");

    if (!file.is_open())
    {
        WriteLog("ERRORE CRITICO: Impossibile trovare dati/strings.txt");
        return;
    }

    std::string riga;
    std::vector<std::string>* vettoreCorrente = nullptr; // Puntatore al vettore che stiamo riempiendo - ATTENZIONE: qua non posso usare la reference, perchè in C++ non possono essere riassegnate

    while (std::getline(file, riga))
    {
        trimString(riga); // Pulisce spazi finali o \r

        // Salta righe vuote
        if (riga.empty()) continue;

        // Se la riga inizia con '[' e finisce con ']', è un TAG (es. [METALLARI])
        if (riga.front() == '[' && riga.back() == ']')
        {
            // Estrai il nome del tag (togli le parentesi)
            std::string tag = riga.substr(1, riga.size() - 2);

            // Selettore del vettore attivo
            if (tag == "METALLARI")      vettoreCorrente = &m_frasiMetallari;
            else if (tag == "SCUOLA")    vettoreCorrente = &m_frasiScuola;
            else if (tag == "FORTUNA")   vettoreCorrente = &m_frasiFortuna;
            else if (tag == "SFIGHE")    vettoreCorrente = &m_frasiSfighe;
            else if (tag == "SEPARAZIONE") vettoreCorrente = &m_frasiSeparazione;
            else if (tag == "PALESTRA")  vettoreCorrente = &m_frasiPalestra;
            else if (tag == "SIGARETTE") vettoreCorrente = &m_frasiSigarette;
            else if (tag == "NOMI_TIPE") vettoreCorrente = &m_nomiTipe;
            else if (tag == "VIE")       vettoreCorrente = &m_vieStr;
            else {
                vettoreCorrente = nullptr; // Tag sconosciuto, ignora le righe successive
                WriteLog("WARNING: Tag sconosciuto nel file frasi: " + tag);
            }
        }
        else
        {
            // E' una frase
            if (vettoreCorrente != nullptr)
                vettoreCorrente->push_back(riga);
        }
    }

    file.close();
    WriteLog("Caricamento frasi completato da file unico.");
}


std::vector<std::string> SplitString(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        // Rimuovi eventuali spazi bianchi iniziali/finali
        tokens.push_back(token);
    }
    return tokens;
}

// Helper per convertire stringa in enum/int senza impazzire con le eccezioni
int ParseInt(const std::string& s) {
    try { return std::stoi(s); }
    catch (...) { return 0; }
}
long long ParseLong(const std::string& s) {
    try { return std::stoll(s); }
    catch (...) { return 0; }
}

void TabbyGame::CaricaAbbonamenti()
{
    m_abbonamenti.clear();
    std::ifstream file("dati/abbonamenti.txt");
    if (!file.is_open()) return;

    std::string riga;
    while (std::getline(file, riga))
    {
        trimString(riga);
        if (riga.empty() || riga[0] == '#') continue;

        auto tokens = SplitString(riga, '|');
        // FORMATO: Nome | Img | CostoFisso | CostoVariabile1 | CostoVariabile2
        if (tokens.size() >= 5)
        {
            std::string nome = tokens[0];
            std::string img = tokens[1];
            long long fisso = ParseLong(tokens[2]);
            long long var1 = ParseLong(tokens[3]);
            long long var2 = ParseLong(tokens[4]);

            // Costruttore: Nome, Img, CostoAttivazione, VectorRicariche
            Abbonamento abb(nome, img, fisso, { var1, var2 });
            m_abbonamenti.push_back(abb);
        }
    }
}

void TabbyGame::CaricaDiscoteche() {
    m_discoteche.clear();
    std::ifstream file("dati/discoteche.txt");
    if (!file.is_open()) { WriteLog("ERR: discoteche.txt missing"); return; }

    std::string riga;
    while (std::getline(file, riga)) {
        trimString(riga);
        if (riga.empty() || riga[0] == '#') continue; // Salta commenti e vuote

        auto tokens = SplitString(riga, '|');
        if (tokens.size() >= 8) {
            Disco d;
            d.m_nome = tokens[0];
            d.m_descrizione = tokens[1];
            d.m_giornoChiuso = (Chrono::WeekDay)ParseInt(tokens[2]);
            d.m_fuoriPorta = (bool)ParseInt(tokens[3]);
            d.m_reqFama = ParseInt(tokens[4]);
            d.m_prezzoIngresso = ParseLong(tokens[5]);
            d.m_incFama = ParseInt(tokens[6]);
            d.m_incRep = ParseInt(tokens[7]);

            m_discoteche.push_back(d);
        }
    }
}

void TabbyGame::CaricaDitte() {
    m_ditte.clear();
    std::ifstream file("dati/ditte.txt");
    std::string riga;

    while (std::getline(file, riga)) {
        trimString(riga);
        if (riga == "[DITTA]") {
            Ditta d;

            // Riga 1: Dati Base
            if (!std::getline(file, riga)) break;
            auto tokens = SplitString(riga, '|');
            if (tokens.size() < 3) continue;

            d.m_nome = tokens[0];
            d.m_sede = tokens[1];
            d.m_fatturato = ParseLong(tokens[2]);

            // Riga 2: Offerta
            if (!std::getline(file, riga)) break;
            auto offTokens = SplitString(riga, '|');
            if (offTokens.size() >= 3) {
                d.m_offerta.m_descrizione = offTokens[0];
                d.m_offerta.m_accettaStr = offTokens[1];
                d.m_offerta.m_rifiutaStr = offTokens[2];
            }

            // Riga 3: Presentazione
            if (!std::getline(file, riga)) break;
            d.m_presentazione = riga;

            // Riga 4: Produzioni
            if (!std::getline(file, riga)) break;
            d.m_produzioni = riga;

            m_ditte.push_back(d);
        }
    }
}

void TabbyGame::CaricaFeste()
{
    m_feste.clear();
    std::ifstream file("dati/feste.txt");
    if (!file.is_open()) return;

    std::string riga;
    while (std::getline(file, riga))
    {
        trimString(riga);
        if (riga.empty() || riga[0] == '#') continue;

        auto tokens = SplitString(riga, '|');
        // FORMATO: Giorno | Mese | Nome | Messaggio
        if (tokens.size() >= 4)
        {
            FestaFissa f;
            f.m_giorno = ParseInt(tokens[0]);
            f.m_mese = ParseInt(tokens[1]);
            f.m_nome = tokens[2];
            f.m_messaggio = tokens[3];
            m_feste.push_back(f);
        }
    }
}

void TabbyGame::CaricaNegozi() {
    // IMPORTANTE: Se usi puntatori grezzi nel vettore, devi prima pulirli!
    // (Se hai il distruttore in Negozio come ti ho detto prima, basta clear())
    m_negozi.clear();

    std::ifstream file("dati/negozi.txt");
    if (!file.is_open()) return;

    Negozio* currentNegozio = nullptr;
    std::string riga;

    while (std::getline(file, riga)) {
        trimString(riga);
        if (riga.empty() || riga[0] == '#') continue;

        auto tokens = SplitString(riga, '|');
        if (tokens.empty()) continue;

        // --- NUOVO NEGOZIO ---
        if (tokens[0] == "[NEGOZIO]") {
            Negozio n;
            n.m_nome = tokens[1];
            // Cast int -> Enum Categoria
            n.m_merce = (CategoriaOggetto)ParseInt(tokens[2]);

            m_negozi.push_back(n);
            currentNegozio = &m_negozi.back(); // Puntiamo all'ultimo inserito
        }
        else if (tokens[0] == "[TELEFONIA]")
        {
            m_telefonia.m_nome = tokens[1];
            // Cast int -> Enum Categoria
            m_telefonia.m_merce = (CategoriaOggetto)ParseInt(tokens[2]);
            currentNegozio = &m_telefonia;
        }
        else if (tokens[0] == "[CONCESSIONARIO]")
        {
            m_concessionario.m_nome = tokens[1];
            m_concessionario.m_merce = (CategoriaOggetto)ParseInt(tokens[2]);
            currentNegozio = &m_concessionario;
        }
        else if (tokens[0] == "[MECCANICO]")
        {
            m_meccanico.m_nome = tokens[1];
            m_meccanico.m_merce = (CategoriaOggetto)ParseInt(tokens[2]);
            currentNegozio = &m_meccanico;
        }
        // --- ITEM (OGGETTO) ---
        else if (tokens[0] == "ITEM" && currentNegozio != nullptr) {
            std::string tipo = tokens[1];
            Acquistabile* nuovoItem = nullptr;

            if (tipo == "VESTITO" && tokens.size() >= 8) {
                // ITEM|VESTITO|TipoVest|Nome|Desc|Img|Prezzo|Fama
                TipoVestito tv = (TipoVestito)ParseInt(tokens[2]);
                nuovoItem = new Vestito(tv, tokens[3], tokens[4], tokens[5], ParseLong(tokens[6]), ParseInt(tokens[7]));
            }
            else if (tipo == "SIZZE" && tokens.size() >= 7) {
                // ITEM|SIZZE|Nome|Desc|Img|Prezzo|Fama
                nuovoItem = new Sizze(tokens[2], tokens[3], tokens[4], ParseLong(tokens[5]), ParseInt(tokens[6]));
            }
            else if (tipo == "TEL" && tokens.size() >= 8) {
                nuovoItem = new Telefono{ tokens[2], tokens[3], tokens[4], ParseLong(tokens[5]), ParseInt(tokens[6]), ParseInt(tokens[7]), 0, Abbonamento() };
            }
            else if (tipo == "SCOOTER" && tokens.size() >= 8)
            {
                nuovoItem = new Scooter{ tokens[2], tokens[3], ParseLong(tokens[4]), ParseInt(tokens[5]), ParseInt(tokens[6]), ParseInt(tokens[7]), 5.0f };
            }
            else if (tipo == "PEZZO" && tokens.size() >= 6)
            {
                TipoPezzo tp = (TipoPezzo)ParseInt(tokens[2]);
                nuovoItem = new Pezzo{ tp, tokens[3], tokens[4], ParseLong(tokens[5]) };
            }

            if (nuovoItem) {
                currentNegozio->m_catalogo.push_back(nuovoItem);
            }
        }
    }
}

void TabbyGame::CaricaQuiz()
{
    m_schede.clear();
    std::ifstream file("dati/quiz.txt");
    if (!file.is_open()) return;

    std::string riga;
    QuizScheda* currentQuiz = nullptr;

    // Macchina a stati per parsare il quiz
    enum State { FIND_QUIZ, READ_TITLE, READ_INTRO, READ_QUESTIONS };
    State state = FIND_QUIZ;

    while (std::getline(file, riga))
    {
        trimString(riga);
        if (riga.empty()) continue;

        if (riga == "[QUIZ]")
        {
            // Inizia nuova scheda
            m_schede.push_back(QuizScheda{});
            currentQuiz = &m_schede.back();
            state = READ_TITLE;
            continue;
        }

        if (currentQuiz == nullptr) continue;

        if (state == READ_TITLE)
        {
            currentQuiz->m_titolo = riga;
            state = READ_INTRO;
        }
        else if (state == READ_INTRO)
        {
            currentQuiz->m_intro = riga;
            state = READ_QUESTIONS;
        }
        else if (state == READ_QUESTIONS)
        {
            // Cerchiamo [DOMANDA]|Testo
            if (riga.find("[DOMANDA]|") == 0)
            {
                std::string testoDomanda = riga.substr(10); // Salta "[DOMANDA]|"

                // Leggiamo subito la riga successiva per le risposte
                std::string rigaRisposte;
                if (std::getline(file, rigaRisposte))
                {
                    trimString(rigaRisposte);
                    auto risposte = SplitString(rigaRisposte, '|');

                    if (risposte.size() >= 3)
                    {
                        // Aggiunge la domanda alla scheda corrente
                        // QuizDomanda { testo, vector<string> risposte }
                        currentQuiz->m_domande.push_back({ testoDomanda, risposte });
                    }
                }
            }
        }
    }
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
    const Disco& disco = m_discoteche[discoIndex];
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

    m_tabbyGuy.GetScooter().DecBenza(3);
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

    if (!m_tabbyGuy.HaTelefono() && m_tabbyGuy.GetSoldi() <= 5 && m_tabbyGuy.GetTelefono().GetCredito() < 2)
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Non toccare quel telefono...", "\"Se ti azzardi a fare anche una singola telefonata, ti spezzo le gambe\", disse tuo padre con un'accetta in mano...", "" };
        PushMessaggio(msg);
        return;
    }

    // TODO: PLAY SUONO
    // TODO: LOGICA STRANA
    if (m_tabbyGuy.HaTelefono() && m_tabbyGuy.GetTelefono().GetCredito() <= 2)
        m_tabbyGuy.GetTelefono().DecCredito(2);
    else
        m_tabbyGuy.SpendiSoldi(5);

    WriteLog("AzioneTelefonaTipa: Telefona alla tipa " + GetSoldiStr(5));

    if (m_tabbyGuy.GetRapporti() <= 60)
        m_tabbyGuy.IncRapporti(1);
}

bool TabbyGame::TriggerNegozio(CategoriaOggetto merce)
{
    if (m_tipoGiorno == TipoGiorno::FESTIVO)
    {
        std::string str{};
        switch (merce)
        {
        case CategoriaOggetto::CONSUMABILE:
            str = "Rimani fisso a guardare la saracinesca del tabaccaio irrimediabilmente chiusa...";
            break;
        case CategoriaOggetto::SCOOTER:
            str = "Oh, tipo... oggi il concessionario è chiuso...";
            break;
        case CategoriaOggetto::SCOOTER_PART:
            str = "Oh, tipo... oggi il meccanico è chiuso...";
            break;
        default:
            str = "Oh, tipo... i negozi sono chiusi di festa...";
        }

        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Torna a casa", str, "" };
        PushMessaggio(msg);
        return false;
    }

    return true;
}

// TODO: PASSARE NEGOZIO ??
void TabbyGame::AzioneCompra(const Acquistabile& prod)
{
    // TODO: IMPLEMENTA

    if (!m_tabbyGuy.SpendiSoldi(prod.GetPrezzo()))
    {
        std::string str{};
        switch (prod.GetCategoria())
        {
        case CategoriaOggetto::VESTITO:
            str = "Con cosa avresti intenzione di pagare, stronzetto ??? Caramelle ??? ";
            break;
        case CategoriaOggetto::CONSUMABILE:
            str = "\"Vai fuori dal mio locale, brutto pezzente !\", esclama il tabaccaio con un'AK-47 in mano...";
            break;
        case CategoriaOggetto::TELEFONO:
            str = "Forse non ti sei accorto di non avere abbastanza soldi, stronzetto...";
            break;
        case CategoriaOggetto::SCOOTER:
            str = "Ti piacerebbe comprare lo scooter, vero ?\nPurtroppo, non hai abbastanza soldi...";
            break;
        case CategoriaOggetto::SCOOTER_PART:
            str = "L'enorme meccanico ti afferra con una sola mano, ti riempie di pugni, e non esita a scaraventare te e il tuo motorino merdoso fuori dall'officina.";
            break;
        default:
            str = "Te sei bruciato tutti i soldi, coglione...";
        }

        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Non hai abbastanza soldi...", str, "" };
        PushMessaggio(msg);

        return;
    }

    if (prod.GetCategoria() == CategoriaOggetto::VESTITO)
    {
        // VIENE EFFETTUATO UN CAST STATICO PER REPERIRE LE INFORMAZIONI DEL VESTITO
        const Vestito& v{ static_cast<const Vestito&>(prod) };
        switch (v.GetTipoVestito())
        {
        case TipoVestito::GIUBBOTTO:
            m_tabbyGuy.SetGiubbotto(v);
            m_tabbyGuy.IncFama(v.GetFama());
            break;
        case TipoVestito::PANTALONI:
            m_tabbyGuy.SetPantaloni(v);
            m_tabbyGuy.IncFama(v.GetFama());
            break;
        case TipoVestito::SCARPE:
            m_tabbyGuy.SetScarpe(v);
            m_tabbyGuy.IncFama(v.GetFama());
            break;
        }
    }
    else if (prod.GetCategoria() == CategoriaOggetto::CONSUMABILE)
    {
        const Sizze& sizze{ static_cast<const Sizze&>(prod) };
        m_tabbyGuy.IncSizze(20);
        m_tabbyGuy.IncFama(sizze.GetFama());
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "ART. 46 L. 29/12/1990 n. 428", m_frasiSigarette[GenRandomInt(0, m_frasiSigarette.size()-1)], "" };
        PushMessaggio(msg);
    }
    else if (prod.GetCategoria() == CategoriaOggetto::TELEFONO)
    {
        const Telefono& tel{ static_cast<const Telefono&>(prod) };
        // Trasferiamo la sim e il credito
        Abbonamento abb = m_tabbyGuy.GetTelefono().GetAbbonamento();
        long long cred = m_tabbyGuy.GetTelefono().GetCredito();

        m_tabbyGuy.GetTelefono() = tel;
        m_tabbyGuy.GetTelefono().GetAbbonamento() = abb;
        m_tabbyGuy.GetTelefono().IncCredito(cred);
        m_tabbyGuy.IncFama(tel.GetFama());
    }

    WriteLog("AzioneCompra: acquista " + prod.GetNome() + " per " + GetSoldiStr(prod.GetPrezzo()));
    NuovoGiorno();
}

bool TabbyGame::TriggerPalestra()
{
    if (m_tipoGiorno != TipoGiorno::FESTIVO)
        return true;

    Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Palestra", "Il tuo fisico da atleta dovrà aspettare... visto che oggi la palestra è chiusa...", "" };
    PushMessaggio(msg);

    return false;
}

void TabbyGame::AzioneVaiPalestra()
{
    if (!PalestraAttiva())
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Palestra", "Prima di poter venire in palestra devi fare un abbonamento !", "" };
        PushMessaggio(msg);
        return;
    }

    // TODO: SUONO
    if (m_tabbyGuy.GetFama() < 82)
        m_tabbyGuy.IncFama(1);

    // EVENTI PALESTRA
    int rnd = GenRandomInt(0, 28 + m_tabbyGuy.GetFortuna() * 0.5f);

    // AVVIENE L'EVENTO RANDOMICO
    if (rnd < 9)
    {
        rnd = GenRandomInt(0, m_frasiPalestra.size() - 1);
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Palestra...", m_frasiPalestra[rnd], "" };
        PushMessaggio(msg);

        if (m_tabbyGuy.GetRep() > 10)
            m_tabbyGuy.DecRep(4);

        WriteLog("AzioneVaiPalestra: Evento riguardante la palestra");
    }
}

void TabbyGame::AzioneLampada()
{
    if (m_tabbyGuy.GetPelle() != Pelle::ABBR_CARBONIZZATO)
    {
        if (m_tabbyGuy.SpendiSoldi(PREZZO_LAMPADA))
        {
            m_tabbyGuy.Abbronza();
            m_coolDownPelle = 7;

            if (m_tabbyGuy.GetPelle() != Pelle::ABBR_CARBONIZZATO)
            {
                if (m_tabbyGuy.GetFama() < 20)
                    m_tabbyGuy.IncFama(1);
                if (m_tabbyGuy.GetFama() < 45)
                    m_tabbyGuy.IncFama(1);
                if (m_tabbyGuy.GetFama() < 96)
                    m_tabbyGuy.IncFama(1);

            }
            else
            {
                m_tabbyGuy.DecFama(8);
                m_tabbyGuy.DecRep(5);

                Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Lampada", "L'eccessiva esposizione del tuo corpo ai raggi ultravioletti provoca un avanzato grado di carbonizzazione e pure qualche piccola mutazione genetica...", "" };
                PushMessaggio(msg);
            }

            // TODO: SUONO

            WriteLog("AzioneLampada: Paga " + GetSoldiStr(PREZZO_LAMPADA));
        }
        else
        {
            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Non hai abbastanza soldi...", "L'enorme istruttore di bodybulding ultra-palestrato ti suona come una zampogna e ti scaraventa fuori dalla palestra.", "" };
            PushMessaggio(msg);
        }
    }

    int rnd = GenRandomInt(0, 4 + m_tabbyGuy.GetFortuna());
    if (rnd == 0)
        NuovoGiorno();
}

void TabbyGame::AzioneAbbonamento(int mesi)
{
    long long importo{};
    switch (mesi)
    {
    case 1:
        importo = PALESTRA_ABB_1;
        break;
    case 6:
        importo = PALESTRA_ABB_6;
        break;
    case 12:
        importo = PALESTRA_ABB_12;
        break;
    }

    if (PalestraAttiva())   // Hai già un abbonamento
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Ma che ?", "Hai già un abbonamento, perché te ne serve un altro ???", "" };
        PushMessaggio(msg);
        return;
    }

    if (!m_tabbyGuy.SpendiSoldi(importo))
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Non hai abbastanza soldi...", "L'enorme istruttore di bodybulding ultra-palestrato ti suona come una zampogna e ti scaraventa fuori dalla palestra.", "" };
        PushMessaggio(msg);
        return;
    }

    // Abbonamento acquistato
    m_scadenzaPal = m_date;
    m_scadenzaPal.AddMonth(mesi);
    NuovoGiorno();
}

void TabbyGame::AzioneVendiTelefono()
{
    if (!m_tabbyGuy.HaTelefono())
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Ma che ???", "Che telefonino vuoi vendere, pirletta ???", "" };
        PushMessaggio(msg);
        return;
    }

    m_offertaTel = m_tabbyGuy.GetTelefono().GetPrezzo() * 0.5 + 8;

    Messaggio msg{ TipoMsg::SCELTA, MsgAzione::VENDI_TEL, "Un buon affare...", "Ti posso dare " + GetSoldiStr(m_offertaTel) + " per il tuo telefonino... Vuoi vendermelo ?", "" };
    PushMessaggio(msg);
}

void TabbyGame::AzioneAttivaSim(int abbonIndex)
{
    if (!m_tabbyGuy.SpendiSoldi(m_abbonamenti[abbonIndex].GetCostoAttivazione()))
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Non hai abbastanza soldi...", "Forse non ti sei accorto di non avere abbastanza soldi, stronzetto...", "" };
        PushMessaggio(msg);
        return;
    }

    m_tabbyGuy.GetTelefono().GetAbbonamento() = m_abbonamenti[abbonIndex];
    // TODO: SUONO
    /*
    if(ha telefono)
        suono
    */
}

void TabbyGame::AzioneRicarica(long long taglio, std::string nomeOp)
{
    if (m_tabbyGuy.GetTelefono().GetAbbonamento().GetNome() != nomeOp)
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Svegliati...", "Oh, che te ne fai di una ricarica se non hai la sim ???", "" };
        PushMessaggio(msg);
        return;
    }

    if (!m_tabbyGuy.SpendiSoldi(taglio))
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Non hai abbastanza soldi...", "Forse non ti sei accorto di non avere abbastanza soldi, stronzetto...", "" };
        PushMessaggio(msg);
        return;
    }

    m_tabbyGuy.GetTelefono().IncCredito(taglio);

    // TODO: SUONO
    /*
    if(ha telefono)
        suono
    */
}

void TabbyGame::AzioneRiparaScooter()
{
    if (!m_tabbyGuy.HaScooter())
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Hai le allucinazioni ???", "Mi spieghi come fai a farti riparare lo scooter se manco ce l'hai ???", "" };
        PushMessaggio(msg);
        return;
    }
    else if (m_tabbyGuy.GetScooter().GetStato() == 100)
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Ripara lo scooter", "Che motivi hai per voler riparare il tuo scooter visto ch è al 100% di efficienza ???", "" };
        PushMessaggio(msg);
        return;
    }

    m_costoRiparazione = m_tabbyGuy.GetScooter().GetPrezzo() * 0.01 * (100 - m_tabbyGuy.GetScooter().GetStato()) + 10;

    Messaggio msg{ TipoMsg::SCELTA, MsgAzione::RIPARA_SCOOTER, "Ripara scooter", "Vuoi riparare lo scooter per " + GetSoldiStr(m_costoRiparazione) + " ?", "" };
    PushMessaggio(msg);
}

void TabbyGame::AzioneUsaScooter()
{
    if (!m_tabbyGuy.HaScooter())
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Ma che ???", "Mi spieghi come fai a parcheggiare lo scooter se manco ce l'hai ???", "" };
        PushMessaggio(msg);
        return;
    }

    if (m_tabbyGuy.GetScooter().GetAttivita() != Attivita::IN_GIRO)
    {
        if (m_tabbyGuy.GetScooter().GetAttivita() != Attivita::PARCHEGGIATO)
        {
            Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Ma che ???", "Mi spieghi come fai a parcheggiare lo scooter visto che è "+m_tabbyGuy.GetScooter().GetAttivitaStr(true), ""};
            PushMessaggio(msg);
        }
        else
            m_tabbyGuy.GetScooter().SetAttivita(Attivita::PARCHEGGIATO);
    }
    else
        m_tabbyGuy.GetScooter().SetAttivita(Attivita::IN_GIRO);
}

void TabbyGame::AzioneFaiBenza()
{
    if (!m_tabbyGuy.HaScooter())
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Ma che ???", "Mi spieghi come fai a far benzina allo scooter se manco ce l'hai ???", "" };
        PushMessaggio(msg);
        return;
    }

    long long costoBenza = 10;

    if (!m_tabbyGuy.SpendiSoldi(costoBenza))
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Fai benzina", "Al distributore automatico puoi fare un minimo di "+GetSoldiStr(costoBenza)+" di benzina...", "" };
        PushMessaggio(msg);
        return;
    }

    // TODO: IMPLEMENTA STA ROBA, GUARDA CODICE SORGENTE
    // 85 litri, per la macchinina un po' figa...
    /*
    if(m_tabbyGuy.GetScooter().GetCilindrata() = 5)
        setbenza 85 litri
    */
    m_tabbyGuy.GetScooter().SetBenza(5);
    // TODO: CALCOLA VELOCITA
}

bool TabbyGame::TriggerMeccanico()
{
    if (!m_tabbyGuy.HaScooter())
    {
        Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Ma che ???", "Scusa, ma quale scooter avresti intenzione di truccare visto che non ne hai neanche uno ???", "" };
        PushMessaggio(msg);
        return false;
    }

    return true;
}
