#include "Telefonino.h"

Telefono::Telefono(std::string nome, std::string desc, long long prezzo, int fama, int stato, long long credito, const Abbonamento& abb, std::string icon)
	: m_nome{ nome }, m_desc{ desc }, m_prezzo{ prezzo }, m_fama{ fama }, m_stato{ stato }, m_credito{ credito }, m_abb{ abb }, m_icon{ icon }
{}

void Telefono::IncStato(int punti) {
	if (punti > 0) m_stato += punti;
}

void Telefono::DecStato(int punti) {
	if (punti > 0)
	{
		m_stato -= punti;
		if (m_stato < 0) m_stato = 0;
	}
}

void Telefono::IncCredito(int punti)
{
	if (punti > 0) m_credito += punti;
}

void Telefono::DecCredito(int punti)
{
	if (punti > 0)
	{
		m_credito -= punti;
		if (m_credito < 0) m_credito = 0;
	}
}

Abbonamento::Abbonamento(std::string nome, long long attivazione, const std::vector<long long>& ricariche, std::string icon)
	: m_nome{ nome }, m_attivazione{ attivazione }, m_ricariche{ ricariche }, m_icon{ icon }
{}