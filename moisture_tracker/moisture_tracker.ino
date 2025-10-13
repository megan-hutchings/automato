#include <Wire.h>
int LED_PIN = 12;
bool LED_STATUS = false;

#define SEESAW_ADDR 0x36

// Base addresses
#define SEESAW_STATUS_BASE 0x00
#define SEESAW_TOUCH_BASE       0x0F

// Function registers
#define SEESAW_STATUS_HW_ID     0x01
#define SEESAW_STATUS_VERSION   0x02
#define SEESAW_STATUS_TEMP      0x04
#define SEESAW_STATUS_SWRST     0x7F
#define SEESAW_TOUCH_CHANNEL_OFFSET 0x10


// Function to read a 32-bit value from Seesaw
uint32_t read32(uint8_t base, uint8_t function) {
  Wire.beginTransmission(SEESAW_ADDR);
  Wire.write(base);
  Wire.write(function);
  Wire.endTransmission();

  delay(5); // give it a moment to respond

  Wire.requestFrom(SEESAW_ADDR, (uint8_t)4);
  uint32_t ret = 0;
  for (int i = 0; i < 4 && Wire.available(); i++) {
    ret <<= 8;
    ret |= Wire.read();
  }
  return ret;
}

void getTemp(){};

uint16_t getCapacitance(){
  Wire.beginTransmission(SEESAW_ADDR);
  Wire.write(SEESAW_TOUCH_BASE);
  Wire.write(SEESAW_TOUCH_CHANNEL_OFFSET);
  Wire.endTransmission();

  delay(5); // give it a moment to respond

  Wire.requestFrom(SEESAW_ADDR, (uint8_t)2);
  uint16_t ret = 65535;
  for (int i = 0; i < 2 && Wire.available(); i++) {
    ret <<= 8;
    ret |= Wire.read();
  }
  return ret;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting Seesaw I2C communication...");
  Wire.begin();
  delay(100);

  // Check if device responds
  Wire.beginTransmission(SEESAW_ADDR);
  if (Wire.endTransmission() == 0) {
    Serial.println("Seesaw device found!");
  } else {
    Serial.println("No Seesaw device detected!");
  }

  // Read and print board info
  // Version
  uint32_t version = read32(SEESAW_STATUS_BASE, SEESAW_STATUS_VERSION);
  Serial.print("Version: 0x");
  Serial.println(version, HEX);

  // HW ID
  uint32_t hw_ID = read32(SEESAW_STATUS_BASE, SEESAW_STATUS_HW_ID);
  Serial.print("HWID: 0x");
  Serial.println(hw_ID, HEX);

  // temp status
  uint32_t temp = read32(SEESAW_STATUS_BASE, SEESAW_STATUS_TEMP);
  Serial.print("temp: 0x");
  Serial.println(temp, HEX);

  // swrst
  uint32_t swrst = read32(SEESAW_STATUS_BASE, SEESAW_STATUS_SWRST);
  Serial.print("swrst: 0x");
  Serial.println(swrst, HEX);

  //indicator LED
  pinMode(LED_PIN, OUTPUT);

}

void loop() {

  uint16_t cap =  getCapacitance();
  Serial.print("capacitance:"); //200-500 = dry 1000+ = wet
  Serial.println(cap);

  uint32_t temp = read32(SEESAW_STATUS_BASE, SEESAW_STATUS_TEMP);
  Serial.print("temp: ");
  Serial.println(temp);

  if (cap < 1000){
    digitalWrite(LED_PIN,HIGH);
    // power pump
  } else {
    digitalWrite(LED_PIN,LOW);
  }

  delay(1000);
}
