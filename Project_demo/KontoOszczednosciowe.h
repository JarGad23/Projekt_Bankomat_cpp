#ifndef KONTOOSZCZEDNOSCIOWE_H
#define KONTOOSZCZEDNOSCIOWE_H

#include <locale.h>
#include "KontoBankowe.h"

class KontoOszczednosciowe : public KontoBankowe {
private: 
  int wyplatyWykonczone = 0;
	const int limitWyplat = 3;
	const double prowizja = 2.0;

public:
	bool wyplata(double kwota) override {
		setlocale(LC_ALL, "Polish");
		if (wyplatyWykonczone < limitWyplat) {
			bool ok = KontoBankowe::wyplata(kwota);
			if (ok) {
				wyplatyWykonczone++;
				std::cout << "Wypłata wykonana (" << wyplatyWykonczone << "/" << limitWyplat << ").\n";
			}
			return ok;
		}
		else {
			double razem = kwota + prowizja;
			bool ok = KontoBankowe::wyplata(razem);
			if (ok) {
				wyplatyWykonczone++;
				std::cout << "Wypłata z prowizj¹ " << prowizja << " PLN (po limicie)\n";
			}
			return ok;
		}
	}


    const char* typKonta() const override { return "Oszczednosciowe"; }
};

#endif
