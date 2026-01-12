#include <wx/artprov.h>
#include <wx/sound.h>

#ifdef __WXMSW__
// Questa macro impedisce a windows.h di includere winsock.h e altra robaccia inutile
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h> 
#endif
#include "AppDialogs.h"

void ManifestaEventi(wxWindow* parent, TabbyGame& game)
{
	Messaggio msg;

	while (game.PollMessaggi(msg))
	{
		// GESTIONE DEI SUONI PERSONALI .WAV
		if (msg.m_soundId != -1)
		{
			// Questa riga "costruisce" una stringa di testo. Il segnaposto %04d prende il numero e lo scrive come un intero di 4 cifre, aggiungendo degli zeri davanti se necessario
			wxString path = wxString::Format("sounds/tabs%04d.wav", (int)msg.m_soundId);

			if (wxFileExists(path)) {
				wxSound s(path);
				if (s.IsOk()) {
					s.Play(wxSOUND_ASYNC);
				}
				else {
					// Se entra qui, il file esiste ma il formato non è supportato
					game.WriteLog("ERRORE AUDIO: Formato file non supportato: " + path.ToStdString());
				}
			}
			else {
				// Se entra qui, il file proprio non viene trovato
				game.WriteLog("ERRORE AUDIO: File non trovato: " + path.ToStdString());
			}
		}

		if (msg.m_tipo == TipoMsg::SUONO) continue;	// Solo un suono, nessun messaggio da mostrare

		// GESTIONE SUONI DI SISTEMA
#ifdef __WXMSW__
		UINT soundType = MB_OK;
		if (msg.m_tipo == TipoMsg::ERRORE) soundType = MB_ICONHAND;
		else if (msg.m_tipo == TipoMsg::AVVISO) soundType = MB_ICONWARNING;
		else if (msg.m_tipo == TipoMsg::INFO) soundType = MB_ICONASTERISK;
		else if (msg.m_tipo == TipoMsg::SCELTA) soundType = MB_ICONQUESTION;

		// Riproduce il suono ORIGINALE di Windows
		MessageBeep(soundType);
#endif

		if (msg.m_tipo == TipoMsg::DUE_DONNE)
		{
			DlgDueDonne dlg{ parent, game };
			dlg.ShowModal();
		}
		else if (msg.m_tipo == TipoMsg::PAGELLA) {
			DlgPagella dlg{ parent, game };
			dlg.ShowModal();
		}
		else    // Un evento comune
		{
			DlgEvento dlgEvento{ parent, msg };
			// Nel caso di un pop up evento con scelta (previa implementazione degli appositi bottoni con wxID_YES e wxID_NO), gli id vengono restituiti alla finestra padre
			// Qua valutiamo l'espressione logica
			bool scelta = (dlgEvento.ShowModal() == wxID_YES);

			// Se qui ApplicaScelta aggiunge un NUOVO evento, questo finisce in fondo alla coda. Il ciclo while continuerà a girare e lo pescherà subito dopo
			if (msg.m_tipo == TipoMsg::SCELTA)
				game.ApplicaScelta(msg.m_msgAzione, scelta);
		}
	}
	// Usciti dal while, la coda è sicuramente vuota
}

