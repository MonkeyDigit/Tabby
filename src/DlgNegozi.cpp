#include "DlgNegozi.h"

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

	wxButton* btnConcessionario = new wxButton{ pnlCompravendita, wxID_ANY, "Concessionario", wxDefaultPosition, wxSize(-1, 45) };
	btnConcessionario->Bind(wxEVT_BUTTON, &DlgScooter::OnConcessionario, this);

	wxButton* btnVendi = new wxButton{ pnlCompravendita, wxID_ANY, "Vendi Scooter", wxDefaultPosition, wxSize(-1, 45) };
	btnVendi->Bind(wxEVT_BUTTON, &DlgScooter::OnVendi, this);

	sizerCompravendita->Add(btnConcessionario, 0, wxEXPAND | wxALL, 5);
	sizerCompravendita->Add(btnVendi, 0, wxEXPAND | wxALL, 5);
	pnlCompravendita->SetSizer(sizerCompravendita);

	leftCol->Add(pnlCompravendita, 0, wxEXPAND | wxALL, 5);

	// 2. MODIFICHE
	wxPanel* pnlModifiche = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerModifiche = new wxBoxSizer{ wxVERTICAL };

	// --- FOTO SCOOTER ---
	// Creiamo il controllo vuoto (o con una bitmap nulla), ci pensa AggiornaInterfaccia a riempirlo
	m_imgScooter = new wxStaticBitmap{ pnlModifiche, wxID_ANY, wxNullBitmap };
	sizerModifiche->Add(m_imgScooter, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);
	sizerModifiche->AddStretchSpacer();

	wxButton* btnTrucca = new wxButton{ pnlModifiche, wxID_ANY, "Trucca scooter", wxDefaultPosition, wxSize(-1, 45) };
	wxButton* btnRipara = new wxButton{ pnlModifiche, wxID_ANY, "Ripara scooter", wxDefaultPosition, wxSize(-1, 45) };

	btnTrucca->Bind(wxEVT_BUTTON, &DlgScooter::OnTrucca, this);
	btnRipara->Bind(wxEVT_BUTTON, &DlgScooter::OnRipara, this);

	sizerModifiche->Add(btnTrucca, 0, wxEXPAND | wxALL, 5);
	sizerModifiche->Add(btnRipara, 0, wxEXPAND | wxALL & ~wxTOP, 5);
	pnlModifiche->SetSizer(sizerModifiche);
	leftCol->Add(pnlModifiche, 1, wxEXPAND | wxALL, 5);

	// 3. INFO E OK
	wxPanel* pnlBottom = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };

	wxBitmap bmpIcon = CaricaAsset("Z1.png");
	wxStaticBitmap* imgIcon = new wxStaticBitmap(pnlBottom, wxID_ANY, bmpIcon);
	sizerBottom->Add(imgIcon, 0, wxALIGN_CENTER | wxALL, 5);

	m_lblSoldi = new wxStaticText{ pnlBottom, wxID_ANY, "" }; // Inizializzato vuoto, ci pensa AggiornaInterfaccia
	sizerBottom->Add(m_lblSoldi, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	sizerBottom->AddStretchSpacer();
	sizerBottom->Add(new wxButton{ pnlBottom, wxID_OK, "OK", wxDefaultPosition, wxSize(60, 50) }, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	pnlBottom->SetSizer(sizerBottom);
	leftCol->Add(pnlBottom, 0, wxALL, 5);

	// --- COLONNA DESTRA (Statistiche) ----------------------------------------------------------------------------
	wxPanel* pnlStats = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerStats = new wxBoxSizer{ wxVERTICAL };

	sizerStats->Add(new wxStaticText{ pnlStats, wxID_ANY, "Statistiche scooter" }, 0, wxALIGN_CENTER_HORIZONTAL);

	// Nome Scooter
	m_lblNome = new wxStaticText{ pnlStats, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER | wxBORDER_SUNKEN };
	sizerStats->Add(m_lblNome, 0, wxEXPAND | wxALL & ~wxTOP, 10);

	// GRIGLIA DATI
	wxFlexGridSizer* gridStats = new wxFlexGridSizer{ 2, 5, 10 };

	// Salviamo i puntatori restituiti da AddStat per poterli aggiornare dopo
	m_lblVelocita = AddStat(pnlStats, gridStats, "Velocità:", "");
	m_lblBenza = AddStat(pnlStats, gridStats, "Benzina:", "");
	m_lblStato = AddStat(pnlStats, gridStats, "Stato:", "");

	m_lblMarmitta = new wxStaticText{ pnlStats, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN };
	m_lblCarburatore = new wxStaticText{ pnlStats, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN };
	m_lblCilindrata = new wxStaticText{ pnlStats, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN };
	m_lblFiltro = new wxStaticText{ pnlStats, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN };
	sizerStats->Add(gridStats, 0, wxALL & ~wxTOP, 5);

	sizerStats->AddSpacer(15);
	sizerStats->Add(new wxStaticText{ pnlStats, wxID_ANY, "Componenti" }, 0, wxALIGN_CENTER);
	sizerStats->Add(m_lblMarmitta, 0, wxEXPAND | wxALL, 5);
	sizerStats->Add(m_lblCarburatore, 0, wxEXPAND | wxALL, 5);
	sizerStats->Add(m_lblCilindrata, 0, wxEXPAND | wxALL, 5);
	sizerStats->Add(m_lblFiltro, 0, wxEXPAND | wxALL, 5);


	// BOTTONI IN BASSO A DESTRA
	sizerStats->AddStretchSpacer();

	m_btnUsa = new wxButton{ pnlStats, wxID_ANY, "---", wxDefaultPosition, wxSize(-1, 45) };
	wxButton* btnBenza = new wxButton{ pnlStats, wxID_ANY, "Fai benza", wxDefaultPosition, wxSize(-1, 45) };

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
	sizer->Add(new wxStaticText{ parent, wxID_ANY, label, wxDefaultPosition, wxSize(-1, 25) });
	wxStaticText* valueBox = new wxStaticText{ parent, wxID_ANY, value, wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN };
	valueBox->SetMinSize(wxSize(300, -1));
	sizer->Add(valueBox, 0, wxEXPAND);
	return valueBox;
}

void DlgScooter::AggiornaInterfaccia()
{
	const Scooter& s = m_game.GetTabbyGuy().GetScooter();

	wxBitmap bmpScooter = CaricaAsset(s.GetImageStr());

	// Applica la nuova immagine al controllo
	m_imgScooter->SetBitmap(bmpScooter);
	m_imgScooter->Refresh(); // Forza il ridisegno immediato

	// Aggiorna Soldi
	m_lblSoldi->SetLabel("Soldi " + m_game.GetSoldiStr(m_game.GetTabbyGuy().GetSoldi()));

	// Aggiorna Nome (con spazi padding per estetica)
	m_lblNome->SetLabel("  " + s.GetNome() + "  ");

	if (m_game.GetTabbyGuy().HaScooter())
	{
		// Aggiorna Statistiche
		if (s.GetAttivita() != Attivita::IN_GIRO)
		{
			m_lblVelocita->SetLabel("(" + s.GetAttivitaStr(false) + ")");
		}
		else
			m_lblVelocita->SetLabel(std::to_string(s.GetVelocita()) + " km/h");

		m_lblBenza->SetLabel(wxString::Format("%.1f l", s.GetBenza()));
		m_lblStato->SetLabel(std::to_string(s.GetStato()) + "%");
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

wxStaticText* DlgConcessionario::AddStat(wxWindow* parent, wxSizer* sizer, wxString label, wxString value)
{
	sizer->Add(new wxStaticText{ parent, wxID_ANY, label, wxDefaultPosition, wxSize(-1, 25) });
	wxStaticText* valueBox = new wxStaticText{ parent, wxID_ANY, value, wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN };
	valueBox->SetMinSize(wxSize(300, -1));
	sizer->Add(valueBox, 0, wxEXPAND);
	return valueBox;
}

// DIALOG PALESTRA
DlgPalestra::DlgPalestra(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Palestra", wxDefaultPosition, wxDefaultSize },
	m_game{ game }
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour());

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };
	wxBoxSizer* sizerBody = new wxBoxSizer{ wxHORIZONTAL };

	wxPanel* pnlBottom = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxSize(-1, 70), wxBORDER_SUNKEN };
	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };

	// FOTO
	wxPanel* pnlFoto = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerFoto = new wxBoxSizer{ wxVERTICAL };

	wxBitmap bmpPal = CaricaAsset("palestra.png");
	// Ridimensionamento
	bmpPal = wxBitmap(bmpPal.ConvertToImage().Rescale(bmpPal.GetWidth() * 1.2f, bmpPal.GetHeight() * 1.2f, wxIMAGE_QUALITY_BILINEAR));
	wxStaticBitmap* imgPal = new wxStaticBitmap(pnlFoto, wxID_ANY, bmpPal);
	sizerFoto->Add(imgPal, 0, wxALIGN_CENTER | wxALL, 5);

	pnlFoto->SetSizer(sizerFoto);
	sizerBody->Add(pnlFoto, 0, wxEXPAND | wxALL & ~wxRIGHT, 5);
	// BOTTONI
	wxPanel* pnlButtons = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerButtons = new wxBoxSizer{ wxVERTICAL };

	wxButton* btnVaiPalestra = new wxButton{ pnlButtons, wxID_ANY, "Vai in palestra", wxDefaultPosition, wxSize(-1, 40) };
	sizerButtons->Add(btnVaiPalestra, 0, wxEXPAND | wxALL, 5);
	btnVaiPalestra->Bind(wxEVT_BUTTON, &DlgPalestra::OnVaiPalestra, this);

	sizerButtons->Add(new wxStaticText{ pnlButtons, wxID_ANY, "Prezzi abbonamenti palestra" }, 0, wxALIGN_CENTER | wxALL & ~wxBOTTOM, 5);
	// INFORMAZIONI ABBONAMENTI
	wxPanel* pnlSub = new wxPanel{ pnlButtons, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };	// Faccio il panel solo per avere il rientro
	wxGridSizer* gridSub = new wxGridSizer{ 2, 3, 10, 10 };	// Griglia da 2 righe e 3 colonne

	gridSub->Add(new wxStaticText{ pnlSub, wxID_ANY, m_game.GetSoldiStr(PALESTRA_ABB_1) }, 0, wxALIGN_CENTER | wxALL & wxBOTTOM, 5);
	gridSub->Add(new wxStaticText{ pnlSub, wxID_ANY, m_game.GetSoldiStr(PALESTRA_ABB_6) }, 0, wxALIGN_CENTER | wxALL & wxBOTTOM, 5);
	gridSub->Add(new wxStaticText{ pnlSub, wxID_ANY, m_game.GetSoldiStr(PALESTRA_ABB_12) }, 0, wxALIGN_CENTER | wxALL & wxBOTTOM, 5);

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

	wxButton* btnLampada = new wxButton{ pnlButtons, wxID_ANY, "Lampada (" + m_game.GetSoldiStr(PREZZO_LAMPADA) + ")" };
	sizerButtons->Add(btnLampada, 0, wxALL, 5);
	btnLampada->Bind(wxEVT_BUTTON, &DlgPalestra::OnLampada, this);

	m_lblPelle = new wxStaticText{ pnlButtons, wxID_ANY, "---", wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN | wxALIGN_CENTER_HORIZONTAL };
	sizerButtons->Add(m_lblPelle, 0, wxEXPAND | wxALL, 5);

	pnlButtons->SetSizer(sizerButtons);

	sizerBody->Add(pnlButtons, 1, wxEXPAND | wxALL & ~wxLEFT, 5);
	mainSizer->Add(sizerBody);

	wxBitmap bmpIcon = CaricaAsset("ZPALESTRA.png");
	wxStaticBitmap* imgIcon = new wxStaticBitmap(pnlBottom, wxID_ANY, bmpIcon);
	sizerBottom->Add(imgIcon, 0, wxALIGN_CENTER | wxALL, 5);

	m_lblSoldi = new wxStaticText{ pnlBottom, wxID_ANY, "---" };
	sizerBottom->Add(m_lblSoldi, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	sizerBottom->AddSpacer(50);
	m_lblFama = new wxStaticText{ pnlBottom, wxID_ANY, "---" };
	sizerBottom->Add(m_lblFama, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	sizerBottom->AddStretchSpacer();
	wxButton* btnEsci = new wxButton{ pnlBottom, wxID_CANCEL, "Esci", wxDefaultPosition, wxSize(100, 40) };
	sizerBottom->Add(btnEsci, 0, wxALIGN_CENTER | wxALL, 5);

	pnlBottom->SetSizer(sizerBottom);
	mainSizer->Add(pnlBottom, 0, wxEXPAND | wxALL, 5);

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
		wxButton* btnNegozio = new wxButton{ pnlBody, wxID_ANY, negozi[i].m_nome, wxDefaultPosition, wxSize(-1, 40), wxEXPAND };
		// Sfruttiamo una funzione lambda per associare in modo dinamico il bottone al negozio selezionato
		sizerBody->Add(btnNegozio, 0, wxEXPAND | wxRIGHT | wxLEFT, 5);
		btnNegozio->Bind(wxEVT_BUTTON, [this, negozi, i](wxCommandEvent&) {

			if (m_game.TriggerNegozio(negozi[i].m_merce))
			{
				if (negozi[i].m_merce == CategoriaOggetto::CONSUMABILE)
					m_game.PlayAudio(203);
				else if (m_game.GenRandomInt(0, 1) == 1)
					m_game.PlayAudio(204);
				else
					m_game.PlayAudio(205);
				ManifestaEventi(this, m_game);

				DlgNegozio dlg{ this, m_game, negozi[i] };
				dlg.ShowModal();
				this->AggiornaInterfaccia();
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
	wxButton* btnChiudi = new wxButton{ pnlBottom, wxID_CANCEL, "Chiudi", wxDefaultPosition, wxSize(120, 40) };
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

// DIALOG NEGOZIO
DlgNegozio::DlgNegozio(wxWindow* parent, TabbyGame& game, const Negozio& negozio)
	: wxDialog{ parent, wxID_ANY, negozio.m_nome, wxDefaultPosition, wxDefaultSize },
	m_game{ game }, m_negozio{ negozio }
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour());

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	// TITOLO
	wxStaticText* title = new wxStaticText{ this, wxID_ANY, "BENVENUTO DA " + m_negozio.m_nome };
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
	wxSize gridSize = gridSizer->GetMinSize();
	// 2. Aggiungiamo un margine per la barra di scorrimento verticale (circa 20-30px)
	int larghezzaTotale = gridSize.x + 40;
	// 3. Imponiamo questa larghezza alla finestra di scroll.
	scrollWin->SetMinSize(wxSize(larghezzaTotale, 650));
	mainSizer->Add(scrollWin, 0, wxEXPAND | wxALL, 5);

	// --- BARRA INFERIORE ---
	wxPanel* pnlBottom = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_RAISED);
	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };

	wxBitmap bmpIcon;
	switch (m_negozio.m_merce)
	{
	case CategoriaOggetto::CONSUMABILE:
		bmpIcon = CaricaAsset("ZTABACCHI.png");
		break;
	case CategoriaOggetto::TELEFONO:
		bmpIcon = CaricaAsset("ZTELEFONO.png");
		break;
	case CategoriaOggetto::SCOOTER_PART:
		bmpIcon = CaricaAsset("ZLAVORO.png");
		break;
	default:
		bmpIcon = CaricaAsset("ZSCOOTER.png");
	}

	wxStaticBitmap* imgIcon = new wxStaticBitmap(pnlBottom, wxID_ANY, bmpIcon);
	sizerBottom->Add(imgIcon, 0, wxALIGN_CENTER | wxALL, 10);

	m_lblSoldi = new wxStaticText{ pnlBottom, wxID_ANY, "---" };
	wxFont fSoldi = m_lblSoldi->GetFont(); fSoldi.SetWeight(wxFONTWEIGHT_BOLD); fSoldi.SetPointSize(12);
	m_lblSoldi->SetFont(fSoldi);

	sizerBottom->Add(m_lblSoldi, 0, wxALL | wxALIGN_CENTER_VERTICAL, 20);
	sizerBottom->AddStretchSpacer();

	wxButton* btnEsci = new wxButton{ pnlBottom, wxID_CANCEL, "Esci", wxDefaultPosition, wxSize(150, 50) };
	sizerBottom->Add(btnEsci, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10);

	pnlBottom->SetSizer(sizerBottom);
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

// PANNELLO PRODOTTO
PnlProdotto::PnlProdotto(wxWindow* parent, DlgNegozio* mainDlg, TabbyGame& game, const Acquistabile& prod)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_RAISED),
	m_game(game), m_prodotto(prod), m_parentDlg(mainDlg)
{
	this->SetBackgroundColour(wxColour(255, 240, 170));

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	// 1. TITOLO
	wxStaticText* lblNome = new wxStaticText{ this, wxID_ANY, m_prodotto.GetNome() };
	wxFont fTitle = lblNome->GetFont();
	fTitle.SetWeight(wxFONTWEIGHT_BOLD);
	fTitle.SetPointSize(12);
	lblNome->SetFont(fTitle);
	lblNome->SetForegroundColour(wxColor(0, 0, 150));

	mainSizer->Add(lblNome, 0, wxALL, 8);
	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

	// 2. CORPO (Icona + Descrizione)
	wxBoxSizer* sizerBody = new wxBoxSizer{ wxHORIZONTAL };

	// Icona
	// Usiamo CaricaAsset (definita sopra in questo file)
	wxBitmap bmp = CaricaAsset(m_prodotto.GetImageStr());

	// Fallback se l'immagine manca
	if (!bmp.IsOk()) {
		bmp = wxBitmap(80, 80);
		wxMemoryDC dc(bmp);
		dc.SetBackground(wxBrush(wxColor(240, 240, 240))); // Grigino
		dc.Clear();
		dc.DrawText("NO IMG", 15, 30);
	}

	wxStaticBitmap* imgCtrl = new wxStaticBitmap{ this, wxID_ANY, bmp };

	// Aggiungi immagine al sizer
	sizerBody->Add(imgCtrl, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10);
	// Descrizione
	wxStaticText* lblDesc = new wxStaticText{ this, wxID_ANY, m_prodotto.GetDesc() };
	wxFont fDesc = lblDesc->GetFont();
	fDesc.SetPointSize(10);
	lblDesc->SetFont(fDesc);
	lblDesc->Wrap(350); // Manda a capo il testo per stare nella larghezza rimanente

	sizerBody->Add(lblDesc, 0, wxEXPAND | wxALL, 5);

	mainSizer->Add(sizerBody, 0, wxEXPAND | wxALL, 5); // Proportion 0 qui, lascia che sia il contenuto a spingere

	// 3. DATI E BOTTONE
	mainSizer->AddStretchSpacer();
	wxBoxSizer* footerSizer = new wxBoxSizer{ wxHORIZONTAL };

	wxStaticText* lblPrezzo = new wxStaticText{ this, wxID_ANY, m_game.GetSoldiStr(m_prodotto.GetPrezzo()) };
	wxFont fPrice = lblPrezzo->GetFont(); fPrice.SetWeight(wxFONTWEIGHT_BOLD); lblPrezzo->SetFont(fPrice);
	footerSizer->Add(lblPrezzo, 0, wxALIGN_CENTER_VERTICAL | wxALL, 15);

	footerSizer->AddStretchSpacer();

	// BOTTONE COMPRA VERDE
	wxButton* btnCompra = new wxButton{ this, wxID_ANY, "COMPRA", wxDefaultPosition, wxSize(90, 40) };
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

// DIALOG CONCESSIONARIO
DlgConcessionario::DlgConcessionario(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Concessionario", wxDefaultPosition, wxDefaultSize },
	m_game{ game }
{
	this->SetBackgroundColour(parent->GetBackgroundColour());
	this->SetFont(parent->GetFont());

	m_catalogoPtr = m_game.GetConcessionario().m_catalogo;

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };
	wxBoxSizer* sizerContent = new wxBoxSizer{ wxHORIZONTAL };

	// --- COLONNA SINISTRA: LISTA SCOOTER ---
	wxScrolledWindow* scrollList = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 350), wxBORDER_SUNKEN);
	scrollList->SetScrollRate(0, 20);
	wxBoxSizer* sizerList = new wxBoxSizer{ wxVERTICAL };

	for (size_t i = 0; i < m_catalogoPtr.size(); ++i)
	{
		Scooter* s = static_cast<Scooter*>(m_catalogoPtr[i]);

		wxPanel* pnlItem = new wxPanel(scrollList, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_RAISED);
		wxBoxSizer* sizerItem = new wxBoxSizer{ wxHORIZONTAL };

		// Immagine piccola sinistra
		wxBitmap bmpScooter = CaricaAsset(s->GetImageStr());
		bmpScooter = wxBitmap(bmpScooter.ConvertToImage().Rescale(60, bmpScooter.GetHeight() * 60 / bmpScooter.GetWidth(), wxIMAGE_QUALITY_BILINEAR));
		wxStaticBitmap* icon = new wxStaticBitmap(pnlItem, wxID_ANY, bmpScooter);

		// Radio Button (Senza stile wxRB_GROUP perché sono su parent diversi)
		wxRadioButton* radio = new wxRadioButton(pnlItem, 1000 + i, s->GetNome());

		// Evento click
		radio->Bind(wxEVT_RADIOBUTTON, &DlgConcessionario::OnSelezionaScooter, this);

		sizerItem->Add(icon, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
		sizerItem->Add(radio, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);

		pnlItem->SetSizer(sizerItem);
		sizerList->Add(pnlItem, 0, wxEXPAND | wxALL, 2);
	}

	scrollList->SetSizer(sizerList);
	sizerContent->Add(scrollList, 0, wxEXPAND | wxALL, 5);

	// --- COLONNA DESTRA: STATISTICHE ---
	wxPanel* pnlStats = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
	wxBoxSizer* sizerStats = new wxBoxSizer{ wxVERTICAL };

	// 1. IMMAGINE
	m_imgScooter = new wxStaticBitmap{ pnlStats, wxID_ANY, wxNullBitmap };

	// Centriamo l'immagine nel pannello di destra
	sizerStats->Add(m_imgScooter, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

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
	sizerStats->Add(new wxStaticText{ pnlStats, wxID_ANY, "Componenti" }, 0, wxALIGN_CENTER);
	sizerStats->Add(m_lblMarmitta, 0, wxEXPAND | wxALL, 5);
	sizerStats->Add(m_lblCarburatore, 0, wxEXPAND | wxALL, 5);
	sizerStats->Add(m_lblCilindrata, 0, wxEXPAND | wxALL, 5);
	sizerStats->Add(m_lblFiltro, 0, wxEXPAND | wxALL, 5);

	pnlStats->SetSizer(sizerStats);
	pnlStats->SetMinSize(wxSize(300, -1));

	// Assegnamo proporzione 1 alla parte destra così si prende tutto lo spazio rimasto
	sizerContent->Add(pnlStats, 1, wxEXPAND | wxALL, 5);

	// --- BARRA INFERIORE ---
	wxPanel* pnlBottom = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_RAISED);
	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };

	wxBitmap bmpIcon = CaricaAsset("ZSCOOTER.png");
	wxStaticBitmap* imgIcon = new wxStaticBitmap(pnlBottom, wxID_ANY, bmpIcon);
	sizerBottom->Add(imgIcon, 0, wxALIGN_CENTER | wxALL, 5);

	m_lblSoldi = new wxStaticText{ pnlBottom, wxID_ANY, "Soldi: " + m_game.GetSoldiStr(m_game.GetTabbyGuy().GetSoldi()) };
	wxFont fBold = m_lblSoldi->GetFont(); fBold.SetWeight(wxFONTWEIGHT_BOLD); m_lblSoldi->SetFont(fBold);

	wxButton* btnCompra = new wxButton{ pnlBottom, wxID_ANY, "Compra", wxDefaultPosition, wxSize(-1, 40) };
	wxButton* btnCancel = new wxButton{ pnlBottom, wxID_CANCEL, "Esci", wxDefaultPosition, wxSize(-1, 40) };

	btnCompra->Bind(wxEVT_BUTTON, &DlgConcessionario::OnCompra, this);

	sizerBottom->Add(m_lblSoldi, 1, wxALIGN_CENTER_VERTICAL | wxALL, 15);
	sizerBottom->Add(btnCompra, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	sizerBottom->Add(btnCancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	pnlBottom->SetSizer(sizerBottom);

	mainSizer->Add(sizerContent, 1, wxEXPAND);
	mainSizer->Add(pnlBottom, 0, wxEXPAND);

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

	wxBitmap bmp = CaricaAsset(s->GetImageStr());
	m_imgScooter->SetBitmap(bmp);
	m_imgScooter->Refresh();

	m_lblVelocita->SetLabel(std::to_string(s->GetVelocita()) + " km/h");
	m_lblMarmitta->SetLabel(s->GetMarmitta().GetNome());
	m_lblCarburatore->SetLabel(s->GetCarburatore().GetNome());
	m_lblCilindrata->SetLabel(s->GetCilindrata().GetNome());
	m_lblFiltro->SetLabel(s->GetFiltro().GetNome());

	m_lblCosto->SetLabel(m_game.GetSoldiStr(s->GetPrezzo()));

	// Aggiorna layout se i testi cambiano dimensione
	this->Fit();
	this->Layout();
}

void DlgConcessionario::OnCompra(wxCommandEvent& event)
{
	if (m_selectedIndex >= 0) {
		m_game.AzioneCompra(*m_catalogoPtr[m_selectedIndex]);
		EndModal(wxID_OK);
	}
}

// DIALOG TELEFONO
DlgTelefono::DlgTelefono(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Telefonino" },
	m_game{ game }
{
	this->SetFont(parent->GetFont());
	// Colore grigio plastica per il corpo del telefono
	this->SetBackgroundColour(parent->GetBackgroundColour());

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };
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
	mainSizer->Add(sizerBody);

	// SOLDI E TASTO OK
	wxPanel* pnlBottom = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };

	wxBitmap bmpIcon = CaricaAsset("ZTELEFONO.png");
	wxStaticBitmap* imgIcon = new wxStaticBitmap(pnlBottom, wxID_ANY, bmpIcon);
	sizerBottom->Add(imgIcon, 0, wxALIGN_CENTER | wxALL, 5);
	m_lblSoldi = new wxStaticText{ pnlBottom, wxID_ANY, "---" };
	wxButton* btnOk = new wxButton{ pnlBottom, wxID_OK, "OK", wxDefaultPosition, wxSize(60, 40) };

	sizerBottom->Add(m_lblSoldi, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	sizerBottom->AddStretchSpacer();
	sizerBottom->Add(btnOk, 0, wxALL, 5);

	pnlBottom->SetSizer(sizerBottom);
	mainSizer->Add(pnlBottom, 0, wxEXPAND | wxALL, 5);

	this->SetSizerAndFit(mainSizer);
	this->AggiornaInterfaccia();
}

void DlgTelefono::AggiornaInterfaccia()
{
	m_lblNomeTel->SetLabel(m_game.GetTabbyGuy().GetTelefono().GetNome());
	m_lblOperatore->SetLabel(m_game.GetTabbyGuy().GetTelefono().GetAbbonamento().GetNome());
	m_lblCredito->SetLabel(m_game.GetSoldiStr(m_game.GetTabbyGuy().GetTelefono().GetCredito()));
	m_lblSoldi->SetLabel("Soldi: " + m_game.GetSoldiStr(m_game.GetTabbyGuy().GetSoldi()));

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

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	// --- INFO UTENTE (Alto) ---
	wxPanel* pnlInfo = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
	wxBoxSizer* infoSizer = new wxBoxSizer{ wxVERTICAL };

	m_lblOperatoreAttuale = new wxStaticText{ pnlInfo, wxID_ANY, "---" };
	m_lblCreditoAttuale = new wxStaticText{ pnlInfo, wxID_ANY, "---" };

	// Grassetto per l'operatore
	wxFont fBold = m_lblOperatoreAttuale->GetFont();
	fBold.SetWeight(wxFONTWEIGHT_BOLD);
	m_lblOperatoreAttuale->SetFont(fBold);

	infoSizer->Add(m_lblOperatoreAttuale, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	infoSizer->Add(m_lblCreditoAttuale, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	pnlInfo->SetSizer(infoSizer);

	mainSizer->Add(pnlInfo, 0, wxEXPAND | wxALL, 10);

	// --- AREA SCORREVOLE PER GLI ABBONAMENTI ---
	wxScrolledWindow* scrollWin = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxSize(580, 380), wxBORDER_NONE);
	scrollWin->SetScrollRate(0, 20);

	wxBoxSizer* scrollSizer = new wxBoxSizer{ wxVERTICAL };

	// Recuperiamo la lista abbonamenti
	const std::vector<Abbonamento>& abbonamenti = m_game.GetAbbonamenti();

	for (int i = 0; i < abbonamenti.size(); ++i)
	{
		const Abbonamento& abb = abbonamenti[i];

		// Riquadro per ogni operatore
		wxStaticBoxSizer* groupSizer = new wxStaticBoxSizer(wxVERTICAL, scrollWin, abb.GetNome());

		// Contenitore Orizzontale: [LOGO] | [BOTTONI]
		wxBoxSizer* contentSizer = new wxBoxSizer{ wxHORIZONTAL };

		// 1. Placeholder Immagine (Sinistra)
		wxBitmap bmpAbb = CaricaAsset(abb.GetImageStr());
		// Ridimensionamento
		wxStaticBitmap* imgIncontro = new wxStaticBitmap(scrollWin, wxID_ANY, bmpAbb);
		contentSizer->Add(imgIncontro, 0, wxALIGN_CENTER | wxALL, 5);

		// 2. Colonna Bottoni (Destra)
		wxBoxSizer* rightSizer = new wxBoxSizer{ wxVERTICAL };

		// Tasto Attivazione
		wxString labelAttiva = "Nuova Scheda (" + m_game.GetSoldiStr(abb.GetCostoAttivazione()) + ")";
		wxButton* btnAttiva = new wxButton{ scrollWin, wxID_ANY, labelAttiva, wxDefaultPosition, wxSize(-1, 30) };
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
			wxButton* btnRic = new wxButton{ scrollWin, wxID_ANY, labelRic };

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
	wxPanel* pnlBottom = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(-1, 70), wxBORDER_RAISED);
	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };

	wxBitmap bmpIcon = CaricaAsset("ZTELEFONO.png");
	wxStaticBitmap* imgIcon = new wxStaticBitmap(pnlBottom, wxID_ANY, bmpIcon);
	sizerBottom->Add(imgIcon, 0, wxALIGN_CENTER | wxALL, 5);

	m_lblSoldi = new wxStaticText{ pnlBottom, wxID_ANY, "---" };
	wxFont fSoldi = m_lblSoldi->GetFont();
	fSoldi.SetWeight(wxFONTWEIGHT_BOLD);
	m_lblSoldi->SetFont(fSoldi);

	wxButton* btnChiudi = new wxButton{ pnlBottom, wxID_CANCEL, "Chiudi", wxDefaultPosition, wxSize(-1, 40) };

	sizerBottom->Add(m_lblSoldi, 0, wxALIGN_CENTER_VERTICAL | wxALL, 15);
	sizerBottom->AddStretchSpacer();
	sizerBottom->Add(btnChiudi, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);

	pnlBottom->SetSizer(sizerBottom);
	mainSizer->Add(pnlBottom, 0, wxEXPAND);

	this->SetSizerAndFit(mainSizer);
	this->Centre();

	this->AggiornaInterfaccia();
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