#pragma once
#include <wx/wx.h>
#include <wx/statline.h>
#include "TabbyGame.h"
#define WIN_BKG wxColor(194, 196, 184)

// Forward Declaration della finestra padre
class TabbyFrame;

class DlgEvento : public wxDialog {
public:
	DlgEvento(wxWindow* parent, Messaggio& msg);
};

struct DlgMetallaro : public wxDialog {
	DlgMetallaro(wxWindow* parent, TabbyGame& game, const Messaggio& msg);
};

struct DlgManovale : public wxDialog {
public:
	DlgManovale(wxWindow* parent, TabbyGame& game, const Messaggio& msg);
};

struct DlgIncidente : public wxDialog {
public:
	DlgIncidente(wxWindow* parent, TabbyGame& game, const Messaggio& msg);
};

class DlgPagella : public wxDialog {
public:
	DlgPagella(wxWindow* parent, TabbyGame& game);
private:
	TabbyGame& m_game;
	void OnOk(wxCommandEvent& event);
};

class DlgDueDonne : public wxDialog {
public:
	DlgDueDonne(wxWindow* parent, TabbyGame& game);

private:
	TabbyGame& m_game;
	void OnEntrambe(wxCommandEvent& event);
	void OnResto(wxCommandEvent& event);
	void OnPreferisco(wxCommandEvent& event);
};

// Funzioni helper
void ManifestaEventi(wxWindow* parent, TabbyGame& game);
// Questa funzione permette alle sotto finestre di richiedere il refresh della finestra main se necessario, utile per avere un riscontro visivo immediato sullo scorrere dei giorni
void AggiornaFinestraMain();
wxBitmap CaricaAsset(const std::string& nomeFile);
wxBitmap GeneraAvatar(const TabbyGuy& guy);