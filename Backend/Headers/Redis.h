#pragma once

#include "crow.h"
#include <sw/redis++/redis++.h>
#include <string>
#include <optional>

using namespace std;

enum class OTPstatus
{
    OTP_VERIFIED,
    WRONG_OTP,
    EXPIRED,
    REDIS_ERROR,
    FORGET_PASSWORD
};

struct VerifyOtpResult
{
    bool success;
    optional<string> usecase;
    optional<string> JWT_token;
    string message;
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
            int ttl,
            const string &usecase
          );

          VerifyOtpResult verifyOtp(
            const string &email,
            const string &enteredOTP,
            RedisManager& RedisManager
          );

          bool deleteOTP(
            const string &email
          );

          int Attempt_check(
            const string &email,
            const string &IP,
            const string &password,
            RedisManager &RedisManager
          );

          int increment_attempt(
            const string &email,
            const string &IP,
            const string &password
          );

          bool block_user(
            const string &email,
            const string &IP,
            RedisManager &RedisManager
          );

          void reset_attempt(
            const string &email,
            const string &IP
          );

          string Reset_token_check(
            const string& reset_token,
            RedisManager& RedisManager
          );
};
