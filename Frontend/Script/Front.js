/* ============ Custom cursor ============ */
const dot = document.getElementById('cursor-dot');
const ring = document.getElementById('cursor-ring');
const label = document.getElementById('cursor-label');
let mx=innerWidth/2,my=innerHeight/2,rx=mx,ry=my;
window.addEventListener('mousemove', e=>{
  mx=e.clientX; my=e.clientY;
  dot.style.left=mx+'px'; dot.style.top=my+'px';
  label.style.left=mx+'px'; label.style.top=my+'px';
});
function raf(){
  rx += (mx-rx)*0.18; ry += (my-ry)*0.18;
  ring.style.left=rx+'px'; ring.style.top=ry+'px';
  requestAnimationFrame(raf);
}
raf();
document.querySelectorAll('.interactive, a, button, tbody tr, .mover-item').forEach(el=>{
  el.addEventListener('mouseenter', ()=>{
    ring.classList.add('hover');
    const t = el.getAttribute('data-label');
    if(t){ label.textContent=t; label.classList.add('show'); }
  });
  el.addEventListener('mouseleave', ()=>{
    ring.classList.remove('hover');
    label.classList.remove('show');
  });
});

/* ============ Boot sequence ============ */
const bootLines = document.getElementById('boot-lines');
const bootBar = document.getElementById('boot-bar-fill');
const bootPct = document.getElementById('boot-pct');
const lines = [
  'Establishing secure session...',
  'Authenticating credentials... <span class="ok">OK</span>',
  'Connecting to exchange feed (NSE / BSE / NASDAQ)...',
  'Syncing portfolio snapshot... <span class="ok">OK</span>',
  'Calibrating APEX AI insight engine...',
  'Loading terminal...'
];
let li=0, pct=0;
function typeNext(){
  if(li<lines.length){
    const d=document.createElement('span');
    d.className='ln'; d.innerHTML=lines[li];
    bootLines.appendChild(d);
    requestAnimationFrame(()=>d.classList.add('show'));
    li++;
    setTimeout(typeNext, 320);
  }
}
typeNext();
const pctInt = setInterval(()=>{
  pct += Math.random()*18+6;
  if(pct>=100){ pct=100; clearInterval(pctInt); }
  bootBar.style.width=pct+'%';
  bootPct.textContent=Math.floor(pct)+'%';
  if(pct===100){
    setTimeout(()=>{
      document.getElementById('boot').classList.add('done');
      document.getElementById('app').classList.add('in');
      startApp();
    }, 260);
  }
}, 260);

/* ============ Reveal on scroll ============ */
function initReveal(){
  const els = document.querySelectorAll('.reveal');
  const io = new IntersectionObserver((entries)=>{
    entries.forEach(en=>{ if(en.isIntersecting){ en.target.classList.add('in'); io.unobserve(en.target);} });
  }, {threshold:0.12});
  els.forEach(el=>io.observe(el));
}

/* ============ Ambient particles ============ */
function initParticles(){
  const c = document.getElementById('particles');
  const ctx = c.getContext('2d');
  let w,h,pts=[];
  function resize(){ w=c.width=innerWidth; h=c.height=Math.min(innerHeight,900); }
  resize(); window.addEventListener('resize', resize);
  const N = Math.min(50, Math.floor(innerWidth/28));
  for(let i=0;i<N;i++){
    pts.push({x:Math.random()*w, y:Math.random()*h, vx:(Math.random()-0.5)*0.15, vy:(Math.random()-0.5)*0.15, r:Math.random()*1.4+0.4});
  }
  function draw(){
    ctx.clearRect(0,0,w,h);
    ctx.fillStyle='rgba(0,255,157,0.55)';
    pts.forEach(p=>{
      p.x+=p.vx; p.y+=p.vy;
      if(p.x<0)p.x=w; if(p.x>w)p.x=0;
      if(p.y<0)p.y=h; if(p.y>h)p.y=0;
      ctx.beginPath(); ctx.arc(p.x,p.y,p.r,0,Math.PI*2); ctx.fill();
    });
    requestAnimationFrame(draw);
  }
  draw();
}
initParticles();

/* ============ Data + simulation ============ */
const SYMBOLS = [
  {sym:'NVDA', name:'Nvidia Corp', price:118.42},
  {sym:'TSLA', name:'Tesla Inc', price:243.71},
  {sym:'AAPL', name:'Apple Inc', price:196.85},
  {sym:'AMZN', name:'Amazon.com', price:178.20},
  {sym:'MSFT', name:'Microsoft', price:412.03},
  {sym:'GOOGL', name:'Alphabet Inc', price:164.55},
  {sym:'META', name:'Meta Platforms', price:487.10},
  {sym:'AMD', name:'AMD Inc', price:142.33},
  {sym:'NFLX', name:'Netflix Inc', price:632.90},
  {sym:'BTC', name:'Bitcoin', price:64230},
  {sym:'ETH', name:'Ethereum', price:3412},
  {sym:'RELI', name:'Reliance Ind', price:2945.6},
  {sym:'TATA', name:'Tata Motors', price:987.4},
  {sym:'INFY', name:'Infosys Ltd', price:1789.2},
];
SYMBOLS.forEach(s=>{ s.change = (Math.random()*4-1.5); s.base=s.price; s.hist=[]; for(let i=0;i<30;i++) s.hist.push(s.price*(1+(Math.random()-0.5)*0.01)); });

