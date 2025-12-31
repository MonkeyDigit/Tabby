#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/statline.h>
#include <wx/gbsizer.h>

#define WIN_BKG wxColor(194, 196, 184)

// TODO: CREARE PER SPECIFICARE I DATI DELLA FINESTRA DA FILE
// TODO: fai reperire la merda dai file, anche per i salvataggi
// TODO: RICORDA DI AGGIORNARE L'INTERFACCIA QUANDO MODIFICHI ROBA
// TODO: nei bottoni metti l'id giusto

// ---------- APPLICAZIONE ------------

// il public fa in modo che la classe che stiame creando erediti i membri e le funzioni pubbliche di wxApp
class TabbyApp : public wxApp {
public:
	// il prefisso virtual indica che questa funzione esistente nella classe base, viene ridefinita nella classe figlia
	// override è semplicemente un meccanismo per controllare di aver ridefinito la funzione giusta, in caso contrario il compilatore si incazza
	// questa funzione viene chiamata quando viene creata la finestra
	virtual bool OnInit() override;
};

struct Scooter {
	std::string nome;
	int velocita;
	int cilindrata;
	int efficienza;
	float benzina;
};

// ---------- FINESTRA PRINCIPALE (Frame) -----------
class TabbyFrame : public wxFrame {
public:
	// Costruttore
	TabbyFrame();

private:
	// DATI
	int m_soldi;
	Scooter m_scooter;

	// FUNZIONI DI SUPPORTO
	void UpdateInterface();
	void AddScooterStat(wxWindow* scooterDialog, wxSizer* sizer, wxString label, wxString value);

	// GESTORE EVENTI (Cosa succede quando clicchi)
	void OnScooter(wxCommandEvent& event);
	void OnNegozi(wxCommandEvent& event);
	void OnDisco(wxCommandEvent& event);
	void OnScuola(wxCommandEvent& event);
	void OnLavoro(wxCommandEvent& event);
	void OnCompagnia(wxCommandEvent& event);
	void OnFamiglia(wxCommandEvent& event);
	void OnTipa(wxCommandEvent& event);
	void OnEsci(wxCommandEvent& event);
};

// In wxWidgets (e in Windows in generale), ogni singolo bottone, menu o casella di testo deve avere un'ID univoco (integer). wxWidgets ha già dei numeri prenotati per le sue cose interne (es. wxID_EXIT, wxID_OK, wxID_SAVE), solitamente sono numeri bassi o negativi, dunque per evitare conflitti si usano numeri alti per essere sicuri di stare fuori da quel range

// Definiamo i nostri ID
enum Widget_ID{
	// wxWidgets suggerisce di iniziare da wxID_HIGHEST per non fare cazzate
	ID_START_EVENT = wxID_HIGHEST + 1,
	// Attività
	ID_SCOOTER, ID_DISCO, ID_SCUOLA, ID_LAVORO, ID_COMPAGNIA, ID_FAMIGLIA, ID_TIPA,
	// Negozi
	ID_NEGOZI, ID_BAU_HOUSE, ID_BLUE_RIDER, ID_ZOCCOLARO, ID_FOOTSMOCKER, ID_FOOTSMOCKER_II, ID_BAR_TABACCHI, ID_PALESTRA, ID_TELEFONINO,
	// Special
	ID_INFO, ID_CONFIG,
	// Help
	ID_ABOUT, ID_LOGO
};

// ----------- IMPLEMENTAZIONE ---------------------

// Macro che crea il vero main() di Windows
wxIMPLEMENT_APP(TabbyApp);

// DEFINIZIONE FUNZIONI

bool TabbyApp::OnInit()
{
	// La finestra non è ridimensionabile
	TabbyFrame* frame = new TabbyFrame();
	frame->Show(true);
	return true;
}

