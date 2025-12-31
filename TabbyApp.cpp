#include "TabbyApp.h"

// TODO: CREARE PER SPECIFICARE I DATI DELLA FINESTRA DA FILE
// TODO: fai reperire la merda dai file, anche per i salvataggi
// TODO: METTI L'ID GIUSTO NEI BOTTONI
// TODO: AL POSTO DELL'ENUM ID, USA UN VETTORE
// TODO: AGGIUNGI MESS COMPLEANNO

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
	: wxFrame{ NULL, wxID_ANY, "Tabby Window", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)} // Blocca ridimensionamento (sono operatori bitwise su delle flag binarie)
{
	// Imposta colore di sfondo e il font
	this->SetBackgroundColour(WIN_BKG);
	this->SetFont(wxFont(13, wxFONTFAMILY_MODERN, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL));

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
	gridHeader->Add(2, 1, wxGBPosition(0, 5), wxGBSpan(2, 1));
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
	gridStats->Add(m_lblRapportoTipa, wxGBPosition(2,0), wxDefaultSpan, wxEXPAND | wxALL & ~wxBOTTOM, 10);
	m_barTipa = new wxGauge{ pnlStats, wxID_ANY, 100, wxDefaultPosition, wxSize(350, 10) };
	gridStats->Add(m_barTipa, wxGBPosition(3,0), wxDefaultSpan, wxALL & ~wxTOP, 10);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), wxGBPosition(4,0), wxGBSpan(1,3), wxEXPAND);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL), wxGBPosition(0,1), wxGBSpan(4,1), wxEXPAND & ~wxLEFT, 5);
	
	// Bottoni
	gridStats->Add(btnTipa, wxGBPosition(0, 2), wxDefaultSpan, wxALL, 7);
	gridStats->Add(btnCompagnia, wxGBPosition(2, 2), wxDefaultSpan, wxALL, 7);
	gridStats->Add(btnFamiglia, wxGBPosition(3, 2), wxDefaultSpan, wxALL, 7);
	
	// Soldi
	m_lblSoldi = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	gridStats->Add(m_lblSoldi, wxGBPosition(5, 0), wxGBSpan(1, 3), wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), wxGBPosition(6,0), wxGBSpan(1,3), wxEXPAND);
	
	// Reputazione
	m_lblReputazione = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	gridStats->Add(m_lblReputazione, wxGBPosition(7, 0), wxGBSpan(1,3), wxEXPAND | wxALL & ~wxBOTTOM, 10);
	m_barRep = new wxGauge{ pnlStats, wxID_ANY, 100, wxDefaultPosition, wxSize(350, 10) };
	gridStats->Add(m_barRep, wxGBPosition(8, 0), wxGBSpan(1, 3), wxALL & ~wxTOP, 10);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), wxGBPosition(9,0), wxGBSpan(1,3), wxEXPAND);
	
	// Figosità
	m_lblFigo = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	gridStats->Add(m_lblFigo, wxGBPosition(10, 0), wxGBSpan(1, 3), wxEXPAND | wxALL & ~wxBOTTOM, 10);
	m_barFigo = new wxGauge{ pnlStats, wxID_ANY, 100, wxDefaultPosition, wxSize(350, 10) };
	gridStats->Add(m_barFigo, wxGBPosition(11, 0), wxGBSpan(1, 3), wxALL & ~wxTOP, 10);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), wxGBPosition(12,0), wxGBSpan(1,3), wxEXPAND);
	
	// Profitto scolastico
	m_lblStudio = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	gridStats->Add(m_lblStudio, wxGBPosition(13, 0), wxGBSpan(1, 3), wxEXPAND | wxALL & ~wxBOTTOM, 10);
	m_barStudio = new wxGauge{ pnlStats, wxID_ANY, 100, wxDefaultPosition, wxSize(350, 10) };
	gridStats->Add(m_barStudio, wxGBPosition(14, 0), wxGBSpan(1, 3), wxALL & ~wxTOP, 10);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), wxGBPosition(15,0), wxGBSpan(1,3), wxEXPAND);
	
	// Stato scooter
	m_lblScooter = new wxStaticText{ pnlStats, wxID_ANY, "---" };
	gridStats->Add(m_lblScooter, wxGBPosition(16, 0), wxGBSpan(1, 3), wxALL & ~wxBOTTOM, 10);
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
	this->Fit();
	this->Centre();
	UpdateInterface();	// AGGIORNA INTERFACCIA
}

