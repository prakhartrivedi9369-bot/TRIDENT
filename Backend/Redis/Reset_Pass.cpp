#include "crypto_utils.h"
#include <sodium.h>
#include <sstream>
#include <iomanip>
#include "JWT_token.h"
#include "Logger.h"
#include "Redis.h"

using namespace std;

string generate_reset_token()
{
    unsigned char buffer[32];
    randombytes_buf(buffer,sizeof(buffer));

    stringstream ss;

    for(size_t i=0;i<sizeof(buffer); i++)
    {
        ss << hex
           << setw(2)
           << setfill('0')
        static_cast<int>(buffer[i]);
    }
    return ss.str();
}
int Reset_token_check(const string& reset_token)
{
    string 
}