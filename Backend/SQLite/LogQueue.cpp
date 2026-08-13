#include "crow.h"
#include "AuditLogs.h"
#include <queue>
#include "LogQueue.h"
#include "database.h"
#include <chrono>

using namespace std;

LogQueue::LogQueue(Table& table) : table(table)
{;}

void LogQueue::push(const AuditLog& log)
{
    {
        std::lock_guard<std::mutex> lock(mutex);

        queue.push(log);
    }

    cv.notify_one();
}

AuditLog LogQueue::pop()
{
    std::unique_lock<std::mutex> lock(mutex);

    cv.wait(lock, [this] {
        return !queue.empty() || !running;
    });

    if (!running && queue.empty())
        return {};

    AuditLog log = std::move(queue.front());
    queue.pop();

    return log;
}

void LogQueue::processLogs()
{
    while (running)
    {
        AuditLog log = pop();

        if (!running)
            break;

        while (running && !saveLogInDB(log))
        {
            std::this_thread::sleep_for(
                std::chrono::seconds(2)
            );
        }
    }
}

void LogQueue::stop()
{
    running = false;
    cv.notify_all();
}