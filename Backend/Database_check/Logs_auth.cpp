#include "database.h"
#include <mongoc/mongoc.h>
#include <bson/bson.h>
#include <chrono>
#include <string>
#include "Logger.h"

using namespace std;

void saveLogInDB(LogEntry event,const string& ip,const string& email)
{
     int64_t timestamp = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
     string status;
     switch(event)
     {
        case LogEntry::DETAILS_MISSING:
        {
             status="Details_Missing";
             break;
        }
        case LogEntry::LOGIN_SUCCESS:
        {
            status="LOGIN_SUCCESS";
            break;
        }
        case LogEntry::SIGNUP_SUCCESS:
        {
            status="SIGNUP_SUCCESS";
            break;
        }
        case LogEntry::USER_NOT_FOUND:
        {
            status="USER_NOT_FOUND";
            break;
        }
        case LogEntry::USER_ALREADY_EXIST:
        {
            status="USER_ALREADY_EXIST";
            break;
        }
        case LogEntry::INVALID_PASSWORD:
        {
            status="INVALID_PASSWORD";
            break;
        }
        case LogEntry::DB_CONNECTION_ISSUE:
        {
            status="DB_CONNECTION_ISSUE";
            break;
        }
        case LogEntry::SERVER_ERROR:
        {
            status="SERVER_ERROR";
            break;
        }
     }
    mongoc_collection_t *collection = mongoc_client_get_collection(global_db_client,"CPP-database","Logs");
    bson_error_t error;

    string ip_json = "{\"IP_Address\":\"" + ip + "\"}";

    bson_t *ip_query = bson_new_from_json((const uint8_t*)ip_json.c_str(),-1,&error);
    
    if(!ip_query)
    {
        mongoc_collection_count_documents(collection,ip_query,NULL,NULL,NULL,&error); 
    }

    int64_t ip_count = mongoc_collection_count_documents(collection,ip_query,NULL,NULL,NULL,&error);
    bson_destroy(ip_query);

    bson_t *new_user = BCON_NEW(
    "Event",     BCON_UTF8(status.c_str()),
    "IP_Address",BCON_UTF8(ip.c_str()),
    "Email",     BCON_UTF8(email.c_str()),
    "Timestamp", BCON_DATE_TIME(timestamp)
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
