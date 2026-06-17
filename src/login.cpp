//login.cpp
#include "../Headers/login.h"
#include "crow.h"
#include <string>
using namespace std;

//auth.cpp wale function ka dclaration (agar header file nahi banayi toh)
int verifyCredentialsInDB(const string& email, const string& password);

void handleLogin(const crow::request& req, crow::response& res)
{
    //Crow mai response headers ko JSON set kar dete hain
    res.set_header("Content-Type", "application/json");
  
    try
    {
        // 1. Request body ko JSON mai parse karna
        auto json_data = crow::json::load(req.body);
        if(!json_data || !json_data.has("email") || !json_data.has("password"))
        {
            res.code = 400; //Bad request
            res.body = "{\"error\": \"Email and password required\"}";
            return;
        }

        string email = json_data["email"].s();
        string password = json_data["password"].s();

        // 2. auth.cpp ke function ko call lagaya DB check karne ke liye
        int login_status = verifyCredentialsInDB(email, password);

        // 3.Response Generation (If-Else logic)
        if(login_status == 1)
        {
            res.code = 201;
            res.body = "{\"status\": \"otp_required\",""\"message\": \"Login successful!\"}";
        }
        else if(login_status == 0)
        {
            res.code = 401;
            res.body = "{\"status\": \"fail\", \"error\": \"Unknown user / No user exist!\"}";
        }
        else if(login_status == 2)
        {
            res.code = 404;
            res.body = "{\"status\":\"Invalid_pass\"}";
        }
        else
        {
            res.code = 500;
            res.body = "{\"status\":\"error\",\"error\":\"Database connection issue.\"}";
        }
    }
    catch(const std::exception& e)
    {
        //Agar database down hua ya koei aur crash hua toh server nahi rukega
        res.code = 500;
        res.body = "{\"status\": \"error\", \"error\": \"Internal Server Error. Database connectivity issue.\"}";
    }
}