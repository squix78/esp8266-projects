var lastMove = 0;
function move(left, right) {
  var now = Date.now();
  if (lastMove + 200 < now) {
     lastMove = now; 
     var request = new XMLHttpRequest();
     request.open('GET', '/engines/' + Math.round(left) + "," + Math.round(right), true);
     request.send(null);
  }
}
document.onkeydown = function detectKey(event) {
    var e = event.keyCode;
    if (e==87){ move(600, 600);}
    if (e==83){ move(600, -600);}
    if (e==65){ move(-600, 600);}
    if (e==68){ move(-600, -600);}
}
if (window.DeviceMotionEvent) {
  window.addEventListener('devicemotion', deviceMotionHandler, false);
} else {
  document.getElementById("dmEvent").innerHTML = "Accelerometer not supported."
}
function deviceMotionHandler(eventData) {
  acceleration = eventData.accelerationIncludingGravity;
  var left = 0;
  var right = 0;
  if (Math.abs(acceleration.y) > 1) {
    var speed = acceleration.y * 123;
    left = Math.min(1023, speed + acceleration.x * 100);
    right = Math.min(1023, speed - acceleration.x * 100);
  } else if (Math.abs(acceleration.x) > 1) {
    var speed = Math.min(1023, Math.abs(acceleration.x) * 123);
    if (acceleration.x > 0) {
      left = speed;
      right = -speed; 
    } else {
      left = -speed;
      right = speed;
    }
  }
  if (Math.abs(left) > 100 || Math.abs(right) > 100) {
    move(left, right);
  }
  var direction = "stop";
  direction = "[" + Math.round(acceleration.x) + "," + Math.round(acceleration.y) + "," + Math.round(acceleration.z) + "]<BR/>" + Math.round(left) + ", " + Math.round(right); 
  document.getElementById("vector").innerHTML =direction;
}
