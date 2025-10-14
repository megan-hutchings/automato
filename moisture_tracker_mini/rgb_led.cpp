#include "rgb_led.h"



void setStatusLED(int red_pin,int green_pin, int blue_pin, String colour){
  if (colour == "RED"){
    digitalWrite(red_pin, HIGH);
    digitalWrite(green_pin, LOW);
    digitalWrite(blue_pin, LOW);
  } else if (colour == "GREEN"){
    digitalWrite(green_pin, HIGH);
    digitalWrite(blue_pin, LOW);
    digitalWrite(red_pin, LOW);
  } else if (colour == "BLUE"){
    digitalWrite(blue_pin, HIGH);
    digitalWrite(green_pin, LOW);
    digitalWrite(red_pin, LOW);
  } else if (colour == "YELLOW"){
    digitalWrite(red_pin, HIGH);
    digitalWrite(green_pin, HIGH);
    digitalWrite(blue_pin, LOW);
  } else if (colour == "PURPLE"){
    digitalWrite(red_pin, HIGH);
    digitalWrite(green_pin, LOW);
    digitalWrite(blue_pin, HIGH);
  } else if (colour == "CYAN"){
    digitalWrite(red_pin, LOW);
    digitalWrite(green_pin,HIGH);
    digitalWrite(blue_pin, HIGH);
  } else if (colour == "WHITE"){
    digitalWrite(red_pin, HIGH);
    digitalWrite(green_pin,HIGH);
    digitalWrite(blue_pin, HIGH);
  } else if (colour == "OFF"){
    digitalWrite(red_pin, LOW);
    digitalWrite(green_pin,LOW);
    digitalWrite(blue_pin, LOW);
  }
}