// DIALOG SCOOTER
DlgScooter::DlgScooter(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Scooter", wxDefaultPosition, wxDefaultSize },
	m_game{ game }
{
	this->SetBackgroundColour(parent->GetBackgroundColour());
	this->SetFont(parent->GetFont());
	const Scooter& scootref = m_game.GetTabbyGuy().GetScooter();

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxHORIZONTAL };
	wxBoxSizer* leftCol = new wxBoxSizer{ wxVERTICAL };
	wxBoxSizer* rightCol = new wxBoxSizer{ wxVERTICAL };

	// --- COLONNA SINISTRA --------------------------------------------------------------

	// 1. CONCESSIONARIO E VENDITA
	wxPanel* pnlCompravendita = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerCompravendita = new wxBoxSizer{ wxVERTICAL };

	wxButton* btnConcessionario = new wxButton(pnlCompravendita, wxID_ANY, "Concessionario", wxDefaultPosition, wxSize(-1, 45));
	btnConcessionario->Bind(wxEVT_BUTTON, &DlgScooter::OnConcessionario, this);

	wxButton* btnVendi = new wxButton(pnlCompravendita, wxID_ANY, "Vendi Scooter", wxDefaultPosition, wxSize(-1, 45));
	btnVendi->Bind(wxEVT_BUTTON, &DlgScooter::OnVendi, this);

	sizerCompravendita->Add(btnConcessionario, 0, wxEXPAND | wxALL, 5);
	sizerCompravendita->Add(btnVendi, 0, wxEXPAND | wxALL, 5);
	pnlCompravendita->SetSizer(sizerCompravendita);
	// TODO: SOSTITUISCI STRETCH SPACER CON PANNELLO IMMAGINE
	leftCol->AddStretchSpacer();
	leftCol->Add(pnlCompravendita, 0, wxEXPAND | wxALL, 5);

	// 2. MODIFICHE
	wxPanel* pnlModifiche = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerModifiche = new wxBoxSizer{ wxVERTICAL };

	wxButton* btnTrucca = new wxButton(pnlModifiche, wxID_ANY, "Trucca scooter", wxDefaultPosition, wxSize(-1, 45));
	wxButton* btnRipara = new wxButton(pnlModifiche, wxID_ANY, "Ripara scooter", wxDefaultPosition, wxSize(-1, 45));

	btnTrucca->Bind(wxEVT_BUTTON, &DlgScooter::OnTrucca, this);
	btnRipara->Bind(wxEVT_BUTTON, &DlgScooter::OnRipara, this);

	sizerModifiche->Add(btnTrucca, 0, wxEXPAND | wxALL, 5);
	sizerModifiche->Add(btnRipara, 0, wxEXPAND | wxALL & ~wxTOP, 5);
	pnlModifiche->SetSizer(sizerModifiche);
	leftCol->Add(pnlModifiche, 0, wxEXPAND | wxALL, 5);

	// 3. INFO E OK
	wxPanel* pnlBottom = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };

	sizerBottom->Add(new wxStaticText(pnlBottom, wxID_ANY, "[IMG]"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	m_lblSoldi = new wxStaticText(pnlBottom, wxID_ANY, ""); // Inizializzato vuoto, ci pensa AggiornaInterfaccia
	sizerBottom->Add(m_lblSoldi, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	sizerBottom->AddStretchSpacer();
	sizerBottom->Add(new wxButton(pnlBottom, wxID_OK, "OK", wxDefaultPosition, wxSize(60, 50)), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	pnlBottom->SetSizer(sizerBottom);
	leftCol->Add(pnlBottom, 0, wxEXPAND | wxALL, 5);

	// --- COLONNA DESTRA (Statistiche) ----------------------------------------------------------------------------
	wxPanel* pnlStats = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerStats = new wxBoxSizer{ wxVERTICAL };

	sizerStats->Add(new wxStaticText(pnlStats, wxID_ANY, "Statistiche scooter"), 0, wxALIGN_CENTER_HORIZONTAL);

	// Nome Scooter
	m_lblNome = new wxStaticText(pnlStats, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER | wxBORDER_SUNKEN);
	sizerStats->Add(m_lblNome, 0, wxEXPAND | wxALL & ~wxTOP, 10);

	// GRIGLIA DATI
	wxFlexGridSizer* gridStats = new wxFlexGridSizer{ 2, 5, 10 };

	// Salviamo i puntatori restituiti da AddStat per poterli aggiornare dopo
	m_lblVelocita = AddStat(pnlStats, gridStats, "Velocità:", "");
	m_lblBenza = AddStat(pnlStats, gridStats, "Benzina:", "");
	m_lblStato = AddStat(pnlStats, gridStats, "Stato:", "");

	m_lblMarmitta = new wxStaticText{ pnlStats, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN};
	m_lblCarburatore = new wxStaticText{ pnlStats, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN};
	m_lblCilindrata = new wxStaticText{ pnlStats, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN};
	m_lblFiltro = new wxStaticText{ pnlStats, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN};
	sizerStats->Add(gridStats, 0, wxALL & ~wxTOP, 5);

	sizerStats->AddSpacer(15);
	sizerStats->Add(new wxStaticText(pnlStats, wxID_ANY, "Componenti"), 0, wxALIGN_CENTER);
	sizerStats->Add(m_lblMarmitta, 0, wxEXPAND | wxALL, 5);
	sizerStats->Add(m_lblCarburatore, 0, wxEXPAND | wxALL, 5);
	sizerStats->Add(m_lblCilindrata, 0, wxEXPAND | wxALL, 5);
	sizerStats->Add(m_lblFiltro, 0, wxEXPAND | wxALL, 5);


	// BOTTONI IN BASSO A DESTRA
	sizerStats->AddStretchSpacer();

	m_btnUsa = new wxButton(pnlStats, wxID_ANY, "---", wxDefaultPosition, wxSize(-1, 45));
	wxButton* btnBenza = new wxButton(pnlStats, wxID_ANY, "Fai benza", wxDefaultPosition, wxSize(-1, 45));

	m_btnUsa->Bind(wxEVT_BUTTON, &DlgScooter::OnUsa, this);
	btnBenza->Bind(wxEVT_BUTTON, &DlgScooter::OnFaiBenza, this);

	sizerStats->Add(m_btnUsa, 0, wxEXPAND | wxALL & ~wxBOTTOM, 5);
	sizerStats->Add(btnBenza, 0, wxEXPAND | wxALL & ~wxTOP, 5);

	pnlStats->SetSizer(sizerStats);
	rightCol->Add(pnlStats, 1, wxEXPAND | wxALL, 5);

	// --- CHIUSURA LAYOUT ---
	mainSizer->Add(leftCol, 0, wxEXPAND | wxALL, 5);
	mainSizer->Add(rightCol, 0, wxEXPAND | wxALL, 5);

	this->SetSizerAndFit(mainSizer);
	// Popola tutti i testi
	this->AggiornaInterfaccia();
}

void DlgScooter::OnConcessionario(wxCommandEvent& event)
{
	if (m_game.TriggerNegozio(CategoriaOggetto::SCOOTER))
	{
		DlgConcessionario dlg(this, m_game);
		dlg.ShowModal();
	}

	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
}

void DlgScooter::OnVendi(wxCommandEvent& event)
{
	if (m_game.TriggerNegozio(CategoriaOggetto::GENERICO))
	{
		m_game.AzioneVendiScooter();
	}

	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
}

void DlgScooter::OnTrucca(wxCommandEvent& event)
{
	if (m_game.TriggerMeccanico())
	{
		DlgNegozio dlg{ this, m_game, m_game.GetMeccanico() };
		dlg.ShowModal();
	}

	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
}

void DlgScooter::OnRipara(wxCommandEvent& event)
{
	m_game.AzioneRiparaScooter();
	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
}

void DlgScooter::OnFaiBenza(wxCommandEvent& event)
{
	m_game.AzioneFaiBenza();
	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
}

void DlgScooter::OnUsa(wxCommandEvent& event)
{
	m_game.AzioneUsaScooter();
	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
}

wxStaticText* DlgScooter::AddStat(wxWindow* parent, wxSizer* sizer, wxString label, wxString value)
{
	sizer->Add(new wxStaticText(parent, wxID_ANY, label, wxDefaultPosition, wxSize(-1, 25)));
	wxStaticText* valueBox = new wxStaticText{ parent, wxID_ANY, value, wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN };
	valueBox->SetMinSize(wxSize(300, -1));
	sizer->Add(valueBox, 0, wxEXPAND);
	return valueBox;
}

void DlgScooter::AggiornaInterfaccia()
{
	const Scooter& s = m_game.GetTabbyGuy().GetScooter();

	// Aggiorna Soldi
	m_lblSoldi->SetLabel("Soldi " + m_game.GetSoldiStr(m_game.GetTabbyGuy().GetSoldi()));

	// Aggiorna Nome (con spazi padding per estetica)
	m_lblNome->SetLabel("  " + s.GetNome() + "  ");

	if (m_game.GetTabbyGuy().HaScooter())
	{
		// Aggiorna Statistiche
		if (s.GetAttivita() != Attivita::IN_GIRO)
		{
			m_lblVelocita->SetLabel("("+s.GetAttivitaStr(false)+")");
		}
		else
			m_lblVelocita->SetLabel(std::to_string(s.GetVelocita())+" km/h");

		m_lblBenza->SetLabel(wxString::Format("%.1f l", s.GetBenza()));
		m_lblStato->SetLabel(std::to_string(s.GetStato())+"%");
		m_lblMarmitta->SetLabel(s.GetMarmitta().GetNome());
		m_lblCarburatore->SetLabel(s.GetCarburatore().GetNome());
		m_lblCilindrata->SetLabel(s.GetCilindrata().GetNome());
		m_lblFiltro->SetLabel(s.GetFiltro().GetNome());
	}
	else    // Bisogna azzerare i label in modo da cancellare le informazioni dopo aver venduto lo scooter
	{
		m_lblVelocita->SetLabel("");
		m_lblBenza->SetLabel("");
		m_lblStato->SetLabel("");
		m_lblMarmitta->SetLabel("");
		m_lblCarburatore->SetLabel("");
		m_lblCilindrata->SetLabel("");
		m_lblFiltro->SetLabel("");
	}

	if (m_game.GetTabbyGuy().GetScooter().GetAttivita() != Attivita::PARCHEGGIATO)
		m_btnUsa->SetLabel("Parcheggia scooter");
	else
		m_btnUsa->SetLabel("Usa scooter");

	this->Fit();
	this->Layout();
}

// DIALOG SCUOLA
DlgScuola::DlgScuola(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Scuola", wxDefaultPosition, wxDefaultSize },
	m_game{ game }, m_materiaIndex{0}
{
	this->SetBackgroundColour(parent->GetBackgroundColour());
	this->SetFont(parent->GetFont());

	const Scuola& scuolaref = m_game.GetTabbyGuy().GetScuola();

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
		// Sfrutto una funzione lambda per associare la materia che viene selezionata ai bottoni del pannello a destra, perché sono troppo intelligente...
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
	const Scuola& scuolaref = m_game.GetTabbyGuy().GetScuola();
	m_btnStudia->SetLabel("Studia " + scuolaref.m_materie[m_materiaIndex].GetNome());
	m_btnMinaccia->SetLabel("Minaccia il prof di " + scuolaref.m_materie[m_materiaIndex].GetNome());
	m_btnCorrompi->SetLabel("Corrompi il prof di " + scuolaref.m_materie[m_materiaIndex].GetNome());

	for (int i = 0; i < scuolaref.m_materie.size(); i++)
		m_lblVoti[i]->SetLabel(wxString::Format("%d", scuolaref.m_materie[i].GetVoto()));

	m_lblStudio->SetLabel(wxString::Format("Profitto scolastico %d/100", m_game.GetTabbyGuy().GetStudio()));
	m_lblRep->SetLabel(wxString::Format("Reputazione %d/100", m_game.GetTabbyGuy().GetRep()));

	this->Layout();
}

// DIALOG COMPAGNIA
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

// DIALOG EVENTI/MESSAGGI
DlgEvento::DlgEvento(wxWindow* parent, Messaggio& eventoDati)
	: wxDialog{ parent, wxID_ANY, eventoDati.m_titolo, wxDefaultPosition, wxDefaultSize, wxCAPTION | wxSTAY_ON_TOP }	// Stile: CAPTION (barra titolo) ma niente tasto X (CLOSE_BOX) così l'utente è obbligato a premere i bottoni
{
	this->SetFont(parent->GetFont());
	this->CentreOnParent();	// Appare al centro della finestra padre
	Messaggio& msgref = eventoDati;
	// Icona windows
	wxArtID artId = wxART_INFORMATION;
	if (msgref.m_tipo == TipoMsg::ERRORE) artId = wxART_ERROR;
	if (msgref.m_tipo == TipoMsg::AVVISO) artId = wxART_WARNING;
	if (msgref.m_tipo == TipoMsg::SCELTA) artId = wxART_QUESTION;
	if (msgref.m_tipo == TipoMsg::SUCCESSO) artId = wxART_TICK_MARK;
	// TODO: implementa immagine

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	wxBoxSizer* sizerText = new wxBoxSizer{ wxHORIZONTAL };

	wxStaticBitmap* icon = new wxStaticBitmap(this, wxID_ANY,
		wxArtProvider::GetBitmap(artId, wxART_MESSAGE_BOX));
	sizerText->Insert(0, icon, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);

	wxStaticText* lblTesto = new wxStaticText{ this, wxID_ANY, msgref.m_testo, wxDefaultPosition, wxSize(-1, -1), wxALIGN_LEFT };

	// Manda a capo il testo automaticamente
	lblTesto->Wrap(600);
	sizerText->Add(lblTesto, 1, wxALIGN_CENTER | wxALL, 15);

	mainSizer->Add(sizerText);
	// Linea separatrice
	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

	// BOTTONI
	wxBoxSizer* sizerBtn = new wxBoxSizer(wxHORIZONTAL);

	if (msgref.m_tipo != TipoMsg::SCELTA)
	{
		// Caso semplice: Solo OK
		// wxID_OK chiude automaticamente il dialogo ritornando wxID_OK
		wxButton* btnOk = new wxButton(this, wxID_OK, "OK");
		sizerBtn->Add(btnOk, 0, wxALL, 10);
	}
	else
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
		sizerBtn->Add(btnSi, 0, wxALL, 10);
		sizerBtn->Add(btnNo, 0, wxALL, 10);
	}

	mainSizer->Add(sizerBtn, 0, wxALIGN_CENTER | wxBOTTOM, 5);
	this->SetSizerAndFit(mainSizer);
}

// DIALOG FAMIGLIA
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
	m_lblSoldi->SetLabel("Soldi " + m_game.GetSoldiStr(m_game.GetTabbyGuy().GetSoldi()));
	m_lblPaghetta->SetLabel("Paghetta " + m_game.GetSoldiStr(m_game.GetTabbyGuy().GetPaghetta()));
	this->Fit();
	this->Layout();
}

// DIALOG LAVORO
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
	m_lblImpegno->SetLabel("Impegno " + std::to_string(guy.GetCarriera().GetImpegno()) + "/100");
	m_lblStipendio->SetLabel("Stipendio: " + m_game.GetSoldiStr(guy.GetCarriera().GetStipendio()));
	m_lblSoldi->SetLabel("Soldi: " + m_game.GetSoldiStr(guy.GetSoldi()));
	this->Fit();
	this->Layout();
}

// DIALOG OFFERTA LAVORATIVA
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

// DIALOG QUESTIONARIO DI LAVORO
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

// DIALOG ELENCO DITTE
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
	std::vector<Ditta>& ditte = m_game.GetDitte();
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
		if (d.m_fatturato <= 0)
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
	std::vector<Ditta>& ditte = m_game.GetDitte();
	if (dittaIndex >= 0 && dittaIndex < ditte.size())
	{
		// Creiamo e mostriamo la finestra INFO (senza offerta lavoro)
		DlgInfoDitta dlg(this, m_game, ditte[dittaIndex]);
		dlg.ShowModal();
	}
}

// DIALOG INFORMAZIONI DITTA
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
	if (ditta.m_fatturato <= 0)
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

// DIALOG DISCOTECA
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
	std::vector<Disco>& discoteche = m_game.GetDiscoteche();
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

	const Disco& d = m_game.GetDiscoteche()[m_selectedIndex];

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

	const Disco& d = m_game.GetDiscoteche()[m_selectedIndex];
	// Chiama la funzione logica che hai già in TabbyGame
	m_game.AzionePagaDisco(m_selectedIndex);
	ManifestaEventi(this, m_game);

	// Chiudi la finestra
	EndModal(wxID_OK);
}

// DIALOG TIPA
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

	m_lblFama->SetLabel("Figosità " + std::to_string(m_game.GetTabbyGuy().GetFama()) + "/100");

	this->Fit();
	this->Layout();
}

// DIALOG INCONTRO TIPA
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

// DIALOG ELENCO NEGOZI
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
	std::vector<Negozio>& negozi = m_game.GetNegozi();
	for (int i = 0; i < negozi.size(); i++)
	{
		wxButton* btnNegozio = new wxButton{ pnlBody, wxID_ANY, negozi[i].m_nome, wxDefaultPosition, wxSize(-1, 40), wxEXPAND};
		// Sfruttiamo una funzione lambda per associare in modo dinamico il bottone al negozio selezionato
		sizerBody->Add(btnNegozio, 0, wxEXPAND | wxRIGHT | wxLEFT, 5);
		btnNegozio->Bind(wxEVT_BUTTON, [this, negozi, i](wxCommandEvent&) {

			if(m_game.TriggerNegozio(negozi[i].m_merce))
			{
				if(negozi[i].m_merce == CategoriaOggetto::CONSUMABILE)
					m_game.PlaySound(203);
				else if (m_game.GenRandomInt(0, 1) == 1)
					m_game.PlaySound(204);
				else
					m_game.PlaySound(205);
				ManifestaEventi(this, m_game);

				DlgNegozio dlg{ this, m_game, negozi[i] };
				dlg.ShowModal();
				AggiornaInterfaccia();
			}
			ManifestaEventi(this, m_game);
		});
	}

	// PALESTRA E TELEFONINO
	wxButton* btnPalestra = new wxButton{ pnlBody, wxID_ANY, "Palestra", wxDefaultPosition, wxSize(-1,40), wxEXPAND };
	wxButton* btnTelefonino = new wxButton{ pnlBody, wxID_ANY, "Telefonino", wxDefaultPosition, wxSize(-1,40), wxEXPAND };
	sizerBody->Add(btnPalestra, 0, wxEXPAND | wxRIGHT | wxLEFT, 5);
	sizerBody->Add(btnTelefonino, 0, wxEXPAND | wxRIGHT | wxLEFT, 5);

	btnPalestra->Bind(wxEVT_BUTTON, &DlgElencoNegozi::OnPalestra, this);
	btnTelefonino->Bind(wxEVT_BUTTON, &DlgElencoNegozi::OnTelefonino, this);


	sizerBody->AddSpacer(5);

	pnlBody->SetSizer(sizerBody);
	mainSizer->Add(pnlBody, 1, wxEXPAND | wxALL, 5);

	m_lblSoldi = new wxStaticText{ pnlBottom, wxID_ANY, "---" };
	sizerBottom->Add(m_lblSoldi, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
	sizerBottom->AddStretchSpacer();
	wxButton* btnChiudi = new wxButton{ pnlBottom, wxID_CANCEL, "Chiudi", wxDefaultPosition, wxSize(120, 40)};
	sizerBottom->Add(btnChiudi, 0, wxALL, 10);

	pnlBottom->SetSizer(sizerBottom);
	mainSizer->Add(pnlBottom, 0, wxEXPAND | wxALL, 5);

	this->SetSizerAndFit(mainSizer);
	this->AggiornaInterfaccia();
}