void TabbyFrame::UpdateInterface()
{ 
	TabbyGuy* guy = m_game.GetTabbyGuy();

	m_lblNomeTabby->SetLabel(guy->GetID()->m_nome + " " + guy->GetID()->m_cognome + " ");
	m_lblNomeTipa->SetLabel(guy->GetTipa()->GetNome());
	m_lblRapportoTipa->SetLabel(wxString::Format("< Rapporto con la tipa %d/100 >", guy->GetTipa()->GetRapporto()));
	m_barTipa->SetValue(guy->GetTipa()->GetRapporto());
	m_lblSoldi->SetLabel(wxString::Format("< Soldi  %d€ >", guy->GetSoldi()));
	m_lblReputazione->SetLabel(wxString::Format("< Reputazione %d/100 >", guy->GetRep()));
	m_barRep->SetValue(guy->GetRep());
	m_lblFigo->SetLabel(wxString::Format("< Figosità %d/100 >", guy->GetFigo()));
	m_barFigo->SetValue(guy->GetFigo());
	m_lblStudio->SetLabel(wxString::Format("< Profitto scolastico %d/100 >", guy->GetStudio()));
	m_barStudio->SetValue(guy->GetStudio());
	m_lblScooter->SetLabel(wxString::Format("< %s >\n< Stato scooter %d/100 >", guy->GetScooter()->GetNome(), guy->GetScooter()->GetEfficienza()));
	m_barScooter->SetValue(guy->GetScooter()->GetEfficienza());
	m_lblDate->SetLabel(" " + 
		wxString::Format("%s %d %s %d",
		m_game.GetDate()->GetWeekDayStr().c_str(),
		m_game.GetDate()->GetDay(),
		m_game.GetDate()->GetMonthStr().c_str(),
		m_game.GetDate()->GetYear()));

	// Forza il rinfresco del layout se le stringhe cambiano lunghezza
	this->Fit();
	this->Layout();
}

