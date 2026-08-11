#include "crow.h"
#include "routes.h"
#include "database.h"
#include "env_config.h"
#include "crypto_utils.h"
#include "Redis.h"
#include "SQLite.h"
#include "Tables.h"
#include "AuditLogs.h"
#include "AuditLogger.h"
#include "LogQueue.h"

using namespace std;

int main()
{
     // .env load
     loadEnv("../.env");

     // Redis initialization
     RedisManager RedisManager(getEnvValue("REDIS_URI"));

     //Argon2id initialization
     CryptoUtils::init();

     //MongoDB initialization
     init_database();  

     //SQLite initialization
     initSQLite();

     Table auditTable("../SQLite/Data/audit_logs.db");
     auditTable.initialize();

     LogQueue logQueue;

     AuditLogger auditLogger(auditTable, logQueue);

     std::thread worker(&LogQueue::processLogs, &logQueue);

     crow::SimpleApp app;

     registerRoutes(app,RedisManager,auditLogger);
     
     //Server start
     app.port(18080).multithreaded().run();

     logQueue.stop();
     worker.join();

     closeSQLite();

     // Server band hone par safely cleanup karo
     cleanup_database();
     return 0;
}