#include "signup.h"
#include "crow.h"
#include <string>

using namespace std;

int createUserInDB(const string& username, const string& email, const string& password);

void handle_signup(const crow::request& req, crow::response& res)
{
    res.set_header
    ("Content-Type","application/json");
    try
    {
         auto json_data = crow::json::load(req.body);
         if(!json_data || !json_data.has("username") || !json_data.has("email") || !json_data.has("password"))
         {
            res.code = 400;
            res.body= "{\"error\":\"username,email,password required\"}";
            return;
         }

         string username = json_data["username"].s();
         string email = json_data["email"].s();
         string password = json_data["password"].s();

         int signup_status = createUserInDB(username,email,password);

         if(signup_status == 1)
         {
            res.code = 200;
            res.body = "{\"status\":\"otp_required\",""\"message\":\"OTP sent to email\"}";
         }
         else if(signup_status == 0)
         {
             res.code = 409;
             res.body = "{\"status\":\"fail\",""\"error\":\"User already exists\"}";
         }
         else
         {
            res.code = 500;
            res.body = "{\"status\":\"fail\",""\"error\":\"Database error\"}";
         }
    }
    catch(const exception& e)
    {
          res.code = 500;
          res.body = "{\"status\":\"fail\",""\"error\":\"Internal server error\"}";
    }
}