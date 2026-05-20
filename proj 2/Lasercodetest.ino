// Libraries
#include <Servo.h>

Servo servoX;
Servo servoY;


// pin nummmer
int laserPin = 10;
int Laser = 0;


// variabler
int angleX = 90;
int angleY = 90;

void setup() {

  // cummunication
  Serial.begin(9600);


  // hardware
  servoX.attach(9);   
  servoY.attach(8);
  pinMode(laserPin, OUTPUT);
}

void loop() {

  // reads values from python code
  if (Serial.available()) {

    // takes data from python code and saves it in a variable
    String data = Serial.readStringUntil('\n');


    // separates values for the laser from values for the servos and saves it in a variable 
    if (data.startsWith("L:")) {
      Laser = data.substring(2).toInt();
    }


    // reads the data from python by searching for the indexnumber where the comma in the string is (makes it more smooth)
    int commaIndex = data.indexOf(',');

    if (commaIndex > 0) {
      // takes the data from python and with the "commaIndex" saves it in an variable 
      angleX = data.substring(0, commaIndex).toInt();
      angleY = data.substring(commaIndex + 1).toInt();


      // servos reads whats angels are saved in "angleX" and "angleY" 
      servoX.write(angleX);
      servoY.write(angleY);
    }

    // laserpin reads what values where in the "Laser" variable 
    digitalWrite(laserPin, Laser); 
  }
}
