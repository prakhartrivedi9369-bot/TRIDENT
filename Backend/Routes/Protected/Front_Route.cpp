#include "routes.h"
#include "crow.h"
#include "Paths.h"
#include <iostream>
#include "auth_helper.h"
#include "Redis.h"

using namespace std;

void register_Front_Routes(crow::SimpleApp& app,RedisManager &RedisManager)
{
    //Front Route
     CROW_ROUTE(app, "/Front_page").methods("GET"_method)([&RedisManager](const crow::request& req)
     {
        if (!check_authentication(req, RedisManager))
        {
            crow::response res(302);
            res.set_header("Location", "/404_Not_Found");
            return res;
        }

        // authenticated — page serve karo
        ifstream file(Paths::HTML + "Front.html");
        if(!file.is_open()) return crow::response(404);

        stringstream buffer;
        buffer << file.rdbuf();
         
        crow::response res(buffer.str());
        res.set_header("Content-Type", "text/html");
        return res;
     });  

     CROW_ROUTE(app, "/Front.css")([]()
     {
           ifstream file(Paths::CSS + "Front.css");

           stringstream buffer;
           buffer << file.rdbuf();

           crow::response res(buffer.str());
           res.set_header("Content-Type", "text/css");

           return res;
     });
     CROW_ROUTE(app, "/Front.js")([]()
     {
           ifstream file(Paths::JS+ "Front.js");

           stringstream buffer;
           buffer << file.rdbuf();

           crow::response res(buffer.str());
           res.set_header("Content-Type", "text/js");

           return res;
     });
}