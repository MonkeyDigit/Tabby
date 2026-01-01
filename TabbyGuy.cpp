#include "TabbyGuy.h"

TabbyGuy::TabbyGuy()
	: m_soldi{ 12000 }, m_rep{ 20 },
	m_figo{ 75 }, m_studio{ 50 }, 
	m_scooter{}, m_haScooter{ false }, 
	m_tipa{}, m_haTipa{ false },
	m_paghetta{15}, m_testa{0},
	m_giubotto{0}, m_pantaloni{0},
	m_scarpe{0}, m_sesso{0}
{
	m_identita.m_nome = "Alfonso";
	m_identita.m_cognome = "Signorini";
	m_identita.m_dataNascita = Chrono::Date{ 1971, 8, 10 };
	m_identita.m_attoNascita = "Atto n. 6333 P. 1 S. A";
	m_identita.m_luogoNascita = "Cusano Milanino";
	m_identita.m_provinciaNascita = "MI";
	m_identita.m_cittadinanza = "Italiana";
	m_identita.m_residenza = "Milano";
	m_identita.m_indirizzo = "Via Bagarotti n.123";
	m_identita.m_statoCivile = "Celibe";
	m_identita.m_professione = "Sfruttato";
}

bool TabbyGuy::SpendiSoldi(int importo)
{	
	// Con quali soldi...
	if (importo > m_soldi)
		return false;

	m_soldi -= importo;
	return true;
}

void TabbyGuy::GuadagnaSoldi(int importo)
{
	if (importo > 0)
		m_soldi += importo;
}
