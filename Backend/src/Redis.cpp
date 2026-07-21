#include <sw/redis++/redis++.h>
#include <iostream>
#include "Redis.h"
#include "Logger.h"
#include "database.h"

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
        throw;  //object banne hi nahi dega agar Redis unavailable hai.
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
        return true;
    }
    catch(const sw::redis::Error &e)
    {
        cerr << "Error storing OTP in Redis: " << e.what() << endl;
        return false;
    }
}

// 2. Verify OTP Logic
OTPstatus RedisManager::verifyOtp(const string &email, const string &user_otp)
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
int Attempt_check(const string& email,const string& IP,const string &password)
{
    //Variable for Keys
    string email_key = "email_key:" + email;
    string IP_key = "IP_key:" + IP;
     
    //Value fetched from redis
    auto stored_email_attempt = redis.get(email_key);
    auto stored_ip_attempt = redis.get(IP_key);

    //User are not stored in Redis
    if(!stored_email_attempt || !stored_ip_attempt)
    {
       return increment_attempt(email,IP,password);
    }

    int email_attempt = stoi(*stored_email_attempt);
    int ip_attempt = stoi(*stored_email_attempt);

    //Attempt limit exceed
    if(email_attempt==3 || ip_attempt==3)
    {
       block_user(email,ip);
       return false;
    }

    return increment_attempt(email,IP,password);
}
void increment_attempt(const string &email,const string &IP,const string &password)
{
    int status = verifyCredentialsInDB(email,password);
    //Variable for Keys
    string email_key = "email_key:" + email;
    string IP_key = "IP_key:" + IP;

    if(status == 1)
    {
        
    }
    else if(status == 0 || status ==2)
    {
        auto E_attempts = redis.incr(email_key);

        if(E_attempts==1)
        {
            redis.expire(email_key,chrono::seconds(300));
        }

        auto IP_attempts = redis.incr(IP_key);

        if(IP_attempt==1)
        {
            redis.expire(IP_key,chrono::seconds(300));
        }
    }
}
