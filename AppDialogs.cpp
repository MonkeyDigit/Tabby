#include "AppDialogs.h"

void ManifestaEventi(wxWindow* parent, TabbyGame& game)
{
	Messaggio msg;

	while (game.PollMessaggi(msg))
	{
		DlgEvento dlgEvento{ parent, msg };
		// Nel caso di un pop up evento con scelta (previa implementazione degli appositi bottoni con wxID_YES e wxID_NO), gli id vengono restituiti alla finestra padre
		// Qua valutiamo l'espressione logica
		bool scelta = (dlgEvento.ShowModal() == wxID_YES);

		// Se qui ApplicaScelta aggiunge un NUOVO evento, questo finisce in fondo alla coda. Il ciclo while continuerà a girare e lo pescherà subito dopo
		if (msg.m_tipo == TipoMsg::SCELTA)
			game.ApplicaScelta(msg.m_msgAzione, scelta);
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

	this->SetSizerAndFit(mainSizer);
}

void DlgScooter::OnConcessionario(wxCommandEvent& event)
{
	// TODO: IMPLEMENTA
}

void DlgScooter::OnTrucca(wxCommandEvent& event)
{
	// TODO: IMPLEMENTA
}

void DlgScooter::OnRipara(wxCommandEvent& event)
{
	// TODO: IMPLEMENTA
}

void DlgScooter::OnFaiBenza(wxCommandEvent& event)
{
	// TODO: IMPLEMENTA
}

void DlgScooter::AddStat(wxWindow* parent, wxSizer* sizer, wxString label, wxString value)
{
	sizer->Add(new wxStaticText(parent, wxID_ANY, label, wxDefaultPosition, wxSize(-1, 25)));
	wxStaticText* valueBox = new wxStaticText{ parent, wxID_ANY, value, wxDefaultPosition, wxSize(200, 25), wxALIGN_CENTER | wxBORDER_SUNKEN };
	sizer->Add(valueBox, 0, wxALIGN_RIGHT);
}

void DlgScooter::AggiornaInterfaccia()
{
	// TODO: IMPLEMENTA
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

	// TODO: FOTO

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

	this->SetSizerAndFit(mainSizer);
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

DlgCompagnia::DlgCompagnia(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Compagnia", wxDefaultPosition, wxDefaultSize },
	m_game{ game }
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour());
	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	wxPanel* pnlFoto = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxSize(300, 200), wxBORDER_SUNKEN};
	wxBoxSizer* sizerFoto = new wxBoxSizer{ wxVERTICAL };
	wxPanel* pnlButtons = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerButtons = new wxBoxSizer{ wxVERTICAL };
	wxPanel* pnlInfoOk = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerInfoOk = new wxBoxSizer{ wxHORIZONTAL };

	// TODO: foto
	pnlFoto->SetSizer(sizerFoto);
	mainSizer->Add(pnlFoto, 0, wxEXPAND | wxALL, 5);

	// Bottoni
	wxButton* btnGara = new wxButton{ pnlButtons, wxID_ANY, "Gareggia con lo Scooter", wxDefaultPosition, wxSize(300,-1) };
	wxButton* btnEsci = new wxButton{ pnlButtons, wxID_ANY, "Esci con la Compagnia", wxDefaultPosition, wxSize(300,-1) };
	wxButton* btnChiama = new wxButton{ pnlButtons, wxID_ANY, "Chiama la Compagnia", wxDefaultPosition, wxSize(300,-1) };

	btnGara->Bind(wxEVT_BUTTON, &DlgCompagnia::OnGara, this);
	btnEsci->Bind(wxEVT_BUTTON, &DlgCompagnia::OnEsci, this);
	btnChiama->Bind(wxEVT_BUTTON, &DlgCompagnia::OnChiama, this);

	sizerButtons->Add(btnGara, 0, wxALL, 5);
	sizerButtons->Add(btnEsci, 0, wxALL, 5);
	sizerButtons->Add(btnChiama, 0, wxALL, 5);

	pnlButtons->SetSizer(sizerButtons);
	mainSizer->Add(pnlButtons, 0, wxEXPAND | wxALL, 5);

	// Info e ok
	m_lblRep = new wxStaticText(pnlInfoOk, wxID_ANY, "---");
	wxButton* btnOk = new wxButton{ pnlInfoOk, wxID_OK, "OK", wxDefaultPosition, wxSize(60, 50) };;
	sizerInfoOk->Add(m_lblRep, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	sizerInfoOk->AddStretchSpacer();
	sizerInfoOk->Add(btnOk, 0, wxALL, 5);

	pnlInfoOk->SetSizer(sizerInfoOk);
	mainSizer->Add(pnlInfoOk, 0, wxEXPAND | wxALL, 5);

	this->SetSizerAndFit(mainSizer);
	this->AggiornaInterfaccia();
}

void DlgCompagnia::OnGara(wxCommandEvent& event)
{
	m_game.AzioneGara();
	ManifestaEventi(this, m_game);
	this->EndModal(wxID_ANY);
}

void DlgCompagnia::OnEsci(wxCommandEvent& event)
{
	m_game.AzioneEsci();
	ManifestaEventi(this, m_game);
	this->EndModal(wxID_ANY);

}

void DlgCompagnia::OnChiama(wxCommandEvent& event)
{
	m_game.AzioneChiama();
	ManifestaEventi(this, m_game);
	this->EndModal(wxID_ANY);
}

void DlgCompagnia::AggiornaInterfaccia()
{
	m_lblRep->SetLabel(wxString::Format("Reputazione %d/100", m_game.GetTabbyGuy().GetRep()));
	this->Layout();
}


// TODO: ICONA INFO
DlgEvento::DlgEvento(wxWindow* parent, Messaggio& eventoDati)
	: wxDialog{ parent, wxID_ANY, eventoDati.m_titolo, wxDefaultPosition, wxDefaultSize, wxCAPTION | wxSTAY_ON_TOP }	// Stile: CAPTION (barra titolo) ma niente tasto X (CLOSE_BOX) così l'utente è obbligato a premere i bottoni
{
	this->SetFont(parent->GetFont());
	this->CenterOnParent();	// Appare al centro della finestra padre
	Messaggio& msgref = eventoDati;
	// TODO: implementa immagine

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	wxStaticText* lblTesto = new wxStaticText{ this, wxID_ANY, msgref.m_testo, wxDefaultPosition, wxSize(-1, -1), wxALIGN_LEFT };

	// Manda a capo il testo automaticamente
	lblTesto->Wrap(600);
	mainSizer->Add(lblTesto, 1, wxALIGN_CENTER | wxALL, 15);

	// Linea separatrice
	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

	// BOTTONI
	wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

	if (msgref.m_tipo == TipoMsg::INFO)
	{
		// Caso semplice: Solo OK
		// wxID_OK chiude automaticamente il dialogo ritornando wxID_OK
		wxButton* btnOk = new wxButton(this, wxID_OK, "OK");
		btnSizer->Add(btnOk, 0, wxALL, 10);
	}
	else if (msgref.m_tipo == TipoMsg::SCELTA)
	{
		wxButton* btnSi = new wxButton(this, wxID_YES, "Sì");
		wxButton* btnNo = new wxButton(this, wxID_NO, "No");

		// Diciamo esplicitamente: "Quando clicchi, chiudi la finestra e ritorna questo ID"
		btnSi->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
			EndModal(wxID_YES);
			});

		btnNo->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
			EndModal(wxID_NO);
			});

		// wxID_YES e wxID_NO chiudono automaticamente ritornando il rispettivo ID
		btnSizer->Add(btnSi, 0, wxALL, 10);
		btnSizer->Add(btnNo, 0, wxALL, 10);
	}

	mainSizer->Add(btnSizer, 0, wxALIGN_CENTER | wxBOTTOM, 5);
	this->SetSizerAndFit(mainSizer);
}