// Costruttore finestra
// TODO: INIZIALIZZA DATI
TabbyFrame::TabbyFrame()
	: wxFrame{ NULL, wxID_ANY, "Tabby Window", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX) }, // Blocca ridimensionamento (sono operatori bitwise su delle flag binarie)
	m_soldi{50},
	m_scooter{"Mizzubisci R200 Millennium", 180, 150, 75, 5.0}
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
	// Crea il pannello globale dove appoggiare i controlli
	wxPanel* pnlMain = new wxPanel{ this, wxID_ANY };
	wxPanel* pnlStats = new wxPanel{ pnlMain, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxBORDER_SUNKEN };
	// SIZER GLOBALE (permette di mettere roba e di ridimensionare gli elementi)
	wxBoxSizer* mainSizer = new wxBoxSizer{ wxVERTICAL };
	// HEADER
	wxGridBagSizer* gridHeader = new wxGridBagSizer{ 5, 5 };	// gap verticale, gap orizzontale
	wxPanel* pnlNome = new wxPanel{ pnlMain, wxID_ANY, wxDefaultPosition, wxSize(-1, 40), wxBORDER_SUNKEN };
	wxBoxSizer* sizerNome = new wxBoxSizer{ wxHORIZONTAL };
	// BODY SOTTOSTANTE
	wxBoxSizer* sizerBody = new wxBoxSizer{ wxHORIZONTAL };
	wxPanel* pnlFoto = new wxPanel{ pnlMain, wxID_ANY, wxDefaultPosition, wxSize(300, 300), wxBORDER_SUNKEN };
	wxBoxSizer* sizerFoto = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizerStats = new wxBoxSizer(wxVERTICAL);
	wxGridBagSizer* gridStats = new wxGridBagSizer{ 0,0 };
	// Bottoni
	wxButton* btnScooter = new wxButton{ pnlMain, ID_SCOOTER, "Scooter", wxDefaultPosition, wxSize(140, -1) };
	wxButton* btnNegozi = new wxButton{ pnlMain, ID_NEGOZI, "Negozi", wxDefaultPosition, wxSize(140, -1) };
	wxButton* btnDisco = new wxButton{ pnlMain, ID_DISCO, "Disco", wxDefaultPosition, wxSize(140, -1) };
	wxButton* btnScuola = new wxButton{ pnlMain, ID_SCUOLA, "Scuola", wxDefaultPosition, wxSize(140, -1) };
	wxButton* btnLavoro = new wxButton{ pnlMain, ID_LAVORO, "Lavoro", wxDefaultPosition, wxSize(140, -1) };
	wxButton* btnTipa = new wxButton{ pnlStats, ID_TIPA, "Tipa", wxDefaultPosition, wxSize(140, -1) };
	wxButton* btnCompagnia = new wxButton{ pnlStats, ID_COMPAGNIA, "Compagnia", wxDefaultPosition, wxSize(140, -1) };
	wxButton* btnFamiglia = new wxButton{ pnlStats, ID_FAMIGLIA, "Famiglia", wxDefaultPosition, wxSize(140, -1) };

	// Associazione funzioni ai bottoni
	Bind(wxEVT_BUTTON, &TabbyFrame::OnScooter, this, ID_SCOOTER);
	Bind(wxEVT_BUTTON, &TabbyFrame::OnNegozi, this, ID_NEGOZI);
	Bind(wxEVT_BUTTON, &TabbyFrame::OnDisco, this, ID_DISCO);
	Bind(wxEVT_BUTTON, &TabbyFrame::OnScuola, this, ID_SCUOLA);
	Bind(wxEVT_BUTTON, &TabbyFrame::OnLavoro, this, ID_LAVORO);
	Bind(wxEVT_BUTTON, &TabbyFrame::OnTipa, this, ID_TIPA);
	Bind(wxEVT_BUTTON, &TabbyFrame::OnCompagnia, this, ID_COMPAGNIA);
	Bind(wxEVT_BUTTON, &TabbyFrame::OnFamiglia, this, ID_FAMIGLIA);


	// HEADER --------------------------------------------------------------------------------------------------
	// RIGA 0
	// Add(Oggetto, Posizione(Riga, Colonna), Estensione(Righe, Colonne), Flag, Bordo)
	gridHeader->Add(btnScooter, wxGBPosition(0, 0), wxDefaultSpan, wxALL, 5);	// wxALL impone di lasciare un padding in ogni direzione, di 5px
	gridHeader->Add(btnNegozi, wxGBPosition(0, 1), wxDefaultSpan, wxALL, 5);
	gridHeader->Add(btnDisco, wxGBPosition(0, 2), wxDefaultSpan, wxALL & ~wxRIGHT, 5);
	// Linea Verticale (Parte da riga 0, alta 2 righe)
	gridHeader->Add(new wxStaticLine(pnlMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL), wxGBPosition(0, 3), wxGBSpan(2, 1), wxEXPAND);	// wxEXPAND fa un override della dimensione ed estende l'elemento per tutta la lunghezza disponibile
	gridHeader->Add(btnScuola, wxGBPosition(0, 4), wxDefaultSpan, wxALL & ~wxLEFT, 5);
	// RIGA 1
	sizerNome->Add(new wxStaticText(pnlNome, wxID_ANY, " Nome"), 0, wxALIGN_CENTER_VERTICAL);
	sizerNome->AddStretchSpacer();	// Aggiungi spazio che si estende fino a destra
	sizerNome->Add(new wxStaticText(pnlNome, wxID_ANY, "Tizio Caio "), 0, wxALIGN_CENTER_VERTICAL);
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
	mainSizer->Add(new wxStaticLine(pnlMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), 0, wxEXPAND);


	// BODY ---------------------------------------------------------------------------------------------
	// FOTO
	pnlFoto->SetBackgroundColour(*wxWHITE);
	sizerFoto->Add(new wxStaticText(pnlFoto, wxID_ANY, "IMG TIZIO"), 0, wxALIGN_CENTER | wxTOP, 80);
	pnlFoto->SetSizer(sizerFoto);
	sizerBody->Add(pnlFoto, 0, wxEXPAND | wxALL, 5);

	// GRIGLIA STATISTICHE
	// Tipa
	gridStats->Add(new wxStaticText(pnlStats, wxID_ANY, "[Nome Tipa]"), wxGBPosition(0,0), wxDefaultSpan, wxEXPAND | wxALIGN_RIGHT | wxALL, 5);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), wxGBPosition(1,0), wxGBSpan(1,1), wxEXPAND);
	gridStats->Add(new wxStaticText(pnlStats, wxID_ANY, wxString::Format("< Rapporto con la tipa 96/100 >")), wxGBPosition(2,0), wxDefaultSpan, wxEXPAND | wxALL & ~wxBOTTOM, 10);
	wxGauge* barTipa = new wxGauge(pnlStats, wxID_ANY, 100, wxDefaultPosition, wxSize(200, 20));
	barTipa->SetValue(96);
	gridStats->Add(barTipa, wxGBPosition(3,0), wxDefaultSpan, wxALL & ~wxTOP, 10);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), wxGBPosition(4,0), wxGBSpan(1,3), wxEXPAND);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL), wxGBPosition(0,1), wxGBSpan(4,1), wxEXPAND & ~wxLEFT, 5);
	
	// Bottoni
	gridStats->Add(btnTipa, wxGBPosition(0, 2), wxDefaultSpan, wxALL, 7);
	gridStats->Add(btnCompagnia, wxGBPosition(2, 2), wxDefaultSpan, wxALL, 7);
	gridStats->Add(btnFamiglia, wxGBPosition(3, 2), wxDefaultSpan, wxALL, 7);
	
	// Soldi
	gridStats->Add(new wxStaticText(pnlStats, wxID_ANY, wxString::Format("< Soldi  %d€ >", m_soldi)), wxGBPosition(5, 0), wxGBSpan(1, 3), wxEXPAND | wxALIGN_CENTER_HORIZONTAL | wxALL, 10);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), wxGBPosition(6,0), wxGBSpan(1,3), wxEXPAND);
	
	// Reputazione
	gridStats->Add(new wxStaticText(pnlStats, wxID_ANY, wxString::Format("< Reputazione 84/100 >")), wxGBPosition(7, 0), wxGBSpan(1,3), wxEXPAND | wxALL & ~wxBOTTOM, 10);
	wxGauge* barRep = new wxGauge(pnlStats, wxID_ANY, 100, wxDefaultPosition, wxSize(200, 20));
	barRep->SetValue(84);
	gridStats->Add(barRep, wxGBPosition(8, 0), wxGBSpan(1, 3), wxALL & ~wxTOP, 10);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), wxGBPosition(9,0), wxGBSpan(1,3), wxEXPAND);
	
	// Figosità
	gridStats->Add(new wxStaticText(pnlStats, wxID_ANY, wxString::Format("< Figosità 92/100 >")), wxGBPosition(10, 0), wxGBSpan(1, 3), wxEXPAND | wxALL & ~wxBOTTOM, 10);
	wxGauge* barFigo = new wxGauge(pnlStats, wxID_ANY, 100, wxDefaultPosition, wxSize(200, 20));
	barFigo->SetValue(92);
	gridStats->Add(barFigo, wxGBPosition(11, 0), wxGBSpan(1, 3), wxALL & ~wxTOP, 10);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), wxGBPosition(12,0), wxGBSpan(1,3), wxEXPAND);
	
	// Profitto scolastico
	gridStats->Add(new wxStaticText(pnlStats, wxID_ANY, wxString::Format("< Profitto scolastico 6/100 >")), wxGBPosition(13, 0), wxGBSpan(1, 3), wxEXPAND | wxALL & ~wxBOTTOM, 10);
	wxGauge* barScuola = new wxGauge(pnlStats, wxID_ANY, 100, wxDefaultPosition, wxSize(200, 20));
	barScuola->SetValue(6);
	gridStats->Add(barScuola, wxGBPosition(14, 0), wxGBSpan(1, 3), wxALL & ~wxTOP, 10);
	gridStats->Add(new wxStaticLine(pnlStats, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), wxGBPosition(15,0), wxGBSpan(1,3), wxEXPAND);
	
	// Stato scooter
	gridStats->Add(new wxStaticText(pnlStats, wxID_ANY, wxString::Format("< %s >\n< Stato scooter 75/100 >", m_scooter.nome)), wxGBPosition(16, 0), wxGBSpan(1, 3), wxALL & ~wxBOTTOM, 10);
	wxGauge* barScooter = new wxGauge(pnlStats, wxID_ANY, 100, wxDefaultPosition, wxSize(200, 20));
	barScooter->SetValue(75);
	gridStats->Add(barScooter, wxGBPosition(17, 0), wxGBSpan(1, 3), wxALL & ~wxTOP, 10);
	
	// FISSIAMO LA GRID AL SIZER
	gridStats->AddGrowableCol(0);
	sizerStats->Add(gridStats, 1, wxEXPAND);
	pnlStats->SetSizer(sizerStats);
	sizerBody->Add(pnlStats, 1, wxEXPAND | wxALL, 5);

	// IMPLEMENTAZIONE BODY
	mainSizer->Add(sizerBody, 1, wxEXPAND);
	// IMPLEMENTAZIONE FINALE
	pnlMain->SetSizer(mainSizer);
	pnlMain->Layout();
	this->Fit();
	this->Centre();

	UpdateInterface();	// AGGIORNA INTERFACCIA
}

