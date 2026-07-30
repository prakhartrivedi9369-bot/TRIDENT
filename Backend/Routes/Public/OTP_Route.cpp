#include "routes.h"
#include "crow.h"
#include "Paths.h"
#include <string>
#include <ctime>
#include <iostream>
#include "Redis.h"
#incldue "JWT_token.h"

using namespace std;

string generateOTP();
string sendEmail(const string&  recipient,const string& otp,RedisManager& RedisManager,const string &usecase);
OTPstatus verifyOTP(const string& email,const string& enteredOtp,RedisManager& RedisManager);

void register_Otp_Routes(crow::SimpleApp& app,RedisManager& RedisManager)
{
     CROW_ROUTE(app, "/otp")([]()
     {
           ifstream file(Paths::HTML + "otp.html");

           if(!file.is_open()) return crow::response(404);

           stringstream buffer;
           buffer << file.rdbuf();

           return crow::response (buffer.str());
     });
     CROW_ROUTE(app, "/send-otp").methods("POST"_method)([&RedisManager](const crow::request& req)
     {
           auto body = crow::json::load(req.body);
            
           string email = body["email"].s();
           string usecase = body["usecase"].s();
           cout<<"fuck"<<endl;
           string otp = generateOTP();

           cout<<otp<<endl;

           auto result = sendEmail(email,otp,RedisManager,usecase);

           crow::json::wvalue response;

           response["success"] = result.success;
           response["usecase"] = result.usecase;
           response["reset_token"] = result.reset_token;
           response["message"] = result.message;

           return crow::response(response);
     });
     CROW_ROUTE(app, "/verify-otp").methods("POST"_method)([&RedisManager](const crow::request& req)
     {
       auto body = crow::json::load(req.body);

       string email = body["email"].s();
       string otp = body["otp"].s();

       OTPstatus result = verifyOTP(email, otp, RedisManager);

       crow::json::wvalue response;

            if(result == OTPstatus::OTP_VERIFIED)
            {
                response["success"] = true;
                response["code"] = "OTP_VERIFIED";
                response["message"] = "OTP verified successfully";
            }
            if(result == OTPstatus::FORGET_PASSWORD)
            {
                response["success"] = true;
                response["code"] = "FORGET_PASSWORD";
                response["reset_token"] = generate_reset_token();
                response["message"] = "OTP verified successfully";
            }
            if(result == OTPstatus::WRONG_OTP)
            {
                response["success"] = false;
                response["code"] = "WRONG_OTP";
                response["message"] = "Wrong OTP, Try again!";
            }
            
            if(result == OTPstatus::EXPIRED)
            {
                response["success"] = false;
                response["code"] = "OTP_EXPIRED";
                response["message"] = "OTP Expired, Try again!";
            }
            if(result == OTPstatus::REDIS_ERROR)
            {
                response["success"] = false;
                response["code"] = "REDIS_ERROR";
                response["message"] = "Server Error!";
            }
            return crow::response(response);
    });
}

