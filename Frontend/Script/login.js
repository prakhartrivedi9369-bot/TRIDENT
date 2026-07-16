// Password Hide/Show
function togglePass() {

    const passwordInput = document.getElementById('password');
    const toggleIcon = document.querySelector('.toggle-password');

    if (passwordInput.type === 'password') {

        passwordInput.type = 'text';

        toggleIcon.classList.remove('fa-eye-slash');
        toggleIcon.classList.add('fa-eye');

    } else {

        passwordInput.type = 'password';

        toggleIcon.classList.remove('fa-eye');
        toggleIcon.classList.add('fa-eye-slash');
    }
}


// Login Form Handling
async function handleLogin(event) {

    event.preventDefault();

    const submitBtn = document.getElementById('submitBtn');

    const email = document.getElementById('email').value.trim();

    const password = document.getElementById('password').value.trim();

    // Validation
    if (!email || !password) {

        alert("Please fill all fields");
        return;
    }

    // Loading Start
    submitBtn.classList.add('loading');

    submitBtn.disabled = true;

    try {

        const response = await fetch('/api/login', {

            method: 'POST',

            headers: {
                'Content-Type': 'application/json'
            },

            body: JSON.stringify({
                email: email,
                password: password
            })
        });

        const result = await response.json();

        console.log(result);

        // Loading Stop
        submitBtn.classList.remove('loading');

        submitBtn.disabled = false;

        // SUCCESS LOGIN
        if(result.status === "Invalid_pass")
        {
            alert("Invalid Password");
        }
        else if (result.status === "otp_required") 
        {
            submitBtn.style.background = '#00e676';

            submitBtn.innerHTML =
                '<span>Success <i class="fa-solid fa-check"></i></span>';


            // Small delay for animation
            setTimeout(() => {
                // Redirect to OTP page
                localStorage.setItem("userEmail", email);
                window.location.href = "/otp";

            }, 1000);

              // Call send-otp route
             await fetch('/send-otp', {
             method: 'POST',
             headers: {
                       'Content-Type': 'application/json'
                      },
                      body: JSON.stringify({
                      email: email
                   })
             });

            alert("OTP sent to registered email successfully");
        }
         else
             {

            alert(result.message || "Invalid Login");
        }

    } catch (error) {

        console.error(error);

        submitBtn.classList.remove('loading');

        submitBtn.disabled = false;

        alert("Server Error");
    }
}


// Dynamic Hover Effect
const container = document.querySelector('.login-container');

document.addEventListener('mousemove', (e) => {

    if (!container) return;

    let xAxis = (window.innerWidth / 2 - e.pageX) / 45;

    let yAxis = (window.innerHeight / 2 - e.pageY) / 45;

    container.style.transform =
        `rotateY(${xAxis}deg) rotateX(${yAxis}deg)`;
});


// Reset Position
document.addEventListener('mouseleave', () => {

    if (!container) return;

    container.style.transform =
        `rotateY(0deg) rotateX(0deg)`;
});