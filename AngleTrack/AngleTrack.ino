#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

float roll = 0, pitch = 0, yaw = 0;
const float alpha = 0.98; // Complementary filter constant
const float driftCorrectionFactor = 0.3; // Adjust this value based on the observed drift
unsigned long previousTime, currentTime;
float dt;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  byte status = mpu.begin();
  if (status != 0) {
    Serial.print(F("MPU6050 initialization failed with code: "));
    Serial.println(status);
    while (1) delay(10);
  }

  Serial.println(F("MPU6050 initialized successfully"));

  // Initialize time
  previousTime = millis();
}

void loop() {
  mpu.update();

  // Calculate elapsed time in seconds
  currentTime = millis();
  dt = (currentTime - previousTime) / 1000.0;
  previousTime = currentTime;

  // Get accelerometer data
  float accelX = mpu.getAccX();
  float accelY = mpu.getAccY();
  float accelZ = mpu.getAccZ();

  // Get gyroscope data
  float gyroX = mpu.getGyroX();
  float gyroY = mpu.getGyroY();
  float gyroZ = mpu.getGyroZ();

  // Calculate pitch and roll from accelerometer data
  float accelRoll = atan2(accelY, accelZ) * 180 / PI;
  float accelPitch = atan2(-accelX, sqrt(accelY * accelY + accelZ * accelZ)) * 180 / PI;

  // Gyroscope data is in degrees/sec, no need to convert to radians/sec
  float gyroRollRate = gyroX; // already in degrees/sec
  float gyroPitchRate = gyroY; // already in degrees/sec
  float gyroYawRate = gyroZ; // already in degrees/sec

  // Apply complementary filter for roll and pitch
  roll = alpha * (roll + gyroRollRate * dt) + (1 - alpha) * accelRoll;
  pitch = alpha * (pitch + gyroPitchRate * dt) + (1 - alpha) * accelPitch;

  // Integrate gyroscope data for yaw with drift correction
  yaw += gyroYawRate * dt;
  yaw -= driftCorrectionFactor * dt; // Apply drift correction

  // Ensure yaw stays within 0-360 degrees
  if (yaw >= 360.0) yaw -= 360.0;
  else if (yaw < 0.0) yaw += 360.0;

  // Display the results for Serial Plotter
  Serial.print(roll, 2);
  Serial.print(",");
  Serial.print(pitch, 2);
  Serial.print(",");
  Serial.println(yaw, 2);

  delay(10); // Adjust delay as needed
}