DlgFamiglia::DlgFamiglia(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Famiglia", wxDefaultPosition, wxDefaultSize },
	m_game{ game }
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour());
	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	wxPanel* pnlButtons = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerButtons = new wxBoxSizer{ wxVERTICAL };
	wxPanel* pnlInfoOk = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerInfoOk = new wxBoxSizer{ wxHORIZONTAL };

	// Bottoni
	wxButton* btnAumento = new wxButton{ pnlButtons, wxID_ANY, "Chiedi aumento paghetta", wxDefaultPosition, wxSize(600,-1) };
	wxButton* btnSoldiExtra = new wxButton{ pnlButtons, wxID_ANY, "Chiedi soldi extra", wxDefaultPosition, wxSize(600,-1) };
	wxButton* btnChiediSoldi = new wxButton{ pnlButtons, wxID_ANY, "Papà, mi dai " + m_game.GetSoldiStr(50) + " ?", wxDefaultPosition, wxSize(600,-1) };

	btnAumento->Bind(wxEVT_BUTTON, &DlgFamiglia::OnAumentoPaghetta, this);
	btnSoldiExtra->Bind(wxEVT_BUTTON, &DlgFamiglia::OnSoldiExtra, this);
	btnChiediSoldi->Bind(wxEVT_BUTTON, &DlgFamiglia::OnChiediSoldi, this);

	sizerButtons->Add(btnAumento, 0, wxTOP | wxRIGHT | wxLEFT, 5);
	sizerButtons->Add(btnSoldiExtra, 0, wxRIGHT | wxLEFT, 5);
	sizerButtons->Add(btnChiediSoldi, 0, wxRIGHT | wxLEFT | wxBOTTOM, 5);

	pnlButtons->SetSizer(sizerButtons);
	mainSizer->Add(pnlButtons, 0, wxEXPAND | wxALL, 5);

	// Info e ok
	m_lblSoldi = new wxStaticText(pnlInfoOk, wxID_ANY, "---");
	m_lblPaghetta = new wxStaticText(pnlInfoOk, wxID_ANY, "---");
	wxButton* btnOk = new wxButton{ pnlInfoOk, wxID_OK, "OK", wxDefaultPosition, wxSize(60, 50) };

	// Creiamo un sizer VERTICALE solo per le scritte
	wxBoxSizer* sizerTesti = new wxBoxSizer{ wxVERTICAL };
	sizerTesti->Add(m_lblSoldi, 0, wxBOTTOM, 2);
	sizerTesti->Add(m_lblPaghetta, 0, wxTOP, 2);

	// Ora aggiungiamo al sizer orizzontale principale del pannello
	// 1. I testi (con proportion=1 così spingono l'OK a destra se serve)
	sizerInfoOk->Add(sizerTesti, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);

	// 2. Il bottone OK (fisso a destra)
	sizerInfoOk->Add(btnOk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	pnlInfoOk->SetSizer(sizerInfoOk);
	mainSizer->Add(pnlInfoOk, 0, wxEXPAND | wxALL, 5);

	this->SetSizerAndFit(mainSizer);
	this->AggiornaInterfaccia();
}

void DlgFamiglia::OnAumentoPaghetta(wxCommandEvent& event)
{
	m_game.AzioneAumentoPaghetta();
	ManifestaEventi(this, m_game);
	this->EndModal(wxID_ANY);
}

void DlgFamiglia::OnSoldiExtra(wxCommandEvent& event)
{
	m_game.AzioneSoldiExtra();
	ManifestaEventi(this, m_game);
	this->EndModal(wxID_ANY);
}

void DlgFamiglia::OnChiediSoldi(wxCommandEvent& event)
{
	m_game.AzioneChiediSoldi();
	ManifestaEventi(this, m_game);
	this->EndModal(wxID_ANY);
}

void DlgFamiglia::AggiornaInterfaccia()
{
	m_lblSoldi->SetLabel("< Soldi " + m_game.GetSoldiStr(m_game.GetTabbyGuy().GetSoldi()) + " >");
	m_lblPaghetta->SetLabel("< Paghetta " + m_game.GetSoldiStr(m_game.GetTabbyGuy().GetPaghetta()) + " >");
	this->Layout();
}

DlgLavoro::DlgLavoro(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Lavoro", wxDefaultPosition, wxDefaultSize },
	m_game{ game }
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour());
	// TODO: Immagine e icona sotto
	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };
	wxPanel* pnlBody	= new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN};
	wxBoxSizer* sizerBody = new wxBoxSizer{ wxHORIZONTAL };
	wxPanel* pnlFoto	= new wxPanel{ pnlBody, wxID_ANY, wxDefaultPosition, wxSize(300,200), wxBORDER_SUNKEN };
	wxBoxSizer* sizerFoto = new wxBoxSizer{ wxVERTICAL };
	wxPanel* pnlButtons = new wxPanel{ pnlBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerButtons = new wxBoxSizer{ wxVERTICAL };
	wxPanel* pnlBottom = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };
	wxPanel* pnlInfo	= new wxPanel{ pnlBottom, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN};
	wxFlexGridSizer* sizerInfo = new wxFlexGridSizer{ 2, 5, 10 };

	// Foto
	pnlFoto->SetSizer(sizerFoto);
	sizerBody->Add(pnlFoto, 0, wxALL, 5);
	sizerBody->AddStretchSpacer();

	// Bottoni
	wxButton* btnCercaLavoro = new wxButton{ pnlButtons, wxID_ANY, "Cerca lavoro", wxDefaultPosition, wxSize(300, 40) };
	wxButton* btnLicenziati = new wxButton{ pnlButtons, wxID_ANY, "Licenziati", wxDefaultPosition, wxSize(300, 40) };
	wxButton* btnInformazioni = new wxButton{ pnlButtons, wxID_ANY, "Elenco Ditte", wxDefaultPosition, wxSize(300, 40) };
	wxButton* btnLavora = new wxButton{ pnlButtons, wxID_ANY, "Lavora", wxDefaultPosition, wxSize(300, 40) };
	wxButton* btnLeccaculo = new wxButton{ pnlButtons, wxID_ANY, "Fai il Leccaculo", wxDefaultPosition, wxSize(300,40) };
	wxButton* btnAumento = new wxButton{ pnlButtons, wxID_ANY, "Chiedi aumento salario", wxDefaultPosition, wxSize(300,40) };
	wxButton* btnSciopera = new wxButton{ pnlButtons, wxID_ANY, "Sciopera", wxDefaultPosition, wxSize(300,40) };

	btnCercaLavoro->Bind(wxEVT_BUTTON, &DlgLavoro::OnCercaLavoro, this);
	btnLicenziati->Bind(wxEVT_BUTTON, &DlgLavoro::OnLicenziati, this);
	btnInformazioni->Bind(wxEVT_BUTTON, &DlgLavoro::OnInformazioni, this);
	btnLavora->Bind(wxEVT_BUTTON, &DlgLavoro::OnLavora, this);
	btnLeccaculo->Bind(wxEVT_BUTTON, &DlgLavoro::OnLeccaculo, this);
	btnAumento->Bind(wxEVT_BUTTON, &DlgLavoro::OnAumento, this);
	btnSciopera->Bind(wxEVT_BUTTON, &DlgLavoro::OnSciopera, this);

	sizerButtons->Add(btnCercaLavoro, 0, wxTOP | wxRIGHT | wxLEFT, 10);
	sizerButtons->Add(btnLicenziati, 0, wxALL & ~wxTOP, 10);
	sizerButtons->Add(btnInformazioni, 0, wxALL, 10);
	sizerButtons->Add(btnLavora, 0, wxALL & ~wxBOTTOM, 10);
	sizerButtons->Add(btnLeccaculo, 0, wxALL & ~(wxBOTTOM | wxTOP), 10);
	sizerButtons->Add(btnAumento, 0, wxALL & ~(wxBOTTOM | wxTOP), 10);
	sizerButtons->Add(btnSciopera, 0, wxALL & ~wxTOP, 10);

	pnlButtons->SetSizer(sizerButtons);
	sizerBody->Add(pnlButtons, 0, wxALL, 5);

	pnlBody->SetSizer(sizerBody);

	// Info e ok
	m_lblDitta = new wxStaticText(pnlInfo, wxID_ANY, "---");
	m_lblImpegno = new wxStaticText(pnlInfo, wxID_ANY, "---");
	m_lblSoldi = new wxStaticText(pnlInfo, wxID_ANY, "---");
	m_lblStipendio = new wxStaticText(pnlInfo, wxID_ANY, "---");

	sizerInfo->Add(m_lblDitta, 0, wxEXPAND | wxALL, 10);
	sizerInfo->Add(m_lblImpegno, 0, wxEXPAND | wxALL, 10);
	sizerInfo->Add(m_lblSoldi, 0, wxEXPAND | wxALL, 10);
	sizerInfo->Add(m_lblStipendio, 0, wxEXPAND | wxALL, 10);

	pnlInfo->SetSizer(sizerInfo);
	sizerBottom->Add(pnlInfo, 1, wxEXPAND | wxALL, 5);
	//sizerBottom->AddStretchSpacer();

	wxButton* btnOk = new wxButton{ pnlBottom, wxID_OK, "OK", wxDefaultPosition, wxSize(60, 50) };
	sizerBottom->Add(btnOk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
	pnlBottom->SetSizer(sizerBottom);

	mainSizer->Add(pnlBody, 0, wxEXPAND | wxALL, 5);
	mainSizer->Add(pnlBottom, 1, wxEXPAND | wxALL, 5);

	this->SetSizerAndFit(mainSizer);
	this->AggiornaInterfaccia();
}

