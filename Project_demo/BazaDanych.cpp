#include "BazaDanych.h"
#include "sqlite/sqlite3.h"
#include <iostream>
#include "KontoPremium.h"
#include "KontoOszczednosciowe.h"
#include "util.h"

sqlite3* db;

bool BazaDanych::polacz() {
    return sqlite3_open("bankomat.db", &db) == SQLITE_OK;
}

Klient* BazaDanych::zaloguj(int id, const std::string& pin) {
    std::string sql = "SELECT imie, nazwisko, typ_konta, saldo FROM klienci WHERE id = ? AND pin = ?";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return nullptr;

    sqlite3_bind_int(stmt, 1, id);
    sqlite3_bind_text(stmt, 2, pin.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string imie = (const char*)sqlite3_column_text(stmt, 0);
        std::string nazwisko = (const char*)sqlite3_column_text(stmt, 1);
        std::string typ = (const char*)sqlite3_column_text(stmt, 2);
        double saldo = sqlite3_column_double(stmt, 3);
        sqlite3_finalize(stmt);
        
        KontoBankowe* konto = (typ == "Premium") ? (KontoBankowe*)new KontoPremium()
            : (KontoBankowe*)new KontoOszczednosciowe();
        konto->ustawSaldo(saldo);
        return new Klient(id, imie, nazwisko, konto);
    }

    sqlite3_finalize(stmt);
    return nullptr;
}

void BazaDanych::zapiszTransakcje(int klientId, const std::string& typ, double kwota) {
    std::string sql = "INSERT INTO transakcje(id_klienta, typ, kwota, data) VALUES (?, ?, ?, datetime('now'))";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, klientId);
    sqlite3_bind_text(stmt, 2, typ.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 3, kwota);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
}

void BazaDanych::pokazHistorie(int klientId) {
    std::string sql = "SELECT typ, kwota, data FROM transakcje WHERE id_klienta = ?";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_int(stmt, 1, klientId);
    std::cout << "\nHistoria transakcji:\n";
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string typ = (const char*)sqlite3_column_text(stmt, 0);
        double kwota = sqlite3_column_double(stmt, 1);
        std::string data = (const char*)sqlite3_column_text(stmt, 2);
        std::cout << "- " << typ << ": " << kwota << " PLN, " << data << "\n";
    }
    sqlite3_finalize(stmt);
}

void BazaDanych::aktualizujSaldo(int klientId, double noweSaldo) {
	std::string sql = "UPDATE klienci SET saldo = ? WHERE id = ?";
	sqlite3_stmt* stmt;
	sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
	sqlite3_bind_double(stmt, 1, noweSaldo);
	sqlite3_bind_int(stmt, 2, klientId);
	sqlite3_step(stmt);
	sqlite3_finalize(stmt);
}

void BazaDanych::dodajNowegoKlienta() {
    std::string pin, imie, nazwisko;
    int typWybor;
    std::string typKonta;

    std::cout << "\n=== Tworzenie nowego konta ===\n";

    while (true) {
        std::cout << "Ustaw PIN (4 cyfry): ";
        std::cin >> pin;
        if (pin.length() == 4 && pin.find_first_not_of("0123456789") == std::string::npos)
            break;
        std::cout << "B³¹d: PIN musi mieæ dok³adnie 4 cyfry.\n";
    }

    std::cout << "Podaj imiê: ";
    std::cin >> imie;

    std::cout << "Podaj nazwisko: ";
    std::cin >> nazwisko;

    std::cout << "\nWybierz typ konta:\n";
    std::cout << "[1] Konto Premium – bonusy przy wp³atach\n";
    std::cout << "[2] Konto Oszczêdnoœciowe – limit darmowych wyp³at\n";
    std::cout << "[3] Konto Zwyk³e – brak dodatkowych funkcji\n";
    std::cout << "Twój wybór: ";
    std::cin >> typWybor;

    switch (typWybor) {
    case 1: typKonta = "Premium"; break;
    case 2: typKonta = "Oszczednosciowe"; break;
    case 3: typKonta = "Zwykle"; break;
    default: std::cout << "B³êdny wybór. Przerywam tworzenie konta.\n"; return;
    }

    std::string sql = "INSERT INTO klienci (imie, nazwisko, pin, typ_konta, saldo) VALUES (?, ?, ?, ?, 0)";
    sqlite3_stmt* stmt;
    sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    sqlite3_bind_text(stmt, 1, imie.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, nazwisko.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, pin.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, typKonta.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        int assignedId = (int)sqlite3_last_insert_rowid(db);
        std::cout << "Konto utworzone pomyœlnie!\n";
        std::cout << "Twój numer ID to: " << assignedId << " - zapamiêtaj ten numer do logowania" << "\n";
    }
    else {
        std::cout << "B³¹d przy tworzeniu konta.\n";
    }
    sqlite3_finalize(stmt);
    pauza();
}