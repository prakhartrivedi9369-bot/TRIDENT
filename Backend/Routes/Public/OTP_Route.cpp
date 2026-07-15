#include "routes.h"
#include "otp.h"
#include "crow.h"
#include "Paths.h"
#include <string>
#include <iostream>

using namespace std;

void register_Otp_Routes(crow::SimpleApp& app)
{
     CROW_ROUTE(app, "/otp")([]()
     {
           ifstream file(Paths::HTML + "otp.html");

           if(!file.is_open()) return crow::response(404);

           stringstream buffer;
           buffer << file.rdbuf();

           return crow::response (buffer.str());
     });
     CROW_ROUTE(app, "/send-otp").methods("POST"_method)([](const crow::request& req)
     {
           auto body = crow::json::load(req.body);

           string email = body["email"].s();
           string otp = generateOTP();

           cout<<"fuck"<<endl;

           cout<<otp<<endl;

           sendEmail(email,otp);

           return crow::response(200);
     });
     CROW_ROUTE(app, "/verify-otp").methods("POST"_method)([](const crow::request& req)
     {
       auto body = crow::json::load(req.body);

       string email = body["email"].s();
       string otp = body["otp"].s();

       string result = verifyOTP(email, otp);

       crow::json::wvalue response;

            if(result=="OTP_VERIFIED")
            {
                response["success"] = true;
                response["code"] = "OTP_VERIFIED";
                response["message"] = "OTP verified successfully";
            }

            if(result=="WRONG_OTP")
            {
                response["success"] = false;
                response["code"] = "WRONG_OTP";
                response["message"] = "Wrong OTP, Try again!";
            }
            
            if(result == "OTP_EXPIRED")
            {
                response["success"] = false;
                response["code"] = "OTP_EXPIRED";
                response["message"] = "OTP Expired, Try again!";
            }

            if(result == "REDIS_ERROR")
            {
                response["success"] = false;
                response["code"] = "REDIS_ERROR";
                response["message"] = "Server Error!";
            }
    return crow::response(response);
    });
}

