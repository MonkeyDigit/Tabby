#pragma once
#include <string>
#include <vector>

class Materia {
public:
	Materia(std::string nome);

	std::string GetNome() const { return m_nome; };
	int GetVoto() const { return m_voto; };

	void IncVoto(int punti);
	void DecVoto(int punti);
	void SetVoto(int voto);
	void Reset() { m_voto = 0; };
private:
	std::string m_nome;
	int m_voto;
};

struct Scuola {
	Scuola();
	void Reset();
	std::vector<Materia> m_materie;
};