#pragma once
#include <wx/wx.h>
#include "TabbyGame.h"

class DlgScooter : public wxDialog {
public:
	DlgScooter(wxWindow* parent, TabbyGame* game);
private:
	TabbyGame* m_game;	// Il riferimento al motore di gioco

	// Eventi interni alla finestra
	void OnConcessionario(wxCommandEvent& event);
	void OnTrucca(wxCommandEvent& event);
	void OnRipara(wxCommandEvent& event);
	void OnFaiBenza(wxCommandEvent& event);

	// Funzione helper
	void AddStat(wxWindow* parent, wxSizer* sizer, wxString label, wxString value);
};

class DlgScuola : public wxDialog {
public:
	DlgScuola(wxWindow* parent, TabbyGame* game);
private:
	TabbyGame* m_game;	// Il riferimento al motore di gioco
	int m_materiaIndex;

	// Eventi interni alla finestra
	void OnStudia(wxCommandEvent& event);
	void OnMinaccia(wxCommandEvent& event);
	void OnCorrompi(wxCommandEvent& event);

	// Funzione per aggiornare i testi dei bottoni
	void AggiornaBottoni();
};