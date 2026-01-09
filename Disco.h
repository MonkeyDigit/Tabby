#pragma once
#include <string>
#include "Chrono.h"

struct Disco {
    std::string m_nome;
    std::string m_descrizione;
    Chrono::WeekDay m_giornoChiuso;
    bool m_fuoriPorta;                  // Se true, serve lo scooter
    int m_prezzoIngresso;
    int m_reqFama;
    int m_incFama;
    int m_incRep;
};