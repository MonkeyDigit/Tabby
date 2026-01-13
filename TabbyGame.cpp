#define _CRT_SECURE_NO_WARNINGS // Senza questa mi fotte la funzione localtime
#include <fstream>              // Gestione file
#include <ctime>                // Per l'orario nel log
#include "TabbyGame.h"

Messaggio::Messaggio() {}

Messaggio::Messaggio(TipoMsg tipo, std::string titolo, std::string testo, Scelta id)
    : m_tipo{ tipo }, m_msgAzione{ id }, m_titolo{ titolo }, m_testo{ testo }
{}

TabbyGame::TabbyGame()	// Lunedì 16 settembre 1991
	: m_tabbyGuy{}, m_date{1991, 9, 16}, 
    m_valutaCorrente{Valuta::LIRE}, 
    m_coolDownPestaggio{ 5 }, 
    m_tipoGiorno{ TipoGiorno::NORMALE },
    m_attesa{ATTESA_MAX},
    m_soundActive{ true }
{

    CaricaStringhe();
    CaricaAbbonamenti();
    CaricaDiscoteche();
    CaricaDitte();
    CaricaFeste();
    CaricaNegozi();
    CaricaQuiz();

    // Inizializzo il generatore randomico UNA VOLTA SOLA qui nel costruttore
    // 'rd' è un dispositivo hardware che restituisce un numero casuale vero per il seme
     std::random_device rd;
    // Inizializzo il motore (m_rng) con quel seme
    m_rng.seed(rd());
    std::string indirizzo = m_vieStr[GenRandomInt(0, m_vieStr.size() - 1)] + " n. " + std::to_string(GenRandomInt(1, 150));
    CartaIdentita id{ "Tizio", "Caio", Chrono::Date(1973, 8, 10), "Atto n. 6333 P. 1 S. A.", "Cusano Milanino", "MI", "Italiana", "Milano", indirizzo, "Celibe", "Sfruttato"};
    m_tabbyGuy.SetIdentita(id);
    m_tabbyGuy.SetSoldi(100000);
    m_tabbyGuy.SetPaghetta(20);
    m_tabbyGuy.SetFama(0);
    m_tabbyGuy.SetRep(0);
    m_tabbyGuy.SetTelefono(Telefono{});
    m_tabbyGuy.SetTipa(Tipa{});
    m_tabbyGuy.SetScooter(Scooter{});
    m_tabbyGuy.SetGiubbotto(Vestito(TipoVestito::GIUBBOTTO, "Giubbotto Fatiscenza Nero", "Giubbotto iniziale", "fatiscenza_black.png", 0, 0));
    m_tabbyGuy.SetPantaloni(Vestito(TipoVestito::PANTALONI,"Pantaloni a scacchiera bianchi", "Pantaloni iniziali", "scacchiera_white.png", 0, 0));
    m_tabbyGuy.SetScarpe(Vestito(TipoVestito::SCARPE, "Scarpe sneakers", "Scarpe iniziali", "sneakers.png", 0, 0));
    // TODO: SETTA ALTRE ROBE

    WriteLog(" =======|| AVVIO TABBY - LOG SESSIONE ||======= ");
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
        Messaggio msg{ TipoMsg::INFO, "Anno Bisesto", "Anno bisesto, anno funesto..." };
        PushMessaggio(msg);
    }

    // Cambio di valuta: dopo il 2002 scatta l'euro
    if (m_valutaCorrente == Valuta::LIRE && m_date.GetYear() >= 2002)
    {
        m_valutaCorrente = Valuta::EURO;
        PlaySound(2);
        Messaggio msg{ TipoMsg::SUCCESSO, "L'italia è il paese che amo <3", "Oggi entra in vigore l'Euro €.\n Grazie di cuore, Silvio !!!" };
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
            long long stipendietto{};  // Stipendio calcolato secondo i giorni effettivi di lavoro

            if (m_tabbyGuy.GetCarriera().GetGiorniLavorati() > 29)
                stipendietto = m_tabbyGuy.GetCarriera().GetStipendio();
            else
                stipendietto = m_tabbyGuy.GetCarriera().GetStipendio() * (long)m_tabbyGuy.GetCarriera().GetGiorniLavorati() / 30;

            m_tabbyGuy.GetCarriera().ResetGiorni();

            // EVENTO STIPENDIO
            std::string testo =  "Visto che sei stato un bravo dipendente sottomesso, ora ti arriva il tuo misero stipendio di " + GetSoldiStr(stipendietto);
            Messaggio msg{ TipoMsg::SUCCESSO, "Stipendio !", testo};
            PushMessaggio(msg);

            m_tabbyGuy.GuadagnaSoldi(stipendietto);

            WriteLog("Calendario: Stipendio (" + GetSoldiStr(stipendietto) + ")");
        }

    }

    // ---------------> P A L E S T R A <---------------
    if (m_date == m_scadenzaPal)
    {
        PlaySound(4);
        Messaggio msg{ TipoMsg::INFO, "Pagah", "E' appena scaduto il tuo abbonamento della palestra..." };
        PushMessaggio(msg);
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
            Messaggio msg{ TipoMsg::INFO, "Ultimo giorno di scuola", "Da domani iniziano le vacanze estive !" };
            PushMessaggio(msg);
        }
        // GIORNO DELLA PAGELLA
        if (m_date.GetDay() > 15)
        {
            if (m_date.GetDay() == 22)
                PushMessaggio(Messaggio(TipoMsg::PAGELLA));

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
            Messaggio msg{ TipoMsg::INFO, "Primo giorno di scuola", "Questa mattina devi tornare a scuola..." };
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
                if (m_tabbyGuy.GetPantaloni() == 19 && m_tabbyGuy.GetGiubbotto() == 19)
                {
                    Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Natale...", "Con il tuo vestito da Babbo Natale riesci a stupire tutti..." };
                    PushMessaggio(msg);
                    m_tabbyGuy.IncFama(20);
                }
            }
            else if (m_date.GetDay() == 28 && m_tabbyGuy.GetPantaloni() == 19 && m_tabbyGuy.GetGiubbotto() == 19)
            {
                Messaggio msg{ TipoMsg::INFO, MsgAzione::NONE, "Natale...", "Natale è già passato... Togliti quel dannato vestito..." };
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
            Messaggio msg{ TipoMsg::INFO, festa.m_nome, festa.m_messaggio };
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
            Messaggio msg{ TipoMsg::ERRORE, "Sei senza sigarette !", "Apri il tuo pacchetto di sigarette e lo trovi disperatamente vuoto..." };
            PushMessaggio(msg);
            if (m_tabbyGuy.GetRep() > 10)
                m_tabbyGuy.DecRep(3);
        }
        else if (m_tabbyGuy.GetSizze() < 3)
        {
            Messaggio msg{ TipoMsg::AVVISO, "Sigarette...", "Ti accorgi che stai per finire le tue sizze." };
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
            Messaggio msg{ TipoMsg::ERRORE, "Telefonino", "Cerchi di telefonare e ti accorgi di aver finito i soldi a tua disposizione..." };
            PushMessaggio(msg);
        }
        else if (m_tabbyGuy.GetTelefono().GetCredito() < 3)
        {
            Messaggio msg{ TipoMsg::AVVISO, "Telefonino", "Ti accorgi che stai per finire la ricarica del tuo telefonino." };
            PushMessaggio(msg);
        }
    }

    // Telefono
    if (m_tabbyGuy.GetTelefono().GetStato() == 1)
    {
        // Cellulare morente
        m_tabbyGuy.GetTelefono().Azzera();

        Messaggio msg{ TipoMsg::ERRORE, "Telefonino", "Dopo una vita di duro lavoro, a furia di prendere botte, il tuo cellulare si spacca..." };
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
                PlaySound(603);

                m_tabbyGuy.LasciaTipa();
                m_tabbyGuy.DecRep(11 - rnd);    // Quelle con numero più basso, sono peggiori...

                rnd = GenRandomInt(0, m_frasiSeparazione.size() - 1);
                Messaggio msg{ TipoMsg::ERRORE, "La tipa ti molla...", m_frasiSeparazione[rnd]};
                PushMessaggio(msg);
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

            PlaySound(504);
            Messaggio msg{ TipoMsg::ERRORE, "Perdi il lavoro...", "Un bel giorno ti svegli e scopri di essere stato licenziato." };
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

                Messaggio msg{ TipoMsg::SUCCESSO, "Paghetta settimanale", "Visto che vai bene a scuola, ti diamo il doppio della paghetta..." };
                PushMessaggio(msg);
            }
        }
        else
        {
            m_tabbyGuy.GuadagnaSoldi(m_tabbyGuy.GetPaghetta() * 0.5f);
            // DEBUG LOG
            WriteLog("GestioneEconomia: Metà paghetta (" + GetSoldiStr(m_tabbyGuy.GetPaghetta() * 0.5f) + ")");

            PlaySound(1200);
            Messaggio msg{ TipoMsg::INFO, "Paghetta settimanale", "Finché non andrai bene a scuola, ti daremo solo metà della paghetta..." };
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
            m_tabbyGuy.DecRep(caso);
            Messaggio msg{};
            int totFrasi = m_frasiMetallari.size() + m_frasiManovali.size();    // Ogni frase, che sia del metallaro o del manovale, ha la stessa probabilità di uscire
            int rndFrase = GenRandomInt(0, totFrasi - 1);
            int rndVia = GenRandomInt(0, m_vieStr.size() - 1);

            PlaySound(1300);
            if (rndFrase >= m_frasiMetallari.size())
            {
                rndFrase %= m_frasiMetallari.size();
                msg = Messaggio{ TipoMsg::MANOVALE, "Fai incazzare un manovale", sostituisci(m_frasiManovali[rndFrase],"{LUOGO}", m_vieStr[rndVia]) };
                // DEBUG LOG
                WriteLog("GestioneEventiCasuali: Manovale n. " + std::to_string(rndFrase));
            }
            else
            {
                msg = Messaggio{ TipoMsg::METALLARO, "Vieni pestato", sostituisci(m_frasiMetallari[rndFrase],"{LUOGO}", m_vieStr[rndVia]) };
                // DEBUG LOG
                WriteLog("GestioneEventiCasuali: Metallaro n. " + std::to_string(rndFrase));
            }

            PushMessaggio(msg);
            m_coolDownPestaggio = 5;
        }
        else if (11 <= caso && caso <= 20)   // SCOOTER
        {
            if (m_tabbyGuy.HaScooter() && m_tabbyGuy.GetScooter().GetAttivita() == Attivita::IN_GIRO)
            {
                if (m_tabbyGuy.HaTelefono())
                    m_tabbyGuy.GetTelefono().DecStato(GenRandomInt(1, 8));  // A furia di prendere botte, il cellulare si spacca...

                if (caso < 17)
                {
                    m_tabbyGuy.GetScooter().DecStato(35);
                    int rndFrase = GenRandomInt(0, m_frasiCamionista.size() - 1);
                    int rndVia = GenRandomInt(0, m_vieStr.size() - 1);

                    Messaggio msg{ TipoMsg::INCIDENTE_CAMION, "Camionista bastardo", sostituisci(m_frasiCamionista[rndFrase],"{LUOGO}", m_vieStr[rndVia]) };
                    PushMessaggio(msg);
                    // DEBUG LOG
                    WriteLog("GestioneEventiCasuali: Scooter - Camionista...");
                }
                else
                {
                    m_tabbyGuy.GetScooter().DecStato(20);
                    int rnd = GenRandomInt(0, m_frasiMuro.size() - 1);
                    Messaggio msg{ TipoMsg::INCIDENTE_MURO, "Muro bastardo", m_frasiMuro[rnd]};
                    PushMessaggio(msg);
                    // DEBUG LOG
                    WriteLog("GestioneEventiCasuali: Scooter - Muro...");
                }

                m_tabbyGuy.DecRep(2);

                if (m_tabbyGuy.GetScooter().GetStato() <= 0)
                {
                    m_tabbyGuy.SetScooter(Scooter{});
                    Messaggio msg{ TipoMsg::ERRORE, "Scooter Distrutto", "Quando ti rialzi ti accorgi che il tuo scooter è ormai ridotto a un ammasso di rottami..." };
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
            Messaggio msg{ TipoMsg::INFO, "Sei fortunato...", m_frasiFortuna[rndFrase]};
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
                Messaggio msg{ TipoMsg::INFO, "Scuola", sostituisci(m_frasiScuola[rndFrase],"{MATERIA}",mat.GetNome()) };
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
                m_tipaNuova = GeneraTipa();
                PlaySound(1);
                Messaggio msg{ TipoMsg::SCELTA, "Qualcuno ti caga...", "Una tipa, di nome " + m_tipaNuova.GetNome() + " (Figosità " + std::to_string(m_tipaNuova.GetFama()) + "/100), ci prova con te...\nCi stai ???", Scelta::TIPA_CI_PROVA };
                PushMessaggio(msg);

                WriteLog("GestioneEventiCasuali: Una tipa ci prova...");
            }
        }
        else if (caso == 43)    // Domande inutili
        {
            if (m_tabbyGuy.HaTipa())
            {
                Messaggio msg{ TipoMsg::SCELTA, "Domande inutili della tipa...", "Mi ami ???", Scelta::TIPA_MI_AMI };
                PushMessaggio(msg);

            }

            WriteLog("GestioneEventiCasuali: Domande inutili della tipa...");
        }
        else if (caso == 44)
        {
            if (m_tabbyGuy.HaTipa())
            {
                Messaggio msg{ TipoMsg::SCELTA, "Domande inutili della tipa...", "Ma sono ingrassata ???", Scelta::TIPA_INGRASSATA };
                PushMessaggio(msg);
            }
            WriteLog("GestioneEventiCasuali: Domande inutili della tipa...");
        }
        else if (45 <= caso && caso <= 48)
        {
            // Qua non c'è nulla
        }
        else if (caso == 49 || caso == 50)  // Vari ed eventuali
        {
            if (m_tabbyGuy.HaTelefono())
            {
                m_tabbyGuy.GetTelefono().DecStato(GenRandomInt(0, 8));

                Messaggio msg{ TipoMsg::INFO, "Telefonino", "Il telefonino ti cade di tasca e vola per terra..." };
                PushMessaggio(msg);
                // DEBUG LOG
                WriteLog("GestioneEventiCasuali: Telefonino - Cade...");

            }
        }
    }
}

