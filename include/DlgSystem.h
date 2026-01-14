#pragma once
#include "DlgBase.h"

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

// --- FINESTRA ABOUT ---
class DlgAbout : public wxDialog {
public:
	DlgAbout(wxWindow* parent);

private:
	void OnOk(wxCommandEvent& event);
	void OnNorme(wxCommandEvent& event);
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
	wxTextCtrl* m_txtProvincia;
	wxTextCtrl* m_txtResidenza;
	wxTextCtrl* m_txtIndirizzo;

	void OnOk(wxCommandEvent& event);

	// Helpers
	void AddEditRow(wxGridBagSizer* sizer, int& row, wxString label, wxTextCtrl*& ctrlPtr, wxString value);
	void AddStaticRow(wxGridBagSizer* sizer, int& row, wxString label, wxString value);
	void AddSeparator(wxGridBagSizer* sizer, int& row);

	// Per mettere Città e Provincia sulla stessa riga
	void AddCityProvRow(wxGridBagSizer* sizer, int& row, wxString label,
		wxTextCtrl*& ctrlCitta, wxString valCitta,
		wxTextCtrl*& ctrlProv, wxString valProv);
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