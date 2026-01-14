#include "TabbyGuy.h"

TabbyGuy::TabbyGuy()
	: m_soldi{ 0 }, m_soldiDelta{ 0 }, m_fama{ 0 }, m_paghetta{ 0 }, m_rep{ 0 }, m_studio{ 0 },
	m_rapporti{ 0 }, m_sizze{ 0 }, m_pelle{ Pelle::ABBR_NO }, m_fortuna{ 0 },
	m_identita{}, m_giubbotto{}, m_pantaloni{}, m_scarpe{}, m_carriera{}, m_scuola{}, m_scooter{},
	m_tipa{}, m_telefono{}
{}

void TabbyGuy::SetRapporti(const int rapporti)
{
	m_rapporti = rapporti;
	if (m_rapporti < 0) m_rapporti = 0;
	else if (m_rapporti > 100) m_rapporti = 100;
}

bool TabbyGuy::SpendiSoldi(const long long importo)
{	
	// Con quali soldi...
	if (importo < 0 || importo > m_soldi)
		return false;

	m_soldi -= importo;
	m_soldiDelta -= importo;
	return true;
}

void TabbyGuy::GuadagnaSoldi(const long long importo)
{
	if (importo > 0)
	{
		m_soldi += importo;
		m_soldiDelta += importo;
	}
}

void TabbyGuy::IncPaghetta(const int importo)
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

void TabbyGuy::SetFortuna(const int fortuna)
{
	m_fortuna = fortuna;
	if (m_fortuna > 100) m_fortuna = 100;
	else if (m_fortuna < 0) m_fortuna = 0;
}

void TabbyGuy::IncSizze(const int sizze) {
	if (sizze > 0) m_sizze += sizze;
}

void TabbyGuy::DecSizze(const int sizze) {
	if (sizze > 0)
	{
		m_sizze -= sizze;
		if (m_sizze < 0) m_sizze = 0;
	}
}

void TabbyGuy::IncRep(const int punti) {
	if (punti > 0)
	{
		m_rep += punti;
		if (m_rep > 100)
			m_rep = 100;
	}
}

void TabbyGuy::DecRep(const int punti) {
	if (punti > 0)
	{
		m_rep -= punti;
		if (m_rep < 0) m_rep = 0;
	}
}

void TabbyGuy::IncFama(const int punti) {
	if (punti > 0)
	{
		m_fama += punti;
		if (m_fama > 100) m_fama = 100;
	}
}

void TabbyGuy::DecFama(int punti) {
	if (punti > 0)
	{
		m_fama -= punti;
		if (m_fama < 0) m_fama = 0;
	}
}

void TabbyGuy::IncRapporti(int punti) {
	if (punti > 0)
	{
		m_rapporti += punti;
		if (m_rapporti > 100) m_rapporti = 100;
	}
}

void TabbyGuy::DecRapporti(int punti) {
	if (punti > 0)
	{
		m_rapporti -= punti;
		if (m_rapporti < 0) m_rapporti = 0;
	}
}

void TabbyGuy::LasciaTipa()
{
	m_tipa.Azzera();
	m_rapporti = 0;
}

void TabbyGuy::Licenziati()
{
	m_carriera.Azzera();
}

bool TabbyGuy::SetGiubbotto(const Vestito& giubbotto)
{
	if(giubbotto.GetTipoVestito() != TipoVestito::GIUBBOTTO)
		return false;

	m_giubbotto = giubbotto;

	return true;
}

bool TabbyGuy::SetPantaloni(const Vestito& pantaloni)
{
	if (pantaloni.GetTipoVestito() != TipoVestito::PANTALONI)
		return false;

	m_pantaloni = pantaloni;

	return true;
}

bool TabbyGuy::SetScarpe(const Vestito& scarpe)
{
	if (scarpe.GetTipoVestito() != TipoVestito::SCARPE)
		return false;

	m_scarpe = scarpe;

	return true;
}

void TabbyGuy::Abbronza()
{
	if (m_pelle < Pelle::ABBR_CARBONIZZATO)
		m_pelle = Pelle((int)m_pelle + 1);
}

void TabbyGuy::Sbianca()
{
	if (m_pelle > Pelle::ABBR_NO)
		m_pelle = Pelle((int)m_pelle - 1);
}

std::string TabbyGuy::GetPelleStr()
{
	switch (m_pelle)
	{
	case Pelle::ABBR_NO:
		return "Non abbronzato";
	case Pelle::ABBR_LIEVE:
		return "Abbronzatura lieve";
	case Pelle::ABBR_MEDIA:
		return "Abbronzatura media";
	case Pelle::ABBR_PESANTE:
		return "Abbronzatura pesante";
	case Pelle::ABBR_CARBONIZZATO:
		return "Carbonizzato";
	default:
		return "Errore";
	}
}

void TabbyGuy::SetFama(const int punti)
{
	m_fama = punti;
	if (m_fama > 100) m_fama = 100;
	else if (m_fama < 0) m_fama = 0;
}

void TabbyGuy::SetRep(const int punti)
{
	m_rep = punti;
	if (m_rep > 100) m_rep = 100;
	else if (m_rep < 0) m_rep = 0;
}

void TabbyGuy::SetStudio(const int punti)
{
	m_studio = punti;
	if (m_studio > 100) m_studio = 100;
	else if (m_studio < 0) m_studio = 0;
}
void TabbyGuy::SetSizze(const int sizze)
{
	m_sizze = sizze;
	if (m_sizze > 100) m_sizze = 100;
	else if (m_sizze < 0) m_sizze = 0;
}