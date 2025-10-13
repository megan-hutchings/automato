# automato_tomato
Automate Tomato Growing

# Moisture Tracker
./moisture_tracker/
- sketch for arduino nano esp32 to interface with soil moisture sensor and power an LED indicator

## Current Functionality
Monitors status of capacitive touch sensor. If sensor records capacitance below threshold (1000) the pump pin is set to high and an email is sent to the set address. If the sensor detects above the threshold the pump pin will be driven low

## LED Status
- Blue: cannot connect to wifi
- Purple: cannot connect to sensor
- Red: moisture is below threshold (1000), plant needs watering
- Green: moisture is above threshold, plant is ok

# Examples
./examples/smtp_test/
- example of sending an email from arduino nano esp32

./examples/rgb_led_test/
- example of controlling an RGB LED

