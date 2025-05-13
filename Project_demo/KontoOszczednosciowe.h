#ifndef KONTOOSZCZEDNOSCIOWE_H
#define KONTOOSZCZEDNOSCIOWE_H

#include "KontoBankowe.h"

class KontoOszczednosciowe : public KontoBankowe {
public:
    const char* typKonta() const override { return "Oszczednosciowe"; }
};

#endif
