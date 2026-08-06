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

     Table auditTable("../SQLite/Data/audit_logs.db");

     auditTable.initialize();

     AuditLog testLog;

     testLog.event = AuditEvent::LOGIN;
     testLog.email = "test@gmail.com";
     testLog.ip_address = "127.0.0.1";
     testLog.status = AuditStatus::SUCCESS;
     testLog.timestamp = "2026-08-06 11:30:00";

     if(auditTable.insert(testLog))
     {
        std::cout << "Test audit log inserted successfully."
                  << std::endl;
     }
     else
     {
        std::cerr << "Failed to insert test audit log."
                  << std::endl;
     }

     crow::SimpleApp app;

     registerRoutes(app,RedisManager);
     
     //Server start
     app.port(18080).multithreaded().run();

     closeSQLite();

     // Server band hone par safely cleanup karo
     cleanup_database();
     return 0;
}