void TabbyGame::CheckScooter()
{
    if (!m_tabbyGuy.HaScooter())
        return;

    const Scooter& s = m_tabbyGuy.GetScooter();

    switch (s.GetAttivita())
    {
    case Attivita::A_SECCO:
    case Attivita::INVASATO:
    case Attivita::INGRIPPATO:
    case Attivita::SEQUESTRATO:
        PushMessaggio(Messaggio{ TipoMsg::AVVISO, "Manutenzione Scooter", "Attenzione, il tuo scooter è " + s.GetAttivitaStr(true + " !") });
        break;
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

void TabbyGame::ApplicaScelta(Scelta msgAzione, bool sceltaYes)
{
    switch (msgAzione)
    {
    case Scelta::CORROMPI:
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
                Messaggio msg{ TipoMsg::INFO, "Errore critico", "Cosa ??? Prima cerchi di corrompermi, poi si scopre che non hai abbastanza soldi !!!" };
                PushMessaggio(msg);
            }
        }
        NuovoGiorno();
        break;

    case Scelta::GARA:
        if (sceltaYes)
        {
            if ((m_scooterRnd.GetVelocita() + GenRandomInt(80, 119)) > (m_tabbyGuy.GetScooter().GetVelocita() + m_tabbyGuy.GetScooter().GetStato() + m_tabbyGuy.GetFortuna()))
            {   // Perdi
                if (m_tabbyGuy.GetRep() > 80)
                    m_tabbyGuy.DecRep(3);
                if (m_tabbyGuy.GetRep() > 10)
                    m_tabbyGuy.DecRep(2);
                Messaggio msg{ TipoMsg::ERRORE, "Hai perso, imbranato...", "Dopo pochi metri si vede l'inferiorità del tuo scooter..." };
                PushMessaggio(msg);
            }
            else
            {   // Vinci
                m_tabbyGuy.IncRep(10);
                Messaggio msg{ TipoMsg::SUCCESSO, "Hai vinto", "Con il tuo scooter, bruci l'avversario in partenza..." };
                PushMessaggio(msg);
            }
        }
        else
        {
            if (m_tabbyGuy.GetRep() > 80)    // Se non accetti la sfida, perdi reputazione...
                m_tabbyGuy.DecRep(3);
            if (m_tabbyGuy.GetRep() > 10)
                m_tabbyGuy.DecRep(2);
        }
        m_tabbyGuy.GetScooter().DecBenza(0.5);
        CheckScooter();
        NuovoGiorno();
        break;

    case Scelta::LICENZIATI:

        if (sceltaYes)
        {
            m_tabbyGuy.Licenziati();
            NuovoGiorno();
        }
        break;

    case Scelta::LASCIA_TIPA:
        if (sceltaYes)
        {
            PlaySound(603);
            if (m_tabbyGuy.GetTipa().GetFama() >= 79)
            {
                Messaggio msg{ TipoMsg::ERRORE, "Coglione...", "Appena vengono a sapere quello che hai fatto, i tuoi amici ti prendono a scarpate.\nQualcuno più furbo di te, va a consolarla..." };
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

    case Scelta::TIPA_CI_PROVA:
        if (sceltaYes)
        {
            // Controlla che tu non abbia già una tipa...
            if (m_tabbyGuy.HaTipa())
                PushMessaggio(Messaggio(TipoMsg::DUE_DONNE));
            else   // Bravo, non hai una tipa...
            {
                m_tabbyGuy.SetTipa(m_tipaNuova);
                m_tabbyGuy.SetRapporti(GenRandomInt(45, 60));
                m_tabbyGuy.IncFama(m_tipaNuova.GetFama() * 0.1f);
                m_tabbyGuy.IncRep(m_tipaNuova.GetFama() * 0.08f);
            }
        }
        else if(m_tipaNuova.GetFama() >= 79 && !m_tabbyGuy.HaTipa())
        {
            Messaggio msg{ TipoMsg::ERRORE, "Idiota...", "Appena vengono a sapere che non ti vuoi mettere insieme ad una figona come quella, i tuoi amici ti prendono a scarpate." };
            PushMessaggio(msg);

            m_tabbyGuy.DecRep(4);
        }

        break;

    case Scelta::TIPA_MI_AMI:
        if (sceltaYes == false) // Rispondo no
        {
            Messaggio msg{ TipoMsg::ERRORE, "Risposta sbagliata...", "Sei sempre il solito stronzo... Non capisco perché resto ancora con uno così..." };
            PushMessaggio(msg);

            if(m_tabbyGuy.GetRapporti() > 5)
                m_tabbyGuy.DecRapporti(45);
        }
        break;

    case Scelta::TIPA_INGRASSATA:
        if (sceltaYes == false) // Rispondo no
        {
            Messaggio msg{ TipoMsg::ERRORE, "Risposta sbagliata...", "Sei un bastardo, non capisci mai i miei problemi..." };
            PushMessaggio(msg);

            if(m_tabbyGuy.GetRapporti() > 5)
                m_tabbyGuy.DecRapporti(20);
        }
        break;

    case Scelta::VENDI_TEL:
        if (sceltaYes)
        {
            // Trasferiamo sim e credito
            const Abbonamento abb = m_tabbyGuy.GetTelefono().GetAbbonamento();
            long long cred = m_tabbyGuy.GetTelefono().GetCredito();

            m_tabbyGuy.GetTelefono().Azzera();

            m_tabbyGuy.GetTelefono().IncCredito(cred);
            m_tabbyGuy.GetTelefono().SetAbbonamento(abb);
            m_tabbyGuy.GuadagnaSoldi(m_offertaTel);
        }
        else
        {
            Messaggio msg{ TipoMsg::INFO, "Telefonino", "Allora vai a farti fottere, pirletta !" };
            PushMessaggio(msg);
        }
        break;

    case Scelta::RIPARA_SCOOTER:
        if (sceltaYes)
        {
            if (m_tabbyGuy.SpendiSoldi(m_costoRiparazione))
            {
                PlaySound(102);
                m_tabbyGuy.GetScooter().Ripara();
                WriteLog("AzioneRiparaScooter: Paga riparazione " + GetSoldiStr(m_costoRiparazione));
            }
            else
            {
                Messaggio msg{ TipoMsg::ERRORE, "Non hai abbastanza soldi...", "L'enorme meccanico ti afferra con una sola mano, ti riempie di pugni, e non esita a scaraventare te e il tuo motorino merdoso fuori dall'officina." };
                PushMessaggio(msg);
            }
        }
        break;
    case Scelta::VENDI_SCOOTER:
        if (sceltaYes)
        {
            m_tabbyGuy.SetScooter(Scooter{});
            m_tabbyGuy.GuadagnaSoldi(m_offertaScooter);
            WriteLog("ApplicaScelta: Vendi lo scooter per " + GetSoldiStr(m_offertaScooter));
        }
        break;
    }
    
}

bool TabbyGame::TriggerScuola()
{
    if (m_tipoGiorno == TipoGiorno::NORMALE)
        return true;

    Messaggio msg{ TipoMsg::AVVISO, "Scuola", "Non puoi andare a scuola in un giorno di vacanza!" };
    PushMessaggio(msg);

    return false;
}

bool TabbyGame::TriggerLavoro()
{
    if (m_tipoGiorno != TipoGiorno::FESTIVO)
        return true;

    Messaggio msg{ TipoMsg::INFO, "Lavoro", "Arrivato davanti ai cancelli della ditta, li trovi irrimediabilmente chiusi..." };
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

    if (m_tabbyGuy.GetRep() >= 30 || GenRandomInt(1, 10) == 1)
        mat.IncVoto(2);
    else
    {
        m_tabbyGuy.DecRep(2);
        mat.DecVoto(1);

        PlaySound(402);
        Messaggio msg{ TipoMsg::ERRORE, "Bella figura", "Cosa ??? Credi di farmi paura piccolo pezzettino di letame vestito da zarro...\nDeve ancora nascere chi può minacciarmi..." };
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
    Messaggio msg{ TipoMsg::SCELTA, "Corrompi i professori", "Ma... forse per " + GetSoldiStr(m_costoCorruzione) + " potrei dimenticare i tuoi ultimi compiti in classe...", Scelta::CORROMPI };
    PushMessaggio(msg);
}

void TabbyGame::AzioneGara()
{
    if (!m_tabbyGuy.HaScooter())
    {
        Messaggio msg{ TipoMsg::ERRORE, "Ma che sei scemo ???", "Con quale scooter vorresti gareggiare, visto che non lo possiedi ?" };
        PushMessaggio(msg);
    }
    else if (m_tabbyGuy.GetScooter().GetAttivita() != Attivita::IN_GIRO)
    {
        Messaggio msg{ TipoMsg::ERRORE, "Svegliati...", "Purtroppo non puoi gareggiare visto che il tuo scooter è " + m_tabbyGuy.GetScooter().GetAttivitaStr(true) + "." };
        PushMessaggio(msg);
    }
    else
    {
        m_scooterRnd = GeneraScooter();
        PlaySound(701);
        Messaggio msg{ TipoMsg::SCELTA, "Gareggia con lo scooter", "Accetti la sfida con un tabbozzo che ha un " + m_scooterRnd.GetNome() + " ?", Scelta::GARA };
        PushMessaggio(msg);
        if (m_tabbyGuy.GetScooter().GetStato() > 30)
            m_tabbyGuy.GetScooter().DecStato(GenRandomInt(0, 1));
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
        Messaggio msg{ TipoMsg::ERRORE, "Sfigato", "Con la scarsa reputazione che hai, tutti trovano qualcosa di meglio da fare piuttosto che venire." };
        PushMessaggio(msg);
        NuovoGiorno();
    }
    else if (m_coolDownPestaggio > 0)
    {
        if (GenRandomInt(0, 1) == 1)
        {
            if (m_tabbyGuy.GetRep() < 80)
                m_tabbyGuy.IncRep(3);
            Messaggio msg{ TipoMsg::SUCCESSO, "Chiama la Compagnia", "Dopo aver visto i tuoi amici, chi ti ha picchiato non si farà più vedere in giro per un bel pezzo..." };
            PushMessaggio(msg);
        }
        else
        {
            if (m_tabbyGuy.GetRep() < 95)
                m_tabbyGuy.IncRep(5);
            Messaggio msg{ TipoMsg::INFO, "Chiama la Compagnia", "Anche i tuoi amici, al gran completo, vengono sacagnati di botte da chi ti aveva picchiato, accorgendosi così che non sei solo tu ad essere una chiavica, ma lo sono anche loro..." };
            PushMessaggio(msg);
        }
        NuovoGiorno();
    }
    else
    {
        Messaggio msg{ TipoMsg::ERRORE, "Chiama la Compagnia (perché ?)", "Visto che non c'è nessuno da minacciare, tutti se ne vanno avviliti..." };
        PushMessaggio(msg);
    }
}

void TabbyGame::AzioneAumentoPaghetta()
{
    if (m_tabbyGuy.GetStudio() > 40)
    {
        if (((m_tabbyGuy.GetStudio() - m_tabbyGuy.GetPaghetta() + m_tabbyGuy.GetFortuna()) > (100 + GenRandomInt(0, 50)) && (m_tabbyGuy.GetPaghetta() < 50)))
        {
            Messaggio msg{ TipoMsg::SUCCESSO, "Aumento Paghetta !", "Va bene... ti daremo " + GetSoldiStr(5) + " di paghetta in più..." };
            PushMessaggio(msg);
            m_tabbyGuy.IncPaghetta(5);
        }
        else
        {
            Messaggio msg{ TipoMsg::ERRORE, "Errore irrecuperabile", "Vedi di scordartelo... Dovrà passare molto tempo prima che ti venga aumentata la paghetta..." };
            PushMessaggio(msg);
        }

        NuovoGiorno();
    }
    else
    {
        Messaggio msg{ TipoMsg::INFO, "Errore irrecuperabile", "Quando andrai meglio a scuola, forse..." };
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
            Messaggio msg{ TipoMsg::ERRORE, "Non te li diamo, viziato", "Ma insomma ! Non puoi continuamente chiedere soldi ! Aspetta ancora qualche giorno. Fai qualche cosa di economico nel frattempo..." };
            PushMessaggio(msg);
        }

        NuovoGiorno();
    }
    else
    {
        Messaggio msg{ TipoMsg::ERRORE, "Errore irrecuperabile",
            "Quando andrai meglio a scuola potrai tornare a chiederci dei soldi, non ora. "
            "\nMa non lo sai che per la tua vita è importante studiare, e dovresti impegnarti "
            "di più, perché quando ti impegni i risultati si vedono, solo che sei svogliato "
            "e non fai mai nulla, mi ricordo che quando ero giovane io era tutta un altra cosa..."
            "allora sì che i giovani studiavano..." };
        PushMessaggio(msg);
    }
}

void TabbyGame::AzioneChiediSoldi()
{
    PlaySound(801);
    Messaggio msg{ TipoMsg::ERRORE, "Errore irrecuperabile", "Non pensarci neanche lontanamente..." };
    PushMessaggio(msg);
    NuovoGiorno();
}

const Ditta& TabbyGame::ProponiDitta()
{
    int indiceLavoro = GenRandomInt(0, m_ditte.size() - 1);
    return m_ditte[indiceLavoro];
}

void TabbyGame::AzioneRifiutaProposta()
{
    Messaggio msg{ TipoMsg::INFO, "Levati dai coglioni", "Allora vai a farti fottere..." };
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
    m_tipaNuova = tipa;
    if ((m_tipaNuova.GetFama() * 2 + GenRandomInt(0, 49)) <= (m_tabbyGuy.GetFama() + m_tabbyGuy.GetRep() + GenRandomInt(0, 29)))
    {
        // E' andata bene...
        Messaggio msg{ TipoMsg::SUCCESSO, "E' andata bene !", "Con il tuo fascino nascosto da tabbozzo, seduci la tipa e ti ci metti insieme." };
        PushMessaggio(msg);
        // ...Ma comunque controlla che tu non abbia già una tipa
        if (m_tabbyGuy.HaTipa())
            PushMessaggio(Messaggio(TipoMsg::DUE_DONNE));
        else
        {   // Bravo, non hai una tipa
            m_tabbyGuy.SetTipa(m_tipaNuova);
            m_tabbyGuy.SetRapporti(GenRandomInt(30, 45));
            m_tabbyGuy.IncFama(m_tipaNuova.GetFama() * 0.1f);
        }
    }
    else
    {
        // Fai cagare...
        m_paloCounter++;
        m_tabbyGuy.DecRep(2);
        m_tabbyGuy.DecFama(2);

        int rnd = GenRandomInt(0, m_frasiPalo.size() - 1);
        Messaggio msg{ TipoMsg::ERRORE, "Due di picche", m_frasiPalo[rnd] };
        PushMessaggio(msg);

        if (m_paloCounter % 5 == 0)  // Ogni 5 pali, appare un messaggio di consolazione
        {
            Messaggio msgPalo{ TipoMsg::ERRORE, "La vita è bella...", "Fino ad ora hai preso " + std::to_string(m_paloCounter) + " pali ! \nNon ti preoccupare, capita a tutti di prendere qualche due di picche nella vita..." };
            PushMessaggio(msgPalo);
        }
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
        Messaggio msg{ TipoMsg::ERRORE, "Sei un po' stupido...", "Mi spieghi perché dovremmo assumere qualcuno che non è neanche in grado di mettere delle crocette su un foglio ???" };
        PushMessaggio(msg);
    }
    else if (m_tabbyGuy.GetRep() + m_tabbyGuy.GetFortuna() + GenRandomInt(1, 80) > GenRandomInt(1, 200))
    {
        m_tabbyGuy.GetCarriera().SetImpegno(10 + GenRandomInt(0, 20));
        m_tabbyGuy.GetCarriera().SetGiorniLavorati(1);
        m_tabbyGuy.GetCarriera().SetStipendio(520 + GenRandomInt(0, 6) * 60);
        m_tabbyGuy.GetCarriera().SetDitta(nomeDitta);

        Messaggio msg{ TipoMsg::SUCCESSO, "Ora sei sfruttato !", "SEI STATO ASSUNTO ! Ora sei un felice dipendente della " + nomeDitta + " !"};
        PushMessaggio(msg);
    }
    else
    {
        Messaggio msg{ TipoMsg::ERRORE, "Sei inutile", "Mi dispiace ragazzo, ma non sei riuscito a superare il test... Ora puoi anche portare la tua brutta faccia fuori dal mio ufficio, prima che ti faccia buttare fuori a calci... Grazie e arrivederci..." };
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
        Messaggio msg{ TipoMsg::AVVISO, "Non ti permettere", "Forse non ti ricordi che hai già un lavoro..." };
        PushMessaggio(msg);
        return false;
    }

    return true;
}

void TabbyGame::AzioneLicenziati()
{
    if (!m_tabbyGuy.HaUnLavoro())
    {
        Messaggio msg{ TipoMsg::AVVISO, "Disabile", "Forse non ti ricordi che sei disokkupato..." };
        PushMessaggio(msg);
    }
    else
    {
        if (!TriggerLavoro())
            return;
        
        Messaggio msg{ TipoMsg::SCELTA, "Licenziati", "Sei proprio sicuro di voler dare le dimissioni dalla " + m_tabbyGuy.GetCarriera().GetNomeDitta() + " ?", Scelta::LICENZIATI };
        PushMessaggio(msg);
    }
}

long long TabbyGame::ConvertiValuta(long long valoreBase) const
{
	if (m_valutaCorrente == Valuta::EURO)
		return valoreBase;

	return (long long)(valoreBase * CAMBIO_EURO_LIRA);
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
    std::vector<std::string>* vettoreCorrente = nullptr; // Puntatore al vettore che stiamo riempiendo - ATTENZIONE: qua non posso usare la reference, perché in C++ non possono essere riassegnate

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
            else if (tag == "MANOVALI")    vettoreCorrente = &m_frasiManovali;
            else if (tag == "SCUOLA")    vettoreCorrente = &m_frasiScuola;
            else if (tag == "FORTUNA")   vettoreCorrente = &m_frasiFortuna;
            else if (tag == "PALO")    vettoreCorrente = &m_frasiPalo;
            else if (tag == "SEPARAZIONE") vettoreCorrente = &m_frasiSeparazione;
            else if (tag == "PALESTRA")  vettoreCorrente = &m_frasiPalestra;
            else if (tag == "SIGARETTE") vettoreCorrente = &m_frasiSigarette;
            else if (tag == "CAMIONISTA") vettoreCorrente = &m_frasiCamionista;
            else if (tag == "MURO") vettoreCorrente = &m_frasiMuro;
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

        auto tokens = splitString(riga, '|');
        // FORMATO: Nome | Img | CostoFisso | CostoVariabile1 | CostoVariabile2
        if (tokens.size() >= 5)
        {
            std::string nome = tokens[0];
            std::string img = tokens[1];
            long long fisso = parseLong(tokens[2]);
            long long var1 = parseLong(tokens[3]);
            long long var2 = parseLong(tokens[4]);

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

        auto tokens = splitString(riga, '|');
        if (tokens.size() >= 8) {
            Disco d;
            d.m_nome = tokens[0];
            d.m_descrizione = tokens[1];
            d.m_giornoChiuso = (Chrono::WeekDay)parseInt(tokens[2]);
            d.m_fuoriPorta = (bool)parseInt(tokens[3]);
            d.m_prezzoIngresso = parseLong(tokens[4]);
            d.m_reqFama = parseInt(tokens[5]);
            d.m_incFama = parseInt(tokens[6]);
            d.m_incRep = parseInt(tokens[7]);

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

            // Riga 1: Dati Base + IMMAGINE (Token finale)
            if (!std::getline(file, riga)) break;
            auto tokens = splitString(riga, '|');

            if (tokens.size() < 3) continue; // Minimo sindacale

            d.m_nome = tokens[0];
            d.m_sede = tokens[1];
            d.m_fatturato = parseLong(tokens[2]);

            // Se c'è un quarto token, è l'immagine. Altrimenti metti un default.
            if (tokens.size() >= 4) {
                d.m_img = tokens[3];
            }
            else {
                d.m_img = "ditte.png"; // Immagine di default se manca nel file
            }
            // --------------------

            // Riga 2: Offerta
            if (!std::getline(file, riga)) break;
            auto offTokens = splitString(riga, '|');
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

        auto tokens = splitString(riga, '|');
        // FORMATO: Giorno | Mese | Nome | Messaggio
        if (tokens.size() >= 4)
        {
            FestaFissa f;
            f.m_giorno = parseInt(tokens[0]);
            f.m_mese = parseInt(tokens[1]);
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

        auto tokens = splitString(riga, '|');
        if (tokens.empty()) continue;

        // --- NUOVO NEGOZIO ---
        if (tokens[0] == "[NEGOZIO]") {
            Negozio n;
            n.m_nome = tokens[1];
            // Cast int -> Enum Categoria
            n.m_merce = (CategoriaOggetto)parseInt(tokens[2]);

            m_negozi.push_back(n);
            currentNegozio = &m_negozi.back(); // Puntiamo all'ultimo inserito
        }
        else if (tokens[0] == "[TELEFONIA]")
        {
            m_telefonia.m_nome = tokens[1];
            // Cast int -> Enum Categoria
            m_telefonia.m_merce = (CategoriaOggetto)parseInt(tokens[2]);
            currentNegozio = &m_telefonia;
        }
        else if (tokens[0] == "[CONCESSIONARIO]")
        {
            m_concessionario.m_nome = tokens[1];
            m_concessionario.m_merce = (CategoriaOggetto)parseInt(tokens[2]);
            currentNegozio = &m_concessionario;
        }
        else if (tokens[0] == "[MECCANICO]")
        {
            m_meccanico.m_nome = tokens[1];
            m_meccanico.m_merce = (CategoriaOggetto)parseInt(tokens[2]);
            currentNegozio = &m_meccanico;
        }
        // --- ITEM (OGGETTO) ---
        else if (tokens[0] == "ITEM" && currentNegozio != nullptr) {
            std::string tipo = tokens[1];
            Acquistabile* nuovoItem = nullptr;

            if (tipo == "VESTITO" && tokens.size() >= 8) {
                // ITEM|VESTITO|TipoVest|Nome|Desc|Img|Prezzo|Fama
                TipoVestito tv = (TipoVestito)parseInt(tokens[2]);
                nuovoItem = new Vestito(tv, tokens[3], tokens[4], tokens[5], parseLong(tokens[6]), parseInt(tokens[7]));
            }
            else if (tipo == "SIZZE" && tokens.size() >= 7) {
                // ITEM|SIZZE|Nome|Desc|Img|Prezzo|Fama
                nuovoItem = new Sizze(tokens[2], tokens[3], tokens[4], parseLong(tokens[5]), parseInt(tokens[6]));
            }
            else if (tipo == "TEL" && tokens.size() >= 8) {
                nuovoItem = new Telefono{ tokens[2], tokens[3], tokens[4], parseLong(tokens[5]), parseInt(tokens[6]), parseInt(tokens[7]), 0, Abbonamento() };
            }
            else if (tipo == "SCOOTER")
            {
                // FORMATO NUOVO (23 token totali attesi):
                // 0:ITEM | 1:SCOOTER | 2:Nome | 3:Img | 4:Prezzo | 5:Fama | 6:Benza |
                // MARMITTA:    7:Nome | 8:Img | 9:Prezzo | 10:Power
                // CARBURATORE: 11:Nome | 12:Img | 13:Prezzo | 14:Power
                // PISTONE:     15:Nome | 16:Img | 17:Prezzo | 18:Power
                // FILTRO:      19:Nome | 20:Img | 21:Prezzo | 22:Power

                if (tokens.size() >= 23)
                {
                    // 1. Dati Base Scooter
                    std::string nome = tokens[2];
                    std::string img = tokens[3];
                    long long prezzo = parseLong(tokens[4]);
                    int fama = parseInt(tokens[5]);
                    int benza = parseInt(tokens[6]); // Capacità serbatoio o benza iniziale

                    // 2. Costruzione Componenti
                    Pezzo marmitta(TipoPezzo::MARMITTA, tokens[7], tokens[8], parseLong(tokens[9]), parseInt(tokens[10]));
                    Pezzo carburatore(TipoPezzo::CARBURATORE, tokens[11], tokens[12], parseLong(tokens[13]), parseInt(tokens[14]));
                    Pezzo pistone(TipoPezzo::PISTONE, tokens[15], tokens[16], parseLong(tokens[17]), parseInt(tokens[18]));
                    Pezzo filtro(TipoPezzo::FILTRO, tokens[19], tokens[20], parseLong(tokens[21]), parseInt(tokens[22]));

                    // 3. Creazione Scooter con i pezzi
                    nuovoItem = new Scooter(nome, img, prezzo, fama, benza, marmitta, carburatore, pistone, filtro);
                }
            }
            else if (tipo == "PEZZO" && tokens.size() >= 7)
            {
                TipoPezzo tp = (TipoPezzo)parseInt(tokens[2]);
                nuovoItem = new Pezzo{ tp, tokens[3], tokens[4], parseLong(tokens[5]), parseInt(tokens[6]) };
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
                    auto risposte = splitString(rigaRisposte, '|');

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

void TabbyGame::PlaySound(int id)
{
    if (!m_soundActive)
        return;

    Messaggio msg{ TipoMsg::SUONO };
    msg.m_soundId = id;
    PushMessaggio(msg);
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
        Messaggio msg{ TipoMsg::AVVISO, "Disabile", "Forse non ti ricordi che sei disokkupato..." };
        PushMessaggio(msg);
    }
    else
    {
        if (!TriggerLavoro())
            return;

        if (m_tabbyGuy.GetCarriera().GetImpegno() < 85)
            m_tabbyGuy.GetCarriera().IncImpegno(1);

        PlaySound(501);
        NuovoGiorno();
    }
}

void TabbyGame::AzioneLeccaculo()
{
    if (!m_tabbyGuy.HaUnLavoro())
    {
        Messaggio msg{ TipoMsg::AVVISO, "Disabile", "Forse non ti ricordi che sei disokkupato..." };
        PushMessaggio(msg);
    }
    else
    {
        if (!TriggerLavoro())
            return;
        
        PlaySound(503);

        if (m_tabbyGuy.GetRep() > 20)    // Facendo il leccaculo perdi reputazione e fama...
            m_tabbyGuy.DecRep(1);

        if (m_tabbyGuy.GetCarriera().GetImpegno() < 99) // perché non ti impegnerai mai abbastanza...
            m_tabbyGuy.GetCarriera().IncImpegno(1);

        if (GenRandomInt(1, m_tabbyGuy.GetFortuna() + 3) == 1)
            NuovoGiorno();
    }
}

void TabbyGame::AzioneAumentoSalario()
{
    if (!m_tabbyGuy.HaUnLavoro())
    {
        Messaggio msg{ TipoMsg::AVVISO, "Disabile", "Forse non ti ricordi che sei disokkupato..." };
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
                    Messaggio msg{ TipoMsg::SUCCESSO, "Aumento salario", "Forse per questa volta potremmo darti qualcosina in più..." };
                    PushMessaggio(msg);
                    m_tabbyGuy.GetCarriera().IncStipendio(GenRandomInt(1, 2) * 50);
                    m_tabbyGuy.GetCarriera().DecImpegno(30);
                }
                else
                {
                    Messaggio msg{ TipoMsg::ERRORE, "Come osi...", "Vedi di scordartelo, bastardo..." };
                    PushMessaggio(msg);
                    m_tabbyGuy.GetCarriera().DecImpegno(20);
                }

                NuovoGiorno();
            }
            else
            {
                Messaggio msg{ TipoMsg::ERRORE, "Sei impazzito ???", "Che cosa vorresti ??? SCORDATELO !!!" };
                PushMessaggio(msg);
            }
    }
}

void TabbyGame::AzioneSciopera()
{
    if (!m_tabbyGuy.HaUnLavoro())
    {
        Messaggio msg{ TipoMsg::AVVISO, "Disabile", "Forse non ti ricordi che sei disokkupato..." };
        PushMessaggio(msg);
    }
    else
    {
        if (!TriggerLavoro())
            return;

        PlaySound(502);

        if (m_tabbyGuy.GetRep() < 85)
            m_tabbyGuy.IncRep(10);

        if (m_tabbyGuy.GetCarriera().GetImpegno() > 19)
            m_tabbyGuy.GetCarriera().DecImpegno(15);

        NuovoGiorno();
    }
}

void TabbyGame::AzionePagaDisco(int discoIndex)
{
    const Disco& disco = m_discoteche[discoIndex];
    if (m_date.GetWeekDay() == disco.m_giornoChiuso)
    {
        Messaggio msg{ TipoMsg::INFO, "Giorno di chiusura", "Un cartello recita che oggi è il giorno di chiusura settimanale..." };
        PushMessaggio(msg);
        return;
    }

    if (disco.m_fuoriPorta && m_tabbyGuy.GetScooter().GetAttivita() != Attivita::IN_GIRO)
    {
        Messaggio msg{ TipoMsg::AVVISO, "Discoteca fuori porta", "Senza lo scooter non puoi andare nelle discoteche fuori porta..." };
        PushMessaggio(msg);
        return;
    }

    if (m_tabbyGuy.GetFama() < disco.m_reqFama)
    {
        Messaggio msg{ TipoMsg::ERRORE, "Selezione all'ingresso", "Mi dispiace signore, conciato così, qui non può entrare...\nVenga vestito meglio la prossima volta, signore." };
        PushMessaggio(msg);
        return;
    }

    if (!m_tabbyGuy.SpendiSoldi(disco.m_prezzoIngresso))
    {
        Messaggio msg{ TipoMsg::ERRORE, "Bella figura di merda...", "Appena entraro ti accorgi di non avere abbastanza soldi per pagare il biglietto.\nUn energumeno buttafuori ti deposita gentilmente in un cassonetto della spazzatura poco distante dalla discoteca." };
        PushMessaggio(msg);
    }
    else
    {
        // FINALMENTE VAI IN DISCO
        PlaySound(GenRandomInt(303, 305));
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
        Messaggio msg{ TipoMsg::ERRORE, "Hai le allucinazioni ???", "Scusa, che ragazza avresti intenzione di lasciare ???" };
        PushMessaggio(msg);
        return;
    }

    Messaggio msg{ TipoMsg::SCELTA, "Lascia tipa", "Sei proprio sicuro di voler lasciare "+m_tabbyGuy.GetTipa().GetNome()+" ?", Scelta::LASCIA_TIPA };
    PushMessaggio(msg);
}

void TabbyGame::AzioneEsciTipa()
{
    if (!m_tabbyGuy.HaTipa())
    {
        Messaggio msg{ TipoMsg::ERRORE, "Non sei molto intelligente...", "Scusa, con che tipa vorresti uscire ???" };
        PushMessaggio(msg);
        return;
    }

    if (!m_tabbyGuy.HaScooter())
    {
        Messaggio msg{ TipoMsg::INFO, "Compra lo scooter", "Finché non comprerai lo scooter, non usciremo più insieme..." };
        PushMessaggio(msg);
        return;
    }

    if (m_tabbyGuy.GetScooter().GetAttivita() != Attivita::IN_GIRO)
    {
        Messaggio msg{ TipoMsg::INFO, "Risistema lo scooter", "Finché il tuo scooter resterà "+m_tabbyGuy.GetScooter().GetAttivitaStr(true) + " non potremo uscire insieme..."};
        PushMessaggio(msg);
        return;
    }

    if (m_tabbyGuy.GetScooter().GetStato() <= 35)
    {
        Messaggio msg{ TipoMsg::INFO, "Ripara lo scooter", "Finché non riparerai lo scooter, non usciremo più insieme..." };
        PushMessaggio(msg);
        return;
    }

    // FINALMENTE ESCI CON LA TIPA
    if (!m_tabbyGuy.SpendiSoldi(10))
    {
        Messaggio msg{ TipoMsg::INFO, "Sei povero", "Se mi vuoi portare fuori, cerca di avere almeno un po' di soldi..." };
        PushMessaggio(msg);
        return;
    }

    WriteLog("AzioneEsciTipa: Esci con la tipa " + GetSoldiStr(10));

    m_tabbyGuy.IncRapporti(5);
    if (m_tabbyGuy.GetTipa().GetFama() > m_tabbyGuy.GetFama())
        m_tabbyGuy.IncFama(1);

    // TODO: NUOVO GIORNO?
    m_tabbyGuy.GetScooter().DecBenza(0.3f);
    CheckScooter();
}

void TabbyGame::AzioneTelefonaTipa()
{
    if (!m_tabbyGuy.HaTipa())
    {
        Messaggio msg{ TipoMsg::ERRORE, "Non sei molto intelligente...", "Scusa, che ragazza vorresti chiamare ???" };
        PushMessaggio(msg);
        return;
    }

    if (!m_tabbyGuy.HaTelefono() && m_tabbyGuy.GetSoldi() <= 5 && m_tabbyGuy.GetTelefono().GetCredito() < 2)
    {
        Messaggio msg{ TipoMsg::ERRORE, "Non toccare quel telefono...", "\"Se ti azzardi a fare anche una singola telefonata, ti spezzo le gambe\", disse tuo padre con un'accetta in mano..." };
        PushMessaggio(msg);
        return;
    }

    PlaySound(602);
    if (m_tabbyGuy.HaTelefono() && m_tabbyGuy.GetTelefono().GetCredito() <= 2)
        m_tabbyGuy.GetTelefono().DecCredito(2);
    else
        m_tabbyGuy.SpendiSoldi(5);

    WriteLog("AzioneTelefonaTipa: Telefona alla tipa " + GetSoldiStr(5));

    if (m_tabbyGuy.GetRapporti() <= 60)
        m_tabbyGuy.IncRapporti(1);
}

void TabbyGame::AzioneVoglioEntrambe()
{
    PlaySound(3);
    Messaggio msg{ TipoMsg::ERRORE, "La vita è bella", "Mentre sei appartato con la " + m_tipaNuova.GetNome() + ", arriva la tua ragazza, " + m_tabbyGuy.GetTipa().GetNome() + ", ti tira uno schiaffo e ti lascia. \nCapendo finalmente di che pasta sei fatto, anche la " + m_tipaNuova.GetNome() + " si allontana..." };
    PushMessaggio(msg);

    m_tabbyGuy.LasciaTipa();

    m_tabbyGuy.DecRep(8);
    m_tabbyGuy.DecFama(4);
}

void TabbyGame::AzionePreferiscoNuova()
{
    m_tabbyGuy.SetTipa(m_tipaNuova);
    m_tabbyGuy.SetRapporti(GenRandomInt(30, 45));
    m_tabbyGuy.IncFama(m_tipaNuova.GetFama() * 0.1f);
    m_tabbyGuy.IncRep(m_tipaNuova.GetFama() * 0.08f);
}

bool TabbyGame::TriggerNegozio(CategoriaOggetto merce)
{
    if (m_tipoGiorno != TipoGiorno::FESTIVO)
        return true;

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

    Messaggio msg{ TipoMsg::ERRORE, "Torna a casa", str };
    PushMessaggio(msg);
    return false;
}

void TabbyGame::AzioneCompra(const Acquistabile& prod)
{
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
            PlaySound(3);
            str = "Ti piacerebbe comprare lo scooter, vero ?\nPurtroppo, non hai abbastanza soldi...";
            if (m_tabbyGuy.GetRep() > 3) m_tabbyGuy.DecRep(1);
            break;
        case CategoriaOggetto::SCOOTER_PART:
            str = "L'enorme meccanico ti afferra con una sola mano, ti riempie di pugni, e non esita a scaraventare te e il tuo motorino merdoso fuori dall'officina.";
            break;
        default:
            str = "Te sei bruciato tutti i soldi, coglione...";
        }

        Messaggio msg{ TipoMsg::ERRORE, "Non hai abbastanza soldi...", str };
        PushMessaggio(msg);
        // TODO: da spostare?
        NuovoGiorno();
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
        default:
            WriteLog("Errore AzioneCompra");
        }
    }
    else if (prod.GetCategoria() == CategoriaOggetto::CONSUMABILE)
    {
        const Sizze& sizze{ static_cast<const Sizze&>(prod) };
        m_tabbyGuy.IncSizze(20);
        m_tabbyGuy.IncFama(sizze.GetFama());
        Messaggio msg{ TipoMsg::INFO, "ART. 46 L. 29/12/1990 n. 428", m_frasiSigarette[GenRandomInt(0, m_frasiSigarette.size() - 1)] };
        PushMessaggio(msg);
    }
    else if (prod.GetCategoria() == CategoriaOggetto::TELEFONO)
    {
        const Telefono& tel{ static_cast<const Telefono&>(prod) };
        // Trasferiamo la sim e il credito
        const Abbonamento abb = m_tabbyGuy.GetTelefono().GetAbbonamento();
        long long cred = m_tabbyGuy.GetTelefono().GetCredito();

        m_tabbyGuy.SetTelefono(tel);
        m_tabbyGuy.GetTelefono().SetAbbonamento(abb);
        m_tabbyGuy.GetTelefono().IncCredito(cred);
        m_tabbyGuy.IncFama(tel.GetFama());
    }
    else if (prod.GetCategoria() == CategoriaOggetto::SCOOTER)
    {
        if (m_tabbyGuy.HaScooter())
        {
            long long rottam = 600;
            Messaggio msg{ TipoMsg::INFO, "Incentivi", "Per il tuo vecchio scooter da rottamare ti diamo "+GetSoldiStr(rottam)+" di supervalutazione..."};
            PushMessaggio(msg);

            m_tabbyGuy.GuadagnaSoldi(rottam);
        }
        
        const Scooter& scooter{ static_cast<const Scooter&>(prod) };

        m_tabbyGuy.SetScooter(scooter);
        m_tabbyGuy.GetScooter().IncBenza(2);
        m_tabbyGuy.GetScooter().SetAttivita(Attivita::IN_GIRO);

        Messaggio msg{ TipoMsg::INFO, "Lo scooter nuovo...", "Fai un giro del quartiere per farti vedere con lo scooter nuovo..."};
        PushMessaggio(msg);

        m_tabbyGuy.IncRep(4);
        
    }
    else if (prod.GetCategoria() == CategoriaOggetto::SCOOTER_PART)
    {
        const Pezzo& pezzo{ static_cast<const Pezzo&>(prod) };
        PlaySound(101);
        m_tabbyGuy.GetScooter().InstallaPezzo(pezzo);
        CheckScooter();
    }

    WriteLog("AzioneCompra: acquista " + prod.GetNome() + " per " + GetSoldiStr(prod.GetPrezzo()));
    NuovoGiorno();
}

bool TabbyGame::TriggerPalestra()
{
    if (m_tipoGiorno != TipoGiorno::FESTIVO)
        return true;

    Messaggio msg{ TipoMsg::ERRORE, "Palestra", "Il tuo fisico da atleta dovrà aspettare... visto che oggi la palestra è chiusa..." };
    PushMessaggio(msg);

    return false;
}

void TabbyGame::AzioneVaiPalestra()
{
    if (!PalestraAttiva())
    {
        Messaggio msg{ TipoMsg::AVVISO, "Palestra", "Prima di poter venire in palestra devi fare un abbonamento !" };
        PushMessaggio(msg);
        return;
    }

    PlaySound(201);
    if (m_tabbyGuy.GetFama() < 82)
        m_tabbyGuy.IncFama(1);

    // EVENTI PALESTRA
    int rnd = GenRandomInt(0, 28 + m_tabbyGuy.GetFortuna() * 0.5f);

    // AVVIENE L'EVENTO RANDOMICO
    if (rnd < 9)
    {
        rnd = GenRandomInt(0, m_frasiPalestra.size() - 1);
        Messaggio msg{ TipoMsg::INFO, "Palestra...", m_frasiPalestra[rnd] };
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

                Messaggio msg{ TipoMsg::AVVISO, "Lampada", "L'eccessiva esposizione del tuo corpo ai raggi ultravioletti provoca un avanzato grado di carbonizzazione e pure qualche piccola mutazione genetica..." };
                PushMessaggio(msg);
            }

            PlaySound(202);
            WriteLog("AzioneLampada: Paga " + GetSoldiStr(PREZZO_LAMPADA));
        }
        else
        {
            Messaggio msg{ TipoMsg::ERRORE, "Non hai abbastanza soldi...", "L'enorme istruttore di bodybulding ultra-palestrato ti suona come una zampogna e ti scaraventa fuori dalla palestra." };
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
        Messaggio msg{ TipoMsg::ERRORE, "Ma che ?", "Hai già un abbonamento, perché te ne serve un altro ???" };
        PushMessaggio(msg);
        return;
    }

    if (!m_tabbyGuy.SpendiSoldi(importo))
    {
        Messaggio msg{ TipoMsg::ERRORE, "Non hai abbastanza soldi...", "L'enorme istruttore di bodybulding ultra-palestrato ti suona come una zampogna e ti scaraventa fuori dalla palestra." };
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
        Messaggio msg{ TipoMsg::ERRORE, "Ma che ???", "Che telefonino vuoi vendere, pirletta ???" };
        PushMessaggio(msg);
        return;
    }

    m_offertaTel = m_tabbyGuy.GetTelefono().GetPrezzo() * 0.5f + 8;

    Messaggio msg{ TipoMsg::SCELTA, "Un buon affare...", "Ti posso dare " + GetSoldiStr(m_offertaTel) + " per il tuo telefonino... Vuoi vendermelo ?", Scelta::VENDI_TEL };
    PushMessaggio(msg);
}

void TabbyGame::AzioneAttivaSim(int abbonIndex)
{
    if (!m_tabbyGuy.SpendiSoldi(m_abbonamenti[abbonIndex].GetCostoAttivazione()))
    {
        Messaggio msg{ TipoMsg::ERRORE, "Non hai abbastanza soldi...", "Forse non ti sei accorto di non avere abbastanza soldi, stronzetto..." };
        PushMessaggio(msg);
        return;
    }

    m_tabbyGuy.GetTelefono().SetAbbonamento(m_abbonamenti[abbonIndex]);

    if(m_tabbyGuy.HaTelefono())
        PlaySound(602);
}

void TabbyGame::AzioneRicarica(long long taglio, std::string nomeOp)
{
    if (m_tabbyGuy.GetTelefono().GetAbbonamento().GetNome() != nomeOp)
    {
        Messaggio msg{ TipoMsg::ERRORE, "Svegliati...", "Oh, che te ne fai di una ricarica se non hai la sim ???" };
        PushMessaggio(msg);
        return;
    }

    if (!m_tabbyGuy.SpendiSoldi(taglio))
    {
        Messaggio msg{ TipoMsg::ERRORE, "Non hai abbastanza soldi...", "Forse non ti sei accorto di non avere abbastanza soldi, stronzetto..." };
        PushMessaggio(msg);
        return;
    }

    m_tabbyGuy.GetTelefono().IncCredito(taglio);

    if(m_tabbyGuy.HaTelefono())
        PlaySound(602);
}

void TabbyGame::AzioneVendiScooter()
{
    if (!m_tabbyGuy.HaScooter())
    {
        Messaggio msg{ TipoMsg::ERRORE, "Ma che ???", "Scusa, ma quale scooter avresti intenzione di vendere visto che non ne hai neanche uno ???" };
        PushMessaggio(msg);
        return;
    }

    const Scooter& s = m_tabbyGuy.GetScooter();

    if (s.GetAttivita() == Attivita::INGRIPPATO || s.GetAttivita() == Attivita::INVASATO)    // Se lo scooter è sputtanato vale meno...
        m_offertaScooter = s.GetPrezzo() * 0.01f * (s.GetStato() - 50 - GenRandomInt(0, 9)) * 0.6f;
    else
        m_offertaScooter = s.GetPrezzo() * 0.01f * (s.GetStato() - 10 - GenRandomInt(0, 9)) * 0.6f;

    if (m_offertaScooter < 30)  // Se vale meno di 30 euro nessuno lo vuole
        m_offertaScooter = 30;

    Messaggio msg{ TipoMsg::SCELTA, "Vendi lo scooter", "Il tuo scooterino viene valutato per " + GetSoldiStr(m_offertaScooter)+". \nLo vendi ?", Scelta::VENDI_SCOOTER };
    PushMessaggio(msg);
}

void TabbyGame::AzioneRiparaScooter()
{
    if (!m_tabbyGuy.HaScooter())
    {
        Messaggio msg{ TipoMsg::ERRORE, "Hai le allucinazioni ???", "Mi spieghi come fai a farti riparare lo scooter se manco ce l'hai ???" };
        PushMessaggio(msg);
        return;
    }
    else if (m_tabbyGuy.GetScooter().GetStato() == 100)
    {
        Messaggio msg{ TipoMsg::ERRORE, "Ripara lo scooter", "Che motivi hai per voler riparare il tuo scooter visto ch è al 100% di efficienza ???" };
        PushMessaggio(msg);
        return;
    }

    m_costoRiparazione = m_tabbyGuy.GetScooter().GetPrezzo() * 0.01f * (100 - m_tabbyGuy.GetScooter().GetStato()) + 10;

    Messaggio msg{ TipoMsg::SCELTA, "Ripara scooter", "Vuoi riparare lo scooter per " + GetSoldiStr(m_costoRiparazione) + " ?", Scelta::RIPARA_SCOOTER };
    PushMessaggio(msg);
}

void TabbyGame::AzioneUsaScooter()
{
    if (!m_tabbyGuy.HaScooter())
    {
        Messaggio msg{ TipoMsg::ERRORE, "Ma che ???", "Mi spieghi come fai a parcheggiare lo scooter se manco ce l'hai ???" };
        PushMessaggio(msg);
        return;
    }

    if (m_tabbyGuy.GetScooter().GetAttivita() != Attivita::IN_GIRO)
    {
        if (m_tabbyGuy.GetScooter().GetAttivita() != Attivita::PARCHEGGIATO)
        {
            Messaggio msg{ TipoMsg::ERRORE, "Ma che ???", "Mi spieghi come fai a parcheggiare lo scooter visto che è "+m_tabbyGuy.GetScooter().GetAttivitaStr(true)+" ???"};
            PushMessaggio(msg);
        }
        else
            m_tabbyGuy.GetScooter().SetAttivita(Attivita::IN_GIRO);
    }
    else
        m_tabbyGuy.GetScooter().SetAttivita(Attivita::PARCHEGGIATO);
}

void TabbyGame::AzioneFaiBenza()
{
    if (!m_tabbyGuy.HaScooter())
    {
        Messaggio msg{ TipoMsg::ERRORE, "Ma che ???", "Mi spieghi come fai a far benzina allo scooter se manco ce l'hai ???" };
        PushMessaggio(msg);
        return;
    }

    long long costoBenza = 10;

    if (!m_tabbyGuy.SpendiSoldi(costoBenza))
    {
        Messaggio msg{ TipoMsg::INFO, "Fai benzina", "Al distributore automatico puoi fare un minimo di "+GetSoldiStr(costoBenza)+" di benzina..." };
        PushMessaggio(msg);
        return;
    }

    m_tabbyGuy.GetScooter().IncBenza(5);
}

bool TabbyGame::TriggerMeccanico()
{
    if (m_tabbyGuy.HaScooter())
        return true;

    Messaggio msg{ TipoMsg::ERRORE, "Ma che ???", "Scusa, ma quale scooter avresti intenzione di truccare visto che non ne hai neanche uno ???" };
    PushMessaggio(msg);
    return false;
}

Scooter TabbyGame::GeneraScooter()
{
    const std::vector<Acquistabile*> vec = m_concessionario.m_catalogo;
    const Scooter& scooter{ static_cast<const Scooter&>(*(vec[GenRandomInt(0,vec.size() - 1)])) };

    return scooter;
}