#pragma once
#include <string>
#include "Scooter.h"
#include "Tipa.h"
#include "Chrono.h"
#include "Scuola.h"
#include "Telefonino.h"
#include "Carriera.h"
#include "Acquistabile.h"

enum class Pelle {
	ABBR_NO,
	ABBR_LIEVE,
	ABBR_MEDIA,
	ABBR_PESANTE,
	ABBR_CARBONIZZATO
};

struct CartaIdentita {
	std::string m_nome;
	std::string m_cognome;
	Chrono::Date m_dataNascita;
	std::string m_attoNascita;
	std::string m_luogoNascita;
	std::string m_provinciaNascita;
	std::string m_cittadinanza;
	std::string m_residenza;
	std::string m_indirizzo;
	std::string m_statoCivile;
	std::string m_professione;
};

class TabbyGuy {
public:
	TabbyGuy();		// Constructor

	// SETTER
	void SetRapporti(int rapporti);

	// MODIFICHE
	// SOLDI
	// Restituisce true se l'acquisto avviene con successo
	bool SpendiSoldi(long long importo);
	void GuadagnaSoldi(long long importo);
	void IncPaghetta(long long importo);
	void ResetSoldiDelta() { m_soldiDelta = 0; };	// è solo un dato informativo...
	// SCUOLA
	void CalcolaStudio();
	// FORTUNA
	void SetFortuna(int fortuna);
	// SIGARETTE
	void IncSizze(int sizze);
	void DecSizze(int sizze);
	// REPUTAZIONE
	void IncRep(int punti);
	void DecRep(int punti);
	// FIGOSITA'
	void IncFama(int punti);
	void DecFama(int punti);
	// RAPPORTO TIPA
	void IncRapporti(int punti);
	void DecRapporti(int punti);
	void SetTipa(const Tipa& tipa) { m_tipa = tipa; };
	void LasciaTipa();
	// LAVORO
	void Licenziati();
	// VESTITI
	bool SetGiubbotto(const Vestito& giubbotto);	// Restituisce true se il prodotto è effettivamente un giubbotto
	bool SetPantaloni(const Vestito& pantaloni);
	bool SetScarpe(const Vestito& scarpe);
	// ABBRONZATURA
	void Abbronza();
	void Sbianca();

	// TODO: RIORDINA TUTTA STA MERDA
	long long GetSoldi() const { return m_soldi; };
	long long GetSoldiDelta() const { return m_soldiDelta; };
	int GetRep() const { return m_rep; };
	int GetFama() const { return m_fama; };
	int GetStudio() const { return m_studio; };
	int GetFortuna() const { return m_fortuna; };
	int GetRapporti() const { return m_rapporti; };
	int GetPaghetta() const { return m_paghetta; };
	int GetSizze() const { return m_sizze; };
	Pelle GetPelle() const { return m_pelle; };
	std::string GetPelleStr();

	// PASS-BY REFERENCE
	Carriera& GetCarriera() { return m_carriera; };
	CartaIdentita& GetID() { return m_identita; };
	Scuola& GetScuola() { return m_scuola; };
	Scooter& GetScooter() { return m_scooter; };
	Tipa& GetTipa() { return m_tipa; };
	Telefono& GetTelefono() { return m_telefono; };
	Vestito& GetGiubotto() { return m_giubotto; };
	Vestito& GetPantaloni() { return m_pantaloni; };
	Vestito& GetScarpe() { return m_scarpe; };

	// Funzione helper
	bool HaUnLavoro() const { return !m_carriera.IsDisoccupato(); }
	bool HaTipa() const { return !m_tipa.GetNome().empty(); };
	bool HaScooter() const { return !m_scooter.GetNome().empty(); };
	bool HaTelefono() const { return !m_telefono.GetNome().empty(); };

private:
	int m_rapporti;
	long long m_soldi;			// intero a 64 bit (max 9 miliardi DI miliardi): il tabbozzo è rikko...
	long long m_soldiDelta;		// Guadagno o perdita
	int m_paghetta;
	int m_rep;
	int m_fama;
	int m_studio;
	int m_fortuna;
	Pelle m_pelle;				// Abbronzatura
	int m_sizze;				// sigarette

	// Qua potevo usare dei puntatori inizializzando lo scooter e la tipa a NULL, ma non voglio fare un puttanaio con destructor e magagne dei puntatori
	CartaIdentita m_identita;
	Vestito m_giubotto;
	Vestito m_pantaloni;
	Vestito m_scarpe;
	Carriera m_carriera;
	Scuola m_scuola;
	Scooter m_scooter;
	Tipa m_tipa;
	Telefono m_telefono;
};