#pragma once
#include <string>

struct OffertaLavoro {
	std::string m_descrizione;
	std::string m_accettaStr;
	std::string m_rifiutaStr;
};

struct Ditta {
    std::string m_nome;
    long long m_fatturato;
    std::string m_sede;
    std::string m_presetazione;
    std::string m_produzioni;
    OffertaLavoro m_offerta;
    std::string m_immagine;
};

class Carriera {
public:
    Carriera() : m_idDitta(0), m_stipendio(0), m_impegno(0), m_giorniLavorati(0) {}

    // Metodi di stato
    bool IsDisoccupato() const { return (m_idDitta == 0); }

    // Azioni
    void FattiAssumere(int idDitta, long stipendioIniziale) {
        m_idDitta = idDitta;
        m_stipendio = stipendioIniziale;
        m_impegno = 50; // TODO: RANDOMICO
        m_giorniLavorati = 0;
    }

    void Licenziati() {
        m_idDitta = 0;
        m_stipendio = 0;
        m_impegno = 0;
        m_giorniLavorati = 0;
    }

    // Getters & Setters
    int GetIdDitta() const { return m_idDitta; }
    long GetStipendio() const { return m_stipendio; }
    int GetImpegno() const { return m_impegno; }
    void IncImpegno(int punti) {
        if (punti > 0) m_impegno += punti;
    }

    void DecImpegno(int punti) {
        if (punti > 0)
        {
            m_impegno -= punti;
            if (m_impegno < 0) m_impegno = 0;
        }
    }

    // Gestione tempo
    void Lavora() { if (!IsDisoccupato()) m_giorniLavorati++; }

    int GetGiorniLavorati() const { return m_giorniLavorati; }
    void ResetGiorni() { m_giorniLavorati = 0; }

private:
    int m_idDitta;      // 0 = Nessuna
    long m_stipendio;   // Stipendio attuale
    int m_impegno;      // Quanto ti sbatti
    int m_giorniLavorati; // Contatore per la busta paga
};