void DlgElencoNegozi::AggiornaInterfaccia()
{
	m_lblSoldi->SetLabel("Soldi: " + m_game.GetSoldiStr(m_game.GetTabbyGuy().GetSoldi()));
	this->Fit();
	this->Layout();
}

void DlgElencoNegozi::OnPalestra(wxCommandEvent& event)
{
	if (m_game.TriggerPalestra())
	{
		DlgPalestra dlg{ this, m_game };
		dlg.ShowModal();
		this->AggiornaInterfaccia();
	}
	else
		ManifestaEventi(this, m_game);
}

void DlgElencoNegozi::OnTelefonino(wxCommandEvent& event)
{
	DlgTelefono dlg{ this, m_game };
	dlg.Centre();
	dlg.ShowModal();
	this->AggiornaInterfaccia();
}

PnlProdotto::PnlProdotto(wxWindow* parent, DlgNegozio* mainDlg, TabbyGame& game, const Acquistabile& prod)
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
	wxStaticText* lblDesc = new wxStaticText(this, wxID_ANY, m_prodotto.GetDescrizione());
	wxFont fDesc = lblDesc->GetFont();
	fDesc.SetPointSize(10);
	lblDesc->SetFont(fDesc);
	lblDesc->Wrap(350); // Manda a capo il testo per stare nella larghezza rimanente

	bodySizer->Add(lblDesc, 0, wxEXPAND | wxALL, 5);

	mainSizer->Add(bodySizer, 0, wxEXPAND | wxALL, 5); // Proportion 0 qui, lascia che sia il contenuto a spingere
	
	// 3. DATI E BOTTONE
	mainSizer->AddStretchSpacer();
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
	m_parentDlg->EndModal(wxID_ANY);	// Chiudiamo la finestra del negozio
}

// DIALOG NEGOZIO
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

	for (const auto& prodPtr : m_negozio.m_catalogo) {
		PnlProdotto* scheda = new PnlProdotto(scrollWin, this, m_game, *prodPtr);

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
	m_lblSoldi->SetLabel("Soldi: " + m_game.GetSoldiStr(m_game.GetTabbyGuy().GetSoldi()));
	this->Fit();
	this->Layout();
}

