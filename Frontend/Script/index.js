/* --- TICKER DATA & GENERATION --- */
        const tickerData = [
            { symbol: "AAPL", price: "175.20", change: "+1.2%", up: true },
            { symbol: "TSLA", price: "180.55", change: "-2.4%", up: false },
            { symbol: "NVDA", price: "875.12", change: "+5.8%", up: true },
            { symbol: "BTC", price: "67,420", change: "+0.9%", up: true },
            { symbol: "AMZN", price: "178.15", change: "-0.4%", up: false },
            { symbol: "MSFT", price: "420.22", change: "+1.1%", up: true }
        ];

        const tickerTape = document.getElementById('ticker-tape');
        
        // Duplicate data to make the infinite scroll smooth
        const doubleData = [...tickerData, ...tickerData, ...tickerData]; 
        doubleData.forEach(item => {
            const div = document.createElement('div');
            div.className = 'ticker-item';
            div.innerHTML = `${item.symbol} <span style="color:#fff; margin-left:5px;">$${item.price}</span> <span class="${item.up ? 'up' : 'down'}">${item.change}</span>`;
            tickerTape.appendChild(div);
        });

        /* --- REAL-TIME CANVAS STOCK CHART ANIMATION --- */
        const canvas = document.getElementById('stockChart');
        const ctx = canvas.getContext('2d');

        // Resize canvas correctly for high-res displays
        function resizeCanvas() {
            canvas.width = canvas.parentElement.clientWidth - 48;
            canvas.height = 250;
        }
        resizeCanvas();
        window.addEventListener('resize', resizeCanvas);

        let dataPoints = Array.from({length: 40}, () => 120 + Math.random() * 60);
        let basePrice = 342.50;

        function drawChart() {
            ctx.clearRect(0, 0, canvas.width, canvas.height);
            
            const widthBetweenPoints = canvas.width / (dataPoints.length - 1);
            
            // Create Gradient Fill below the line
            let gradient = ctx.createLinearGradient(0, 0, 0, canvas.height);
            gradient.addColorStop(0, 'rgba(0, 230, 118, 0.25)');
            gradient.addColorStop(1, 'rgba(0, 230, 118, 0.0)');

            // Draw Area
            ctx.beginPath();
            ctx.moveTo(0, canvas.height);
            for (let i = 0; i < dataPoints.length; i++) {
                ctx.lineTo(i * widthBetweenPoints, dataPoints[i]);
            }
            ctx.lineTo(canvas.width, canvas.height);
            ctx.closePath();
            ctx.fillStyle = gradient;
            ctx.fill();

            // Draw Trend Line
            ctx.beginPath();
            ctx.moveTo(0, dataPoints[0]);
            for (let i = 0; i < dataPoints.length; i++) {
                ctx.lineTo(i * widthBetweenPoints, dataPoints[i]);
            }
            ctx.strokeStyle = '#00e676';
            ctx.lineWidth = 3;
            ctx.lineJoin = 'round';
            ctx.lineCap = 'round';
            ctx.stroke();

            // Draw Grid Lines (To make it look realistic)
            ctx.strokeStyle = 'rgba(255, 255, 255, 0.03)';
            ctx.lineWidth = 1;
            for(let i = 1; i < 5; i++) {
                ctx.beginPath();
                ctx.moveTo(0, (canvas.height / 5) * i);
                ctx.lineTo(canvas.width, (canvas.height / 5) * i);
                ctx.stroke();
            }
        }

        // Live Market Simulation Tick
        setInterval(() => {
            // Calculate simulated price movement
            let change = (Math.random() - 0.48) * 2; // slight upward bias
            basePrice += change;
            
            // Update Text Elements
            const priceEl = document.getElementById('live-price');
            priceEl.innerText = `$${basePrice.toFixed(2)}`;
            priceEl.style.color = change >= 0 ? '#00e676' : '#ff3d00';
            
            setTimeout(() => { priceEl.style.color = '#ffffff'; }, 300);

            // Shift chart data points to simulate movement
            let lastPoint = dataPoints[dataPoints.length - 1];
            let newPoint = lastPoint - (change * 5); // scale fluctuation to pixel space
            
            // Keep graph within canvas bounds
            if(newPoint < 20) newPoint = 20;
            if(newPoint > canvas.height - 20) newPoint = canvas.height - 20;

            dataPoints.shift();
            dataPoints.push(newPoint);

            drawChart();
        }, 800);

        // Initial paint
        drawChart();