#include "routes.h"
#include "database.h"
#include "crow.h"
#include "Paths.h"

using namespace std;

void handleLogin(const crow::request& req,crow::response& res);

void register_Login_Routes(crow::SimpleApp& app)
{
     //Login Route
     CROW_ROUTE(app, "/login")([]()
     {
        ifstream file(Paths::HTML + "login.html");

         if(!file.is_open()) return crow::response(404);

         stringstream buffer;
         buffer << file.rdbuf();
         
         return crow::response(buffer.str());
     });
     CROW_ROUTE(app, "/api/login").methods(crow::HTTPMethod::POST)([](const crow::request& req)
     {
            crow::response res;
            handleLogin(req,res);
            return res;
     });
     CROW_ROUTE(app, "/login.css")([]()
     {
           ifstream file(Paths::CSS + "login.css");

           stringstream buffer;
           buffer << file.rdbuf();

           crow::response res(buffer.str());
           res.set_header("Content-Type", "text/css");

           return res;
     });
     CROW_ROUTE(app, "/login.js")([]()
     {
           ifstream file(Paths::JS+ "login.js");

           stringstream buffer;
           buffer << file.rdbuf();

           crow::response res(buffer.str());
           res.set_header("Content-Type", "text/js");

           return res;
     });
}