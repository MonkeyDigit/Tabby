#include <wx/artprov.h>
#include <wx/sound.h>
#include <wx/app.h> // Serve per wxTheApp
#ifdef __WXMSW__
// Questa macro impedisce a windows.h di includere winsock.h e altra robaccia inutile
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h> 
#endif
#include "DlgBase.h"
#include "TabbyFrame.h"

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

		switch (msg.m_tipo)
		{
		case TipoMsg::METALLARO:
		{
			DlgMetallaro dlg{ parent, game, msg };
			dlg.ShowModal();
			break;
		}
		case TipoMsg::MANOVALE:
		{
			DlgManovale dlg{ parent, game, msg };
			dlg.ShowModal();
			break;
		}
		case TipoMsg::INCIDENTE_MURO:
		case TipoMsg::INCIDENTE_CAMION:
		{
			DlgIncidente dlg{ parent, game, msg };
			dlg.ShowModal();
			break;
		}
		case TipoMsg::DUE_DONNE:
		{
			DlgDueDonne dlg{ parent, game };
			dlg.ShowModal();
			break;
		}
		case TipoMsg::PAGELLA:
		{
			DlgPagella dlg{ parent, game };
			dlg.ShowModal();
			break;
		}
		default:
		{
			DlgEvento dlgEvento{ parent, msg };
			// Nel caso di un pop up evento con scelta (previa implementazione degli appositi bottoni con wxID_YES e wxID_NO), gli id vengono restituiti alla finestra padre
			// Qua valutiamo l'espressione logica
			bool scelta = (dlgEvento.ShowModal() == wxID_YES);

			// Se qui ApplicaScelta aggiunge un NUOVO evento, questo finisce in fondo alla coda. Il ciclo while continuerà a girare e lo pescherà subito dopo
			if (msg.m_tipo == TipoMsg::SCELTA)
				game.ApplicaScelta(msg.m_scelta, scelta);
		}
		}
	}
	// Usciti dal while, la coda è sicuramente vuota
}

void AggiornaFinestraMain()
{
	// Ottieni il puntatore alla finestra principale
	TabbyFrame* mainFrame = dynamic_cast<TabbyFrame*>(wxTheApp->GetTopWindow());

	if (mainFrame)
		mainFrame->AggiornaInterfaccia();	// Forza l'aggiornamento
}

wxBitmap CaricaAsset(const std::string& nomeFile) {
	if (nomeFile.empty())
		return wxNullBitmap;

	wxString path = "img/" + wxString(nomeFile);

	// Se il nome nel txt non ha l'estensione, proviamo ad aggiungerla
	if (!path.Lower().EndsWith(".png"))
		path += ".png";

	if (wxFileExists(path))
		return wxBitmap(path, wxBITMAP_TYPE_ANY);

	return wxNullBitmap;
}

