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

          bool storeOtp(
            const string &email,
            const string &otp,
            int ttl
          );

          OTPstatus verifyOtp(
            const string &email,
            const string &enteredOTP
          );

          bool deleteOTP(
            const string &email
          );

          int Attempt_check(
            const string &email,
            const string &IP,
            const string &password
          );

          int increment_attempt(
            const string &email,
            const string &IP,
            const string &password
          );

          bool block_user(
            const string &email,
            const string &IP
          );

          void reset_attempt(
            const string &email,
            const string &IP
          );
};
