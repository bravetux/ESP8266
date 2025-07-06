// Updated ESP8266 Relay Controller with UI Timer Selection and Countdown Display
// Features:
// - Relay control via web interface
// - Selectable auto-OFF timer (10s / 15s)
// - Live countdown display in UI

// Developed by B.Vignesh Kumar <vigneshkumarb@bravetux.com>
// On 6th July 2025
// Co-Developed by S.Sai VishnuKumar 

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>

// Relay Configuration
#define RELAY_NO false                           // false = Normally Closed
#define NUM_RELAYS 2                             // Number of relays
int relayGPIOs[NUM_RELAYS] = {5, 4};             // GPIO pins for Relay 1 and Relay 2

// Tracking relay state and timers
unsigned long relayOnTimes[NUM_RELAYS] = {0, 0}; // Timestamp when relays were turned ON
int relayTimers[NUM_RELAYS] = {10000, 15000};    // Duration in milliseconds (10s and 15s default)
bool relayStates[NUM_RELAYS] = {false, false};  // Whether each relay is currently ON or OFF

// WiFi Access Point credentials
AsyncWebServer server(80);
const char* ssid = "vishnu-ap";
const char* password = "123456789";

// HTML Web UI stored in flash memory
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; text-align: center;}
    h2 {font-size: 2.5rem;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px}
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0;
             background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px;
                    left: 8px; bottom: 8px; background-color: white;
                    transition: .4s; border-radius: 68px}
    input:checked + .slider {background-color: #2196F3}
    input:checked + .slider:before {transform: translateX(52px);}
  </style>
</head>
<body>
  <h2>ESP8266 Relay Control</h2>
  %BUTTONPLACEHOLDER%
  <div class="footer">
    Web Interface - Designed by S Sai Vishnu Kumar (C) 2025
  </div>
  <script>
    var countdownIntervals = {}; // Hold reference to countdown interval per relay

    // Handle switch toggle from UI
    function toggleCheckbox(element, relay) {
      var timer = document.getElementById("timer" + relay).value;
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/update?relay=" + relay + "&state=" + (element.checked ? "1" : "0") + "&timer=" + timer, true);
      xhr.send();

      // Start or stop countdown based on toggle
      if (element.checked) {
        startCountdown(relay, timer);
      } else {
        clearInterval(countdownIntervals[relay]);
        document.getElementById("countdown" + relay).innerHTML = "OFF";
      }
    }

    // Countdown display handler
    function startCountdown(relay, duration) {
      var countdown = parseInt(duration);
      var label = document.getElementById("countdown" + relay);
      label.innerHTML = countdown + "s left";

      clearInterval(countdownIntervals[relay]);

      countdownIntervals[relay] = setInterval(function() {
        countdown--;
        if (countdown <= 0) {
          clearInterval(countdownIntervals[relay]);
          label.innerHTML = "OFF";
          document.getElementById(relay).checked = false;
        } else {
          label.innerHTML = countdown + "s left";
        }
      }, 1000);
    }
  </script>
</body>
</html>
)rawliteral";

// Return "checked" if relay is currently ON
String relayState(int relay) {
  bool isOn = digitalRead(relayGPIOs[relay - 1]) == (RELAY_NO ? LOW : HIGH);
  return isOn ? "checked" : "";
}

// Generate button HTML dynamically for each relay
String processor(const String& var) {
  if (var == "BUTTONPLACEHOLDER") {
    String buttons;
    for (int i = 0; i < NUM_RELAYS; i++) {
      int relayNumber = i + 1;
      String relayName = (relayNumber == 1) ? "Solenoid Valve Relay #1 " : "Water Pump Relay #2 ";

      buttons += "<p><strong>" + relayName + "</strong> (GPIO " + String(relayGPIOs[i]) + ")</p>";
      buttons += "<label class=\"switch\">";
      buttons += "<input type=\"checkbox\" id=\"" + String(relayNumber) + "\" onchange=\"toggleCheckbox(this, " + String(relayNumber) + ")\" " + relayState(relayNumber) + ">";
      buttons += "<span class=\"slider\"></span></label>";
      buttons += "<br><label>Timer: <select id=\"timer" + String(relayNumber) + "\">";
      buttons += "<option value=\"10\">10 sec</option><option value=\"15\">15 sec</option></select></label>";
      buttons += "<p id=\"countdown" + String(relayNumber) + "\">" + (relayStates[i] ? String(relayTimers[i] / 1000) + "s left" : "OFF") + "</p><br><br>";
    }
    return buttons;
  }
  return String();
}

void setup() {
  Serial.begin(115200);

  // Initialize GPIO pins and set relays to OFF
  for (int i = 0; i < NUM_RELAYS; i++) {
    pinMode(relayGPIOs[i], OUTPUT);
    digitalWrite(relayGPIOs[i], RELAY_NO ? HIGH : LOW);
  }

  // Start WiFi AP mode
  WiFi.softAP(ssid, password);
  delay(100);
  Serial.println("Access Point Started");
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());

  // Serve web interface
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  // Handle relay control updates from UI
  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
    if (request->hasParam("relay") && request->hasParam("state") && request->hasParam("timer")) {
      int relayIndex = request->getParam("relay")->value().toInt() - 1;
      int state = request->getParam("state")->value().toInt();
      int duration = request->getParam("timer")->value().toInt() * 1000;

      if (relayIndex >= 0 && relayIndex < NUM_RELAYS) {
        digitalWrite(relayGPIOs[relayIndex], RELAY_NO ? !state : state);
        relayStates[relayIndex] = state;
        if (state == 1) {
          relayOnTimes[relayIndex] = millis();
          relayTimers[relayIndex] = duration;
        }
        Serial.printf("Relay %d -> %s for %d ms\n", relayIndex + 1, state ? "ON" : "OFF", duration);
        request->send(200, "text/plain", "OK");
      } else {
        request->send(400, "text/plain", "Invalid relay index");
      }
    } else {
      request->send(400, "text/plain", "Missing parameters");
    }
  });

  server.begin();
}

void loop() {
  // Check if any relays need to be turned off automatically
  unsigned long now = millis();
  for (int i = 0; i < NUM_RELAYS; i++) {
    if (relayStates[i] && (now - relayOnTimes[i] >= relayTimers[i])) {
      digitalWrite(relayGPIOs[i], RELAY_NO ? HIGH : LOW);
      relayStates[i] = false;
      Serial.printf("Auto-OFF -> Relay %d\n", i + 1);
    }
  }
}
