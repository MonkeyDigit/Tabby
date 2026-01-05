#pragma once
#include <string>
#include "Scooter.h"
#include "Tipa.h"
#include "Chrono.h"
#include "Scuola.h"
#include "Telefonino.h"
#include "Lavoro.h"

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
	void SetImpegno(int impegno);

	// MODIFICHE
	// Restituisce true se l'acquisto avviene con successo
	bool SpendiSoldi(long long importo);
	void GuadagnaSoldi(long long importo);
	void IncPaghetta(long long importo);
	void ResetSoldiDelta() { m_soldiDelta = 0; };	// E' solo un dato informativo...
	void CalcolaStudio();
	void SetFortuna(int fortuna);
	void IncSizze(int numSizze);
	void DecSizze(int numSizze);
	void IncRep(int punti);
	void DecRep(int punti);
	void IncFama(int punti);
	void DecFama(int punti);
	void IncRapporti(int punti);
	void DecRapporti(int punti);
	void IncImpegno(int punti);
	void DecImpegno(int punti);
	void IncGiorniLavoro(int giorni);	// I giorni possono solo aumentare
	void ResetGiorniLavoro();
	void ResetLavoro();

	long long GetSoldi() const { return m_soldi; };
	long long GetSoldiDelta() const { return m_soldiDelta; };
	int GetRep() const { return m_rep; };
	int GetFama() const { return m_fama; };
	int GetStudio() const { return m_studio; };
	int GetFortuna() const { return m_fortuna; };
	int GetRapporti() const { return m_rapporti; };
	int GetPaghetta() const { return m_paghetta; };
	int GetGiubotto() const { return m_giubotto; };
	int GetPantaloni() const { return m_pantaloni; };
	int GetScarpe() const { return m_scarpe; };
	int GetSizze() const { return m_sizze; };
	int GetImpegno() const { return m_impegno; };
	int GetGiorniLavoro() const { return m_giorniLavoro; };
	bool isAssunto() const { return m_assunto; };
	long GetStipendio() const { return m_stipendio; };
	Ditta& GetDitta() { return m_ditta; };
	Chrono::Date& GetScadenzaGym() { return m_scadenzaGym; };
	CartaIdentita& GetID() { return m_identita; };
	Scuola& GetScuola() { return m_scuola; };
	Scooter& GetScooter() { return m_scooter; };
	Tipa& GetTipa() { return m_tipa; };
	Telefono& GetTelefono() { return m_telefono; };
	OperatoreTel& GetOperatore() { return m_operatore; };


private:
	CartaIdentita m_identita;
	long long m_soldi;			// intero a 64 bit (max 9 miliardi DI miliardi): il tabbozzo è rikko...
	long long m_soldiDelta;		// Guadagno o perdita
	int m_rep;
	int m_fama;
	int m_studio;
	int m_fortuna;
	Ditta m_ditta;
	int m_impegno;
	int m_rapporti;
	int m_paghetta;
	int m_testa;	// abbronzatura
	int m_giubotto;
	int m_pantaloni;
	int m_scarpe;
	int m_sizze;	// sigarette
	// TODO: POSSIBILE CLASSE LAVORO/CARRIERA???
	int m_numDitta;
	int m_giorniLavoro;
	long m_stipendio;
	bool m_assunto;
	Chrono::Date m_scadenzaGym;
	// Qua potevo usare dei puntatori inizializzando lo scooter e la tipa a NULL, ma non voglio fare un puttanaio con destructor e magagne dei puntatori
	Scuola m_scuola;
	Scooter m_scooter;
	Tipa m_tipa;
	Telefono m_telefono;
	OperatoreTel m_operatore;
};