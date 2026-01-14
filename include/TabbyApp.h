#pragma once
#include <wx/wx.h>
#include "TabbyGame.h"

// ---------- APPLICAZIONE ------------
// il public fa in modo che la classe che stiame creando erediti i membri e le funzioni pubbliche di wxApp
class TabbyApp : public wxApp {
public:
	// il prefisso virtual indica che questa funzione esistente nella classe base, viene ridefinita nella classe figlia
	// override è semplicemente un meccanismo per controllare di aver ridefinito la funzione giusta, in caso contrario il compilatore si incazza
	// questa funzione viene chiamata quando viene creata la finestra
	virtual bool OnInit() override;
	virtual int OnExit() override;

	void SetSpegnimentoRichiesto(bool spegni) { m_spegni = spegni; }
private:
	TabbyGame m_game;
	bool m_spegni = false;
	void SalvaDatiRegistro();
	bool CaricaDatiRegistro();
};

// Importante: questa macro permette di usare wxGetApp() (per lo spegnimento del pc)
DECLARE_APP(TabbyApp)