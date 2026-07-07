<!-- 
  🔧 TODO: Replace "[Your Project Name]" everywhere in this file once the name is finalized.
  Tip: use Find & Replace in your editor for a quick swap.
-->

<div align="center">

# [Your Project Name]

### A high-performance, security-first stock exchange web application built in C++

[![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=cplusplus)](https://isocpp.org/)
[![Crow](https://img.shields.io/badge/Framework-Crow-black?style=for-the-badge)](https://crowcpp.org/)
[![MongoDB](https://img.shields.io/badge/Database-MongoDB-47A248?style=for-the-badge&logo=mongodb&logoColor=white)](https://www.mongodb.com/)
[![Redis](https://img.shields.io/badge/Cache-Redis-DC382D?style=for-the-badge&logo=redis&logoColor=white)](https://redis.io/)
[![License](https://img.shields.io/badge/License-Not%20Decided-lightgrey?style=for-the-badge)](#license)
[![Build](https://img.shields.io/badge/Build-CMake-064F8C?style=for-the-badge&logo=cmake&logoColor=white)](https://cmake.org/)

</div>

---

## 📖 Overview

**[Your Project Name]** is a full-stack stock exchange platform built from the ground up in **C++**, designed with a focus on **security, performance, and clean architecture**. The backend handles authentication, data persistence, and business logic, while the frontend delivers a real-time trading experience.

This project is under active development. The **authentication system** — signup, login, OTP-based two-factor verification, and JWT-based session management — is the current focus and is largely complete.

> ⚠️ **Status:** Work in progress. Core authentication flow is implemented; trading, portfolio, and market-data features are planned next.

---

## 📸 Screenshots

<div align="center">

### Landing Page
<img src="assets/landing-page.png" alt="Landing Page" width="800"/>

### Log In
<img src="assets/login-page.png" alt="Login Page" width="500"/>

### Create Account
<img src="assets/signup-page.png" alt="Signup Page" width="500"/>

### OTP Verification
<img src="assets/otp-verification.png" alt="OTP Verification" width="500"/>

</div>

---

## ✨ Features

### ✅ Implemented
- 🔐 **Secure Signup/Login** — passwords hashed with **Libsodium**, never stored in plaintext
- 📧 **Two-Factor Authentication (2FA)** — OTP sent via **Brevo**, verified before session is granted
- 🪪 **JWT-based Authentication** — stateless session management using **jwt-cpp (Thalhammer)**
- 🍪 **httpOnly Cookie Sessions** — tokens stored securely, inaccessible to client-side JavaScript (XSS-safe)
- ⚡ **Redis-backed OTP Sessions** — short-lived, auto-expiring (TTL) OTP verification flow
- 🛡️ **Injection-safe MongoDB Queries** — built using `BCON_NEW`, avoiding raw string concatenation
- 🧱 **Custom Auth Middleware** — Crow middleware for protecting routes with minimal boilerplate

### 🚧 Planned
- 📈 Live market data & price feeds
- 💹 Order placement and matching engine
- 💼 Portfolio & holdings dashboard
- 🔔 Real-time notifications
- 📊 Analytics and trade history

---

## 🏗️ Tech Stack

| Layer | Technology |
|---|---|
| **Language** | C++ (17) |
| **Web Framework** | [Crow](https://crowcpp.org/) |
| **Database** | MongoDB (via `libmongoc` / MongoDB C Driver) |
| **Cache / Session Store** | Redis |
| **Password Hashing** | Libsodium |
| **Authentication** | JWT via [jwt-cpp (Thalhammer)](https://github.com/Thalhammer/jwt-cpp) |
| **Networking** | Asio |
| **Security** | OpenSSL |
| **OTP Delivery** | [Brevo](https://www.brevo.com/) (Email API) |
| **Build System** | CMake |
| **Dev Environment** | MSYS64 (MinGW toolchain, compiler & dependency management) |
| **Frontend** | HTML, CSS, JavaScript |

---

## 🔄 Authentication Flow

The app uses a **two-stage login** process combining password verification and OTP-based 2FA before issuing a full-access JWT:

```
┌──────────┐     ┌───────────────┐     ┌─────────────┐     ┌──────────────┐     ┌───────────────┐
│  Signup  │ ──▶ │ Password Hash │ ──▶ │  Login      │ ──▶ │  OTP Sent    │ ──▶ │  OTP Verify   │
│          │     │  (Libsodium)  │     │  (verify)   │     │  (Brevo)     │     │   (Redis TTL) │
└──────────┘     └───────────────┘     └─────────────┘     └──────────────┘     └───────┬───────┘
                                                                                          │
                                                                                          ▼
                                                                              ┌────────────────────┐
                                                                              │  JWT Issued         │
                                                                              │  (httpOnly Cookie)  │
                                                                              └────────────────────┘
```

1. **Signup** — user registers; password is hashed via Libsodium before storage in MongoDB
2. **Login** — credentials verified against MongoDB; on success, an OTP session is created
3. **OTP Session** — OTP is generated, stored in **Redis with a TTL**, and emailed via **Brevo**
4. **OTP Verification** — user submits the OTP; on match, the Redis session is invalidated (single-use)
5. **JWT Issued** — a signed JWT is created and set as an **httpOnly, Secure cookie** — full access begins here, not before

---

## 📂 Project Structure

```
[your-project-name]/
├── CMakeLists.txt
├── .env                    # environment variables (not committed)
├── .gitignore
├── src/
│   ├── main.cpp             # entry point, route registration, env loading
│   ├── auth/
│   │   ├── JWT_token.h      # JWT function declarations
│   │   ├── JWT_token.cpp    # JWT create/verify logic
│   │   ├── login.cpp        # login route
│   │   ├── signup.cpp       # signup route
│   │   └── otp.cpp          # OTP generation & verification routes
│   ├── db/
│   │   └── mongo_client.cpp # MongoDB connection & query helpers
│   └── middleware/
│       └── auth_middleware.h
├── frontend/
│   ├── index.html
│   ├── login.html
│   ├── signup.html
│   ├── otp.html
│   ├── css/
│   └── js/
└── README.md
```

> 📝 Adjust this tree to match your actual folder layout if it has evolved.

---

## ⚙️ Getting Started

### Prerequisites

This project is developed using **MSYS64** (MinGW toolchain) on Windows. Make sure the following are installed:

- [MSYS64](https://www.msys2.org/)
- CMake (≥ 3.15)
- A C++17-compatible compiler (MinGW-w64 GCC)
- MongoDB C Driver (`libmongoc`, `libbson`)
- Redis (server + a C/C++ client library)
- OpenSSL
- Libsodium
- Asio
- [jwt-cpp](https://github.com/Thalhammer/jwt-cpp) (fetched via CMake `FetchContent`)

### Installation

```bash
# 1. Clone the repository
git clone <your-repo-link-here>
cd [your-project-name]

# 2. Install dependencies via MSYS64 pacman (adjust package names as needed)
pacman -S mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-mongo-c-driver
pacman -S mingw-w64-x86_64-libsodium
pacman -S mingw-w64-x86_64-openssl
pacman -S mingw-w64-x86_64-redis

# 3. Configure environment variables
cp .env.example .env
# Fill in your MongoDB URI, Redis config, JWT secret, and Brevo API key

# 4. Build the project
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
cmake --build .

# 5. Run the server
./[your_executable_name]
```

### Environment Variables

Create a `.env` file in the project root with the following keys:

```env
MONGO_URI=mongodb://localhost:27017
JWT_SECRET_KEY=your_generated_secret_key_here
REDIS_HOST=127.0.0.1
REDIS_PORT=6379
BREVO_API_KEY=your_brevo_api_key_here
```

> 🔑 Generate a strong JWT secret with: `openssl rand -hex 32`
>
> ⚠️ **Never commit your `.env` file.** It's already included in `.gitignore`.

---

## 🔌 API Endpoints (Auth Module)

| Method | Endpoint | Description |
|---|---|---|
| `POST` | `/signup` | Register a new user |
| `POST` | `/login` | Verify credentials, trigger OTP |
| `POST` | `/verify-otp` | Verify OTP, issue JWT cookie |
| `GET` | `/verify` | Validate current session token |
| `POST` | `/logout` | Clear session cookie |

> More endpoints (trading, portfolio, market data) will be documented here as they're built.

---

## 🗺️ Roadmap

- [x] Secure signup & password hashing
- [x] Login with credential verification
- [x] OTP-based 2FA via Brevo
- [x] JWT issuance & httpOnly cookie sessions
- [x] Redis-backed OTP session management
- [ ] Refresh token support
- [ ] Order placement & matching engine
- [ ] Real-time market data integration
- [ ] Portfolio & trade history dashboard
- [ ] Rate limiting & abuse protection
- [ ] Deployment setup (Docker / cloud)

---

## 🤝 Contributing

This is currently a solo/learning project, but contributions, suggestions, and code reviews are welcome!

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/your-feature`)
3. Commit your changes (`git commit -m "Add: your feature"`)
4. Push to the branch (`git push origin feature/your-feature`)
5. Open a Pull Request

Please open an issue first for major changes to discuss what you'd like to change.

---

## 📄 License

No license has been chosen yet for this project. Until one is added, all rights are reserved by the author. *(Consider adding an [MIT](https://choosealicense.com/licenses/mit/) or [Apache 2.0](https://choosealicense.com/licenses/apache-2.0/) license once ready.)*

---

## 📬 Contact

Have questions or suggestions? Feel free to open an issue on this repository.

<div align="center">

**[GitHub Repo Link Here]**

</div>

