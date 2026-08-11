#include "database.h"
#include <mongoc/mongoc.h>
#include <bson/bson.h>
#include <chrono>
#include <string>
#include "Logger.h"
#include "AuditLogs.h"

using namespace std;

void saveLogInDB(AuditLog log)
{   
    string status = toString(log.status);
    string event = toString(log.event);
    string Reason = log.Reason;
    string email = log.email;
    string ip_address = log.ip_address;
    string timestamp = log.timestamp;

    mongoc_collection_t *collection = mongoc_client_get_collection(global_db_client,"CPP-database","Logs");
    bson_error_t error;

    string ip_json = "{\"IP_Address\":\"" + ip_address + "\"}";

    bson_t *ip_query = bson_new_from_json((const uint8_t*)ip_json.c_str(),-1,&error);
    
    if(!ip_query)
    {
        mongoc_collection_count_documents(collection,ip_query,NULL,NULL,NULL,&error); 
    }

    int64_t ip_count = mongoc_collection_count_documents(collection,ip_query,NULL,NULL,NULL,&error);
    bson_destroy(ip_query);

    bson_t *new_user = BCON_NEW(
    "Event",     BCON_UTF8(event.c_str()),
    "IP_Address",BCON_UTF8(ip_address.c_str()),
    "Email",     BCON_UTF8(email.c_str()),
    "Reason",    BCON_UTF8(Reason.c_str()),
    "Status",    BCON_UTF8(status.c_str()),
    "Timestamp", BCON_UTF8(timestamp.c_str())
    );

    if(!new_user)
    { 
        mongoc_collection_destroy(collection);
        return; //return -1
    }

    bool inserted = mongoc_collection_insert_one(collection,new_user,NULL,NULL,&error);

    bson_destroy(new_user);
    mongoc_collection_destroy(collection);

    if(!inserted)
    {
        return; //return -1
    }
    return; //return 1;
}
