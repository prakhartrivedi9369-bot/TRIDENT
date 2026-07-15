#pragma once

#include "crow.h"
#include <sw/redis++/redis++.h>
#include <string>

using namespace std;

enum class OTPstatus
{
    OTP_VERIFIED,
    WRONG_OTP,
    EXPIRED,
    REDIS_ERROR
};

class RedisManager
{
     private:
         sw::redis::Redis redis;

     public:
          RedisManager(const string &redisUri);

          bool connect();

          bool storeOtp(
            const string &email,
            const string &otp,
            int ttl
          );

          OTPstatus verifyOTP(
            const string &email,
            const string &enteredOTP
          );

          bool deleteOTP(
            const string &email
          );
};
