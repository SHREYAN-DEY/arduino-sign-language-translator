/************************************
    scl-----A5   sda-----A4
*************************************/
#include <Wire.h>
#include <MPU6050.h>
#include <SoftwareSerial.h>  // Include SoftwareSerial library

MPU6050 mpu;
#define adc1 A0
#define adc2 A1
#define adc3 A2
#define adc4 A3
#define ledd 13

int flex1 = 0, flex2 = 0, flex3 = 0, flex4 = 0;

// Bluetooth Communication Pins
SoftwareSerial BTSerial(2, 3);  // RX, TX pins for Bluetooth module

unsigned long previousMillis = 0;
const long interval = 1000; // 1 second interval

void setup() {
  pinMode(ledd, OUTPUT);
  digitalWrite(ledd, LOW);
  Serial.begin(9600);       // Start serial communication for debugging
  BTSerial.begin(9600);     // Start Bluetooth serial communication

  // Initialize MPU6050
  Serial.println("Initializing MPU6050...");
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed! Check wiring.");
    while (1);
  }
  Serial.println("MPU6050 Initialized");
}

void loop() {
  // Read flex sensors
  flex1 = analogRead(adc1);
  flex2 = analogRead(adc2);
  flex3 = analogRead(adc3);
  flex4 = analogRead(adc4);

  // Get raw accelerometer data
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  // Convert to G-forces (assuming range ±2G, scale factor = 16384 LSB/G)
  float normAx = ax / 16384.0;
  float normAy = ay / 16384.0;
  float normAz = az / 16384.0;

  // Print accelerometer values
  Serial.print("Xnorm = ");
  Serial.print(normAx);
  Serial.print(" Ynorm = ");
  Serial.print(normAy);
  Serial.print(" Znorm = ");
  Serial.println(normAz);

  // Print flex sensor values to Serial Monitor
  Serial.print(flex1);
  Serial.print("- ");
  Serial.print(flex2);
  Serial.print("- ");
  Serial.print(flex3);
  Serial.print("- ");
  Serial.println(flex4);

  // Non-blocking delay using millis()
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Logic to control LED and print messages based on sensor values
    if ((flex2 < 200) && (flex1 < 200)) {
      digitalWrite(ledd, LOW);
      if (normAccel.XAxis > 5) {
        BTSerial.println("YES");  // Send to Bluetooth
      } else if (normAccel.XAxis < -3) {
        BTSerial.println("NO");
      } else if (normAccel.YAxis > 3) {
        BTSerial.println("OK");
      } else if (normAccel.YAxis < -3) {
        BTSerial.println("BYE");
      }
    }
    else if (flex1 < 200) {
      digitalWrite(ledd, LOW);
      if (normAccel.XAxis > 5) {
        BTSerial.println("THANK YOU");
      } else if (normAccel.XAxis < -5) {
        BTSerial.println("NICE TO MEET YOU");
      } else if (normAccel.YAxis > 5) {
        BTSerial.println("Glad to be here");
      } else if (normAccel.YAxis < -5) {
        BTSerial.println("HELLO, this is my project");
        delay(1500);  // Allow time for message to be visible
      }
    }
    else if (flex2 < 200) {
      digitalWrite(ledd, LOW);
      if (normAccel.XAxis > 5) {
        BTSerial.println("WATER");
      } else if (normAccel.XAxis < -5) {
        BTSerial.println("WHAT IS YOUR NAME");
        delay(500);  // Allow time for message to be visible
      } else if (normAccel.YAxis > 5) {
        BTSerial.println("SORRY");
      } else if (normAccel.YAxis < -5) {
        BTSerial.println("Fine");
      }
    }
    else {
      digitalWrite(ledd, HIGH);
      if (normAccel.XAxis > 5) {
        BTSerial.println("HELP");
      } else if (normAccel.XAxis < -5) {
        BTSerial.println("WASHROOM");
      } else if (normAccel.YAxis > 5) {
        BTSerial.println("Medicine");
      } else if (normAccel.YAxis < -5) {
        BTSerial.println("FOOD");
      }
    }
  }
}
