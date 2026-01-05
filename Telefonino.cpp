#include "Telefonino.h"

void OperatoreTel::IncCredito(int punti) {
	if (punti > 0) m_credito += punti;
}

void OperatoreTel::DecCredito(int punti) {
	if (punti > 0)
	{
		m_credito -= punti;
		if (m_credito < 0) m_credito = 0;
	}
}

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