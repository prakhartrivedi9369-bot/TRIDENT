#pragma once

#include "crow.h"
#include "AuditLogs.h"
#include <bits/stdc++.h>
#include "Tables.h"

using namespace std;

class LogQueue
{
    private:
        std::queue<AuditLog> queue;
        std::mutex mutex;
        std::condition_variable cv;

        std::atomic<bool> running{true};

        Table& table;

    public:
        void push(const AuditLog& log);
        AuditLog pop();

        LogQueue(Table& table);

        void processLogs();

        void stop();
};