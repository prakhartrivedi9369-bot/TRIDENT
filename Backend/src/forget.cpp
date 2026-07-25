#include "crow.h"
#include <string>
#include "database.h"
#include "Logger.h"

using namespace std;

void handle_forget(const crow::request& req,crow::response& res);

void handle_forget(const crow::request& req, crow::response& res)
{
    res.set_header
    ("Content-Type","application/json");
    auto json_data = crow::json::load(req.body);
    string email = json_data["email"].s();
    string IP=req.remote_ip_address;

    try
    {
         if(!json_data || !json_data.has("email"))
         {
            res.code = 400;
            res.body= "{\"error\":\"username,email,password required\"}";
            saveLogInDB(LogEntry::DETAILS_MISSING,IP,email);
            return;
         }

         int forget_status = verifyUserInDB(email);

         if(forget_status == 1)
         {
            saveLogInDB(LogEntry::FORGET_PASSWORD,IP,email);
            res.code = 200;
            res.body = "{\"status\":\"otp_required\",""\"message\":\"OTP sent to email\"}";
         }
         else if(forget_status == 0)
         {
             saveLogInDB(LogEntry::USER_NOT_FOUND,IP,email);
             res.code = 409;
             res.body = "{\"status\":\"fail\",""\"message\":\"No user found!\"}";
         }
         else
         {
            saveLogInDB(LogEntry::DB_CONNECTION_ISSUE,IP,email);
            res.code = 500;
            res.body = "{\"status\":\"fail\",""\"error\":\"Database error\"}";
         }
    }
    catch(const exception& e)
    {
          saveLogInDB(LogEntry::SERVER_ERROR,IP,email);
          res.code = 500;
          res.body = "{\"status\":\"fail\",""\"error\":\"Internal server error\"}";
    }
}