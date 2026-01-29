const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Servo Control</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      padding: 20px;
      background: lightgrey;
      color: black;
      margin: 0;
    }

    .controls {
      display: flex;
      justify-content: center;
      gap: 20px;
      flex-wrap: nowrap; /* Prevent wrapping */
      align-items: flex-start;
      margin-top: 20px;
    }

    .servo-control {
      margin: 20px 0;
      padding: 5px;
      border-radius: 10px;
      width: 320px;
      box-sizing: border-box;
      display: flex;
      flex-direction: column;
      align-items: center;
    }

    .servo-control h3 {
      margin: 0 0 10px 0;
      width: 100%;
    }

    /* Make buttons stretch to the full width of their container */
    .servo-control button {
      width: 100%;
      box-sizing: border-box;
      margin: 5px 0;
      padding: 10px;
      font-size: 16px;
      display: block;
    }
  </style>
</head>
<body>
  <div class="controls">
    <div class="servo-control">
      <h3>Rudder (Servo 1)</h3>
      <!-- hold to move, release to return to center (90) -->
      <button
        onpointerdown="setRudder(30)"
        onpointerup="resetRudder()"
        onpointercancel="resetRudder()"
        onpointerleave="resetRudder()"
      >Left</button>
      <button
        onpointerdown="setRudder(150)"
        onpointerup="resetRudder()"
        onpointercancel="resetRudder()"
        onpointerleave="resetRudder()"
      >Right</button>
    </div>
    <div class="servo-control">
      <h3>Sails (Servo 2)</h3>
      <!-- hold to continuously adjust sail position -->
      <button
        onpointerdown="startSailAdjust('in')"
        onpointerup="stopSailAdjust()"
        onpointercancel="stopSailAdjust()"
        onpointerleave="stopSailAdjust()"
      >Sheet In</button>
      <button
        onpointerdown="startSailAdjust('out')"
        onpointerup="stopSailAdjust()"
        onpointercancel="stopSailAdjust()"
        onpointerleave="stopSailAdjust()"
      >Sheet Out</button>
    </div>
  </div>
  <script>

  let sailPos = 90;
  let sailAdjustInterval = null;

  function setSail(position) {
      if (position === 'in') {
          sailPos = Math.max(0, sailPos - 10);
      } else if (position === 'out') {
          sailPos = Math.min(180, sailPos + 10);
      }
      control(2, sailPos);
  }

  function startSailAdjust(direction) {
      // perform one immediate step
      adjustSailOnce(direction);
      // then continue adjusting while pressed
      if (sailAdjustInterval) clearInterval(sailAdjustInterval);
      sailAdjustInterval = setInterval(() => adjustSailOnce(direction), 150); // adjust every 150ms
  }

  function adjustSailOnce(direction) {
      const step = 2; // degrees per step (tweak as needed)
      if (direction === 'in') {
          sailPos = Math.max(0, sailPos - step);
      } else if (direction === 'out') {
          sailPos = Math.min(180, sailPos + step);
      }
      control(2, sailPos);
  }

  function stopSailAdjust() {
      if (sailAdjustInterval) {
          clearInterval(sailAdjustInterval);
          sailAdjustInterval = null;
      }
  }

  function setRudder(position) {
      control(1, position);
  }
  function resetRudder() {
      control(1, 90);
  }

  function control(servo, value) {
    fetch('/control?servo=' + servo + '&value=' + value)
      .then(response => response.text())
      .then(data => console.log(data));
  }
  </script>
</body>
</html>
)rawliteral";