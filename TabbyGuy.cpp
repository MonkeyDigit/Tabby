#include "TabbyGuy.h"

TabbyGuy::TabbyGuy()
	: m_soldi{ 10 }, m_soldiDelta{ 0 }, m_rep { 0 },
	m_fama{ 0 }, m_studio{ 0 }, m_giorniLavoro{ 0 },
	m_scooter{}, m_tipa{}, m_rapporti{ 0 }, m_stipendio{ 0 },
	m_scuola{}, m_paghetta{ 20 }, m_testa{ 0 },
	m_giubotto{ 0 }, m_pantaloni{ 0 }, m_impegno{ 0 },
	m_scarpe{ 0 }, m_fortuna{ 0 }, m_sizze{ 0 }, m_numDitta{ 0 }
{
	m_identita.m_nome = "Alfonso";
	m_identita.m_cognome = "Signorini";
	m_identita.m_dataNascita = Chrono::Date{ 1973, 8, 10 };
	m_identita.m_attoNascita = "Atto n. 6333 P. 1 S. A";
	m_identita.m_luogoNascita = "Cusano Milanino";
	m_identita.m_provinciaNascita = "MI";
	m_identita.m_cittadinanza = "Italiana";
	m_identita.m_residenza = "Milano";
	m_identita.m_indirizzo = "Via Bagarotti n.123";
	m_identita.m_statoCivile = "Celibe";
	m_identita.m_professione = "Sfruttato";
}

void TabbyGuy::SetRapporti(int rapporti)
{
	if (0 <= rapporti && rapporti <= 100)
		m_rapporti = rapporti;
}

void TabbyGuy::SetImpegno(int impegno)
{
	if (0 <= impegno && impegno <= 100)
		m_impegno = impegno;
}

bool TabbyGuy::SpendiSoldi(long long importo)
{	
	// Con quali soldi...
	if (importo < 0 || importo > m_soldi)
		return false;

	m_soldi -= importo;
	m_soldiDelta -= importo;
	return true;
}

void TabbyGuy::GuadagnaSoldi(long long importo)
{
	if (importo > 0)
	{
		m_soldi += importo;
		m_soldiDelta += importo;
	}
}

void TabbyGuy::CalcolaStudio()
{
	m_studio = 0;
	// Per ottenere la percentuale bisogna fare il rapporto tra il punteggio e il massimo di punti, che va calcolato sul numero di materie
	float adjust = m_scuola.m_materie.size() * 10 / 100.f;
	
	for (auto& mat : m_scuola.m_materie)
	{
		m_studio += mat.GetVoto();
	}

	m_studio /= adjust;
}

void TabbyGuy::SetFortuna(int fortuna)
{
	m_fortuna = fortuna;
	if (m_fortuna > 100)
		m_fortuna = 100;
	else if (m_fortuna < 0)
		m_fortuna = 0;
}

void TabbyGuy::IncSizze(int numSizze)
{
	if (numSizze > 0)
		m_sizze += numSizze;
}

void TabbyGuy::DecSizze(int numSizze)
{
	if (numSizze > 0)
	{
		m_sizze -= numSizze;
		if (m_sizze < 0)
			m_sizze = 0;
	}
}

void TabbyGuy::IncRep(int punti)
{
	if (punti > 0)
		m_rep += punti;
}

void TabbyGuy::DecRep(int punti)
{
	if (punti > 0)
	{
		m_rep -= punti;
		if (m_rep < 0)
			m_rep = 0;
	}
}

void TabbyGuy::IncFama(int punti)
{
	if (punti > 0)
		m_fama += punti;
}

void TabbyGuy::DecFama(int punti)
{
	if (punti > 0)
	{
		m_fama -= punti;
		if (m_fama < 0)
			m_fama = 0;
	}
}

void TabbyGuy::IncRapporti(int punti)
{
	if (punti > 0)
		m_rapporti += punti;
}

void TabbyGuy::DecRapporti(int punti)
{	
	if (punti > 0)
	{
		m_rapporti -= punti;
		if (m_rapporti < 0)
			m_rapporti = 0;
	}
}

void TabbyGuy::IncImpegno(int punti)
{
	if (punti > 0)
		m_impegno += punti;
}

void TabbyGuy::DecImpegno(int punti)
{
	if (punti > 0)
	{
		m_impegno -= punti;
		if (m_impegno < 0)
			m_impegno = 0;
	}
}

void TabbyGuy::IncGiorniLavoro(int giorni)
{
	if (giorni > 0)
		m_giorniLavoro += giorni;
}

void TabbyGuy::ResetGiorniLavoro()
{
	m_giorniLavoro = 0;	// TODO: nell'originale c'è 1
}

void TabbyGuy::ResetLavoro()
{
	m_impegno = 0;
	m_giorniLavoro = 0;
	m_stipendio = 0;
	m_numDitta = 0;
}
