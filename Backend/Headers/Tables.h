#pragma once

#include <sqlite3.h>
#include <string>
#include "AuditLogs.h"

class Table
{
    private:
        sqlite3* db;

    public:
        Table(const std::string& dbPath);
       ~Table();

        bool initialize();
        bool insert(const AuditLog& log);
        bool markAsSynced(int id);
};