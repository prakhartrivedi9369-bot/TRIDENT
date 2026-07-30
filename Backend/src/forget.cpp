#include "crow.h"
#include <string>
#include "database.h"
#include "Logger.h"

using namespace std;

void handle_forget(const crow::request& req,crow::response& res);
void Pass_reset(const crow::request& req,crow::response& res);

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
   string reset_token = json_data["Reset_token"].s();
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
        
            int Attempt_status = New_password(new_password,reset_token);
                // 3.Response Generation (If-Else logic)
                if(Attempt_status == 1)
                {
                     saveLogInDB(LogEntry::LOGIN_SUCCESS,IP,email);
                     res.code = 201;
                     res.body = "{\"status\": \"otp_required\",""\"message\": \"Login successful!\"}";
                }
                else if(Attempt_status == 0)
                {
                     saveLogInDB(LogEntry::USER_NOT_FOUND,IP,email);
                     res.code = 401;
                     res.body = "{\"status\": \"USER_NOT_FOUND\", \"error\": \"Unknown user / No user exist!\"}";
                }
                else if(Attempt_status == 2)
                {
                     saveLogInDB(LogEntry::INVALID_PASSWORD,IP,email);
                     res.code = 404;
                     res.body = "{\"status\":\"Invalid_pass\"}";
                }
                else if(Attempt_status==3)
                {
                     saveLogInDB(LogEntry::ATTEMPT_LIMIT_EXCEED,IP,email);
                     res.code = 429;
                     res.body = "{\"status\":\"ATTEMPT_LIMIT_EXCEED\",\"error\":\"Attempt Limit Exceed.Please Try again later!\"}";
                }
                else
                {
                     saveLogInDB(LogEntry::DB_CONNECTION_ISSUE,IP,email);
                     res.code = 500;
                     res.body = "{\"status\":\"error\",\"error\":\"Database connection issue.\"}";
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