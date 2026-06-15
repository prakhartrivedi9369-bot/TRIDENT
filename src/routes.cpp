#include "../Headers/routes.h"
#include "../Headers/login.h"
#include "../Headers/database.h"
#include "../Headers/signup.h"
#include "crow.h"

using namespace std;

void registerRoutes(crow::SimpleApp& app)
{
     //Home Route
     CROW_ROUTE(app,"/")([]()
     {
         ifstream file("../frontend/HTML/index.html");

         if(!file.is_open()) return crow::response(404);

         stringstream buffer;
         buffer << file.rdbuf();
         
         return crow::response(buffer.str());
     });

     //Login Route
     CROW_ROUTE(app, "/login")([]()
     {
        ifstream file("../frontend/HTML/login.html");

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
           ifstream file("../frontend/CSS/login.css");

           stringstream buffer;
           buffer << file.rdbuf();

           crow::response res(buffer.str());
           res.set_header("Content-Type", "text/css");

           return res;
     });
     CROW_ROUTE(app, "/login.js")([]()
     {
           ifstream file("../frontend/Script/login.js");

           stringstream buffer;
           buffer << file.rdbuf();

           crow::response res(buffer.str());
           res.set_header("Content-Type", "text/js");

           return res;
     });
     CROW_ROUTE(app, "/otp")([]()
     {
           ifstream file("../frontend/HTML/otp.html");

           if(!file.is_open()) return crow::response(404);

           stringstream buffer;
           buffer << file.rdbuf();

           return crow::response (buffer.str());
     });
     
     //Signup Route
     CROW_ROUTE(app, "/signup")([]()
     {
        ifstream file("../frontend/HTML/signup.html");

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