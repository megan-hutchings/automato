# automato_tomato
Automate Tomato Growing

# Moisture Tracker
./moisture_tracker/
- sketch for arduino nano esp32 to interface with soil moisture sensor and power an LED indicator

## Current Functionality
Monitors status of capacitive touch sensor. If sensor records capacitance below threshold (1000) the pump pin is set to high and an email is sent to the set address. If the sensor detects above the threshold the pump pin will be driven low. 

## LED Status
- Blue: cannot connect to wifi
- Purple: cannot connect to sensor
- Red: moisture is below threshold (1000), plant needs watering
- Green: moisture is above threshold, plant is ok

## Wifi mode
- to use Wifi mode (connect to wifi + send emails) Pin D11 must be driven high. Else the board will be in 'offline' mode
- pin can be switched to high at any time however if the board is actively trying to connect to the wifi (status LED = blue) then it will not detect if wifi mode is disabled (D11 driven low). to turn off wifi mode from this state you need to reset the board after you have set D11 low.

# Moisture Tracker Mini
./moisture_tracker_mini/
- sketch for ESP32C3 DEV to interface with soil moisture sensor and power an LED indicator

## Board
### Setup steps
https://www.waveshare.com/wiki/ESP32-C3-Zero
### Datasheet
https://www.espressif.com/sites/default/files/documentation/esp32-c3_datasheet_en.pdf

## Current Functionality
Monitors status of capacitive touch sensor. If sensor records capacitance below threshold (1000) the indication LED will be red if its above it will be green.

## LED Status
- RED + BLUE (or PURPLE if using RGB LED): cannot connect to sensor
- Red: moisture is below threshold (1000), plant needs watering
- Green: moisture is above threshold, plant is ok

# Examples
./examples/smtp_test/
- example of sending an email from arduino nano esp32

./examples/rgb_led_test/
- example of controlling an RGB LED

