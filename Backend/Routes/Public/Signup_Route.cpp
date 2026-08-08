#include "routes.h"
#include "crow.h"
#include "Paths.h"
#include "database.h"
#include "AuditLogger.h"

using namespace std;

void handle_signup(const crow::request& req,crow::response& res,AuditLogger &AuditLogger);

void register_Signup_Routes(crow::SimpleApp& app,AuditLogger &AuditLogger)
{
     //Signup Route
     CROW_ROUTE(app, "/signup")([]()
     {
        ifstream file(Paths::HTML + "signup.html");

         if(!file.is_open()) return crow::response(404);

         stringstream buffer;
         buffer << file.rdbuf();
         
         return crow::response(buffer.str());
     });
     CROW_ROUTE(app, "/api/signup").methods(crow::HTTPMethod::POST)([&AuditLogger](const crow::request& req)
     {
             crow::response res;
             handle_signup(req,res,AuditLogger);
             return res;
     });
}