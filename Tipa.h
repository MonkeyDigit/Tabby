#pragma once
#include <string>

class Tipa
{
public:
	Tipa();	// Constructor
	std::string GetNome() const { return m_nome; };
	int GetFigo() const { return m_figo; };
	int GetRapporto() const { return m_rapporto; };
private:
	std::string m_nome;
	int m_figo;
	int m_rapporto;
};