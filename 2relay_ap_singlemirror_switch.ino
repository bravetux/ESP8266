// Updated ESP8266 Relay Controller with UI Timer Selection and Countdown Display
// Features:
// - Relay control via web interface
// - Selectable auto-OFF timer (3s / 5s)
// - Live countdown display in UI

// Developed by B.Vignesh Kumar <vigneshkumarb@bravetux.com>
// On 6th July 2025
// Co-Developed by S.Sai VishnuKumar 

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "vishnu-ap";
const char* password = "123456789";

ESP8266WebServer server(80);

const int relayGPIO = 5;  // D1
const int mirrorGPIO = 4; // D2
bool relayState = false;
unsigned long relayOnTime = 0;
unsigned long relayTimer = 0;

const bool RELAY_NO = false; // relay is Normally Open

// HTML page with slider and timer
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
<style>
  body {
    font-family: sans-serif;
    margin: 0;
    padding: 0;
    height: 100vh;
    display: flex;
    flex-direction: column;
    justify-content: center;  /* Vertical center */
    align-items: center;      /* Horizontal center */
    background-color: #f4f4f4;
  }

  h2 {
    margin-bottom: 20px;
  }

  .slider {
    width: 120px;
    height: 60px;
    position: relative;
    display: inline-block;
    margin-bottom: 20px;
  }

  .slider input {
    display: none;
  }

  .slider span {
    position: absolute;
    cursor: pointer;
    background-color: #ccc;
    border-radius: 50px;
    width: 100%;
    height: 100%;
    transition: 0.4s;
    box-shadow: 0 2px 5px rgba(0,0,0,0.2);
  }

  .slider span:before {
    content: "";
    position: absolute;
    height: 50px;
    width: 50px;
    left: 5px;
    bottom: 5px;
    background-color: white;
    transition: 0.4s;
    border-radius: 50%;
  }

  input:checked + span {
    background-color: #4CAF50;
  }

  input:checked + span:before {
    transform: translateX(60px);
  }

  select {
    font-size: 18px;
    padding: 10px 15px;
    margin-bottom: 20px;
  }

  #countdown {
    font-size: 22px;
    margin-bottom: 20px;
    color: #333;
  }

  footer {
    position: absolute;
    bottom: 10px;
    font-size: 14px;
    color: gray;
    text-align: center;
    width: 100%;
  }
</style>



</head>
<body>
  <h2>Water Dispenser Control Switch</h2>
  <label class="slider">
    <input type="checkbox" id="relayToggle" onchange="toggleRelay(this.checked)">
    <span></span>
  </label>
  <br><br>
  <select id="timer">
    <option value="3">3 seconds</option>
    <option value="5">5 seconds</option>
  </select>
  <div id="countdown"></div>
  <footer>Designed by S Sai Vishnukumar</footer>

  <script>
    let countdownInterval;

    function toggleRelay(state) {
      const timer = document.getElementById("timer").value;
      fetch(`/update?state=${state ? 1 : 0}&timer=${timer}`)
        .then(() => {
          if (state) startCountdown(timer);
          else stopCountdown();
        });
    }

    function startCountdown(seconds) {
      let remaining = seconds;
      document.getElementById("countdown").innerText = `Auto off in ${remaining}s`;
      countdownInterval = setInterval(() => {
        remaining--;
        if (remaining > 0) {
          document.getElementById("countdown").innerText = `Auto off in ${remaining}s`;
        } else {
          clearInterval(countdownInterval);
          document.getElementById("countdown").innerText = "";
          document.getElementById("relayToggle").checked = false;
        }
      }, 1000);
    }

    function stopCountdown() {
      clearInterval(countdownInterval);
      document.getElementById("countdown").innerText = "";
    }
  </script>
</body>
</html>
)rawliteral";

void setup() {
  Serial.begin(115200);

  pinMode(relayGPIO, OUTPUT);
  pinMode(mirrorGPIO, OUTPUT);
  digitalWrite(relayGPIO, RELAY_NO ? HIGH : LOW);
  digitalWrite(mirrorGPIO, RELAY_NO ? HIGH : LOW);

  WiFi.softAP(ssid, password);
  Serial.print("Access Point started. IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", index_html);
  });

  server.on("/update", HTTP_GET, []() {
    if (server.hasArg("state") && server.hasArg("timer")) {
      int state = server.arg("state").toInt();
      relayTimer = server.arg("timer").toInt() * 1000;
      relayState = (state == 1);
      digitalWrite(relayGPIO, RELAY_NO ? !state : state);
      digitalWrite(mirrorGPIO, RELAY_NO ? !state : state);

      if (relayState) {
        relayOnTime = millis();
      }
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Missing parameters");
    }
  });

  server.begin();
}

void loop() {
  server.handleClient();

  if (relayState && relayTimer > 0 && millis() - relayOnTime >= relayTimer) {
    relayState = false;
    digitalWrite(relayGPIO, RELAY_NO ? HIGH : LOW);
    digitalWrite(mirrorGPIO, RELAY_NO ? HIGH : LOW);
    relayTimer = 0;
    Serial.println("Auto-off: GPIO5 and GPIO4 turned OFF");
  }
}
