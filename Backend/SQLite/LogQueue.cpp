#include "crow.h"
#include "AuditLogs.h"
#include <queue>
#include "LogsQueue.h"
#include "database.h"

using namespace std;

void LogQueue::push(const AuditLog& log)
{
    {
        std::lock_guard<std::mutex> lock(mutex);

        queue.push(log);
    }

    cv.notify_one();
}

LogData LogQueue::pop()
{
    std::unique_lock<std::mutex> lock(queueMutex);

    cv.wait(lock, [this] {
        return !queue.empty() || !running;
    });

    if (!running && queue.empty())
        return {};

    LogData log = std::move(queue.front());
    queue.pop();

    return log;
}

void LogQueue::process()
{
    while (running)
    {
        LogData log = pop();

        // MongoDB mein bhejo
        saveLogInDB(log);
    }
}