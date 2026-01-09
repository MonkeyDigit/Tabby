#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/statline.h>
#include <wx/listctrl.h>
#include <wx/scrolwin.h> // Necessario per lo scroll
#include "TabbyGame.h"
#define WIN_BKG wxColor(194, 196, 184)

class DlgScooter : public wxDialog {
public:
	DlgScooter(wxWindow* parent, TabbyGame& game);
private:
	TabbyGame& m_game;	// Il riferimento al motore di gioco

	// Eventi interni alla finestra
	void OnConcessionario(wxCommandEvent& event);
	void OnTrucca(wxCommandEvent& event);
	void OnRipara(wxCommandEvent& event);
	void OnFaiBenza(wxCommandEvent& event);
	// Funzione helper
	void AddStat(wxWindow* parent, wxSizer* sizer, wxString label, wxString value);
	// Funzione per aggiornare i testi dei bottoni
	void AggiornaInterfaccia();
};

class DlgScuola : public wxDialog {
public:
	DlgScuola(wxWindow* parent, TabbyGame& game);
private:
	TabbyGame& m_game;
	int m_materiaIndex;
	std::vector<wxStaticText*> m_lblVoti;

	// I puntatori ai bottoni d'azione (per cambiargli il testo dinamicamente)
	wxButton* m_btnStudia;
	wxButton* m_btnMinaccia;
	wxButton* m_btnCorrompi;
	wxStaticText* m_lblStudio;
	wxStaticText* m_lblRep;

	// Eventi interni alla finestra
	void OnStudia(wxCommandEvent& event);
	void OnMinaccia(wxCommandEvent& event);
	void OnCorrompi(wxCommandEvent& event);
	void AggiornaInterfaccia();
};

class DlgCompagnia : public wxDialog {
public:
	DlgCompagnia(wxWindow* parent, TabbyGame& game);
private:
	TabbyGame& m_game;

	// I puntatori ai bottoni d'azione (per cambiargli il testo dinamicamente)
	wxStaticText* m_lblRep;

	// Eventi interni alla finestra
	void OnGara(wxCommandEvent& event);
	void OnEsci(wxCommandEvent& event);
	void OnChiama(wxCommandEvent& event);
	void AggiornaInterfaccia();
};

class DlgFamiglia : public wxDialog {
public:
	DlgFamiglia(wxWindow* parent, TabbyGame& game);
private:
	TabbyGame& m_game;
	wxStaticText* m_lblSoldi;
	wxStaticText* m_lblPaghetta;

	// Eventi interni alla finestra
	void OnAumentoPaghetta(wxCommandEvent& event);
	void OnSoldiExtra(wxCommandEvent& event);
	void OnChiediSoldi(wxCommandEvent& event);
	void AggiornaInterfaccia();
};

class DlgLavoro : public wxDialog {
public:
	DlgLavoro(wxWindow* parent, TabbyGame& game);
private:
	TabbyGame& m_game;
	wxStaticText* m_lblDitta;
	wxStaticText* m_lblImpegno;
	wxStaticText* m_lblSoldi;
	wxStaticText* m_lblStipendio;

	// Eventi interni alla finestra
	void OnCercaLavoro(wxCommandEvent& event);
	void OnLicenziati(wxCommandEvent& event);
	void OnInformazioni(wxCommandEvent& event);
	void OnLavora(wxCommandEvent& event);
	void OnLeccaculo(wxCommandEvent& event);
	void OnAumento(wxCommandEvent& event);
	void OnSciopera(wxCommandEvent& event);
	void AggiornaInterfaccia();
};

class DlgOffertaLavoro : public wxDialog {
public:
	DlgOffertaLavoro(wxWindow* parent, TabbyGame& game);

private:
	void OnAccetta(wxCommandEvent& event);
	void OnRifiuta(wxCommandEvent& event);

	TabbyGame& m_game;
	std::string m_ditta;

};

class DlgQuiz : public wxDialog {
public:
	DlgQuiz(wxWindow* parent, TabbyGame& game, const QuizScheda& quiz, std::string ditta);
private:
	TabbyGame& m_game;
	std::string m_ditta;
	// Memorizziamo le checkbox divisa per domande (3 gruppi da 3)
	// m_checkboxes[0] contiene le 3 opzioni della domanda A
	std::vector<std::vector<wxCheckBox*>> m_gruppiRisposte;
	// Funzione di supporto per creare il blocco "Domanda + 3 Risposte"
	void AggiungiDomanda(wxWindow* parent, wxSizer* sizer, const QuizDomanda& domanda, int indiceDomanda);

	// Invia i dati del gioco al motore di gioco per processarli
	void OnFinito(wxCommandEvent& event);
};

