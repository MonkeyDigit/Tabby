#include "DlgAttivita.h"

// DIALOG SCUOLA
DlgScuola::DlgScuola(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Scuola", wxDefaultPosition, wxDefaultSize },
	m_game{ game }, m_materiaIndex{ 0 }
{
	this->SetBackgroundColour(parent->GetBackgroundColour());
	this->SetFont(parent->GetFont());

	const Scuola& scuolaref = m_game.GetTabbyGuy().GetScuola();

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };
	wxBoxSizer* sizerBody = new wxBoxSizer{ wxHORIZONTAL };

	// Pannello bottoni
	wxPanel* pnlButtons = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerButtons = new wxBoxSizer{ wxVERTICAL };
	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };

	// Creiamo i bottoni, inizialmente vuoti, per poi aggiornarli come si deve in AggiornaInterfaccia...
	// Siccome i bottoni sono già membri della classe DlgScuola, va bene wxID_ANY, perché possediamo già i loro puntatori
	m_btnStudia = new wxButton{ pnlButtons, wxID_ANY, "---", wxDefaultPosition, wxSize(530, 30) };
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
		m_lblVoti.push_back(new wxStaticText{ pnlVoti, wxID_ANY, "---", wxDefaultPosition, wxSize(45, 25), wxBORDER_SUNKEN | wxALIGN_CENTER_HORIZONTAL });

		// Aggiungiamo alla griglia
		gridVoti->Add(radio, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
		gridVoti->Add(m_lblVoti[i], 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

		// Binding
		// Sfrutto una funzione lambda per associare la materia che viene selezionata ai bottoni del pannello a destra, perché sono troppo intelligente...
		radio->Bind(wxEVT_RADIOBUTTON, [this, i](wxCommandEvent& ev) {
			m_materiaIndex = i;
			this->AggiornaInterfaccia();
			});
	}

	// Creo un sizer "invisibile" che serve a dare MARGINE (altrimenti metallurgia sprofonda)
	wxBoxSizer* wrapperSizer = new wxBoxSizer{ wxVERTICAL };
	//Inserisco la griglia dentro questo wrapper con 5px di respiro ovunque(wxALL)
	wrapperSizer->Add(gridVoti, 1, wxEXPAND | wxALL, 5);
	// Profitto scolastico
	m_lblStudio = new wxStaticText{ pnlVoti, wxID_ANY, "---", wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_VERTICAL };
	wrapperSizer->Add(new wxStaticLine(pnlVoti), 0, wxEXPAND | wxALL, 5);
	wrapperSizer->Add(m_lblStudio, 0, wxALL, 5);
	// Settiamo il sizer
	pnlVoti->SetSizer(wrapperSizer);
	sizerBody->Add(pnlVoti, 0, wxEXPAND | wxALL, 5);

	// PANNELLO DI DESTRA -------------------
	// Immagine
	wxBitmap bmpScuola = CaricaAsset("scuola.png");
	// Ridimensionamento
	bmpScuola = wxBitmap(bmpScuola.ConvertToImage().Rescale(400, bmpScuola.GetHeight() * 400 / bmpScuola.GetWidth(), wxIMAGE_QUALITY_BILINEAR));
	wxStaticBitmap* imgScuola = new wxStaticBitmap(pnlButtons, wxID_ANY, bmpScuola);

	sizerButtons->Add(imgScuola, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	sizerButtons->AddStretchSpacer();

	// Bottoni del pannello a destra
	sizerButtons->Add(m_btnStudia, 0, wxALIGN_CENTER_HORIZONTAL | wxRIGHT | wxLEFT, 5);
	sizerButtons->Add(m_btnMinaccia, 0, wxALIGN_CENTER_HORIZONTAL | wxRIGHT | wxLEFT, 5);
	sizerButtons->Add(m_btnCorrompi, 0, wxALIGN_CENTER_HORIZONTAL | wxRIGHT | wxLEFT | wxBOTTOM, 5);
	// Settiamo il sizer
	pnlButtons->SetSizer(sizerButtons);
	sizerBody->Add(pnlButtons, 0, wxEXPAND | wxALL, 5);
	mainSizer->Add(sizerBody, 0);

	// Pannello reputazione e tasto ok
	wxPanel* pnlBottom = new wxPanel{ this,wxID_ANY, wxDefaultPosition, wxSize(-1, 50), wxBORDER_SUNKEN };

	wxBitmap bmpDoc = CaricaAsset("ZSCUOLA.png");
	wxStaticBitmap* imgDoc = new wxStaticBitmap(pnlBottom, wxID_ANY, bmpDoc);
	sizerBottom->Add(imgDoc, 0, wxALIGN_CENTER | wxALL, 5);

	m_lblRep = new wxStaticText{ pnlBottom, wxID_ANY, "---" };
	sizerBottom->Add(m_lblRep, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	wxButton* btnOk = new wxButton{ pnlBottom, wxID_OK, "OK", wxDefaultPosition, wxSize(60, 50) };
	sizerBottom->AddStretchSpacer();
	sizerBottom->Add(btnOk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	pnlBottom->SetSizer(sizerBottom);

	mainSizer->Add(pnlBottom, 0, wxEXPAND | wxALL & ~wxTOP, 5);

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
	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };
	wxBoxSizer* sizerBody = new wxBoxSizer{ wxHORIZONTAL };
	wxPanel* pnlFoto = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerFoto = new wxBoxSizer{ wxVERTICAL };
	wxPanel* pnlButtons = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerButtons = new wxBoxSizer{ wxVERTICAL };
	wxPanel* pnlBottom = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };
	wxFlexGridSizer* sizerInfo = new wxFlexGridSizer{ 2, 5, 10 };

	// Immagine
	wxBitmap bmpLavoro = CaricaAsset("lavoro.png");
	// Ridimensionamento
	//bmpLavoro = wxBitmap(bmpLavoro.ConvertToImage().Rescale(400, bmpLavoro.GetHeight() * 400 / bmpLavoro.GetWidth(), wxIMAGE_QUALITY_BILINEAR));
	wxStaticBitmap* imgLavoro = new wxStaticBitmap(pnlFoto, wxID_ANY, bmpLavoro);
	sizerFoto->Add(imgLavoro, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	pnlFoto->SetSizer(sizerFoto);
	sizerBody->Add(pnlFoto, 0, wxALL, 5);
	sizerBody->AddStretchSpacer();

	// Bottoni
	wxButton* btnCercaLavoro = new wxButton{ pnlButtons, wxID_ANY, "Cerca lavoro", wxDefaultPosition, wxSize(-1, 40) };
	wxButton* btnLicenziati = new wxButton{ pnlButtons, wxID_ANY, "Licenziati", wxDefaultPosition, wxSize(-1, 40) };
	wxButton* btnInformazioni = new wxButton{ pnlButtons, wxID_ANY, "Elenco Ditte", wxDefaultPosition, wxSize(-1, 40) };
	wxButton* btnLavora = new wxButton{ pnlButtons, wxID_ANY, "Lavora", wxDefaultPosition, wxSize(-1, 40) };
	wxButton* btnLeccaculo = new wxButton{ pnlButtons, wxID_ANY, "Fai il Leccaculo", wxDefaultPosition, wxSize(-1, 40) };
	wxButton* btnAumento = new wxButton{ pnlButtons, wxID_ANY, "Chiedi aumento salario", wxDefaultPosition, wxSize(-1,40) };
	wxButton* btnSciopera = new wxButton{ pnlButtons, wxID_ANY, "Sciopera", wxDefaultPosition, wxSize(-1, 40) };

	btnCercaLavoro->Bind(wxEVT_BUTTON, &DlgLavoro::OnCercaLavoro, this);
	btnLicenziati->Bind(wxEVT_BUTTON, &DlgLavoro::OnLicenziati, this);
	btnInformazioni->Bind(wxEVT_BUTTON, &DlgLavoro::OnInformazioni, this);
	btnLavora->Bind(wxEVT_BUTTON, &DlgLavoro::OnLavora, this);
	btnLeccaculo->Bind(wxEVT_BUTTON, &DlgLavoro::OnLeccaculo, this);
	btnAumento->Bind(wxEVT_BUTTON, &DlgLavoro::OnAumento, this);
	btnSciopera->Bind(wxEVT_BUTTON, &DlgLavoro::OnSciopera, this);

	sizerButtons->Add(btnCercaLavoro, 0, wxEXPAND | wxTOP | wxRIGHT | wxLEFT, 5);
	sizerButtons->Add(btnLicenziati, 0, wxEXPAND | wxALL & ~wxTOP, 5);
	sizerButtons->Add(btnInformazioni, 0, wxEXPAND | wxALL, 5);
	sizerButtons->Add(btnLavora, 0, wxEXPAND | wxALL & ~wxBOTTOM, 5);
	sizerButtons->Add(btnLeccaculo, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);
	sizerButtons->Add(btnAumento, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);
	sizerButtons->Add(btnSciopera, 0, wxEXPAND | wxALL & ~wxTOP, 5);

	pnlButtons->SetSizer(sizerButtons);
	sizerBody->Add(pnlButtons, 0, wxALL, 5);

	// Icona
	wxBitmap bmpIcon = CaricaAsset("ZLAVORO.png");
	wxStaticBitmap* imgIcon = new wxStaticBitmap(pnlBottom, wxID_ANY, bmpIcon);
	sizerBottom->Add(imgIcon, 0, wxALIGN_CENTER | wxALL, 5);

	// Info e ok
	m_lblDitta = new wxStaticText{ pnlBottom, wxID_ANY, "---" };
	m_lblImpegno = new wxStaticText{ pnlBottom, wxID_ANY, "---" };
	m_lblSoldi = new wxStaticText{ pnlBottom, wxID_ANY, "---" };
	m_lblStipendio = new wxStaticText{ pnlBottom, wxID_ANY, "---" };

	sizerInfo->Add(m_lblDitta, 0, wxEXPAND | wxALL, 10);
	sizerInfo->Add(m_lblImpegno, 0, wxEXPAND | wxALL, 10);
	sizerInfo->Add(m_lblSoldi, 0, wxEXPAND | wxALL, 10);
	sizerInfo->Add(m_lblStipendio, 0, wxEXPAND | wxALL, 10);

	sizerBottom->Add(sizerInfo);

	wxButton* btnOk = new wxButton{ pnlBottom, wxID_OK, "OK", wxDefaultPosition, wxSize(60, 50) };
	sizerBottom->AddStretchSpacer();
	sizerBottom->Add(btnOk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
	pnlBottom->SetSizer(sizerBottom);

	mainSizer->Add(sizerBody, 0, wxEXPAND);
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
	m_game{ game }
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(wxColor(25, 25, 25));
	this->SetForegroundColour(*wxRED);

	// Chiediamo al CERVELLO (TabbyGame) quale lavoro esce per il nostro tabbozzo
	const Ditta& ditta = m_game.ProponiDitta();
	m_ditta = ditta.m_nome;

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };
	wxPanel* pnlFoto = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerFoto = new wxBoxSizer{ wxVERTICAL };
	// Immagine
	wxBitmap bmpDitta = CaricaAsset(ditta.m_img);
	wxStaticBitmap* imgDitta = new wxStaticBitmap(pnlFoto, wxID_ANY, bmpDitta);
	sizerFoto->Add(imgDitta, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	pnlFoto->SetSizer(sizerFoto);
	mainSizer->Add(pnlFoto, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	// Linea di separazione
	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

	// Descrizione
	wxStaticText* lblNomeDitta = new wxStaticText{ this, wxID_ANY, "Ditta: " + ditta.m_nome };
	wxStaticText* lblSedeDitta = new wxStaticText{ this, wxID_ANY, "Sede: " + ditta.m_sede };
	wxStaticText* lblDesc = new wxStaticText{ this, wxID_ANY, ditta.m_offerta.m_descrizione };
	lblDesc->Wrap(600); // Manda a capo il testo se troppo lungo
	mainSizer->Add(lblNomeDitta, 0, wxALL, 10);
	mainSizer->Add(lblSedeDitta, 0, wxALL, 10);
	mainSizer->Add(lblDesc, 0, wxALL, 10);

	// Linea di separazione
	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

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
	this->EndModal(wxID_ANY);
	DlgQuiz dlg{ this, m_game, m_ditta };
	dlg.Centre();
	dlg.ShowModal();
}

void DlgOffertaLavoro::OnRifiuta(wxCommandEvent& event)
{
	m_game.AzioneRifiutaProposta();
	this->EndModal(wxID_ANY);
}

// DIALOG QUESTIONARIO DI LAVORO
DlgQuiz::DlgQuiz(wxWindow* parent, TabbyGame& game, std::string ditta)
	: wxDialog{ parent, wxID_ANY, "", wxDefaultPosition, wxDefaultSize,  wxCAPTION },
	m_game{ game }, m_ditta{ ditta }
{
	const QuizScheda& quiz = m_game.AssegnaQuiz();
	this->SetTitle(quiz.m_titolo);
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(WIN_BKG);

	// Inizializziamo il vettore per contenere 3 gruppi di risposte
	m_gruppiRisposte.resize(3);

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };
	wxPanel* pnlMain = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* pnlSizer = new wxBoxSizer{ wxVERTICAL };

	// INTRODUZIONE
	wxStaticText* lblIntro = new wxStaticText{ pnlMain, wxID_ANY, quiz.m_intro, wxDefaultPosition, wxSize(-1, -1), wxALIGN_CENTER };
	lblIntro->Wrap(700);
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
	wxButton* btnFinito = new wxButton{ pnlMain, wxID_ANY, "Clicca qui quando hai finito di fare il test-a di cazzo!", wxDefaultPosition, wxSize(-1, 40) };
	btnFinito->Bind(wxEVT_BUTTON, &DlgQuiz::OnFinito, this);
	pnlSizer->Add(btnFinito, 0, wxALL | wxALIGN_CENTER, 15);


	pnlMain->SetSizer(pnlSizer);
	mainSizer->Add(pnlMain, 1, wxEXPAND | wxALL, 5);

	this->SetSizerAndFit(mainSizer);
	this->Centre();
}

void DlgQuiz::AggiungiDomanda(wxWindow* parent, wxSizer* sizer, const QuizDomanda& domanda, int indiceDomanda)
{
	wxStaticText* txtDomanda = new wxStaticText{ parent, wxID_ANY, domanda.m_testo };
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
	wxStaticText* lblTitolo = new wxStaticText{ this, wxID_ANY, "DATABASE AZIENDE E MULTINAZIONALI" };
	wxFont fontTitolo = lblTitolo->GetFont();
	fontTitolo.SetWeight(wxFONTWEIGHT_BOLD);
	fontTitolo.SetPointSize(12);
	lblTitolo->SetFont(fontTitolo);

	mainSizer->Add(lblTitolo, 0, wxALL | wxALIGN_CENTER, 10);
	mainSizer->Add(new wxStaticText{ this, wxID_ANY, "Seleziona una ditta per visualizzare ulteriori informazioni" }, wxALIGN_CENTER | wxALL, 5);

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
	wxButton* btnChiudi = new wxButton{ this, wxID_CANCEL, "Chiudi", wxDefaultPosition, wxSize(100, 35) };
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

	// 1. Immagine
	wxPanel* pnlFoto = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerFoto = new wxBoxSizer{ wxVERTICAL };
	wxBitmap bmpDitta = CaricaAsset(ditta.m_img);
	wxStaticBitmap* imgDitta = new wxStaticBitmap(pnlFoto, wxID_ANY, bmpDitta);
	sizerFoto->Add(imgDitta, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
	pnlFoto->SetSizer(sizerFoto);
	mainSizer->Add(pnlFoto, 0, wxEXPAND | wxALL, 5);

	// Separatore
	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxALL, 10);

	// 2. DATI PRINCIPALI (Nome, Sede, Fatturato)
	wxBoxSizer* headerSizer = new wxBoxSizer{ wxVERTICAL };

	// Nome Ditta (Grande e Grassetto)
	wxStaticText* lblNome = new wxStaticText{ this, wxID_ANY, ditta.m_nome };
	wxFont fontGrande = lblNome->GetFont();
	fontGrande.SetPointSize(14);
	fontGrande.SetWeight(wxFONTWEIGHT_BOLD);
	lblNome->SetFont(fontGrande);

	headerSizer->Add(lblNome, 0, wxALL, 10);

	// Sede
	headerSizer->Add(new wxStaticText{ this, wxID_ANY, "Sede: " + ditta.m_sede }, 0, wxALL, 5);

	// Fatturato
	wxString fatturatoStr;
	if (ditta.m_fatturato <= 0)
		fatturatoStr = "Sconosciuto";
	else
		fatturatoStr = game.GetSoldiStr(ditta.m_fatturato);

	headerSizer->Add(new wxStaticText{ this, wxID_ANY, "Fatturato annuo: " + fatturatoStr }, 0, wxALL, 5);

	mainSizer->Add(headerSizer, 0, wxEXPAND | wxALL, 5);

	// Separatore
	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxALL, 10);

	// 3. DESCRIZIONI (Presentazione e Produzioni)
	wxBoxSizer* descSizer = new wxBoxSizer{ wxVERTICAL };

	// Presentazione
	if (!ditta.m_presentazione.empty())
	{
		wxStaticText* lblPres = new wxStaticText{ this, wxID_ANY, ditta.m_presentazione };
		lblPres->Wrap(550); // Manda a capo
		descSizer->Add(lblPres, 0, wxALL, 5);
	}

	// Produzioni (Elenco prodotti)
	if (!ditta.m_produzioni.empty())
	{
		wxStaticText* lblProd = new wxStaticText{ this, wxID_ANY, ditta.m_produzioni };
		lblProd->Wrap(550);
		lblProd->SetForegroundColour(*wxBLUE);
		descSizer->Add(lblProd, 0, wxALL, 5);
	}

	mainSizer->Add(descSizer, 0, wxEXPAND | wxALL, 10);

	// Separatore
	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxALL, 5);

	// BOTTONE CHIUDI
	wxButton* btnOk = new wxButton{ this, wxID_OK, "Chiudi", wxDefaultPosition, wxSize(150, 35) };
	mainSizer->Add(btnOk, 0, wxALIGN_CENTER | wxALL, 10);

	this->SetSizerAndFit(mainSizer);
	this->Centre();
}

// DIALOG DISCOTECA
DlgDisco::DlgDisco(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Disco", wxDefaultPosition, wxDefaultSize },
	m_game{ game }, m_selectedIndex{ 0 }
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour());

	// Sizer Principale Verticale
	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };
	wxBoxSizer* sizerBody = new wxBoxSizer{ wxHORIZONTAL };
	wxPanel* pnlBottom = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxSize(-1, 70), wxBORDER_SUNKEN };
	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };

	// COLONNA SINISTRA (Radio Buttons)
	wxPanel* pnlLocali = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerLocali = new wxBoxSizer{ wxVERTICAL };

	// Creiamo i radio button dinamicamente
	std::vector<Disco>& discoteche = m_game.GetDiscoteche();
	for (int i = 0; i < discoteche.size(); i++)
	{
		// wxRB_GROUP solo al primo, ma vogliamo deselezionarli all'inizio
		long style = (i == 0) ? wxRB_GROUP : 0;
		wxRadioButton* rad = new wxRadioButton(pnlLocali, wxID_ANY, discoteche[i].m_nome, wxDefaultPosition, wxDefaultSize, style);
		// Viene selezionato il primo di default
		if (i == 0) rad->SetValue(true);
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
	wxPanel* pnlDesc = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(550, -1), wxBORDER_SUNKEN);
	wxBoxSizer* sizerDesc = new wxBoxSizer{ wxVERTICAL };

	const Disco& d = m_game.GetDiscoteche()[m_selectedIndex];
	m_lblDescrizione = new wxStaticText{ pnlDesc, wxID_ANY, d.m_descrizione };
	m_lblDescrizione->Wrap(500);

	m_lblPrezzo = new wxStaticText{ pnlDesc, wxID_ANY, "Ingresso: " + m_game.GetSoldiStr(d.m_prezzoIngresso) };
	wxFont fBold = m_lblPrezzo->GetFont();
	fBold.SetWeight(wxFONTWEIGHT_BOLD);
	m_lblPrezzo->SetFont(fBold);

	sizerDesc->Add(m_lblDescrizione, 0, wxEXPAND | wxALL, 5);
	sizerDesc->AddStretchSpacer();
	sizerDesc->Add(new wxStaticLine(pnlDesc), 0, wxEXPAND | wxALL, 5);
	sizerDesc->Add(m_lblPrezzo, 0, wxALL, 5);

	pnlDesc->SetSizer(sizerDesc);
	sizerBody->Add(pnlDesc, 0, wxEXPAND | wxALL, 5);
	mainSizer->Add(sizerBody);

	// --- PARTE INFERIORE (Icona, Soldi, Bottoni) ---
	// Icona
	wxBitmap bmpIcon = CaricaAsset("ZDISCO.png");
	wxStaticBitmap* imgIcon = new wxStaticBitmap(pnlBottom, wxID_ANY, bmpIcon);
	sizerBottom->Add(imgIcon, 0, wxALIGN_CENTER | wxALL, 10);

	// Soldi
	wxString soldiStr = "Soldi: " + m_game.GetSoldiStr(m_game.GetTabbyGuy().GetSoldi());
	m_lblSoldiAttuali = new wxStaticText{ pnlBottom, wxID_ANY, soldiStr };
	sizerBottom->Add(m_lblSoldiAttuali, 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 15);

	sizerBottom->AddStretchSpacer();

	// Bottoni
	wxButton* btnOk = new wxButton{ pnlBottom, wxID_OK, "OK", wxDefaultPosition, wxSize(-1, -1) };
	wxButton* btnCancel = new wxButton{ pnlBottom, wxID_CANCEL, "Cancel", wxDefaultPosition, wxSize(-1, -1) };

	// Bind OK manuale per fare i controlli prima di chiudere
	btnOk->Bind(wxEVT_BUTTON, &DlgDisco::OnOk, this);

	sizerBottom->Add(btnOk, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
	sizerBottom->Add(btnCancel, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);
	pnlBottom->SetSizer(sizerBottom);

	mainSizer->Add(pnlBottom, 0, wxEXPAND | wxALL, 5);

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
	//m_lblDescrizione->GetParent()->Layout();

	this->Fit();
	this->Layout();
}

void DlgDisco::OnOk(wxCommandEvent& event)
{
	m_game.AzionePagaDisco(m_selectedIndex);
	ManifestaEventi(this, m_game);

	// Chiudi la finestra
	EndModal(wxID_OK);
}