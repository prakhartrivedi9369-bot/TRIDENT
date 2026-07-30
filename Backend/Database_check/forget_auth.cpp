#include "database.h"
#include "crypto_utils.h"
#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <string>
#include <iostream>
#include "Redis.h"

using namespace std;

int verifyUserInDB(const string& email)
{
      if(!global_db_client)
      {
           return -1;
      }

     mongoc_collection_t *collection = mongoc_client_get_collection(
          global_db_client, "CPP_database", "users"
        );
     bson_error_t error;

     // STEP 1: Sirf email se document dhundo
     bson_t *email_query = BCON_NEW("email", BCON_UTF8(email.c_str()));

     mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(
          collection, email_query, NULL, NULL
        );
     bson_destroy(email_query);

     // STEP 2: Document mila ya nahi
     const bson_t *doc;
     if(!mongoc_cursor_next(cursor, &doc))
     {
          // Email DB mein nahi mili
          mongoc_cursor_destroy(cursor);
          mongoc_collection_destroy(collection);
          return 0;
     }

     mongoc_cursor_destroy(cursor);
     mongoc_collection_destroy(collection);

     return 1; // ✅ Success
}
int New_password(const string& new_password,const string& reset_token)
{
     int Redis_status = Reset_token_check(reset_token);
}