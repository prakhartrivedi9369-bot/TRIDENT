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

     RedisManager redisManager(getEnvValue("REDIS_URI"));

     CryptoUtils::init();

     init_database();  

     crow::SimpleApp app;

     registerRoutes(app);
     
     //Server start
     app.port(18080).multithreaded().run();

     // Server band hone par safely cleanup karo
     cleanup_database();
     return 0;
}