void DlgLavoro::OnCercaLavoro(wxCommandEvent& event)
{
	if (m_game.AzioneCercaLavoro())
	{
		DlgOffertaLavoro dlg{ this, m_game };
		dlg.Centre();
		dlg.ShowModal();
	}

	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
}

void DlgLavoro::OnLicenziati(wxCommandEvent& event)
{
	m_game.AzioneLicenziati();
	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
}

void DlgLavoro::OnInformazioni(wxCommandEvent& event)
{
	DlgElencoDitte dlg{ this, m_game };
	dlg.Centre();
	dlg.ShowModal();
	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
}

void DlgLavoro::OnLavora(wxCommandEvent& event)
{
	m_game.AzioneLavora();
	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
}

void DlgLavoro::OnLeccaculo(wxCommandEvent& event)
{
	m_game.AzioneLeccaculo();
	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
}

void DlgLavoro::OnAumento(wxCommandEvent& event)
{
	m_game.AzioneAumentoSalario();
	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
}

void DlgLavoro::OnSciopera(wxCommandEvent& event)
{
	m_game.AzioneSciopera();
	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
}

void DlgLavoro::AggiornaInterfaccia()
{
	TabbyGuy& guy = m_game.GetTabbyGuy();

	m_lblDitta->SetLabel("Ditta: " + guy.GetCarriera().GetNomeDitta());
	m_lblImpegno->SetLabel("< Impegno " + std::to_string(guy.GetCarriera().GetImpegno()) + "/100 >");
	m_lblStipendio->SetLabel("Stipendio: " + m_game.GetSoldiStr(guy.GetCarriera().GetStipendio()));
	m_lblSoldi->SetLabel("Soldi: " + m_game.GetSoldiStr(guy.GetSoldi()));
	this->Fit();
	this->Layout();
}

DlgOffertaLavoro::DlgOffertaLavoro(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Annunci - Offerta di lavoro", wxDefaultPosition, wxDefaultSize,  wxCAPTION },
	m_game{game}
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(wxColor(25, 25, 25));
	this->SetForegroundColour(*wxRED);

	// Chiediamo al CERVELLO (TabbyGame) quale lavoro esce per il nostro tabbozzo
	const Ditta& ditta = m_game.ProponiDitta();
	m_ditta = ditta.m_nome;
	
	// TODO: Immagine e icona sotto
	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };
	wxPanel* pnlFoto = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxSize(500,300), wxBORDER_SUNKEN };
	wxBoxSizer* sizerFoto = new wxBoxSizer{ wxVERTICAL };
	// Foto
	pnlFoto->SetSizer(sizerFoto);
	mainSizer->Add(pnlFoto, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	// Linea di separazione
	mainSizer->Add(new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

	// Descrizione
	wxStaticText* lblNomeDitta = new wxStaticText(this, wxID_ANY, "Ditta: " + ditta.m_nome);
	wxStaticText* lblSedeDitta = new wxStaticText(this, wxID_ANY, "Sede: " + ditta.m_sede);
	wxStaticText* lblDesc = new wxStaticText(this, wxID_ANY, ditta.m_offerta.m_descrizione);
	lblDesc->Wrap(600); // Manda a capo il testo se troppo lungo
	mainSizer->Add(lblNomeDitta, 0, wxALL, 10);
	mainSizer->Add(lblSedeDitta, 0, wxALL, 10);
	mainSizer->Add(lblDesc, 0, wxALL, 10);

	// Linea di separazione
	mainSizer->Add(new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

	// Bottoni
	wxButton* btnAccetta = new wxButton{ this, wxID_ANY, ditta.m_offerta.m_accettaStr, wxDefaultPosition, wxSize(-1, 40) };
	wxButton* btnRifiuta = new wxButton{ this, wxID_ANY, ditta.m_offerta.m_rifiutaStr, wxDefaultPosition, wxSize(-1, 40) };

	btnAccetta->Bind(wxEVT_BUTTON, &DlgOffertaLavoro::OnAccetta, this);
	btnRifiuta->Bind(wxEVT_BUTTON, &DlgOffertaLavoro::OnRifiuta, this);

	mainSizer->Add(btnAccetta, 0, wxEXPAND | wxTOP | wxRIGHT | wxLEFT, 10);
	mainSizer->Add(btnRifiuta, 0, wxEXPAND | wxALL & ~wxTOP, 10);

	this->SetSizerAndFit(mainSizer);
}

void DlgOffertaLavoro::OnAccetta(wxCommandEvent& event)
{
	const QuizScheda& quiz = m_game.AssegnaQuiz();
	this->EndModal(wxID_ANY);
	// TODO: IL QUIZ LO POSSO GENERARE DENTRO
	DlgQuiz dlg{ this, m_game, quiz, m_ditta };
	dlg.Centre();
	dlg.ShowModal();
}

void DlgOffertaLavoro::OnRifiuta(wxCommandEvent& event)
{
	m_game.AzioneRifiutaProposta();
	this->EndModal(wxID_ANY);
}

DlgQuiz::DlgQuiz(wxWindow* parent, TabbyGame& game, const QuizScheda& quiz, std::string ditta)
	: wxDialog{ parent, wxID_ANY, quiz.m_titolo, wxDefaultPosition, wxDefaultSize,  wxCAPTION },
	m_game{ game }, m_ditta{ ditta }
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(WIN_BKG);

	// Inizializziamo il vettore per contenere 3 gruppi di risposte
	m_gruppiRisposte.resize(3);

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };
	wxPanel* pnlMain = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* pnlSizer = new wxBoxSizer{ wxVERTICAL };

	// INTRODUZIONE
	wxStaticText* lblIntro = new wxStaticText(pnlMain, wxID_ANY, quiz.m_intro, wxDefaultPosition, wxSize(-1, -1), wxALIGN_CENTER);
	lblIntro->Wrap(700);
	// Un po' di grassetto per l'intro
	wxFont fontIntro = lblIntro->GetFont();
	fontIntro.SetWeight(wxFONTWEIGHT_BOLD);
	lblIntro->SetFont(fontIntro);

	pnlSizer->Add(lblIntro, 0, wxALL | wxALIGN_CENTER, 10);
	pnlSizer->Add(new wxStaticLine(pnlMain), 0, wxEXPAND | wxALL, 5);

	// LE TRE DOMANDE
	for (int i = 0; i < 3; i++)
	{
		AggiungiDomanda(pnlMain, pnlSizer, quiz.m_domande[i], i);
		pnlSizer->Add(new wxStaticLine(pnlMain), 0, wxEXPAND | wxALL, 5);
	}

	// BOTTONE FINALE
	wxButton* btnFinito = new wxButton(pnlMain, wxID_ANY, "Clicca qui quando hai finito di fare il test-a di cazzo!", wxDefaultPosition, wxSize(-1, 40));
	btnFinito->Bind(wxEVT_BUTTON, &DlgQuiz::OnFinito, this);
	pnlSizer->Add(btnFinito, 0, wxALL | wxALIGN_CENTER, 15);


	pnlMain->SetSizer(pnlSizer);
	mainSizer->Add(pnlMain, 1, wxEXPAND | wxALL, 5);

	this->SetSizerAndFit(mainSizer);
	this->Centre();
}

