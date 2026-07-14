#include "otp.h"
#include "env_config.h"
#include <random>
#include <curl/curl.h>
#include <sstream>
#include <string>
#include <iostream>
#include "Redis.h"

using namespace std;

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

string verifyOTP(const string& email,const string& enteredOtp)
{
    try
    {
        string key = "otp:" + email;
        auto value = redis.get(key);
        if(!value) 
        {
           return EXPIRED;
        }
        if(value!=enteredOtp)
        {
           return WRONG_OTP;
        }
        if(value==enteredOtp)
        {
           redis.del(key);
           return VERIFIED;
        }
    }
    catch(const sw::redis::Error &e)
    {
        return REDIS_ERROR;
    }
}
bool sendEmail(const string& recipient,const string& otp)
{
    string key = "otp:" + recipient;
    redis.setex(key,120,otp);

    string API_KEY = getEnvValue("Brevo_REST_API");

    CURL* curl = curl_easy_init();

    if(!curl)
    {
        cout<<"Curl init failed"<<endl;
        return false;
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

    return (res == CURLE_OK);
}
