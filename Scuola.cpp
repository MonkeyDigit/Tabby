#include "Scuola.h"

Materia::Materia(std::string nome)
	: m_nome{nome}, m_voto{0}
{}

void Materia::IncVoto(int punti) {
	if (punti > 0)
	{
		m_voto += punti;
		if (m_voto > 10) m_voto = 10;
	}
}

void Materia::DecVoto(int punti) {
	if (punti > 0)
	{
		m_voto -= punti;
		if (m_voto < 0) m_voto = 0;
	}
}

void Materia::SetVoto(int voto)
{
	if (0 <= voto && voto <= 10)
		m_voto = voto;
}

Scuola::Scuola()
	: m_materie{
	Materia("Agraria"),
	Materia("Fisica"),
	Materia("Attività culturali"),
	Materia("Attività matematiche"),
	Materia("Scienze industriali"),
	Materia("Elettrochimica"),
	Materia("Petrolchimica"),
	Materia("Filosofia aziendale"),
	Materia("Metallurgia")
	}
{}

void Scuola::Reset()
{
	for (auto& mat : m_materie)
		mat.Reset();
}