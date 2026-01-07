#include "Tipa.h"

Tipa::Tipa()
    : m_nome{""}, m_desc{""}, m_fama{0}
{
}

Tipa::Tipa(std::string nome, int fama)
	: m_nome{nome}
{
	SetFama(fama);

    if (m_fama > 97)
        m_desc = "Ultramegafiga";
    else if(m_fama > 90)
        m_desc = "Fighissima";
    else if (m_fama > 83)
        m_desc = "Molto figa";
    else if (m_fama > 72)
        m_desc = "Figa";
    else if (m_fama > 67)
        m_desc = "Abbastanza Figa";
    else if (m_fama > 55)
        m_desc = "Interessante";
    else if (m_fama > 45)
        m_desc = "Passabile";
    else if (m_fama > 35)
        m_desc = "Puo' piacere..";
    else
        m_desc = "E' un tipo...";
}

void Tipa::Azzera()
{
	m_nome = "";
	m_desc = "";
	m_fama = 0;
}