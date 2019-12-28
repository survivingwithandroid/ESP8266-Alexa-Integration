#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <fauxmoESP.h>
#include <Adafruit_NeoPixel.h>

#define PIN D2
#define PIN_STRIP D1
#define NUM_LEDS 12
#define NUM_LEDS_STRIP 8

const char* ssid = "xxxx";
const char* password = "nnnnn";

fauxmoESP fauxmo;

// LEDS
Adafruit_NeoPixel pixelsRing(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixelsStrip(NUM_LEDS_STRIP, PIN_STRIP, NEO_GRB + NEO_KHZ800);

void turnOffRing() {
  pixelsRing.fill();
  pixelsRing.show();
}


void turnOffStrip() {
  pixelsStrip.fill();
  pixelsStrip.show();
}

void setRingColor(int r, int g, int b) {
  for (int i=0; i < NUM_LEDS; i++)
    pixelsRing.setPixelColor(i, pixelsRing.Color(r,g,b));

  pixelsRing.show();
}

void setStripColor(int r, int g, int b) {
  pixelsStrip.fill(pixelsStrip.Color(r,g,b));
  pixelsStrip.show();
}

boolean connectWifi() {
  // Let us connect to WiFi
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(".......");
  Serial.println("WiFi Connected....IP Address:");
  Serial.println(WiFi.localIP());

  return true;
}

void setup() { 
  Serial.begin(115200);
  pixelsRing.setBrightness(200);
  pixelsRing.begin();
  pixelsStrip.begin();

  
  if (connectWifi()) {
    // Setup fauxmo
    Serial.println("Adding LED device");
     fauxmo.setPort(80);  
     fauxmo.enable(true);
     fauxmo.addDevice("Ring");   
     fauxmo.addDevice("Strip");
     
     fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
       Serial.print("Device name:");
       Serial.println(device_name);
       if (strcmp(device_name, "Ring") == 0) {
          Serial.print("Ring status:");
          Serial.print(state);
          Serial.println("");
          // Set RGB Color
          if (state)
             setRingColor(10,100,40);
          else
             turnOffRing();         
       }
       else if (strcmp(device_name, "Strip") == 0) {
         Serial.print("Strip status:");
          Serial.print(state);
          Serial.println("");
          // Set RGB Color
          if (state)
             setStripColor(180,30,40);
          else
             turnOffStrip();                 
       }
     });
   }
 }
  

void loop() {
  fauxmo.handle();
}



