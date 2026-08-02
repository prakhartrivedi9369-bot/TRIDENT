#include "routes.h"
#include "crow.h"
#include "Redis.h"

using namespace std;

void registerRoutes(crow::SimpleApp& app, RedisManager& RedisManager)
{
    register_Home_Routes(app);
    register_Login_Routes(app,RedisManager);
    register_Signup_Routes(app);
    register_Otp_Routes(app,RedisManager);
    register_forget_Routes(app,RedisManager);
}