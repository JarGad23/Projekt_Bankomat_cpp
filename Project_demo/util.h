#ifndef UTIL_H
#define UTIL_H

#include <iostream>

inline void wypiszNaglowek(const std::string& tytul) {
    std::cout << "=======================================\n";
    std::cout << "     " << tytul << "\n";
    std::cout << "=======================================\n";
}

inline void czyscEkran() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

inline void pauza() {
    std::cout << "\nWciœnij Enter aby kontynuowaæ...";
    std::cin.ignore();
    std::cin.get();
}

inline void ustawPolskieZnaki() {
#ifdef _WIN32
    system("chcp 65001 > nul");
#endif
}

#endif
