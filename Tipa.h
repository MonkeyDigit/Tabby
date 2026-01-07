#pragma once
#include <string>

class Tipa
{
public:
	Tipa();
	Tipa(std::string nome, int fama);
	std::string GetNome() const { return m_nome; };
	std::string GetDesc() const { return m_desc; };
	int GetFama() const { return m_fama; };

	void SetFama(int punti) {
		m_fama = punti;
		if (m_fama < 0) m_fama = 0;
		if (m_fama > 100) m_fama = 100;
	}
	void Azzera();
private:
	std::string m_nome;
	std::string m_desc;
	int m_fama;
};