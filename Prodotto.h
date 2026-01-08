#pragma once
#include <string>
#include <vector>

enum class TipoProd {
	MISC,
	GIUBBOTTO,
	PANTALONI,
	SCARPE,
	SIGARETTE
};

// TODO: AGGIUNGI IMMAGINE
class Prodotto
{
public:
	Prodotto(TipoProd tipoCapo, std::string nome, std::string desc, std::string icon, long long prezzo, int fama)
		: m_tipoCapo{ tipoCapo }, m_nome{ nome }, m_desc{ desc }, m_icon{ icon }
	{
		SetPrezzo(prezzo);
		SetFama(fama);
	}

	// SETTER & GETTER
	void SetPrezzo(long long prezzo)
	{
		m_prezzo = prezzo;
		if (m_prezzo < 0) m_prezzo = 0;
	}
	void SetFama(int fama)
	{
		m_fama = fama;
		if (m_fama < 0) m_fama = 0;
	}

	TipoProd GetTipoCapo() const { return m_tipoCapo; };
	std::string GetNome() const { return m_nome; };
	std::string GetDesc() const { return m_desc; };
	std::string GetIcon() const { return m_icon; };
	long long GetPrezzo() const { return m_prezzo; };
	int GetFama() const { return m_fama; };
private:
	TipoProd m_tipoCapo;
	std::string m_nome;
	std::string m_desc;
	std::string m_icon;
	long long m_prezzo;
	int m_fama;
};

struct Negozio {
	std::string m_nome;
	TipoProd m_merce;
	std::vector<Prodotto> m_catalogo;
};