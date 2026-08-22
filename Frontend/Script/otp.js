const inputs = document.querySelectorAll(".otp-input");
const form = document.getElementById("otpForm");
const statusEl = document.getElementById("status");
const verifyBtn = document.getElementById("verifyBtn");
const resendBtn = document.getElementById("resendBtn");
const toggleBtn = document.getElementById("toggleBtn");

let hidden = false;
let attempts = 0;

/* FRONTEND-ONLY SEND LIMITING CONFIG */
const MAX_SEND_ATTEMPTS = 3;
const COOLDOWN_SECONDS = 60;

const SEND_COUNT_KEY = "otp_send_count";
const LAST_SEND_TIME_KEY = "otp_last_send_time";

/* ===========================
   OTP INPUT HANDLING
=========================== */

inputs.forEach((input, index) => {

    input.addEventListener("input", () => {

        input.value = input.value.replace(/\D/g, "");

        if (input.value && index < inputs.length - 1) {
            inputs[index + 1].focus();
        }

        input.classList.remove("error");
        clearStatus();
    });

    input.addEventListener("keydown", (e) => {

        if (e.key === "Backspace" && !input.value && index > 0) {
            inputs[index - 1].focus();
        }

        if (e.key === "ArrowLeft" && index > 0) {
            inputs[index - 1].focus();
        }

        if (e.key === "ArrowRight" && index < inputs.length - 1) {
            inputs[index + 1].focus();
        }
    });

    input.addEventListener("paste", (e) => {

        e.preventDefault();

        const pasted = (e.clipboardData || window.clipboardData)
            .getData("text")
            .replace(/\D/g, "")
            .slice(0, 6);

        pasted.split("").forEach((digit, i) => {
            if (inputs[i]) {
                inputs[i].value = digit;
            }
        });

        if (pasted.length < inputs.length) {
            inputs[pasted.length].focus();
        } else {
            inputs[inputs.length - 1].focus();
        }

    });

});

/* ===========================
   SHOW / HIDE OTP
=========================== */

toggleBtn.addEventListener("click", () => {

    hidden = !hidden;

    inputs.forEach(input => {
        input.type = hidden ? "password" : "text";
    });

    toggleBtn.textContent = hidden ? "Show Digits" : "Hide Digits";

});

/* ===========================
   SEND LIMIT HELPERS
=========================== */

function getSendCount() {
    return parseInt(sessionStorage.getItem(SEND_COUNT_KEY) || "0", 10);
}

function incrementSendCount() {

    const count = getSendCount() + 1;
    sessionStorage.setItem(SEND_COUNT_KEY, String(count));
    return count;

}

function hasReachedMaxAttempts() {
    return getSendCount() >= MAX_SEND_ATTEMPTS;
}

function markOtpSent() {
    sessionStorage.setItem(LAST_SEND_TIME_KEY, String(Date.now()));
}

function getRemainingCooldown() {

    const lastSend = parseInt(sessionStorage.getItem(LAST_SEND_TIME_KEY) || "0", 10);

    if (!lastSend) return 0;

    const elapsed = Math.floor((Date.now() - lastSend) / 1000);

    return Math.max(0, COOLDOWN_SECONDS - elapsed);

}

function lockResendPermanently(message) {

    resendBtn.disabled = true;
    resendBtn.textContent = "Resend Unavailable";

    showStatus(
        message || "Maximum resend attempts reached.",
        "error"
    );

}

/* ===========================
   VERIFY OTP
=========================== */

