#include "../Headers/database.h"
#include <mongoc/mongoc.h>
#include <bson/bson.h>
#include <string>
#include <iostream>

using namespace std;

int createUserInDB(const string& username, const string& email, const string& password)
{
    if(!global_db_client)
    { 
        return -1;
    }
     
    mongoc_collection_t *collection = mongoc_client_get_collection(global_db_client,"YOUR_DATABASE_NAME","users");
    bson_error_t error;

    string email_json = "{\"email\":\"" + email + "\"}";

    bson_t *email_query = bson_new_from_json((const uint8_t*)email_json.c_str(),-1,&error);
    
    if(!email_query)
    {
        mongoc_collection_count_documents(collection,email_query,NULL,NULL,NULL,&error);
    }

    int64_t email_count = mongoc_collection_count_documents(collection,email_query,NULL,NULL,NULL,&error);
    bson_destroy(email_query);

    if(email_count > 0)
    {
        mongoc_collection_destroy(collection);
        return 0;
    }

    string user_json = "{""\"username\":\"" + username + "\",""\"email\":\"" + email + "\",""\"password\":\"" + password + "\"""}";
    bson_t *new_user = bson_new_from_json((const uint8_t*)user_json.c_str(),-1,&error);

    if(!new_user)
    { 
        mongoc_collection_destroy(collection);
        return -1;
    }

    bool inserted = mongoc_collection_insert_one(collection,new_user,NULL,NULL,&error);

    bson_destroy(new_user);
    mongoc_collection_destroy(collection);

    if(!inserted)
    {
        return -1;
    }
    return 1;
}

