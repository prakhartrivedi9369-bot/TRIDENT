#pragma once

#include "crow.h"
#include <string>

using namespace std;

enum class AuditEvent
{
    OTP_SENT,
    OTP_VERIFIED,
    OTP_FAILED,

    PASSWORD_RESET,

    RATE_LIMIT_BLOCK,

    UNKNOWN,

    LOGIN,
    SIGNUP
};

enum class AuditStatus
{
    SUCCESS,
    FAILED,
    BLOCKED,
    EXPIRED,
    UNKNOWN
};

enum class AuditReason
{
    LOGIN_SUCCESS,
    INVALID_PASSWORD,
    DATABASE_CONNECTIVITY_ISSUE,
    REDIS_ERROR,
    SERVER_ERROR
};

struct AuditLog
{
    AuditEvent event;
    AuditStatus status;
    AuditReason Reason;

    std::string email;
    std::string ip_address;
    std::string timestamp;
};

string toString(AuditEvent event);
string toString(AuditStatus status);
string toString(AuditReason Reason);