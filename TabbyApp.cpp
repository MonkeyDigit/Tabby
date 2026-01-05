#include "TabbyApp.h"
#include "AppDialogs.h"

// TODO: CREARE PER SPECIFICARE I DATI DELLA FINESTRA DA FILE
// TODO: fai reperire la merda dai file, anche per i salvataggi
// TODO: AGGIUNGI MESS COMPLEANNO
// TODO: SISTEMA COMMENTI
// TODO: MECCANICHE FUTURE: con l'avanzamento tecnologico, escono nuove robe. + investimenti in borsa, crypto, per per arricchirsi...
// TODO: La scuola la fa per 5 anni poi va a lavorare e investire in borsa???
// TODO: SISTEMA BEST PRACTICE
// TODO: Attento ai range dei vettori nei random
// TODO: FAI FUNZIONI ISOLATE per la roba degli eventi
// TODO: metti const per le pass by reference
// TODO: msg sborsa spilla investi. e io pago!
// TODO: METTI I PUNTI
// TODO: SPOSTA TUTTI I VETTORI DI STRINGHE E DIALOGHI IN UN HEADER DEDICATO
// TODO: cosa cambia con setsizer and fit
// TODO: Rizz sfx
// TODO: Controlla bene calcolastudio

bool TabbyApp::OnInit()
{
	// La finestra non è ridimensionabile
	TabbyFrame* frame = new TabbyFrame();
	frame->Show(true);
	return true;
}
// ----------- IMPLEMENTAZIONE ---------------------

