#include "SQLite.h"
#include "Tables.h"
#include "AuditLogs.h"
#include "crow.h"
#include "AuditLogger.h"
    
using namespace std;

void AuditLogger(AuditEvent event,const string &email,const string &IP,Audit,const string &timestamp)
{
    Table auditTable("../SQLite/Data/audit_logs.db");

    auditTable.initialize();
    AuditLog testLog;

    testLog.email = email;
    testLog.ip_address = IP;
    testLog.timestamp = timestamp;

    testLog.event = AuditEvent::event;
    testLog.status = AuditStatus::status;

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
}