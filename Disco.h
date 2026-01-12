#pragma once
#include <string>
#include "Chrono.h"

struct Disco {
    Disco() : m_nome{}, m_descrizione{}, m_giornoChiuso{ Chrono::WeekDay::monday }, m_fuoriPorta{ false }, m_prezzoIngresso{ 0 }, m_reqFama{ 0 }, m_incFama{ 0 }, m_incRep{ 0 }
    {}
    std::string m_nome;
    std::string m_descrizione;
    Chrono::WeekDay m_giornoChiuso;
    bool m_fuoriPorta;                  // Se true, serve lo scooter
    int m_prezzoIngresso;
    int m_reqFama;
    int m_incFama;
    int m_incRep;
};