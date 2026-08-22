async function handleRecovery(event) {
    event.preventDefault();

    const btnText = document.querySelector('.btn-text');
    const loader = document.getElementById('btnLoader');
    const submitBtn = document.getElementById('submitBtn');
    const emailInput = document.getElementById('email');
    const email = emailInput.value.trim();

    // Basic client-side validation
    if (!email) {
        alert('Please enter your email address.');
        return;
    }

    // Loading state
    btnText.style.opacity = '0.5';
    btnText.innerText = 'Verifying email...';
    loader.style.display = 'block';
    submitBtn.style.pointerEvents = 'none';
    emailInput.disabled = true;

    try {
        const response = await fetch('/api/forget', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ email })
        });

        const data = await response.json();

        if (data.status === "otp_required") {

            btnText.innerText = 'Sending OTP...';
            loader.style.display = 'none';
            submitBtn.style.background = 'var(--market-green)';
            submitBtn.style.boxShadow = '0 0 25px rgba(16, 185, 129, 0.6)';
            emailInput.style.borderColor = 'var(--market-green)';

            // Small delay for animation
            setTimeout(async () => {

                const usecase = "FORGET_PASSWORD";

                // Store for OTP page
                localStorage.setItem("userEmail", email);
              //  sessionStorage.setItem("usecase", usecase);

                try {

                    const otpResponse = await fetch('/send-otp', {
                        method: 'POST',
                        headers: {
                            'Content-Type': 'application/json'
                        },
                        body: JSON.stringify({
                            email: email,
                            usecase: "FORGET_PASSWORD"
                        })
                    });

                    const result = await otpResponse.json();

                    if (result.success) {
                        sessionStorage.setItem("otp_send_count", "1");
                        sessionStorage.setItem("otp_last_send_time", String(Date.now()));
                        window.location.href = "/otp";
                    } else {
                        alert(result.message);

                        btnText.innerText = 'Try Again';
                        submitBtn.style.pointerEvents = 'auto';
                        emailInput.disabled = false;
                    }

                } catch (err) {
                    console.error(err);

                    alert("Failed to send OTP.");

                    btnText.innerText = 'Try Again';
                    submitBtn.style.pointerEvents = 'auto';
                    emailInput.disabled = false;
                }

            }, 1000);

        } else {

            btnText.innerText = 'Try Again';
            loader.style.display = 'none';
            submitBtn.style.pointerEvents = 'auto';
            emailInput.disabled = false;
            emailInput.style.borderColor = 'red';

            alert(data.message);
        }

    } catch (err) {

        console.error('Password reset request failed:', err);

        btnText.innerText = 'Try Again';
        loader.style.display = 'none';
        submitBtn.style.pointerEvents = 'auto';
        emailInput.disabled = false;
        emailInput.style.borderColor = 'red';

        alert('Network error. Please check your connection and try again.');

    } finally {
        btnText.style.opacity = '1';
    }
}