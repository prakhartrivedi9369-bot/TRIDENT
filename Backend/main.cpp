#include "crow.h"
#include "routes.h"
#include "database.h"
#include "env_config.h"
#include "crypto_utils.h"
#include "Redis.h"

using namespace std;

void Redis();

int main()
{
     loadEnv("../.env");

     if(!CryptoUtils::init())
     {
          cerr << "Failed to initialize libsodium\n";
     }

     //Server chalane se phele databse connect karos
     init_database();  

     crow::SimpleApp app;

     registerRoutes(app);
     
     //Server start
     app.port(18080).multithreaded().run();

     // Server band hone par safely cleanup karo
     cleanup_database();
     return 0;
}