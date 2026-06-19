#pragma once

#include <string>
#include <unordered_map>
#include <ctime>
#include "crow.h"

using namespace std;

struct OTPData
{
    string otp;
    time_t createdAt;
};

extern unordered_map<string,OTPData> otpStore;

string generateOTP();

bool verifyOTP
(
     const string& email,
     const string& enteredOtp
);

bool storeOTP(const string& email,const string& otp);
bool verifyOTP(const string& email,const string& otp);

bool sendEmail(
    const string&  recipient,
    const string& otp
);