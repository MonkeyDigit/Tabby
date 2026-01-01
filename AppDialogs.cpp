#include "AppDialogs.h"

DlgScooter::DlgScooter(wxWindow* parent, TabbyGuy* guy)
	: wxDialog{ parent, wxID_ANY, "Scooter", wxDefaultPosition, wxDefaultSize },
	m_guy{guy}
{
	Scooter* scootptr = guy->GetScooter();
	this->SetBackgroundColour(parent->GetBackgroundColour());
	this->SetFont(parent->GetFont());

	// Layout orizzontale principale (Colonna SX | Colonna DX)
	wxBoxSizer* mainSizer = new wxBoxSizer{ wxHORIZONTAL };
	wxBoxSizer* leftCol = new wxBoxSizer{ wxVERTICAL };
	wxBoxSizer* rightCol = new wxBoxSizer{ wxVERTICAL };
	// Qua si poteva usare un semplice staticboxsizer, ma vogliamo la rientranza (non è supportata dal sizer), quindi bisogna fare prima un panel
	wxPanel* pnlConcessionario = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerConcessionario = new wxBoxSizer{ wxVERTICAL };
	wxPanel* pnlModifiche = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerModifiche = new wxBoxSizer{ wxVERTICAL };
	wxPanel* pnlBottom = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };

	wxPanel* pnlStats = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerStats = new wxBoxSizer{ wxVERTICAL };

	// --- COLONNA SINISTRA --------------------------------------------------------------
	// 1. CONCESSIONARIO
	sizerConcessionario->Add(new wxButton(pnlConcessionario, wxID_ANY, "Concessionario", wxDefaultPosition, wxSize(300, 50)), 0, wxEXPAND | wxALL, 5);
	pnlConcessionario->SetSizer(sizerConcessionario);
	leftCol->Add(pnlConcessionario, 0, wxEXPAND | wxALL, 5);

	// 2. MODIFICHE
	sizerModifiche->Add(new wxButton(pnlModifiche, wxID_ANY, "Trucca scooter", wxDefaultPosition, wxSize(300, 45)), 0, wxALL | wxEXPAND, 10);
	sizerModifiche->Add(new wxButton(pnlModifiche, wxID_ANY, "Ripara scooter", wxDefaultPosition, wxSize(300, 45)), 0, wxEXPAND | wxALL & ~wxTOP, 10);
	pnlModifiche->SetSizer(sizerModifiche);
	leftCol->Add(pnlModifiche, 0, wxEXPAND | wxALL, 5);

	// 3. INFO E OK
	sizerBottom->Add(new wxStaticText(pnlBottom, wxID_ANY, "[IMG]"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	sizerBottom->Add(new wxStaticText(pnlBottom, wxID_ANY, wxString::Format("< Soldi  %d€ >", guy->GetSoldi())), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	sizerBottom->Add(new wxButton(pnlBottom, wxID_OK, "OK", wxDefaultPosition, wxSize(60, 50)), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	pnlBottom->SetSizer(sizerBottom);
	leftCol->Add(pnlBottom, 1, wxEXPAND | wxALL, 5);

	// --- COLONNA DESTRA (Statistiche) ----------------------------------------------------------------------------
	sizerStats->Add(new wxStaticText(pnlStats, wxID_ANY, "Statistiche scooter"), 0, wxALIGN_CENTER);
	sizerStats->Add(new wxStaticText(pnlStats, wxID_ANY, "  " + scootptr->GetNome() + "  ", wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN | wxST_NO_AUTORESIZE), 0, wxEXPAND | wxALL & ~wxTOP, 10);
	// GRIGLIA DATI
	// Usiamo FlexGridSizer per allineare "Etichetta" -> "Valore"
	wxFlexGridSizer* gridStats = new wxFlexGridSizer{ 2, 5, 10 }; // 2 Colonne, gap di 5px e 10px

	AddStat(pnlStats, gridStats, "Velocità:", wxString::Format("%d km/h", scootptr->GetVelocita()));
	AddStat(pnlStats, gridStats, "Cilindrata:", wxString::Format("%d cc", scootptr->GetCilindrata()));
	// scrivo %% per escapeare '%'
	AddStat(pnlStats, gridStats, "Efficienza:", wxString::Format("%d %%", scootptr->GetEfficienza()));
	AddStat(pnlStats, gridStats, "Benzina:", wxString::Format("%.2f l", scootptr->GetBenza()));

	sizerStats->Add(gridStats, 0, wxEXPAND | wxALL & ~wxTOP, 10);

	// BOTTONI IN BASSO A DESTRA
	sizerStats->AddStretchSpacer();	// Spinge i bottoni in fondo
	sizerStats->Add(new wxButton(pnlStats, wxID_ANY, "Usa scooter", wxDefaultPosition, wxSize(300, 45)), 0, wxEXPAND | wxALL & ~wxBOTTOM, 10);
	sizerStats->Add(new wxButton(pnlStats, wxID_ANY, "Fai benza", wxDefaultPosition, wxSize(300, 45)), 0, wxEXPAND | wxALL & ~wxTOP, 10);

	pnlStats->SetSizer(sizerStats);
	rightCol->Add(pnlStats, 0, wxEXPAND | wxALL, 5);

	// --- CHIUSURA LAYOUT ---
	mainSizer->Add(leftCol, 0, wxEXPAND | wxALL, 5);
	mainSizer->Add(rightCol, 0, wxEXPAND | wxALL, 5);

	this->SetSizer(mainSizer);
	this->Fit();
}

void DlgScooter::OnConcessionario(wxCommandEvent& event)
{
}

void DlgScooter::OnTrucca(wxCommandEvent& event)
{
}

void DlgScooter::OnRipara(wxCommandEvent& event)
{
}

void DlgScooter::OnFaiBenza(wxCommandEvent& event)
{
}

void DlgScooter::AddStat(wxWindow* parent, wxSizer* sizer, wxString label, wxString value)
{
	sizer->Add(new wxStaticText(parent, wxID_ANY, label, wxDefaultPosition, wxSize(-1, 25)));
	wxStaticText* valueBox = new wxStaticText{ parent, wxID_ANY, value, wxDefaultPosition, wxSize(200, 25), wxALIGN_CENTER | wxBORDER_SUNKEN };
	sizer->Add(valueBox, 0, wxALIGN_RIGHT);
}