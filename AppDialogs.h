#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/statline.h>
#include "TabbyGame.h"

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

class DlgEvento : public wxDialog {
public:
	DlgEvento(wxWindow* parent, EventoDati& eventoDati);

private:
	// TODO: Roba
};

// Funzione helper
void ManifestaEventi(wxWindow* parent, TabbyGame& game);