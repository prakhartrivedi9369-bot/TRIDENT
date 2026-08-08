#include "Tables.h"
#include <iostream>

using namespace std;

Table::Table(const std::string& dbPath)
    : db(nullptr)
{
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK)
    {
        std::cerr << "Failed to open SQLite database: "
                  << sqlite3_errmsg(db)
                  << std::endl;

        sqlite3_close(db);
        db = nullptr;
    }
}

Table::~Table()
{
    if (db)
    {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool Table::initialize()
{
    if (db == nullptr)
    {
        std::cerr << "Database is not open." << std::endl;
        return false;
    } 

    const char* sql =R"(
                        CREATE TABLE IF NOT EXISTS audit_logs
                        (
                            id INTEGER PRIMARY KEY AUTOINCREMENT,
                            event TEXT NOT NULL,
                            email TEXT,
                            ip_address TEXT,
                            status TEXT,
                            Reason TEXT,
                            timestamp TEXT
                        );
                    )";

    char* errorMessage = nullptr;

    const int result = sqlite3_exec(
        db,
        sql,
        nullptr,
        nullptr,
        &errorMessage
    );

    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to create audit_logs table: "
                  << (errorMessage != nullptr
                          ? errorMessage
                          : sqlite3_errmsg(db))
                  << std::endl;

        if (errorMessage != nullptr)
        {
            sqlite3_free(errorMessage);
        }

        return false;
    }

    std::cout << "audit_logs table initialized successfully."
              << std::endl;

    return true;
}

bool Table::insert(const AuditLog& log)
{
    if (db == nullptr)
    {
        std::cerr << "Cannot insert audit log: database is not open."
                  << std::endl;
        return false;
    }

    const char* sql = R"(
        INSERT INTO audit_logs
        (
            event,
            email,
            ip_address,
            status,
            Reason,
            timestamp
        )
        VALUES (?, ?, ?, ?, ?);
    )";

    sqlite3_stmt* statement = nullptr;

    int result = sqlite3_prepare_v2(
        db,
        sql,
        -1,
        &statement,
        nullptr
    );

    if (result != SQLITE_OK)
    {
        std::cerr << "Failed to prepare audit log statement: "
                  << sqlite3_errmsg(db)
                  << std::endl;

        return false;
    }

    const string eventString = toString(log.event);
    const string statusString = toString(log.status);
    const string ReasonString = toString(log.Reason);

    sqlite3_bind_text(
        statement,
        1,
        eventString.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        statement,
        2,
        log.email.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        statement,
        3,
        log.ip_address.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        statement,
        4,
        statusString.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        statement,
        5,
        ReasonString.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    sqlite3_bind_text(
        statement,
        6,
        log.timestamp.c_str(),
        -1,
        SQLITE_TRANSIENT
    );

    result = sqlite3_step(statement);

    if (result != SQLITE_DONE)
    {
        std::cerr << "Failed to insert audit log: "
                  << sqlite3_errmsg(db)
                  << std::endl;

        sqlite3_finalize(statement);
        return false;
    }

    sqlite3_finalize(statement);

    return true;
}