form.addEventListener("submit", async (e) => {

    e.preventDefault();

    const code = [...inputs].map(i => i.value).join("");
    const email = localStorage.getItem("userEmail");

    if (code.length !== 6) {

        showStatus("Please enter all 6 digits.", "error");

        inputs.forEach(input => {
            if (!input.value)
                input.classList.add("error");
        });

        return;
    }

    verifyBtn.disabled = true;

    showStatus("Verifying code...", "loading");

    await wait(1500);

    try {

        const response = await fetch("/verify-otp", {

            method: "POST",

            headers: {
                "Content-Type": "application/json"
            },

            body: JSON.stringify({

                email: email,
                otp: code

            }),
            credentials: 'include'
        });

        const result = await response.json();

        alert(result.message);

        if (result.success)
        {
            if(result.usecase == "FORGET_PASSWORD")
            {
               verifyBtn.disabled = false;

               sessionStorage.setItem("reset_token",result.reset_token);

               window.location.href = "/Pass_reset";

               return;
            }

            if(result.usecase == "LOGIN")
            {
                verifyBtn.disabled = false;

                const response2 = await fetch("/JWT_check", {

                method: "POST",

                headers: {
                    "Content-Type": "application/json"
                },

                body: JSON.stringify({

                    email: email
                }),
                    credentials: 'include'
                });

                const result2 = await response2.json();

                if(result2.success)
                {            
                    window.location.href = "/Front_page";
                }
                else
                {
                    alert(result2.message);
                }
            }

            if(result.usecase == "SIGNUP")
            {
                verifyBtn.disabled = false;

                const response2 = await fetch("/JWT_check", {

                method: "POST",

                headers: {
                    "Content-Type": "application/json"
                },

                body: JSON.stringify({

                    email: email
                }),
                    credentials: 'include'
                });

                const result2 = await response2.json();

                if(result2.success)
                {            
                    window.location.href = "/Front_page";
                }
                else
                {
                    alert(result2.message);
                }
            }
        }

        if (result.code === "WRONG_OTP") {
            alert("Wrong OTP");
        }

        if (result.code === "OTP_EXPIRED") {

            alert("OTP expired");

            if (!hasReachedMaxAttempts()) {

                resendBtn.disabled = false;
                resendBtn.textContent = "Resend Code";

            } else {

                lockResendPermanently();

            }
        }

        if (result.code === "REDIS_ERROR") {
            alert("Internal Server Error");
        }

        attempts++;

        inputs.forEach(input => {
            input.classList.add("error");
        });

        if (attempts >= 3) {

            showStatus(
                "Too many failed attempts. Please wait 60 seconds.",
                "error"
            );

            startCooldown(COOLDOWN_SECONDS);

            attempts = 0;
        }

    }
    catch (err) {

        console.error(err);

        showStatus(
            "Something went wrong.",
            "error"
        );

    }
    finally {

        verifyBtn.disabled = false;

    }

});

/* ===========================
   COMMON HELPERS
=========================== */

function showStatus(message, type) {

    statusEl.className = "status";

    if (type)
        statusEl.classList.add(type);

    statusEl.textContent = message;

}

function clearStatus() {

    statusEl.textContent = "";
    statusEl.className = "status";

}

function wait(ms) {

    return new Promise(resolve => setTimeout(resolve, ms));

}
/* ===========================
   RESEND COOLDOWN
=========================== */

function startCooldown(seconds) {

    let remaining = seconds;

    resendBtn.disabled = true;
    resendBtn.textContent = `Resend Code (${remaining}s)`;

    const timer = setInterval(() => {

        remaining--;

        if (remaining <= 0) {

            clearInterval(timer);

            if (hasReachedMaxAttempts()) {

                lockResendPermanently();

            } else {

                resendBtn.disabled = false;
                resendBtn.textContent = "Resend Code";

            }

            return;
        }

        resendBtn.textContent = `Resend Code (${remaining}s)`;

    }, 1000);

}

/* ===========================
   RESEND OTP
=========================== */

resendBtn.addEventListener("click", async () => {

    if (hasReachedMaxAttempts()) {
        lockResendPermanently();
        return;
    }

    const email = localStorage.getItem("userEmail");
    const usecase = sessionStorage.getItem("usecase");

    if (!email || !usecase) {
        showStatus("Session expired. Please restart the process.", "error");
        return;
    }

    startCooldown(COOLDOWN_SECONDS);

    inputs.forEach(input => {
        input.value = "";
        input.classList.remove("error");
    });

    inputs[0].focus();

    showStatus("Sending new verification code...", "loading");

    try {

        const response = await fetch("/send-otp", {

            method: "POST",

            headers: {
                "Content-Type": "application/json"
            },

            body: JSON.stringify({

                email: email,
                usecase: usecase

            })

        });

        const result = await response.json();

        if (result.success) {

            alert(result.message);

            clearStatus();

            markOtpSent();

            const count = incrementSendCount();

            if (count >= MAX_SEND_ATTEMPTS) {
                // Current cooldown finish hone ke baad
                // button permanently disable ho jayega.
            }

        }
        else {

            showStatus(result.message, "error");

        }

    }
    catch (err) {

        console.error(err);

        showStatus(
            "Error connecting to server.",
            "error"
        );

    }

});

/* ===========================
   PAGE LOAD
=========================== */

window.addEventListener("load", () => {

    inputs[0].focus();

    if (hasReachedMaxAttempts()) {

        lockResendPermanently();
        return;

    }

    const remaining = getRemainingCooldown();

    if (remaining > 0) {

        startCooldown(remaining);

    } else {

        resendBtn.disabled = false;
        resendBtn.textContent = "Resend Code";

    }

});