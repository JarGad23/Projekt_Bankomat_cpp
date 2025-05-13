#include <iostream>
#include <locale.h>
#include "BazaDanych.h"
#include "util.h"

int main() {
	setlocale(LC_ALL, "Polish");
    BazaDanych db;

    if (!db.polacz()) {
        std::cerr << "Nie można połączyć z bazą danych!\n";
        return 1;
    }

    while (true) {
        czyscEkran();
        wypiszNaglowek("SYMULATOR BANKOMATU");

        int id;
        std::string pin;
        std::cout << "Podaj ID: ";
        std::cin >> id;
        std::cout << "Podaj PIN: ";
        std::cin >> pin;

        Klient* klient = db.zaloguj(id, pin);
        if (!klient) {
            std::cout << "Błąd logowania.\n";
            pauza(); continue;
        }

        while (true) {
            czyscEkran();
            std::cout << "Zalogowano jako: " << klient->imie << " " << klient->nazwisko << "\n";
            std::cout << "Typ konta: " << klient->konto->typKonta() << "\n";
            std::cout << "Saldo: " << klient->konto->saldo() << " PLN\n\n";
            std::cout << "[1] Wpłata\n[2] Wypłata\n[3] Historia\n[4] Wyloguj\n> ";

            int wybor;
            std::cin >> wybor;
            if (wybor == 1) {
                double kwota;
                std::cout << "Kwota wpłaty: "; std::cin >> kwota;
                klient->konto->wplata(kwota);
                db.zapiszTransakcje(id, "Wpłata", kwota);
            }
            else if (wybor == 2) {
                double kwota;
                std::cout << "Kwota wypłaty: "; std::cin >> kwota;
                if (klient->konto->wyplata(kwota))
                    db.zapiszTransakcje(id, "Wypłata", kwota);
                else std::cout << "Brak środków.\n";
                pauza();
            }
            else if (wybor == 3) {
                db.pokazHistorie(id);
                pauza();
            }
            else break;
        }
        delete klient;
    }
    return 0;
}
