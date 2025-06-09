#ifndef KONTOOSZCZEDNOSCIOWE_H
#define KONTOOSZCZEDNOSCIOWE_H

#include "KontoBankowe.h"

class KontoOszczednosciowe : public KontoBankowe {
private: 
    int wyp³atyWykoñczone = 0;
	const int limitWyp³at = 3;
	const double prowizja = 2.0;

public:
	bool wyplata(double kwota) override {
		if (wyp³atyWykoñczone < limitWyp³at) {
			bool ok = KontoBankowe::wyplata(kwota);
			if (ok) {
				wyp³atyWykoñczone++;
				std::cout << "Wyp³ata wykonana (" << wyp³atyWykoñczone << "/" << limitWyp³at << ").\n";
			}
			return ok;
		}
		else {
			double razem = kwota + prowizja;
			bool ok = KontoBankowe::wyplata(razem);
			if (ok) {
				wyp³atyWykoñczone++;
				std::cout << "Wyp³ata z prowizj¹ " << prowizja << " PLN (po limicie)\n";
			}
			return ok;
		}
	}


    const char* typKonta() const override { return "Oszczednosciowe"; }
};

#endif
