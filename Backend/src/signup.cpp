#include "crow.h"
#include <string>
#include "database.h"
#include "Logger.h"
#include "AuditLogger.h"

using namespace std;

void handle_signup(const crow::request& req,crow::response& res,AuditLogger &AuditLogger);

void handle_signup(const crow::request& req, crow::response& res,AuditLogger &AuditLogger)
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
            AuditLogger.logSignupFailure(email,IP,"Incomplete Input");
            res.body= "{\"error\":\"username,email,password required\"}";
            return;
         }

         string username = json_data["username"].s();
         string password = json_data["password"].s();

         int signup_status = createUserInDB(username,email,password);

         if(signup_status == 1)
         {
            res.code = 200;
            AuditLogger.logSignupSuccess(email,IP,"Signup success");
            res.body = "{\"status\":\"otp_required\",""\"message\":\"OTP sent to email\"}";
         }
         else if(signup_status == 0)
         {
             res.code = 409;
             AuditLogger.logSignupFailure(email,IP,"User already exist");
             res.body = "{\"status\":\"fail\",""\"error\":\"User already exists\"}";
         }
         else
         {
            res.code = 500;
            AuditLogger.logSignupFailure(email,IP,"Database error");
            res.body = "{\"status\":\"fail\",""\"error\":\"Database error\"}";
         }
    }
    catch(const exception& e)
    {
          res.code = 500;
          AuditLogger.logSignupFailure(email,IP,"Internal server error");
          res.body = "{\"status\":\"fail\",""\"error\":\"Internal server error\"}";
    }
}