void DlgQuiz::AggiungiDomanda(wxWindow* parent, wxSizer* sizer, const QuizDomanda& domanda, int indiceDomanda)
{
	wxStaticText* txtDomanda = new wxStaticText(parent, wxID_ANY, domanda.m_testo);
	txtDomanda->Wrap(700);
	// Colore per distinguere la domanda
	txtDomanda->SetForegroundColour(*wxBLUE);

	sizer->Add(txtDomanda, 0, wxALL, 10);

	// Creiamo le 3 Checkbox
	for (int i = 0; i < 3; i++)
	{
		wxCheckBox* chk = new wxCheckBox(parent, wxID_ANY, domanda.m_risposte[i]);
		sizer->Add(chk, 0, wxLEFT | wxRIGHT | wxBOTTOM, 5);

		// Salviamo il puntatore nel gruppo giusto per controllarlo dopo
		m_gruppiRisposte[indiceDomanda].push_back(chk);
	}
}

void DlgQuiz::OnFinito(wxCommandEvent& event)
{
	// Raccogliamo solo i dati (Quante crocette per ogni domanda?)
	std::vector<int> conteggi;

	for (const auto& gruppo : m_gruppiRisposte)
	{
		int crocette = 0;
		for (auto cb : gruppo) {
			if (cb->GetValue()) crocette++;
		}
		conteggi.push_back(crocette);
	}

	// Inviamo i dati al cervello
	m_game.AzioneTerminaQuiz(conteggi, m_ditta);
	this->EndModal(wxID_ANY);
}

DlgElencoDitte::DlgElencoDitte(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Elenco Aziende e multinazionali", wxDefaultPosition, wxDefaultSize }, // Nota: wxDefaultSize, la calcoliamo dopo
	m_game{ game }
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour());

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	// TITOLO
	wxStaticText* lblTitolo = new wxStaticText(this, wxID_ANY, "DATABASE AZIENDE E MULTINAZIONALI");
	wxFont fontTitolo = lblTitolo->GetFont();
	fontTitolo.SetWeight(wxFONTWEIGHT_BOLD);
	fontTitolo.SetPointSize(12);
	lblTitolo->SetFont(fontTitolo);

	mainSizer->Add(lblTitolo, 0, wxALL | wxALIGN_CENTER, 10);
	mainSizer->Add(new wxStaticText(this, wxID_ANY, "Seleziona una ditta per visualizzare ulteriori informazioni"), wxALIGN_CENTER | wxALL, 5);

	// ======================================================================================
	// PUNTO 1: BORDI (Grid Lines)
	// Aggiungiamo wxLC_HRULES (orizzontali) e wxLC_VRULES (verticali) allo stile.
	// ======================================================================================
	long style = wxLC_REPORT | wxLC_SINGLE_SEL | wxBORDER_SUNKEN | wxLC_HRULES | wxLC_VRULES;

	m_lista = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxSize(600, 300), style);

	// Creiamo le colonne
	m_lista->InsertColumn(0, "Nome Ditta", wxLIST_FORMAT_LEFT);
	m_lista->InsertColumn(1, "Sede", wxLIST_FORMAT_LEFT);
	m_lista->InsertColumn(2, "Fatturato", wxLIST_FORMAT_RIGHT);

	// Riempiamo la lista
	for (int i = 0; i < ditte.size(); i++)
	{
		const Ditta& d = ditte[i];

		// ==================================================================================
		// PUNTO 2: PADDING DEL TESTO
		// Il trucco più semplice è aggiungere spazi manualmente alla stringa ("  " + testo + "  ")
		// perché wxListCtrl non supporta il CSS o il padding nativo facilmente.
		// ==================================================================================

		// IMPORTANTE: wxListCtrl possiede ItemData, un contenitore in cui inserire dei dati
		wxString nomePad = "  " + d.m_nome + "  ";
		long index = m_lista->InsertItem(i, nomePad);

		wxString sedePad = "  " + d.m_sede + "  ";
		m_lista->SetItem(index, 1, sedePad);

		wxString fatturatoStr;
		if (d.m_fatturato == -1)
			fatturatoStr = "Sconosciuto";
		else
			fatturatoStr = m_game.GetSoldiStr(d.m_fatturato);

		// Padding anche per il fatturato
		m_lista->SetItem(index, 2, fatturatoStr + "  ");
		// VENGONO ASSOCIATI I DATI
		m_lista->SetItemData(index, i);
	}

	// Calcoliamo le larghezze automatiche ORA che c'è il testo (anche con gli spazi extra)
	m_lista->SetColumnWidth(0, wxLIST_AUTOSIZE);
	m_lista->SetColumnWidth(1, wxLIST_AUTOSIZE);
	m_lista->SetColumnWidth(2, wxLIST_AUTOSIZE_USEHEADER); // UseHeader evita che diventi minuscola se "Sconosciuto"

	// ======================================================================================
	// PUNTO 3: ESTENDERE LA FINESTRA (Niente scroll orizzontale)
	// Calcoliamo la somma delle larghezze delle colonne e allarghiamo la finestra.
	// ======================================================================================
	int larghezzaTotale = 0;
	larghezzaTotale += m_lista->GetColumnWidth(0);
	larghezzaTotale += m_lista->GetColumnWidth(1);
	larghezzaTotale += m_lista->GetColumnWidth(2);

	// Aggiungiamo un po' di margine (es. 60px) per la barra di scorrimento verticale e i bordi della finestra
	int larghezzaFinestra = larghezzaTotale + 60;

	// Impostiamo un minimo (es. 600) per non farla troppo stretta se ci sono pochi dati
	if (larghezzaFinestra < 600) larghezzaFinestra = 600;

	// Impostiamo la dimensione della finestra (Larghezza calcolata, Altezza fissa 450)
	this->SetSize(larghezzaFinestra, 450);

	// Bind evento
	m_lista->Bind(wxEVT_LIST_ITEM_ACTIVATED, &DlgElencoDitte::OnDittaSelezionata, this);

	mainSizer->Add(m_lista, 1, wxEXPAND | wxALL, 10);

	// Bottone Chiudi
	wxButton* btnChiudi = new wxButton(this, wxID_CANCEL, "Chiudi", wxDefaultPosition, wxSize(100, 35));
	mainSizer->Add(btnChiudi, 0, wxALIGN_RIGHT | wxALL, 10);

	this->SetSizer(mainSizer);
	// Non chiamiamo Fit() qui perché abbiamo settato manualmente la dimensione con SetSize sopra
	this->Layout();
	this->Centre();
}

