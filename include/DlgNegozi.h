#pragma once
#include "DlgBase.h"

class DlgScooter : public wxDialog {
public:
	DlgScooter(wxWindow* parent, TabbyGame& game);
private:
	TabbyGame& m_game;	// Il riferimento al motore di gioco
	wxStaticBitmap* m_imgScooter;
	wxStaticText* m_lblSoldi;
	wxStaticText* m_lblNome;
	wxStaticText* m_lblVelocita;
	wxStaticText* m_lblBenza;
	wxStaticText* m_lblStato;
	wxStaticText* m_lblCilindrata;
	wxStaticText* m_lblCarburatore;
	wxStaticText* m_lblMarmitta;
	wxStaticText* m_lblFiltro;
	wxButton* m_btnUsa;

	// Eventi interni alla finestra
	void OnConcessionario(wxCommandEvent& event);
	void OnVendi(wxCommandEvent& event);
	void OnTrucca(wxCommandEvent& event);
	void OnRipara(wxCommandEvent& event);
	void OnFaiBenza(wxCommandEvent& event);
	void OnUsa(wxCommandEvent& event);
	// Funzione helper
	wxStaticText* AddStat(wxWindow* parent, wxSizer* sizer, wxString label, wxString value);
	// Funzione per aggiornare i testi dei bottoni
	void AggiornaInterfaccia();
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

class DlgConcessionario : public wxDialog
{
public:
	DlgConcessionario(wxWindow* parent, TabbyGame& game);

private:
	TabbyGame& m_game;
	int m_selectedIndex = -1;
	std::vector<Acquistabile*> m_catalogoPtr;

	// GUI Destra
	wxStaticBitmap* m_imgScooter;
	wxStaticText* m_lblVelocita;
	wxStaticText* m_lblMarmitta;
	wxStaticText* m_lblCarburatore;
	wxStaticText* m_lblCilindrata;
	wxStaticText* m_lblFiltro;
	wxStaticText* m_lblCosto;
	wxStaticText* m_lblSoldi;

	void OnSelezionaScooter(wxCommandEvent& event);
	void OnCompra(wxCommandEvent& event);
	void AggiornaStatistiche();
	wxStaticText* AddStat(wxWindow* parent, wxSizer* sizer, wxString label, wxString value);
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

class DlgRicariche : public wxDialog
{
public:
	DlgRicariche(wxWindow* parent, TabbyGame& game);

private:
	TabbyGame& m_game;
	wxStaticText* m_lblSoldi;
	wxStaticText* m_lblOperatoreAttuale;
	wxStaticText* m_lblCreditoAttuale;

	void AggiornaInterfaccia();
};