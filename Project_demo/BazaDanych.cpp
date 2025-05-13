#include "BazaDanych.h"
#include "sqlite/sqlite3.h"
#include <iostream>
#include "KontoPremium.h"
#include "KontoOszczednosciowe.h"

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
