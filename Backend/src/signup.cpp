#include "crow.h"
#include <string>
#include "database.h"
#include "Logger.h"

using namespace std;

void handle_signup(const crow::request& req,crow::response& res);

void handle_signup(const crow::request& req, crow::response& res)
{
    res.set_header
    ("Content-Type","application/json");
   auto json_data = crow::json::load(req.body);
   string email = json_data["email"].s();
   string IP=req.remote_ip_address;

    try
    {
         if(!json_data || !json_data.has("username") || !json_data.has("email") || !json_data.has("password"))
         {
            res.code = 400;
            res.body= "{\"error\":\"username,email,password required\"}";
            saveLogInDB(LogEntry::DETAILS_MISSING,IP,email);
            return;
         }

         string username = json_data["username"].s();
         string password = json_data["password"].s();

         int signup_status = createUserInDB(username,email,password);

         if(signup_status == 1)
         {
            saveLogInDB(LogEntry::SIGNUP_SUCCESS,IP,email);
            res.code = 200;
            res.body = "{\"status\":\"otp_required\",""\"message\":\"OTP sent to email\"}";
         }
         else if(signup_status == 0)
         {
             saveLogInDB(LogEntry::USER_ALREADY_EXIST,IP,email);
             res.code = 409;
             res.body = "{\"status\":\"fail\",""\"error\":\"User already exists\"}";
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