// #include "otp.h"
#include "env_config.h"
#include <random>
#include <ctime>
#include <curl/curl.h>
#include <sstream>
#include <string>
#include <iostream>
#include "Redis.h"

using namespace std;

string generateOTP();
string sendEmail(const string&  recipient,const string& otp,RedisManager& RedisManager,const string &usecase);
OTPstatus verifyOTP(const string& email,const string& enteredOtp,RedisManager& RedisManager);

static size_t WriteCallback(
    void* contents,
    size_t size,
    size_t nmemb,
    string* output)
{
    output->append(
        (char*)contents,
        size * nmemb
    );
    return size * nmemb;
}

string generateOTP()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(100000,999999);

    return to_string(dist(gen));
}

VerifyOtpResult verifyOTP(const string& email,const string& enteredOtp, RedisManager& RedisManager)
{
    auto status = RedisManager.verifyOtp(email,enteredOtp);

    if(status=="FORGET_PASSWORD")
    {
        return OTPstatus::FORGET_PASSWORD;
    }
    if(status=="LOGIN_SUCCESS")
    {
        return OTPstatus::OTP_VERIFIED;
    }
    if(status=="WRONG_OTP")
    {
        return OTPstatus::WRONG_OTP;
    }
    if(status=="EXPIRED")
    {
        return OTPstatus::EXPIRED;
    }
    return OTPstatus::REDIS_ERROR;
}
VerifyOtpResult sendEmail(const string& recipient,const string& otp, RedisManager& RedisManager, const string &usecase)
{
    if(RedisManager.storeOtp(recipient,otp,120,usecase))
    {
        cout<<"OTP stored in Redis successFully! " << endl;
    }
    else{
        cout << "OTP store in Redis failed " << endl;
    }

    string API_KEY = getEnvValue("Brevo_REST_API");

    CURL* curl = curl_easy_init();

    if(!curl)
    {
        cout<<"Curl init failed"<<endl;
        return {
               false,
               nullopt,
               nullopt,
               "CURL_INIT_FAILED"
        };
    }

    curl_easy_setopt(
        curl,
        CURLOPT_URL,"https://api.brevo.com/v3/smtp/email"
    );

    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(
        headers,
        "accept: application/json"
    );

    headers = curl_slist_append(
        headers,
        "content-type: application/json"
    );

    string apiHeader = "api-key: " + API_KEY;

    headers = curl_slist_append(
        headers,
        apiHeader.c_str()
    );

    curl_easy_setopt(
        curl,
        CURLOPT_HTTPHEADER,
        headers
    );

    stringstream payload;
    string sender = getEnvValue("Email");

    payload
    <<"{"
    <<"\"sender\":{"
    <<"\"name\":\"Prakhar\","
    <<
    "\"email\":\"" + sender + "\""
    << "},"
    
    << "\"to\":[{"
    << "\"email\":\"" << recipient << "\""
    << "}],"

    <<"\"subject\":\"YOUR OTP CODE\","

    <<"\"htmlContent\":\"<h2>Your OTP is: "
    << otp
    <<"<h2>\""

    <<"}";

    string jsonData = payload.str();

    curl_easy_setopt(
        curl,
        CURLOPT_POSTFIELDS,
        jsonData.c_str()
    );

    curl_easy_setopt(
        curl,
        CURLOPT_POST,
        1L
    );
    string responseBody;

    curl_easy_setopt(
        curl,
        CURLOPT_WRITEFUNCTION,
        WriteCallback
    );
    curl_easy_setopt(
        curl,
        CURLOPT_WRITEDATA,
        &responseBody
    );

    CURLcode res = curl_easy_perform(curl);

    long response_code = 0;

    curl_easy_getinfo(
        curl,
        CURLINFO_RESPONSE_CODE,
        &response_code
    );

    if(res != CURLE_OK)
    {
        cerr<<"Request failed! : "<<curl_easy_strerror(res)<<endl;
    }
    else
    {
        cout<<"Email request sent successfully"<<endl;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if(res == CURLE_OK)
    {
        return{
            true,
            usecase,
            nullopt,
            usecase
        };
    }
}
