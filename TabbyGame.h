#pragma once
#include "TabbyGuy.h"
#include "Chrono.h"
// Per attivare il debug
#define TABBY_DEBUG

/* TODO:
giorno vacanza
anno bisesto
data scadenza palestra
data del compleanno
attesa prima di riavere soldi
fortuna
numeroditta
impegno
giorni di lavoro -> serve a calcolare lo stipendio solo per il primo mese...
stipendio
tempo trascorso dal pestaggio

*/

class TabbyGame
{
public:
	TabbyGame();
	TabbyGuy* GetTabbyGuy() { return &m_tabbyGuy; };
private:
	TabbyGuy m_tabbyGuy;
	Chrono::Date m_date;
};