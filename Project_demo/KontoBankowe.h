#ifndef KONTOBANKOWE_H
#define KONTOBANKOWE_H

class KontoBankowe {
protected:
    double _saldo;
public:
    KontoBankowe() : _saldo(0.0) {}
    virtual void wplata(double kwota) { _saldo += kwota; }
    virtual bool wyplata(double kwota) {
        if (kwota <= _saldo) { _saldo -= kwota; return true; }
        return false;
    }
    virtual double saldo() const { return _saldo; }
    virtual void ustawSaldo(double s) { _saldo = s; }
    virtual const char* typKonta() const = 0;
    virtual ~KontoBankowe() = default;
};

#endif