// Macro che crea il vero main() di Windows
wxIMPLEMENT_APP(TabbyApp);
// Costruttore finestra
TabbyFrame::TabbyFrame()
	: wxFrame{ NULL, wxID_ANY, "Tabby Window", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)}, // Blocca ridimensionamento (sono operatori bitwise su delle flag binarie)
	m_game{}
{
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
	menuNegozi->Append(ID_BAU_HOUSE, "Bau House");
	menuNegozi->Append(ID_BLUE_RIDER, "Blue Rider");
	menuNegozi->Append(ID_ZOCCOLARO, "Zoccolaro");
	menuNegozi->Append(ID_FOOTSMOCKER, "Footsmocker");
	menuNegozi->Append(ID_FOOTSMOCKER_II, "Footsmocker II");
	menuNegozi->AppendSeparator();
	menuNegozi->Append(ID_BAR_TABACCHI, "Bar Tabacchi");
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
	Bind(wxEVT_MENU, &TabbyFrame::OnScooter, this, ID_SCOOTER);
	Bind(wxEVT_MENU, &TabbyFrame::OnDisco, this, ID_DISCO);
	Bind(wxEVT_MENU, &TabbyFrame::OnScuola, this, ID_SCUOLA);
	Bind(wxEVT_MENU, &TabbyFrame::OnLavoro, this, ID_LAVORO);
	Bind(wxEVT_MENU, &TabbyFrame::OnCompagnia, this, ID_COMPAGNIA);
	Bind(wxEVT_MENU, &TabbyFrame::OnFamiglia, this, ID_FAMIGLIA);
	Bind(wxEVT_MENU, &TabbyFrame::OnTipa, this, ID_TIPA);

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
	Bind(wxEVT_BUTTON, &TabbyFrame::OnScooter, this, ID_SCOOTER);
	Bind(wxEVT_BUTTON, &TabbyFrame::OnNegozi, this, ID_NEGOZI);
	Bind(wxEVT_BUTTON, &TabbyFrame::OnDisco, this, ID_DISCO);
	Bind(wxEVT_BUTTON, &TabbyFrame::OnScuola, this, ID_SCUOLA);
	Bind(wxEVT_BUTTON, &TabbyFrame::OnLavoro, this, ID_LAVORO);
	Bind(wxEVT_BUTTON, &TabbyFrame::OnTipa, this, ID_TIPA);
	Bind(wxEVT_BUTTON, &TabbyFrame::OnCompagnia, this, ID_COMPAGNIA);
	Bind(wxEVT_BUTTON, &TabbyFrame::OnFamiglia, this, ID_FAMIGLIA);

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
	mainSizer->Add(new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), 0, wxEXPAND);

	// BODY ---------------------------------------------------------------------------------------------
	// FOTO
	wxPanel* pnlFoto = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxSize(300, 300), wxBORDER_SUNKEN };
	wxBoxSizer* sizerFoto = new wxBoxSizer(wxVERTICAL);
	pnlFoto->SetBackgroundColour(*wxWHITE);
	sizerFoto->Add(new wxStaticText(pnlFoto, wxID_ANY, "IMG TIZIO"), 0, wxALIGN_CENTER | wxTOP, 80);
	pnlFoto->SetSizer(sizerFoto);
	wxBoxSizer* sizerBody = new wxBoxSizer{ wxHORIZONTAL };
	sizerBody->Add(pnlFoto, 0, wxEXPAND | wxALL, 5);

	// GRIGLIA STATISTICHE
	wxPanel* pnlStats = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxBORDER_SUNKEN };
	wxGridBagSizer* gridStats = new wxGridBagSizer{ 0,0 };
	wxBoxSizer* sizerStats = new wxBoxSizer(wxVERTICAL);
	
	// Bottoni laterali
	wxButton* btnTipa = new wxButton{ pnlStats, ID_TIPA, "Tipa", wxDefaultPosition, wxSize(140, -1) };
	wxButton* btnCompagnia = new wxButton{ pnlStats, ID_COMPAGNIA, "Compagnia", wxDefaultPosition, wxSize(140, -1) };
	wxButton* btnFamiglia = new wxButton{ pnlStats, ID_FAMIGLIA, "Famiglia", wxDefaultPosition, wxSize(140, -1) };
	
	// Tipa
	m_lblNomeTipa = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	gridStats->Add(m_lblNomeTipa, wxGBPosition(0,0), wxDefaultSpan, wxEXPAND | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT | wxALL, 5);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), wxGBPosition(1,0), wxGBSpan(1,1), wxEXPAND);
	m_lblRapportoTipa = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	gridStats->Add(m_lblRapportoTipa, wxGBPosition(2,0), wxDefaultSpan, wxEXPAND | wxALL, 10);
	m_barTipa = new wxGauge{ pnlStats, wxID_ANY, 100, wxDefaultPosition, wxSize(350, 10) };
	gridStats->Add(m_barTipa, wxGBPosition(3,0), wxDefaultSpan, wxALL & ~wxTOP, 10);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), wxGBPosition(4,0), wxGBSpan(1,3), wxEXPAND);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL), wxGBPosition(0,1), wxGBSpan(4,1), wxEXPAND & ~wxLEFT, 5);
	
	// Bottoni
	gridStats->Add(btnTipa, wxGBPosition(0, 2), wxDefaultSpan, wxALL, 7);
	gridStats->Add(btnCompagnia, wxGBPosition(2, 2), wxDefaultSpan, wxALL, 7);
	gridStats->Add(btnFamiglia, wxGBPosition(3, 2), wxDefaultSpan, wxALL, 7);
	
	// Soldi
	wxBoxSizer* sizerSoldiRow = new wxBoxSizer(wxHORIZONTAL);
	m_lblSoldi = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	// Soldi Delta - Singoli guadagni e perdite
	m_lblSoldiDelta = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	m_lblSoldiDelta->SetFont(wxFont(10, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	sizerSoldiRow->Add(m_lblSoldi, 0, wxALIGN_CENTER_VERTICAL);
	sizerSoldiRow->AddSpacer(10); // Un po' di spazio tra il saldo e la variazione
	sizerSoldiRow->Add(m_lblSoldiDelta, 0, wxALIGN_CENTER_VERTICAL);
	gridStats->Add(sizerSoldiRow, wxGBPosition(5, 0), wxGBSpan(1, 3), wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), wxGBPosition(6,0), wxGBSpan(1,3), wxEXPAND);

	// Reputazione
	m_lblReputazione = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	gridStats->Add(m_lblReputazione, wxGBPosition(7, 0), wxGBSpan(1,3), wxEXPAND | wxALL, 10);
	m_barRep = new wxGauge{ pnlStats, wxID_ANY, 100, wxDefaultPosition, wxSize(350, 10) };
	gridStats->Add(m_barRep, wxGBPosition(8, 0), wxGBSpan(1, 3), wxALL & ~wxTOP, 10);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), wxGBPosition(9,0), wxGBSpan(1,3), wxEXPAND);
	
	// Figosità
	m_lblFigo = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	gridStats->Add(m_lblFigo, wxGBPosition(10, 0), wxGBSpan(1, 3), wxEXPAND | wxALL, 10);
	m_barFigo = new wxGauge{ pnlStats, wxID_ANY, 100, wxDefaultPosition, wxSize(350, 10) };
	gridStats->Add(m_barFigo, wxGBPosition(11, 0), wxGBSpan(1, 3), wxALL & ~wxTOP, 10);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), wxGBPosition(12,0), wxGBSpan(1,3), wxEXPAND);
	
	// Profitto scolastico
	m_lblStudio = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	gridStats->Add(m_lblStudio, wxGBPosition(13, 0), wxGBSpan(1, 3), wxEXPAND | wxALL, 10);
	m_barStudio = new wxGauge{ pnlStats, wxID_ANY, 100, wxDefaultPosition, wxSize(350, 10) };
	gridStats->Add(m_barStudio, wxGBPosition(14, 0), wxGBSpan(1, 3), wxALL & ~wxTOP, 10);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), wxGBPosition(15,0), wxGBSpan(1,3), wxEXPAND);
	
	// Stato scooter
	m_lblScooter = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	gridStats->Add(m_lblScooter, wxGBPosition(16, 0), wxGBSpan(1, 3), wxALL, 10);
	m_barScooter = new wxGauge{ pnlStats, wxID_ANY, 100, wxDefaultPosition, wxSize(350, 10) };
	gridStats->Add(m_barScooter, wxGBPosition(17, 0), wxGBSpan(1, 3), wxALL & ~wxTOP, 10);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), wxGBPosition(18,0), wxGBSpan(1,3), wxEXPAND);

	// Data
	wxPanel* pnlDate = new wxPanel{ pnlStats, wxID_ANY, wxDefaultPosition, wxSize(-1,40), wxBORDER_SUNKEN};
	wxBoxSizer* sizerDate = new wxBoxSizer{ wxHORIZONTAL };
	m_lblDate = new wxStaticText{ pnlDate, wxID_ANY, "---" };
	pnlDate->SetSizer(sizerDate);
	sizerDate->Add(m_lblDate, wxEXPAND | wxALL & ~wxBOTTOM, 5);
	sizerDate->AddStretchSpacer();
	wxButton* btnAbout = new wxButton{ pnlDate, ID_ABOUT, "About", wxDefaultPosition, wxSize(140, -1) };
	sizerDate->Add(btnAbout, 0, wxALL, 2);
	gridStats->Add(pnlDate, wxGBPosition(19, 0), wxGBSpan(1, 3), wxEXPAND | wxALL, 5);
	
	// FISSIAMO LA GRID AL SIZER
	gridStats->AddGrowableCol(0);
	sizerStats->Add(gridStats, 1, wxEXPAND);
	pnlStats->SetSizer(sizerStats);
	sizerBody->Add(pnlStats, 1, wxEXPAND | wxALL, 5);

	// IMPLEMENTAZIONE BODY
	mainSizer->Add(sizerBody, 1, wxEXPAND);
	// IMPLEMENTAZIONE FINALE
	this->SetSizer(mainSizer);
	this->Fit();	// Chiede al Sizer principale (mainSizer) quanto spazio gli serve al minimo per far stare dentro tutti i widget (bottoni, pannelli, immagini) senza schiacciarli. Poi ridimensiona la finestra (TabbyFrame) esattamente a quella misura
	this->Centre();	// Posiziona la finestra al centro dello schermo. Senza, la finestra si aprirebbe nell'angolo in alto a sinistra (coordinate 0,0) o dove decide Windows a caso
	this->AggiornaInterfaccia();	// AGGIORNA INTERFACCIA
}

