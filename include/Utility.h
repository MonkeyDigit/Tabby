#pragma once
#include <string>
#include <vector>

// Funzione per caricare una risorsa incorporata (RCDATA) e trasformarla in uno stream
std::stringstream CaricaRisorsa(const char* nomeRisorsa);

// Funzione helper per la formattazione dei soldi
std::string formattaConPunti(const long long numero);

// Funzione Helper: Cerca 'tag' dentro 'testo' e lo sostituisce con 'valore'
std::string sostituisci(std::string testo, const std::string& tag, const std::string& valore);

// Funzione helper locale per rimuovere spazi o caratteri di a capo residui (es. \r su Windows)
void trimString(std::string& s);

std::vector<std::string> splitString(const std::string& s, char delimiter);

// Helper per convertire stringa in enum/int senza impazzire con le eccezioni
int parseInt(const std::string& s);
long long parseLong(const std::string& s);