wxBitmap GeneraAvatar(const TabbyGuy& guy)
{
	// 1. CARICA LO SFONDO (Layer Base Assoluto)
	wxBitmap avatar = CaricaAsset("sfondo.png");

	// Se manca lo sfondo, usiamo un fallback bianco o il corpo come base
	if (!avatar.IsOk()) {
		avatar = wxBitmap(200, 300); // Dimensioni totali della scena
		wxMemoryDC dc(avatar);
		dc.SetBackground(*wxWHITE_BRUSH); // O wxLIGHT_GREY_BRUSH
		dc.Clear();
	}

	wxMemoryDC dc(avatar);

	// Funzione helper per disegnare un pezzo alle coordinate giuste
	auto DisegnaPezzo = [&](const std::string& nomeFile, int x, int y) {
		if (nomeFile.empty())
			return;
		wxBitmap img = CaricaAsset(nomeFile);
		if (img.IsOk()) {
			dc.DrawBitmap(img, x, y, true); // true = usa trasparenza
		}
		};

	// 2. SOVRAPPOSIZIONE DEI VESTITI (Ordine Z-Order da tabimg.c)
	// Ordine: Pantaloni -> Scarpe -> Giubbotto -> Testa

	// LAYER 1: PANTALONI
	// Coordinate originali tabimg.c: X=28, Y=93
	DisegnaPezzo(guy.GetPantaloni().GetImageStr(), 28, 93);

	// LAYER 2: SCARPE
	// Coordinate originali tabimg.c: X=-6, Y=205
	DisegnaPezzo(guy.GetScarpe().GetImageStr(), -6, 205);

	// LAYER 3: GIUBBOTTO
	// Coordinate originali tabimg.c: X=-20, Y=18
	DisegnaPezzo(guy.GetGiubbotto().GetImageStr(), -20, 18);

	// LAYER 4: TESTA
	// Coordinate originali tabimg.c: X=42, Y=3
	DisegnaPezzo(guy.GetTestaImage(), 42, 3);

	return avatar;
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

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	wxBoxSizer* sizerText = new wxBoxSizer{ wxHORIZONTAL };

	wxStaticBitmap* icon = new wxStaticBitmap{ this, wxID_ANY,
		wxArtProvider::GetBitmap(artId, wxART_MESSAGE_BOX) };
	sizerText->Insert(0, icon, 0, wxALIGN_CENTER_VERTICAL | wxALL, 10);

	wxStaticText* lblTesto = new wxStaticText{ this, wxID_ANY, msgref.m_testo, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT };

	// Manda a capo il testo automaticamente
	lblTesto->Wrap(600);
	sizerText->Add(lblTesto, 1, wxALIGN_CENTER | wxALL, 15);

	mainSizer->Add(sizerText);
	// Linea separatrice
	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

	// BOTTONI
	wxBoxSizer* sizerBtn = new wxBoxSizer{wxHORIZONTAL};

	if (msgref.m_tipo != TipoMsg::SCELTA)
	{
		// Caso semplice: Solo OK
		// wxID_OK chiude automaticamente il dialogo ritornando wxID_OK
		wxButton* btnOk = new wxButton{ this, wxID_OK, "OK" };
		sizerBtn->Add(btnOk, 0, wxALL, 10);
	}
	else
	{
		wxButton* btnSi = new wxButton{ this, wxID_YES, "Sì" };
		wxButton* btnNo = new wxButton{ this, wxID_NO, "No" };

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

DlgMetallaro::DlgMetallaro(wxWindow* parent, TabbyGame& game, const Messaggio& msg)
	: wxDialog{ parent, wxID_ANY, msg.m_titolo }
{
	this->SetBackgroundColour(*wxBLACK);
	this->SetForegroundColour(*wxRED);
	this->SetFont(parent->GetFont());

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxHORIZONTAL };

	wxBitmap bmpMetallaro = CaricaAsset(game.GetMetallaroImageStr());
	wxStaticBitmap* imgMetallaro = new wxStaticBitmap(this, wxID_ANY, bmpMetallaro);
	mainSizer->Add(imgMetallaro, 0, wxALIGN_CENTER | wxALL, 5);

	mainSizer->Add(new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL), 0, wxEXPAND | wxLEFT | wxRIGHT, 5);

	wxBoxSizer* rightCol = new wxBoxSizer{ wxVERTICAL };
	wxStaticText* lblTesto = new wxStaticText{ this, wxID_ANY, msg.m_testo, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT};

	lblTesto->Wrap(200);
	rightCol->Add(lblTesto, 0, wxALIGN_CENTER | wxALL, 15);

	wxButton* btnOk = new wxButton{ this, wxID_OK, "OK", wxDefaultPosition, wxSize(60,50) };
	rightCol->Add(btnOk, 0, wxALIGN_CENTER | wxALL, 5);
	rightCol->AddStretchSpacer();
	wxBitmap bmpPenta = CaricaAsset("ZPENTA.png");
	wxStaticBitmap* imgPenta = new wxStaticBitmap(this, wxID_ANY, bmpPenta);
	rightCol->Add(imgPenta, 0, wxALIGN_RIGHT);

	mainSizer->Add(rightCol, 0, wxEXPAND | wxALL, 5);

	this->SetSizerAndFit(mainSizer);
	this->Layout();
}

DlgManovale::DlgManovale(wxWindow* parent, TabbyGame& game, const Messaggio& msg)
	: wxDialog{ parent, wxID_ANY, msg.m_titolo }
{
	this->SetBackgroundColour(*wxBLACK);
	this->SetForegroundColour(*wxRED);
	this->SetFont(parent->GetFont());

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	wxPanel* pnlImg = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerImg = new wxBoxSizer{ wxVERTICAL };

	wxBitmap bmpManovale = CaricaAsset("manovale.png");
	wxStaticBitmap* imgManovale = new wxStaticBitmap(pnlImg, wxID_ANY, bmpManovale);
	sizerImg->Add(imgManovale, 0, wxALIGN_CENTER | wxALL, 5);
	pnlImg->SetSizer(sizerImg);
	mainSizer->Add(pnlImg, 0, wxEXPAND | wxALL, 5);

	wxStaticText* lblTesto = new wxStaticText{ this, wxID_ANY, msg.m_testo, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT };

	lblTesto->Wrap(300);
	mainSizer->Add(lblTesto, 0, wxALIGN_CENTER | wxALL, 15);

	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };

	wxBitmap bmpCamion = CaricaAsset("ZCAMION.png");
	wxStaticBitmap* imgCamion = new wxStaticBitmap(this, wxID_ANY, bmpCamion);
	sizerBottom->Add(imgCamion, 0, wxALIGN_CENTER);

	sizerBottom->AddStretchSpacer();
	wxButton* btnOk = new wxButton{ this, wxID_OK, "OK", wxDefaultPosition, wxSize(60,50) };
	sizerBottom->Add(btnOk, 0, wxALIGN_CENTER | wxALL, 5);

	mainSizer->Add(sizerBottom, 0, wxEXPAND | wxALL, 5);

	this->SetSizerAndFit(mainSizer);
	this->Layout();
}

