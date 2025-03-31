
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
  Serial.println("Initialize MPU6050");
  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)) {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  Serial.println("MPU6050 Initialized");
  checkSettings();
}

void checkSettings() {
  Serial.println();
  Serial.print(" * Sleep Mode:            ");
  Serial.println(mpu.getSleepEnabled() ? "Enabled" : "Disabled");

  Serial.print(" * Clock Source:          ");
  switch (mpu.getClockSource()) {
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }

  Serial.print(" * Accelerometer offsets: ");
  Serial.print(mpu.getAccelOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getAccelOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getAccelOffsetZ());
  Serial.println();
}

void loop() {
  // Read flex sensors
  flex1 = analogRead(adc1);
  flex2 = analogRead(adc2);
  flex3 = analogRead(adc3);
  flex4 = analogRead(adc4);

  // Get normalized accelerometer values
  Vector normAccel = mpu.readNormalizeAccel();

  // Print normalized accelerometer data
  Serial.print("Xnorm = ");
  Serial.print(normAccel.XAxis);
  Serial.print(" Ynorm = ");
  Serial.print(normAccel.YAxis);
  Serial.print(" Znorm = ");
  Serial.println(normAccel.ZAxis);

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
      if (normAccel.XAxis > 5) {
        Serial.println("YES");
      } else if (normAccel.XAxis < -3) {
        Serial.println("NO");
      } else if (normAccel.YAxis > 3) {
        Serial.println("OK");
      } else if (normAccel.YAxis < -3) {
        Serial.println("BYE");
      }
    }
    else if (flex1 < 200) {
      digitalWrite(ledd, LOW);
      if (normAccel.XAxis > 5) {
        Serial.println("THANK YOU");
      } else if (normAccel.XAxis < -5) {
        Serial.println("NICE TO MEET YOU");
      } else if (normAccel.YAxis > 5) {
        Serial.println("Glad to be here");
      } else if (normAccel.YAxis < -5) {
        Serial.println("HELLO, this is my project");
        delay(1500);  // Allow time for message to be visible
      }
    }
    else if (flex2 < 200) {
      digitalWrite(ledd, LOW);
      if (normAccel.XAxis > 5) {
        Serial.println("WATER");
      } else if (normAccel.XAxis < -5) {
        Serial.println("WHAT IS YOUR NAME");
        delay(500);  // Allow time for message to be visible
      } else if (normAccel.YAxis > 5) {
        Serial.println("SORRY");
      } else if (normAccel.YAxis < -5) {
        Serial.println("Fine");
      }
    }
    else {
      digitalWrite(ledd, HIGH);
      if (normAccel.XAxis > 5) {
        Serial.println("HELP");
      } else if (normAccel.XAxis < -5) {
        Serial.println("WASHROOM");
      } else if (normAccel.YAxis > 5) {
        Serial.println("Medicine");
      } else if (normAccel.YAxis < -5) {
        Serial.println("FOOD");
      }
    }
  }
}
