#ifndef KONTOPREMIUM_H
#define KONTOPREMIUM_H

#include "KontoBankowe.h"

class KontoPremium : public KontoBankowe {
public:
    void wplata(double kwota) override {
        _saldo += kwota;
        if (kwota >= 1000) _saldo += 20; // bonus
    }
    const char* typKonta() const override { return "Premium"; }
};

#endif
