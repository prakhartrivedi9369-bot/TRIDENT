#include "database.h"
#include "crypto_utils.h"
#include <bson/bson.h>
#include <mongoc/mongoc.h>
#include <string>
#include <iostream>

using namespace std;

int verifyCredentialsInDB(const string& email, const string& password);

int verifyCredentialsInDB(const string& email, const string& password)
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

// STEP 3: DB se hashed password nikalo
string stored_hash = "";
bson_iter_t iter;

if(bson_iter_init_find(&iter, doc, "password") && BSON_ITER_HOLDS_UTF8(&iter))
{
    stored_hash = bson_iter_utf8(&iter, nullptr);
}

mongoc_cursor_destroy(cursor);
mongoc_collection_destroy(collection);

// STEP 4: Hash empty check
if(stored_hash.empty())
{
    return -1; // Server side problem
}

// STEP 5: Libsodium se verify karo
if(CryptoUtils::verify_password(password, stored_hash))
{
    return 1; // ✅ Success
}
else
{
    return 2; // ❌ Password galat
}
}