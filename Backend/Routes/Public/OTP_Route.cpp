#include "routes.h"
#include "otp.h"
#include "crow.h"
#include "Paths.h"
#include "database.h"

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

           OTPData data;
           data.otp = otp;
           data.createdAt = time(nullptr);

           otpStore[email] = data;

           cout<<otp<<endl;

           sendEmail(email,otp);

           return crow::response(200);
     });
     CROW_ROUTE(app, "/verify-otp").methods("POST"_method)([](const crow::request& req)
     {
       auto body = crow::json::load(req.body);

       string email = body["email"].s();
       string otp = body["otp"].s();

       bool result = verifyOTP(email, otp);

       crow::json::wvalue response;

       response["success"]= result;

       return crow::response(response);
     });
}

