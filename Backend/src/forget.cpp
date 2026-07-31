#include "crow.h"
#include <string>
#include "database.h"
#include "Logger.h"

using namespace std;

void handle_forget(const crow::request& req,crow::response& res);
void Pass_reset(const crow::request& req,crow::response& res,RedisManager& RedisManager);

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

void Pass_reset(const crow::request& req,crow::response& res,RedisManager& RedisManager)
{
     //Crow mai response headers ko JSON set kar dete hain
     res.set_header("Content-Type", "application/json");

     // 1. Request body ko JSON mai parse karna
     auto json_data = crow::json::load(req.body);
     string new_password = json_data["confirmPassword"].s();
     string reset_token = json_data["reset_token"].s();
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
        
            int Pass_reset_status = New_password(new_password,reset_token,RedisManager);
                // 3.Response Generation (If-Else logic)
                if(Pass_reset_status == 1)
                {
                     saveLogInDB(LogEntry::PASSWORD_RESET,IP,email);
                     res.code = 201;
                     res.body = "{\"status\": \"Success\",""\"message\": \"Password reset successful!\"}";
                }
                else if(Pass_reset_status == 0)
                {
                     saveLogInDB(LogEntry::PASSWORD_RESET_TOKEN_EXPIRED,IP,email);
                     res.code = 401;
                     res.body = "{\"status\": \"fail\",""\"message\": \"PASSWORD_RESET_TOKEN_EXPIRED\"}";
                }
                else if(Pass_reset_status == -1)
                {
                     saveLogInDB(LogEntry::INVALID_PASSWORD,IP,email);
                     res.code = 404;
                     res.body = "{\"status\":\"fail\",""\"message\": \"REDIS_ERROR\"}";
                }
                else if(Pass_reset_status==-2)
                {
                     saveLogInDB(LogEntry::DB_CONNECTION_ISSUE,IP,email);
                     res.code = 429;
                     res.body = "{\"status\":\"fail\",\"message\":\"Database error\"}";
                }
                else
                {
                     saveLogInDB(LogEntry::SERVER_ERROR,IP,email);
                     res.code = 500;
                     res.body = "{\"status\":\"error\",\"message\":\"Server connection issue.\"}";
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