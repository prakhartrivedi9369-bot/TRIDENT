// login.h
#ifndef LOGIN_H
#define LOGIN_H

#include "crow.h" // Crow framework include karna zaroori hai

// Ye function handle karega login ki POST request ko
void handleLogin(const crow::request& req,crow::response& res);

enum class LogEvent
{
    USER_SIGNUP,
    USER_LOGIN,
    USER_LOGOUT,

    OTP_SEND,
    OTP_VERIFY,

    BUY_ORDER,
    SELL_ORDER,
    ORDER_CANCEL,

    PORTFOLIO_VIEW,

    SECURITY,

    DATABASE,

    SYSTEM
};

enum class SecurityStatus
{
    Allowed,

    INVALID_OTP,

    OTP_EXPIRED,

    EMAIL_BLOCKED,

    IP_BLOCKED,

    ATTEMPT_LIMIT_EXCEEDED,

    TOO_MANY_REQUESTS,

    SUSPICIOUS_ACTIVITY,

    REDIS_ERROR,

    DATABASE_ERROR
};

enum class LogLevel
{
    INFO,

    WARNING,

    ERROR,

    CRITICAL
};

enum class SecrityAction
{
    
}

#endif
