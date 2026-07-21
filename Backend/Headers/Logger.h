// login.h
#ifndef LOGIN_H
#define LOGIN_H

#include "crow.h" // Crow framework include karna zaroori hai

enum class LogEntry
{
     ATTEMPT,

     DETAILS_MISSING,

     LOGIN_SUCCESS,

     SIGNUP_SUCCESS,

     USER_NOT_FOUND,

     USER_ALREADY_EXIST,

     INVALID_PASSWORD,

     DB_CONNECTION_ISSUE,

     SERVER_ERROR,

     ATTEMPT_LIMIT_EXCEED
};

enum class Attempt_limit
{
     EMAIL_ATTEMPT,

     IP_ATTEMPT,

     ATTEMPT_NUMBER
};

int Attempt_check(const string &email,const string &IP,const string &password);
void increment_attempt(const string &email,const string &IP,const string &password);
void block_user(const string &email,const string &IP);
void reset_attempt(const string &email,const string &IP);

#endif
