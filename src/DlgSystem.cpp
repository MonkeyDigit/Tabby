#include <wx/gbsizer.h>
#include "DlgSystem.h"

// DIALOG CONFIGURAZIONE
DlgConfig::DlgConfig(wxWindow* parent, TabbyGame& game)
	: wxDialog(parent, wxID_ANY, "Configuration", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE & ~wxRESIZE_BORDER),
	m_game(game)
{
	this->SetBackgroundColour(WIN_BKG);
	this->SetFont(parent->GetFont());

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	// 1. CHOOSE SKILL LEVEL
	wxStaticBoxSizer* grpSkill = new wxStaticBoxSizer(wxVERTICAL, this, "Choose Skill level:");

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
	wxBoxSizer* iconSizer = new wxBoxSizer{ wxHORIZONTAL };
	// Placeholder per le icone (non avendo i png originali)
	wxStaticText* iconEU = new wxStaticText{ grpMisc->GetStaticBox(), wxID_ANY, "[EU]" };
	iconEU->SetForegroundColour(*wxBLUE);
	wxStaticText* iconMoto = new wxStaticText{ grpMisc->GetStaticBox(), wxID_ANY, "[MOTO]" };
	iconMoto->SetForegroundColour(*wxRED);

	iconSizer->Add(iconEU, 0, wxRIGHT, 5);
	iconSizer->Add(iconMoto, 0, wxLEFT, 5);

	gbSizer->Add(iconSizer, wxGBPosition(1, 1), wxDefaultSpan, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);

	// Bottone OK Grande con icona check
	wxButton* btnOk = new wxButton{ grpMisc->GetStaticBox(), wxID_OK, "OK", wxDefaultPosition, wxSize(80, 40) };
	// TODO: Usiamo wxART_TICK_MARK se vogliamo l'icona spunta, ma richiede wxBitmapButton. Per ora testo.

	// Il bottone OK nello screen occupa le ultime righe a destra
	gbSizer->Add(btnOk, wxGBPosition(4, 1), wxGBSpan(2, 1), wxALIGN_BOTTOM | wxALIGN_RIGHT | wxALL, 5);

	grpMisc->Add(gbSizer, 1, wxEXPAND | wxALL, 5);
	mainSizer->Add(grpMisc, 0, wxEXPAND | wxALL, 5);

	// FOOTER
	wxBoxSizer* footerSizer = new wxBoxSizer{ wxHORIZONTAL };
	footerSizer->Add(new wxStaticText{ this, wxID_ANY, "Tabboz Simulator" }, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	footerSizer->AddStretchSpacer();

	wxStaticText* lblReset = new wxStaticText{ this, wxID_ANY, "reset ." };
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

// DIALOG ABOUT
DlgAbout::DlgAbout(wxWindow* parent)
	: wxDialog(parent, wxID_ANY, "About Tabboz Simulator", wxDefaultPosition, wxDefaultSize, wxCAPTION | wxCLOSE_BOX)
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour()); // Grigio standard

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	// --- SEZIONE SUPERIORE (Info Generali + Bottone OK) ---

	wxPanel* pnlMainFrame = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN);
	wxBoxSizer* frameSizer = new wxBoxSizer{ wxVERTICAL };

	// 1. HEADER (Icone, Testo, Bottone OK laterale)
	wxBoxSizer* headerSizer = new wxBoxSizer{ wxHORIZONTAL };

	// Colonna Icone (Sinistra)
	wxBoxSizer* iconsSizer = new wxBoxSizer{ wxVERTICAL };
	// Icona Moto
	iconsSizer->Add(new wxStaticText{ pnlMainFrame, wxID_ANY, "[MOTO]" }, 0, wxBOTTOM, 2);
	// Icona UE
	iconsSizer->Add(new wxStaticText{ pnlMainFrame, wxID_ANY, "[EU]" }, 0, wxTOP, 2);

	headerSizer->Add(iconsSizer, 0, wxALL, 10);

	// Colonna Testo (Centro)
	wxBoxSizer* textSizer = new wxBoxSizer{ wxVERTICAL };
	textSizer->Add(new wxStaticText{ pnlMainFrame, wxID_ANY, "Tabboz simulator" }, 0, wxBOTTOM, 2);
	textSizer->Add(new wxStaticText{ pnlMainFrame, wxID_ANY, "Version 0.9beta, Mar 11 2000" }, 0, wxBOTTOM, 2);
	textSizer->Add(new wxStaticText{ pnlMainFrame, wxID_ANY, "Copyright (C) 1997-2000 by" }, 0, wxBOTTOM, 0);
	textSizer->Add(new wxStaticText{ pnlMainFrame, wxID_ANY, "Obscured Truckware" }, 0, wxBOTTOM, 0);

	headerSizer->Add(textSizer, 1, wxTOP | wxBOTTOM | wxLEFT, 10);

	// Colonna Bottone OK (Destra) con icona check
	wxButton* btnOk = new wxButton{ pnlMainFrame, wxID_OK, "OK", wxDefaultPosition, wxSize(80, 40) };
	btnOk->Bind(wxEVT_BUTTON, &DlgAbout::OnOk, this);

	headerSizer->Add(btnOk, 0, wxALL | wxALIGN_TOP, 10);

	frameSizer->Add(headerSizer, 0, wxEXPAND);

	frameSizer->Add(new wxStaticLine(pnlMainFrame), 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

	// 2. IMMAGINE LOGO (32bit Tabboz Simulator)
	wxPanel* pnlLogo = new wxPanel(pnlMainFrame, wxID_ANY, wxDefaultPosition, wxSize(300, 100), wxBORDER_NONE);
	// pnlLogo->SetBackgroundBitmap(...); 
	wxBoxSizer* logoSizer = new wxBoxSizer{ wxVERTICAL };
	logoSizer->Add(new wxStaticText{ pnlLogo, wxID_ANY, "[LOGO TABBOZ 32bit]" }, 1, wxALIGN_CENTER);
	pnlLogo->SetSizer(logoSizer);

	frameSizer->Add(pnlLogo, 0, wxALIGN_CENTER | wxALL, 10);

	// 3. CREDITS
	wxFlexGridSizer* creditGrid = new wxFlexGridSizer(2, 5, 20); // 2 cols, gap
	creditGrid->Add(new wxStaticText{ pnlMainFrame, wxID_ANY, "Created by:" }, 0, wxALIGN_RIGHT);

	wxBoxSizer* authorsSizer = new wxBoxSizer{ wxVERTICAL };
	authorsSizer->Add(new wxStaticText{ pnlMainFrame, wxID_ANY, "Andrea Bonomi" });
	authorsSizer->Add(new wxStaticText{ pnlMainFrame, wxID_ANY, "Emanuele Caccialanza" });
	authorsSizer->Add(new wxStaticText{ pnlMainFrame, wxID_ANY, "Daniele Gazzarri" });

	creditGrid->Add(authorsSizer, 0, wxALIGN_LEFT);

	frameSizer->Add(creditGrid, 0, wxALIGN_CENTER | wxBOTTOM, 10);

	// LINEA SEPARAZIONE
	frameSizer->Add(new wxStaticLine(pnlMainFrame), 0, wxEXPAND | wxLEFT | wxRIGHT, 2);

	// 4. BETA TESTERS
	wxPanel* pnlTesters = new wxPanel(pnlMainFrame, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
	wxBoxSizer* testerSizer = new wxBoxSizer{ wxHORIZONTAL };
	testerSizer->Add(new wxStaticText{ pnlTesters, wxID_ANY, "Beta testers:" }, 0, wxRIGHT, 20);
	testerSizer->Add(new wxStaticText{ pnlTesters, wxID_ANY, "Dino Lucci" }, 0, wxRIGHT, 40);
	testerSizer->Add(new wxStaticText{ pnlTesters, wxID_ANY, "Giulio Lucci" }, 0, wxRIGHT, 5);
	pnlTesters->SetSizer(testerSizer);

	frameSizer->Add(pnlTesters, 0, wxEXPAND | wxALL, 5);

	// LINEA SEPARAZIONE
	frameSizer->Add(new wxStaticLine(pnlMainFrame), 0, wxEXPAND | wxLEFT | wxRIGHT, 2);

	// 5. DISCLAIMER (Icona rossa + Testo)
	wxBoxSizer* disclaimerSizer = new wxBoxSizer{ wxHORIZONTAL };

	// Icona rossa
	wxStaticText* iconOmino = new wxStaticText{ pnlMainFrame, wxID_ANY, "[!!]", wxDefaultPosition, wxSize(32, 32), wxALIGN_CENTER | wxBORDER_SIMPLE };
	iconOmino->SetBackgroundColour(*wxRED);
	iconOmino->SetForegroundColour(*wxWHITE);

	disclaimerSizer->Add(iconOmino, 0, wxALL | wxALIGN_CENTER_VERTICAL, 10);

	wxStaticText* txtDisclaimer = new wxStaticText{ pnlMainFrame, wxID_ANY,
		"Questo programma contiene un linguaggio\ntalvolta offensivo; ogni riferimento a persone e\ncose e' puramente casuale." };
	txtDisclaimer->SetWindowStyle(wxALIGN_CENTER);

	disclaimerSizer->Add(txtDisclaimer, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);

	frameSizer->Add(disclaimerSizer, 0, wxEXPAND | wxBOTTOM, 5);

	// CHIUSURA PANEL PRINCIPALE
	pnlMainFrame->SetSizer(frameSizer);
	mainSizer->Add(pnlMainFrame, 1, wxEXPAND | wxALL, 5);

	// --- SEZIONE INFERIORE (URL + Bottone Norme) ---
	wxBoxSizer* footerSizer = new wxBoxSizer{ wxHORIZONTAL };

	wxBoxSizer* urlSizer = new wxBoxSizer{ wxVERTICAL };
	urlSizer->Add(new wxStaticText{ this, wxID_ANY, "http://www.tabboz.com" }, 0);
	urlSizer->Add(new wxStaticText{ this, wxID_ANY, "e-mail: andrea@tabboz.com" }, 0);

	footerSizer->Add(urlSizer, 1, wxALIGN_CENTER_VERTICAL | wxLEFT, 10);

	wxButton* btnNorme = new wxButton{ this, wxID_ANY, "Norme di utilizzo", wxDefaultPosition, wxSize(120, -1) };
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

// DIALOG DOCUMENTO D'IDENTITA'
DlgPersonalInfo::DlgPersonalInfo(wxWindow* parent, TabbyGame& game)
	: wxDialog(parent, wxID_ANY, "Documento d'identità", wxDefaultPosition, wxSize(650, -1)),
	m_game(game)
{
	this->SetFont(parent->GetFont());
	// Sfondo giallino carta vecchia
	this->SetBackgroundColour(wxColor(245, 235, 210));

	const CartaIdentita& id = m_game.GetTabbyGuy().GetID();

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	// --- INTESTAZIONE ---
	wxStaticText* lblHeader = new wxStaticText{ this, wxID_ANY, "REPVBBLICA ITALIANA" };
	wxFont fHeader = lblHeader->GetFont();
	fHeader.SetWeight(wxFONTWEIGHT_BOLD);
	fHeader.SetPointSize(14);
	lblHeader->SetFont(fHeader);
	mainSizer->Add(lblHeader, 0, wxALIGN_CENTER | wxTOP, 15);

	wxBitmap bmpIcon = CaricaAsset("ZCOMUNE.png");
	wxStaticBitmap* imgIcon = new wxStaticBitmap(this, wxID_ANY, bmpIcon);
	mainSizer->Add(imgIcon, 0, wxALIGN_CENTER | wxALL, 5);

	wxStaticText* lblSubHeader = new wxStaticText{ this, wxID_ANY, "CARTA D'IDENTITA'" };
	mainSizer->Add(lblSubHeader, 0, wxALIGN_CENTER | wxBOTTOM, 10);

	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 20);

	// --- CONTENUTO (Due Colonne) ---
	wxBoxSizer* contentSizer = new wxBoxSizer{ wxHORIZONTAL };

	// 1. COLONNA SINISTRA (Dati)
	wxGridBagSizer* grid = new wxGridBagSizer(5, 5);
	int r = 0;

	// ANAGRAFICA
	AddEditRow(grid, r, "Cognome:", m_txtCognome, id.m_cognome);
	AddEditRow(grid, r, "Nome:", m_txtNome, id.m_nome);

	// NASCITA
	wxString dataNascita = wxString::Format("%02d/%02d/%d",
		id.m_dataNascita.GetDay(), (int)id.m_dataNascita.GetMonth(), id.m_dataNascita.GetYear());
	AddStaticRow(grid, r, "Nato il:", dataNascita);
	AddStaticRow(grid, r, "Atto N.:", id.m_attoNascita);

	// CITTA' (Riga speciale con 2 campi)
	AddCityProvRow(grid, r, "a:", m_txtLuogoNascita, id.m_luogoNascita, m_txtProvincia, id.m_provinciaNascita);

	AddStaticRow(grid, r, "Cittadinanza:", id.m_cittadinanza);

	// RESIDENZA
	AddEditRow(grid, r, "Residenza:", m_txtResidenza, id.m_residenza);
	AddEditRow(grid, r, "Via:", m_txtIndirizzo, id.m_indirizzo);

	// STATO CIVILE E PROFESSIONE
	AddStaticRow(grid, r, "Stato Civile:", id.m_statoCivile);
	AddStaticRow(grid, r, "Professione:", id.m_professione);

	// Espandiamo la colonna dei valori per riempire lo spazio
	grid->AddGrowableCol(1);

	// Aggiungi la griglia a sinistra
	contentSizer->Add(grid, 1, wxEXPAND | wxALL, 15);

	// 2. COLONNA DESTRA (Foto e Firma)
	wxBoxSizer* rightSizer = new wxBoxSizer{ wxVERTICAL };

	// Genera l'avatar attuale
	wxBitmap avatarBmp = GeneraAvatar(m_game.GetTabbyGuy());

	// Creiamo il controllo immagine
	wxStaticBitmap* imgFoto = new wxStaticBitmap{ this, wxID_ANY, avatarBmp };
	imgFoto->SetWindowStyle(wxBORDER_SIMPLE);

	rightSizer->Add(new wxStaticText{ this, wxID_ANY, "FOTOGRAFIA" }, 0, wxALIGN_CENTER | wxBOTTOM, 5);
	rightSizer->Add(imgFoto, 0, wxALIGN_CENTER | wxBOTTOM, 20);

	// Aggiungi colonna destra
	contentSizer->Add(rightSizer, 0, wxALL, 15);

	// Uniamo al layout principale
	mainSizer->Add(contentSizer, 1, wxEXPAND);

	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 20);

	// --- BOTTONI ---
	wxBoxSizer* btnSizer = new wxBoxSizer{ wxHORIZONTAL };
	wxButton* btnOk = new wxButton{ this, wxID_OK, "Salva modifiche" };
	wxButton* btnCancel = new wxButton{ this, wxID_CANCEL, "Annulla" };

	btnSizer->Add(btnOk, 0, wxRIGHT, 10);
	btnSizer->Add(btnCancel, 0, wxLEFT, 10);

	mainSizer->Add(btnSizer, 0, wxALIGN_CENTER | wxALL, 15);

	// Bind
	btnOk->Bind(wxEVT_BUTTON, &DlgPersonalInfo::OnOk, this);

	this->SetSizerAndFit(mainSizer);
	this->Centre();
}

