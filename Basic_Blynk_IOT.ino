
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266TimeAlarms.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "<insert blynk token>";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "<insert SSID here";
char pass[] = "<password>";

WidgetLED led1(V1);

#define ESP8266_GPIO5 5           // Relay Control

BlynkTimer timer;

void setup()
{

  pinMode( LED_BUILTIN, OUTPUT );         // ESP8266 module blue LED.
  digitalWrite( LED_BUILTIN, LOW );       // Turn OFF LED.
  
  pinMode( ESP8266_GPIO5, OUTPUT );       // Relay control pin.
  digitalWrite( ESP8266_GPIO5, HIGH );    // Turn off RELAY,  when powered up
  
  // Debug console
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);

  while (Blynk.connect() == false) {
    Serial.println("Blynk Connection Failed...");
  }
}

void loop()
{
  Blynk.run();

  /*  
   *  Read GPIO5, and determine the pin
   *  and show the Status in LED Widget in the App
   */
  if (digitalRead (ESP8266_GPIO5) == HIGH) {
      Blynk.email("ESP8266", "Light OFF");
      led1.off();
  } else {
      Blynk.email("ESP8266", "Light ON");
      led1.on();
  } 
}
