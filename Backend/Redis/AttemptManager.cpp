#include <sw/redis++/redis++.h>
#include <iostream>
#include "Redis.h"
#include "Logger.h"
#include "database.h"
#include "crypto_utils.h"
#include <sodium.h>
#include <sstream>
#include <iomanip>

using namespace std;

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