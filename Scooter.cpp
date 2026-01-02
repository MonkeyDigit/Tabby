#include "Scooter.h"

Scooter::Scooter()
	: m_nome{"Mizzubisci R200 Millenium"}, 
	m_velocita{-1}, m_cilindrata{-1}, 
	m_efficienza{75}, m_benzina{-1.0},
	m_stato{-1}, m_fama{-1}, m_marmitta{0},
	m_carburatore{0}, m_filtro{0}, m_attivita{0},
	m_prezzo{3800}
{}

void Scooter::IncStato(int punti)
{
	if (punti > 0)
		m_stato += punti;
}

void Scooter::DecStato(int punti)
{

	if (punti > 0)
	{
		m_stato -= punti;
		if (m_stato < 0)
			m_stato = 0;
	}
}

void Scooter::Reset()
{
	m_stato = -1;
	m_attivita = 0;
	m_nome = "Nessuno Scooter";
}