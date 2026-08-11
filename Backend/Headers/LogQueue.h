#pragma once

#include "crow.h"
#include "AuditLogs.h"
#include <bits/stdc++.h>

using namespace std;

class LogQueue
{
    private:
        std::queue<AuditLog> queue;
        std::mutex mutex;
        std::condition_variable cv;

        std::atomic<bool> running{true};

    public:
        void push(const AuditLog& log);
        AuditLog pop();

        void processLogs();

        void stop();
};