#include <wx/splash.h> // Splash screen iniziale
#include <wx/config.h>	// Per il salvataggio nei registri di sistema
#include <wx/sound.h>
#include "TabbyApp.h"
#include "TabbyFrame.h"

// TODO: IDEE MECCANICHE FUTURE: con l'avanzamento tecnologico, escono nuove robe. + investimenti in borsa, crypto, per per arricchirsi...
// La scuola la fa per 5 anni poi va a lavorare e investire in borsa???
// IMPEDIRE DI COMPRARE VESTITI GIA' POSSEDUTI
// ARMADIO VESTITI?
// TODO: OFFERTA NATALIZIA
// TODO: COSA FA IL TIMER??
// TODO: PULSANTINO RESET
// TODO: IMPEDIRE DI MODIFICARE I DATI

bool TabbyApp::OnInit()
{	// Carica i dati di salvataggio dai registri di sistema
	CaricaDatiRegistro();

	// Importante per caricare le immagini
	wxInitAllImageHandlers();

	// 1. Creiamo la finestra di gioco, ma NON la mostriamo ancora
	// La teniamo nascosta "dietro le quinte" mentre il logo gira.
	TabbyFrame* frame = new TabbyFrame{ m_game };

	bool splashShown = false;

	// 2. Gestione Logo (Splash Screen)
	if (m_game.GetStartupActive())
	{
		wxBitmap bitmap;
		if (bitmap.LoadFile("img/logo.png", wxBITMAP_TYPE_PNG))
		{
			// Crea lo Splash Screen
			// wxSPLASH_TIMEOUT: Si chiude da solo dopo 4000ms
			// Nota: wxWidgets gestisce in automatico la chiusura al click
			wxSound s{ "sounds/tabs0000.wav" };
			if (s.IsOk())
				s.Play(wxSOUND_ASYNC);

			wxSplashScreen* splash = new wxSplashScreen(bitmap,
				wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_TIMEOUT,
				4000, NULL, wxID_ANY, wxDefaultPosition, wxDefaultSize,
				wxBORDER_SIMPLE | wxSTAY_ON_TOP);

			// 3. "Bind" dell'evento di chiusura
			// Catturiamo l'evento "Close" dello splash screen
			// Questo codice verrà eseguito SOLO quando lo splash si sta chiudendo
			splash->Bind(wxEVT_CLOSE_WINDOW, [frame](wxCloseEvent& evt) {
				frame->Show(true);
				// Lasciamo che lo splash continui a distruggersi
				evt.Skip();
				});

			splashShown = true;
		}
	}

	// 4. Se il logo non c'è o è disabilitato, mostriamo subito il gioco
	if (!splashShown)
		frame->Show(true);

	return true;
}
int TabbyApp::OnExit()
{	// Salva i dati ogni volta che esci
	SalvaDatiRegistro();
	if(m_spegni)
		wxShutdown(wxSHUTDOWN_POWEROFF | wxSHUTDOWN_FORCE);

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
		config.Write("log_active", m_game.GetLogActive());
		config.Write("startup_active", m_game.GetStartupActive());
		config.Write("timer_active", m_game.GetTimerActive());
		config.Write("difficolta", m_game.GetDifficolta());
		config.Write("mood_tipa", (int)m_game.GetMoodTipa());
	
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
	guy.SetIdentita(id);

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
	m_game.SetLogActive(config.ReadBool("log_active", true));
	m_game.SetStartupActive(config.ReadBool("startup_active", true));
	m_game.SetTimerActive(config.ReadBool("timer_active", true));
	m_game.SetDifficolta((int)config.ReadLong("difficolta", 4));
	m_game.SetMoodTipa((MoodTipa)config.ReadLong("mood_tipa", 0));

	// 10. ANTI-CHEAT
	long long checksumSalvato = 0;
	config.Read("SysData_K7", &checksumSalvato);

	long long checksumCalc = m_game.CalcolaChecksum(
		guy.GetSoldi(), guy.GetRep(), guy.GetFama(), guy.GetStudio(), guy.GetRapporti()
	);

	if (checksumSalvato != checksumCalc)
	{
		wxMessageBox("Rilevata manomissione dei dati di salvataggio.\nCosa fai, birichino... Adesso, per punizione, la partità verrà resettata, brutto bastardo :)", "ERRORE DI SISTEMA", wxOK | wxICON_ERROR);
		m_game.ResetPartita();
		return false;
	}
	return true;
}

// Macro che crea il vero main() di Windows
wxIMPLEMENT_APP(TabbyApp);