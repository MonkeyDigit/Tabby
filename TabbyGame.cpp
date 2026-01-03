#include "TabbyGame.h"

TabbyGame::TabbyGame()	// Lunedì 16 settembre 1991
	: m_tabbyGuy{}, m_date{1991, 9, 16}, m_valutaCorrente{Valuta::LIRE}, m_coolDownPestaggio{ 0 }
{
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

    // Chiamata ai reparti
    GestioneConsumi();
    GestioneRelazioni();
    GestioneLavoro();
    GestioneEconomia();
    GestioneEventiCasuali();    // Il dado del destino...
}

// TODO: RENDI MODULARE???
void TabbyGame::AvanzaCalendario()
{
    m_date.AddDay(1);
    
    if (Chrono::leapYear(m_date.GetYear()) &&
        m_date.GetMonth() == Chrono::Month::feb &&
        m_date.GetDay() == 29)
    {
        // TODO: Dialog anno bisesto anno funesto
    }

    // ---------------> S T I P E N D I O <---------------

    if (m_tabbyGuy.GetImpegno() > 0)
    {
        m_tabbyGuy.IncGiorniLavoro(1);

        if (m_date.GetDay() == GIORNO_STIPENDIO)
        {
            long stipendietto{};  // Stipendio calcolato secondo i giorni effettivi di lavoro

            if (m_tabbyGuy.GetGiorniLavoro() > 29)
                stipendietto = m_tabbyGuy.GetStipendio();
            else
                stipendietto = m_tabbyGuy.GetStipendio() * (long)m_tabbyGuy.GetGiorniLavoro() / 30;

            m_tabbyGuy.ResetGiorniLavoro();

            // TODO: EVENTO stipendio da sottomesso
            m_tabbyGuy.GuadagnaSoldi(stipendietto);

            // TODO: DEBUG
        }

    }

    // ---------------> P A L E S T R A <---------------
    if (m_date == m_tabbyGuy.GetScadenzaGym())
    {
        // TODO: Dialog abbonamento scaduto
        // TODO: RESETTA DATA SCADENZA
        // TODO: DEBUG
    }

    switch (m_date.GetMonth())
    {
    case Chrono::Month::jan:    // Gennaio
        if (m_date.GetDay() < 7)
        {
            // TODO: Cos'è sta roba x vacanza
        }
        break;

    case Chrono::Month::jun:    // Giugno
        if (m_date.GetDay() == 15)
        {
            // TODO: DIALOG vacanze estive
        }
        
        if (m_date.GetDay() > 15)
        {
            if (m_date.GetDay() == 22)
            {
                // TODO: DIALOG pagella
            }

            // TODO: roba vacanza
        }
        break;

    case Chrono::Month::jul:    // Luglio
    case Chrono::Month::aug:    // Agosto
        // TODO: roba vacanza
        // TODO: roba tipa
        break;

    case Chrono::Month::sep:    // Settembre

        if (m_date.GetDay() < 15)
        {
            // TODO: Roba vacanza
        }

        if (m_date.GetDay() == 15)
        {
            // TODO: Dialog rientro scuola
            m_tabbyGuy.GetScuola().Reset();
        }
        break;

    case Chrono::Month::dec:    // Dicembre
        if (m_date.GetDay() > 22)
        {
            // TODO: Roba vacanza
            // TODO: Roba tipa

            if (m_date.GetDay() == 25)
            {   // TODO: VESTITI NATALIZI
                if (m_tabbyGuy.GetPantaloni() == 19 && m_tabbyGuy.GetGiubotto() == 19)
                {
                    // TODO: Dialog vestito stupendo
                    m_tabbyGuy.IncFama(20);
                }
            }
            else if (m_date.GetDay() == 28 && m_tabbyGuy.GetPantaloni() == 19 && m_tabbyGuy.GetGiubotto() == 19)
            {
                // TODO: Dialog togliti il vestito
                m_tabbyGuy.DecFama(5);
            }
        }
    }

    // TODO: vacanze tipo 2
    // Domeniche e festività varie
    if (m_date.GetWeekDay() == Chrono::WeekDay::sunday)
    {
        // TODO: vacanza = 2 ?
    }

    // TODO: if natale
    /*
    if (natale2 == 0)
    {
        int a = 0;
        while (InfoVacanze[a].giorno != 0)
        {
            if (InfoVacanze[a].mese == x_mese)
                if (InfoVacanze[a].giorno == x_giorno)
                {
                    MessageBox(hInstance,
                               InfoVacanze[a].descrizione,
                               InfoVacanze[a].nome, MB_OK | MB_ICONINFORMATION);

                    x_vacanza = 2; // 2 = sono chiusi anche i negozi...
                }
                a++;
        }
    }
    */

    // TODO: DEBUG
}

