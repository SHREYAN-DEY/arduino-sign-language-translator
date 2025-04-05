#include <Wire.h>
#include <MPU6050.h>
#include <SoftwareSerial.h>

// Bluetooth Module Setup
SoftwareSerial BTSerial(10, 11);  // RX, TX pins for Bluetooth module

MPU6050 mpu;

// Flex sensor pins
#define adc1 A0
#define adc2 A1
#define adc3 A2
#define adc4 A3
#define ledd 13

// Flex thresholds
const int flexThreshold = 1023;
int flex1 = 0, flex2 = 0, flex3 = 0, flex4 = 0;

// Timing variables
unsigned long previousMillis = 0;
const long interval = 1000;  // 1 second

// I2C address variables
uint8_t mpuAddress = 0x68;  // Default
bool mpuConnected = false;

void setup() {
  pinMode(ledd, OUTPUT);
  digitalWrite(ledd, LOW);
  Serial.begin(9600);       // Serial Monitor
  BTSerial.begin(9600);     // Bluetooth Communication
  delay(1000);
  Wire.begin();

  Serial.println("Scanning for I2C devices...");
  mpuAddress = scanI2C();
  
  if (mpuAddress == 0x00) {
    Serial.println("MPU6050 not found. Check wiring and try again.");
    while (1);
  }

  Serial.print("MPU6050 found at address 0x");
  Serial.println(mpuAddress, HEX);

  mpu.initialize();  // Initialize MPU6050
  
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 initialization failed!");
    while (1);
  }

  Serial.println("MPU6050 initialized successfully.");
}

void loop() {
  // Read flex sensor values
  flex1 = analogRead(adc1);
  flex2 = analogRead(adc2);
  flex3 = analogRead(adc3);
  flex4 = analogRead(adc4);

  // Read accelerometer data
  int16_t ax, ay, az;
  mpu.getAcceleration(&ax, &ay, &az);

  float normAx = ax / 16384.0;
  float normAy = ay / 16384.0;
  float normAz = az / 16384.0;

  Serial.print("Xnorm = "); Serial.print(normAx);
  Serial.print(" Ynorm = "); Serial.print(normAy);
  Serial.print(" Znorm = "); Serial.println(normAz);

  Serial.print("Flex: ");
  Serial.print(flex1); Serial.print("-");
  Serial.print(flex2); Serial.print("-");
  Serial.print(flex3); Serial.print("-");
  Serial.println(flex4);

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    String message = interpretGesture(normAx, normAy, flex1, flex2);

    if (message != "") {
      Serial.println(message);
      BTSerial.println(message);  // Send over Bluetooth
      digitalWrite(ledd, LOW);
    } else {
      Serial.println("Idle...");
      digitalWrite(ledd, HIGH);
    }
  }
}

// Interpret flex and motion input
String interpretGesture(float ax, float ay, int f1, int f2) {
  if (f1 < flexThreshold && f2 < flexThreshold) {
    if (ax > 0.3) return "YES";
    if (ax < -0.2) return "NO";
    if (ay > 0.2) return "OK";
    if (ay < -0.2) return "BYE";
  }
  else if (f1 < flexThreshold) {
    if (ax > 0.3) return "THANK YOU";
    if (ax < -0.3) return "NICE TO MEET YOU";
    if (ay > 0.3) return "Glad to be here";
    if (ay < -0.3) return "HELLO, this is my project";
  }
  else if (f2 < flexThreshold) {
    if (ax > 0.3) return "WATER";
    if (ax < -0.3) return "WHAT IS YOUR NAME";
    if (ay > 0.3) return "SORRY";
    if (ay < -0.3) return "FINE";
  }
  else {
    if (ax > 0.3) return "HELP";
    if (ax < -0.3) return "WASHROOM";
    if (ay > 0.3) return "MEDICINE";
    if (ay < -0.3) return "FOOD";
  }

  return "";
}

// 🔍 I2C Scanner to detect MPU6050 address
uint8_t scanI2C() {
  for (uint8_t address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      return address;  // Found device
    }
  }
  return 0x00;  // No device found
}
