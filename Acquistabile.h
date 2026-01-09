#pragma once
#include <string>
#include <vector>

// Questo enum serve al TabbyGame per capire cosa sta comprando e fare il cast giusto
enum class CategoriaOggetto { GENERICO, VESTITO, TELEFONO, CONSUMABILE, SCOOTER, SCOOTER_PART };

// CLASSE BASE per tutto ciò che è acquistabile (quasi tutto)
class Acquistabile {
public:
    Acquistabile() : m_nome{ "" }, m_desc{ "" }, m_img{ "" }, m_prezzo{ 0 }, m_categoria{ CategoriaOggetto::GENERICO }
    {}

    // Costruttore che inizializza i dati comuni
    Acquistabile(std::string nome, std::string desc, std::string img, long long prezzo, CategoriaOggetto cat)
        : m_nome{ nome }, m_desc{ desc }, m_img{ img }, m_prezzo{ prezzo }, m_categoria{ cat }
    {}

    virtual ~Acquistabile() = default;  // Il destructor può essere personalizzato dalla classe ereditante, oppure lasciato default

    // Getter comuni (NON puri, hanno un'implementazione)
    // Li facciamo virtual così se vuoi cambiarli in una classe figlia puoi farlo
    virtual std::string GetNome() const { return m_nome; }
    virtual std::string GetDescrizione() const { return m_desc; }
    virtual std::string GetImageStr() const { return m_img; }
    virtual long long GetPrezzo() const { return m_prezzo; }
    virtual CategoriaOggetto GetCategoria() const { return m_categoria; }   // Metodo di identificazione

protected:
    // Protected - Membri che rimangono accessibili solamente alla classe base e alle figlie, così possono leggerli/modificarli direttamente se serve
    std::string m_nome;
    std::string m_desc;
    std::string m_img;
    long long m_prezzo;
    CategoriaOggetto m_categoria;
};

// VESTITI
enum class TipoVestito { NONE, GIUBBOTTO, PANTALONI, SCARPE };
class Vestito : public Acquistabile {
public:
    Vestito() : Acquistabile{}, m_fama{ 0 }, m_tipo{ TipoVestito::NONE }
    {}
    // Costruttore: Passa i dati comuni al padre, e si tiene i suoi
    Vestito(TipoVestito tipo,std::string nome, std::string desc, std::string img, long long prezzo, int fama)
        : Acquistabile{ nome, desc, img, prezzo, CategoriaOggetto::VESTITO }, // Chiama costruttore base
        m_fama{ fama }, m_tipo{ tipo }
    {}

    // Non serve riscrivere GetNome, GetPrezzo ecc... li eredita gratis!
    // Getter specifici
    int GetFama() const { return m_fama; }
    TipoVestito GetTipoVestito() const { return m_tipo; }

private:
    int m_fama;
    TipoVestito m_tipo;
};

// LE SIZZE
class Sizze : public Acquistabile {
public:
    Sizze() : Acquistabile{}, m_fama{ -1 }
    {}

    Sizze(std::string nome, std::string desc, std::string img, long long prezzo, int fama)
        : Acquistabile{ nome, desc, img, prezzo, CategoriaOggetto::CONSUMABILE },
        m_fama{ fama }
    {}

    int GetFama() const { return m_fama; }

private:
    int m_fama;
};