function fmt(n, d=2){ return n.toLocaleString('en-US',{minimumFractionDigits:d,maximumFractionDigits:d}); }

/* ---- ticker tape ---- */
function buildTicker(){
  const track = document.getElementById('ticker-track');
  function block(){
    return SYMBOLS.map(s=>`
      <div class="tick-item" data-sym="${s.sym}">
        <span class="sym">${s.sym}</span>
        <span>${fmt(s.price, s.price>1000?1:2)}</span>
        <span class="chg ${s.change>=0?'up':'down'}">${s.change>=0?'▲':'▼'} ${Math.abs(s.change).toFixed(2)}%</span>
      </div>`).join('');
  }
  track.innerHTML = block()+block();
}
buildTicker();

/* ---- sparkline drawer ---- */
function drawSpark(canvas, hist, up){
  const ctx = canvas.getContext('2d');
  const w = canvas.width = canvas.clientWidth*2;
  const h = canvas.height = canvas.clientHeight*2;
  ctx.clearRect(0,0,w,h);
  const min=Math.min(...hist), max=Math.max(...hist);
  const pad=4*2;
  ctx.beginPath();
  hist.forEach((v,i)=>{
    const x = pad + (w-pad*2) * (i/(hist.length-1));
    const y = h-pad - (h-pad*2) * ((v-min)/(max-min||1));
    if(i===0) ctx.moveTo(x,y); else ctx.lineTo(x,y);
  });
  ctx.strokeStyle = up? '#00ff9d':'#ff3b5c';
  ctx.lineWidth=3.2;
  ctx.lineJoin='round';
  ctx.shadowColor = up? 'rgba(0,255,157,.6)':'rgba(255,59,92,.6)';
  ctx.shadowBlur=6;
  ctx.stroke();
}

/* ---- movers ---- */
function buildMovers(){
  const sorted = [...SYMBOLS].sort((a,b)=>b.change-a.change);
  const gainers = sorted.slice(0,4);
  const losers = sorted.slice(-4).reverse();
  const active = [...SYMBOLS].sort(()=>Math.random()-0.5).slice(0,4);
  function row(s){
    return `<div class="mover-item">
      <div class="l"><b>${s.sym}</b><span>${s.name}</span></div>
      <div class="r"><div class="p">${fmt(s.price, s.price>1000?1:2)}</div><div class="c ${s.change>=0?'up':'down'}">${s.change>=0?'+':''}${s.change.toFixed(2)}%</div></div>
    </div>`;
  }
  document.getElementById('gainers').innerHTML = gainers.map(row).join('');
  document.getElementById('losers').innerHTML = losers.map(row).join('');
  document.getElementById('active').innerHTML = active.map(row).join('');
}
buildMovers();

/* ---- watchlist table ---- */
function buildWatchlist(){
  const body = document.getElementById('watch-body');
  body.innerHTML = SYMBOLS.slice(0,8).map(s=>`
    <tr data-sym="${s.sym}">
      <td class="stk-name"><b>${s.sym}</b><span>${s.name}</span></td>
      <td class="price">${fmt(s.price, s.price>1000?1:2)}</td>
      <td class="change ${s.change>=0?'up':'down'}">${s.change>=0?'+':''}${s.change.toFixed(2)}%</td>
      <td><canvas class="spark" width="90" height="30"></canvas></td>
      <td style="color:var(--text-faint)">${(Math.random()*80+10).toFixed(1)}M</td>
    </tr>
  `).join('');
  body.querySelectorAll('tr').forEach(tr=>{
    const sym = tr.getAttribute('data-sym');
    const s = SYMBOLS.find(x=>x.sym===sym);
    drawSpark(tr.querySelector('.spark'), s.hist, s.change>=0);
  });
}
buildWatchlist();

/* ---- hero index chart + number ---- */
const heroCanvas = document.getElementById('hero-chart');
let indexHist = []; for(let i=0;i<60;i++) indexHist.push(24500 + Math.sin(i/6)*180 + (Math.random()-0.5)*60);
let indexVal = indexHist[indexHist.length-1];
let indexStart = 24180;

