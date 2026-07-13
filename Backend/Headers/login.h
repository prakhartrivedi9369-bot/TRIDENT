// login.h
#ifndef LOGIN_H
#define LOGIN_H

#include "crow.h" // Crow framework include karna zaroori hai

// Ye function handle karega login ki POST request ko
void handleLogin(const crow::request& req,crow::response& res);

#endif
