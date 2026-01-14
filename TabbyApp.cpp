#include <wx/config.h>	// Per il salvataggio nei registri di sistema
#include "TabbyApp.h"
#include "AppDialogs.h"

// TODO: SISTEMA COMMENTI
// TODO: MECCANICHE FUTURE: con l'avanzamento tecnologico, escono nuove robe. + investimenti in borsa, crypto, per per arricchirsi...
// TODO: La scuola la fa per 5 anni poi va a lavorare e investire in borsa???
// TODO: SISTEMA BEST PRACTICE
// TODO: Attento ai range dei vettori nei random
// TODO: metti const per le pass by reference
// TODO: IMPEDIRE DI COMPRARE VESTITI GIA' POSSEDUTI
// TODO: ARMADIO VESTITI?
// TODO: FAI EXPAND AI BOTTONI AL POSTO DI DARE IL SIZE, PER UNIFORMARLI
// TODO: METTI LE GRAFFE NEGLI INIZIALIZZATORI
// TODO: LA FAMA DEI VESTITI E' SMINCHIATA ?
// TODO: NON PER TUTTI I SIZER SERVE IL PANEL
// TODO: this fit this layout non si capisce nulla
// TODO: ATTENTO ALL'ORDINE DI CHIUSURA E MANIFESTAEVENTI
// TODO: wxgridbag flexgrid gridsizer ???
// TODO: OFFERTA NATALIZIA
// TODO: METTI LINEE ORIZZONTALI TRA SIZER AL POSTO DI DOPPI SUNKEN
// TODO: ATTENTO ALLA STRINGA DEI SOLDI LUNGA -> USA align center al posto di wxexpand
// TODO: METTI FIT IN AGGIORNAINTERFACCIA
// TODO: RICORDA DI METTERE LE MINI ICONE + ICONA GIOCO
// TODO: SOSTITUISCI TUTTI I PULSANTI CHIUDI E ESCI CON OK
// TODO: AGGIUNGI COSTRUTTORI DEFAULT
// TODO: HEADER UTILITY CON LE FUNZIONI DEFINITE NEI CPP
// TODO: CONTROLLA TUTTI GLI ELSE MANIFESTAEVENTO
// TODO: CONTROLLA TUTTE LE POSIZIONI DI EVENTO NEL CODICE ORIGINALE
// TODO: TRIGGERPALESTRA ANCHE PER LAMPADA E ABBONAMENTI
// TODO: COSA FA IL TIMER??
// TODO: CONTROLLA TUTTE LE STATIC LINE
// TODO: SISTEMA I COSTRUTTORI DI WXDIALOG
// TODO: CONTROLLA LA SIZE DI TUTTI I PNLFOTO
// TODO: CONTROLLA TUTTI I COMMENTI // e /**/
// TODO: IMPOSTA LA FAMA DEI CELL
// TODO: CAMBIA FIGOSITA' VESTITI E NEGOZI
// TODO: METTI PREZZI LAMPADA E PALESTRA DENTRO TABBYGAME

bool TabbyApp::OnInit()
{	// Carica i dati di salvataggio dai registri di sistema
	if (!CaricaDatiRegistro()) {
		// Se il caricamento fallisce o è la prima volta, m_game è già inizializzato dal suo costruttore
	}
	// importante per caricare le immagini
	wxInitAllImageHandlers();
	TabbyFrame* frame = new TabbyFrame(m_game);
	frame->Show(true);
	return true;
}
int TabbyApp::OnExit()
{	// Salva i dati ogni volta che esci
	SalvaDatiRegistro();
	return wxApp::OnExit();
}

