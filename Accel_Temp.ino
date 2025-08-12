
#include <Wire.h>
#include <MPU6050.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// MPU6050 setup
MPU6050 mpu;

// DS18B20 setup
#define ONE_WIRE_BUS PA5  // Pin connected to DS18B20
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);

  // Initialize MPU6050
  Wire.begin();
  mpu.initialize();
  if (mpu.testConnection()) {
    Serial.println("✅ MPU6050 connected successfully!");
  } else {
    Serial.println("❌ MPU6050 connection failed! Check wiring.");
    while (1);
  }

  // Initialize DS18B20
  sensors.begin();
  delay(1000);
  Serial.println("✅ DS18B20 initialized!");
}

void loop() {
  // Read temperature from DS18B20
  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  // Get MPU6050 motion data (Accelerometer and Gyroscope)
  int16_t ax, ay, az;
  int16_t gx, gy, gz;
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Convert raw accelerometer data to g-force
  float Ax = ax / 16384.0;
  float Ay = ay / 16384.0;
  float Az = az / 16384.0;

  // Calculate total acceleration magnitude
  float accMagnitude = sqrt(Ax * Ax + Ay * Ay + Az * Az);

  // Display Temperature and Acceleration Data
  Serial.print("Temperature: ");
  Serial.print(tempC);
  Serial.println(" °C");

  Serial.print("Accel [g]: X=");
  Serial.print(Ax, 2);
  Serial.print(" Y=");
  Serial.print(Ay, 2);
  Serial.print(" Z=");
  Serial.print(Az, 2);

  Serial.print(" | Gyro [°/s]: X=");
  Serial.print(gx);
  Serial.print(" Y=");
  Serial.print(gy);
  Serial.print(" Z=");
  Serial.print(gz);

  Serial.print(" | Acceleration Magnitude: ");
  Serial.print(accMagnitude, 2);

  // Fall detection (if acceleration exceeds threshold)
  if (accMagnitude > 2.5) {
    Serial.println(" ⚠ FALL DETECTED!");
    delay(2000);  // Prevent repeated fall detection
  } else {
    Serial.println();
  }

  delay(300);  // Refresh every 300 ms
}
