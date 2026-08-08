#include "SQLite.h"
#include "Tables.h"
#include "AuditLogs.h"
#include "crow.h"
#include "AuditLogger.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
    
using namespace std;

string getCurrentTimestamp()
{
    auto now = std::chrono::system_clock::now();

    std::time_t currentTime =
        std::chrono::system_clock::to_time_t(now);

    std::tm localTime{};

    localtime_r(&currentTime, &localTime);

    std::ostringstream timestamp;

    timestamp << std::put_time(
        &localTime,
        "%Y-%m-%d %H:%M:%S"
    );

    return timestamp.str();
}

AuditLogger::AuditLogger(Table& table) : table(table)
{;}

void AuditLogger::logLoginSuccess(const string& email,const string& ip)
{
    AuditLog log;

    log.event = AuditEvent::LOGIN;
    log.email = email;
    log.ip_address = ip;
    log.status = AuditStatus::SUCCESS;
    log.reason = AuditReason::LOGIN_SUCCESS;
    log.timestamp = getCurrentTimestamp();

    table.insert(log);
}

void AuditLogger::logLoginFailure(const string& email,const string& ip)
{
    AuditLog log;

    log.event = AuditEvent::LOGIN;
    log.email = email;
    log.ip_address = ip;
    log.status = AuditStatus::FAILED;
    log.status = AuditReason::INVALID_PASSWORD;
    log.timestamp = getCurrentTimestamp();

    table.insert(log);
}
