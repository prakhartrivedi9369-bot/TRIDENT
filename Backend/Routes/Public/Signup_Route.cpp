#include "routes.h"
#include "signup.h"
#include "crow.h"
#include "Paths.h"
#include "database.h"

using namespace std;

void register_Signup_Routes(crow::SimpleApp& app)
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
     CROW_ROUTE(app, "/api/signup").methods(crow::HTTPMethod::POST)([](const crow::request& req)
     {
             crow::response res;
             handle_signup(req,res);
             return res;
     });
}