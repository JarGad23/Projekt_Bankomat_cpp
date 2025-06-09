#ifndef KONTOPREMIUM_H
#define KONTOPREMIUM_H

#include "KontoBankowe.h"

class KontoPremium : public KontoBankowe {
public:
    void wplata(double kwota) override {
        double bonus = 0.0;
        if (kwota >= 1000.0) {
            bonus += 20.0;
            bonus += kwota * 0.01;
        }
		_saldo += kwota + bonus;
		std::cout << "Bonus Premium: " << bonus << " PLN\n";
    }
    const char* typKonta() const override { return "Premium"; }
};

#endif