DlgIncidente::DlgIncidente(wxWindow* parent, TabbyGame& game, const Messaggio& msg)
	: wxDialog{ parent, wxID_ANY, msg.m_titolo }
{
	this->SetBackgroundColour(*wxBLACK);
	this->SetForegroundColour(*wxRED);
	this->SetFont(parent->GetFont());

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	wxPanel* pnlImg = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerImg = new wxBoxSizer{ wxVERTICAL };

	wxBitmap bmpIncidente = (msg.m_tipo == TipoMsg::INCIDENTE_MURO) ? CaricaAsset("muro.png") : CaricaAsset("camionista.png");
	wxStaticBitmap* imgIncidente = new wxStaticBitmap(pnlImg, wxID_ANY, bmpIncidente);
	sizerImg->Add(imgIncidente, 0, wxALIGN_CENTER | wxALL, 5);
	pnlImg->SetSizer(sizerImg);
	mainSizer->Add(pnlImg, 0, wxEXPAND | wxALL, 5);

	wxStaticText* lblTesto = new wxStaticText{ this, wxID_ANY, msg.m_testo, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT };

	lblTesto->Wrap(300);
	mainSizer->Add(lblTesto, 0, wxALIGN_CENTER | wxALL, 15);

	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };

	wxBitmap bmpScooter = CaricaAsset("ZSCOOTER.png");
	wxStaticBitmap* imgScooter = new wxStaticBitmap(this, wxID_ANY, bmpScooter);
	sizerBottom->Add(imgScooter, 0, wxALIGN_CENTER);

	sizerBottom->AddStretchSpacer();
	wxButton* btnOk = new wxButton{ this, wxID_OK, "OK", wxDefaultPosition, wxSize(60,50) };
	sizerBottom->Add(btnOk, 0, wxALIGN_CENTER | wxALL, 5);

	mainSizer->Add(sizerBottom, 0, wxEXPAND | wxALL, 5);

	this->SetSizerAndFit(mainSizer);
	this->Layout();
}

