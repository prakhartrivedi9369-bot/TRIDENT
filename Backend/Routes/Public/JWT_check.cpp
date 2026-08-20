#include "routes.h"
#include "crow.h"
#include "JWT_token.h"
#include "Redis.h"

using namespace std;

void register_JWT_check_Routes(crow::SimpleApp& app,RedisManager &RedisManager,AuditLogger &AuditLogger)
{
    CROW_ROUTE(app, "/JWT_check").methods("POST"_method)([&RedisManager,&AuditLogger](const crow::request& req)
    {
        string IP=req.remote_ip_address;

        auto body = crow::json::load(req.body);
        
        string cookie_header = req.get_header_value("Cookie");
        if (cookie_header.empty()) return "";

        string key = "token=";
        auto pos = cookie_header.find(key);
        if (pos == std::string::npos) return "";

        pos += key.length();
        auto end = cookie_header.find(';', pos);
        string Recieved_JWT_token = cookie_header.substr(pos, end - pos);

        verify_JWT_token(Recieved_JWT_token);
    }
}