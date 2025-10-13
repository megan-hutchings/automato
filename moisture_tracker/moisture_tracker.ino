#include <Wire.h>
#include <WiFi.h>
#include <ESP_Mail_Client.h>

// wifi + email functionality
#define WIFI_SSID "gabriel"
#define WIFI_PASSWORD "gabrielgabriel"

/** The smtp host name e.g. smtp.gmail.com for GMail or smtp.office365.com for Outlook or smtp.mail.yahoo.com */
#define SMTP_HOST "smtp.gmail.com"
#define SMTP_PORT 465

/* The sign in credentials */
#define AUTHOR_EMAIL "plant.updates1@gmail.com"
#define AUTHOR_PASSWORD "insert password here"

/* Recipient's email*/
#define RECIPIENT_EMAIL "mhg@akkodis.se"

/* Declare the global used SMTPSession object for SMTP transport */
SMTPSession smtp;

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status);



//////////////////////// SENSOR COMMUNICATION
int LED_PIN = 12;
bool LED_STATUS = false;
String plantName = "Plant 1";


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

/* Callback function to get the Email sending status */
void smtpCallback(SMTP_Status status){
  /* Print the current status */
  Serial.println(status.info());

  /* Print the sending result */
  if (status.success()){
    // ESP_MAIL_PRINTF used in the examples is for format printing via debug Serial port
    // that works for all supported Arduino platform SDKs e.g. AVR, SAMD, ESP32 and ESP8266.
    // In ESP8266 and ESP32, you can use Serial.printf directly.

    Serial.println("----------------");
    ESP_MAIL_PRINTF("Message sent success: %d\n", status.completedCount());
    ESP_MAIL_PRINTF("Message sent failed: %d\n", status.failedCount());
    Serial.println("----------------\n");

    for (size_t i = 0; i < smtp.sendingResult.size(); i++)
    {
      /* Get the result item */
      SMTP_Result result = smtp.sendingResult.getItem(i);

      // In case, ESP32, ESP8266 and SAMD device, the timestamp get from result.timestamp should be valid if
      // your device time was synched with NTP server.
      // Other devices may show invalid timestamp as the device time was not set i.e. it will show Jan 1, 1970.
      // You can call smtp.setSystemTime(xxx) to set device time manually. Where xxx is timestamp (seconds since Jan 1, 1970)
      
      ESP_MAIL_PRINTF("Message No: %d\n", i + 1);
      ESP_MAIL_PRINTF("Status: %s\n", result.completed ? "success" : "failed");
      ESP_MAIL_PRINTF("Date/Time: %s\n", MailClient.Time.getDateTimeString(result.timestamp, "%B %d, %Y %H:%M:%S").c_str());
      ESP_MAIL_PRINTF("Recipient: %s\n", result.recipients.c_str());
      ESP_MAIL_PRINTF("Subject: %s\n", result.subject.c_str());
    }
    Serial.println("----------------\n");

    // You need to clear sending result as the memory usage will grow up.
    smtp.sendingResult.clear();
  }
}

void sendEmail(String plantName){
  // connect to mail server
  MailClient.networkReconnect(true);
  smtp.debug(1); // set to 0 to not see debug data
  smtp.callback(smtpCallback);
  Session_Config config;
  config.server.host_name = SMTP_HOST;
  config.server.port = SMTP_PORT;
  config.login.email = AUTHOR_EMAIL;
  config.login.password = AUTHOR_PASSWORD;
  config.login.user_domain = "";
    /*
  Set the NTP config time
  For times east of the Prime Meridian use 0-12
  For times west of the Prime Meridian add 12 to the offset.
  Ex. American/Denver GMT would be -6. 6 + 12 = 18
  See https://en.wikipedia.org/wiki/Time_zone for a list of the GMT/UTC timezone offsets
  */
  config.time.ntp_server = F("pool.ntp.org,time.nist.gov");
  config.time.gmt_offset = 3;
  config.time.day_light_offset = 0;

  /* Declare the message class */
  SMTP_Message message;

  /* Set the message headers */
  message.sender.name = F("ESP");
  message.sender.email = AUTHOR_EMAIL;
  message.subject = F("Plant Needs Watering!");
  message.addRecipient(F("Megan"), RECIPIENT_EMAIL);
    
  /*Send HTML message*/
  /*String htmlMsg = "<div style=\"color:#2f4468;\"><h1>Hello World!</h1><p>- Sent from ESP board</p></div>";
  message.html.content = htmlMsg.c_str();
  message.html.content = htmlMsg.c_str();
  message.text.charSet = "us-ascii";
  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;*/

   
  //Send raw text message
  String textMsg = "Please water " + plantName;
  message.text.content = textMsg.c_str();
  message.text.charSet = "us-ascii";
  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;
  
  message.priority = esp_mail_smtp_priority::esp_mail_smtp_priority_low;
  message.response.notify = esp_mail_smtp_notify_success | esp_mail_smtp_notify_failure | esp_mail_smtp_notify_delay;


  /* Connect to the server */
  if (!smtp.connect(&config)){
    ESP_MAIL_PRINTF("Connection error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());
    return;
  }

  if (!smtp.isLoggedIn()){
    Serial.println("\nNot yet logged in.");
  }
  else{
    if (smtp.isAuthenticated())
      Serial.println("\nSuccessfully logged in.");
    else
      Serial.println("\nConnected with no Auth.");
  }

  /* Start sending Email and close the session */
  if (!MailClient.sendMail(&smtp, &message))
    ESP_MAIL_PRINTF("Error, Status Code: %d, Error Code: %d, Reason: %s", smtp.statusCode(), smtp.errorCode(), smtp.errorReason().c_str());

}


void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting Seesaw I2C communication...");


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
  delay(100);
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

  if (cap < 1000){// if dry
    if (!LED_STATUS ){ // if previously damp
      digitalWrite(LED_PIN,HIGH);
      Serial.println("Plant watering required");
      // send email
      sendEmail(plantName);
      LED_STATUS = true;
      //turn on pump
    }
   
  } else { // damp
      if (LED_STATUS) { // if previously too dry
        digitalWrite(LED_PIN,LOW);
        Serial.println("Plant watering complete");
        LED_STATUS = false;
      }
    }
    

  delay(1000);
}