void DlgPersonalInfo::AddEditRow(wxGridBagSizer* sizer, int& row, wxString label, wxTextCtrl*& ctrlPtr, wxString value) {
	wxFont fontLabel = this->GetFont();
	fontLabel.SetWeight(wxFONTWEIGHT_BOLD);

	wxStaticText* txtLabel = new wxStaticText{ this, wxID_ANY, label };
	txtLabel->SetFont(fontLabel);

	// Non mettiamo dimensione fissa, lasciamo fare al sizer
	ctrlPtr = new wxTextCtrl(this, wxID_ANY, value);

	sizer->Add(txtLabel, wxGBPosition(row, 0), wxDefaultSpan, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	sizer->Add(ctrlPtr, wxGBPosition(row, 1), wxDefaultSpan, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	row++;
}

void DlgPersonalInfo::AddCityProvRow(wxGridBagSizer* sizer, int& row, wxString label, wxTextCtrl*& ctrlCitta, wxString valCitta, wxTextCtrl*& ctrlProv, wxString valProv)
{
	wxFont fontLabel = this->GetFont();
	fontLabel.SetWeight(wxFONTWEIGHT_BOLD);

	wxStaticText* txtLabel = new wxStaticText{ this, wxID_ANY, label };
	txtLabel->SetFont(fontLabel);

	// Creiamo un contenitore orizzontale per: [Citta] ( [Prov] )
	wxBoxSizer* hSizer = new wxBoxSizer{ wxHORIZONTAL };

	ctrlCitta = new wxTextCtrl(this, wxID_ANY, valCitta);
	hSizer->Add(ctrlCitta, 1, wxEXPAND | wxRIGHT, 5); // La città prende lo spazio (1)

	hSizer->Add(new wxStaticText{ this, wxID_ANY, "(" }, 0, wxALIGN_CENTER_VERTICAL);

	ctrlProv = new wxTextCtrl(this, wxID_ANY, valProv, wxDefaultPosition, wxSize(40, -1)); // La provincia è piccola
	hSizer->Add(ctrlProv, 0, wxALIGN_CENTER_VERTICAL | wxLEFT | wxRIGHT, 2);

	hSizer->Add(new wxStaticText{ this, wxID_ANY, ")" }, 0, wxALIGN_CENTER_VERTICAL);

	sizer->Add(txtLabel, wxGBPosition(row, 0), wxDefaultSpan, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	sizer->Add(hSizer, wxGBPosition(row, 1), wxDefaultSpan, wxEXPAND | wxALIGN_CENTER_VERTICAL);
	row++;
}

void DlgPersonalInfo::AddStaticRow(wxGridBagSizer* sizer, int& row, wxString label, wxString value) {
	wxFont fontLabel = this->GetFont();
	fontLabel.SetWeight(wxFONTWEIGHT_BOLD);
	wxFont fontValue = this->GetFont();
	fontValue.SetStyle(wxFONTSTYLE_ITALIC);

	wxStaticText* txtLabel = new wxStaticText{ this, wxID_ANY, label };
	txtLabel->SetFont(fontLabel);

	wxStaticText* txtValue = new wxStaticText{ this, wxID_ANY, value };
	txtValue->SetFont(fontValue);
	txtValue->SetForegroundColour(wxColor(0, 0, 150));

	sizer->Add(txtLabel, wxGBPosition(row, 0), wxDefaultSpan, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	sizer->Add(txtValue, wxGBPosition(row, 1), wxDefaultSpan, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	row++;
}

void DlgPersonalInfo::AddSeparator(wxGridBagSizer* sizer, int& row) {
	wxStaticLine* line = new wxStaticLine(this);
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

// DIALOG USCITA
DlgUscita::DlgUscita(wxWindow* parent)
	: wxDialog(parent, wxID_ANY, "Fine della sessione del Tabboz Simulator", wxDefaultPosition, wxDefaultSize, wxCAPTION | wxCLOSE_BOX),
	m_sceltaUscita(false)
{
	this->SetFont(parent->GetFont());
	this->SetBackgroundColour(parent->GetBackgroundColour());

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };
	wxBoxSizer* bodySizer = new wxBoxSizer{ wxHORIZONTAL };

	// 1. ICONA A SINISTRA
	// Usiamo un pannello placeholder o uno static text per ora
	wxStaticText* iconBox = new wxStaticText{ this, wxID_ANY, "[ICON]", wxDefaultPosition, wxSize(48, 48), wxALIGN_CENTER | wxBORDER_NONE };
	// In futuro qui caricherai l'icona del monitor
	bodySizer->Add(iconBox, 0, wxALL | wxALIGN_TOP, 15);

	// 2. PARTE DESTRA (Testo + Radio Buttons)
	wxBoxSizer* rightSizer = new wxBoxSizer{ wxVERTICAL };

	wxStaticText* lblIntro = new wxStaticText{ this, wxID_ANY, "Scegli una delle seguenti opzioni:" };
	rightSizer->Add(lblIntro, 0, wxBOTTOM, 10);

	m_radioChiudi = new wxRadioButton(this, wxID_ANY, "Chiudi il Tabboz Simulator", wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
	m_radioChiudi->SetValue(true); // Selezionato di default

	m_radioSpegni = new wxRadioButton(this, wxID_ANY, "Spegni il computer ed esci di casa");

	rightSizer->Add(m_radioChiudi, 0, wxBOTTOM, 5);
	rightSizer->Add(m_radioSpegni, 0, wxBOTTOM, 5);

	bodySizer->Add(rightSizer, 1, wxEXPAND | wxALL, 10);
	mainSizer->Add(bodySizer, 1, wxEXPAND | wxALL, 5);

	// 3. BOTTONI IN BASSO
	wxBoxSizer* btnSizer = new wxBoxSizer{ wxHORIZONTAL };

	// Nello screenshot i bottoni sono centrati/espansi. Usiamo dimensioni fisse simili a Windows standard.
	wxButton* btnOk = new wxButton{ this, wxID_OK, "OK", wxDefaultPosition, wxSize(75, 25) };
	wxButton* btnCancel = new wxButton{ this, wxID_CANCEL, "Cancel", wxDefaultPosition, wxSize(75, 25) };
	wxButton* btnHelp = new wxButton{ this, wxID_HELP, "Help", wxDefaultPosition, wxSize(75, 25) };

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