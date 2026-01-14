#pragma once
#include "DlgBase.h"

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