void TabbyFrame::UpdateInterface()
{
	// Forza il rinfresco del layout se le stringhe cambiano lunghezza
	this->Layout();
}

void TabbyFrame::OnScooter(wxCommandEvent& event)
{
	// Dichiariamo la finestra popup per lo scooter
	wxDialog scooterDialog{this, wxID_ANY, "Scooter", wxDefaultPosition, wxDefaultSize};
	scooterDialog.SetBackgroundColour(WIN_BKG);
	scooterDialog.SetFont(this->GetFont());

	// Layout orizzontale principale (Colonna SX | Colonna DX)
	wxBoxSizer* mainSizer = new wxBoxSizer{ wxHORIZONTAL };
	wxBoxSizer* leftCol = new wxBoxSizer{ wxVERTICAL };
	wxBoxSizer* rightCol = new wxBoxSizer{ wxVERTICAL };
	wxPanel* pnlStats = new wxPanel{ &scooterDialog, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerStats = new wxBoxSizer{ wxVERTICAL };

	// --- COLONNA SINISTRA --------------------------------------------------------------
	// 1. CONCESSIONARIO
	// 
	// Qua si poteva usare un semplice staticboxsizer, ma vogliamo la rientranza (non è supportata dal sizer), quindi bisogna fare prima un panel
	wxPanel* pnlConcessionario = new wxPanel{ &scooterDialog, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerConcessionario = new wxBoxSizer{ wxVERTICAL };

	pnlConcessionario->SetBackgroundColour(GetBackgroundColour());
	sizerConcessionario->Add(new wxButton(pnlConcessionario, wxID_ANY, "Concessionario", wxDefaultPosition, wxSize(300, 50)), 0, wxEXPAND | wxALL, 10);
	pnlConcessionario->SetSizer(sizerConcessionario);

	leftCol->Add(pnlConcessionario, 0, wxEXPAND | wxALL, 5);

	// 2. MODIFICHE
	wxPanel* pnlModifiche = new wxPanel{ &scooterDialog, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	pnlModifiche->SetBackgroundColour(GetBackgroundColour());
	wxBoxSizer* sizerModifiche = new wxBoxSizer{ wxVERTICAL };
	sizerModifiche->Add(new wxButton(pnlModifiche, wxID_ANY, "Trucca scooter", wxDefaultPosition, wxSize(300, 45)), 0, wxALL | wxEXPAND, 10);
	sizerModifiche->Add(new wxButton(pnlModifiche, wxID_ANY, "Ripara scooter", wxDefaultPosition, wxSize(300, 45)), 0, wxEXPAND | wxALL & ~wxTOP, 10);
	pnlModifiche->SetSizer(sizerModifiche);
	leftCol->Add(pnlModifiche, 0, wxEXPAND | wxALL, 5);

	// 3. INFO E OK
	wxBoxSizer* bottomRow = new wxBoxSizer{wxHORIZONTAL};
	wxPanel* pnlSoldi = new wxPanel{ &scooterDialog, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	pnlSoldi->SetBackgroundColour(GetBackgroundColour()); // Grigio standard
	// Creiamo un sizer orizzontale INTERNO al pannello soldi (per allineare Icona e Testo)
	wxBoxSizer* sizerInternoSoldi = new wxBoxSizer(wxHORIZONTAL);
	// Nota: il genitore è pnlSoldi!
	sizerInternoSoldi->Add(new wxStaticText(pnlSoldi, wxID_ANY, "[IMG]"), 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	// Testo dei soldi
	sizerInternoSoldi->Add(new wxStaticText(pnlSoldi, wxID_ANY, wxString::Format("Soldi: %d€", m_soldi)), 0, wxALIGN_CENTER_VERTICAL | wxLEFT, 5);
	// Chiudiamo il pannello dei soldi
	pnlSoldi->SetSizer(sizerInternoSoldi);
	// AGGIUNGIAMO IL BOX SOLDI ALLA FILA (Si espande per spingere OK a destra)
	bottomRow->Add(pnlSoldi, 1, wxEXPAND | wxRIGHT, 5);
	// Lo aggiungiamo alla fila
	wxButton* btnOk = new wxButton(&scooterDialog, wxID_OK, "OK", wxDefaultPosition, wxSize(60, 50));
	bottomRow->Add(btnOk, 0, wxEXPAND);
	// --- AGGIUNTA FINALE ALLA COLONNA SINISTRA ---
	// Aggiungiamo tutta la fila in fondo alla colonna
	leftCol->Add(bottomRow, 0, wxEXPAND | wxALL, 5);


	// --- COLONNA DESTRA (Statistiche) ----------------------------------------------------------------------------
	sizerStats->Add(new wxStaticText(pnlStats, wxID_ANY, "Statistiche scooter"), 0, wxALIGN_CENTER);
	// Il -1 in wxsize significa AUTO
	wxStaticText* lblNomeScooter = new wxStaticText(pnlStats, wxID_ANY, "  "+m_scooter.nome+"  ", wxDefaultPosition, wxSize(-1, 25), wxALIGN_CENTER | wxBORDER_SUNKEN | wxST_NO_AUTORESIZE);
	sizerStats->Add(lblNomeScooter, 0, wxEXPAND | wxALL & ~wxTOP, 10);

	// GRIGLIA DATI
	// Usiamo FlexGridSizer per allineare "Etichetta" -> "Valore"
	wxFlexGridSizer* gridStats = new wxFlexGridSizer{ 2, 5, 10 }; // 2 Colonne, gap di 5px e 10px

	AddScooterStat(pnlStats, gridStats, "Velocità:", wxString::Format("%d km/h", m_scooter.velocita));
	AddScooterStat(pnlStats, gridStats, "Cilindrata:", wxString::Format("%d cc", m_scooter.cilindrata));
	// scrivo %% per escapeare '%'
	AddScooterStat(pnlStats, gridStats, "Efficienza:", wxString::Format("%d %%", m_scooter.efficienza));
	AddScooterStat(pnlStats, gridStats, "Benzina:", wxString::Format("%.2f l", m_scooter.benzina));

	sizerStats->Add(gridStats, 0, wxEXPAND | wxALL & ~wxTOP, 10);

	// BOTTONI IN BASSO A DESTRA
	sizerStats->AddStretchSpacer();	// Spinge i bottoni in fondo
	sizerStats->Add(new wxButton(pnlStats, wxID_ANY, "Usa scooter", wxDefaultPosition, wxSize(300, 45)), 0, wxEXPAND | wxALL & ~wxBOTTOM, 10);
	sizerStats->Add(new wxButton(pnlStats, wxID_ANY, "Fai benza", wxDefaultPosition, wxSize(300, 45)), 0, wxEXPAND | wxALL & ~wxTOP, 10);

	pnlStats->SetSizer(sizerStats);
	rightCol->Add(pnlStats, 0, wxEXPAND | wxALL, 5);

	// --- CHIUSURA LAYOUT ---
	mainSizer->Add(leftCol, 0, wxEXPAND | wxALL, 5);
	mainSizer->Add(rightCol, 1, wxEXPAND | wxALL, 5);	// La destra si espande

	scooterDialog.SetSizer(mainSizer);
	mainSizer->SetSizeHints(&scooterDialog);
	scooterDialog.Centre();

	scooterDialog.ShowModal();
}

void TabbyFrame::AddScooterStat(wxWindow* parent, wxSizer* sizer, wxString label, wxString value)
{ 
	sizer->Add(new wxStaticText(parent, wxID_ANY, label, wxDefaultPosition, wxSize(-1, 25)), 0);
	wxStaticText* valueBox = new wxStaticText{ parent, wxID_ANY, value, wxDefaultPosition, wxSize(200, 25), wxALIGN_CENTER | wxBORDER_SUNKEN | wxST_NO_AUTORESIZE };
	sizer->Add(valueBox, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
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