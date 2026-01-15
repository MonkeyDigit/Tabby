#include <wx/gbsizer.h>
#include <wx/utils.h> // Serve per wxShutdown
#include "TabbyFrame.h"
#include "DlgBase.h"
#include "DlgAttivita.h"
#include "DlgNegozi.h"
#include "DlgSocial.h"
#include "DlgSystem.h"


// ----------------------------------------------------------------------------
// TABBY FRAME (Solo Grafica)
// ----------------------------------------------------------------------------
// Costruttore finestra
TabbyFrame::TabbyFrame(TabbyGame& game)
	: wxFrame{ NULL, wxID_ANY, "Tabby Sim", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX) }, // Blocca ridimensionamento (sono operatori bitwise su delle flag binarie)
	m_game{ game }
{
	wxIcon icona;
	// Carica il file .ico dalla cartella img
	if (icona.LoadFile("img/tabby.ico", wxBITMAP_TYPE_ICO))
		this->SetIcon(icona);
	// Imposta colore di sfondo e il font
	this->SetBackgroundColour(WIN_BKG);
	this->SetFont(wxFont(12, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

	// BARRA MENU -------------------------------------------------------
	wxMenuBar* menuBar = new wxMenuBar{};
	wxMenu* menuAttivita = new wxMenu{};
	wxMenu* menuNegozi = new wxMenu{};
	wxMenu* menuSpecial = new wxMenu{};
	wxMenu* menuHelp = new wxMenu{};
	// VOCI DEI MENU A TENDINA
	// Attività
	menuAttivita->Append(ID_SCOOTER, "Scooter");
	menuAttivita->Append(ID_DISCO, "Disco");
	menuAttivita->AppendSeparator();
	menuAttivita->Append(ID_SCUOLA, "Scuola");
	menuAttivita->Append(ID_LAVORO, "Lavoro");
	menuAttivita->AppendSeparator();
	menuAttivita->Append(ID_COMPAGNIA, "Compagnia");
	menuAttivita->Append(ID_FAMIGLIA, "Famiglia");
	menuAttivita->Append(ID_TIPA, "Tipa");
	// Negozi
	std::vector<Negozio>& negozi = m_game.GetNegozi();
	for (int i = 0; i < negozi.size(); i++)
	{
		// Aggiungiamo la voce al menu. 
		// Usiamo wxID_ANY: wxWidgets assegnerà un ID numerico univoco automatico.
		// Salviamo il puntatore 'item' per recuperare questo ID subito dopo.
		wxMenuItem* item = menuNegozi->Append(wxID_ANY, negozi[i].m_nome);

		// 2. Bindiamo l'evento "al volo".
		// Usiamo una lambda che cattura 'this' (per usare m_game) e 'i' (l'indice del negozio).
		// Il bind viene fatto sull'ID specifico di questo item (item->GetId()).
		this->Bind(wxEVT_MENU, [this, negozi, i](wxCommandEvent& evt) {

			if (m_game.TriggerNegozio(negozi[i].m_merce))
			{
				DlgNegozio dlg{ this, m_game, negozi[i] };
				dlg.ShowModal();
				this->AggiornaInterfaccia();
			}
			ManifestaEventi(this, m_game);

			}, item->GetId());
	}
	menuNegozi->AppendSeparator();
	menuNegozi->Append(ID_PALESTRA, "Palestra");
	menuNegozi->Append(ID_TELEFONINO, "Telefonino");
	// Special
	menuSpecial->Append(ID_INFO, "Documento d'identità");
	menuSpecial->Append(ID_CONFIG, "Configurazione");
	// Help
	menuHelp->Append(ID_ABOUT, "About Tabby");
	menuHelp->Append(ID_LOGO, "Startup logo");
	// Vengono fissati i menu a tendina sulla barra principale
	menuBar->Append(menuAttivita, "Attività");
	menuBar->Append(menuNegozi, "Negozi");
	menuBar->Append(menuSpecial, "Special");
	menuBar->Append(menuHelp, "Help");
	// Diciamo alla finestra di usare questa barra
	this->SetMenuBar(menuBar);
	// Le funzioni vengono associati alle funzioni
	this->Bind(wxEVT_MENU, &TabbyFrame::OnScooter, this, ID_SCOOTER);
	this->Bind(wxEVT_MENU, &TabbyFrame::OnDisco, this, ID_DISCO);
	this->Bind(wxEVT_MENU, &TabbyFrame::OnScuola, this, ID_SCUOLA);
	this->Bind(wxEVT_MENU, &TabbyFrame::OnLavoro, this, ID_LAVORO);
	this->Bind(wxEVT_MENU, &TabbyFrame::OnCompagnia, this, ID_COMPAGNIA);
	this->Bind(wxEVT_MENU, &TabbyFrame::OnFamiglia, this, ID_FAMIGLIA);
	this->Bind(wxEVT_MENU, &TabbyFrame::OnTipa, this, ID_TIPA);
	this->Bind(wxEVT_MENU, &TabbyFrame::OnPalestra, this, ID_PALESTRA);
	this->Bind(wxEVT_MENU, &TabbyFrame::OnTelefonino, this, ID_TELEFONINO);
	this->Bind(wxEVT_MENU, &TabbyFrame::OnPersonalInfo, this, ID_INFO);
	this->Bind(wxEVT_MENU, &TabbyFrame::OnAbout, this, ID_ABOUT);
	this->Bind(wxEVT_MENU, &TabbyFrame::OnConfig, this, ID_CONFIG);
	this->Bind(wxEVT_CLOSE_WINDOW, &TabbyFrame::OnClose, this);

	// PANNELLI --------------------------------------------------------------------------------------
	// SIZER GLOBALE (permette di mettere roba e di ridimensionare gli elementi)
	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };

	// Bottoni
	wxButton* btnScooter = new wxButton{ this, ID_SCOOTER, "Scooter", wxDefaultPosition, wxSize(140, -1) };
	wxButton* btnNegozi = new wxButton{ this, ID_NEGOZI, "Negozi", wxDefaultPosition, wxSize(140, -1) };
	wxButton* btnDisco = new wxButton{ this, ID_DISCO, "Disco", wxDefaultPosition, wxSize(140, -1) };
	wxButton* btnScuola = new wxButton{ this, ID_SCUOLA, "Scuola", wxDefaultPosition, wxSize(140, -1) };
	wxButton* btnLavoro = new wxButton{ this, ID_LAVORO, "Lavoro", wxDefaultPosition, wxSize(140, -1) };

	// Associazione funzioni ai bottoni
	this->Bind(wxEVT_BUTTON, &TabbyFrame::OnScooter, this, ID_SCOOTER);
	this->Bind(wxEVT_BUTTON, &TabbyFrame::OnNegozi, this, ID_NEGOZI);
	this->Bind(wxEVT_BUTTON, &TabbyFrame::OnDisco, this, ID_DISCO);
	this->Bind(wxEVT_BUTTON, &TabbyFrame::OnScuola, this, ID_SCUOLA);
	this->Bind(wxEVT_BUTTON, &TabbyFrame::OnLavoro, this, ID_LAVORO);
	this->Bind(wxEVT_BUTTON, &TabbyFrame::OnTipa, this, ID_TIPA);
	this->Bind(wxEVT_BUTTON, &TabbyFrame::OnCompagnia, this, ID_COMPAGNIA);
	this->Bind(wxEVT_BUTTON, &TabbyFrame::OnFamiglia, this, ID_FAMIGLIA);
	this->Bind(wxEVT_BUTTON, &TabbyFrame::OnAbout, this, ID_ABOUT);

	// HEADER
	wxGridBagSizer* gridHeader = new wxGridBagSizer{ 5, 5 };	// gap verticale, gap orizzontale
	// HEADER --------------------------------------------------------------------------------------------------
	// RIGA 0
	// Add(Oggetto, Posizione(Riga, Colonna), Estensione(Righe, Colonne), Flag, Bordo)
	gridHeader->Add(btnScooter, wxGBPosition(0, 0), wxDefaultSpan, wxALL, 5);	// wxALL impone di lasciare un padding in ogni direzione, di 5px
	gridHeader->Add(btnNegozi, wxGBPosition(0, 1), wxDefaultSpan, wxALL, 5);
	gridHeader->Add(btnDisco, wxGBPosition(0, 2), wxDefaultSpan, wxALL & ~wxRIGHT, 5);

	// Linea Verticale (Parte da riga 0, alta 2 righe)
	gridHeader->Add(new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL), wxGBPosition(0, 3), wxGBSpan(2, 1), wxEXPAND);	// wxEXPAND fa un override della dimensione ed estende l'elemento per tutta la lunghezza disponibile
	gridHeader->Add(btnScuola, wxGBPosition(0, 4), wxDefaultSpan, wxALL & ~wxLEFT, 5);

	// RIGA 1
	wxPanel* pnlNome = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxSize(-1, 40), wxBORDER_SUNKEN };
	wxBoxSizer* sizerNome = new wxBoxSizer{ wxHORIZONTAL };
	sizerNome->Add(new wxStaticText(pnlNome, wxID_ANY, " Nome"), 0, wxALIGN_CENTER_VERTICAL);
	sizerNome->AddStretchSpacer();	// Aggiungi spazio che si estende fino a destra
	m_lblNomeTabby = new wxStaticText{ pnlNome, wxID_ANY, "---" };
	sizerNome->Add(m_lblNomeTabby, 0, wxALIGN_CENTER_VERTICAL);
	pnlNome->SetSizer(sizerNome);
	gridHeader->Add(pnlNome, wxGBPosition(1, 0), wxGBSpan(1, 3), wxEXPAND | wxALL & ~(wxRIGHT | wxTOP), 5);

	// Aggiungi bottone lavoro
	gridHeader->Add(btnLavoro, wxGBPosition(1, 4), wxDefaultSpan, wxALL & ~wxLEFT, 5);

	// QUESTO LO AGGIUNGO SOLO PER ALLINEARE I BOTTONI CON QUELLI DI SOTTO
	gridHeader->Add(6, 1, wxGBPosition(0, 5), wxGBSpan(2, 1));
	gridHeader->AddGrowableCol(2);	// Questo fa sì che la colonna si estenda il più possibile

	// Aggiungi Header al Main
	mainSizer->Add(gridHeader, 0, wxEXPAND);
	// LINEA SEPARATRICE ORIZZONTALE
	mainSizer->Add(new wxStaticLine(this), 0, wxEXPAND);

	// BODY ---------------------------------------------------------------------------------------------
	wxBoxSizer* sizerBody = new wxBoxSizer{ wxHORIZONTAL };

	// FOTO
	wxPanel* pnlFoto = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerFoto = new wxBoxSizer{ wxVERTICAL };
	wxBitmap bmpAvatar = GeneraAvatar(m_game.GetTabbyGuy());
	m_fotoTabbozzo = new wxStaticBitmap{ pnlFoto, wxID_ANY, bmpAvatar };
	m_fotoTabbozzo->SetCursor(wxCursor(wxCURSOR_HAND)); // Manina quando passi sopra
	m_fotoTabbozzo->Bind(wxEVT_LEFT_DOWN, [this](wxMouseEvent& event) {
		DlgPersonalInfo dlg(this, m_game);
		dlg.ShowModal();
		this->AggiornaInterfaccia();
		});

	sizerFoto->Add(m_fotoTabbozzo, 0, wxALL, 5);
	pnlFoto->SetSizer(sizerFoto);
	sizerBody->Add(pnlFoto, 0, wxALL, 5);

	// GRIGLIA STATISTICHE
	wxPanel* pnlStats = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxBORDER_SUNKEN };
	wxGridBagSizer* gridStats = new wxGridBagSizer{ 0,0 };
	wxBoxSizer* sizerStats = new wxBoxSizer{ wxVERTICAL };

	// Bottoni laterali
	wxButton* btnTipa = new wxButton{ pnlStats, ID_TIPA, "Tipa", wxDefaultPosition, wxSize(140, -1) };
	wxButton* btnCompagnia = new wxButton{ pnlStats, ID_COMPAGNIA, "Compagnia", wxDefaultPosition, wxSize(140, -1) };
	wxButton* btnFamiglia = new wxButton{ pnlStats, ID_FAMIGLIA, "Famiglia", wxDefaultPosition, wxSize(140, -1) };

	// Tipa
	m_lblNomeTipa = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	gridStats->Add(m_lblNomeTipa, wxGBPosition(0, 0), wxDefaultSpan, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 5);
	gridStats->Add(new wxStaticLine(pnlStats), wxGBPosition(1, 0), wxGBSpan(1, 1), wxEXPAND);
	m_lblRapportoTipa = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	gridStats->Add(m_lblRapportoTipa, wxGBPosition(2, 0), wxDefaultSpan, wxEXPAND | wxALL, 10);
	m_barTipa = new wxGauge{ pnlStats, wxID_ANY, 100, wxDefaultPosition, wxSize(350, 10) };
	gridStats->Add(m_barTipa, wxGBPosition(3, 0), wxDefaultSpan, wxALL & ~wxTOP, 10);
	gridStats->Add(new wxStaticLine(pnlStats), wxGBPosition(4, 0), wxGBSpan(1, 3), wxEXPAND);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL), wxGBPosition(0, 1), wxGBSpan(4, 1), wxEXPAND & ~wxLEFT, 5);

	// Bottoni
	gridStats->Add(btnTipa, wxGBPosition(0, 2), wxDefaultSpan, wxALL | wxALIGN_RIGHT, 7);
	gridStats->Add(btnCompagnia, wxGBPosition(2, 2), wxDefaultSpan, wxALL | wxALIGN_RIGHT, 7);
	gridStats->Add(btnFamiglia, wxGBPosition(3, 2), wxDefaultSpan, wxALL | wxALIGN_RIGHT, 7);

	// Soldi
	wxBoxSizer* sizerSoldiRow = new wxBoxSizer{ wxHORIZONTAL };
	m_lblSoldi = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	// Soldi Delta - Singoli guadagni e perdite
	m_lblSoldiDelta = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	m_lblSoldiDelta->SetFont(wxFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	sizerSoldiRow->Add(m_lblSoldi, 0, wxALIGN_CENTER_VERTICAL);
	sizerSoldiRow->AddSpacer(10); // Un po' di spazio tra il saldo e la variazione
	sizerSoldiRow->Add(m_lblSoldiDelta, 0, wxALIGN_CENTER_VERTICAL);
	gridStats->Add(sizerSoldiRow, wxGBPosition(5, 0), wxGBSpan(1, 3), wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
	gridStats->Add(new wxStaticLine(pnlStats), wxGBPosition(6, 0), wxGBSpan(1, 3), wxEXPAND);

	// Reputazione
	m_lblReputazione = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	gridStats->Add(m_lblReputazione, wxGBPosition(7, 0), wxGBSpan(1, 3), wxEXPAND | wxALL, 10);
	m_barRep = new wxGauge{ pnlStats, wxID_ANY, 100, wxDefaultPosition, wxSize(350, 10) };
	gridStats->Add(m_barRep, wxGBPosition(8, 0), wxGBSpan(1, 3), wxALL & ~wxTOP, 10);
	gridStats->Add(new wxStaticLine(pnlStats), wxGBPosition(9, 0), wxGBSpan(1, 3), wxEXPAND);

	// Figosità
	m_lblFama = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	gridStats->Add(m_lblFama, wxGBPosition(10, 0), wxGBSpan(1, 3), wxEXPAND | wxALL, 10);
	m_barFama = new wxGauge{ pnlStats, wxID_ANY, 100, wxDefaultPosition, wxSize(350, 10) };
	gridStats->Add(m_barFama, wxGBPosition(11, 0), wxGBSpan(1, 3), wxALL & ~wxTOP, 10);
	gridStats->Add(new wxStaticLine(pnlStats), wxGBPosition(12, 0), wxGBSpan(1, 3), wxEXPAND);

	// Profitto scolastico
	m_lblStudio = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	gridStats->Add(m_lblStudio, wxGBPosition(13, 0), wxGBSpan(1, 3), wxEXPAND | wxALL, 10);
	m_barStudio = new wxGauge{ pnlStats, wxID_ANY, 100, wxDefaultPosition, wxSize(350, 10) };
	gridStats->Add(m_barStudio, wxGBPosition(14, 0), wxGBSpan(1, 3), wxALL & ~wxTOP, 10);
	gridStats->Add(new wxStaticLine(pnlStats), wxGBPosition(15, 0), wxGBSpan(1, 3), wxEXPAND);

	// Stato scooter
	m_lblScooter = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	gridStats->Add(m_lblScooter, wxGBPosition(16, 0), wxGBSpan(1, 3), wxALL, 10);
	m_barScooter = new wxGauge{ pnlStats, wxID_ANY, 100, wxDefaultPosition, wxSize(350, 10) };
	gridStats->Add(m_barScooter, wxGBPosition(17, 0), wxGBSpan(1, 3), wxALL & ~wxTOP, 10);
	gridStats->Add(new wxStaticLine(pnlStats), wxGBPosition(18, 0), wxGBSpan(1, 3), wxEXPAND);

	// Data
	wxPanel* pnlDate = new wxPanel{ pnlStats, wxID_ANY, wxDefaultPosition, wxSize(-1,40), wxBORDER_SUNKEN };
	wxBoxSizer* sizerDate = new wxBoxSizer{ wxHORIZONTAL };
	m_lblDate = new wxStaticText{ pnlDate, wxID_ANY, "---" };
	pnlDate->SetSizer(sizerDate);
	sizerDate->Add(m_lblDate, wxEXPAND | wxALL & ~wxBOTTOM, 5);
	sizerDate->AddStretchSpacer();
	wxButton* btnAbout = new wxButton{ pnlDate, ID_ABOUT, "About", wxDefaultPosition, wxSize(140, -1) };
	sizerDate->Add(btnAbout, 0, wxALL, 2);
	gridStats->Add(pnlDate, wxGBPosition(19, 0), wxGBSpan(1, 3), wxEXPAND | wxALL, 5);

	// PILASTRO INVISIBILE
	// Lo mettiamo alla riga 20 (libera), colonna 0.
	// Add(width, height, ...)
	gridStats->Add(350, 0, wxGBPosition(20, 0));

	// FISSIAMO LA GRID AL SIZER
	gridStats->AddGrowableCol(0);
	sizerStats->Add(gridStats, 1, wxEXPAND);
	pnlStats->SetSizer(sizerStats);
	sizerBody->Add(pnlStats, 1, wxEXPAND | wxALL, 5);

	// IMPLEMENTAZIONE BODY
	mainSizer->Add(sizerBody, 1, wxEXPAND);
	// IMPLEMENTAZIONE FINALE
	this->SetSizerAndFit(mainSizer); // Chiede al Sizer principale (mainSizer) quanto spazio gli serve al minimo per far stare dentro tutti i widget (bottoni, pannelli, immagini) senza schiacciarli. Poi ridimensiona la finestra (TabbyFrame) esattamente a quella misura
	this->Centre();	// Posiziona la finestra al centro dello schermo. Senza, la finestra si aprirebbe nell'angolo in alto a sinistra (coordinate 0,0) o dove decide Windows a caso
	this->AggiornaInterfaccia();	// AGGIORNA INTERFACCIA

	// --- SETUP TIMER ---
	// Collega l'evento
	this->Bind(wxEVT_TIMER, &TabbyFrame::OnTimer, this, m_timer.GetId());

	// Inizializza il contatore casuale (tra 12 e 32 minuti)
	m_minutiAlRisveglio = m_game.GenRandomInt(12, 32);

	// Avvia il timer
	if (m_game.GetTimerActive())
	{
		m_timer.Start(60000); // 60000 ms = 1 minuto
	}
}

void TabbyFrame::OnTimer(wxTimerEvent& event)
{
	// Corrisponde a: if (IsIconic(hWndMain) != 0)
	if (this->IsIconized())
	{
		// Se il contatore è ancora alto, aspettiamo un altro minuto
		if (m_minutiAlRisveglio > 0)
		{
			m_minutiAlRisveglio--;
		}
		else
		{
			// IL MOMENTO E' GIUNTO!
			m_minutiAlRisveglio = m_game.GenRandomInt(12, 32);

			// 2. Riapri la finestra (ShowWindow SW_SHOWNORMAL)
			this->Iconize(false); // False = Ripristina (non iconizzare)
			this->RequestUserAttention(); // Fa lampeggiare la barra se necessario
			this->Show(true);
			this->Raise(); // Porta in primo piano
		}
	}
}

void TabbyFrame::AggiornaInterfaccia()
{
	// Viene rinfrescato il tabbozzo...
	wxBitmap av = GeneraAvatar(m_game.GetTabbyGuy());
	if (av.IsOk()) av = wxBitmap(av.ConvertToImage().Rescale(av.GetWidth() * 1.6, av.GetHeight() * 1.6, wxIMAGE_QUALITY_BILINEAR));

	m_fotoTabbozzo->SetBitmap(av);
	m_fotoTabbozzo->Refresh();

	TabbyGuy& guy = m_game.GetTabbyGuy();

	m_lblNomeTabby->SetLabel(guy.GetID().m_nome + " " + guy.GetID().m_cognome + " ");
	if (m_game.GetTabbyGuy().HaTipa())
	{
		m_lblNomeTipa->SetLabel(guy.GetTipa().GetNome() + " ");
		m_lblRapportoTipa->SetLabel(wxString::Format("Rapporto con la tipa %d/100", guy.GetRapporti()));
		m_barTipa->Show(true);
		m_barTipa->SetValue(guy.GetRapporti());
	}
	else
	{
		m_lblNomeTipa->SetLabel("");
		m_lblRapportoTipa->SetLabel("");
		m_barTipa->Hide();
	}

	m_lblSoldi->SetLabel("Soldi " + m_game.GetSoldiStr(guy.GetSoldi()));
	// Delta soldi
	long long delta = m_game.GetTabbyGuy().GetSoldiDelta();
	if (delta > 0)
	{
		m_lblSoldiDelta->SetLabel("+" + m_game.GetSoldiStr(delta));
		m_lblSoldiDelta->SetForegroundColour(wxColor(0, 150, 0)); // Verde scuro
		m_game.GetTabbyGuy().ResetSoldiDelta();

	}
	else if (delta < 0)
	{
		m_lblSoldiDelta->SetLabel(m_game.GetSoldiStr(delta));
		m_lblSoldiDelta->SetForegroundColour(wxColor(200, 0, 0)); // Rosso
		m_game.GetTabbyGuy().ResetSoldiDelta();
	}
	else
		m_lblSoldiDelta->SetLabel("");

	m_lblReputazione->SetLabel(wxString::Format("Reputazione %d/100", guy.GetRep()));
	m_barRep->SetValue(guy.GetRep());
	m_lblFama->SetLabel(wxString::Format("Figosità %d/100", guy.GetFama()));
	m_barFama->SetValue(guy.GetFama());
	m_lblStudio->SetLabel(wxString::Format("Profitto scolastico %d/100", guy.GetStudio()));
	m_barStudio->SetValue(guy.GetStudio());
	// Lo scooter viene mostrato solo se posseduto
	if (m_game.GetTabbyGuy().HaScooter())
	{
		m_lblScooter->SetLabel(wxString::Format("%s\nStato scooter %d/100", guy.GetScooter().GetNome(), guy.GetScooter().GetStato()));
		m_barScooter->Show(true);
		m_barScooter->SetValue(guy.GetScooter().GetStato());
	}
	else
	{
		m_lblScooter->SetLabel("");
		m_barScooter->Hide();
	}

	m_lblDate->SetLabel(" " +
		wxString::Format("%s %d %s %d",
			m_game.GetDate().GetWeekDayStr().c_str(),
			m_game.GetDate().GetDay(),
			m_game.GetDate().GetMonthStr().c_str(),
			m_game.GetDate().GetYear()));

	// Forza il rinfresco del layout se le stringhe cambiano lunghezza
	this->Fit();
	this->Layout();
}

void TabbyFrame::OnScooter(wxCommandEvent& event)
{
	DlgScooter dlg{ this, m_game };
	dlg.Centre();
	dlg.ShowModal();
	this->AggiornaInterfaccia();
}

void TabbyFrame::OnDisco(wxCommandEvent& event)
{
	DlgDisco dlg{ this, m_game };
	dlg.Centre();
	dlg.ShowModal();
	this->AggiornaInterfaccia();
}

void TabbyFrame::OnScuola(wxCommandEvent& event)
{
	if (m_game.TriggerScuola())
	{
		DlgScuola dlg{ this, m_game };
		dlg.Centre();
		dlg.ShowModal();
		this->AggiornaInterfaccia();
	}
	else
		ManifestaEventi(this, m_game);	// Dentro trigger scuola si controlla se è vacanza, e nel caso viene creato il dialog popup

}

void TabbyFrame::OnNegozi(wxCommandEvent& event)
{
	DlgElencoNegozi dlg{ this, m_game };
	dlg.Centre();
	dlg.ShowModal();
	this->AggiornaInterfaccia();
}

void TabbyFrame::OnPalestra(wxCommandEvent& event)
{
	if (m_game.TriggerPalestra())			// Dentro trigger palestra si controlla se è vacanza, e nel caso viene creato il dialog popup
	{
		DlgPalestra dlg{ this, m_game };
		dlg.ShowModal();
		this->AggiornaInterfaccia();
	}
	else
		ManifestaEventi(this, m_game);
}

void TabbyFrame::OnTelefonino(wxCommandEvent& event)
{
	DlgTelefono dlg{ this, m_game };
	dlg.Centre();
	dlg.ShowModal();
	this->AggiornaInterfaccia();
}

void TabbyFrame::OnLavoro(wxCommandEvent& event)
{
	if (m_game.TriggerLavoro())
	{
		DlgLavoro dlg{ this, m_game };
		dlg.Centre();
		dlg.ShowModal();
		this->AggiornaInterfaccia();
	}
	else
		ManifestaEventi(this, m_game);
}

void TabbyFrame::OnCompagnia(wxCommandEvent& event)
{
	DlgCompagnia dlg{ this, m_game };
	dlg.Centre();
	dlg.ShowModal();
	this->AggiornaInterfaccia();
}

void TabbyFrame::OnFamiglia(wxCommandEvent& event)
{
	DlgFamiglia dlg{ this, m_game };
	dlg.Centre();
	dlg.ShowModal();
	this->AggiornaInterfaccia();
}

void TabbyFrame::OnTipa(wxCommandEvent& event)
{
	DlgTipa dlg{ this, m_game };
	dlg.Centre();
	dlg.ShowModal();
	this->AggiornaInterfaccia();
}

void TabbyFrame::OnPersonalInfo(wxCommandEvent& event)
{
	DlgPersonalInfo dlg(this, m_game);
	dlg.ShowModal();
	this->AggiornaInterfaccia();
}

void TabbyFrame::OnConfig(wxCommandEvent& event)
{
	DlgConfig dlg(this, m_game);
	dlg.ShowModal();
	this->AggiornaInterfaccia();
}

void TabbyFrame::OnAbout(wxCommandEvent& event)
{
	DlgAbout dlg(this);
	dlg.ShowModal();
}

void TabbyFrame::OnClose(wxCloseEvent& event)
{
	// Se la chiusura non può essere bloccata (es. il PC si sta spegnendo), lascia perdere e chiudi
	if (event.CanVeto())
	{
		DlgUscita dlg{ this };
		// Se l'utente NON preme OK (quindi preme Annulla o chiude la finestrella)
		if (dlg.ShowModal() != wxID_OK)
		{
			event.Veto(); // BLOCCA la chiusura della finestra
			return;
		}
		if (dlg.SpegnimentoRichiesto())
			wxGetApp().SetSpegnimentoRichiesto(true);
	}
	// Se l'utente ha premuto OK, lasciamo che wxWidgets distrugga la finestra
	event.Skip();
}