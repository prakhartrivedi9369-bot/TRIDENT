#ifndef DATABASE_H
#define DATABASE_H

#include <mongoc/mongoc.h>

//Global client pointer taaki baaki files use kar sakein
extern mongoc_client_t *global_db_client;

//Functions declarations
void init_database();
void cleanup_database();

#endif