#pragma once

#include "crow.h"
#include "Redis.h"

void registerRoutes(crow::SimpleApp& app, RedisManager& RedisManager);
void register_Home_Routes(crow::SimpleApp& app);
void register_Login_Routes(crow::SimpleApp& app, RedisManager &RedisManager);
void register_Signup_Routes(crow::SimpleApp& app);
void register_Otp_Routes(crow::SimpleApp& app,RedisManager& RedisManager);
void register_forget_Routes(crow::SimpleApp& app);