function drawHeroChart(){
  const ctx = heroCanvas.getContext('2d');
  const dpr = window.devicePixelRatio||1;
  const cw = heroCanvas.clientWidth, ch = heroCanvas.clientHeight;
  heroCanvas.width = cw*dpr; heroCanvas.height = ch*dpr;
  ctx.setTransform(dpr,0,0,dpr,0,0);
  ctx.clearRect(0,0,cw,ch);
  const min = Math.min(...indexHist), max = Math.max(...indexHist);
  const pts = indexHist.map((v,i)=>({
    x: (cw) * (i/(indexHist.length-1)),
    y: ch - 8 - (ch-16) * ((v-min)/(max-min||1))
  }));
  // fill
  const grad = ctx.createLinearGradient(0,0,0,ch);
  grad.addColorStop(0,'rgba(0,255,157,0.28)');
  grad.addColorStop(1,'rgba(0,255,157,0)');
  ctx.beginPath();
  ctx.moveTo(pts[0].x, ch);
  pts.forEach(p=>ctx.lineTo(p.x,p.y));
  ctx.lineTo(pts[pts.length-1].x, ch);
  ctx.closePath();
  ctx.fillStyle = grad;
  ctx.fill();
  // line
  ctx.beginPath();
  pts.forEach((p,i)=> i===0? ctx.moveTo(p.x,p.y): ctx.lineTo(p.x,p.y));
  ctx.strokeStyle = '#00ff9d';
  ctx.lineWidth = 2;
  ctx.shadowColor='rgba(0,255,157,.7)';
  ctx.shadowBlur=10;
  ctx.stroke();
  // end dot
  const last = pts[pts.length-1];
  ctx.beginPath(); ctx.arc(last.x,last.y,4,0,Math.PI*2);
  ctx.fillStyle='#00ff9d'; ctx.shadowBlur=14; ctx.fill();
}

function animateIndexNumber(){
  const el = document.getElementById('index-num');
  const chgEl = document.getElementById('index-chg-text');
  const chgWrap = document.getElementById('index-chg');
  let t=0;
  const target = indexVal;
  function step(){
    t += 0.03;
    const eased = 1-Math.pow(1-Math.min(t,1),3);
    const v = target*eased;
    el.textContent = fmt(v,2);
    const chg = ((v-indexStart)/indexStart*100);
    chgEl.textContent = (chg>=0?'+':'')+chg.toFixed(2)+'%';
    chgWrap.classList.toggle('down', chg<0);
    if(t<1) requestAnimationFrame(step);
  }
  step();
}

/* ============ live update loop ============ */
function liveTick(){
  // index
  indexHist.shift();
  indexVal = indexVal + (Math.random()-0.48)*22;
  indexHist.push(indexVal);
  drawHeroChart();
  const el = document.getElementById('index-num');
  const chgEl = document.getElementById('index-chg-text');
  const chgWrap = document.getElementById('index-chg');
  el.textContent = fmt(indexVal,2);
  const chg = ((indexVal-indexStart)/indexStart*100);
  chgEl.textContent = (chg>=0?'+':'')+chg.toFixed(2)+'%';
  chgWrap.classList.toggle('down', chg<0);

  // random symbol updates
  const s = SYMBOLS[Math.floor(Math.random()*SYMBOLS.length)];
  const delta = (Math.random()-0.5) * s.price * 0.004;
  s.price += delta;
  s.change += delta/s.base*100;
  s.hist.shift(); s.hist.push(s.price);

  // ticker flash
  document.querySelectorAll(`.tick-item[data-sym="${s.sym}"]`).forEach(item=>{
    item.querySelector('.chg').textContent = (s.change>=0?'▲ ':'▼ ')+Math.abs(s.change).toFixed(2)+'%';
    item.querySelector('.chg').className = 'chg '+(s.change>=0?'up':'down');
    item.querySelector('span:nth-child(2)').textContent = fmt(s.price, s.price>1000?1:2);
    item.classList.remove('flash-up','flash-down'); void item.offsetWidth;
    item.classList.add(delta>=0?'flash-up':'flash-down');
  });

  // watchlist row flash
  const row = document.querySelector(`#watch-body tr[data-sym="${s.sym}"]`);
  if(row){
    row.querySelector('.price').textContent = fmt(s.price, s.price>1000?1:2);
    const ce = row.querySelector('.change');
    ce.textContent = (s.change>=0?'+':'')+s.change.toFixed(2)+'%';
    ce.className = 'change '+(s.change>=0?'up':'down');
    drawSpark(row.querySelector('.spark'), s.hist, s.change>=0);
    row.classList.remove('row-flash-up','row-flash-down'); void row.offsetWidth;
    row.classList.add(delta>=0?'row-flash-up':'row-flash-down');
  }
}

function startApp(){
  initReveal();
  animateIndexNumber();
  drawHeroChart();
  document.querySelectorAll('.sector-row .bar i').forEach(i=>{
    setTimeout(()=>{ i.style.width = i.getAttribute('data-w')+'%'; }, 300);
  });
  setInterval(liveTick, 1400);
  setInterval(()=>{
    document.getElementById('clock').textContent = new Date().toLocaleTimeString('en-US',{hour12:false});
  }, 1000);
}

window.addEventListener('resize', ()=>{ drawHeroChart(); buildWatchlist(); });