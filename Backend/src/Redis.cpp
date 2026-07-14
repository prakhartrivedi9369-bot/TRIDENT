#include <sw/redis++/redis++.h>
#include <iostream>
#include "Redis.h"

using namespace std;

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

