#include "Scooter.h"

Scooter::Scooter()
	: m_nome{"Mizzubisci R200 Millenium"}, 
	m_velocita{-1}, m_cilindrata{-1}, 
	m_efficienza{75}, m_benzina{-1.0},
	m_stato{-1}, m_fama{-1}, m_marmitta{0},
	m_carburatore{0}, m_filtro{0}, m_attivita{Attivita::NESSUNA},
	m_prezzo{3800}
{}

void Scooter::IncStato(int punti) {
	if (punti > 0) m_stato += punti;
}

void Scooter::DecStato(int punti) {
	if (punti > 0)
	{
		m_stato -= punti;
		if (m_stato < 0) m_stato = 0;
	}
}

void Scooter::IncBenzina(int litri) {
	if (litri > 0) m_benzina += litri;
}

void Scooter::DecBenzina(int litri) {
	if (litri > 0)
	{
		m_benzina -= litri;
		if (m_benzina < 0) m_benzina = 0;
	}
}

void Scooter::Reset()
{
	m_stato = -1;
	m_attivita = Attivita::NESSUNA;
	m_nome = "Nessuno Scooter";
}

std::string Scooter::GetAttivitaStr(bool tolower) const
{
	std::string s{};
	switch (m_attivita)
	{
	case Attivita::NESSUNA:
		s = "Nessuna";
		break;
	case Attivita::PARCHEGGIATO:
		s = "Parcheggiato";
		break;
	case Attivita::IN_GIRO:
		s = "In giro";
	}

	if (tolower)
		s[0] = std::tolower(s[0]);

	return s;
}
