#include <sw/redis++/redis++.h>
#include <iostream>
#include "Redis.h"
#include "Logger.h"
#include "JWT_token.h"

using namespace std;

bool verify_JWT_token(const string& Recieved_JWT_token)
{
     string JWT_token_key = "JWT_token:" + reset_token;
}