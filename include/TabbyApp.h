#pragma once
#include <wx/wx.h>
#include "TabbyGame.h"
#include "TabbyFrame.h"

// ---------- APPLICAZIONE ------------
// il public fa in modo che la classe che stiame creando erediti i membri e le funzioni pubbliche di wxApp
class TabbyApp : public wxApp {
public:
	// il prefisso virtual indica che questa funzione esistente nella classe base, viene ridefinita nella classe figlia
	// override è semplicemente un meccanismo per controllare di aver ridefinito la funzione giusta, in caso contrario il compilatore si incazza
	// questa funzione viene chiamata quando viene creata la finestra
	virtual bool OnInit() override;
	virtual int OnExit() override;

private:
	TabbyGame m_game;
	void SalvaDatiRegistro();
	bool CaricaDatiRegistro();
};