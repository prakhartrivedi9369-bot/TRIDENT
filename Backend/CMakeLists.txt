cmake_minimum_required(VERSION 3.15)
project(CrowProject CXX)

set(CMAKE_CXX_STANDARD 17)

# Crow ko batana ki Asio bina kisi system package ke standalone kaam karega
add_compile_definitions(ASIO_STANDALONE)
add_compile_definitions(CROW_ENABLE_SSL=OFF)

#---------------------------------------------------------------------------------------
# 1. Executable and includes(Headers ka raasta)
#---------------------------------------------------------------------------------------
  include_directories(Crow/include)
# Note-- Use paths according to your local system.

#--------------------------------------------------------------------------------------
# 2. Existing Crow & Boost Dependencies(NO find_package for Boost)
#-------------------------------------------------------------------------------------- 
# Crow repository ko as a sub-project add karein
add_subdirectory(External/Crow)

# Apne main executable ko define karein
add_executable(my_app 
               main.cpp 
               src/login.cpp
               src/database.cpp 
               src/signup.cpp 
               src/otp.cpp
               src/env_config.cpp
               src/crypto_utils.cpp
               Database_check/login_auth.cpp 
               Database_check/signup_auth.cpp
               Database_check/Logs_auth.cpp
               src/JWT_token.cpp
               Routes/Public/Home_Route.cpp
               Routes/Public/Login_Route.cpp
               Routes/Public/Signup_Route.cpp
               Routes/Public/OTP_Route.cpp
               src/Routes_Manager.cpp
               src/Redis.cpp
               )

target_include_directories(my_app PRIVATE
            ${CMAKE_SOURCE_DIR}/Headers
            ${CMAKE_SOURCE_DIR}/External/Crow/include
            ${CMAKE_SOURCE_DIR}/External/jwt-cpp/include/jwt-cpp
            ${CMAKE_SOURCE_DIR}/External/jwt-cpp/include
            /usr/include
            /usr/include/mongoc-2.3.0
            /usr/include/bson-2.3.0
)

target_compile_definitions(my_app 
PRIVATE
    PROJECT_ROOT="${CMAKE_SOURCE_DIR}"
)

#----------------------------------------------------------------------------------------
# 3. Linking Libraries (Crow, Boost, Windows Networking, & MongoDB C Driver)
#----------------------------------------------------------------------------------------
target_link_libraries(my_app PRIVATE
    Crow::Crow
    sodium
    mongoc2
    bson2
    ssl
    crypto
    z
    snappy
    zstd
    curl
    redis++
    hiredis
)