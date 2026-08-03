#include "routes.h"
#include "crow.h"
#include "Paths.h"
#include <iostream>

using namespace std;

void register_Front_Routes(crow::SimpleApp& app)
{
    //Front Route
     CROW_ROUTE(app,"/Front_page")([]()
     {
        ifstream file(Paths::HTML + "Front.html");

         if(!file.is_open()) return crow::response(404);

         stringstream buffer;
         buffer << file.rdbuf();
         
         return crow::response(buffer.str());
     });
}