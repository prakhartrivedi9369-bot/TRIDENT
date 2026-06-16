#include "../Headers/otp.h"
#include <random>

using namespace std;

unordered_map<string, OTPData> otpStore;
string generateOTP()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(100000,999999);

    return to_string(dist(gen));
}

bool verifyOTP(const string& email,const string& enteredOtp)
{
    auto it = otpStore.find(email);

    if(it == otpStore.end()) 
    {
        return false;
    }

    if(it->second.otp != enteredOtp)
    {
        return false;
    }

    otpStore.erase(email);
    return true;
}
