#pragma once
#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/listctrl.h>
#include <wx/scrolwin.h> // Necessario per lo scroll
#include <wx/gbsizer.h>
#include <wx/dcmemory.h> // Serve per disegnare in memoria
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/filename.h>
#include "TabbyGame.h"
#define WIN_BKG wxColor(194, 196, 184)

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

class DlgPersonalInfo : public wxDialog {
public:
	DlgPersonalInfo(wxWindow* parent, TabbyGame& game);

private:
	TabbyGame& m_game;

	// Puntatori ai controlli modificabili
	wxTextCtrl* m_txtNome;
	wxTextCtrl* m_txtCognome;
	wxTextCtrl* m_txtLuogoNascita;
	wxTextCtrl* m_txtProvincia; // <--- NUOVO: Provincia
	wxTextCtrl* m_txtResidenza;
	wxTextCtrl* m_txtIndirizzo;

	void OnOk(wxCommandEvent& event);

	// Helpers
	void AddEditRow(wxGridBagSizer* sizer, int& row, wxString label, wxTextCtrl*& ctrlPtr, wxString value);
	void AddStaticRow(wxGridBagSizer* sizer, int& row, wxString label, wxString value);
	void AddSeparator(wxGridBagSizer* sizer, int& row);

	// NUOVO HELPER: Per mettere Città e Provincia sulla stessa riga
	void AddCityProvRow(wxGridBagSizer* sizer, int& row, wxString label,
		wxTextCtrl*& ctrlCitta, wxString valCitta,
		wxTextCtrl*& ctrlProv, wxString valProv);
};

class DlgConfig : public wxDialog {
public:
	DlgConfig(wxWindow* parent, TabbyGame& game);

private:
	TabbyGame& m_game;

	// Skill Level
	std::vector<wxRadioButton*> m_radiosSkill;

	// Misc Options
	wxCheckBox* m_chkTimer;
	wxCheckBox* m_chkEuro;
	wxCheckBox* m_chkLogo;
	wxCheckBox* m_chkLog;
	wxCheckBox* m_chkSuoni;

	void OnOk(wxCommandEvent& event);
};

// --- FINESTRA DI USCITA (Fine della sessione) ---
class DlgUscita : public wxDialog {
public:
	DlgUscita(wxWindow* parent);
	// Ritorna true se l'utente ha scelto di spegnere il PC (uscire dal gioco)
	// Ritorna false se ha annullato
	bool VuoleUscire() const { return m_sceltaUscita; }

private:
	bool m_sceltaUscita; // true = esce, false = annulla
	wxRadioButton* m_radioChiudi;
	wxRadioButton* m_radioSpegni;

	void OnOk(wxCommandEvent& event);
	void OnCancel(wxCommandEvent& event);
	void OnHelp(wxCommandEvent& event);
};

// --- FINESTRA ABOUT ---
class DlgAbout : public wxDialog {
public:
	DlgAbout(wxWindow* parent);

private:
	void OnOk(wxCommandEvent& event);
	void OnNorme(wxCommandEvent& event);
};

class DlgDueDonne : public wxDialog {
public:
	DlgDueDonne(wxWindow* parent, TabbyGame& game);

private:
	TabbyGame& m_game;
	void OnEntrambe(wxCommandEvent& event);
	void OnResto(wxCommandEvent& event);
	void OnPreferisco(wxCommandEvent& event);
};

class DlgPagella : public wxDialog {
public:
	DlgPagella(wxWindow* parent, TabbyGame& game);
private:
	TabbyGame& m_game;
	void OnOk(wxCommandEvent& event);
};

struct DlgMetallaro : public wxDialog {
	DlgMetallaro(wxWindow* parent, TabbyGame& game, const Messaggio& msg);
};

struct DlgManovale : public wxDialog {
public:
	DlgManovale(wxWindow* parent, TabbyGame& game, const Messaggio& msg);
};

struct DlgIncidente : public wxDialog {
public:
	DlgIncidente(wxWindow* parent, TabbyGame& game, const Messaggio& msg);
};

// Funzioni helper
void ManifestaEventi(wxWindow* parent, TabbyGame& game);
wxBitmap CaricaAsset(const std::string& nomeFile);
wxBitmap GeneraAvatar(const TabbyGuy& guy);