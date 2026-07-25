#include "routes.h"
#include "crow.h"
#include "Paths.h"

using namespace std;

void handle_forget(const crow::request& req,crow::response& res);

void register_forget_Routes(crow::SimpleApp& app)
{
     //Forget Route
     CROW_ROUTE(app, "/forget")([]()
     {
        ifstream file(Paths::HTML + "forget.html");

         if(!file.is_open()) return crow::response(404);

         stringstream buffer;
         buffer << file.rdbuf();
         
         return crow::response(buffer.str());
     });
     CROW_ROUTE(app, "/api/forget").methods(crow::HTTPMethod::POST)([](const crow::request& req)
     {
             crow::response res;
             handle_forget(req,res);
             return res;
     });
     CROW_ROUTE(app, "/Pass_reset")([]()
     {
             ifstream file(Paths::HTML + "Password_Reset.html");

             if(!file.is_open()) return crow::response(404);

             stringstream buffer;
             buffer << file.rdbuf();

             return crow::response(buffer.str());
     });
}