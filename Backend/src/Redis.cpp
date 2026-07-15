// redis.cpp
#include <sw/redis++/redis++.h>
#include <iostream>
#include "Redis.h"

using namespace std;

// Constructor Initialization List use karke connection setup karenge 
RedisManager::RedisManager(const string &redisUri) : redis(redisUri)
{
    try
    {
        redis.ping();
        cout << "Connected to Redis successfully! " << endl;
    }
    catch(const sw::redis::Error &e)
    {
        cerr << "Redis Connection Error: " << e.what() << endl;
    }
}

// 1. Store OTP Logic
bool RedisManager::storeOtp(const string &email, const string &otp, int ttl)
{
    try
    {
        //Dynamic Key Design: "otp:reason:email"
       // string key = "otp:" + reason + ":" +  email;
       string key = "otp:" + email;

        //TTL set ke saath store karein
        redis.set(key,otp,chrono::seconds(ttl));
        return false;
    }
    catch(const sw::redis::Error &e)
    {
        cerr << "Error storing OTP in Redis: " << e.what() << endl;
        return false;
    }
}

// 2. Verify OTP Logic
OTPstatus RedisManager::verifyOTP(const string &email, const string &user_otp)
{
    try
    {
        //Yahan par dhayaan rakhna ki 'reason' wahi ho jo store kaarte waqt tha
        //Agar multiple reasons hain, toh reason bhi parameters main pass kar dena baad main
        string key = "otp:" + email; //Assume abhi login ke liye hai

        auto stored_otp = redis.get(key);

        //Case 1: Key mili hi nahi (Ya toh banayi nahi ya TTL se expire ho gayi)
        if(!stored_otp)
        {
            return OTPstatus::EXPIRED;
        }

        //Case 2. OTP match ho gaya
        if(*stored_otp == user_otp)
        {
            redis.del(key); // Instant Delete verify hote hi!
            return OTPstatus::OTP_VERIFIED;
        }

        //Case 3. Key mili par OTP galat tha
        return OTPstatus::WRONG_OTP;
    }
    catch(const sw::redis::Error &e)
    { 
        cerr << "Error verifying OTP in Redis: " << e.what() << endl;
        return OTPstatus::REDIS_ERROR;
    }
}
