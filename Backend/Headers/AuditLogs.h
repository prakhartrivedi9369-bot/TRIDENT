#pragma once

#include "crow.h"
#include <string>

using namespace std;

enum class AuditEvent
{
    LOGIN_SUCCESS,
    LOGIN_FAILED,

    SIGNUP_SUCCESS,
    SIGNUP_FAILED,

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

struct AuditLog
{
    AuditEvent event;
    AuditStatus status;

    std::string email;
    std::string ip_address;
    std::string timestamp;
};

std::string toString(AuditEvent event);
std::string toString(AuditStatus status);