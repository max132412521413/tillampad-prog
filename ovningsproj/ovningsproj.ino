/*
* Name: övningsprojekt
* Author: Victor Huke
* Date: 2025-10-14
* Description: This project uses a ds3231 to measure time and displays the time to an 1306 oled display, 
* Further, it measures temprature with ds3231 and displays a mapped value to a 9g-servo-motor.
*/

// Include Libraries
#include <RTClib.h>
#include <Wire.h>
#include <Servo.h>
#include "U8glib.h"


// Init constants

// Init global variables

// Construct objects
RTC_DS3231 rtc;
Servo myservo;
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);  // Display which does not send AC


void setup() {
  // init communication
  Serial.begin(9600);
  Wire.begin();

  // Init Hardware
  rtc.begin();
  myservo.attach(9);
  pinMode(10, OUTPUT);

  // Settings
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
  oledWrite(getTime(), getTemp());  //remove comment when the function is written
  servoWrite(getTemp());            //remove comment when the function is written

  Serial.println(getTime());

  delay(200);
  fan();
}



//This function reads time from an ds3231 module and package the time as a String
//Parameters: Void
//Returns: time in hh:mm:ss as String
String getTime() {
  DateTime now = rtc.now();
  return String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
}

/*
* This function takes temprature from ds3231 and returns as a float
*Parameters: Void
*Returns: temprature as float 
*/
float getTemp() {
  return rtc.getTemperature();
}

/*
* This function takes a string and draws it to an oled display
*Parameters: - text: String to write to display
*Returns: void
*/
void oledWrite(String text, float temp) {
  u8g.setFont(u8g_font_unifont);

  u8g.firstPage();
  do {
    u8g.drawStr(0, 20, ("Time: " + text).c_str());
    u8g.drawStr(0, 40, ("Temp: " + String(temp)).c_str());
  } while (u8g.nextPage());
}

/*
* takes a temprature value and maps it to corresppnding degree on a servo
*Parameters: - value: temprature
*Returns: void
*/
void servoWrite(float value) {
  myservo.write(map(value * 100, 1500, 3500, 0, 179));
}


void fan() {
  if (rtc.getTemperature() >= 27) {
    digitalWrite(10, HIGH);

  } else {
    digitalWrite(10, LOW);
  }
}