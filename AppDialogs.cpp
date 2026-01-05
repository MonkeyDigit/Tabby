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

void DlgScooter::AggiornaInterfaccia()
{
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
		// Caso scelta: SÌ e NO
		// TODO: accetta e rifiuta
		// TODO: aggiungi stringhe speciali per roba tipo lascia tipa, fai questo fai quello
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
	wxPanel* pnlFoto	= new wxPanel{ pnlBody, wxID_ANY, wxDefaultPosition, wxSize(300,200), wxBORDER_SUNKEN};
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

	// Bottoni
	wxButton* btnCercaLavoro = new wxButton{ pnlButtons, wxID_ANY, "Cerca lavoro", wxDefaultPosition, wxSize(300, 40) };
	wxButton* btnLicenziati = new wxButton{ pnlButtons, wxID_ANY, "Licenziati", wxDefaultPosition, wxSize(300, 40) };
	wxButton* btnInformazioni = new wxButton{ pnlButtons, wxID_ANY, "Informazioni", wxDefaultPosition, wxSize(300, 40) };
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
	sizerButtons->Add(btnInformazioni, 0, wxALL,10);
	sizerButtons->Add(btnLavora, 0, wxALL & ~wxBOTTOM, 10);
	sizerButtons->Add(btnLeccaculo, 0, wxALL & ~(wxBOTTOM | wxTOP), 10);
	sizerButtons->Add(btnAumento, 0, wxALL & ~(wxBOTTOM | wxTOP), 10);
	sizerButtons->Add(btnSciopera, 0, wxALL & ~wxTOP, 10);

	pnlButtons->SetSizer(sizerButtons);
	sizerBody->Add(pnlButtons, 0, wxEXPAND | wxALL, 5);

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
	mainSizer->Add(pnlBottom, 0, wxEXPAND | wxALL, 5);

	this->SetSizerAndFit(mainSizer);
	this->AggiornaInterfaccia();
}

void DlgLavoro::OnCercaLavoro(wxCommandEvent& event)
{
	if (m_game.AzioneCercaLavoro())
	{
		// Chiediamo al CERVELLO (TabbyGame) quale lavoro esce per il nostro tabbozzo
		// TODO: FARE REFERENCE?
		const Ditta& ditta = m_game.ProponiDitta();
		DlgOffertaLavoro dlg{ this, ditta };
		dlg.Centre();
		dlg.ShowModal();
	}

	// TODO: TOGLIERE ???
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
	m_game.AzioneInformazioni();
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
	std::string nomeDitta{};
	if (guy.HaUnLavoro())
		nomeDitta = ditte[guy.GetCarriera().GetIdDitta()].m_nome;

	m_lblDitta->SetLabel("Ditta: " + nomeDitta);
	m_lblImpegno->SetLabel("Impegno " + std::to_string(guy.GetCarriera().GetImpegno()) + "/100");
	m_lblStipendio->SetLabel("Stipendio: " + m_game.GetSoldiStr(guy.GetCarriera().GetStipendio()));
	m_lblSoldi->SetLabel("Soldi: " + m_game.GetSoldiStr(guy.GetSoldi()));
}

DlgOffertaLavoro::DlgOffertaLavoro(wxWindow* parent, const Ditta& ditta)
	: wxDialog{ parent, wxID_ANY, "Annunci - Offerta di lavoro", wxDefaultPosition, wxDefaultSize,  wxCAPTION }
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(*wxBLACK);
	this->SetForegroundColour(*wxRED);

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
	// TODO: COMPLETA
	this->EndModal(wxID_ANY);
}

void DlgOffertaLavoro::OnRifiuta(wxCommandEvent& event)
{
	// TODO: COMPLETA
	this->EndModal(wxID_ANY);
}