cmake_minimum_required(VERSION 3.15)
project(CrowProject CXX)

set(CMAKE_CXX_STANDARD 17)

# Crow ko batana ki Asio bina kisi system package ke standalone kaam karega
add_compile_definitions(ASIO_STANDALONE)
add_compile_definitions(CROW_ENABLE_SSL=OFF)

#---------------------------------------------------------------------------------------
# 1. Executable and includes(Headers ka raasta)
#---------------------------------------------------------------------------------------
include_directories(crow/include)
include_directories("C:/msys64/mingw64/include")
include_directories("C:/msys64/ucrt64/include")
# Note-- Use paths according to your local system.

#--------------------------------------------------------------------------------------
# 2. Existing Crow & Boost Dependencies(NO find_package for Boost)
#-------------------------------------------------------------------------------------- 
# Crow repository ko as a sub-project add karein
add_subdirectory(External/crow)

# Apne main executable ko define karein
add_executable(my_app 
               src/main.cpp 
               src/login.cpp 
               src/database.cpp 
               src/signup.cpp 
               src/routes.cpp 
               src/otp.cpp
               src/env_config.cpp
               src/crypto_utils.cpp
               Database_check/login_auth.cpp 
               Database_check/signup_auth.cpp)

target_include_directories(my_app PRIVATE
            ${CMAKE_SOURCE_DIR}/Headers
            ${CMAKE_SOURCE_DIR}/External/Crow/include
            ${CMAKE_SOURCE_DIR}/External/jwt-cpp/include/jwt-cpp
)

#----------------------------------------------------------------------------------------
# 3. Linking Libraries (Crow, Boost, Windows Networking, & MongoDB C Driver)
#----------------------------------------------------------------------------------------
target_link_libraries(my_app PRIVATE
    Crow::Crow
    "C:/msys64/ucrt64/lib/libsodium.a"
    "C:/msys64/ucrt64/lib/libmongoc2.a"
    "C:/msys64/ucrt64/lib/libbson2.a"
    ssl
    crypto
    dnsapi
    bcrypt
    secur32
    crypt32
    ncrypt
    utf8proc
    z
    snappy
    zstd
    ws2_32 
    mswsock
    curl
)