void DlgElencoDitte::OnDittaSelezionata(wxListEvent& event)
{
	long dittaIndex = event.GetItem().GetData();

	if (dittaIndex >= 0 && dittaIndex < ditte.size())
	{
		// Creiamo e mostriamo la finestra INFO (senza offerta lavoro)
		DlgInfoDitta dlg(this, m_game, ditte[dittaIndex]);
		dlg.ShowModal();
	}
}

DlgInfoDitta::DlgInfoDitta(wxWindow* parent, TabbyGame& game, const Ditta& ditta)
	: wxDialog{ parent, wxID_ANY, "Informazioni Azienda", wxDefaultPosition, wxDefaultSize, wxCAPTION | wxCLOSE_BOX }
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour());

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	// TODO: IMMAGINE
	wxPanel* pnlFoto = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxSize(500, 300), wxBORDER_SUNKEN };
	mainSizer->Add(pnlFoto, 0, wxALIGN_CENTER | wxALL, 10);

	// Separatore
	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxALL, 10);

	// 2. DATI PRINCIPALI (Nome, Sede, Fatturato)
	wxBoxSizer* headerSizer = new wxBoxSizer{ wxVERTICAL };

	// Nome Ditta (Grande e Grassetto)
	wxStaticText* lblNome = new wxStaticText(this, wxID_ANY, ditta.m_nome);
	wxFont fontGrande = lblNome->GetFont();
	fontGrande.SetPointSize(14);
	fontGrande.SetWeight(wxFONTWEIGHT_BOLD);
	lblNome->SetFont(fontGrande);

	headerSizer->Add(lblNome, 0, wxALL, 10);

	// Sede
	headerSizer->Add(new wxStaticText(this, wxID_ANY, "Sede: " + ditta.m_sede), 0, wxALL, 5);

	// Fatturato
	wxString fatturatoStr;
	if (ditta.m_fatturato == -1)
		fatturatoStr = "Sconosciuto";
	else
		fatturatoStr = game.GetSoldiStr(ditta.m_fatturato);

	headerSizer->Add(new wxStaticText(this, wxID_ANY, "Fatturato annuo: " + fatturatoStr), 0, wxALL, 5);

	mainSizer->Add(headerSizer, 0, wxEXPAND | wxALL, 5);

	// Separatore
	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxALL, 10);

	// 3. DESCRIZIONI (Presentazione e Produzioni)
	wxBoxSizer* descSizer = new wxBoxSizer{ wxVERTICAL };

	// Presentazione
	if (!ditta.m_presentazione.empty())
	{
		wxStaticText* lblPres = new wxStaticText(this, wxID_ANY, ditta.m_presentazione);
		lblPres->Wrap(550); // Manda a capo
		descSizer->Add(lblPres, 0, wxALL, 5);
	}

	// Produzioni (Elenco prodotti)
	if (!ditta.m_produzioni.empty())
	{
		wxStaticText* lblProd = new wxStaticText(this, wxID_ANY, ditta.m_produzioni);
		lblProd->Wrap(550);
		lblProd->SetForegroundColour(*wxBLUE);
		descSizer->Add(lblProd, 0, wxALL, 5);
	}

	mainSizer->Add(descSizer, 0, wxEXPAND | wxALL, 10);

	// Separatore
	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxALL, 10);

	// BOTTONE CHIUDI
	wxButton* btnOk = new wxButton(this, wxID_OK, "Chiudi", wxDefaultPosition, wxSize(150, 35));
	mainSizer->Add(btnOk, 0, wxALIGN_CENTER | wxALL, 15);

	this->SetSizerAndFit(mainSizer);
	this->Centre();
}