// DIALOG PALESTRA
DlgPalestra::DlgPalestra(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Palestra", wxDefaultPosition, wxDefaultSize},
	m_game{ game }
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour());

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };
	wxBoxSizer* sizerBody = new wxBoxSizer{ wxHORIZONTAL };

	wxPanel* pnlBottom = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };

	// FOTO
	wxPanel* pnlFoto = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxSize(200,300), wxBORDER_SUNKEN};
	wxBoxSizer* sizerFoto = new wxBoxSizer{ wxVERTICAL };

	pnlFoto->SetSizer(sizerFoto);
	sizerBody->Add(pnlFoto, 0, wxEXPAND | wxALL, 5);
	sizerBody->Add(new wxStaticLine(this), 0, wxEXPAND | wxRIGHT | wxLEFT, 5);
	// BOTTONI
	wxPanel* pnlButtons = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN};
	wxBoxSizer* sizerButtons = new wxBoxSizer{ wxVERTICAL };

	wxButton* btnVaiPalestra = new wxButton{ pnlButtons, wxID_ANY, "Vai in palestra", wxDefaultPosition, wxSize(-1, 40) };
	sizerButtons->Add(btnVaiPalestra, 0, wxEXPAND | wxALL, 5);
	btnVaiPalestra->Bind(wxEVT_BUTTON, &DlgPalestra::OnVaiPalestra, this);

	sizerButtons->Add(new wxStaticText(pnlButtons, wxID_ANY, "Prezzi abbonamenti palestra"), 0, wxALIGN_CENTER | wxALL & ~wxBOTTOM, 5);
	// INFORMAZIONI ABBONAMENTI
	wxPanel* pnlSub = new wxPanel{ pnlButtons, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };	// Faccio il panel solo per avere il rientro
	wxGridSizer* gridSub = new wxGridSizer{ 2, 3, 10, 10 };	// Griglia da 2 righe e 3 colonne

	gridSub->Add(new wxStaticText(pnlSub, wxID_ANY, m_game.GetSoldiStr(PALESTRA_ABB_1)), 0, wxALIGN_CENTER | wxALL & wxBOTTOM, 5);
	gridSub->Add(new wxStaticText(pnlSub, wxID_ANY, m_game.GetSoldiStr(PALESTRA_ABB_6)), 0, wxALIGN_CENTER | wxALL & wxBOTTOM, 5);
	gridSub->Add(new wxStaticText(pnlSub, wxID_ANY, m_game.GetSoldiStr(PALESTRA_ABB_12)), 0, wxALIGN_CENTER | wxALL & wxBOTTOM, 5);

	wxButton* btnMesi1 = new wxButton{ pnlSub, wxID_ANY, "1 mese" };
	wxButton* btnMesi6 = new wxButton{ pnlSub, wxID_ANY, "6 mesi" };
	wxButton* btnMesi12 = new wxButton{ pnlSub, wxID_ANY, "1 anno" };

	gridSub->Add(btnMesi1, 0, wxEXPAND | wxALL & ~wxTOP, 5);
	gridSub->Add(btnMesi6, 0, wxEXPAND | wxALL & ~wxTOP, 5);
	gridSub->Add(btnMesi12, 0, wxEXPAND | wxALL & ~wxTOP, 5);

	btnMesi1->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
		m_game.AzioneAbbonamento(1);
		ManifestaEventi(this, m_game);
		this->AggiornaInterfaccia();
		});
	btnMesi6->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
		m_game.AzioneAbbonamento(6);
		ManifestaEventi(this, m_game);
		this->AggiornaInterfaccia();
		});
	btnMesi12->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
		m_game.AzioneAbbonamento(12);
		ManifestaEventi(this, m_game);
		this->AggiornaInterfaccia();
		});

	pnlSub->SetSizer(gridSub);
	sizerButtons->Add(pnlSub, 0, wxEXPAND | wxALL, 5);

	m_lblScadenza = new wxStaticText{ pnlButtons, wxID_ANY, "---", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN | wxALIGN_CENTER_HORIZONTAL };
	sizerButtons->Add(m_lblScadenza, 0, wxEXPAND | wxALL, 5);

	sizerButtons->Add(new wxStaticLine(pnlButtons), 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

	wxButton* btnLampada = new wxButton{ pnlButtons, wxID_ANY, "Lampada ("+m_game.GetSoldiStr(PREZZO_LAMPADA)+")"};
	sizerButtons->Add(btnLampada, 0, wxALL, 5);
	btnLampada->Bind(wxEVT_BUTTON, &DlgPalestra::OnLampada, this);

	m_lblPelle = new wxStaticText{ pnlButtons, wxID_ANY, "---", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN | wxALIGN_CENTER_HORIZONTAL };
	sizerButtons->Add(m_lblPelle, 0, wxEXPAND | wxALL, 5);

	pnlButtons->SetSizer(sizerButtons);

	sizerBody->Add(pnlButtons, 1, wxEXPAND | wxALL, 5);
	mainSizer->Add(sizerBody, 0, wxEXPAND | wxALL, 5);

	m_lblSoldi = new wxStaticText{ pnlBottom, wxID_ANY, "---" };
	sizerBottom->Add(m_lblSoldi, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	sizerBottom->AddSpacer(50);
	m_lblFama = new wxStaticText{ pnlBottom, wxID_ANY, "---" };
	sizerBottom->Add(m_lblFama, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	sizerBottom->AddStretchSpacer();
	wxButton* btnEsci = new wxButton(pnlBottom, wxID_CANCEL, "Esci", wxDefaultPosition, wxSize(100, 40));
	sizerBottom->Add(btnEsci, 0, wxALL, 5);

	pnlBottom->SetSizer(sizerBottom);
	mainSizer->Add(pnlBottom, 0, wxEXPAND | wxALL, 10);

	this->SetSizerAndFit(mainSizer);
	this->AggiornaInterfaccia();
}

void DlgPalestra::AggiornaInterfaccia()
{
	if (!m_game.PalestraAttiva())
		m_lblScadenza->SetLabel("Nessun abbonamento");
	else
	{
		std::string s = "Scadenza: " + 
			std::to_string(m_game.GetScadenzaPal().GetDay()) + " " + 
			m_game.GetScadenzaPal().GetMonthStr() + " " + 
			std::to_string(m_game.GetScadenzaPal().GetYear());
		m_lblScadenza->SetLabel(s);
	}
	m_lblPelle->SetLabel(m_game.GetTabbyGuy().GetPelleStr());
	m_lblSoldi->SetLabel("Soldi " + m_game.GetSoldiStr(m_game.GetTabbyGuy().GetSoldi()));
	m_lblFama->SetLabel("Figosità " + std::to_string(m_game.GetTabbyGuy().GetFama()) + "/100");

	this->Fit();
	this->Layout();
}

void DlgPalestra::OnVaiPalestra(wxCommandEvent& event)
{
	if (m_game.TriggerPalestra())
		m_game.AzioneVaiPalestra();
	
	this->AggiornaInterfaccia();
	ManifestaEventi(this, m_game);
}

void DlgPalestra::OnLampada(wxCommandEvent& event)
{
	if (m_game.TriggerPalestra())
		m_game.AzioneLampada();

	this->AggiornaInterfaccia();
	ManifestaEventi(this, m_game);
}

// DIALOG TELEFONO
DlgTelefono::DlgTelefono(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Telefonino" },
	m_game{ game }
{
	this->SetFont(parent->GetFont());
	// Colore grigio plastica per il corpo del telefono
	this->SetBackgroundColour(parent->GetBackgroundColour());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizerBody = new wxBoxSizer{ wxHORIZONTAL };

	// BOTTONI A SINISTRA
	wxPanel* pnlButtons = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerButtons = new wxBoxSizer{ wxVERTICAL };

	wxButton* btnCompraTel = new wxButton{ pnlButtons, wxID_ANY, "Compra telefonino", wxDefaultPosition, wxSize(-1, 40) };
	wxButton* btnVendiTel = new wxButton{ pnlButtons, wxID_ANY, "Vendi telefonino", wxDefaultPosition, wxSize(-1, 40) };
	wxButton* btnRicarica = new wxButton{ pnlButtons, wxID_ANY, "Ricarica telefonino", wxDefaultPosition, wxSize(-1, 40) };

	sizerButtons->Add(btnCompraTel, 0, wxEXPAND | wxALL, 5);
	sizerButtons->Add(btnVendiTel, 0, wxEXPAND | wxALL, 5);
	sizerButtons->Add(new wxStaticLine(pnlButtons), 0, wxEXPAND | wxTOP | wxBOTTOM, 5);
	sizerButtons->AddSpacer(10);
	sizerButtons->Add(btnRicarica, 0, wxEXPAND | wxALL, 5);

	btnCompraTel->Bind(wxEVT_BUTTON, &DlgTelefono::OnCompraTel, this);
	btnVendiTel->Bind(wxEVT_BUTTON, &DlgTelefono::OnVendiTel, this);
	btnRicarica->Bind(wxEVT_BUTTON, &DlgTelefono::OnRicarica, this);

	pnlButtons->SetSizer(sizerButtons);
	sizerBody->Add(pnlButtons, 0, wxEXPAND | wxALL, 5);

	// DATI TELEFONO
	wxPanel* pnlDati = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerDati = new wxBoxSizer{ wxVERTICAL };

	wxStaticText* lblTel = new wxStaticText{ pnlDati, wxID_ANY, "Telefono:" };
	wxStaticText* lblAbb = new wxStaticText{ pnlDati, wxID_ANY, "Abbonamento:" };
	wxStaticText* lblCred = new wxStaticText{ pnlDati, wxID_ANY, "Credito restante:" };
	m_lblNomeTel = new wxStaticText{ pnlDati, wxID_ANY, "---", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN | wxALIGN_CENTER };
	m_lblOperatore = new wxStaticText{ pnlDati, wxID_ANY, "---", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN | wxALIGN_CENTER };
	m_lblCredito = new wxStaticText{ pnlDati, wxID_ANY, "---", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN | wxALIGN_CENTER };

	sizerDati->Add(lblTel, 0, wxALIGN_CENTER_HORIZONTAL | wxALL & ~wxBOTTOM, 5);
	sizerDati->Add(m_lblNomeTel, 0, wxEXPAND | wxALL, 5);
	sizerDati->Add(lblAbb, 0, wxALIGN_CENTER_HORIZONTAL | wxALL & ~wxBOTTOM, 5);
	sizerDati->Add(m_lblOperatore, 0, wxEXPAND | wxALL, 5);
	sizerDati->Add(lblCred, 0, wxALIGN_CENTER_HORIZONTAL | wxALL & ~wxBOTTOM, 5);
	sizerDati->Add(m_lblCredito, 0, wxEXPAND | wxALL, 5);

	pnlDati->SetSizer(sizerDati);
	sizerBody->Add(pnlDati, 0, wxEXPAND | wxALL, 5);
	// CHIUDIAMO SIZERBODY
	mainSizer->Add(sizerBody, 0, wxEXPAND | wxALL, 5);

	// SOLDI E TASTO OK
	wxPanel* pnlBottom = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };

	// TODO: ADD ICON
	m_lblSoldi = new wxStaticText{ pnlBottom, wxID_ANY, "---" };
	wxButton* btnOk = new wxButton{ pnlBottom, wxID_OK, "OK", wxDefaultPosition, wxSize(60, 40) };

	sizerBottom->Add(m_lblSoldi, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	sizerBottom->AddStretchSpacer();
	sizerBottom->Add(btnOk, 0, wxALL, 5);

	pnlBottom->SetSizer(sizerBottom);
	mainSizer->Add(pnlBottom, 0, wxEXPAND | wxALL, 10);

	this->SetSizerAndFit(mainSizer);
	this->AggiornaInterfaccia();
}

void DlgTelefono::AggiornaInterfaccia()
{
	m_lblNomeTel->SetLabel(m_game.GetTabbyGuy().GetTelefono().GetNome());
	// TODO: SISTEMA STA MERDA
	m_lblOperatore->SetLabel(m_game.GetTabbyGuy().GetTelefono().GetAbbonamento().GetNome());
	m_lblCredito->SetLabel(m_game.GetSoldiStr(m_game.GetTabbyGuy().GetTelefono().GetCredito()));
	m_lblSoldi->SetLabel("Soldi: "+m_game.GetSoldiStr(m_game.GetTabbyGuy().GetSoldi()));

	this->Fit();
	this->Layout();
}

void DlgTelefono::OnCompraTel(wxCommandEvent& event)
{
	if (m_game.TriggerNegozio(m_game.GetTelefonia().m_merce))
	{
		DlgNegozio dlg{ this, m_game, m_game.GetTelefonia() };
		dlg.ShowModal();

		this->AggiornaInterfaccia();
	}
	
	ManifestaEventi(this, m_game);
}

void DlgTelefono::OnVendiTel(wxCommandEvent& event)
{
	if (m_game.TriggerNegozio(m_game.GetTelefonia().m_merce))
		m_game.AzioneVendiTelefono();
	
	ManifestaEventi(this, m_game);
	this->AggiornaInterfaccia();
}

void DlgTelefono::OnRicarica(wxCommandEvent& event)
{
	if (m_game.TriggerNegozio(m_game.GetTelefonia().m_merce))
	{
		DlgRicariche dlg{ this, m_game };
		dlg.ShowModal();
		this->AggiornaInterfaccia();
	}
	else
		ManifestaEventi(this, m_game);
	
}

// DIALOG RICARICHE
DlgRicariche::DlgRicariche(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Negozio Telefonia", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE }, // Tolto wxRESIZE_BORDER
	m_game{ game }
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	// --- INFO UTENTE (Alto) ---
	wxPanel* pnlInfo = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
	wxBoxSizer* infoSizer = new wxBoxSizer(wxVERTICAL);

	m_lblOperatoreAttuale = new wxStaticText(pnlInfo, wxID_ANY, "---");
	m_lblCreditoAttuale = new wxStaticText(pnlInfo, wxID_ANY, "---");

	// Grassetto per l'operatore
	wxFont fBold = m_lblOperatoreAttuale->GetFont();
	fBold.SetWeight(wxFONTWEIGHT_BOLD);
	m_lblOperatoreAttuale->SetFont(fBold);

	infoSizer->Add(m_lblOperatoreAttuale, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	infoSizer->Add(m_lblCreditoAttuale, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	pnlInfo->SetSizer(infoSizer);

	mainSizer->Add(pnlInfo, 0, wxEXPAND | wxALL, 10);

	// --- AREA SCORREVOLE PER GLI ABBONAMENTI ---
	wxScrolledWindow* scrollWin = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxSize(580, 400), wxBORDER_NONE);
	scrollWin->SetScrollRate(0, 20);

	wxBoxSizer* scrollSizer = new wxBoxSizer(wxVERTICAL);

	// Recuperiamo la lista abbonamenti
	const std::vector<Abbonamento>& abbonamenti = m_game.GetAbbonamenti();

	for (int i = 0; i < abbonamenti.size(); ++i)
	{
		const Abbonamento& abb = abbonamenti[i];

		// Riquadro per ogni operatore
		wxStaticBoxSizer* groupSizer = new wxStaticBoxSizer(wxVERTICAL, scrollWin, abb.GetNome());

		// Contenitore Orizzontale: [LOGO] | [BOTTONI]
		wxBoxSizer* contentSizer = new wxBoxSizer(wxHORIZONTAL);

		// 1. Placeholder Immagine (Sinistra)
		wxPanel* imgPlaceholder = new wxPanel(scrollWin, wxID_ANY, wxDefaultPosition, wxSize(80, 80), wxBORDER_SIMPLE);
		imgPlaceholder->SetBackgroundColour(wxColor(200, 200, 200)); // Grigio chiaro

		// Centriamo la scritta LOGO nel placeholder
		wxBoxSizer* imgSizer = new wxBoxSizer(wxVERTICAL);
		imgSizer->AddStretchSpacer();
		imgSizer->Add(new wxStaticText(imgPlaceholder, wxID_ANY, "LOGO"), 0, wxALIGN_CENTER);
		imgSizer->AddStretchSpacer();
		imgPlaceholder->SetSizer(imgSizer);

		contentSizer->Add(imgPlaceholder, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

		// 2. Colonna Bottoni (Destra)
		wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

		// Tasto Attivazione
		wxString labelAttiva = "Nuova Scheda (" + m_game.GetSoldiStr(abb.GetCostoAttivazione()) + ")";
		wxButton* btnAttiva = new wxButton(scrollWin, wxID_ANY, labelAttiva, wxDefaultPosition, wxSize(-1, 30));
		btnAttiva->SetForegroundColour(wxColor(0, 0, 150));

		btnAttiva->Bind(wxEVT_BUTTON, [this, i](wxCommandEvent&) {
			if (m_game.TriggerNegozio(CategoriaOggetto::TELEFONO))
				m_game.AzioneAttivaSim(i);

			ManifestaEventi(this, m_game);
			this->AggiornaInterfaccia();
			});

		rightSizer->Add(btnAttiva, 0, wxEXPAND | wxBOTTOM, 5);

		// Griglia Ricariche
		wxGridSizer* ricaricheGrid = new wxGridSizer(2, 4, 5); // 2 righe, gap 4px e 5px

		const std::vector<long long>& tagli = abb.GetRicariche();
		for (long long taglio : tagli)
		{
			// Abbreviamo la label per farla stare nei bottoni piccoli
			wxString labelRic = m_game.GetSoldiStr(taglio);
			wxButton* btnRic = new wxButton(scrollWin, wxID_ANY, labelRic);

			std::string nomeOp = abb.GetNome();
			btnRic->Bind(wxEVT_BUTTON, [this, taglio, nomeOp](wxCommandEvent&) {
				m_game.AzioneRicarica(taglio, nomeOp);
				ManifestaEventi(this, m_game);
				this->AggiornaInterfaccia();
				});

			ricaricheGrid->Add(btnRic, 0, wxEXPAND);
		}

		rightSizer->Add(ricaricheGrid, 0, wxEXPAND);

		// Aggiungiamo la colonna di destra al contenitore orizzontale
		contentSizer->Add(rightSizer, 1, wxEXPAND | wxLEFT, 10); // proportion 1 per espandersi

		// Aggiungiamo il contentSizer al gruppo
		groupSizer->Add(contentSizer, 1, wxEXPAND | wxALL, 5);

		// Aggiungiamo il gruppo alla lista principale
		scrollSizer->Add(groupSizer, 0, wxEXPAND | wxALL, 5);
	}

	scrollWin->SetSizer(scrollSizer);
	mainSizer->Add(scrollWin, 1, wxEXPAND | wxALL, 5);

	// --- BARRA INFERIORE (Soldi + Chiudi) ---
	wxPanel* pnlBottom = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_RAISED);
	wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);

	m_lblSoldi = new wxStaticText(pnlBottom, wxID_ANY, "---");
	wxFont fSoldi = m_lblSoldi->GetFont();
	fSoldi.SetWeight(wxFONTWEIGHT_BOLD);
	m_lblSoldi->SetFont(fSoldi);

	wxButton* btnChiudi = new wxButton(pnlBottom, wxID_CANCEL, "Chiudi", wxDefaultPosition, wxSize(-1, 40));

	bottomSizer->Add(m_lblSoldi, 0, wxALIGN_CENTER_VERTICAL | wxALL, 15);
	bottomSizer->AddStretchSpacer();
	bottomSizer->Add(btnChiudi, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);

	pnlBottom->SetSizer(bottomSizer);
	mainSizer->Add(pnlBottom, 0, wxEXPAND);

	this->SetSizerAndFit(mainSizer);
	this->Centre();

	AggiornaInterfaccia();
}

void DlgRicariche::AggiornaInterfaccia()
{
	// Aggiorna Soldi
	m_lblSoldi->SetLabel("Soldi: " + m_game.GetSoldiStr(m_game.GetTabbyGuy().GetSoldi()));

	// Aggiorna Info Telefono
	if (m_game.GetTabbyGuy().GetTelefono().HaSim())
	{
		const Telefono& tel = m_game.GetTabbyGuy().GetTelefono();
		// Se ha una sim
		if (tel.HaSim())
		{
			m_lblOperatoreAttuale->SetLabel("Operatore: " + tel.GetAbbonamento().GetNome());
			m_lblCreditoAttuale->SetLabel("Credito: " + m_game.GetSoldiStr(tel.GetCredito()));
		}
		else
		{
			m_lblOperatoreAttuale->SetLabel("Nessuna SIM inserita");
			m_lblCreditoAttuale->SetLabel("");
		}
	}
	else
	{
		m_lblOperatoreAttuale->SetLabel("Non hai sim attive !");
		m_lblCreditoAttuale->SetLabel("");
	}

	this->Fit();
	this->Layout();
}

// DIALOG CONCESSIONARIO
DlgConcessionario::DlgConcessionario(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Concessionario", wxDefaultPosition, wxDefaultSize },
	m_game{ game }
{
	this->SetBackgroundColour(parent->GetBackgroundColour());
	this->SetFont(parent->GetFont());

	m_catalogoPtr = m_game.GetConcessionario().m_catalogo;

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizerContent = new wxBoxSizer(wxHORIZONTAL);

	// --- COLONNA SINISTRA: LISTA SCOOTER ---
	wxScrolledWindow* scrollList = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 350), wxBORDER_SUNKEN);
	scrollList->SetScrollRate(0, 20);
	wxBoxSizer* sizerList = new wxBoxSizer(wxVERTICAL);

	for (size_t i = 0; i < m_catalogoPtr.size(); ++i)
	{
		Scooter* s = static_cast<Scooter*>(m_catalogoPtr[i]);

		wxPanel* pnlItem = new wxPanel(scrollList, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_RAISED);
		wxBoxSizer* sizerItem = new wxBoxSizer(wxHORIZONTAL);

		// Immagine piccola sinistra
		wxPanel* imgPlaceholder = new wxPanel(pnlItem, wxID_ANY, wxDefaultPosition, wxSize(60, 40), wxBORDER_SIMPLE);
		imgPlaceholder->SetBackgroundColour(*wxWHITE);

		// Radio Button (Senza stile wxRB_GROUP perché sono su parent diversi)
		wxRadioButton* radio = new wxRadioButton(pnlItem, 1000 + i, s->GetNome());

		// Evento click
		radio->Bind(wxEVT_RADIOBUTTON, &DlgConcessionario::OnSelezionaScooter, this);

		sizerItem->Add(imgPlaceholder, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
		sizerItem->Add(radio, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);

		pnlItem->SetSizer(sizerItem);
		sizerList->Add(pnlItem, 0, wxEXPAND | wxALL, 2);
	}

	scrollList->SetSizer(sizerList);
	sizerContent->Add(scrollList, 0, wxEXPAND | wxALL, 5);

	// --- COLONNA DESTRA: STATISTICHE ---
	wxPanel* pnlStats = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
	wxBoxSizer* sizerStats = new wxBoxSizer(wxVERTICAL);

	// 1. IMMAGINE GRANDE (SOPRA)
	m_pnlImgBig = new wxPanel(pnlStats, wxID_ANY, wxDefaultPosition, wxSize(200, 120), wxBORDER_DOUBLE);
	m_pnlImgBig->SetBackgroundColour(*wxWHITE);
	// Centriamo l'immagine nel pannello di destra
	sizerStats->Add(m_pnlImgBig, 0, wxALL, 10);

	// 2. GRIGLIA STATISTICHE
	// 2 Colonne, gap verticale 5, orizzontale 10
	wxFlexGridSizer* gridStats = new wxFlexGridSizer(2, 5, 10);

	m_lblCosto = AddStat(pnlStats, gridStats, "Costo", "");
	m_lblVelocita = AddStat(pnlStats, gridStats, "Velocità:", "");
	sizerStats->Add(gridStats, 0, wxALL, 5);

	m_lblMarmitta = new wxStaticText{ pnlStats, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN };
	m_lblCarburatore = new wxStaticText{ pnlStats, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN };
	m_lblCilindrata = new wxStaticText{ pnlStats, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN };
	m_lblFiltro = new wxStaticText{ pnlStats, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN };

	sizerStats->AddSpacer(15);
	sizerStats->Add(new wxStaticText(pnlStats, wxID_ANY, "Componenti"), 0, wxALIGN_CENTER);
	sizerStats->Add(m_lblMarmitta, 0, wxEXPAND | wxALL, 5);
	sizerStats->Add(m_lblCarburatore, 0, wxEXPAND | wxALL, 5);
	sizerStats->Add(m_lblCilindrata, 0, wxEXPAND | wxALL, 5);
	sizerStats->Add(m_lblFiltro, 0, wxEXPAND | wxALL, 5);

	pnlStats->SetSizer(sizerStats);
	pnlStats->SetMinSize(wxSize(300, -1));

	// Assegnamo proporzione 1 alla parte destra così si prende tutto lo spazio rimasto
	sizerContent->Add(pnlStats, 1, wxEXPAND | wxALL, 5);

	// --- BARRA INFERIORE ---
	wxPanel* bottomPnl = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_RAISED);
	wxBoxSizer* bottomSizer = new wxBoxSizer(wxHORIZONTAL);

	m_lblSoldi = new wxStaticText(bottomPnl, wxID_ANY, "Soldi: " + m_game.GetSoldiStr(m_game.GetTabbyGuy().GetSoldi()));
	wxFont fBold = m_lblSoldi->GetFont(); fBold.SetWeight(wxFONTWEIGHT_BOLD); m_lblSoldi->SetFont(fBold);

	wxButton* btnCompra = new wxButton(bottomPnl, wxID_ANY, "Compra", wxDefaultPosition, wxSize(-1, 40));
	wxButton* btnCancel = new wxButton(bottomPnl, wxID_CANCEL, "Esci", wxDefaultPosition, wxSize(-1, 40));

	btnCompra->Bind(wxEVT_BUTTON, &DlgConcessionario::OnCompra, this);

	bottomSizer->Add(m_lblSoldi, 1, wxALIGN_CENTER_VERTICAL | wxALL, 15);
	bottomSizer->Add(btnCompra, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	bottomSizer->Add(btnCancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	bottomPnl->SetSizer(bottomSizer);

	mainSizer->Add(sizerContent, 1, wxEXPAND);
	mainSizer->Add(bottomPnl, 0, wxEXPAND);

	this->SetSizerAndFit(mainSizer);
	this->Centre();

	if (!m_catalogoPtr.empty()) {
		// Simuliamo il click sul primo per inizializzare (accende radio + aggiorna stat)
		wxCommandEvent fakeEvent(wxEVT_RADIOBUTTON, 1000);
		OnSelezionaScooter(fakeEvent);

		// Dobbiamo settare visivamente il primo radio a true perché l'evento fake non lo fa sulla GUI
		wxWindow* w = FindWindow(1000);
		if (w) ((wxRadioButton*)w)->SetValue(true);
	}
}

void DlgConcessionario::OnSelezionaScooter(wxCommandEvent& event)
{
	int selectedId = event.GetId();
	m_selectedIndex = selectedId - 1000;

	// --- FIX RADIO BUTTONS ---
	// Poiché i radio button sono su pannelli diversi, wxWidgets non gestisce l'esclusività.
	// Dobbiamo spegnere manualmente tutti quelli che NON sono quello cliccato.
	for (size_t i = 0; i < m_catalogoPtr.size(); ++i)
	{
		int currentId = 1000 + i;
		if (currentId == selectedId) continue; // Salta quello appena cliccato

		// Cerchiamo il controllo nella finestra tramite ID
		wxWindow* w = FindWindow(currentId);
		if (w)
		{
			wxRadioButton* rb = wxDynamicCast(w, wxRadioButton);
			if (rb) {
				// Lo spegniamo senza generare un altro evento
				rb->SetValue(false);
			}
		}
	}

	AggiornaStatistiche();
}

void DlgConcessionario::AggiornaStatistiche()
{
	if (m_selectedIndex < 0 || m_selectedIndex >= m_catalogoPtr.size()) return;

	const Scooter* s = static_cast<const Scooter*>(m_catalogoPtr[m_selectedIndex]);

	m_lblVelocita->SetLabel(std::to_string(s->GetVelocita()) + " km/h");
	m_lblMarmitta->SetLabel(s->GetMarmitta().GetNome());
	m_lblCarburatore->SetLabel(s->GetCarburatore().GetNome());
	m_lblCilindrata->SetLabel(s->GetCilindrata().GetNome());
	m_lblFiltro->SetLabel(s->GetFiltro().GetNome());

	m_lblCosto->SetLabel(m_game.GetSoldiStr(s->GetPrezzo()));

	// Aggiorna layout se i testi cambiano dimensione
	this->Layout();
}

void DlgConcessionario::OnCompra(wxCommandEvent& event)
{
	if (m_selectedIndex >= 0) {
		m_game.AzioneCompra(*m_catalogoPtr[m_selectedIndex]);
		EndModal(wxID_OK);
	}
}

wxStaticText* DlgConcessionario::AddStat(wxWindow* parent, wxSizer* sizer, wxString label, wxString value)
{
	sizer->Add(new wxStaticText(parent, wxID_ANY, label, wxDefaultPosition, wxSize(-1, 25)));
	wxStaticText* valueBox = new wxStaticText{ parent, wxID_ANY, value, wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN };
	valueBox->SetMinSize(wxSize(300, -1));
	sizer->Add(valueBox, 0, wxEXPAND);
	return valueBox;
}

// DIALOG DOCUMENTO D'IDENTITA'
DlgPersonalInfo::DlgPersonalInfo(wxWindow* parent, TabbyGame& game)
	: wxDialog(parent, wxID_ANY, "Documento d'identità", wxDefaultPosition, wxSize(500, -1)),
	m_game(game)
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(wxColor(245, 235, 200));
	const CartaIdentita& id = m_game.GetTabbyGuy().GetID();

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* contentSizer = new wxBoxSizer(wxHORIZONTAL);

	// --- FOTO (Sinistra) ---
	wxPanel* pnlFoto = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(120, 160), wxBORDER_SUNKEN);
	pnlFoto->SetBackgroundColour(*wxWHITE);
	wxStaticText* lblFoto = new wxStaticText(pnlFoto, wxID_ANY, "FOTO", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
	wxBoxSizer* sizerFoto = new wxBoxSizer(wxVERTICAL);
	sizerFoto->Add(lblFoto, 1, wxEXPAND | wxTOP, 70);
	pnlFoto->SetSizer(sizerFoto);
	contentSizer->Add(pnlFoto, 0, wxALL, 15);

	// --- DATI (Destra) ---
	wxGridBagSizer* grid = new wxGridBagSizer(5, 5);
	int r = 0;

	// BLOCCA 1: ANAGRAFICA
	AddEditRow(grid, r, "Cognome:", m_txtCognome, id.m_cognome);
	AddEditRow(grid, r, "Nome:", m_txtNome, id.m_nome);

	AddSeparator(grid, r);

	// BLOCCO 2: NASCITA
	wxString dataNascita = wxString::Format("%02d/%02d/%d",
		id.m_dataNascita.GetDay(), (int)id.m_dataNascita.GetMonth(), id.m_dataNascita.GetYear());

	AddStaticRow(grid, r, "Nato il:", dataNascita);
	// Visualizziamo l'Atto di nascita (mancante prima)
	AddStaticRow(grid, r, "Atto N.:", id.m_attoNascita);

	// Città e Provincia sulla stessa riga
	AddCityProvRow(grid, r, "a:", m_txtLuogoNascita, id.m_luogoNascita, m_txtProvincia, id.m_provinciaNascita);

	AddStaticRow(grid, r, "Cittadinanza:", id.m_cittadinanza);

	AddSeparator(grid, r);

	// BLOCCO 3: RESIDENZA
	AddEditRow(grid, r, "Residenza:", m_txtResidenza, id.m_residenza);
	AddEditRow(grid, r, "Via:", m_txtIndirizzo, id.m_indirizzo);

	AddSeparator(grid, r);

	// BLOCCO 4: STATO
	AddStaticRow(grid, r, "Stato Civile:", id.m_statoCivile);
	AddStaticRow(grid, r, "Professione:", id.m_professione);

	// *** RISOLUZIONE PROBLEMA TESTO TAGLIATO ***
	// Diciamo alla colonna 1 (quella dei valori) di occupare tutto lo spazio orizzontale disponibile
	grid->AddGrowableCol(1);

	contentSizer->Add(grid, 1, wxEXPAND | wxTOP | wxRIGHT | wxBOTTOM, 15);
	mainSizer->Add(contentSizer, 1, wxEXPAND);

	// BOTTONI
	wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
	wxButton* btnOk = new wxButton(this, wxID_OK, "Salva modifiche");
	wxButton* btnCancel = new wxButton(this, wxID_CANCEL, "Annulla");
	btnSizer->Add(btnOk, 0, wxALL, 5);
	btnSizer->Add(btnCancel, 0, wxALL, 5);
	mainSizer->Add(btnSizer, 0, wxALIGN_CENTER | wxALL, 10);

	btnOk->Bind(wxEVT_BUTTON, &DlgPersonalInfo::OnOk, this);

	this->SetSizerAndFit(mainSizer);
	this->Centre();
}

void DlgPersonalInfo::AddEditRow(wxGridBagSizer* sizer, int& row, wxString label, wxTextCtrl*& ctrlPtr, wxString value) {
	wxFont fontLabel = this->GetFont();
	fontLabel.SetWeight(wxFONTWEIGHT_BOLD);

	wxStaticText* txtLabel = new wxStaticText(this, wxID_ANY, label);
	txtLabel->SetFont(fontLabel);

	// Non mettiamo dimensione fissa, lasciamo fare al sizer
	ctrlPtr = new wxTextCtrl(this, wxID_ANY, value);

	sizer->Add(txtLabel, wxGBPosition(row, 0), wxDefaultSpan, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	// wxEXPAND è fondamentale qui
	sizer->Add(ctrlPtr, wxGBPosition(row, 1), wxDefaultSpan, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	row++;
}

void DlgPersonalInfo::AddCityProvRow(wxGridBagSizer* sizer, int& row, wxString label, wxTextCtrl*& ctrlCitta, wxString valCitta, wxTextCtrl*& ctrlProv, wxString valProv)
{
	wxFont fontLabel = this->GetFont();
	fontLabel.SetWeight(wxFONTWEIGHT_BOLD);

	wxStaticText* txtLabel = new wxStaticText(this, wxID_ANY, label);
	txtLabel->SetFont(fontLabel);

	// Creiamo un contenitore orizzontale per: [Citta] ( [Prov] )
	wxBoxSizer* hSizer = new wxBoxSizer(wxHORIZONTAL);

	ctrlCitta = new wxTextCtrl(this, wxID_ANY, valCitta);
	hSizer->Add(ctrlCitta, 1, wxEXPAND | wxRIGHT, 5); // La città prende lo spazio (1)

	hSizer->Add(new wxStaticText(this, wxID_ANY, "("), 0, wxALIGN_CENTER_VERTICAL);

	ctrlProv = new wxTextCtrl(this, wxID_ANY, valProv, wxDefaultPosition, wxSize(40, -1)); // La provincia è piccola
	hSizer->Add(ctrlProv, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 2);

	hSizer->Add(new wxStaticText(this, wxID_ANY, ")"), 0, wxALIGN_CENTER_VERTICAL);

	sizer->Add(txtLabel, wxGBPosition(row, 0), wxDefaultSpan, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	sizer->Add(hSizer, wxGBPosition(row, 1), wxDefaultSpan, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	row++;
}

void DlgPersonalInfo::AddStaticRow(wxGridBagSizer* sizer, int& row, wxString label, wxString value) {
	wxFont fontLabel = this->GetFont();
	fontLabel.SetWeight(wxFONTWEIGHT_BOLD);
	wxFont fontValue = this->GetFont();
	fontValue.SetStyle(wxFONTSTYLE_ITALIC);

	wxStaticText* txtLabel = new wxStaticText(this, wxID_ANY, label);
	txtLabel->SetFont(fontLabel);

	wxStaticText* txtValue = new wxStaticText(this, wxID_ANY, value);
	txtValue->SetFont(fontValue);
	txtValue->SetForegroundColour(wxColor(0, 0, 150));

	sizer->Add(txtLabel, wxGBPosition(row, 0), wxDefaultSpan, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	sizer->Add(txtValue, wxGBPosition(row, 1), wxDefaultSpan, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	row++;
}

void DlgPersonalInfo::AddSeparator(wxGridBagSizer* sizer, int& row) {
	wxStaticLine* line = new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL);
	sizer->Add(line, wxGBPosition(row++, 0), wxGBSpan(1, 2), wxEXPAND | wxTOP | wxBOTTOM, 5);
}

void DlgPersonalInfo::OnOk(wxCommandEvent& event) {
	CartaIdentita id = m_game.GetTabbyGuy().GetID();

	id.m_nome = m_txtNome->GetValue().ToStdString();
	id.m_cognome = m_txtCognome->GetValue().ToStdString();

	id.m_luogoNascita = m_txtLuogoNascita->GetValue().ToStdString();
	id.m_provinciaNascita = m_txtProvincia->GetValue().ToStdString(); // Salvataggio Provincia

	id.m_residenza = m_txtResidenza->GetValue().ToStdString();
	id.m_indirizzo = m_txtIndirizzo->GetValue().ToStdString();

	m_game.GetTabbyGuy().SetIdentita(id);

	EndModal(wxID_OK);
}

// DIALOG CONFIGURAZIONE
DlgConfig::DlgConfig(wxWindow* parent, TabbyGame& game)
	: wxDialog(parent, wxID_ANY, "Configuration", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE & ~wxRESIZE_BORDER),
	m_game(game)
{
	this->SetBackgroundColour(WIN_BKG);
	this->SetFont(parent->GetFont());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	// 1. CHOOSE SKILL LEVEL
	wxStaticBoxSizer* grpSkill = new wxStaticBoxSizer(wxVERTICAL, this, "Choose Skill level:");

	// Nomi presi dallo screenshot (Citazioni di Doom)
	std::vector<wxString> livelli = {
		"I'm too young be a tabbozzo.",
		"Hey, not to rough.",
		"Hurt me plenty.",
		"Ultra-violence.",
		"Nightmare. (Like my real life...)"
	};

	for (int i = 0; i < livelli.size(); i++)
	{
		// Il primo ha lo stile GROUP
		long style = (i == 0) ? wxRB_GROUP : 0;
		wxRadioButton* rad = new wxRadioButton(grpSkill->GetStaticBox(), wxID_ANY, livelli[i], wxDefaultPosition, wxDefaultSize, style);

		// TODO: Qui dovresti leggere dal gioco il livello attuale
		if (i == 0) rad->SetValue(true);

		m_radiosSkill.push_back(rad);
		grpSkill->Add(rad, 0, wxLEFT | wxTOP, 5);
	}
	grpSkill->AddSpacer(5); // Margine sotto

	mainSizer->Add(grpSkill, 0, wxEXPAND | wxALL, 5);


	// 2. MISC
	wxStaticBoxSizer* grpMisc = new wxStaticBoxSizer(wxVERTICAL, this, "Misc:");

	// Usiamo un GridBagSizer per posizionare checkbox a sinistra e Icone/OK a destra
	wxGridBagSizer* gbSizer = new wxGridBagSizer(2, 5);

	// -- Colonna Sinistra (Checkbox) --
	m_chkTimer = new wxCheckBox(grpMisc->GetStaticBox(), wxID_ANY, "Enable Timer");
	m_chkTimer->SetValue(true);

	m_chkEuro = new wxCheckBox(grpMisc->GetStaticBox(), wxID_ANY, "Enable Euro");
	m_chkEuro->SetValue(true);

	m_chkLogo = new wxCheckBox(grpMisc->GetStaticBox(), wxID_ANY, "Enable Startup Logo");
	m_chkLogo->SetValue(true);

	m_chkLog = new wxCheckBox(grpMisc->GetStaticBox(), wxID_ANY, "Enable Log File");
	m_chkLog->SetValue(false);

	m_chkSuoni = new wxCheckBox(grpMisc->GetStaticBox(), wxID_ANY, "Enable Sounds");
	m_chkSuoni->SetValue(true);

	// Aggiunta alla griglia (Colonna 0)
	gbSizer->Add(m_chkTimer, wxGBPosition(0, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	gbSizer->Add(m_chkEuro, wxGBPosition(1, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	gbSizer->Add(m_chkLogo, wxGBPosition(3, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	gbSizer->Add(m_chkLog, wxGBPosition(4, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxALL, 2);
	gbSizer->Add(m_chkSuoni, wxGBPosition(5, 0), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxALL, 2);

	// -- Colonna Destra (Icone e Bottone OK) --

	// Riga dell'Euro: Icone bandiera e moto
	wxBoxSizer* iconSizer = new wxBoxSizer(wxHORIZONTAL);
	// Placeholder per le icone (non avendo i png originali)
	wxStaticText* iconEU = new wxStaticText(grpMisc->GetStaticBox(), wxID_ANY, "[EU]");
	iconEU->SetForegroundColour(*wxBLUE);
	wxStaticText* iconMoto = new wxStaticText(grpMisc->GetStaticBox(), wxID_ANY, "[MOTO]");
	iconMoto->SetForegroundColour(*wxRED);

	iconSizer->Add(iconEU, 0, wxRIGHT, 5);
	iconSizer->Add(iconMoto, 0, wxLEFT, 5);

	gbSizer->Add(iconSizer, wxGBPosition(1, 1), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);

	// Bottone OK Grande con icona check
	wxButton* btnOk = new wxButton(grpMisc->GetStaticBox(), wxID_OK, "OK", wxDefaultPosition, wxSize(80, 40));
	// TODO: Usiamo wxART_TICK_MARK se vogliamo l'icona spunta, ma richiede wxBitmapButton. Per ora testo.

	// Il bottone OK nello screen occupa le ultime righe a destra
	gbSizer->Add(btnOk, wxGBPosition(4, 1), wxGBSpan(2, 1), wxALIGN_BOTTOM | wxALIGN_RIGHT | wxALL, 5);

	grpMisc->Add(gbSizer, 1, wxEXPAND | wxALL, 5);
	mainSizer->Add(grpMisc, 0, wxEXPAND | wxALL, 5);

	// FOOTER
	wxBoxSizer* footerSizer = new wxBoxSizer(wxHORIZONTAL);
	footerSizer->Add(new wxStaticText(this, wxID_ANY, "Tabboz Simulator"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	footerSizer->AddStretchSpacer();

	// "reset ." scritta piccola rossa
	wxStaticText* lblReset = new wxStaticText(this, wxID_ANY, "reset .");
	lblReset->SetForegroundColour(*wxRED);
	footerSizer->Add(lblReset, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	mainSizer->Add(footerSizer, 0, wxEXPAND | wxALL, 2);

	// Bind
	btnOk->Bind(wxEVT_BUTTON, &DlgConfig::OnOk, this);

	this->SetSizerAndFit(mainSizer);
	this->Centre();
}

void DlgConfig::OnOk(wxCommandEvent& event)
{
	// LOGICA DI SALVATAGGIO
	// Qui applichiamo le modifiche a TabbyGame.
	// Esempio:
	// m_game.SetEuroEnabled(m_chkEuro->GetValue());
	// m_game.SetSoundEnabled(m_chkSuoni->GetValue());

	// Trova il livello di difficoltà selezionato
	int skillLevel = 0;
	for (size_t i = 0; i < m_radiosSkill.size(); i++) {
		if (m_radiosSkill[i]->GetValue()) {
			skillLevel = i;
			break;
		}
	}
	// m_game.SetDifficulty(skillLevel);

	EndModal(wxID_OK);
}

// DIALOG USCITA
DlgUscita::DlgUscita(wxWindow* parent)
	: wxDialog(parent, wxID_ANY, "Fine della sessione del Tabboz Simulator", wxDefaultPosition, wxDefaultSize, wxCAPTION | wxCLOSE_BOX),
	m_sceltaUscita(false)
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* bodySizer = new wxBoxSizer(wxHORIZONTAL);

	// 1. ICONA A SINISTRA (Il monitor col punto esclamativo rosso nello screen)
	// Usiamo un pannello placeholder o uno static text per ora
	wxStaticText* iconBox = new wxStaticText(this, wxID_ANY, "[ICON]", wxDefaultPosition, wxSize(48, 48), wxALIGN_CENTER | wxBORDER_NONE);
	// In futuro qui caricherai l'icona del monitor
	bodySizer->Add(iconBox, 0, wxALL | wxALIGN_TOP, 15);

	// 2. PARTE DESTRA (Testo + Radio Buttons)
	wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* lblIntro = new wxStaticText(this, wxID_ANY, "Scegli una delle seguenti opzioni:");
	rightSizer->Add(lblIntro, 0, wxBOTTOM, 10);

	m_radioChiudi = new wxRadioButton(this, wxID_ANY, "Chiudi il Tabboz Simulator", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	m_radioChiudi->SetValue(true); // Selezionato di default

	m_radioSpegni = new wxRadioButton(this, wxID_ANY, "Spegni il computer ed esci di casa");

	rightSizer->Add(m_radioChiudi, 0, wxBOTTOM, 5);
	rightSizer->Add(m_radioSpegni, 0, wxBOTTOM, 5);

	bodySizer->Add(rightSizer, 1, wxEXPAND | wxALL, 10);
	mainSizer->Add(bodySizer, 1, wxEXPAND | wxALL, 5);

	// 3. BOTTONI IN BASSO
	wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);

	// Nello screenshot i bottoni sono centrati/espansi. Usiamo dimensioni fisse simili a Windows standard.
	wxButton* btnOk = new wxButton(this, wxID_OK, "OK", wxDefaultPosition, wxSize(75, 25));
	wxButton* btnCancel = new wxButton(this, wxID_CANCEL, "Cancel", wxDefaultPosition, wxSize(75, 25));
	wxButton* btnHelp = new wxButton(this, wxID_HELP, "Help", wxDefaultPosition, wxSize(75, 25));

	btnOk->Bind(wxEVT_BUTTON, &DlgUscita::OnOk, this);
	btnCancel->Bind(wxEVT_BUTTON, &DlgUscita::OnCancel, this);
	btnHelp->Bind(wxEVT_BUTTON, &DlgUscita::OnHelp, this);

	btnSizer->Add(btnOk, 0, wxRIGHT, 5);
	btnSizer->Add(btnCancel, 0, wxRIGHT, 5);
	btnSizer->Add(btnHelp, 0, wxLEFT, 5);

	mainSizer->Add(btnSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxBOTTOM, 10);

	this->SetSizerAndFit(mainSizer);
	this->Centre();
}

void DlgUscita::OnOk(wxCommandEvent& event)
{
	// Se è selezionato "Chiudi" o "Spegni", in entrambi i casi nel simulatore chiudiamo.
	// Ma tecnicamente potresti voler fare cose diverse (es. salvare prima di chiudere).
	m_sceltaUscita = true;
	EndModal(wxID_OK);
}

void DlgUscita::OnCancel(wxCommandEvent& event)
{
	m_sceltaUscita = false;
	EndModal(wxID_CANCEL);
}

void DlgUscita::OnHelp(wxCommandEvent& event)
{
	wxMessageBox("Non c'è aiuto per te, Tabbozzo.", "Help", wxOK | wxICON_INFORMATION);
}

// DIALOG ABOUT
DlgAbout::DlgAbout(wxWindow* parent)
	: wxDialog(parent, wxID_ANY, "About Tabboz Simulator", wxDefaultPosition, wxDefaultSize, wxCAPTION | wxCLOSE_BOX)
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour()); // Grigio standard

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	// --- SEZIONE SUPERIORE (Info Generali + Bottone OK) ---
	// Useremo un pannello "Container" con bordo SUNKEN per raggruppare tutto come nello screen
	// Nota: Nello screen originale sembra un unico grande riquadro che contiene tutto tranne l'URL in basso.

	wxPanel* pnlMainFrame = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
	wxBoxSizer* frameSizer = new wxBoxSizer(wxVERTICAL);

	// 1. HEADER (Icone, Testo, Bottone OK laterale)
	wxBoxSizer* headerSizer = new wxBoxSizer(wxHORIZONTAL);

	// Colonna Icone (Sinistra)
	wxBoxSizer* iconsSizer = new wxBoxSizer(wxVERTICAL);
	// Icona Moto
	iconsSizer->Add(new wxStaticText(pnlMainFrame, wxID_ANY, "[MOTO]"), 0, wxBOTTOM, 2);
	// Icona UE
	iconsSizer->Add(new wxStaticText(pnlMainFrame, wxID_ANY, "[EU]"), 0, wxTOP, 2);

	headerSizer->Add(iconsSizer, 0, wxALL, 10);

	// Colonna Testo (Centro)
	wxBoxSizer* textSizer = new wxBoxSizer(wxVERTICAL);
	textSizer->Add(new wxStaticText(pnlMainFrame, wxID_ANY, "Tabboz simulator"), 0, wxBOTTOM, 2);
	textSizer->Add(new wxStaticText(pnlMainFrame, wxID_ANY, "Version 0.9beta, Mar 11 2000"), 0, wxBOTTOM, 2);
	textSizer->Add(new wxStaticText(pnlMainFrame, wxID_ANY, "Copyright (C) 1997-2000 by"), 0, wxBOTTOM, 0);
	textSizer->Add(new wxStaticText(pnlMainFrame, wxID_ANY, "Obscured Truckware"), 0, wxBOTTOM, 0);

	headerSizer->Add(textSizer, 1, wxTOP | wxBOTTOM | wxLEFT, 10);

	// Colonna Bottone OK (Destra) con icona check
	// Nota: Nello screen il bottone è grande e ha un'icona verde.
	wxButton* btnOk = new wxButton(pnlMainFrame, wxID_OK, "OK", wxDefaultPosition, wxSize(80, 40));
	// btnOk->SetBitmap(...); // Se avessi l'icona
	btnOk->Bind(wxEVT_BUTTON, &DlgAbout::OnOk, this);

	headerSizer->Add(btnOk, 0, wxALL | wxALIGN_TOP, 10);

	frameSizer->Add(headerSizer, 0, wxEXPAND);

	// LINKE HORIZONTAL
	frameSizer->Add(new wxStaticLine(pnlMainFrame), 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

	// 2. IMMAGINE LOGO (32bit Tabboz Simulator)
	wxPanel* pnlLogo = new wxPanel(pnlMainFrame, wxID_ANY, wxDefaultPosition, wxSize(300, 100), wxBORDER_NONE);
	// Qui andrebbe caricata l'immagine "32bit Tabboz Simulator"
	// pnlLogo->SetBackgroundBitmap(...); 
	wxBoxSizer* logoSizer = new wxBoxSizer(wxVERTICAL);
	logoSizer->Add(new wxStaticText(pnlLogo, wxID_ANY, "[LOGO TABBOZ 32bit]"), 1, wxALIGN_CENTER);
	pnlLogo->SetSizer(logoSizer);

	frameSizer->Add(pnlLogo, 0, wxALIGN_CENTER | wxALL, 10);

	// 3. CREDITS (Created by)
	wxFlexGridSizer* creditGrid = new wxFlexGridSizer(2, 5, 20); // 2 cols, gap
	creditGrid->Add(new wxStaticText(pnlMainFrame, wxID_ANY, "Created by:"), 0, wxALIGN_RIGHT);

	wxBoxSizer* authorsSizer = new wxBoxSizer(wxVERTICAL);
	authorsSizer->Add(new wxStaticText(pnlMainFrame, wxID_ANY, "Andrea Bonomi"));
	authorsSizer->Add(new wxStaticText(pnlMainFrame, wxID_ANY, "Emanuele Caccialanza"));
	authorsSizer->Add(new wxStaticText(pnlMainFrame, wxID_ANY, "Daniele Gazzarri"));

	creditGrid->Add(authorsSizer, 0, wxALIGN_LEFT);

	frameSizer->Add(creditGrid, 0, wxALIGN_CENTER | wxBOTTOM, 10);

	// LINEA SEPARAZIONE
	frameSizer->Add(new wxStaticLine(pnlMainFrame), 0, wxEXPAND | wxLEFT | wxRIGHT, 2);

	// 4. BETA TESTERS (Barra grigia scura/incassata con testo)
	wxPanel* pnlTesters = new wxPanel(pnlMainFrame, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE); // Nello screen sembra piatto ma separato
	wxBoxSizer* testerSizer = new wxBoxSizer(wxHORIZONTAL);
	testerSizer->Add(new wxStaticText(pnlTesters, wxID_ANY, "Beta testers:"), 0, wxRIGHT, 20);
	testerSizer->Add(new wxStaticText(pnlTesters, wxID_ANY, "Dino Lucci"), 0, wxRIGHT, 40);
	testerSizer->Add(new wxStaticText(pnlTesters, wxID_ANY, "Giulio Lucci"), 0, wxRIGHT, 5);
	pnlTesters->SetSizer(testerSizer);

	frameSizer->Add(pnlTesters, 0, wxEXPAND | wxALL, 5);

	// LINEA SEPARAZIONE
	frameSizer->Add(new wxStaticLine(pnlMainFrame), 0, wxEXPAND | wxLEFT | wxRIGHT, 2);

	// 5. DISCLAIMER (Icona rossa + Testo)
	wxBoxSizer* disclaimerSizer = new wxBoxSizer(wxHORIZONTAL);

	// Icona rossa (Omino)
	wxStaticText* iconOmino = new wxStaticText(pnlMainFrame, wxID_ANY, "[!!]", wxDefaultPosition, wxSize(32, 32), wxALIGN_CENTER | wxBORDER_SIMPLE);
	iconOmino->SetBackgroundColour(*wxRED);
	iconOmino->SetForegroundColour(*wxWHITE);

	disclaimerSizer->Add(iconOmino, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10);

	wxStaticText* txtDisclaimer = new wxStaticText(pnlMainFrame, wxID_ANY,
		"Questo programma contiene un linguaggio\ntalvolta offensivo; ogni riferimento a persone e\ncose e' puramente casuale.");
	txtDisclaimer->SetWindowStyle(wxALIGN_CENTER);

	disclaimerSizer->Add(txtDisclaimer, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	frameSizer->Add(disclaimerSizer, 0, wxEXPAND | wxBOTTOM, 5);

	// CHIUSURA PANEL PRINCIPALE
	pnlMainFrame->SetSizer(frameSizer);
	mainSizer->Add(pnlMainFrame, 1, wxEXPAND | wxALL, 5);

	// --- SEZIONE INFERIORE (URL + Bottone Norme) ---
	wxBoxSizer* footerSizer = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* urlSizer = new wxBoxSizer(wxVERTICAL);
	urlSizer->Add(new wxStaticText(this, wxID_ANY, "http://www.tabboz.com"), 0);
	urlSizer->Add(new wxStaticText(this, wxID_ANY, "e-mail: andrea@tabboz.com"), 0);

	footerSizer->Add(urlSizer, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);

	wxButton* btnNorme = new wxButton(this, wxID_ANY, "Norme di utilizzo", wxDefaultPosition, wxSize(120, -1));
	btnNorme->Bind(wxEVT_BUTTON, &DlgAbout::OnNorme, this);

	footerSizer->Add(btnNorme, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

	mainSizer->Add(footerSizer, 0, wxEXPAND | wxBOTTOM, 10);

	this->SetSizerAndFit(mainSizer);
	this->Centre();
}

void DlgAbout::OnOk(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}

void DlgAbout::OnNorme(wxCommandEvent& event)
{
	wxMessageBox("Non copiare, non rubare, sii un bravo tabbozzo.", "Norme", wxOK);
}

DlgDueDonne::DlgDueDonne(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Troppe donne", wxDefaultPosition, wxDefaultSize, wxCAPTION },
	m_game{game}
{
	this->SetBackgroundColour(parent->GetBackgroundColour());
	this->SetFont(parent->GetFont());

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };
	
	wxBoxSizer* sizerHeader = new wxBoxSizer{ wxHORIZONTAL };

	// TODO: AGGIUNGI ICONA
	wxStaticText* titolo = new wxStaticText{ this, wxID_ANY, "Ti ritrovi nell'imbarazzante (?) situazione di avere 2 ragazze... \nChe cosa fai ???" };
	titolo->Wrap(400);
	sizerHeader->Add(titolo, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	mainSizer->Add(sizerHeader, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxTOP | wxBOTTOM, 5);
	mainSizer->Add(new wxStaticText(this, wxID_ANY, "Viva l'abbondanza !"), 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	wxButton* btnEntrambe = new wxButton{ this, wxID_ANY, "Le voglio tutte e due !", wxDefaultPosition, wxSize(-1, 40) };
	btnEntrambe->Bind(wxEVT_BUTTON, &DlgDueDonne::OnEntrambe, this);
	mainSizer->Add(btnEntrambe, 0, wxEXPAND | wxALL, 5);
	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

	wxStaticText* scelta = new wxStaticText{ this, wxID_ANY, "Purtroppo avere due ragazze è troppo anche per uno stallone come il sottoscritto..." };
	scelta->Wrap(400);
	mainSizer->Add(scelta, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	wxButton* btnResto = new wxButton{ this, wxID_ANY, "Resto con " + m_game.GetTabbyGuy().GetTipa().GetNome(), wxDefaultPosition, wxSize(-1, 40) };
	btnResto->Bind(wxEVT_BUTTON, &DlgDueDonne::OnResto, this);
	mainSizer->Add(btnResto, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);
	wxButton* btnPreferisco = new wxButton{ this, wxID_ANY, "Preferisco " + m_game.GetTipaNuova().GetNome(), wxDefaultPosition, wxSize(-1, 40) };
	btnPreferisco->Bind(wxEVT_BUTTON, &DlgDueDonne::OnPreferisco, this);
	mainSizer->Add(btnPreferisco, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

	this->SetSizerAndFit(mainSizer);
	this->Layout();
}

void DlgDueDonne::OnEntrambe(wxCommandEvent& event)
{
	m_game.AzioneVoglioEntrambe();
	this->EndModal(wxID_ANY);
}

void DlgDueDonne::OnResto(wxCommandEvent& event) { this->EndModal(wxID_OK); }

void DlgDueDonne::OnPreferisco(wxCommandEvent& event)
{
	m_game.AzionePreferiscoNuova();
	this->EndModal(wxID_ANY);
}

// --- DIALOGO PAGELLA SCOLASTICA ---
DlgPagella::DlgPagella(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Pagella Scolastica", wxDefaultPosition, wxDefaultSize, wxCAPTION | wxCLOSE_BOX },
	m_game{ game }
{
	this->SetBackgroundColour(wxColor(240, 240, 220)); // Colore carta vecchia
	this->SetFont(parent->GetFont());

	wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

	// 1. INTESTAZIONE
	wxStaticText* lblIntestazione = new wxStaticText(this, wxID_ANY, "SCRUTINIO FINALE");
	wxFont fTitle = lblIntestazione->GetFont();
	fTitle.SetWeight(wxFONTWEIGHT_BOLD);
	fTitle.SetPointSize(14);
	lblIntestazione->SetFont(fTitle);
	mainSizer->Add(lblIntestazione, 0, wxALL | wxALIGN_CENTER, 15);

	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

	// 2. LISTA VOTI
	// Usiamo una griglia a 2 colonne: [Materia] ...... [Voto]
	wxFlexGridSizer* gridVoti = new wxFlexGridSizer(2, 5, 10);

	const std::vector<Materia>& materie = m_game.GetTabbyGuy().GetScuola().m_materie;
	int insufficienze = 0;

	for (const auto& m : materie)
	{
		// Nome Materia
		wxStaticText* lblMat = new wxStaticText(this, wxID_ANY, m.GetNome());
		// Voto
		wxStaticText* lblVoto = new wxStaticText(this, wxID_ANY, std::to_string(m.GetVoto()));
		lblVoto->SetFont(fTitle); // Usiamo il font grassetto per il voto

		// Se è insufficiente, coloriamo di rosso
		if (m.GetVoto() < 6) {
			insufficienze++;
			if (m.GetVoto() < 4)
				insufficienze++;	// Le insufficienze gravi contano
			lblMat->SetForegroundColour(*wxRED);
			lblVoto->SetForegroundColour(*wxRED);
		}

		gridVoti->Add(lblMat, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
		gridVoti->Add(lblVoto, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	}

	gridVoti->Add(new wxStaticText(this, wxID_ANY, "Condotta:"), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	std::string condottaStr = (m_game.GetTabbyGuy().GetFama() > 75) ? "9" : "8";	// Più è un figo, più sembra un bravo ragazzo...
	wxStaticText* lblCondotta = new wxStaticText(this, wxID_ANY, condottaStr);
	lblCondotta->SetFont(fTitle);
	gridVoti->Add(lblCondotta, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);

	// Centriamo la griglia
	wxBoxSizer* gridWrapper = new wxBoxSizer(wxVERTICAL);
	gridWrapper->Add(gridVoti, 1, wxALIGN_CENTER | wxALL, 15);
	mainSizer->Add(gridWrapper, 1, wxEXPAND);

	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

	// 3. VERDETTO FINALE
	bool promosso = (insufficienze < 5);

	wxString esitoStr = promosso ? "PROMOSSO" : "BOCCIATO";
	wxColor esitoCol = promosso ? wxColor(0, 150, 0) : *wxRED;

	wxStaticText* lblEsito = new wxStaticText(this, wxID_ANY, esitoStr);
	wxFont fEsito = lblEsito->GetFont();
	fEsito.SetPointSize(18);
	fEsito.SetWeight(wxFONTWEIGHT_BOLD);
	lblEsito->SetFont(fEsito);
	lblEsito->SetForegroundColour(esitoCol);

	mainSizer->Add(lblEsito, 0, wxALL | wxALIGN_CENTER, 15);

	wxString commento = promosso ? "Ammesso all'anno successivo" : "NON ammesso all'anno successivo";
	mainSizer->Add(new wxStaticText(this, wxID_ANY, commento), 0, wxALL | wxALIGN_CENTER, 15);

	// 4. BOTTONE OK
	wxButton* btnOk = new wxButton(this, wxID_OK, "OK", wxDefaultPosition, wxSize(100, 40));
	btnOk->Bind(wxEVT_BUTTON, &DlgPagella::OnOk, this);
	mainSizer->Add(btnOk, 0, wxALL | wxALIGN_CENTER, 15);

	this->SetSizerAndFit(mainSizer);
	this->Centre();
}

void DlgPagella::OnOk(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}