// --- SALVATAGGIO REGISTRO ---
void TabbyApp::SalvaDatiRegistro()
{
	wxConfig config("Tabby", "MonkeyDigit");
	
	// DATI DI TABBYGUY
	TabbyGuy& guy = m_game.GetTabbyGuy();
	config.Write("soldi", guy.GetSoldi());
	config.Write("fama", guy.GetFama());
	config.Write("paghetta", guy.GetPaghetta());
	config.Write("reputazione", guy.GetRep());
	config.Write("studio", guy.GetStudio());
	config.Write("rapporti", guy.GetRapporti());
	config.Write("sizze", guy.GetSizze());
	config.Write("pelle", (int)guy.GetPelle());
	config.Write("fortuna", guy.GetFortuna());

	// DATI CARTA D'IDENTITA'
		const CartaIdentita& id = guy.GetID();
		config.Write("nome", (wxString)id.m_nome);
		config.Write("cognome", (wxString)id.m_cognome);
		config.Write("giorno_nascita", id.m_dataNascita.GetDay());
		config.Write("mese_nascita", (int)id.m_dataNascita.GetMonth());
		config.Write("anno_nascita", id.m_dataNascita.GetYear());
		config.Write("atto_nascita", (wxString)id.m_attoNascita);
		config.Write("luogo_nascita", (wxString)id.m_luogoNascita);
		config.Write("provincia_nascita", (wxString)id.m_provinciaNascita);
		config.Write("cittadinanza", (wxString)id.m_cittadinanza);
		config.Write("residenza", (wxString)id.m_residenza);
		config.Write("indirizzo", (wxString)id.m_indirizzo);
		config.Write("stato_civile", (wxString)id.m_statoCivile);
		config.Write("professione", (wxString)id.m_professione);
	// DATI VESTITI
		auto SalvaVestito = [&](const Vestito& v, const wxString& suffix) {
			config.Write("tipo_" + suffix, (int)v.GetTipoVestito());
			config.Write("nome_" + suffix, (wxString)v.GetNome());
			config.Write("desc_" + suffix, (wxString)v.GetDesc());
			config.Write("img_" + suffix, (wxString)v.GetImageStr());
			config.Write("prezzo_" + suffix, v.GetPrezzo());
			config.Write("fama_" + suffix, v.GetFama());
			};

	SalvaVestito(guy.GetGiubbotto(), "giubbotto");
	SalvaVestito(guy.GetPantaloni(), "pantaloni");
	SalvaVestito(guy.GetScarpe(), "scarpe");
	// DATI CARRIERA
		const Carriera& carr = guy.GetCarriera();
		config.Write("nome_ditta", (wxString)carr.GetNomeDitta());
		config.Write("stipendio", carr.GetStipendio());
		config.Write("impegno", carr.GetImpegno());
		config.Write("giorni_lavorati", carr.GetGiorniLavorati());
	// DATI SCUOLA
		const Scuola& scuola = guy.GetScuola();
		config.Write("scuola_num_materie", (int)scuola.m_materie.size());
		for (int i = 0; i < scuola.m_materie.size(); i++)
		{
			config.Write(wxString::Format("materia_%d", i), (wxString)scuola.m_materie[i].GetNome());
			config.Write(wxString::Format("voto_%d", i), scuola.m_materie[i].GetVoto());
		}
	// DATI SCOOTER
		const Scooter& scoot = guy.GetScooter();
		config.Write("nome_scooter", (wxString)scoot.GetNome());
		config.Write("img_scooter", (wxString)scoot.GetImageStr());
		config.Write("prezzo_scooter", scoot.GetPrezzo());
		config.Write("fama_scooter", scoot.GetFama());
		config.Write("serbatoio_scooter", scoot.GetCapSerbatoio());
		config.Write("attivita_scooter", (int)scoot.GetAttivita());
		// Lambda helper per i pezzi dello scooter
		auto SalvaPezzo = [&](const Pezzo& p, const wxString& suffix) {
			config.Write("tipo_" + suffix, (int)p.GetTipo());
			config.Write("nome_" + suffix, (wxString)p.GetNome());
			config.Write("img_" + suffix, (wxString)p.GetImageStr());
			config.Write("prezzo_" + suffix, p.GetPrezzo());
			config.Write("power_" + suffix, p.GetPower());
			};

		// Utilizzo (Sostituisci tutto quel blocco lungo con questo):
		SalvaPezzo(scoot.GetMarmitta(), "marmitta");
		SalvaPezzo(scoot.GetFiltro(), "filtro");
		SalvaPezzo(scoot.GetCarburatore(), "carburatore");
		SalvaPezzo(scoot.GetCilindrata(), "cilindrata");
	// FINE SCOOTER
	// DATI TIPA
		const Tipa& tipa = guy.GetTipa();
		config.Write("nome_tipa", (wxString)tipa.GetNome());
		config.Write("fama_tipa", tipa.GetFama());
	// DATI TELEFONO
		const Telefono& tel = guy.GetTelefono();
		config.Write("nome_tel", (wxString)tel.GetNome());
		config.Write("desc_tel", (wxString)tel.GetDesc());
		config.Write("img_tel", (wxString)tel.GetImageStr());
		config.Write("prezzo_tel", tel.GetPrezzo());
		config.Write("fama_tel", tel.GetFama());
		config.Write("stato_tel", tel.GetStato());
		config.Write("credito_tel", tel.GetCredito());
		//	DATI ABBONAMENTO
		const Abbonamento& abb = tel.GetAbbonamento();
		config.Write("nome_abb", (wxString)abb.GetNome());
		config.Write("img_abb", (wxString)abb.GetImageStr());
		config.Write("attivazione_abb", abb.GetCostoAttivazione());
		// RICARICHE
		config.Write("ricariche_count", (int)abb.GetRicariche().size());
		for (int i = 0; i < abb.GetRicariche().size(); i++)
			config.Write(wxString::Format("ricarica_%d", i), abb.GetRicariche()[i]);
	// FINE TELEFONO

	// ALTRI DATI DI GIOCO
		config.Write("data_giorno", m_game.GetDate().GetDay());
		config.Write("data_mese", (int)m_game.GetDate().GetMonth());
		config.Write("data_anno", m_game.GetDate().GetYear());
		config.Write("scadenza_pal_giorno", m_game.GetScadenzaPal().GetDay());
		config.Write("scadenza_pal_mese", (int)m_game.GetScadenzaPal().GetMonth());
		config.Write("scadenza_pal_anno", m_game.GetScadenzaPal().GetYear());
		config.Write("valuta_corrente", (int)m_game.GetValutaCorrente());
		config.Write("cooldown_pestaggio", m_game.GetCoolDownPestaggio());
		config.Write("cooldown_pelle", m_game.GetCoolDownPelle());
		config.Write("palo_count", m_game.GetPaloCount());
		config.Write("attesa", m_game.GetAttesa());
		config.Write("tipo_giorno", (int)m_game.GetTipoGiorno());
		config.Write("sound_active", m_game.GetSoundActive());
	
	// ANTI-CHEAT
	long long checksum = m_game.CalcolaChecksum(guy.GetSoldi(), guy.GetRep(), guy.GetFama(), guy.GetStudio(), guy.GetRapporti());
	config.Write("SysData_K7", checksum);

	config.Flush();
}

