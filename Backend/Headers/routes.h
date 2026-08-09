#pragma once

#include "crow.h"
#include "Redis.h"
#include "AuditLogger.h"

void registerRoutes(crow::SimpleApp& app, RedisManager& RedisManager,AuditLogger& AuditLogger);

//Public Routes
void register_Home_Routes(crow::SimpleApp& app);
void register_Login_Routes(crow::SimpleApp& app, RedisManager &RedisManager,AuditLogger& AuditLogger);
void register_Signup_Routes(crow::SimpleApp& app,AuditLogger &AuditLogger);
void register_Otp_Routes(crow::SimpleApp& app,RedisManager& RedisManager,AuditLogger &AuditLogger);
void register_forget_Routes(crow::SimpleApp& app,RedisManager &RedisManager,AuditLogger &AuditLogger);


//Protected Routes
void register_Front_Routes(crow::SimpleApp& app);