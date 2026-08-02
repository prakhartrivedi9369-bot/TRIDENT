#include "routes.h"
#include "crow.h"
#include "Paths.h"
#include <string>
#include <ctime>
#include <iostream>
#include "Redis.h"
#include "JWT_token.h"

using namespace std;

string generateOTP();
VerifyOtpResult sendEmail(const string&  recipient,const string& otp,RedisManager& RedisManager,const string &usecase);
VerifyOtpResult verifyOTP(const string& email,const string& enteredOtp,RedisManager& RedisManager);

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

           VerifyOtpResult result = sendEmail(email,otp,RedisManager,usecase);

           crow::json::wvalue json;

           json["success"] = result.success;
           json["message"] = result.message;

           if(result.usecase)
           {
              json["usecase"] = *result.usecase;
           }
           if(result.reset_token)
           {
              json["reset_token"] = *result.reset_token;
           }

           return crow::response(200,json);
     });
     CROW_ROUTE(app, "/verify-otp").methods("POST"_method)([&RedisManager](const crow::request& req)
     {
        auto body = crow::json::load(req.body);

        string email = body["email"].s();
        string otp = body["otp"].s();

        VerifyOtpResult result = verifyOTP(email, otp, RedisManager);

        crow::json::wvalue json;

           json["success"] = result.success;
           json["message"] = result.message;

           if(result.usecase)
           {
              json["usecase"] = *result.usecase;
           }
           if(result.reset_token)
           {
              json["reset_token"] = *result.reset_token;
           }

           return crow::response(200,json);
    });
}

