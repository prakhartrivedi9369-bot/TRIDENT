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

     //SQLite3 initialization
     initSQLite();

     Table auditTable("../SQLite/Data/audit_logs.db");

     auditTable.initialize();

     AuditLogger AuditLogger(auditTable);

     LogQueue logQueue;

     std::thread worker(&LogQueue::process,&logQueue);

     AuditLogger auditLogger(logQueue);

     crow::SimpleApp app;

     registerRoutes(app,RedisManager,AuditLogger);
     
     //Server start
     app.port(18080).multithreaded().run();

     closeSQLite();

     // Server band hone par safely cleanup karo
     cleanup_database();
     return 0;
}