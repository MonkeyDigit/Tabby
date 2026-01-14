#pragma once
#include <string>

class Tipa
{
public:
	Tipa()
		: m_nome{ "" }, m_desc{ "" }, m_fama{ 0 }
	{}

	Tipa(std::string nome, int fama)
		: m_nome{ nome }
	{
		SetFama(fama);
	}

	void SetNome(std::string nome) { m_nome = nome; }
	std::string GetNome() const { return m_nome; }
	std::string GetDesc() const { return m_desc; }
	int GetFama() const { return m_fama; }

	void SetFama(int punti) {
		m_fama = punti;
		if (m_fama < 0) m_fama = 0;
		if (m_fama > 100) m_fama = 100;

		if (m_fama > 97)
			m_desc = "Ultramegafiga";
		else if (m_fama > 90)
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
			m_desc = "Può piacere..";
		else
			m_desc = "E' un tipo...";
	}

	void Azzera()
	{
		*this = Tipa{};
	}

private:
	std::string m_nome;
	std::string m_desc;
	int m_fama;
};