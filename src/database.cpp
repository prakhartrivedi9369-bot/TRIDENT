#include "../Headers/database.h"
#include "../Headers/env_config.h"
#include <cstdlib>
#include <iostream>

using namespace std;

mongoc_client_t *global_db_client = nullptr;

void init_database()
{
    mongoc_init();

    bson_error_t error;
    
    string mongodb_url = getEnvValue("MONGO_URI");

    mongoc_uri_t *uri = mongoc_uri_new_with_error(mongodb_url.c_str(), &error);
    
    if(!uri)
    {
        std::cerr << "URI Error: "<<error.message <<std::endl;
        return ;
    }
    
    mongoc_uri_destroy(uri);
    global_db_client = mongoc_client_new(mongodb_url.c_str());
}
void cleanup_database()
{
    if(global_db_client)
    {
        mongoc_client_destroy(global_db_client);
    }
    mongoc_cleanup();
}