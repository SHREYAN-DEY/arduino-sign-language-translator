#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;
#define adc1 A0
#define adc2 A1
#define adc3 A2
#define adc4 A3
#define ledd 13

int flex1 = 0, flex2 = 0, flex3 = 0, flex4 = 0;

// Timing variables for non-blocking delay
unsigned long previousMillis = 0;
const long interval = 1000; // 1 second interval

void setup() {
  pinMode(ledd, OUTPUT);
  digitalWrite(ledd, LOW);
  Serial.begin(9600);

  // Initialize MPU6050
  Serial.println("Initializing MPU6050...");
  mpu.initialize();
  
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed! Check wiring.");
    while (1);
  }

  Serial.println("MPU6050 Connected.");
}

void loop() {
  // Read flex sensor values
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

  // Print flex sensor values
  Serial.print(flex1);
  Serial.print("-");
  Serial.print(flex2);
  Serial.print("-");
  Serial.print(flex3);
  Serial.print("-");
  Serial.println(flex4);

  // Non-blocking delay using millis()
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Logic to control LED and print messages based on sensor values
    if ((flex2 < 200) && (flex1 < 200)) {
      digitalWrite(ledd, LOW);
      if (normAx > 0.3) {
        Serial.println("YES");
      } else if (normAx < -0.2) {
        Serial.println("NO");
      } else if (normAy > 0.2) {
        Serial.println("OK");
      } else if (normAy < -0.2) {
        Serial.println("BYE");
      }
    }
    else if (flex1 < 200) {
      digitalWrite(ledd, LOW);
      if (normAx > 0.3) {
        Serial.println("THANK YOU");
      } else if (normAx < -0.3) {
        Serial.println("NICE TO MEET YOU");
      } else if (normAy > 0.3) {
        Serial.println("Glad to be here");
      } else if (normAy < -0.3) {
        Serial.println("HELLO, this is my project");
        delay(1500);  // Allow time for message to be visible
      }
    }
    else if (flex2 < 200) {
      digitalWrite(ledd, LOW);
      if (normAx > 0.3) {
        Serial.println("WATER");
      } else if (normAx < -0.3) {
        Serial.println("WHAT IS YOUR NAME");
        delay(500);  // Allow time for message to be visible
      } else if (normAy > 0.3) {
        Serial.println("SORRY");
      } else if (normAy < -0.3) {
        Serial.println("Fine");
      }
    }
    else {
      digitalWrite(ledd, HIGH);
      if (normAx > 0.3) {
        Serial.println("HELP");
      } else if (normAx < -0.3) {
        Serial.println("WASHROOM");
      } else if (normAy > 0.3) {
        Serial.println("Medicine");
      } else if (normAy < -0.3) {
        Serial.println("FOOD");
      }
    }
  }
}
