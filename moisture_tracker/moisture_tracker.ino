#include <WiFi.h>
#include "email.h"
#include "soil_sensor.h"

// wifi + email functionality
#define WIFI_SSID "gabriel"
#define WIFI_PASSWORD "gabrielgabriel"

//////////////////////// SENSOR COMMUNICATION
int LED_PIN = 12;
bool LED_STATUS = false;
String plantName = "Plant 1";
// i2C address of sensor
#define SEESAW_ADDR 0x36


void setup() {
  Serial.begin(115200);
  delay(1000);
  
  // connect to wifi
  Serial.println();
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // connect to sensor 
  Wire.begin();
  Serial.println("Starting Soil Sensor I2C communication...");
  delay(100);
  Wire.beginTransmission(SEESAW_ADDR);
  if (Wire.endTransmission() == 0) {
    Serial.println("Seesaw device found!");
  } else {
    Serial.println("No Seesaw device detected!");
  }
  
  // Read and print board info
  // Version
  uint32_t version = genericRead32(SEESAW_ADDR,SEESAW_STATUS_BASE, SEESAW_STATUS_VERSION);
  Serial.print("Version: 0x");
  Serial.println(version, HEX);

  // HW ID
  uint32_t hw_ID = genericRead32(SEESAW_ADDR,SEESAW_STATUS_BASE, SEESAW_STATUS_HW_ID);
  Serial.print("HWID: 0x");
  Serial.println(hw_ID, HEX);

  // temp status
  uint32_t temp = genericRead32(SEESAW_ADDR,SEESAW_STATUS_BASE, SEESAW_STATUS_TEMP);
  Serial.print("temp: 0x");
  Serial.println(temp, HEX);

  //indicator LED
  pinMode(LED_PIN, OUTPUT);

}

void loop() {

  uint16_t cap = getCapacitance(SEESAW_ADDR);
  Serial.print("capacitance:"); //200-500 = dry 1000+ = wet
  Serial.println(cap);

  uint32_t temp = genericRead32(SEESAW_ADDR,SEESAW_STATUS_BASE, SEESAW_STATUS_TEMP);
  Serial.print("temp: ");
  Serial.println(temp);

  if (cap < 1000){// if dry
    if (!LED_STATUS ){ // if previously damp
      digitalWrite(LED_PIN,HIGH);
      Serial.println("Plant watering required");
      // send email
      sendEmail(plantName);
      //turn on pump
      LED_STATUS = true;
    }
   
  } else { // if damp
      if (LED_STATUS) { // if previously too dry
        digitalWrite(LED_PIN,LOW);
        Serial.println("Plant watering complete");
        LED_STATUS = false;
      }
    }
    
  delay(1000);
}
