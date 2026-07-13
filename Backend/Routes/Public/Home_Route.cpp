#include "routes.h"
#include "crow.h"
#include "Paths.h"

using namespace std;

void register_Home_Routes(crow::SimpleApp& app)
{
     //Home Route
     CROW_ROUTE(app,"/")([]()
     {
         ifstream file(Paths::HTML + "index.html");

         if(!file.is_open()) return crow::response(404);

         stringstream buffer;
         buffer << file.rdbuf();
         
         return crow::response(buffer.str());
     });
}
