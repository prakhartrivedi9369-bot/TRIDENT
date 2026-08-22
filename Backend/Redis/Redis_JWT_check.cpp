#include <sw/redis++/redis++.h>
#include <iostream>
#include "Redis.h"
#include "Logger.h"
#include "JWT_token.h"

using namespace std;

bool RedisManager::verify_JWT_token(const string& Recieved_JWT_token)
{
     string hashed = sha256_hash(Recieved_JWT_token);

     string JWT_token_key = "JWT_token:" + hashed;

     auto result = redis.get(JWT_token_key);

     if(result)
     {
          // Key mil gayi!
          return true;
     }
     else
     {
          // Key nahi mili,kuch to gadbad hai!
          return false;
     }
     return false;
}