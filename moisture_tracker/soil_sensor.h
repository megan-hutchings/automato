#ifndef SOIL_SENSOR_H
#define SOIL_SENSOR_H
#include <Wire.h>
#include <Arduino.h>



// Base addresses of sensor
#define SEESAW_STATUS_BASE 0x00
#define SEESAW_TOUCH_BASE       0x0F

// Function registers of sensor
#define SEESAW_STATUS_HW_ID     0x01
#define SEESAW_STATUS_VERSION   0x02
#define SEESAW_STATUS_TEMP      0x04
#define SEESAW_STATUS_SWRST     0x7F
#define SEESAW_TOUCH_CHANNEL_OFFSET 0x10

// read 4 byte response from a given register
uint32_t genericRead32(uint8_t sensor_address,uint8_t base, uint8_t function);
uint16_t getCapacitance(uint8_t sensor_address);
#endif