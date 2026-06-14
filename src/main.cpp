#include "crow.h"
#include "../Headers/routes.h"
#include "../Headers/database.h"
#include <iostream>
using namespace std;

void handlelogin(const crow::request& req, crow::response& res);

int main()
{
     //Server chalane se phele databse connect karo
     init_database();  
     crow::SimpleApp app;

     registerRoutes(app);
     
     //Server start
     app.port(18080).multithreaded().run();

     // Server band hone par safely cleanup karo
     cleanup_database();
     return 0;
}