// --- DIALOG PAGELLA ---
DlgPagella::DlgPagella(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Pagella Scolastica", wxDefaultPosition, wxDefaultSize, wxCAPTION | wxCLOSE_BOX },
	m_game{ game }
{
	this->SetBackgroundColour(wxColor(240, 240, 220)); // Colore carta vecchia
	this->SetFont(parent->GetFont());

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	// 1. INTESTAZIONE
	wxBoxSizer* sizerHeader = new wxBoxSizer{ wxHORIZONTAL };

	wxBitmap bmpIcon = CaricaAsset("ZPAGELLA.png");
	wxStaticBitmap* imgIcon = new wxStaticBitmap(this, wxID_ANY, bmpIcon);
	sizerHeader->Add(imgIcon, 0, wxALIGN_CENTER | wxALL, 5);

	wxStaticText* lblIntestazione = new wxStaticText{ this, wxID_ANY, "SCRUTINIO FINALE" };
	wxFont fTitle = lblIntestazione->GetFont();
	fTitle.SetWeight(wxFONTWEIGHT_BOLD);
	fTitle.SetPointSize(14);
	lblIntestazione->SetFont(fTitle);
	sizerHeader->Add(lblIntestazione, 0, wxALL | wxALIGN_CENTER, 15);

	mainSizer->Add(sizerHeader, 0, wxALIGN_CENTER);

	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

	// 2. LISTA VOTI
	// Usiamo una griglia a 2 colonne: [Materia] ...... [Voto]
	wxFlexGridSizer* gridVoti = new wxFlexGridSizer(2, 5, 10);

	const std::vector<Materia>& materie = m_game.GetTabbyGuy().GetScuola().m_materie;
	int insufficienze = 0;

	for (const auto& m : materie)
	{
		// Nome Materia
		wxStaticText* lblMat = new wxStaticText{ this, wxID_ANY, m.GetNome() };
		// Voto
		wxStaticText* lblVoto = new wxStaticText{ this, wxID_ANY, std::to_string(m.GetVoto()) };
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

	gridVoti->Add(new wxStaticText{ this, wxID_ANY, "Condotta:" }, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
	std::string condottaStr = (m_game.GetTabbyGuy().GetFama() > 75) ? "9" : "8";	// Più è un figo, più sembra un bravo ragazzo...
	wxStaticText* lblCondotta = new wxStaticText{ this, wxID_ANY, condottaStr };
	lblCondotta->SetFont(fTitle);
	gridVoti->Add(lblCondotta, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);

	// Centriamo la griglia
	wxBoxSizer* gridWrapper = new wxBoxSizer{ wxVERTICAL };
	gridWrapper->Add(gridVoti, 1, wxALIGN_CENTER | wxALL, 15);
	mainSizer->Add(gridWrapper, 1, wxEXPAND);

	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxLEFT | wxRIGHT, 10);

	// 3. VERDETTO FINALE
	bool promosso = (insufficienze < 5);

	wxString esitoStr = promosso ? "PROMOSSO" : "BOCCIATO";
	wxColor esitoCol = promosso ? wxColor(0, 150, 0) : *wxRED;

	wxStaticText* lblEsito = new wxStaticText{ this, wxID_ANY, esitoStr };
	wxFont fEsito = lblEsito->GetFont();
	fEsito.SetPointSize(18);
	fEsito.SetWeight(wxFONTWEIGHT_BOLD);
	lblEsito->SetFont(fEsito);
	lblEsito->SetForegroundColour(esitoCol);

	mainSizer->Add(lblEsito, 0, wxALL | wxALIGN_CENTER, 15);

	wxString commento = promosso ? "Ammesso all'anno successivo" : "NON ammesso all'anno successivo";
	mainSizer->Add(new wxStaticText{ this, wxID_ANY, commento }, 0, wxALL | wxALIGN_CENTER, 15);

	// 4. BOTTONE OK
	wxButton* btnOk = new wxButton{ this, wxID_OK, "OK", wxDefaultPosition, wxSize(100, 40) };
	btnOk->Bind(wxEVT_BUTTON, &DlgPagella::OnOk, this);
	mainSizer->Add(btnOk, 0, wxALL | wxALIGN_CENTER, 15);

	this->SetSizerAndFit(mainSizer);
	this->Centre();
}

void DlgPagella::OnOk(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}

DlgDueDonne::DlgDueDonne(wxWindow* parent, TabbyGame& game)
	: wxDialog{ parent, wxID_ANY, "Troppe donne", wxDefaultPosition, wxDefaultSize, wxCAPTION },
	m_game{ game }
{
	this->SetBackgroundColour(parent->GetBackgroundColour());
	this->SetFont(parent->GetFont());

	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	wxBoxSizer* sizerHeader = new wxBoxSizer{ wxHORIZONTAL };


	wxBitmap bmpIcon = CaricaAsset("ZTIPA.png");
	wxStaticBitmap* imgIcon = new wxStaticBitmap(this, wxID_ANY, bmpIcon);
	sizerHeader->Add(imgIcon, 0, wxALL, 5);

	wxStaticText* titolo = new wxStaticText{ this, wxID_ANY, "Ti ritrovi nell'imbarazzante (?) situazione di avere 2 ragazze... \nChe cosa fai ???" };
	titolo->Wrap(400);
	sizerHeader->Add(titolo, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

	mainSizer->Add(sizerHeader, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxTOP | wxBOTTOM, 5);
	mainSizer->Add(new wxStaticText{ this, wxID_ANY, "Viva l'abbondanza !" }, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	wxButton* btnEntrambe = new wxButton{ this, wxID_ANY, "Le voglio tutte e due !", wxDefaultPosition, wxSize(-1, 40) };
	btnEntrambe->Bind(wxEVT_BUTTON, &DlgDueDonne::OnEntrambe, this);
	mainSizer->Add(btnEntrambe, 0, wxEXPAND | wxALL, 5);
	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

	wxStaticText* scelta = new wxStaticText{ this, wxID_ANY, "Purtroppo avere due ragazze è troppo anche per uno stallone come il sottoscritto..." };
	scelta->Wrap(400);
	mainSizer->Add(scelta, 0, wxALIGN_CENTER_HORIZONTAL | wxALL, 5);

	// Se le tipe hanno entrambe lo stesso nome...
	std::string prefer;
	if (m_game.GetTabbyGuy().GetTipa().GetNome() == m_game.GetTipaNuova().GetNome())
		prefer = "Preferisco quella nuova";
	else
		prefer = "Preferisco " + m_game.GetTipaNuova().GetNome();

	wxButton* btnResto = new wxButton{ this, wxID_ANY, "Resto con " + m_game.GetTabbyGuy().GetTipa().GetNome(), wxDefaultPosition, wxSize(-1, 40) };
	btnResto->Bind(wxEVT_BUTTON, &DlgDueDonne::OnResto, this);
	mainSizer->Add(btnResto, 0, wxEXPAND | wxLEFT | wxRIGHT, 5);
	wxButton* btnPreferisco = new wxButton{ this, wxID_ANY, prefer, wxDefaultPosition, wxSize(-1, 40) };
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