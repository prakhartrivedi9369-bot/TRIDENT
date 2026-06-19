#include "../Headers/otp.h"
#include "../Headers/config.h"
#include <random>
#include <curl/curl.h>
#include <sstream>
#include <string>

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
bool sendEmail(const string& recipient,const string& otp)
{
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

    cout << "Key=[" << API_KEY << "]"<<endl;
    cout<< "LEN=" << API_KEY.length() <<endl;

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

    payload
    <<"{"
    <<"\"sender\":{"
    <<"\"name\":\"Prakhar\","
    <<
    "\"email\":\"example@gmail.com\""
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

    cout<<"jsonData:"<<jsonData<<endl;

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
    cout<<responseBody<<endl;

    long response_code = 0;

    curl_easy_getinfo(
        curl,
        CURLINFO_RESPONSE_CODE,
        &response_code
    );

    cout<<"HTTP Code : "<< response_code <<endl;

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
