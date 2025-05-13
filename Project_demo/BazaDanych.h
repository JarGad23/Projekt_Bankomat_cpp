#ifndef BAZADANYCH_H
#define BAZADANYCH_H

#include <string>
#include "Klient.h"

class BazaDanych {
public:
    bool polacz();
    Klient* zaloguj(int id, const std::string& pin);
    void zapiszTransakcje(int klientId, const std::string& typ, double kwota);
    void pokazHistorie(int klientId);
};

#endif
