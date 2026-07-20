// login.h
#ifndef LOGIN_H
#define LOGIN_H

#include "crow.h" // Crow framework include karna zaroori hai

enum class LogEntry
{
     ATTEMPT,

     DETAILS_MISSING,

     LOGIN_SUCCESS,

     USER_NOT_FOUND,

     INVALID_PASSWORD,

     DB_CONNECTION_ISSUE,

     SERVER_ERROR
};


#endif
