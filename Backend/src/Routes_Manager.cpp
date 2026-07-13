#include "routes.h"
#include "crow.h"

using namespace std;

void registerRoutes(crow::SimpleApp& app)
{
    register_Home_Routes(app);
    register_Login_Routes(app);
    register_Signup_Routes(app);
    register_Otp_Routes(app);
}