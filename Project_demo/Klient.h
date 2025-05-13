#ifndef KLIENT_H
#define KLIENT_H

#include <string>
#include "KontoBankowe.h"

struct Klient {
    int id;
    std::string imie;
    std::string nazwisko;
    KontoBankowe* konto;

    Klient(int i, std::string im, std::string naz, KontoBankowe* k)
        : id(i), imie(im), nazwisko(naz), konto(k) {}
    ~Klient() { delete konto; }
};

#endif
