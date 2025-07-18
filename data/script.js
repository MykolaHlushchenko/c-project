// Get informaion about program
function aboutProgram() {
  alert("ESP32 Program for sensor HC-SR04 and step motor 28BYJ-40.");
}

// Start and stop motor
function toggleMotor(isChecked) {
    fetch("/motor?value=" + (isChecked ? "on" : "off"))
        .then(res => {
          if (!res.ok) throw new Error("Server Error!");
        })
        .catch(err => alert(err)
    );
}

// Draw circles with differnces radis
function drawCircles(context) {
    let R = 0;
    for (let count = 0; count < 6; count++) {
        context.beginPath();
        context.arc(300, 300, R+=50, 0, 2*Math.PI);
        context.strokeStyle = '#7e807d';
        context.stroke();
    }
}

// Initialize canvas
function initializeCanvas() {
    const canvas = document.getElementById("myCanvas");
    const context = canvas.getContext("2d");
    context.beginPath();
    context.arc(300, 300, 300, 0, 2*Math.PI);
    context.stroke();
}

// Draw distance range
function drawDistance(context) {
    let right = 0;
    let left = 300;
    let steps = 6;
    // draw zero position	
    context.beginPath();
    context.fillStyle = "#7e807d";
    context.font = "10px Arial";
    context.fillText(0, 300, 320);
    
    for (let count = 0; count < steps; count++) {  
        context.beginPath();
        context.fillText(left, right, 320);
        left-=50;
        
        context.beginPath();
        context.fillText(right+=50, right + 300, 320);
    }
}
// Draw angles for scan from 0 to 180 deg 
function drawAngles(context) {
    let angles = [0, 30, 60, 90, 120, 150, 180];
    
    for (count = 0; count < angles.length; count++) {
        let angleRad = angles[count] * Math.PI/180;
        
    	let x = 300 + 320 * Math.cos(angleRad);
    	let y = 300 - 320 * Math.sin(angleRad);
        
        let x1 = 300 + 250 * Math.cos(angleRad);
    	let y1 = 300 - 250 * Math.sin(angleRad);
 		
        context.beginPath();
        context.moveTo(x, y);
    	context.lineTo(300, 300);
    	context.lineWidth = 1;
    	context.strokeStyle = '#7e807d';
    	context.stroke();
        
        context.beginPath();
        context.fillStyle = "#7e807d";
        context.fillText(angles[count] + String.fromCharCode(176), x1, y1);
    }		
}

// Draw Distance and Angle values
function drawCurrentValues(context, distance, angle) {
    context.beginPath();
    context.fillStyle = "#7e807d";
    context.font = "20px sans-serif";
    context.fillText("Distance: " + distance + " cm", 10, 20);

    context.beginPath();
    context.fillStyle = "#7e807d";
    context.font = "20px sans-serif";
    context.fillText("Angle: " + angle + String.fromCharCode(176), 10, 40);
}

// Draw line with dot to distance
function drawLineScanner (context, distance, angle) {
    let angleRad = angle * Math.PI/180;
    
    let x = 300 + distance * Math.cos(angleRad);
    let y = 300 - distance * Math.sin(angleRad);

    // Draw dot based on distance
    context.beginPath();
    context.arc(x, y, 4, 0, 2 * Math.PI);
    context.fillStyle = '#8aade6';
    context.fill();

    let x1 = 300 + 300 * Math.cos(angleRad);
    let y1 = 300 - 300 * Math.sin(angleRad);

    context.beginPath();
    context.moveTo(x, y);
    context.lineTo(x1, y1);
    context.lineWidth = 2;
    context.strokeStyle = 'red';
    context.stroke();

    context.beginPath();
    context.moveTo(300, 300);
    context.lineTo(x, y);
    context.lineWidth = 2;
    context.strokeStyle = 'green';
    context.stroke();
}


// Ubdate canvas area
async function updateDistance() {
    const response = await fetch('/data');
    const data = await response.json();

    let distance = data.value;
    let angle = data.angle;
    
    const canvas = document.getElementById("myCanvas");
    const context = canvas.getContext("2d");
    
    context.fillStyle = "rgba(0, 0, 0, 0.05)";
    context.fillRect(0, 0, canvas.width, canvas.height);
    
    drawCircles(context);
    drawAngles(context);
    
    drawLineScanner(context, distance, angle);
    drawCurrentValues(context, distance, angle);
    drawDistance(context);
}

addEventListener("load", initializeCanvas);
setInterval(updateDistance, 50);