void TabbyGame::GestioneConsumi()
{
    // Sigarette
    if (m_tabbyGuy.GetSizze() > 0)
    {
        m_tabbyGuy.DecSizze(1);

        if (m_tabbyGuy.GetSizze() == 0)
        {
            // TODO: Dialog sigarette esaurite
            if (m_tabbyGuy.GetRep() > 10)
                m_tabbyGuy.DecRep(3);
        }
        else if (m_tabbyGuy.GetSizze() < 3)
        {
            // TODO: Dialog stai per finire le siga
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
            // TODO: Dialog credito esaurito
        }
        else if (m_tabbyGuy.GetOperatore().GetCredito() < 3)
        {
            // TODO: Dialog poco credito
        }
    }

    // Telefono
    if (m_tabbyGuy.GetTelefono().GetStato() == 1)
    {
        // Cellulare morente
        m_tabbyGuy.GetTelefono().DecStato(1);

        // TODO: Dialog telefono morto
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

    if (m_tabbyGuy.GetNumDitta() > 0)
    {
        rnd = GenRandomInt(0, (m_tabbyGuy.GetImpegno() * 2 + m_tabbyGuy.GetFortuna() * 3) - 1);

        if (rnd < 2)
        {
            // Perdi il lavoro
            m_tabbyGuy.ResetLavoro();

            // TODO: Dialog licenziato + suono

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
            // TODO: DEBUG

            //TODO: Dialog eventi paghetta

            if (m_tabbyGuy.GetStudio() >= 80)
            {
                m_tabbyGuy.GuadagnaSoldi(m_tabbyGuy.GetPaghetta());
                //TODO: DEBUG

                // TODO: Dialog eventi paghetta doppia + suono
            }
        }
        else
        {
            m_tabbyGuy.GuadagnaSoldi(m_tabbyGuy.GetPaghetta() * 0.5f);
            // TODO: DEBUG

            // TODO: Dialog eventi metà paghetta
        }
    }
}

void TabbyGame::GestioneEventiCasuali()
{
    // Eventi casuali
    int caso = GenRandomInt(0, (100 + m_tabbyGuy.GetFortuna() * 2) - 1);
    int rnd{};
    // TODO: DEBUG

    if (caso < 51)
    {
        // METALLONI E MANOVALI
        if (caso <= 10)
        {
            // TODO: FEMMINA

            m_tabbyGuy.DecRep(caso);

            // TODO: FINESTRA PESTAGGIO METALLONE

            rnd = GenRandomInt(100, 105);

            // TODO: DEBUG

            m_coolDownPestaggio = 5;
        }
        else if (11 <= caso && caso <= 20)   // SCOOTER
        {
            // TODO: perchè nell'originale c'è & ???
            if (m_tabbyGuy.GetScooter().GetStato() != -1 && m_tabbyGuy.GetScooter().GetAttivita() == 1)
            {
                if (m_tabbyGuy.GetTelefono().GetStato() > -1)
                {
                    // A furia di prendere botte, il cellulare si spacca...
                    m_tabbyGuy.GetTelefono().DecStato(GenRandomInt(1, 8));
                }

                if (caso < 17)
                {
                    m_tabbyGuy.GetScooter().DecStato(35);
                    // TODO: Dialog camionista

                    // TODO: DEBUG
                }
                else
                {
                    m_tabbyGuy.GetScooter().DecStato(20);
                    // TODO: Dialog muro

                    // TODO: DEBUG
                }

                m_tabbyGuy.DecRep(2);

                if (m_tabbyGuy.GetScooter().GetStato() <= 0)
                {
                    m_tabbyGuy.GetScooter().Reset();
                    // TODO: Dialog scooter distrutto

                    // TODO: DEBUG
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
            // TODO: Dialog sei fortunato... random

            // TODO: DEBUG
        }
        else if (31 <= caso && caso <= 40)   // Skuola
        {
            // Durante i giorni di vacanza non ci sono eventi riguardanti la scuola
            if (!CheckVacanza())
            {
                rnd = GenRandomInt(0, m_tabbyGuy.GetScuola().m_materie.size() - 1);

                // TODO: Dialog scuola random

                m_tabbyGuy.GetScuola().m_materie[rnd].DecVoto(2);
                m_tabbyGuy.CalcolaStudio();

                // TODO: DEBUG
                // TODO: Scuola redraw
            }
        }
        else if (caso == 41 || caso == 42)  // Tipa - una tipa ci prova
        {
            if (m_tabbyGuy.GetFama() > 35)   // Fama < 35 = nessuna speranza...
            {
                // TODO: Sesso m f

                // TODO: Dialog una tipa ci prova
            }

            // TODO: Dialog qualcuno ti caga...

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
            }
            else   // Bravo, non hai una tipa...
            {
                // TODO: DEBUG
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

            // TODO: DEBUG
        }
        else if (caso == 43)    // Domande inutili
        {
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

            // TODO: DEBUG
        }
        else if (caso == 44)
        {
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
            //  TODO: DEBUG
        }
        else if (45 <= caso && caso <= 48)
        {
            // TODO: DEBUG
        }
        else if (caso == 49 || caso == 50)  // Vari ed eventuali
        {
            if (m_tabbyGuy.GetTelefono().GetStato() > -1)
            {
                m_tabbyGuy.GetTelefono().DecStato(GenRandomInt(1, 8));

                // TODO: Dialog telefono morto

                // TODO: DEBUG
            }
        }
    }
}

bool TabbyGame::CheckVacanza()
{
    return false;
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

void TabbyGame::ApplicaScelta(int idEvento, bool sceltaYes)
{

}

void TabbyGame::AzioneStudia(int materiaIndex)
{
    Materia& mat = m_tabbyGuy.GetScuola().m_materie[materiaIndex];

    if (mat.GetVoto() < 10)
    {
        mat.IncVoto(1);
        m_tabbyGuy.CalcolaStudio();
        NuovoGiorno();
    }
    else
    {
        // TODO: MESSAGGIO
    }
}

void TabbyGame::AzioneMinaccia(int materiaIndex)
{
    Materia& mat = m_tabbyGuy.GetScuola().m_materie[materiaIndex];

    if (mat.GetVoto() > 0)
    {
        mat.DecVoto(2);
        m_tabbyGuy.CalcolaStudio();
        NuovoGiorno();
    }
    else
    {
        // TODO: MESSAGGIO
    }
}

void TabbyGame::AzioneCorrompi(int materiaIndex)
{
}

void TabbyGame::CheckCambioValuta()
{	// Dopo il 2002 scatta l'euro
	if (m_valutaCorrente == Valuta::LIRE && m_date.GetYear() >= 2002)
		m_valutaCorrente = Valuta::EURO;
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