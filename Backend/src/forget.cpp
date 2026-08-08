#include "crow.h"
#include <string>
#include "database.h"
#include "Logger.h"
#include "Redis.h"
#include "AuditLogger.h"
#include <sw/redis++/redis++.h>

using namespace std;

void handle_forget(const crow::request& req,crow::response& res,AuditLogger &AuditLogger);
void Pass_reset(const crow::request& req,crow::response& res,RedisManager& RedisManager,AuditLogger &AuditLogger);

void handle_forget(const crow::request& req, crow::response& res,AuditLogger &AuditLogger)
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
            AuditLogger.logForgetVerifyFailure(email,IP,"username,email,password empty");
            res.body= "{\"error\":\"username,email,password required\"}";
            return;
         }

         int forget_status = verifyUserInDB(email);

         if(forget_status == 1)
         {
            res.code = 200;
            AuditLogger.logForgetVerifySuccess(email,IP,"Forget pass request accept")
            res.body = "{\"status\":\"otp_required\",""\"message\":\"OTP sent to email\"}";
         }
         else if(forget_status == 0)
         {
             res.code = 409;
             AuditLogger.logForgetVerifyFailure(email,IP,"No user found!");
             res.body = "{\"status\":\"fail\",""\"message\":\"No user found!\"}";
         }
         else
         {
            res.code = 500;
            AuditLogger.logForgetVerifyFailure(email,IP,"Database error");
            res.body = "{\"status\":\"fail\",""\"error\":\"Database error\"}";
         }
    }
    catch(const exception& e)
    {
          res.code = 500;
          AuditLogger.logForgetVerifyFailure(email,IP,"Internal server error");
          res.body = "{\"status\":\"fail\",""\"error\":\"Internal server error\"}";
    }
}

void Pass_reset(const crow::request& req,crow::response& res,RedisManager& RedisManager,AuditLogger &AuditLogger)
{
     res.set_header("Content-Type", "application/json");

     // 1. Request body ko JSON mai parse karna
     auto json_data = crow::json::load(req.body);
     string new_password = json_data["confirmPassword"].s();
     string reset_token = json_data["reset_token"].s();
     string IP=req.remote_ip_address;

     string token_key = "token_email:" + reset_token;

     auto stored_token = redis.get(token_key);

     if(!stored_token)
     {
          res.code = 401;
          AuditLogger.logPasswordResetFailure(email,IP,"username,email,password empty");
          res.body = "{\"status\": \"fail\",""\"message\": \"PASSWORD_RESET_TOKEN_EXPIRED\"}";
          return;
     }

     string email = *stored_token;

        try
        {
            if(!json_data)
            {
                res.code = 400; //Bad request
                AuditLogger.log
                res.body = "{\"error\": \"password required\"}";
                return;
            }
        
            int Pass_reset_status = New_password(new_password,reset_token,RedisManager);
                // 3.Response Generation (If-Else logic)
                if(Pass_reset_status == 1)
                {
                     res.code = 201;
                     AuditLogger.logPasswordResetSuccess(email,IP,"Password reset success");
                     res.body = "{\"status\": \"Success\",""\"message\": \"Password reset successful!\"}";
                }
                else if(Pass_reset_status == 0)
                {
                     res.code = 401;
                     AuditLogger.logPasswordResetFailure(email,IP,"PASSWORD_RESET_TOKEN_EXPIRED");
                     res.body = "{\"status\": \"fail\",""\"message\": \"PASSWORD_RESET_TOKEN_EXPIRED\"}";
                }
                else if(Pass_reset_status == -1)
                {
                     res.code = 404;
                     AuditLogger.logPasswordResetFailure(email,IP,"REDIS_ERROR");
                     res.body = "{\"status\":\"fail\",""\"message\": \"REDIS_ERROR\"}";
                }
                else if(Pass_reset_status==-2)
                {
                     res.code = 429;
                     AuditLogger.logPasswordResetFailure(email,IP,"Database error");
                     res.body = "{\"status\":\"fail\",\"message\":\"Database error\"}";
                }
                else
                {
                     res.code = 500;
                     AuditLogger.logPasswordResetFailure(email,IP,"Server connection issue.");
                     res.body = "{\"status\":\"error\",\"message\":\"Server connection issue.\"}";
                }
        }
        catch(const std::exception& e)
        {
            //Agar database down hua ya koei aur crash hua toh server nahi rukega
            res.code = 500;
            AuditLogger.logPasswordResetFailure(email,IP,"Internal Server Error. Database connectivity issue.");
            res.body = "{\"status\": \"error\", \"error\": \"Internal Server Error. Database connectivity issue.\"}";
        }
}