bool TabbyApp::CaricaDatiRegistro()
{	// ATTENZIONE: fare molta attenzione alla differenza tra ReadLong e ReadLongLong
	wxConfig config("Tabby", "MonkeyDigit");

	// Se non ci sono soldi salvati, assumiamo sia una nuova partita
	if (!config.HasEntry("soldi")) return false;

	TabbyGuy& guy = m_game.GetTabbyGuy();

	// 1. DATI BASE TABBYGUY
	long long soldi = 0;
	config.Read("soldi", &soldi); // Legge long long direttamente
	guy.SetSoldi(soldi);

	guy.SetFama((int)config.ReadLong("fama", 0));
	guy.SetPaghetta((int)config.ReadLong("paghetta", 0));
	guy.SetRep((int)config.ReadLong("reputazione", 0));
	guy.SetRapporti((int)config.ReadLong("rapporti", 0));
	guy.SetStudio((int)config.ReadLong("studio", 0));
	guy.SetSizze((int)config.ReadLong("sizze", 0));
	guy.SetPelle((Pelle)config.ReadLong("pelle", 0));
	guy.SetFortuna((int)config.ReadLong("fortuna", 0));

	// 2. CARTA D'IDENTITA'
	// Nota: Assumo tu abbia accesso in scrittura all'ID, altrimenti devi creare un oggetto CartaIdentita temp e fare SetID
	CartaIdentita id{};
	wxString s; // Variabile temporanea per leggere le stringhe

	config.Read("nome", &s); id.m_nome = s.ToStdString();
	config.Read("cognome", &s); id.m_cognome = s.ToStdString();

	int g = config.ReadLong("giorno_nascita", 1);
	int m = config.ReadLong("mese_nascita", 1);
	int a = config.ReadLong("anno_nascita", 0);
	id.m_dataNascita = Chrono::Date(a, m, g);

	config.Read("atto_nascita", &s); id.m_attoNascita = s.ToStdString();
	config.Read("luogo_nascita", &s); id.m_luogoNascita = s.ToStdString();
	config.Read("provincia_nascita", &s); id.m_provinciaNascita = s.ToStdString();
	config.Read("cittadinanza", &s); id.m_cittadinanza = s.ToStdString();
	config.Read("residenza", &s); id.m_residenza = s.ToStdString();
	config.Read("indirizzo", &s); id.m_indirizzo = s.ToStdString();
	config.Read("stato_civile", &s); id.m_statoCivile = s.ToStdString();
	config.Read("professione", &s); id.m_professione = s.ToStdString();

	// 3. VESTITI
	// Lambda helper per non ripetere codice 3 volte
	auto LeggiVestito = [&](const wxString& suffix) -> Vestito {
		TipoVestito tipo = (TipoVestito)config.ReadLong("tipo_" + suffix, 0);
		wxString nome;
		wxString desc;
		wxString img;
		config.Read("nome_" + suffix, &nome);
		config.Read("desc_" + suffix, &desc);
		config.Read("img_" + suffix, &img);
		long long prezzo = config.ReadLongLong("prezzo_" + suffix, 0);
		int fama = (int)config.ReadLong("fama_" + suffix, 0);
		
		return Vestito{ tipo, nome.ToStdString(), desc.ToStdString(), img.ToStdString(), prezzo, fama};
		};

	guy.SetGiubbotto(LeggiVestito("giubbotto"));
	guy.SetPantaloni(LeggiVestito("pantaloni"));
	guy.SetScarpe(LeggiVestito("scarpe"));

	// 4. CARRIERA
	Carriera carr;
	config.Read("nome_ditta", &s); carr.SetDitta(s.ToStdString());
	carr.SetStipendio(config.ReadLongLong("stipendio", 0));
	carr.SetImpegno((int)config.ReadLong("impegno", 0));
	carr.SetGiorniLavorati((int)config.ReadLong("giorni_lavorati", 0));
	guy.GetCarriera() = carr;

	// 5. SCUOLA (Lettura Ciclica)
	Scuola scuola;
	scuola.m_materie.clear();
	int numMaterie = (int) config.ReadLong("scuola_num_materie", 0);

	// 2. CICLO FOR DETERMINISTICO
	for (int i = 0; i < numMaterie; i++)
	{
		wxString keyMateria = wxString::Format("materia_%d", (int)i);
		wxString keyVoto = wxString::Format("voto_%d", (int)i);

		// Controllo di sicurezza: se per qualche motivo assurdo manca la chiave, saltiamo o fermiamo
		if (!config.HasEntry(keyMateria)) continue;

		wxString nomeMat;
		config.Read(keyMateria, &nomeMat);
		int votoMat = (int)config.ReadLong(keyVoto, 0);

		scuola.m_materie.push_back(Materia{ nomeMat.ToStdString(), votoMat });
	}
	guy.GetScuola() = scuola;

	// 6. SCOOTER
	Scooter scoot;
	config.Read("nome_scooter", &s); scoot.SetNome(s.ToStdString());
	config.Read("img_scooter", &s); scoot.SetImageStr(s.ToStdString());
	scoot.SetPrezzo(config.ReadLongLong("prezzo_scooter", 0));
	scoot.SetFama((int)config.ReadLong("fama_scooter", 0));
	scoot.SetCapSerbatoio((float)config.ReadDouble("serbatoio_scooter", 0.0));
	scoot.SetAttivita((Attivita)config.ReadLong("attivita_scooter", 0));

	// Helper per i pezzi dello scooter
	auto LeggiPezzo = [&](const wxString& suffix) -> Pezzo {
		TipoPezzo tipo = (TipoPezzo)config.ReadLong("tipo_" + suffix, 0);
		wxString nome;
		wxString img;
		config.Read("nome_" + suffix, &nome);
		config.Read("img_" + suffix, &img);
		long long prezzo = config.ReadLongLong("prezzo_" + suffix, 0);
		int power = (int)config.ReadLong("power_" + suffix, 0);
		return Pezzo{ tipo, nome.ToStdString(), img.ToStdString(), prezzo, power };
		};

	scoot.InstallaPezzo(LeggiPezzo("marmitta"));
	scoot.InstallaPezzo(LeggiPezzo("filtro"));
	scoot.InstallaPezzo(LeggiPezzo("carburatore"));
	scoot.InstallaPezzo(LeggiPezzo("cilindrata"));
	scoot.CalcolaVelocita();

	// Equipaggiamo lo scooter
	guy.GetScooter() = scoot;

	// 7. TIPA
	Tipa tipa;
	config.Read("nome_tipa", &s); tipa.SetNome(s.ToStdString());
	tipa.SetFama((int)config.ReadLong("fama_tipa", 0));
	guy.SetTipa(tipa);

	// 8. TELEFONO
	Telefono tel;
	config.Read("nome_tel", &s); tel.SetNome(s.ToStdString());
	config.Read("desc_tel", &s); tel.SetDesc(s.ToStdString());
	config.Read("img_tel", &s); tel.SetImageStr(s.ToStdString());
	tel.SetPrezzo(config.ReadLongLong("prezzo_tel", 0));
	tel.SetFama((int)config.ReadLong("fama_tel", 0));
	tel.SetStato((int)config.ReadLong("stato_tel", 0));
	tel.SetCredito(config.ReadLongLong("credito_tel", 0));

	// ABBONAMENTO
	Abbonamento abb;
	config.Read("nome_abb", &s); abb.SetNome(s.ToStdString());
	config.Read("img_abb", &s); abb.SetImageStr(s.ToStdString());
	abb.SetCostoAttivazione(config.ReadLongLong("attivazione_abb", 0));

	// Ricariche (Lettura Ciclica)
	int numRicariche = (int)config.ReadLong("ricariche_count", 0);

	// 2. CICLO FOR PULITO
	for (long i = 0; i < numRicariche; i++)
	{
		wxString keyRic = wxString::Format("ricarica_%d", (int)i);

		if (config.HasEntry(keyRic))
		{
			long long valRic = config.ReadLongLong(keyRic, 0);
			abb.GetRicariche().push_back(valRic);
		}
	}

	tel.SetAbbonamento(abb);
	guy.SetTelefono(tel);

	// 9. ALTRI DATI DI GIOCO (m_game)
	int dg = config.ReadLong("data_giorno", 1);
	int dm = config.ReadLong("data_mese", 1);
	int da = config.ReadLong("data_anno", 0);
	m_game.GetDate() = Chrono::Date{ da, dm, dg };

	int sp_g = config.ReadLong("scadenza_pal_giorno", 1);
	int sp_m = config.ReadLong("scadenza_pal_mese", 1);
	int sp_a = config.ReadLong("scadenza_pal_anno", 0);
	m_game.GetScadenzaPal() = Chrono::Date{ sp_a, sp_m, sp_g };

	m_game.SetValutaCorrente((Valuta)config.ReadLong("valuta_corrente", 0));
	m_game.SetCoolDownPestaggio((int)config.ReadLong("cooldown_pestaggio", 0));
	m_game.SetCoolDownPelle((int)config.ReadLong("cooldown_pelle", 0));
	m_game.SetPaloCount((int)config.ReadLong("palo_count", 0));
	m_game.SetAttesa((int)config.ReadLong("attesa", 0));
	m_game.SetTipoGiorno((TipoGiorno)config.ReadLong("tipo_giorno", 0));
	m_game.SetSoundActive(config.ReadBool("sound_active", true));

	// 10. ANTI-CHEAT
	long long checksumSalvato = 0;
	config.Read("SysData_K7", &checksumSalvato);

	long long checksumCalc = m_game.CalcolaChecksum(
		guy.GetSoldi(), guy.GetRep(), guy.GetFama(), guy.GetStudio(), guy.GetRapporti()
	);

	if (checksumSalvato != checksumCalc)
	{
		wxMessageBox("Rilevata manomissione dei dati di salvataggio.\nCosa fai, birichino... Adesso, per punizione, la partità verrà resettata, brutto bastardo :)", "ERRORE DI SISTEMA", wxOK | wxICON_ERROR);
		m_game.ResetPartita(); // Assicurati di avere questa funzione
		return false;
	}
	return true;
}

