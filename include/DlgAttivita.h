#pragma once
#include <wx/listctrl.h>
#include "DlgBase.h"

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
	DlgQuiz(wxWindow* parent, TabbyGame& game, std::string ditta);
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

// DISCO
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