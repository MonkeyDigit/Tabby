#include "TabbyGuy.h"

TabbyGuy::TabbyGuy()
	: m_soldi{ 10 }
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

void TabbyGuy::IncPaghetta(long long importo)
{
	m_paghetta += importo;
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

void TabbyGuy::IncSizze(int sizze) {
	if (sizze > 0) m_sizze += sizze;
}

void TabbyGuy::DecSizze(int sizze) {
	if (sizze > 0)
	{
		m_sizze -= sizze;
		if (m_sizze < 0) m_sizze = 0;
	}
}

void TabbyGuy::IncRep(int punti) {
	if (punti > 0) m_rep += punti;
}

void TabbyGuy::DecRep(int punti) {
	if (punti > 0)
	{
		m_rep -= punti;
		if (m_rep < 0) m_rep = 0;
	}
}

void TabbyGuy::IncFama(int punti) {
	if (punti > 0) m_fama += punti;
}

void TabbyGuy::DecFama(int punti) {
	if (punti > 0)
	{
		m_fama -= punti;
		if (m_fama < 0) m_fama = 0;
	}
}

void TabbyGuy::IncRapporti(int punti) {
	if (punti > 0) m_rapporti += punti;
}

void TabbyGuy::DecRapporti(int punti) {
	if (punti > 0)
	{
		m_rapporti -= punti;
		if (m_rapporti < 0) m_rapporti = 0;
	}
}