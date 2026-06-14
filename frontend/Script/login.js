// 1. Password Hide/Show functionality
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

// 2. Login form handling
async function handleLogin(event) {
    event.preventDefault(); // Default page refresh rokne ke liye
    
    const submitBtn = document.getElementById('submitBtn');
    const email=document.getElementById('email').value;
    const password = document.getElementById('password').value;
    
    // Loading state active karna
    submitBtn.classList.add('loading');
    submitBtn.disabled = true;

    try
    {
        const response = await fetch('/api/login',
            {
                method:'POST',
                headers:
                {
                    'Content-Type':'application/json'
                },
                body:JSON.stringify(
                {
                    email: email,
                    password:password
                })
            });
        const result = await response.text();
        submitBtn.classList.remove('loading');
        submitBtn.disabled=false;
        
        if(response.ok)
        {
            submitBtn.style.background='#00e676';
            submitBtn.innerHTML='<span>Success<i class="fa-solid fa-check"></i></span>';
            alert('Login Successful');

            //Future dashboard redirect
            //window.location.href="/ dashboard";
        }
        else
        {
            alert(result);
        }
    }
    catch(error)
    {
        console.error(error);
        submitBtn.classList.remove('loading');
        submitBtn.disabled=false;
        alert('Server Error');
    }
}

// 3. Dynamic hover effect
const container = document.querySelector('.login-container');
document.addEventListener('mousemove', (e) => {
    let xAxis = (window.innerWidth / 2 - e.pageX) / 45;
    let yAxis = (window.innerHeight / 2 - e.pageY) / 45;
    container.style.transform = `rotateY(${xAxis}deg) rotateX(${yAxis}deg)`;
});

// Reset positioning when mouse leaves window area
document.addEventListener('mouseleave', () => {
    container.style.transform = `rotateY(0deg) rotateX(0deg)`;
});