void TabbyFrame::OnScooter(wxCommandEvent& event)
{
	Scooter* scootptr = m_game.GetTabbyGuy()->GetScooter();
	int auxInt{};
	// Dichiariamo la finestra popup per lo scooter
	wxDialog scooterDialog{this, wxID_ANY, "Scooter", wxDefaultPosition, wxDefaultSize};
	scooterDialog.SetBackgroundColour(WIN_BKG);
	scooterDialog.SetFont(this->GetFont());

	// Layout orizzontale principale (Colonna SX | Colonna DX)
	wxBoxSizer* mainSizer = new wxBoxSizer{ wxHORIZONTAL };
	wxBoxSizer* leftCol = new wxBoxSizer{ wxVERTICAL };
	wxBoxSizer* rightCol = new wxBoxSizer{ wxVERTICAL };
	// Qua si poteva usare un semplice staticboxsizer, ma vogliamo la rientranza (non è supportata dal sizer), quindi bisogna fare prima un panel
	wxPanel* pnlConcessionario = new wxPanel{ &scooterDialog, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerConcessionario = new wxBoxSizer{ wxVERTICAL };
	wxPanel* pnlModifiche = new wxPanel{ &scooterDialog, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerModifiche = new wxBoxSizer{ wxVERTICAL };
	wxPanel* pnlBottom = new wxPanel{ &scooterDialog, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerBottom = new wxBoxSizer{ wxHORIZONTAL };
	
	wxPanel* pnlStats = new wxPanel{ &scooterDialog, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerStats = new wxBoxSizer{ wxVERTICAL };

	// --- COLONNA SINISTRA --------------------------------------------------------------
	// 1. CONCESSIONARIO
	sizerConcessionario->Add(new wxButton(pnlConcessionario, wxID_ANY, "Concessionario", wxDefaultPosition, wxSize(300, 50)), 0, wxEXPAND | wxALL, 5);
	pnlConcessionario->SetSizer(sizerConcessionario);
	leftCol->Add(pnlConcessionario, 0, wxEXPAND | wxALL, 5);

	// 2. MODIFICHE
	sizerModifiche->Add(new wxButton(pnlModifiche, wxID_ANY, "Trucca scooter", wxDefaultPosition, wxSize(300, 45)), 0, wxALL | wxEXPAND, 10);
	sizerModifiche->Add(new wxButton(pnlModifiche, wxID_ANY, "Ripara scooter", wxDefaultPosition, wxSize(300, 45)), 0, wxEXPAND | wxALL & ~wxTOP, 10);
	pnlModifiche->SetSizer(sizerModifiche);
	leftCol->Add(pnlModifiche, 0, wxEXPAND | wxALL, 5);

	// 3. INFO E OK
	auxInt = m_game.GetTabbyGuy()->GetSoldi();
	sizerBottom->Add(new wxStaticText(pnlBottom, wxID_ANY, "[IMG]"), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	sizerBottom->Add(new wxStaticText(pnlBottom, wxID_ANY, wxString::Format("< Soldi  %d€ >", auxInt)), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	sizerBottom->Add(new wxButton(pnlBottom, wxID_OK, "OK", wxDefaultPosition, wxSize(60, 50)), 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);
	pnlBottom->SetSizer(sizerBottom);
	leftCol->Add(pnlBottom, 1, wxEXPAND | wxALL, 5);

	// --- COLONNA DESTRA (Statistiche) ----------------------------------------------------------------------------
	sizerStats->Add(new wxStaticText(pnlStats, wxID_ANY, "Statistiche scooter"), 0, wxALIGN_CENTER);
	sizerStats->Add(new wxStaticText(pnlStats, wxID_ANY, "  " + scootptr->GetNome() + "  ", wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN | wxST_NO_AUTORESIZE), 0, wxEXPAND | wxALL & ~wxTOP, 10);
	// GRIGLIA DATI
	// Usiamo FlexGridSizer per allineare "Etichetta" -> "Valore"
	wxFlexGridSizer* gridStats = new wxFlexGridSizer{ 2, 5, 10 }; // 2 Colonne, gap di 5px e 10px

	AddScooterStat(pnlStats, gridStats, "Velocità:", wxString::Format("%d km/h", scootptr->GetVelocita()));
	AddScooterStat(pnlStats, gridStats, "Cilindrata:", wxString::Format("%d cc", scootptr->GetCilindrata()));
	// scrivo %% per escapeare '%'
	AddScooterStat(pnlStats, gridStats, "Efficienza:", wxString::Format("%d %%", scootptr->GetEfficienza()));
	AddScooterStat(pnlStats, gridStats, "Benzina:", wxString::Format("%.2f l", scootptr->GetBenza()));

	sizerStats->Add(gridStats, 0, wxEXPAND | wxALL & ~wxTOP, 10);

	// BOTTONI IN BASSO A DESTRA
	sizerStats->AddStretchSpacer();	// Spinge i bottoni in fondo
	sizerStats->Add(new wxButton(pnlStats, wxID_ANY, "Usa scooter", wxDefaultPosition, wxSize(300, 45)), 0, wxEXPAND | wxALL & ~wxBOTTOM, 10);
	sizerStats->Add(new wxButton(pnlStats, wxID_ANY, "Fai benza", wxDefaultPosition, wxSize(300, 45)), 0, wxEXPAND | wxALL & ~wxTOP, 10);

	pnlStats->SetSizer(sizerStats);
	rightCol->Add(pnlStats, 0, wxEXPAND | wxALL, 5);

	// --- CHIUSURA LAYOUT ---
	mainSizer->Add(leftCol, 0, wxEXPAND | wxALL, 5);
	mainSizer->Add(rightCol, 0, wxEXPAND | wxALL, 5);

	scooterDialog.SetSizer(mainSizer);
	mainSizer->SetSizeHints(&scooterDialog);
	scooterDialog.Centre();

	scooterDialog.ShowModal();
}

void TabbyFrame::AddScooterStat(wxWindow* parent, wxSizer* sizer, wxString label, wxString value)
{ 
	sizer->Add(new wxStaticText(parent, wxID_ANY, label, wxDefaultPosition, wxSize(-1, 25)));
	wxStaticText* valueBox = new wxStaticText{ parent, wxID_ANY, value, wxDefaultPosition, wxSize(200, 25), wxALIGN_CENTER | wxBORDER_SUNKEN };
	sizer->Add(valueBox, 0, wxALIGN_RIGHT);
}

void TabbyFrame::OnDisco(wxCommandEvent& event)
{

}
void TabbyFrame::OnScuola(wxCommandEvent& event)
{

}

void TabbyFrame::OnNegozi(wxCommandEvent& event)
{

}

void TabbyFrame::OnLavoro(wxCommandEvent& event)
{

}

void TabbyFrame::OnCompagnia(wxCommandEvent& event)
{

}

void TabbyFrame::OnFamiglia(wxCommandEvent& event)
{

}

void TabbyFrame::OnTipa(wxCommandEvent& event)
{

}

void TabbyFrame::OnEsci(wxCommandEvent& event)
{
	Close(true);
}