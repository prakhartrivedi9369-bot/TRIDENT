// Password Strength Real-time Interactive Feature
        const passwordInput = document.getElementById('password');
        const strengthMeter = document.getElementById('strengthMeter');
        const strengthBar = document.getElementById('strengthBar');

        passwordInput.addEventListener('input', () => {
            const val = passwordInput.value;
            if (val.length === 0) {
                strengthMeter.style.display = 'none';
                return;
            }
            
            strengthMeter.style.display = 'block';
            let score = 0;

            if (val.length >= 8) score++;
            if (/[A-Z]/.test(val)) score++;
            if (/[0-9]/.test(val)) score++;
            if (/[^A-Za-z0-9]/.test(val)) score++;

            // Dynamic color shift based on strength
            if (score <= 1) {
                strengthBar.style.width = '30%';
                strengthBar.style.backgroundColor = '#ff5252'; // Weak
            } else if (score === 2 || score === 3) {
                strengthBar.style.width = '60%';
                strengthBar.style.backgroundColor = '#ffb300'; // Medium
            } else {
                strengthBar.style.width = '100%';
                strengthBar.style.backgroundColor = '#00e676'; // Strong (Bullish green)
            }
        });

        // Clean Canvas Ripple Effect on Button Click
        const btn = document.getElementById('submitBtn');
        btn.addEventListener('click', function(e) {
            // Prevent dynamic form submission logic break during demo
            if(!document.getElementById('signupForm').checkValidity()) return;
            
            let x = e.clientX - e.target.getBoundingClientRect().left;
            let y = e.clientY - e.target.getBoundingClientRect().top;
            
            let ripple = document.createElement('span');
            ripple.classList.add('ripple');
            ripple.style.left = x + 'px';
            ripple.style.top = y + 'px';
            this.appendChild(ripple);
            
            setTimeout(() => {
                ripple.remove();
            }, 600);
        });

        // Smooth simulated submit action
        document.getElementById('signupForm').addEventListener('submit',async (e) => {
            console.log("FORM SUBMITTED");
            e.preventDefault();

            const username = document.getElementById('username').value;
            const email = document.getElementById('email').value;
            const password = document.getElementById('password').value;

            try
            {
                console.log("FETCH STARTING");
                const response = await fetch('/api/signup',{
                     
                        method: 'POST',
                        headers: {
                            'Content-Type':'application/json'
                        },
                        body: JSON.stringify({
                             username: username,
                             email: email,
                             password: password
                        })
                });

                const data = await response.json();

                if(data.status === "otp_required")
                {
                    localStorage.setItem("userEmail", email);
                    

                      // Call send-otp route
                    await fetch('/send-otp', {
                          method: 'POST',
                          headers: {
                                     'Content-Type': 'application/json'
                                   },
                          body: JSON.stringify({
                          email: email,
                          usecase:"SIGNUP"
                        })
                    });

                    alert("OTP sent to entered email Successfully");

                    window.location.href = "/otp";
                }
                else
                {
                    alert(data.message);
                }
            }
            catch(error)
            {
                console.error(error);
                alert("Server connection failed");
            }
            btn.style.transform = 'scale(0.98)';
            btn.innerHTML = '<span>Connecting to Secure Node...</span>';
         
        });