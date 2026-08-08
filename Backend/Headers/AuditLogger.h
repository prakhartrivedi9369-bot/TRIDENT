#pragma once

#include "crow.h"
#include <string>
#include "AuditLogger.h"
#include "Tables.h"

using namespace std;

class AuditLogger
{
    private:
        Table& table;

    public:
        AuditLogger(Table& table);

        void logLoginSuccess(const string& email,
                         const string& ip);

        void logLoginFailure(const string& email,
                         const string& ip);

  //      void logSignupSuccess(...);

    //    void logSignupFailure(...);

   //     void logOtpSent(...);

   //     void logOtpVerified(...);

  //      void logPasswordReset(...);
};

string getCurrentTimestamp();