void TabbyFrame::AggiornaInterfaccia()
{ 
	TabbyGuy& guy = m_game.GetTabbyGuy();

	m_lblNomeTabby->SetLabel(guy.GetID().m_nome + " " + guy.GetID().m_cognome + " ");
	m_lblNomeTipa->SetLabel(guy.GetTipa().GetNome() + " ");
	m_lblRapportoTipa->SetLabel(wxString::Format("< Rapporto con la tipa %d/100 >", guy.GetRapporti()));
	m_barTipa->SetValue(guy.GetRapporti());
	m_lblSoldi->SetLabel("< Soldi " + m_game.GetSoldiStr(guy.GetSoldi()) + " >");
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

	m_lblReputazione->SetLabel(wxString::Format("< Reputazione %d/100 >", guy.GetRep()));
	m_barRep->SetValue(guy.GetRep());
	m_lblFigo->SetLabel(wxString::Format("< Figosità %d/100 >", guy.GetFama()));
	m_barFigo->SetValue(guy.GetFama());
	m_lblStudio->SetLabel(wxString::Format("< Profitto scolastico %d/100 >", guy.GetStudio()));
	m_barStudio->SetValue(guy.GetStudio());
	m_lblScooter->SetLabel(wxString::Format("< %s >\n< Stato scooter %d/100 >", guy.GetScooter().GetNome(), guy.GetScooter().GetEfficienza()));
	m_barScooter->SetValue(guy.GetScooter().GetEfficienza());
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
		ManifestaEventi(this, m_game);	// Dentro trigger scuola si controlla se è vacanza, e nel caso viene creato il dialog popup
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

}

void TabbyFrame::OnEsci(wxCommandEvent& event)
{
	Close(true);
}