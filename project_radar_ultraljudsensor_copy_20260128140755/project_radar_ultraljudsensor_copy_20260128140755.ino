// Libraries
#include "U8glib.h"
#include <Servo.h>

Servo Servo1;
Servo Servo2;





// pin nummmer
const int trigPin = 5;
const int echoPin = 4;
const int buzzer = 8;
const int laserPin = 6;
const float laser_length = 15.0;


// variabler
long duration;
int distance;
int laserPos = 0;
int closest = 4000;

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);

void setup() {
  // Communication
  Serial.begin(9600);

  // hardware
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(10, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(laserPin, OUTPUT);
  Servo1.attach(3);
  Servo2.attach(2);
}

void loop() {

  int tempdistance = 50;
  int laserPos = 0;


  for (int pos = 0; pos <= 180; pos = pos + 5) {

    Servo1.write(pos);
    delay(15);


    mesure();


    if (distance > 0 && distance <= tempdistance) {


      digitalWrite(laserPin, 1);

      float theta = radians(pos);
      float d = distance;

      // Law of cosines
      float c = sqrt(d * d + 15.0 * 15.0 - 2 * d * 15.0 * cos(theta));

      // Law of sines
      float alpha = asin((15.0 * sin(theta)) / c);

      // Laser is 15 cm to the LEFT
      int laserAngle = degrees(theta + alpha);

      // Safety
      laserAngle = constrain(laserAngle, 0, 180);

      Servo2.write(laserAngle);

    } else {
      digitalWrite(laserPin, 0);
    }
    oledWrite(distance);

    if (distance > 10) {
      digitalWrite(13, HIGH);
      digitalWrite(12, HIGH);
      digitalWrite(9, LOW);
      digitalWrite(11, LOW);
      noTone(buzzer);
    } else {
      digitalWrite(9, HIGH);
      digitalWrite(11, HIGH);
      digitalWrite(13, LOW);
      digitalWrite(12, LOW);
      tone(buzzer, 1000);
    }

   // Servo2.write(laserPos);
    delay(15);
  }
}


void oledWrite(float distance) {
  u8g.setFont(u8g_font_unifont);

  u8g.firstPage();
  do {
    u8g.drawStr(0, 20, ("Distance: " + String(distance)).c_str());

    if (distance < 10) {
      u8g.drawStr(0, 40, "object identified");
    } else {
      u8g.drawStr(0, 40, "area cleared");
    }
  } while (u8g.nextPage());
}

void mesure() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);  // clears the trigPin

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  // Starts generating the ultra sound (HIGH)

  duration = pulseIn(echoPin, HIGH);  // reads echoPin, returns the sound wave travle time in microseconds
  distance = duration * 0.034 / 2;    // calculating the distance
  Serial.print("distance: ");         // prints distance on the serial monitor
  Serial.println(distance);
}
