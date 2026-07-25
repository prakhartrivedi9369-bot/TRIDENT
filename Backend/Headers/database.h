#ifndef DATABASE_H
#define DATABASE_H

#include <mongoc/mongoc.h>
#include "crow.h"
#include <string>
#include "Logger.h"

using namespace std;

//Global client pointer taaki baaki files use kar sakein
extern mongoc_client_t *global_db_client;

//Functions declarations
void init_database();
void cleanup_database();
int verifyCredentialsInDB(const string& email, const string& password);
int createUserInDB(const string& username, const string& email, const string& password);
void saveLogInDB(LogEntry event,const string& ip,const string& email);
int verifyUserInDB(const string& email);

#endif