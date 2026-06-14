#include "../Headers/database.h"
#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <string>
#include <iostream>
using namespace std;

int verifyCredentialsInDB(const string& email, const string& password)
{
       if(!global_db_client)
       {
          return -1;
       }
       mongoc_collection_t *collection = mongoc_client_get_collection(global_db_client, "YOUR_DATABASE_NAME", "users");
       bson_error_t error;

       string email_json = "{\"email\":\"" + email + "\"}";
       bson_t *email_query = bson_new_from_json((const uint8_t*)email_json.c_str(),-1,&error);

       int64_t email_count = mongoc_collection_count_documents(collection,email_query,NULL,NULL,NULL,&error);
       bson_destroy(email_query);

       if(email_count<=0)
       {
          mongoc_collection_destroy(collection);
          return 0;
       }

       string full_json = "{\"email\":\"" + email + "\", \"password\":\"" + password + "\"}";
       bson_t*full_query = bson_new_from_json((const uint8_t*)full_json.c_str(),-1,&error);

       int64_t full_count = mongoc_collection_count_documents(collection,full_query,NULL,NULL,NULL,&error);
       bson_destroy(full_query);

       mongoc_collection_destroy(collection);
       
       if(full_count>0)
       {
          return 1;
       }
       else
       {
         return 2;
       }
}