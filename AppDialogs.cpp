#include "AppDialogs.h"

DlgScooter::DlgScooter(wxWindow* parent, TabbyGame* game)
	: wxDialog{ parent, wxID_ANY, "Scooter", wxDefaultPosition, wxDefaultSize },
	m_game{game}
{
	this->SetBackgroundColour(parent->GetBackgroundColour());
	this->SetFont(parent->GetFont());
	Scooter* scootptr = m_game->GetTabbyGuy()->GetScooter();

	// Layout orizzontale principale (Colonna SX | Colonna DX)
	wxBoxSizer* mainSizer = new wxBoxSizer{ wxHORIZONTAL };
	wxBoxSizer* leftCol = new wxBoxSizer{ wxVERTICAL };
	wxBoxSizer* rightCol = new wxBoxSizer{ wxVERTICAL };

	// --- COLONNA SINISTRA --------------------------------------------------------------
	// 1. CONCESSIONARIO
	// Qua si poteva usare un semplice staticboxsizer, ma vogliamo la rientranza (non è supportata dal sizer), quindi bisogna fare prima un panel
	wxPanel* pnlConcessionario = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerConcessionario = new wxBoxSizer{ wxVERTICAL };
	sizerConcessionario->Add(new wxButton(pnlConcessionario, wxID_ANY, "Concessionario", wxDefaultPosition, wxSize(350, 45)), 0, wxALIGN_CENTER | wxALL, 5);
	pnlConcessionario->SetSizer(sizerConcessionario);
	leftCol->Add(pnlConcessionario, 0, wxEXPAND | wxALL, 5);

	// 2. MODIFICHE
	wxPanel* pnlModifiche = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerModifiche = new wxBoxSizer{ wxVERTICAL };
	sizerModifiche->Add(new wxButton(pnlModifiche, wxID_ANY, "Trucca scooter", wxDefaultPosition, wxSize(350, 45)), 0, wxALIGN_CENTER | wxALL, 5);
	sizerModifiche->Add(new wxButton(pnlModifiche, wxID_ANY, "Ripara scooter", wxDefaultPosition, wxSize(350, 45)), 0, wxALIGN_CENTER | wxALL & ~wxTOP, 5);
	pnlModifiche->SetSizer(sizerModifiche);
	leftCol->Add(pnlModifiche, 0, wxEXPAND | wxALL, 5);

	// 3. INFO E OK
	wxPanel* pnlBottom = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };
	sizerBottom->Add(new wxStaticText(pnlBottom, wxID_ANY, "[IMG]"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	sizerBottom->Add(new wxStaticText(pnlBottom, wxID_ANY, "< Soldi "+m_game->GetSoldiStr(m_game->GetTabbyGuy()->GetSoldi()) + " >"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	sizerBottom->Add(new wxButton(pnlBottom, wxID_OK, "OK", wxDefaultPosition, wxSize(60, 50)), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	pnlBottom->SetSizer(sizerBottom);
	leftCol->Add(pnlBottom, 1, wxEXPAND | wxALL, 5);

	// --- COLONNA DESTRA (Statistiche) ----------------------------------------------------------------------------
	wxPanel* pnlStats = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerStats = new wxBoxSizer{ wxVERTICAL };
	sizerStats->Add(new wxStaticText(pnlStats, wxID_ANY, "Statistiche scooter"), 0, wxALIGN_CENTER_HORIZONTAL);
	sizerStats->Add(new wxStaticText(pnlStats, wxID_ANY, "  " + scootptr->GetNome() + "  ", wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN), 0, wxALIGN_CENTER_HORIZONTAL | wxALL & ~wxTOP, 10);
	// GRIGLIA DATI
	// Usiamo FlexGridSizer per allineare "Etichetta" -> "Valore"
	wxFlexGridSizer* gridStats = new wxFlexGridSizer{ 2, 5, 10 }; // 2 Colonne, gap di 5px e 10px

	AddStat(pnlStats, gridStats, "Velocità:", wxString::Format("%d km/h", scootptr->GetVelocita()));
	AddStat(pnlStats, gridStats, "Cilindrata:", wxString::Format("%d cc", scootptr->GetCilindrata()));
	// scrivo %% per escapeare '%'
	AddStat(pnlStats, gridStats, "Efficienza:", wxString::Format("%d %%", scootptr->GetEfficienza()));
	AddStat(pnlStats, gridStats, "Benzina:", wxString::Format("%.2f l", scootptr->GetBenza()));

	sizerStats->Add(gridStats, 0, wxALIGN_CENTER | wxALL & ~wxTOP, 5);

	// BOTTONI IN BASSO A DESTRA
	sizerStats->AddStretchSpacer();	// Spinge i bottoni in fondo
	sizerStats->Add(new wxButton(pnlStats, wxID_ANY, "Usa scooter", wxDefaultPosition, wxSize(350, 45)), 0, wxALIGN_CENTER_HORIZONTAL | wxALL & ~wxBOTTOM, 5);
	sizerStats->Add(new wxButton(pnlStats, wxID_ANY, "Fai benza", wxDefaultPosition, wxSize(350, 45)), 0, wxALIGN_CENTER_HORIZONTAL | wxALL & ~wxTOP, 5);

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

// SCUOLA
DlgScuola::DlgScuola(wxWindow* parent, TabbyGame* game)
	: wxDialog{ parent, wxID_ANY, "Scuola", wxDefaultPosition, wxDefaultSize },
	m_game{ game }, m_materiaIndex{0}
{
	this->SetBackgroundColour(parent->GetBackgroundColour());
	this->SetFont(parent->GetFont());

	Scuola* scuola = m_game->GetTabbyGuy()->GetScuola();

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };
	wxBoxSizer* sizerTop = new wxBoxSizer{ wxHORIZONTAL };
	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };

	// Pannello dei voti
	wxPanel* pnlVoti = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxFlexGridSizer* gridVoti = new wxFlexGridSizer{ 2, 10, 5 };

	// Questi aggiungono un po' di padding per evitare che la prima materia sia attaccata al soffitto
	gridVoti->Add(0, 0);
	gridVoti->Add(0, 0);

	// TODO: METTI ROBA DEI VOTI
	for (int i = 0; i < scuola->m_materie.size(); i++)
	{
		// Il Radio Button, Eh Eh :)
		// Solo il primo (i == 0) deve avere lo stile wxRB_GROUP
		// Questo stabilisce il gruppo di radio button, e impone che solo uno è selezionabile alla volta
		long style = (i == 0) ? wxRB_GROUP : 0;
		wxRadioButton* radio = new wxRadioButton{ pnlVoti, wxID_ANY, scuola->m_materie[i].GetNome(), wxDefaultPosition, wxDefaultSize, style };

		// Il primo viene selezionato di default
		if (i == 0) radio->SetValue(true);

		// Il voto
		wxString votoStr = wxString::Format("%d", scuola->m_materie[i].GetVoto());
		wxStaticText* lblVoto = new wxStaticText{ pnlVoti, wxID_ANY, votoStr, wxDefaultPosition, wxSize(45, 25), wxBORDER_SUNKEN | wxALIGN_CENTER_HORIZONTAL};

		// Aggiungiamo alla griglia
		gridVoti->Add(radio, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
		gridVoti->Add(lblVoto, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

		// Binding
		radio->Bind(wxEVT_RADIOBUTTON, [this, i](wxCommandEvent& ev) {
			m_materiaIndex = i;
			AggiornaBottoni();
			});
	}


	pnlVoti->SetSizer(gridVoti);
	sizerTop->Add(pnlVoti, 0, wxEXPAND | wxALL, 5);

	// Pannello bottoni
	wxPanel* pnlButtons = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxSize(600,400), wxBORDER_SUNKEN};
	wxBoxSizer* sizerButtons = new wxBoxSizer{ wxVERTICAL };
	sizerTop->Add(pnlButtons, 0, wxALL, 5);
	mainSizer->Add(sizerTop, 0, wxALL, 5);

	wxPanel* pnlInfoOk = new wxPanel{ this,wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerInfoOk = new wxBoxSizer{ wxHORIZONTAL };

	// TODO: METTI ROBA

	sizerBottom->Add(pnlInfoOk, 0, wxALL, 5);
	mainSizer->Add(sizerBottom, 0, wxALL, 5);

	this->SetSizer(mainSizer);
	this->Fit();
}

void DlgScuola::OnStudia(wxCommandEvent& event)
{
}

void DlgScuola::OnMinaccia(wxCommandEvent& event)
{
}

void DlgScuola::OnCorrompi(wxCommandEvent& event)
{
}

void DlgScuola::AggiornaBottoni()
{
}