DlgDisco::DlgDisco(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Disco", wxDefaultPosition, wxDefaultSize },
	m_game{ game }, m_selectedIndex{ -1 }
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour());

	// Sizer Principale Verticale
	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	wxPanel* pnlBody = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerBody = new wxBoxSizer{ wxHORIZONTAL };
	wxPanel* pnlBottom = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };

	// COLONNA SINISTRA (Radio Buttons)
	wxPanel* pnlLocali = new wxPanel{ pnlBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerLocali = new wxBoxSizer{ wxVERTICAL };

	// Creiamo i radio button dinamicamente
	for (int i = 0; i < discoteche.size(); i++)
	{
		// wxRB_GROUP solo al primo, ma vogliamo deselezionarli all'inizio
		long style = (i == 0) ? wxRB_GROUP : 0;
		wxRadioButton* rad = new wxRadioButton(pnlLocali, wxID_ANY, discoteche[i].m_nome, wxDefaultPosition, wxDefaultSize, style);

		// Hack per deselezionare visivamente tutto all'inizio (wxWidgets tende a selezionare il primo del gruppo)
		rad->SetValue(false);
		// Bind dell'evento (usiamo una lambda per catturare l'indice 'i')
		rad->Bind(wxEVT_RADIOBUTTON, [this, i](wxCommandEvent& ev) {
			m_selectedIndex = i;
			OnRadioSelect(ev);
			});

		m_radios.push_back(rad);
		sizerLocali->Add(rad, 0, wxLEFT | wxTOP | wxBOTTOM, 5);
	}

	pnlLocali->SetSizer(sizerLocali);
	sizerBody->Add(pnlLocali, 0, wxEXPAND | wxALL, 5);

	// Un pannello incassato per il testo
	wxPanel* pnlDesc = new wxPanel(pnlBody, wxID_ANY, wxDefaultPosition, wxSize(550, -1), wxBORDER_SUNKEN);
	wxBoxSizer* sizerDesc = new wxBoxSizer{ wxVERTICAL };

	m_lblDescrizione = new wxStaticText(pnlDesc, wxID_ANY, "");

	m_lblPrezzo = new wxStaticText(pnlDesc, wxID_ANY, "");
	wxFont fBold = m_lblPrezzo->GetFont();
	fBold.SetWeight(wxFONTWEIGHT_BOLD);
	m_lblPrezzo->SetFont(fBold);

	sizerDesc->Add(m_lblDescrizione, 1, wxEXPAND | wxALL, 10);
	sizerDesc->Add(new wxStaticLine(pnlDesc), 0, wxEXPAND | wxALL, 2);
	sizerDesc->Add(m_lblPrezzo, 0, wxALL, 5);

	pnlDesc->SetSizer(sizerDesc);
	sizerBody->Add(pnlDesc, 1, wxEXPAND | wxALL, 5);
	pnlBody->SetSizer(sizerBody);
	mainSizer->Add(pnlBody, 1, wxEXPAND | wxALL, 5);

	// --- PARTE INFERIORE (Icona, Soldi, Bottoni) ---
	// Icona (Placeholder per "ZDISCO")
	wxStaticText* iconBox = new wxStaticText(pnlBottom, wxID_ANY, "[ICON]", wxDefaultPosition, wxSize(-1, -1), wxALIGN_CENTER | wxBORDER_SIMPLE);
	sizerBottom->Add(iconBox, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);

	// Soldi
	wxString soldiStr = "Soldi: " + m_game.GetSoldiStr(m_game.GetTabbyGuy().GetSoldi());
	m_lblSoldiAttuali = new wxStaticText(pnlBottom, wxID_ANY, soldiStr);
	sizerBottom->Add(m_lblSoldiAttuali, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 15);

	sizerBottom->AddStretchSpacer();

	// Bottoni
	wxButton* btnOk = new wxButton(pnlBottom, wxID_OK, "OK", wxDefaultPosition, wxSize(-1, -1));
	wxButton* btnCancel = new wxButton(pnlBottom, wxID_CANCEL, "Cancel", wxDefaultPosition, wxSize(-1, -1));

	// Bind OK manuale per fare i controlli prima di chiudere
	btnOk->Bind(wxEVT_BUTTON, &DlgDisco::OnOk, this);

	sizerBottom->Add(btnOk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
	sizerBottom->Add(btnCancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
	pnlBottom->SetSizer(sizerBottom);

	mainSizer->Add(pnlBottom, 0, wxEXPAND | wxALL, 10);

	this->SetSizerAndFit(mainSizer);
	this->Centre();
}

void DlgDisco::OnRadioSelect(wxCommandEvent& event)
{
	if (m_selectedIndex < 0) return;

	const Disco& d = discoteche[m_selectedIndex];

	// Aggiorna Descrizione
	m_lblDescrizione->SetLabel(d.m_descrizione);
	m_lblDescrizione->Wrap(500); // Wrappa il testo per stare nel pannello

	// Aggiorna Prezzo
	m_lblPrezzo->SetLabel("Ingresso: " + m_game.GetSoldiStr(d.m_prezzoIngresso));

	// Aggiorna layout del pannello descrizione
	m_lblDescrizione->GetParent()->Layout();

	this->Fit();
}

void DlgDisco::OnOk(wxCommandEvent& event)
{
	// Controllo Selezione
	if (m_selectedIndex < 0) {
		wxMessageBox("Non hai selezionato nessun locale!", "Tabboz Simulator", wxOK | wxICON_EXCLAMATION);
		return;
	}

	const Disco& d = discoteche[m_selectedIndex];
	// Chiama la funzione logica che hai già in TabbyGame
	m_game.AzionePagaDisco(m_selectedIndex);
	ManifestaEventi(this, m_game);

	// Chiudi la finestra
	EndModal(wxID_OK);
}

// TODO: SISTEMA
DlgTipa::DlgTipa(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Tipa"},
		m_game{ game }
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour());

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	// --- CORPO CENTRALE (Foto + Bottoni) ---
	wxPanel* pnlBody = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerBody = new wxBoxSizer{ wxHORIZONTAL };

	// FOTO (Sinistra)
	wxPanel* pnlFoto = new wxPanel{ pnlBody, wxID_ANY, wxDefaultPosition, wxSize(200, 150), wxBORDER_SUNKEN };
	// TODO: Caricare immagine tipa qui
	sizerBody->Add(pnlFoto, 0, wxALL, 5);

	// BOTTONI (Destra)
	wxPanel* pnlButtons = new wxPanel{ pnlBody, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerBtn = new wxBoxSizer{ wxVERTICAL };

	wxButton* btnCerca = new wxButton(pnlButtons, wxID_ANY, "Cerca tipa", wxDefaultPosition, wxSize(300, 40));
	wxButton* btnLascia = new wxButton(pnlButtons, wxID_ANY, "Lascia Tipa", wxDefaultPosition, wxSize(300, 40));
	wxButton* btnTelefona = new wxButton(pnlButtons, wxID_ANY, "Telefona alla tipa", wxDefaultPosition, wxSize(300, 40));
	wxButton* btnEsci = new wxButton(pnlButtons, wxID_ANY, "Esci con la tipa", wxDefaultPosition, wxSize(300, 40));

	// Bind eventi
	btnCerca->Bind(wxEVT_BUTTON, &DlgTipa::OnCercaTipa, this);
	btnLascia->Bind(wxEVT_BUTTON, &DlgTipa::OnLasciaTipa, this);
	btnTelefona->Bind(wxEVT_BUTTON, &DlgTipa::OnTelefonaTipa, this);
	btnEsci->Bind(wxEVT_BUTTON, &DlgTipa::OnEsciTipa, this);

	sizerBtn->Add(btnCerca, 0, wxALL & ~wxBOTTOM, 5);
	sizerBtn->Add(btnLascia, 0, wxALL & ~wxTOP, 5);
	sizerBtn->Add(new wxStaticLine(pnlButtons), 0, wxEXPAND | wxALL, 5);
	sizerBtn->Add(btnTelefona, 0, wxALL & ~wxBOTTOM, 5);
	sizerBtn->Add(btnEsci, 0, wxALL & ~wxTOP, 5);

	pnlButtons->SetSizer(sizerBtn);
	sizerBody->Add(pnlButtons, 1, wxEXPAND | wxALL, 5);

	pnlBody->SetSizer(sizerBody);
	mainSizer->Add(pnlBody, 0, wxEXPAND | wxALL, 5);

	// --- PANNELLO INFO (Sotto) ---
	wxPanel* pnlInfo = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerInfo = new wxBoxSizer{ wxVERTICAL };

	// Label statiche e Valori dinamici
	m_lblNome = new wxStaticText(pnlInfo, wxID_ANY, "");
	wxFont fBold = m_lblNome->GetFont(); fBold.SetWeight(wxFONTWEIGHT_BOLD); m_lblNome->SetFont(fBold);
	sizerInfo->Add(m_lblNome, 0, wxALIGN_LEFT);

	m_lblFamaTipa = new wxStaticText(pnlInfo, wxID_ANY, "");
	sizerInfo->Add(m_lblFamaTipa, 0, wxALIGN_LEFT);

	m_lblRapportiTipa = new wxStaticText(pnlInfo, wxID_ANY, "");
	sizerInfo->Add(m_lblRapportiTipa, 0, wxALIGN_LEFT);

	// Centriamo la griglia nel pannello
	wxBoxSizer* infoWrapper = new wxBoxSizer(wxVERTICAL);
	infoWrapper->Add(sizerInfo, 1, wxALL, 10);
	pnlInfo->SetSizer(infoWrapper);

	mainSizer->Add(pnlInfo, 0, wxEXPAND | wxALL, 5);

	// --- FOOTER (Soldi + OK) ---
	wxBoxSizer* footerSizer = new wxBoxSizer(wxHORIZONTAL);

	// Icona e figosità
	footerSizer->Add(new wxStaticText(this, wxID_ANY, "[ICON]"), 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
	m_lblFama = new wxStaticText(this, wxID_ANY, "");
	footerSizer->Add(m_lblFama, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);

	footerSizer->AddStretchSpacer();

	wxButton* btnOk = new wxButton(this, wxID_OK, "OK", wxDefaultPosition, wxSize(80, 30));
	footerSizer->Add(btnOk, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

	mainSizer->Add(footerSizer, 0, wxEXPAND | wxBOTTOM, 10);

	this->SetSizerAndFit(mainSizer);

	// Aggiorniamo lo stato iniziale (bottoni abilitati/disabilitati)
	AggiornaInterfaccia();

	this->Centre();
}

void DlgTipa::OnCercaTipa(wxCommandEvent& event)
{
	DlgIncontroTipa dlg{ this, m_game };
	dlg.Centre();
	dlg.ShowModal();

	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
}

void DlgTipa::OnLasciaTipa(wxCommandEvent& event)
{
	m_game.AzioneLasciaTipa();
	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
}

void DlgTipa::OnTelefonaTipa(wxCommandEvent& event)
{
	m_game.AzioneTelefonaTipa();
	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
}

void DlgTipa::OnEsciTipa(wxCommandEvent& event)
{
	m_game.AzioneEsciTipa();
	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
}

void DlgTipa::AggiornaInterfaccia()
{
	if (m_game.GetTabbyGuy().HaTipa())
	{
		const Tipa& tipa = m_game.GetTabbyGuy().GetTipa();
		m_lblNome->SetLabel("Nome: "+tipa.GetNome());
		m_lblFamaTipa->SetLabel("Figosità: "+std::to_string(tipa.GetFama())+"/100");
		m_lblRapportiTipa->SetLabel("Quanto state bene insieme: "+std::to_string(m_game.GetTabbyGuy().GetRapporti())+"/100");
	}
	else
	{
		m_lblNome->SetLabel("");
		m_lblFamaTipa->SetLabel("");
		m_lblRapportiTipa->SetLabel("");
	}

	m_lblFama->SetLabel("< Figosità " + std::to_string(m_game.GetTabbyGuy().GetFama()) + "/100 >");

	this->Layout();
}

DlgIncontroTipa::DlgIncontroTipa(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Esci allo scoperto...", wxDefaultPosition, wxDefaultSize, wxCAPTION },
	m_game{ game }, m_tipa{}
{
	m_tipa = m_game.GeneraTipa();

	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour());

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	wxPanel* pnlInfo = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerInfo = new wxBoxSizer{ wxVERTICAL };

	wxPanel* pnlFoto = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerFoto = new wxBoxSizer{ wxVERTICAL };

	sizerInfo->Add(new wxStaticText(pnlInfo, wxID_ANY, "Nome:"), 0, wxALL & ~wxBOTTOM, 5);
	sizerInfo->Add(new wxStaticText(pnlInfo, wxID_ANY, m_tipa.GetNome(), wxDefaultPosition, wxSize(250, -1), wxALIGN_CENTER | wxBORDER_SUNKEN), 0, wxALL, 5);
	sizerInfo->Add(new wxStaticText(pnlInfo, wxID_ANY, "Figosità:"), 0, wxALL & ~wxBOTTOM, 5);
	sizerInfo->Add(new wxStaticText(pnlInfo, wxID_ANY, std::to_string(m_tipa.GetFama())+"/100", wxDefaultPosition, wxSize(250, -1), wxALIGN_CENTER | wxBORDER_SUNKEN), 0, wxALL, 5);
	sizerInfo->Add(new wxStaticText(pnlInfo, wxID_ANY, "Giudizio\ncomplessivo:"), 0, wxALL & ~wxBOTTOM, 5);
	sizerInfo->Add(new wxStaticText(pnlInfo, wxID_ANY, m_tipa.GetDesc(), wxDefaultPosition, wxSize(250, -1), wxALIGN_CENTER | wxBORDER_SUNKEN), 0, wxALL, 5);
	sizerInfo->AddStretchSpacer();

	wxButton* btnProvaci = new wxButton(pnlInfo, wxID_ANY, "Ci provo !", wxDefaultPosition, wxSize(250, -1));
	wxButton* btnLasciaStare = new wxButton(pnlInfo, wxID_ANY, "Ritorno a casa...", wxDefaultPosition, wxSize(250, -1));
	btnProvaci->Bind(wxEVT_BUTTON, &DlgIncontroTipa::OnProvaci, this);
	btnLasciaStare->Bind(wxEVT_BUTTON, &DlgIncontroTipa::OnLasciaStare, this);
	sizerInfo->Add(btnProvaci, 0, wxALL & ~wxBOTTOM, 5);
	sizerInfo->Add(btnLasciaStare, 0, wxALL & ~wxTOP, 5);

	pnlInfo->SetSizer(sizerInfo);
	mainSizer->Add(pnlInfo, 0, wxALL, 5);

	mainSizer->Add(pnlFoto, 0, wxALL, 5);

	this->SetSizerAndFit(mainSizer);
	this->Centre();
}

void DlgIncontroTipa::OnProvaci(wxCommandEvent& event)
{
	m_game.AzioneProvaci(m_tipa);
	this->EndModal(wxID_ANY);
}

void DlgIncontroTipa::OnLasciaStare(wxCommandEvent& event)
{
	this->EndModal(wxID_ANY);
}

DlgElencoNegozi::DlgElencoNegozi(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Negozi", wxDefaultPosition, wxDefaultSize },
	m_game{ game }
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour());

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	wxPanel* pnlBody = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerBody = new wxBoxSizer{ wxVERTICAL };
	
	wxPanel* pnlBottom = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };
	sizerBody->AddSpacer(5);
	// BOTTONI NEGOZI
	for (int i = 0; i < negozi.size(); i++)
	{
		wxButton* btnNegozio = new wxButton{ pnlBody, wxID_ANY, negozi[i].m_nome, wxDefaultPosition, wxSize(-1, 40), wxEXPAND};
		// Sfruttiamo una funzione lambda per associare in modo dinamico il bottone al negozio selezionato
		sizerBody->Add(btnNegozio, 0, wxEXPAND | wxRIGHT | wxLEFT, 5);
		btnNegozio->Bind(wxEVT_BUTTON, [this, i](wxCommandEvent&) {

			if(m_game.TriggerNegozio(negozi[i].m_merce))
			{
				DlgNegozio dlg{ this, m_game, negozi[i] };
				dlg.ShowModal();
				AggiornaInterfaccia();
			}
			ManifestaEventi(this, m_game);
			
		});
	}
	sizerBody->AddSpacer(5);

	// TODO: BOTTONI NEGOZI
	pnlBody->SetSizer(sizerBody);
	mainSizer->Add(pnlBody, 1, wxEXPAND | wxALL, 5);

	m_lblSoldi = new wxStaticText{ pnlBottom, wxID_ANY, "---" };
	sizerBottom->Add(m_lblSoldi, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
	sizerBottom->AddStretchSpacer();
	wxButton* btnChiudi = new wxButton{ pnlBottom, wxID_CANCEL, "Chiudi", wxDefaultPosition, wxSize(120, 40)};
	sizerBottom->Add(btnChiudi, 1, wxEXPAND | wxALL, 10);

	pnlBottom->SetSizer(sizerBottom);
	mainSizer->Add(pnlBottom, 0, wxEXPAND | wxALL, 5);

	this->SetSizerAndFit(mainSizer);
	this->AggiornaInterfaccia();
}

void DlgElencoNegozi::AggiornaInterfaccia()
{
	m_lblSoldi->SetLabel("Soldi: " + m_game.GetSoldiStr(m_game.GetTabbyGuy().GetSoldi()));
	this->Layout();
}

PnlProdotto::PnlProdotto(wxWindow* parent, DlgNegozio* mainDlg, TabbyGame& game, const Prodotto& prod)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_RAISED), // Altezza -1 = Auto
	m_game(game), m_prodotto(prod), m_parentDlg(mainDlg)
{
	this->SetBackgroundColour(wxColour(255, 240, 170));

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	// 1. TITOLO
	wxStaticText* lblNome = new wxStaticText(this, wxID_ANY, m_prodotto.GetNome());
	wxFont fTitle = lblNome->GetFont();
	fTitle.SetWeight(wxFONTWEIGHT_BOLD);
	fTitle.SetPointSize(12);
	lblNome->SetFont(fTitle);
	lblNome->SetForegroundColour(wxColor(0, 0, 150));

	mainSizer->Add(lblNome, 0, wxALL, 8);
	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

	// 2. CORPO (Icona + Descrizione)
	wxBoxSizer* bodySizer = new wxBoxSizer(wxHORIZONTAL);

	// Icona
	wxPanel* pnlIcon = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(128, 128), wxBORDER_SIMPLE);
	pnlIcon->SetBackgroundColour(wxColor(220, 220, 220));
	// Qui andrà l'icona vera
	bodySizer->Add(pnlIcon, 0, wxALL, 10);

	// Descrizione
	wxStaticText* lblDesc = new wxStaticText(this, wxID_ANY, m_prodotto.GetDesc());
	wxFont fDesc = lblDesc->GetFont();
	fDesc.SetPointSize(10);
	lblDesc->SetFont(fDesc);
	lblDesc->Wrap(350); // Manda a capo il testo per stare nella larghezza rimanente

	bodySizer->Add(lblDesc, 0, wxEXPAND | wxALL, 5);

	mainSizer->Add(bodySizer, 0, wxEXPAND | wxALL, 5); // Proportion 0 qui, lascia che sia il contenuto a spingere

	// 3. DATI E BOTTONE
	wxBoxSizer* footerSizer = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* lblPrezzo = new wxStaticText(this, wxID_ANY, m_game.GetSoldiStr(m_prodotto.GetPrezzo()));
	wxFont fPrice = lblPrezzo->GetFont(); fPrice.SetWeight(wxFONTWEIGHT_BOLD); lblPrezzo->SetFont(fPrice);
	footerSizer->Add(lblPrezzo, 0, wxALIGN_CENTER_VERTICAL | wxALL, 15);

	footerSizer->AddStretchSpacer();

	// BOTTONE COMPRA VERDE
	wxButton* btnCompra = new wxButton(this, wxID_ANY, "COMPRA", wxDefaultPosition, wxSize(90, 40));
	btnCompra->SetBackgroundColour(wxColor(0, 180, 0)); // Verde scuro
	btnCompra->SetForegroundColour(*wxWHITE);           // Testo bianco
	wxFont fBtn = btnCompra->GetFont(); fBtn.SetWeight(wxFONTWEIGHT_BOLD); btnCompra->SetFont(fBtn);

	btnCompra->Bind(wxEVT_BUTTON, &PnlProdotto::OnCompra, this);

	footerSizer->Add(btnCompra, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10);

	mainSizer->Add(footerSizer, 0, wxEXPAND); // Footer in fondo

	this->SetSizerAndFit(mainSizer);	// Calcola l'altezza necessaria per far stare tutto il testo
}

void PnlProdotto::OnCompra(wxCommandEvent& event)
{
	m_game.AzioneCompra(m_prodotto);
	ManifestaEventi(this, m_game);
}


DlgNegozio::DlgNegozio(wxWindow* parent, TabbyGame& game, const Negozio& negozio)
	: wxDialog{ parent, wxID_ANY, negozio.m_nome, wxDefaultPosition, wxDefaultSize},
	m_game{ game }, m_negozio{ negozio }
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	// TITOLO
	wxStaticText* title = new wxStaticText(this, wxID_ANY, "BENVENUTO DA " + m_negozio.m_nome);
	wxFont fTitle = title->GetFont(); fTitle.SetWeight(wxFONTWEIGHT_BOLD); fTitle.SetPointSize(14);
	title->SetFont(fTitle);
	title->SetForegroundColour(wxColor(50, 50, 50));
	mainSizer->Add(title, 0, wxALL & ~wxBOTTOM, 15);

	// --- AREA SCORREVOLE ---
	wxScrolledWindow* scrollWin = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
	scrollWin->SetScrollRate(0, 20); // Scroll verticale attivo
	scrollWin->SetBackgroundColour(wxColor(40, 40, 40));

	// --- GRIGLIA DEI PRODOTTI ---
	wxGridSizer* gridSizer = new wxGridSizer(0, 2, 10, 10);

	for (const auto& prod : m_negozio.m_catalogo) {
		PnlProdotto* scheda = new PnlProdotto(scrollWin, this, m_game, prod);

		// Aggiungiamo alla griglia.
		gridSizer->Add(scheda, 0, wxEXPAND | wxALL, 5);
	}
	scrollWin->SetSizer(gridSizer);

	// --- CALCOLO AUTOMATICO DIMENSIONI ---
	// 1. Chiediamo al Sizer: "Quanto spazio ti serve MINIMO per far stare tutto?"
	wxSize gridSize = gridSizer->GetMinSize();
	// 2. Aggiungiamo un margine per la barra di scorrimento verticale (circa 20-30px)
	int larghezzaTotale = gridSize.x + 40;
	// 3. Imponiamo questa larghezza alla finestra di scroll.
	scrollWin->SetMinSize(wxSize(larghezzaTotale, 650));
	mainSizer->Add(scrollWin, 0, wxEXPAND | wxALL, 5);

	// --- BARRA INFERIORE ---
	wxPanel* pnlBottom = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_RAISED);
	wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);

	m_lblSoldi = new wxStaticText(pnlBottom, wxID_ANY, "---");
	wxFont fSoldi = m_lblSoldi->GetFont(); fSoldi.SetWeight(wxFONTWEIGHT_BOLD); fSoldi.SetPointSize(12);
	m_lblSoldi->SetFont(fSoldi);

	bottomSizer->Add(m_lblSoldi, 0, wxALL | wxALIGN_CENTER_VERTICAL, 20);
	bottomSizer->AddStretchSpacer();

	wxButton* btnEsci = new wxButton(pnlBottom, wxID_CANCEL, "Esci", wxDefaultPosition, wxSize(150, 50));
	bottomSizer->Add(btnEsci, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10);

	pnlBottom->SetSizer(bottomSizer);
	mainSizer->Add(pnlBottom, 0, wxEXPAND | wxALL, 0);

	this->SetSizerAndFit(mainSizer);
	this->Centre();
	AggiornaInterfaccia();
}

void DlgNegozio::AggiornaInterfaccia()
{
	m_lblSoldi->SetLabel("< Soldi: " + m_game.GetSoldiStr(m_game.GetTabbyGuy().GetSoldi())+" >");
	this->Layout();
}