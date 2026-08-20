#include "routes.h"
#include "crow.h"
#include "Redis.h"
#include "AuditLogger.h"

using namespace std;

void registerRoutes(crow::SimpleApp& app, RedisManager& RedisManager,AuditLogger& AuditLogger)
{
    register_Home_Routes(app);
    register_Login_Routes(app,RedisManager,AuditLogger);
    register_Signup_Routes(app,AuditLogger);
    register_Otp_Routes(app,RedisManager,AuditLogger);
    register_forget_Routes(app,RedisManager,AuditLogger);
    register_Front_Routes(app);
    register_JWT_check_Routes(app,RedisManager,AuditLogger& AuditLogger);
}