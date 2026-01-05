#pragma once
#include <string>

class Ditta
{
public:
	Ditta();

	std::string GetNome() const { return m_nome; };

private:
	std::string m_nome;
};