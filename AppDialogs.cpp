#include "AppDialogs.h"

// Funzione helper
static void ManifestaEventi(wxWindow* parent, TabbyGame& game)
{
	EventoDati evento;

	while (game.PollEvento(evento))
	{
		DlgEvento dlgEvento{ parent, evento };
		// Nel caso di un pop up evento con scelta (previa implementazione degli appositi bottoni con wxID_YES e wxID_NO), gli id vengono restituiti alla finestra padre
		// Qua valutiamo l'espressione logica
		bool scelta = (dlgEvento.ShowModal() == wxID_YES);

		// Se qui ApplicaScelta aggiunge un NUOVO evento, questo finisce in fondo alla coda. Il ciclo while continuerà a girare e lo pescherà subito dopo
		if (evento.tipo == TipoEvento::SCELTA)
			game.ApplicaScelta(evento.idEvento, scelta);
	}
	// Usciti dal while, la coda è sicuramente vuota
}

DlgScooter::DlgScooter(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Scooter", wxDefaultPosition, wxDefaultSize },
	m_game{game}
{
	this->SetBackgroundColour(parent->GetBackgroundColour());
	this->SetFont(parent->GetFont());
	Scooter& scootref = m_game.GetTabbyGuy().GetScooter();

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
	sizerBottom->Add(new wxStaticText(pnlBottom, wxID_ANY, "< Soldi "+m_game.GetSoldiStr(m_game.GetTabbyGuy().GetSoldi()) + " >"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	sizerBottom->Add(new wxButton(pnlBottom, wxID_OK, "OK", wxDefaultPosition, wxSize(60, 50)), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	pnlBottom->SetSizer(sizerBottom);
	leftCol->Add(pnlBottom, 1, wxEXPAND | wxALL, 5);

	// --- COLONNA DESTRA (Statistiche) ----------------------------------------------------------------------------
	wxPanel* pnlStats = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerStats = new wxBoxSizer{ wxVERTICAL };
	sizerStats->Add(new wxStaticText(pnlStats, wxID_ANY, "Statistiche scooter"), 0, wxALIGN_CENTER_HORIZONTAL);
	sizerStats->Add(new wxStaticText(pnlStats, wxID_ANY, "  " + scootref.GetNome() + "  ", wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN), 0, wxALIGN_CENTER_HORIZONTAL | wxALL & ~wxTOP, 10);
	// GRIGLIA DATI
	// Usiamo FlexGridSizer per allineare "Etichetta" -> "Valore"
	wxFlexGridSizer* gridStats = new wxFlexGridSizer{ 2, 5, 10 }; // 2 Colonne, gap di 5px e 10px

	AddStat(pnlStats, gridStats, "Velocità:", wxString::Format("%d km/h", scootref.GetVelocita()));
	AddStat(pnlStats, gridStats, "Cilindrata:", wxString::Format("%d cc", scootref.GetCilindrata()));
	// scrivo %% per escapeare '%'
	AddStat(pnlStats, gridStats, "Efficienza:", wxString::Format("%d %%", scootref.GetEfficienza()));
	AddStat(pnlStats, gridStats, "Benzina:", wxString::Format("%.2f l", scootref.GetBenza()));

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
DlgScuola::DlgScuola(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Scuola", wxDefaultPosition, wxDefaultSize },
	m_game{ game }, m_materiaIndex{0}
{
	this->SetBackgroundColour(parent->GetBackgroundColour());
	this->SetFont(parent->GetFont());

	Scuola& scuolaref = m_game.GetTabbyGuy().GetScuola();

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };
	wxBoxSizer* sizerTop = new wxBoxSizer{ wxHORIZONTAL };

	// Pannello bottoni
	wxPanel* pnlButtons = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerButtons = new wxBoxSizer{ wxVERTICAL };
	wxBoxSizer* sizerRepOk = new wxBoxSizer{ wxHORIZONTAL };

	// Creiamo i bottoni, inizialmente vuoti, per poi aggiornarli come si deve in AggiornaInterfaccia...
	// Siccome i bottoni sono già membri della classe DlgScuola, va bene wxID_ANY, perché possediamo già i loro puntatori
	m_btnStudia = new wxButton{ pnlButtons, wxID_ANY, "---", wxDefaultPosition, wxSize(530, 30)};
	m_btnMinaccia = new wxButton{ pnlButtons, wxID_ANY, "---", wxDefaultPosition, wxSize(530, 30) };
	m_btnCorrompi = new wxButton{ pnlButtons, wxID_ANY, "---", wxDefaultPosition, wxSize(530, 30) };
	m_btnStudia->Bind(wxEVT_BUTTON, &DlgScuola::OnStudia, this);
	m_btnMinaccia->Bind(wxEVT_BUTTON, &DlgScuola::OnMinaccia, this);
	m_btnCorrompi->Bind(wxEVT_BUTTON, &DlgScuola::OnCorrompi, this);

	// Pannello dei voti
	wxPanel* pnlVoti = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxFlexGridSizer* gridVoti = new wxFlexGridSizer{ 2, 10, 5 };

	for (int i = 0; i < scuolaref.m_materie.size(); i++)
	{
		// Il Radio Button, Eh Eh :)
		// Solo il primo (i == 0) deve avere lo stile wxRB_GROUP
		// Questo stabilisce il gruppo di radio button, e impone che solo uno è selezionabile alla volta
		long style = (i == 0) ? wxRB_GROUP : 0;
		wxRadioButton* radio = new wxRadioButton{ pnlVoti, wxID_ANY, scuolaref.m_materie[i].GetNome(), wxDefaultPosition, wxDefaultSize, style };

		// Il primo viene selezionato di default
		if (i == 0) radio->SetValue(true);

		// Il label del voto viene inizializzato vuoto
		m_lblVoti.push_back(new wxStaticText(pnlVoti, wxID_ANY, "---", wxDefaultPosition, wxSize(45, 25), wxBORDER_SUNKEN | wxALIGN_CENTER_HORIZONTAL));

		// Aggiungiamo alla griglia
		gridVoti->Add(radio, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
		gridVoti->Add(m_lblVoti[i], 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

		// Binding
		// Sfrutto una funzione lambda per associare la materia che viene selezionata ai bottoni del pannello a destra, perchè sono troppo intelligente...
		radio->Bind(wxEVT_RADIOBUTTON, [this, i](wxCommandEvent& ev) {
			m_materiaIndex = i;
			AggiornaInterfaccia();
			});
	}

	// Creo un sizer "invisibile" che serve a dare MARGINE (altrimenti metallurgia sprofonda)
	wxBoxSizer* wrapperSizer = new wxBoxSizer{ wxVERTICAL };
	//Inserisco la griglia dentro questo wrapper con 5px di respiro ovunque(wxALL)
	// Questo spingerà "Agraria" giù dal soffitto e "Metallurgia" su dal pavimento
	wrapperSizer->Add(gridVoti, 1, wxEXPAND | wxALL, 5);
	// Profittto scolastico
	m_lblStudio = new wxStaticText{ pnlVoti, wxID_ANY, "---", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_VERTICAL };
	wrapperSizer->Add(new wxStaticLine(pnlVoti, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), 0, wxEXPAND | wxALL, 5);
	wrapperSizer->Add(m_lblStudio, 0, wxALL, 5);
	// Settiamo il sizer
	pnlVoti->SetSizer(wrapperSizer);
	sizerTop->Add(pnlVoti, 0, wxEXPAND | wxALL, 5);

	// Bottoni del pannello a destra
	sizerButtons->AddStretchSpacer();
	sizerButtons->Add(m_btnStudia, 0, wxALIGN_CENTER_HORIZONTAL | wxRIGHT | wxLEFT, 5);
	sizerButtons->Add(m_btnMinaccia, 0, wxALIGN_CENTER_HORIZONTAL | wxRIGHT | wxLEFT, 5);
	sizerButtons->Add(m_btnCorrompi, 0, wxALIGN_CENTER_HORIZONTAL | wxRIGHT | wxLEFT | wxBOTTOM, 5);
	// Settiamo il sizer
	pnlButtons->SetSizer(sizerButtons);
	sizerTop->Add(pnlButtons, 0, wxEXPAND | wxALL, 5);
	mainSizer->Add(sizerTop, 0);

	// Pannello reputazione e tasto ok
	wxPanel* pnlRepOk = new wxPanel{ this,wxID_ANY, wxDefaultPosition, wxSize(-1, 50), wxBORDER_SUNKEN};

	m_lblRep = new wxStaticText{ pnlRepOk, wxID_ANY, "---" };
	sizerRepOk->Add(m_lblRep, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	wxButton* btnOk = new wxButton{ pnlRepOk, wxID_OK, "OK", wxDefaultPosition, wxSize(60, 50) };
	sizerRepOk->AddStretchSpacer();
	sizerRepOk->Add(btnOk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	pnlRepOk->SetSizer(sizerRepOk);

	mainSizer->Add(pnlRepOk, 0, wxEXPAND | wxALL & ~wxTOP, 5);

	this->SetSizer(mainSizer);
	this->Fit();
	this->AggiornaInterfaccia();
}

void DlgScuola::OnStudia(wxCommandEvent& event)
{
	m_game.AzioneStudia(m_materiaIndex);
	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
	
}

void DlgScuola::OnMinaccia(wxCommandEvent& event)
{
	m_game.AzioneMinaccia(m_materiaIndex);
	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
}

void DlgScuola::OnCorrompi(wxCommandEvent& event)
{
	m_game.AzioneCorrompi(m_materiaIndex);
	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
}

void DlgScuola::AggiornaInterfaccia()
{
	Scuola& scuolaref = m_game.GetTabbyGuy().GetScuola();
	m_btnStudia->SetLabel("Studia " + scuolaref.m_materie[m_materiaIndex].GetNome());
	m_btnMinaccia->SetLabel("Minaccia il prof di " + scuolaref.m_materie[m_materiaIndex].GetNome());
	m_btnCorrompi->SetLabel("Corrompi il prof di " + scuolaref.m_materie[m_materiaIndex].GetNome());

	for (int i = 0; i < scuolaref.m_materie.size(); i++)
		m_lblVoti[i]->SetLabel(wxString::Format("%d", scuolaref.m_materie[i].GetVoto()));

	m_lblStudio->SetLabel(wxString::Format("Profitto scolastico %d/100", m_game.GetTabbyGuy().GetStudio()));
	m_lblRep->SetLabel(wxString::Format("Reputazione %d/100", m_game.GetTabbyGuy().GetRep()));

	this->Layout();
}

DlgEvento::DlgEvento(wxWindow* parent, EventoDati& eventoDati)
	: wxDialog{ parent, wxID_ANY, eventoDati.titolo, wxDefaultPosition, wxDefaultSize, wxCAPTION | wxSTAY_ON_TOP }	// Stile: CAPTION (barra titolo) ma niente tasto X (CLOSE_BOX) così l'utente è obbligato a premere i bottoni
{
	this->CenterOnParent();	// Appare al centro della finestra padre
	EventoDati& evref = eventoDati;
	// TODO: implementa immagine

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	wxStaticText* lblTesto = new wxStaticText{ this, wxID_ANY, evref.testo, wxDefaultPosition, wxSize(300, -1), wxALIGN_CENTER | wxST_NO_AUTORESIZE };

	// Manda a capo il testo automaticamente
	lblTesto->Wrap(300);
	mainSizer->Add(lblTesto, 1, wxALIGN_CENTER | wxALL, 15);

	// Linea separatrice
	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

	// BOTTONI
	wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

	if (evref.tipo == TipoEvento::INFO)
	{
		// Caso semplice: Solo OK
		wxButton* btnOk = new wxButton(this, wxID_OK, "OK, Ho capito");
		btnSizer->Add(btnOk, 0, wxALL, 10);
		// wxID_OK chiude automaticamente il dialogo ritornando wxID_OK
	}
	else if (evref.tipo == TipoEvento::SCELTA)
	{
		// Caso scelta: SÌ e NO
		// TODO: accetta e rifiuta
		wxButton* btnSi = new wxButton(this, wxID_YES, "Sì (Fallo)");
		wxButton* btnNo = new wxButton(this, wxID_NO, "No (Lascia stare)");

		btnSizer->Add(btnSi, 0, wxALL, 10);
		btnSizer->Add(btnNo, 0, wxALL, 10);
		// wxID_YES e wxID_NO chiudono automaticamente ritornando il rispettivo ID
		// TODO: Cosa intende sta cosa???


	}

	mainSizer->Add(btnSizer, 0, wxALIGN_CENTER | wxBOTTOM, 5);

	this->SetSizerAndFit(mainSizer);

}