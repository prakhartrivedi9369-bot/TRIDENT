(function(){
  // ---------- Ticker tape content ----------
  const symbols = [
    {s:'VLT', v:'128.42', d:'up'}, {s:'AUTH', v:'56.10', d:'down'},
    {s:'CRYP', v:'340.09', d:'up'}, {s:'SESH', v:'12.88', d:'up'},
    {s:'HASH', v:'204.55', d:'down'}, {s:'TOKN', v:'88.71', d:'up'},
    {s:'2FA', v:'19.03', d:'up'}, {s:'NODE', v:'451.20', d:'down'},
    {s:'SSL', v:'77.64', d:'up'}, {s:'GRID', v:'163.29', d:'up'}
  ];
  function buildTicker(el){
    let html = '';
    for (let i=0;i<3;i++){
      symbols.forEach(item=>{
        const arrow = item.d === 'up' ? '▲' : '▼';
        html += `<span>${item.s} <span class="${item.d}">${arrow} ${item.v}</span></span>`;
      });
    }
    el.innerHTML = html;
  }
  buildTicker(document.getElementById('tickerRow1'));
  buildTicker(document.getElementById('tickerRow2'));

  // ---------- Elements ----------
  const pwInput = document.getElementById('newPassword');
  const confirmInput = document.getElementById('confirmPassword');
  const pwWrap = document.getElementById('pwWrap');
  const confirmWrap = document.getElementById('confirmWrap');
  const matchHint = document.getElementById('matchHint');
  const submitBtn = document.getElementById('submitBtn');
  const btnText = document.getElementById('btnText');
  const spinner = document.getElementById('spinner');
  const strengthValue = document.getElementById('strengthValue');
  const form = document.getElementById('resetForm');
  const successPanel = document.getElementById('successPanel');
  const card = document.getElementById('card');

  // ---------- Password visibility toggles ----------
  document.querySelectorAll('.toggle-vis').forEach(btn=>{
    btn.addEventListener('click', ()=>{
      const target = document.getElementById(btn.dataset.target);
      const isPw = target.type === 'password';
      target.type = isPw ? 'text' : 'password';
      btn.style.color = isPw ? 'var(--green)' : '';
    });
  });

  // ---------- Strength scoring ----------
  function scorePassword(pw){
    const rules = {
      len: pw.length >= 8,
      case: /[a-z]/.test(pw) && /[A-Z]/.test(pw),
      num: /[0-9]/.test(pw),
      sym: /[^A-Za-z0-9]/.test(pw)
    };
    let score = 0;
    if (rules.len) score += 25;
    if (rules.case) score += 25;
    if (rules.num) score += 25;
    if (rules.sym) score += 25;
    if (pw.length >= 12 && score === 100) score = 100;
    else if (pw.length > 8) score = Math.min(100, score + Math.min(10, pw.length - 8));
    return { score, rules };
  }

  // ---------- Canvas "Security Index" chart ----------
  const canvas = document.getElementById('strengthChart');
  const ctx = canvas.getContext('2d');
  let dpr = window.devicePixelRatio || 1;
  let history = [0];

  function resizeCanvas(){
    const rect = canvas.getBoundingClientRect();
    canvas.width = rect.width * dpr;
    canvas.height = rect.height * dpr;
    ctx.setTransform(dpr,0,0,dpr,0,0);
  }
  resizeCanvas();
  window.addEventListener('resize', resizeCanvas);

  function colorForScore(score){
    if (score >= 75) return '#12b981';
    if (score >= 50) return '#e8a94c';
    if (score > 0) return '#e5484d';
    return '#4a5a54';
  }

  function drawChart(){
    const w = canvas.getBoundingClientRect().width;
    const h = canvas.getBoundingClientRect().height;
    ctx.clearRect(0,0,w,h);

    // gridlines
    ctx.strokeStyle = 'rgba(255,255,255,0.05)';
    ctx.lineWidth = 1;
    for(let gy=0; gy<=2; gy++){
      const y = (h/2)*gy;
      ctx.beginPath();
      ctx.moveTo(0,y+0.5);
      ctx.lineTo(w,y+0.5);
      ctx.stroke();
    }

    if (history.length < 2) return;

    const maxPoints = 40;
    const pts = history.slice(-maxPoints);
    const stepX = w / (maxPoints - 1);
    const startX = w - (pts.length - 1) * stepX;

    const currentScore = pts[pts.length-1];
    const lineColor = colorForScore(currentScore);

    // area fill
    ctx.beginPath();
    pts.forEach((val,i)=>{
      const x = startX + i*stepX;
      const y = h - (val/100)*h*0.88 - 3;
      if (i===0) ctx.moveTo(x,y); else ctx.lineTo(x,y);
    });
    ctx.lineTo(startX + (pts.length-1)*stepX, h);
    ctx.lineTo(startX, h);
    ctx.closePath();
    const grad = ctx.createLinearGradient(0,0,0,h);
    grad.addColorStop(0, lineColor + '33');
    grad.addColorStop(1, lineColor + '00');
    ctx.fillStyle = grad;
    ctx.fill();

    // line
    ctx.beginPath();
    pts.forEach((val,i)=>{
      const x = startX + i*stepX;
      const y = h - (val/100)*h*0.88 - 3;
      if (i===0) ctx.moveTo(x,y); else ctx.lineTo(x,y);
    });
    ctx.strokeStyle = lineColor;
    ctx.lineWidth = 2;
    ctx.lineJoin = 'round';
    ctx.stroke();

    // last point dot
    const lastX = startX + (pts.length-1)*stepX;
    const lastY = h - (pts[pts.length-1]/100)*h*0.88 - 3;
    ctx.beginPath();
    ctx.arc(lastX, lastY, 3.2, 0, Math.PI*2);
    ctx.fillStyle = lineColor;
    ctx.shadowColor = lineColor;
    ctx.shadowBlur = 8;
    ctx.fill();
    ctx.shadowBlur = 0;
  }

  function labelForScore(score){
    if (score === 0) return '— idle';
    if (score < 50) return score + ' · weak';
    if (score < 75) return score + ' · fair';
    if (score < 100) return score + ' · strong';
    return '100 · excellent';
  }

  drawChart();

  // ---------- Validation wiring ----------
  function updateTags(rules){
    document.querySelectorAll('.tag').forEach(tag=>{
      const rule = tag.dataset.rule;
      tag.classList.toggle('met', !!rules[rule]);
    });
  }

  function checkMatch(){
    if (!confirmInput.value){
      matchHint.textContent = '';
      confirmWrap.classList.remove('error','ok');
      return false;
    }
    if (confirmInput.value === pwInput.value){
      matchHint.textContent = '✓ Passwords match';
      matchHint.className = 'hint ok';
      confirmWrap.classList.remove('error');
      confirmWrap.classList.add('ok');
      return true;
    } else {
      matchHint.textContent = 'Passwords do not match';
      matchHint.className = 'hint err';
      confirmWrap.classList.remove('ok');
      confirmWrap.classList.add('error');
      return false;
    }
  }

  function evaluateForm(){
    const { score, rules } = scorePassword(pwInput.value);
    strengthValue.textContent = labelForScore(score);
    strengthValue.style.color = colorForScore(score);
    updateTags(rules);

    const last = history[history.length-1];
    if (last !== score){
      history.push(score);
      if (history.length > 40) history.shift();
    }
    drawChart();

    const allRulesMet = Object.values(rules).every(Boolean);
    pwWrap.classList.toggle('ok', allRulesMet && pwInput.value.length > 0);
    pwWrap.classList.toggle('error', pwInput.value.length > 0 && !allRulesMet && pwInput.value.length >= 8);

    const matches = checkMatch();
    submitBtn.disabled = !(allRulesMet && matches);
  }

  pwInput.addEventListener('input', evaluateForm);
  confirmInput.addEventListener('input', evaluateForm);

  // ---------- Submission ----------
  async function handleSubmit(event){
    event.preventDefault();
    if (submitBtn.disabled) return;

    btnText.style.opacity = '0.6';
    btnText.textContent = 'Updating password...';
    spinner.style.display = 'inline-block';
    submitBtn.style.pointerEvents = 'none';
    pwInput.disabled = true;
    confirmInput.disabled = true;

    try {
      const reset_token=sessionStorage.getItem("reset_token");
      const response = await fetch('/api/Pass-reset', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({
          password: pwInput.value,
          confirmPassword: confirmInput.value,
          reset_token: reset_token
          // include your reset token here, e.g. token: new URLSearchParams(location.search).get('token')
        })
      });

      const data = await response.json().catch(()=>({}));

      if (data.status == "Success"){
        btnText.textContent = 'Password updated';
        spinner.style.display = 'none';
        submitBtn.style.boxShadow = '0 8px 24px -6px var(--green-glow)';
        setTimeout(()=>{ successPanel.classList.add('show'); }, 300);
        alert(data.message);
        window.location.href = "/login";
      }
      else 
      {
        btnText.textContent = 'Try again';
        spinner.style.display = 'none';
        submitBtn.style.pointerEvents = 'auto';
        submitBtn.classList.add('error-state');
        pwInput.disabled = false;
        confirmInput.disabled = false;
        btnText.style.opacity = '1';
        matchHint.textContent = data.message || 'Could not update password. Please try again.';
        matchHint.className = 'hint err';
        setTimeout(()=> submitBtn.classList.remove('error-state'), 1800);
      }
    } catch (err){
      console.error('Password update failed:', err);
      btnText.textContent = 'Network error — retry';
      spinner.style.display = 'none';
      submitBtn.style.pointerEvents = 'auto';
      submitBtn.classList.add('error-state');
      pwInput.disabled = false;
      confirmInput.disabled = false;
      btnText.style.opacity = '1';
      setTimeout(()=> submitBtn.classList.remove('error-state'), 1800);
    }
  }

  form.addEventListener('submit', handleSubmit);
})();