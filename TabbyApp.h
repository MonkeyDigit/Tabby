#pragma once
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/statline.h>
#include <wx/gbsizer.h>
#include "TabbyGame.h"
#define WIN_BKG wxColor(194, 196, 184)

// ---------- APPLICAZIONE ------------
// il public fa in modo che la classe che stiame creando erediti i membri e le funzioni pubbliche di wxApp
class TabbyApp : public wxApp {
public:
	// il prefisso virtual indica che questa funzione esistente nella classe base, viene ridefinita nella classe figlia
	// override è semplicemente un meccanismo per controllare di aver ridefinito la funzione giusta, in caso contrario il compilatore si incazza
	// questa funzione viene chiamata quando viene creata la finestra
	virtual bool OnInit() override;
};

// ---------- FINESTRA PRINCIPALE (Frame) -----------
class TabbyFrame : public wxFrame {
public:
	// Costruttore
	TabbyFrame();

private:
	// Classe di gestione gioco
	TabbyGame m_game;

	// Label dei dati che variano
	wxStaticText* m_lblNomeTabby;
	wxStaticText* m_lblNomeTipa;
	wxStaticText* m_lblRapportoTipa;
	wxGauge* m_barTipa;
	wxStaticText* m_lblSoldi;
	wxStaticText* m_lblReputazione;
	wxGauge* m_barRep;
	wxStaticText* m_lblFigo;
	wxGauge* m_barFigo;
	wxStaticText* m_lblStudio;
	wxGauge* m_barStudio;
	wxStaticText* m_lblScooter;
	wxGauge* m_barScooter;
	wxStaticText* m_lblDate;

	// Funzioni Helper
	void AggiornaInterfaccia();

	// GESTORE EVENTI (Cosa succede quando clicchi)
	void OnScooter(wxCommandEvent& event);
	void OnNegozi(wxCommandEvent& event);
	void OnDisco(wxCommandEvent& event);
	void OnScuola(wxCommandEvent& event);
	void OnLavoro(wxCommandEvent& event);
	void OnCompagnia(wxCommandEvent& event);
	void OnFamiglia(wxCommandEvent& event);
	void OnTipa(wxCommandEvent& event);
	void OnEsci(wxCommandEvent& event);
};

// In wxWidgets (e in Windows in generale), ogni singolo bottone, menu o casella di testo deve avere un'ID univoco (integer). wxWidgets ha già dei numeri prenotati per le sue cose interne (es. wxID_EXIT, wxID_OK, wxID_SAVE), solitamente sono numeri bassi o negativi, dunque per evitare conflitti si usano numeri alti per essere sicuri di stare fuori da quel range
// Definiamo i nostri ID
enum Widget_ID {
	// wxWidgets suggerisce di iniziare da wxID_HIGHEST per non fare cazzate
	ID_START_EVENT = wxID_HIGHEST + 1,
	// Attività
	ID_SCOOTER, ID_DISCO, ID_SCUOLA, ID_LAVORO, ID_COMPAGNIA, ID_FAMIGLIA, ID_TIPA,
	// Negozi
	ID_NEGOZI, ID_BAU_HOUSE, ID_BLUE_RIDER, ID_ZOCCOLARO, ID_FOOTSMOCKER, ID_FOOTSMOCKER_II, ID_BAR_TABACCHI, ID_PALESTRA, ID_TELEFONINO,
	// Special
	ID_INFO, ID_CONFIG,
	// Help
	ID_ABOUT, ID_LOGO
};