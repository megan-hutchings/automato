#include <WiFi.h>
#include "email.h"
#include "soil_sensor.h"
#include "rgb_led.h"

// wifi credentials
#define WIFI_SSID "gabriel"
#define WIFI_PASSWORD "gabrielgabriel"
bool WIFI_CONNECTED = false;
int WIFI_MODE_PIN = 11;

// Sensor communication
#define SEESAW_ADDR 0x36 // i2C address of sensor
int PUMP_CONTROL_PIN = 12; // drives motor and motor indication LED
bool PUMP_ON_STATUS = false;
bool SENSOR_CONNECTED = false;
String plantName = "Plant 1";

// status LED pins
int RGBLED_RedPin = 3;
int RGBLED_GreenPin = 5;
int RGBLED_BluePin = 6;

void wifiConnect(){
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  WIFI_CONNECTED = true;
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  /////// indicator LEDs
  // pump PWR
  pinMode(PUMP_CONTROL_PIN, OUTPUT);

  // status
  pinMode(RGBLED_RedPin, OUTPUT);
  pinMode(RGBLED_GreenPin, OUTPUT);
  pinMode(RGBLED_BluePin, OUTPUT);

  // wifi Indicator PIN
  pinMode(WIFI_MODE_PIN, INPUT);
  

  if (digitalRead(WIFI_MODE_PIN)){
    setStatusLED(RGBLED_RedPin,RGBLED_GreenPin,RGBLED_BluePin,"BLUE");
    wifiConnect();
  }

  // connect to sensor 
  Wire.begin();
  Serial.println("Starting Soil Sensor I2C communication...");
  delay(100);
  Wire.beginTransmission(SEESAW_ADDR);
  if (Wire.endTransmission() == 0) {
    Serial.println("Seesaw device found!");
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
  } else {
    Serial.println("No Seesaw device detected!");
    setStatusLED(RGBLED_RedPin,RGBLED_GreenPin,RGBLED_BluePin,"PURPLE");
  }
}

void loop() {
  if ((digitalRead(WIFI_MODE_PIN)) && (!WIFI_CONNECTED)){
    // try to reconnect to wifi
    setStatusLED(RGBLED_RedPin,RGBLED_GreenPin,RGBLED_BluePin,"BLUE");
    wifiConnect();
    setStatusLED(RGBLED_RedPin,RGBLED_GreenPin,RGBLED_BluePin,"GREEN");
    PUMP_ON_STATUS = false;
  } else {
    uint16_t cap = getCapacitance(SEESAW_ADDR);
    if (cap == 65535){
      Serial.println("Sensor Disconnected, trying to reconnect");
      if (SENSOR_CONNECTED){
        SENSOR_CONNECTED = false;
        PUMP_ON_STATUS = false;
        setStatusLED(RGBLED_RedPin,RGBLED_GreenPin,RGBLED_BluePin,"PURPLE");
      }
    } else {
      if (!SENSOR_CONNECTED){
        Serial.println("Sensor Reconnected!");
        SENSOR_CONNECTED = true;
        PUMP_ON_STATUS = false;
        setStatusLED(RGBLED_RedPin,RGBLED_GreenPin,RGBLED_BluePin,"GREEN");
      }

      Serial.print("capacitance:"); //200-500 = dry 1000+ = wet
      Serial.println(cap);

      uint32_t temp = genericRead32(SEESAW_ADDR,SEESAW_STATUS_BASE, SEESAW_STATUS_TEMP);
      Serial.print("temp: ");
      Serial.println(temp);

      if (cap < 1000){// if dry
        if (!PUMP_ON_STATUS){ // if pump not running
          digitalWrite(PUMP_CONTROL_PIN,HIGH);
          //turn on pump
          PUMP_ON_STATUS = true;
          setStatusLED(RGBLED_RedPin,RGBLED_GreenPin,RGBLED_BluePin,"RED");
          Serial.println("Plant watering required");
          if (digitalRead(WIFI_MODE_PIN)){
            // send email
            int status = sendEmail(plantName);
            if (status == 202){
              Serial.println("Email send success");
            } else if (status == -400) {
              Serial.println("Wifi Disconnected");
              WIFI_CONNECTED = false;
              PUMP_ON_STATUS = false; // turn off pump if no longer checking capacitance
            } else if (status == -100) {
              Serial.println("Wifi Disconnected");
              WIFI_CONNECTED = false;
              PUMP_ON_STATUS = false; // turn off pump if no longer checking capacitance
            } else if (status == -105){
              Serial.println("Incorrect Password For Email");
            } else{
              Serial.println("Unhandled Error code for email");
            }
          }
        }
      } else { // if damp
        if (PUMP_ON_STATUS) { // if pump running
          digitalWrite(PUMP_CONTROL_PIN,LOW);
          Serial.println("Plant watering complete");
          PUMP_ON_STATUS = false;
          setStatusLED(RGBLED_RedPin,RGBLED_GreenPin,RGBLED_BluePin,"GREEN");
        }
      }
    }
  }
  delay(1000);
}
