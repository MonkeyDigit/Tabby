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
    std::string m_presentazione;
    std::string m_produzioni;
    OffertaLavoro m_offerta;
    std::string m_immagine;
};

class Carriera {
public:
    Carriera() : m_nomeDitta(""), m_stipendio(0), m_impegno(0), m_giorniLavorati(0) {}

    // Metodi di stato
    bool IsDisoccupato() const { return (m_nomeDitta == ""); }

    void Azzera() {
        m_nomeDitta = "";
        m_stipendio = 0;
        m_impegno = 0;
        m_giorniLavorati = 0;
    }

    // Getters & Setters
    std::string GetNomeDitta() const { return m_nomeDitta; }
    long GetStipendio() const { return m_stipendio; }
    int GetImpegno() const { return m_impegno; }
    void IncImpegno(int punti) {
        if (punti > 0) m_impegno += punti;
        if (m_impegno > 100) m_impegno = 100;
    }
    void DecImpegno(int punti) {
        if (punti > 0)
        {
            m_impegno -= punti;
            if (m_impegno < 0) m_impegno = 0;
        }
    }
    void SetImpegno(int impegno) {
        m_impegno = impegno;
        if (m_impegno < 0) m_impegno = 0;
        else if (m_impegno > 100) m_impegno = 100;
    }
    void SetGiorniLavorati(int gg) {
        m_giorniLavorati = gg;
        if (m_giorniLavorati < 0) m_giorniLavorati = 0;
    }
    void IncStipendio(long long importo) {
        if (importo > 0) m_stipendio += importo;
    }
    void SetStipendio(long long soldi) {
        m_stipendio = soldi;
        if (m_stipendio < 0) m_stipendio = 0;
    }


    void SetDitta(std::string nomeDitta) { m_nomeDitta = nomeDitta; };


    // Gestione tempo
    void Lavora() { if (!IsDisoccupato()) m_giorniLavorati++; }
    int GetGiorniLavorati() const { return m_giorniLavorati; }
    void ResetGiorni() { m_giorniLavorati = 0; }

private:
    std::string m_nomeDitta;    // "" = Nessuna
    long m_stipendio;           // Stipendio attuale
    int m_impegno;              // Quanto ti sbatti
    int m_giorniLavorati;       // Contatore per la busta paga
};

struct QuizDomanda {
    std::string m_testo;
    std::vector<std::string> m_risposte;
};

struct QuizScheda {
    std::string m_titolo;
    std::string m_intro;
    std::vector<QuizDomanda> m_domande;
};