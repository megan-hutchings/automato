#include "soil_sensor.h"


// Function to read a 32-bit value from Seesaw
uint32_t genericRead32(uint8_t sensor_address,uint8_t base, uint8_t function) {
  Wire.beginTransmission(sensor_address);
  Wire.write(base);
  Wire.write(function);
  Wire.endTransmission();

  delay(5); // give it a moment to respond

  Wire.requestFrom(sensor_address, (uint8_t)4);
  uint32_t ret = 0;
  for (int i = 0; i < 4 && Wire.available(); i++) {
    ret <<= 8;
    ret |= Wire.read();
  }
  return ret;
}


uint16_t getCapacitance(uint8_t sensor_address){
  Wire.beginTransmission(sensor_address);
  Wire.write(SEESAW_TOUCH_BASE);
  Wire.write(SEESAW_TOUCH_CHANNEL_OFFSET);
  Wire.endTransmission();

  delay(5); // give it a moment to respond

  Wire.requestFrom(sensor_address, (uint8_t)2);
  uint16_t ret = 65535;
  for (int i = 0; i < 2 && Wire.available(); i++) {
    ret <<= 8;
    ret |= Wire.read();
  }
  return ret;
}