// Macro che crea il vero main() di Windows
wxIMPLEMENT_APP(TabbyApp);

// ----------------------------------------------------------------------------
// TABBY FRAME (Solo Grafica)
// ----------------------------------------------------------------------------
// Costruttore finestra
TabbyFrame::TabbyFrame(TabbyGame& game)
	: wxFrame{ NULL, wxID_ANY, "Tabby Window", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~(wxRESIZE_BORDER | wxMAXIMIZE_BOX)}, // Blocca ridimensionamento (sono operatori bitwise su delle flag binarie)
	m_game{game}
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

			},  item->GetId());
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
	mainSizer->Add(new wxStaticLine(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL), 0, wxEXPAND);

	// BODY ---------------------------------------------------------------------------------------------
	wxBoxSizer* sizerBody = new wxBoxSizer{ wxHORIZONTAL };

	// FOTO
	wxPanel* pnlFoto = new wxPanel{ this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN };
	wxBoxSizer* sizerFoto = new wxBoxSizer{ wxVERTICAL };
	wxBitmap bmpAvatar = GeneraAvatar(m_game.GetTabbyGuy());
	m_fotoTabbozzo = new wxStaticBitmap{ pnlFoto, wxID_ANY, bmpAvatar };
	//m_fotoTabbozzo->SetWindowStyle(wxSS_NOTIFY | wxBORDER_SUNKEN);
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
	wxBoxSizer* sizerStats = new wxBoxSizer{wxVERTICAL};
	
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
	gridStats->Add(btnTipa, wxGBPosition(0, 2), wxDefaultSpan, wxALL | wxALIGN_RIGHT, 7);
	gridStats->Add(btnCompagnia, wxGBPosition(2, 2), wxDefaultSpan, wxALL | wxALIGN_RIGHT, 7);
	gridStats->Add(btnFamiglia, wxGBPosition(3, 2), wxDefaultSpan, wxALL | wxALIGN_RIGHT, 7);
	
	// Soldi
	wxBoxSizer* sizerSoldiRow = new wxBoxSizer{wxHORIZONTAL};
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
	m_lblFigo->SetLabel(wxString::Format("Figosità %d/100", guy.GetFama()));
	m_barFigo->SetValue(guy.GetFama());
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
	}
	// Se l'utente ha premuto OK, lasciamo che wxWidgets distrugga la finestra
	event.Skip();
}