#pragma once
#include <string>
#include "jwt.h"

using namespace std;

namespace AuthUtils {

    string create_jwt_token(const string& user_id);
    string verify_jwt_token(const string& token);
    string build_auth_cookie(const string& token);

}