class DlgElencoDitte : public wxDialog {
public:
	DlgElencoDitte(wxWindow* parent, TabbyGame& game);

private:
	TabbyGame& m_game;
	wxListCtrl* m_lista; // Il controllo lista

	// Evento doppio clic sulla lista
	void OnDittaSelezionata(wxListEvent& event);
};

class DlgInfoDitta : public wxDialog {
public:
	// Prende la ditta specifica come parametro const reference
	DlgInfoDitta(wxWindow* parent, TabbyGame& game, const Ditta& ditta);
};


class DlgDisco : public wxDialog {
public:
	DlgDisco(wxWindow* parent, TabbyGame& game);

private:
	TabbyGame& m_game;
	int m_selectedIndex; // Indice della discoteca selezionata (-1 se nessuna)

	// GUI Elements
	std::vector<wxRadioButton*> m_radios;
	wxStaticText* m_lblDescrizione;
	wxStaticText* m_lblPrezzo; // Opzionale, per mostrare info extra
	wxStaticText* m_lblSoldiAttuali;

	// Eventi
	void OnRadioSelect(wxCommandEvent& event);
	void OnOk(wxCommandEvent& event);
};

class DlgEvento : public wxDialog {
public:
	DlgEvento(wxWindow* parent, Messaggio& msg);
};

class DlgTipa : public wxDialog {
public:
	DlgTipa(wxWindow* parent, TabbyGame& game);
private:
	TabbyGame& m_game;
	wxStaticText* m_lblRapportiTipa;
	wxStaticText* m_lblNome;
	wxStaticText* m_lblFamaTipa;
	wxStaticText* m_lblFama;

	void OnCercaTipa(wxCommandEvent& event);
	void OnLasciaTipa(wxCommandEvent& event);
	void OnTelefonaTipa(wxCommandEvent& event);
	void OnEsciTipa(wxCommandEvent& event);
	void AggiornaInterfaccia();
};

class DlgIncontroTipa : public wxDialog {
public:
	DlgIncontroTipa(wxWindow* parent, TabbyGame& game);
private:
	TabbyGame& m_game;
	Tipa m_tipa;
	void OnProvaci(wxCommandEvent& event);
	void OnLasciaStare(wxCommandEvent& event);
};

class DlgElencoNegozi : public wxDialog {
public:
	DlgElencoNegozi(wxWindow* parent, TabbyGame& game);
private:
	TabbyGame& m_game;
	wxStaticText* m_lblSoldi;
	void AggiornaInterfaccia();
	void OnPalestra(wxCommandEvent& event);
	void OnTelefonino(wxCommandEvent& event);
};

class DlgNegozio : public wxDialog {
public:
	DlgNegozio(wxWindow* parent, TabbyGame& game, const Negozio& negozio);
private:
	TabbyGame& m_game;
	const Negozio& m_negozio;
	wxStaticText* m_lblSoldi;

	void AggiornaInterfaccia();
};

// Questa classe rappresenta la singola "Casella" o "Scheda" del prodotto
class PnlProdotto : public wxPanel {
public:
	PnlProdotto(wxWindow* parent, DlgNegozio* mainDlg, TabbyGame& game, const Acquistabile& prod);

private:
	TabbyGame& m_game;
	const Acquistabile& m_prodotto;
	DlgNegozio* m_parentDlg; // Riferimento alla finestra padre per aggiornare i soldi

	void OnCompra(wxCommandEvent& event);
};

// PALESTRA
class DlgPalestra : public wxDialog {
public:
	DlgPalestra(wxWindow* parent, TabbyGame& game);

private:
	TabbyGame& m_game;
	wxStaticText* m_lblScadenza;
	wxStaticText* m_lblPelle;
	wxStaticText* m_lblSoldi;
	wxStaticText* m_lblFama;
	void AggiornaInterfaccia();
	void OnVaiPalestra(wxCommandEvent& event);
	void OnLampada(wxCommandEvent& event);
};

class DlgTelefono : public wxDialog {
public:
	DlgTelefono(wxWindow* parent, TabbyGame& game);

private:
	TabbyGame& m_game;
	wxStaticText* m_lblNomeTel;
	wxStaticText* m_lblOperatore;
	wxStaticText* m_lblCredito;
	wxStaticText* m_lblSoldi;

	void AggiornaInterfaccia();
	void OnCompraTel(wxCommandEvent& event);
	void OnVendiTel(wxCommandEvent& event);
	void OnRicarica(wxCommandEvent& event);
};

// Funzione helper
void ManifestaEventi(wxWindow* parent, TabbyGame& game);