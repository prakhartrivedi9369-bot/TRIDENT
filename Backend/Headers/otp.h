#pragma once

#include <string>
#include <ctime>
#include "crow.h"
#include "Redis.h"

using namespace std;

string generateOTP();

OTPstatus verifyOTP
(
    const string& email,
    const string& enteredOtp,
    RedisManager& RedisManager
);

// bool storeOTP(const string& email,const string& otp);

bool sendEmail(
    const string&  recipient,
    const string& otp,
    RedisManager& RedisManager
);