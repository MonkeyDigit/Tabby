#pragma once
#include <string>
#include <vector>
#include <iomanip>              // Per formattare l'orario
#include <iostream>             // Per cerr (errori)
#include <sstream>
#include <Windows.h>

// Funzione per caricare una risorsa incorporata (RCDATA) e trasformarla in uno stream
static std::stringstream CaricaRisorsa(const char* nomeRisorsa)
{
    // 1. Cerca la risorsa nell'eseguibile (RT_RCDATA è il tipo standard per dati grezzi)
    // Castiamo (LPCSTR) per dire al compilatore che è un ID numerico, non una stringa vera
    HRSRC hRes = FindResourceA(NULL, nomeRisorsa, (LPCSTR)RT_RCDATA);
    if (!hRes) {
        // Se non la trova, ritorna uno stream vuoto (o potresti loggare l'errore)
        return std::stringstream("");
    }

    // 2. Carica la risorsa in memoria
    HGLOBAL hData = LoadResource(NULL, hRes);
    if (!hData) return std::stringstream("");

    // 3. Ottieni il puntatore ai dati e la dimensione
    const char* data = (const char*)LockResource(hData);
    DWORD size = SizeofResource(NULL, hRes);

    // 4. Crea una stringa con i dati e poi uno stream
    // Nota: costruiamo la stringa specificando la size, così gestisce anche eventuali caratteri nulli
    return std::stringstream(std::string(data, size));
}

// Funzione helper per la formattazione dei soldi
static std::string formattaConPunti(const long long numero)
{
    // Convertiamo in stringa grezza (es. "1000000")
    std::string s = std::to_string(numero);

    // Calcoliamo dove inserire i punti
    // Partiamo dalla fine, saltiamo 3 cifre, e mettiamo un punto.
    // Dobbiamo stare attenti a non mettere il punto se siamo all'inizio (es. non ".100")

    int len = s.length();
    int count = 0;

    // Partiamo dal fondo verso l'inizio
    for (int i = len - 1; i > 0; i--)
    {
        // Se il carattere è un numero (evitiamo il segno '-')
        if (isdigit(s[i])) {
            count++;

            // Ogni 3 cifre, se non siamo all'inizio e il carattere prima non è un '-', inseriamo il punto
            if (count == 3 && i > 0 && s[i - 1] != '-')
            {
                s.insert(i, "."); // Inserisce il punto nella posizione i
                count = 0;    // Resetta il count
            }
        }
    }

    return s;
}

// Funzione Helper: Cerca 'tag' dentro 'testo' e lo sostituisce con 'valore'
static std::string sostituisci(std::string testo, const std::string& tag, const std::string& valore)
{
    size_t pos = testo.find(tag);
    if (pos != std::string::npos) {
        testo.replace(pos, tag.length(), valore);
    }
    return testo; // Ritorna la stringa modificata
}

// Funzione helper locale per rimuovere spazi o caratteri di a capo residui (es. \r su Windows)
static void trimString(std::string& s) {
    while (!s.empty() && (s.back() == '\r' || s.back() == '\n' || s.back() == ' ')) {
        s.pop_back();
    }
}

static std::vector<std::string> splitString(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        // Rimuovi eventuali spazi bianchi iniziali/finali
        tokens.push_back(token);
    }
    return tokens;
}

// Helper per convertire stringa in enum/int senza impazzire con le eccezioni
static int parseInt(const std::string& s) {
    try { return std::stoi(s); }
    catch (...) { return 0; }
}
static long long parseLong(const std::string& s) {
    try { return std::stoll(s); }
    catch (...) { return 0; }
}