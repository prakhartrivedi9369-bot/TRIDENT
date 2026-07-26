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
bool RedisManager::storeOtp(const string &email, const string &otp, int ttl, const string &usecase)
{
    try
    {
        //Dynamic Key Design: "otp:reason:email"
        // string key = "otp:" + reason + ":" +  email;

        //TTL set ke saath store karein
        redis.set("otp:" + email,otp,chrono::seconds(ttl));
        redis.set("usecase:" + email,usecase,chrono::seconds(ttl));
        return true;
    }
    catch(const sw::redis::Error &e)
    {
        cerr << "Error storing OTP in Redis: " << e.what() << endl;
        return false;
    }
}

// 2. Verify OTP Logic
string RedisManager::verifyOtp(const string &email, const string &user_otp)
{
    try
    {
        //Yahan par dhayaan rakhna ki 'reason' wahi ho jo store kaarte waqt tha
        //Agar multiple reasons hain, toh reason bhi parameters main pass kar dena baad main
        string otp_key = "otp:" + email; 
        string usecase_key = "usecase:" + email;

        auto stored_otp = redis.get(otp_key);
        auto stored_usecase = redis.get(usecase_key);

        //Case 1: Key mili hi nahi (Ya toh banayi nahi ya TTL se expire ho gayi)
        if(!stored_otp || !stored_usecase)
        {
            return "EXPIRED";
        }

        //Case 2. OTP match ho gaya
        if(*stored_otp == user_otp)
        {
            redis.del(otp_key); 
            redis.del(usecase_key); // Instant Delete verify hote hi!
            return *stored_usecase;
        }

        //Case 3. Key mili par OTP galat tha
        return "WRONG_OTP";
    }
    catch(const sw::redis::Error &e)
    { 
        cerr << "Error verifying OTP in Redis: " << e.what() << endl;
        return "REDIS_ERROR";
    }
}
bool RedisManager::block_user(const string &email, const string &IP, RedisManager &RedisManager)
{
    //Variable for Keys
    string email_key = "email_key:" + email;
    string IP_key = "IP_key:" + IP;

    //Value fetched from redis
    auto stored_email_attempt = redis.get(email_key);
    auto stored_ip_attempt = redis.get(IP_key);

    if(!stored_email_attempt || !stored_ip_attempt)
    {
        return true;
    }

    int email_int = stoi(*stored_email_attempt);
    int ip_int = stoi(*stored_ip_attempt);

    if(email_int == 3 || ip_int == 3)
    {
        try
        {
           redis.setex("Blocked_email:" + email,300,"3");
           redis.setex("Blocked_ip:" + IP,300,"3");
           return true;
        }
        catch(const sw::redis::Error &e)
        {
           cerr<<"Blocking of Email/IP failed due to some unexpected error"<<e.what()<<endl;
           return false;
        }
    }
    else if(redis.exists("Blocked_email:" + email) || redis.exists("Blocked_ip:" + IP))
    {
        return false;
    }
    else
    {
         return true;
    }
    return false;
}
int RedisManager::Attempt_check(const string& email,const string& IP,const string &password,RedisManager &RedisManager)
{
    //Variable for Keys
    string email_key = "email_key:" + email;
    string IP_key = "IP_key:" + IP;

    //Value fetched from redis
    auto stored_email_attempt = redis.get(email_key);
    auto stored_ip_attempt = redis.get(IP_key);

    //User are not stored in Redis
    if(!stored_email_attempt && !stored_ip_attempt)
    {
       return increment_attempt(email,IP,password);
    }
    cout<<"fuck"<<endl;
    if(!stored_email_attempt || !stored_ip_attempt)
    {
        return 3;
    }

    int email_attempt = stoi(*stored_email_attempt);
    int ip_attempt = stoi(*stored_ip_attempt);


    //Attempt limit exceed
    if((email_attempt>=3) && (ip_attempt>=3))
    {
       cout<<"fuck Prakhar"<<endl;
       block_user(email,IP,RedisManager);
       return 3;
    }
    if((email_attempt>=3) || (ip_attempt>=3))
    {
        return 3;
    }

    return increment_attempt(email,IP,password);
}
int RedisManager::increment_attempt(const string &email,const string &IP,const string &password)
{
    int status = verifyCredentialsInDB(email,password);

        //Variable for Keys
        string email_key = "email_key:" + email;
        string IP_key = "IP_key:" + IP;

        if(status == 1)
        {
           reset_attempt(email,IP);
           return status;
        }
        else if(status ==2)
        {
           auto E_attempts = redis.incr(email_key);

           if(E_attempts==1)
           {
                redis.expire(email_key,chrono::seconds(300));
           }

           auto IP_attempts = redis.incr(IP_key);

           if(IP_attempts==1)
           {
                redis.expire(IP_key,chrono::seconds(300));
           }
           return status;
        }
        else
        {
            return status;
        }
}
void RedisManager::reset_attempt(const string &email, const string &IP)
{
    //Variable for Keys
    string email_key = "email_key:" + email;
    string IP_key = "IP_key:" + IP;

    redis.del(email_key);
    redis.del(IP_key);

    return;
}

