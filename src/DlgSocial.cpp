#include "DlgSocial.h"

// DIALOG COMPAGNIA
DlgCompagnia::DlgCompagnia(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Compagnia", wxDefaultPosition, wxDefaultSize },
	m_game{ game }
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour());
	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	wxPanel* pnlFoto = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxSize(300, 200), wxBORDER_SUNKEN };
	wxBoxSizer* sizerFoto = new wxBoxSizer{ wxVERTICAL };
	wxPanel* pnlButtons = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerButtons = new wxBoxSizer{ wxVERTICAL };
	wxPanel* pnlInfoOk = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerInfoOk = new wxBoxSizer{ wxHORIZONTAL };

	wxBitmap bmpCompa = CaricaAsset("compagnia.png");
	// Ridimensionamento
	bmpCompa = wxBitmap(bmpCompa.ConvertToImage().Rescale(300, bmpCompa.GetHeight() * 300 / bmpCompa.GetWidth(), wxIMAGE_QUALITY_BILINEAR));
	wxStaticBitmap* imgCompa = new wxStaticBitmap(pnlFoto, wxID_ANY, bmpCompa);

	sizerFoto->Add(imgCompa, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	pnlFoto->SetSizer(sizerFoto);
	mainSizer->Add(pnlFoto, 0, wxEXPAND | wxALL, 5);

	// Bottoni
	wxButton* btnGara = new wxButton{ pnlButtons, wxID_ANY, "Gareggia con lo Scooter", wxDefaultPosition, wxSize(-1,40) };
	wxButton* btnEsci = new wxButton{ pnlButtons, wxID_ANY, "Esci con la Compagnia", wxDefaultPosition, wxSize(-1,40) };
	wxButton* btnChiama = new wxButton{ pnlButtons, wxID_ANY, "Chiama la Compagnia", wxDefaultPosition, wxSize(-1,40) };

	btnGara->Bind(wxEVT_BUTTON, &DlgCompagnia::OnGara, this);
	btnEsci->Bind(wxEVT_BUTTON, &DlgCompagnia::OnEsci, this);
	btnChiama->Bind(wxEVT_BUTTON, &DlgCompagnia::OnChiama, this);

	sizerButtons->Add(btnGara, 0, wxEXPAND | wxTOP | wxLEFT | wxRIGHT, 5);
	sizerButtons->Add(btnEsci, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);
	sizerButtons->Add(btnChiama, 0, wxEXPAND | wxBOTTOM | wxLEFT | wxRIGHT, 5);

	pnlButtons->SetSizer(sizerButtons);
	mainSizer->Add(pnlButtons, 0, wxEXPAND | wxALL, 5);

	// Info e ok
	m_lblRep = new wxStaticText{ pnlInfoOk, wxID_ANY, "---" };
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
	this->Fit();
	this->Layout();
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
	wxPanel* pnlBottom = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };

	// Bottoni
	wxButton* btnAumento = new wxButton{ pnlButtons, wxID_ANY, "Chiedi aumento paghetta", wxDefaultPosition, wxSize(-1,40) };
	wxButton* btnSoldiExtra = new wxButton{ pnlButtons, wxID_ANY, "Chiedi soldi extra", wxDefaultPosition, wxSize(-1,40) };
	wxButton* btnChiediSoldi = new wxButton{ pnlButtons, wxID_ANY, "Papà, mi dai " + m_game.GetSoldiStr(50) + " ?", wxDefaultPosition, wxSize(-1,40) };

	btnAumento->Bind(wxEVT_BUTTON, &DlgFamiglia::OnAumentoPaghetta, this);
	btnSoldiExtra->Bind(wxEVT_BUTTON, &DlgFamiglia::OnSoldiExtra, this);
	btnChiediSoldi->Bind(wxEVT_BUTTON, &DlgFamiglia::OnChiediSoldi, this);

	sizerButtons->Add(btnAumento, 0, wxEXPAND | wxTOP | wxRIGHT | wxLEFT, 5);
	sizerButtons->Add(btnSoldiExtra, 0, wxEXPAND | wxRIGHT | wxLEFT, 5);
	sizerButtons->Add(btnChiediSoldi, 0, wxEXPAND | wxRIGHT | wxLEFT | wxBOTTOM, 5);

	pnlButtons->SetSizer(sizerButtons);
	mainSizer->Add(pnlButtons, 0, wxEXPAND | wxALL, 5);

	// Info e ok
	wxBitmap bmpFam = CaricaAsset("ZFAMIGLIA.png");
	wxStaticBitmap* imgFam = new wxStaticBitmap(pnlBottom, wxID_ANY, bmpFam);
	sizerBottom->Add(imgFam, 0, wxALIGN_CENTER | wxALL, 5);

	m_lblSoldi = new wxStaticText{ pnlBottom, wxID_ANY, "---" };
	m_lblPaghetta = new wxStaticText{ pnlBottom, wxID_ANY, "---" };
	wxButton* btnOk = new wxButton{ pnlBottom, wxID_OK, "OK", wxDefaultPosition, wxSize(60, 50) };

	// Creiamo un sizer VERTICALE solo per le scritte
	wxBoxSizer* sizerTesti = new wxBoxSizer{ wxVERTICAL };
	sizerTesti->Add(m_lblSoldi, 0, wxBOTTOM, 2);
	sizerTesti->Add(m_lblPaghetta, 0, wxTOP, 2);

	// Ora aggiungiamo al sizer orizzontale principale del pannello
	sizerBottom->Add(sizerTesti, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);
	sizerBottom->AddSpacer(50);

	// 2. Il bottone OK (fisso a destra)
	sizerBottom->Add(btnOk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	pnlBottom->SetSizer(sizerBottom);
	mainSizer->Add(pnlBottom, 0, wxEXPAND | wxALL, 5);

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

// DIALOG TIPA
DlgTipa::DlgTipa(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Tipa" },
	m_game{ game }
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour());

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	// --- CORPO CENTRALE (Foto + Bottoni) ---
	wxBoxSizer* sizerBody = new wxBoxSizer{ wxHORIZONTAL };

	// FOTO (Sinistra)
	wxPanel* pnlFoto = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerFoto = new wxBoxSizer{ wxVERTICAL };
	double scaleFactor = 1.0;
	wxBitmap bmpTipa;

	switch (m_game.GetMoodTipa())
	{
	case MoodTipa::ESTIVO:
		bmpTipa = CaricaAsset("tipa_estiva.png");
		break;

	case MoodTipa::NATALIZIO:
		bmpTipa = CaricaAsset("tipa_natalizia.png");
		break;
	default:
		bmpTipa = CaricaAsset("tipa.png");
	}
	
	wxStaticBitmap* imgTipa = new wxStaticBitmap(pnlFoto, wxID_ANY, bmpTipa);
	imgTipa->SetCursor(wxCursor(wxCURSOR_HAND)); // Manina quando passi sopra
	imgTipa->Bind(wxEVT_LEFT_DOWN, [this, scaleFactor](wxMouseEvent& ev) {
		// Prendi posizione mouse
		wxPoint p = ev.GetPosition();

		// Riporta le coordinate alla dimensione originale del Tabboz (1999)
		int x = (int)(p.x / scaleFactor);
		int y = (int)(p.y / scaleFactor);

		// --- LOGICA ORIGINALE TABIMG.C ---
		// Tette: X[138-170], Y[50-65]
		bool toccaTette = ((x >= 138) && (x <= 170)) && ((y >= 50) && (y <= 65));

		// Culo: X[104-136], Y[78-166]
		bool toccaCulo = ((x >= 104) && (x <= 136)) && ((y >= 78) && (y <= 166));

		if (toccaTette || toccaCulo)
		{
			// Chiama la logica di gioco
			m_game.AzionePalpatina();

			// Mostra il messaggio (pop-up)
			ManifestaEventi(this, m_game);

			// Aggiorna la barra rapporti che potrebbe essere cambiata
			this->AggiornaInterfaccia();
		}
		else
			ev.Skip(); // Click normale
		});

	sizerFoto->Add(imgTipa, 0, wxALIGN_CENTER | wxALL, 5);
	pnlFoto->SetSizer(sizerFoto);
	sizerBody->Add(pnlFoto, 0, wxEXPAND);

	// BOTTONI (Destra)
	wxPanel* pnlButtons = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerBtn = new wxBoxSizer{ wxVERTICAL };

	wxButton* btnCerca = new wxButton{ pnlButtons, wxID_ANY, "Cerca tipa", wxDefaultPosition, wxSize(-1, 40) };
	wxButton* btnLascia = new wxButton{ pnlButtons, wxID_ANY, "Lascia Tipa", wxDefaultPosition, wxSize(-1, 40) };
	wxButton* btnTelefona = new wxButton{ pnlButtons, wxID_ANY, "Telefona alla tipa", wxDefaultPosition, wxSize(-1, 40) };
	wxButton* btnEsci = new wxButton{ pnlButtons, wxID_ANY, "Esci con la tipa", wxDefaultPosition, wxSize(-1, 40) };

	// Bind eventi
	btnCerca->Bind(wxEVT_BUTTON, &DlgTipa::OnCercaTipa, this);
	btnLascia->Bind(wxEVT_BUTTON, &DlgTipa::OnLasciaTipa, this);
	btnTelefona->Bind(wxEVT_BUTTON, &DlgTipa::OnTelefonaTipa, this);
	btnEsci->Bind(wxEVT_BUTTON, &DlgTipa::OnEsciTipa, this);

	sizerBtn->Add(btnCerca, 0, wxEXPAND | wxALL & ~wxBOTTOM, 5);
	sizerBtn->Add(btnLascia, 0, wxEXPAND | wxALL & ~wxTOP, 5);
	sizerBtn->Add(new wxStaticLine(pnlButtons), 0, wxEXPAND | wxALL, 5);
	sizerBtn->Add(btnTelefona, 0, wxEXPAND | wxALL & ~wxBOTTOM, 5);
	sizerBtn->Add(btnEsci, 0, wxEXPAND | wxALL & ~wxTOP, 5);

	pnlButtons->SetSizer(sizerBtn);
	sizerBody->Add(pnlButtons);

	mainSizer->Add(sizerBody, 0, wxEXPAND | wxALL, 5);

	// --- PANNELLO INFO (Sotto) ---
	wxPanel* pnlInfo = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerInfo = new wxBoxSizer{ wxVERTICAL };

	// Label statiche e Valori dinamici
	m_lblNome = new wxStaticText{ pnlInfo, wxID_ANY, "" };
	wxFont fBold = m_lblNome->GetFont(); fBold.SetWeight(wxFONTWEIGHT_BOLD); m_lblNome->SetFont(fBold);
	sizerInfo->Add(m_lblNome, 0, wxALIGN_LEFT);

	m_lblFamaTipa = new wxStaticText{ pnlInfo, wxID_ANY, "" };
	sizerInfo->Add(m_lblFamaTipa, 0, wxALIGN_LEFT);

	m_lblRapportiTipa = new wxStaticText{ pnlInfo, wxID_ANY, "" };
	sizerInfo->Add(m_lblRapportiTipa, 0, wxALIGN_LEFT);

	// Centriamo la griglia nel pannello
	wxBoxSizer* infoWrapper = new wxBoxSizer{ wxVERTICAL };
	infoWrapper->Add(sizerInfo, 1, wxALL, 10);
	pnlInfo->SetSizer(infoWrapper);

	mainSizer->Add(pnlInfo, 0, wxEXPAND | wxALL, 5);

	// --- FOOTER (Soldi + OK) ---
	wxPanel* pnlFooter = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxSize(-1, 70), wxBORDER_SUNKEN };
	wxBoxSizer* sizerFooter = new wxBoxSizer{ wxHORIZONTAL };

	// Icona e figosità
	wxBitmap bmpIcon = CaricaAsset("ZTIPA.png");
	wxStaticBitmap* imgIcon = new wxStaticBitmap(pnlFooter, wxID_ANY, bmpIcon);
	sizerFooter->Add(imgIcon, 0, wxALIGN_CENTER | wxALL, 5);
	m_lblFama = new wxStaticText{ pnlFooter, wxID_ANY, "" };
	sizerFooter->Add(m_lblFama, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);

	sizerFooter->AddStretchSpacer();

	wxButton* btnOk = new wxButton{ pnlFooter, wxID_OK, "OK", wxDefaultPosition, wxSize(70, 40) };
	sizerFooter->Add(btnOk, 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);

	pnlFooter->SetSizer(sizerFooter);
	mainSizer->Add(pnlFooter, 0, wxEXPAND | wxALL, 5);

	this->SetSizerAndFit(mainSizer);

	// Aggiorniamo lo stato iniziale (bottoni abilitati/disabilitati)
	this->AggiornaInterfaccia();

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
		m_lblNome->SetLabel("Nome: " + tipa.GetNome());
		m_lblFamaTipa->SetLabel("Figosità: " + std::to_string(tipa.GetFama()) + "/100");
		m_lblRapportiTipa->SetLabel("Quanto state bene insieme: " + std::to_string(m_game.GetTabbyGuy().GetRapporti()) + "/100");
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

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxHORIZONTAL };

	wxPanel* pnlInfo = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerInfo = new wxBoxSizer{ wxVERTICAL };

	wxPanel* pnlFoto = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerFoto = new wxBoxSizer{ wxVERTICAL };

	sizerInfo->Add(new wxStaticText{ pnlInfo, wxID_ANY, "Nome:" }, 0, wxALL & ~wxBOTTOM, 5);
	sizerInfo->Add(new wxStaticText{ pnlInfo, wxID_ANY, m_tipa.GetNome(), wxDefaultPosition, wxSize(250, -1), wxALIGN_CENTER | wxBORDER_SUNKEN }, 0, wxALL, 5);
	sizerInfo->Add(new wxStaticText{ pnlInfo, wxID_ANY, "Figosità:" }, 0, wxALL & ~wxBOTTOM, 5);
	sizerInfo->Add(new wxStaticText{ pnlInfo, wxID_ANY, std::to_string(m_tipa.GetFama()) + "/100", wxDefaultPosition, wxSize(250, -1), wxALIGN_CENTER | wxBORDER_SUNKEN }, 0, wxALL, 5);
	sizerInfo->Add(new wxStaticText{ pnlInfo, wxID_ANY, "Giudizio\ncomplessivo:" }, 0, wxALL & ~wxBOTTOM, 5);
	sizerInfo->Add(new wxStaticText{ pnlInfo, wxID_ANY, m_tipa.GetDesc(), wxDefaultPosition, wxSize(250, -1), wxALIGN_CENTER | wxBORDER_SUNKEN }, 0, wxALL, 5);
	sizerInfo->AddStretchSpacer();

	wxBitmap bmpIcon = CaricaAsset("ZTIPA.png");
	wxStaticBitmap* imgIcon = new wxStaticBitmap(pnlInfo, wxID_ANY, bmpIcon);
	sizerInfo->Add(imgIcon, 0, wxALL, 5);

	wxButton* btnProvaci = new wxButton{ pnlInfo, wxID_ANY, "Ci provo !", wxDefaultPosition, wxSize(-1, 40) };
	wxButton* btnLasciaStare = new wxButton{ pnlInfo, wxID_ANY, "Ritorno a casa...", wxDefaultPosition, wxSize(-1, 40) };
	btnProvaci->Bind(wxEVT_BUTTON, &DlgIncontroTipa::OnProvaci, this);
	btnLasciaStare->Bind(wxEVT_BUTTON, &DlgIncontroTipa::OnLasciaStare, this);
	sizerInfo->Add(btnProvaci, 0, wxEXPAND | wxALL & ~wxBOTTOM, 5);
	sizerInfo->Add(btnLasciaStare, 0, wxEXPAND | wxALL & ~wxTOP, 5);

	pnlInfo->SetSizer(sizerInfo);
	mainSizer->Add(pnlInfo, 0, wxEXPAND | wxALL, 5);

	// Pannello foto
	wxBitmap bmpIncontro = CaricaAsset("incontro.png");
	// Ridimensionamento
	bmpIncontro = wxBitmap(bmpIncontro.ConvertToImage().Rescale(bmpIncontro.GetWidth() * 1.5f, bmpIncontro.GetHeight() * 1.5f, wxIMAGE_QUALITY_BILINEAR));
	wxStaticBitmap* imgIncontro = new wxStaticBitmap(pnlFoto, wxID_ANY, bmpIncontro);
	sizerFoto->Add(imgIncontro, 0, wxALIGN_CENTER | wxALL, 5);
	pnlFoto->SetSizer(sizerFoto);
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