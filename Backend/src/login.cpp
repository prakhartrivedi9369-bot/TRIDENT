#include "crow.h"
#include "database.h"
#include <string>
#include "Logger.h"
#include "Redis.h"

using namespace std;

void handleLogin(const crow::request& req,crow::response& res);

void handleLogin(const crow::request& req, crow::response& res)
{
    //Crow mai response headers ko JSON set kar dete hain
    res.set_header("Content-Type", "application/json");

    // 1. Request body ko JSON mai parse karna
    auto json_data = crow::json::load(req.body);
    string email = json_data["email"].s();
    string IP=req.remote_ip_address;
  
    try
    {
        if(!json_data || !json_data.has("email") || !json_data.has("password"))
        {
            res.code = 400; //Bad request
            res.body = "{\"error\": \"Email and password required\"}";
            saveLogInDB(LogEntry::DETAILS_MISSING,IP,email);
            return;
        }

        string password = json_data["password"].s();
        
        Attempt_check(email,IP,password);
        {   
              // 3.Response Generation (If-Else logic)
              if(login_status == 1)
              {
                  saveLogInDB(LogEntry::LOGIN_SUCCESS,IP,email);
                  res.code = 201;
                  res.body = "{\"status\": \"otp_required\",""\"message\": \"Login successful!\"}";
              }
              else if(login_status == 0)
              {
                  saveLogInDB(LogEntry::USER_NOT_FOUND,IP,email);
                  res.code = 401;
                  res.body = "{\"status\": \"fail\", \"error\": \"Unknown user / No user exist!\"}";
              }
              else if(login_status == 2)
              {
                  saveLogInDB(LogEntry::INVALID_PASSWORD,IP,email);
                  res.code = 404;
                  res.body = "{\"status\":\"Invalid_pass\"}";
              }
              else
              {
                  saveLogInDB(LogEntry::DB_CONNECTION_ISSUE,IP,email);
                  res.code = 500;
                  res.body = "{\"status\":\"error\",\"error\":\"Database connection issue.\"}";
              }
        }
        else
        {
              saveLogInDB(LogEntry::ATTEMPT_LIMIT_EXCEED,IP,email);
              res.code = 429;
              res.body = "{\"status\":\"Attempt_Limit\"}";
        }
    }
    catch(const std::exception& e)
    {
        //Agar database down hua ya koei aur crash hua toh server nahi rukega
        saveLogInDB(LogEntry::SERVER_ERROR,IP,email);
        res.code = 500;
        res.body = "{\"status\": \"error\", \"error\": \"Internal Server Error. Database connectivity issue.\"}";
    }
}