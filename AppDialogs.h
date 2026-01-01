#pragma once
#include <wx/wx.h>
#include "TabbyGuy.h"

class DlgScooter : public wxDialog {
public:
	DlgScooter(wxWindow* parent, TabbyGuy* guy);
private:
	TabbyGuy* m_guy;	// Viene conservato il puntatore al tabby per poterci lavorare

	// Eventi interni alla finestra
	void OnConcessionario(wxCommandEvent& event);
	void OnTrucca(wxCommandEvent& event);
	void OnRipara(wxCommandEvent& event);
	void OnFaiBenza(wxCommandEvent& event);

	// Funzione helper
	void AddStat(wxWindow* parent, wxSizer* sizer, wxString label, wxString value);
};