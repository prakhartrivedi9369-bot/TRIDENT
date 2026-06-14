#include "../Headers/database.h"
#include <cstdlib>
#include <iostream>

using namespace std;

mongoc_client_t *global_db_client = nullptr;

void init_database()
{
    mongoc_init();

    bson_error_t error;
    const char *env_uri=getenv("YOUR_MONGODB_URL_HERE");

    mongoc_uri_t *uri = mongoc_uri_new_with_error(env_uri, &error);
    
    if(!uri)
    {
        std::cerr << "URI Error: "<<error.message <<std::endl;
        return ;
    }
    
    mongoc_uri_destroy(uri);
    global_db_client = mongoc_client_new("YOUR_MONGODB_URL_HERE");
}
void cleanup_database()
{
    if(global_db_client)
    {
        mongoc_client_destroy(global_db_client);
    }
    mongoc_cleanup();
}