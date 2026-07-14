#pragma once

#include <string>
#include <unordered_map>
#include <ctime>
#include "crow.h"

using namespace std;

string generateOTP();

string verifyOTP
(
    const string& email,
    const string& enteredOtp
);

// bool storeOTP(const string& email,const string& otp);

bool sendEmail(
    const string&  recipient,
    const string& otp
);