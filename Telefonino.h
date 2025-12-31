#pragma once
#include <string>

class OperatoreTel {
public:

private:
	std::string m_nome;
	bool m_abbonamento;	// 0 = Ricarica, 1 = Abbonamento
	int m_dualOnly;		// Dual band only?
	int m_credito;
	int m_fama;
	int m_prezzo;
};

class Telefonino
{
public:

private:
	std::string m_nome;
	int m_prezzo;
	int m_dual;	// Dual band?
	int m_fama;	// figosità
};

