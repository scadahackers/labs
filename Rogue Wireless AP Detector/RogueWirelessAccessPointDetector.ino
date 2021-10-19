#include "ESP8266WiFi.h"

/////// Customise here //////////

//Your Wireless SSID//
String monitoredssid = "YOURSSID";

//Your Wireless BSSIDS//
const char * monitoredbssids[] = {
  "AA:AA:AA:AA:AA:AA",
  "AA:AA:AA:AA:AA:AB"
};

//How many BSSIDs in your network
int bssidcount = 2;

///// END cusomisation /////
//////////////////////////////////////////////////////////////

#define BLINK_PERIOD 250
long lastBlinkMillis;
boolean ledState;

#define SCAN_PERIOD 5000
long lastScanMillis;

int count = 0;
String mybssid = "";
boolean rogueap = false;

// Buzzer setup
const unsigned char Passive_buzzer = 2;

void setup() {
  Serial.begin(115200);
  Serial.println();
  pinMode (Passive_buzzer,OUTPUT) ;
  pinMode(LED_BUILTIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}

void loop() {
  long currentMillis = millis();

  // blink LED
  if (currentMillis - lastBlinkMillis > BLINK_PERIOD) {
    digitalWrite(LED_BUILTIN, ledState);
    ledState = !ledState;
    lastBlinkMillis = currentMillis;
  }

  // trigger Wi-Fi network scan
  if (currentMillis - lastScanMillis > SCAN_PERIOD) {
    WiFi.scanNetworks(true, true);
    Serial.print("\nScan start ... ");
    lastScanMillis = currentMillis;
  }
  // print out Wi-Fi network scan result upon completion
  int n = WiFi.scanComplete();
  if (n >= 0) {
    Serial.printf("%d network(s) found\n", n);
    for (int i = 0; i < n; i++) {
      // If the discovered SSID matches what we're monitoring for, the loop over our known BSSIDs
      if (monitoredssid == WiFi.SSID(i).c_str()) {
        rogueap = true;
        for (count = 0; count < bssidcount; count++) {
          mybssid = (monitoredbssids[count]);
          if (mybssid == WiFi.BSSIDstr(i).c_str()) {
            rogueap = false;
          }
        }
        if (rogueap == true) {
          Serial.printf("\nRogue AP Detected! %d: %s, Ch:%d (%ddBm) %s\n", i + 1, WiFi.SSID(i).c_str(), WiFi.channel(i), WiFi.RSSI(i), WiFi.BSSIDstr(i).c_str(), "\n");
          roguedetected();
        } else if (rogueap == false) {
          Serial.printf("\n No rogue AP detected.\n");
        }
      }
    }
    WiFi.scanDelete();
  }
}

// Do something when rogue AP is detected
void roguedetected(){
tone(Passive_buzzer, 1046) ; //DO note 523 Hz
delay (1000); 
noTone(Passive_buzzer) ; //Turn off the pin attached to the tone()
tone(Passive_buzzer, 1046) ; //DO note 523 Hz
delay (1000); 
noTone(Passive_buzzer) ; //Turn off the pin attached to the tone()
tone(Passive_buzzer, 1046) ; //DO note 523 Hz
delay (1000); 
noTone(Passive_buzzer) ; //Turn off the pin attached to the tone()
tone(Passive_buzzer, 1046) ; //DO note 523 Hz
delay (1000); 
noTone(Passive_buzzer) ; //Turn off the pin attached to the tone()
}
