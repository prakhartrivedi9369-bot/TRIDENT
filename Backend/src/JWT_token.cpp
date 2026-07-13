#include "JWT_token.h"
#include "jwt.h"
#include <chrono>
#include <string>
#include "env_config.h"

using namespace std;

namespace AuthUtils {

    static const string& get_secret_key() {
        static const string SECRET_KEY = getEnvValue("JWT_SECRET_KEY");
        return SECRET_KEY;
    }

    string create_jwt_token(const string& user_id) {
        return jwt::create()
            .set_type("JWT")
            .set_issuer("your_app")
            .set_issued_now()
            .set_expires_in(chrono::seconds{3600 * 24})
            .set_payload_claim("user_id", jwt::claim(user_id))
            .sign(jwt::algorithm::hs256{get_secret_key()});
    }

    string verify_jwt_token(const string& token) {
        try {
            auto decoded = jwt::decode(token);
            auto verifier = jwt::verify()
                .allow_algorithm(jwt::algorithm::hs256{get_secret_key()})
                .with_issuer("your_app");
            verifier.verify(decoded);
            return decoded.get_payload_claim("user_id").as_string();
        } catch (const exception& e) {
            return "";
        }
    }

    string build_auth_cookie(const string& token) {
        return "token=" + token + "; HttpOnly; Secure; SameSite=Strict; Max-Age=86400; Path=/";
    }
}