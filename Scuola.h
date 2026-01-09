#pragma once
#include <string>
#include <vector>

class Materia {
public:
	Materia(std::string nome)
		: m_nome{ nome }, m_voto{ 0 }
	{}

	std::string GetNome() const { return m_nome; };
	int GetVoto() const { return m_voto; };

	void IncVoto(int punti) {
		if (punti > 0)
		{
			m_voto += punti;
			if (m_voto > 10) m_voto = 10;
		}
	}

	void DecVoto(int punti) {
		if (punti > 0)
		{
			m_voto -= punti;
			if (m_voto < 0) m_voto = 0;
		}
	}

	void SetVoto(int voto) {
		if (0 <= voto && voto <= 10)
			m_voto = voto;
	}

	void Reset() { m_voto = 0; };
private:
	std::string m_nome;
	int m_voto;
};

struct Scuola {
	Scuola()
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
	void Reset(){
		for (auto& mat : m_materie)
			mat.Reset();
	}

	std::vector<Materia> m_materie;
};