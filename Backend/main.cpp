#include "crow.h"
#include "routes.h"
#include "database.h"
#include "env_config.h"
#include "crypto_utils.h"
#include "Redis.h"
#include "SQLite.h"
#include "Tables.h"
#include "AuditLogs.h"

using namespace std;

int main()
{
     loadEnv("../.env");

     RedisManager RedisManager(getEnvValue("REDIS_URI"));

     CryptoUtils::init();

     init_database();  

     initSQLite();

     crow::SimpleApp app;

     registerRoutes(app,RedisManager);
     
     //Server start
     app.port(18080).multithreaded().run();

     closeSQLite();

     // Server band hone par safely cleanup karo
     cleanup_database();
     return 0;
}