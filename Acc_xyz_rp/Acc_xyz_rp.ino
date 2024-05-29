#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();
  if (mpu.testConnection()) {
    Serial.println("MPU6050 connection successful");
  } else {
    Serial.println("MPU6050 connection failed");
    while (1);
  }

  // mpu.setXAccelOffset(-2684);
  // mpu.setYAccelOffset(-2733);
  // mpu.setZAccelOffset(1491);
}





float vx = 0;
float sx = 0;

float vy = 0;
float sy = 0;

float vz = 0;
float sz = 0;


float delaytime = 1;


float xvel(float ax)
{
  vx = vx + ax * delaytime / 10;
  return vx;
}

float xdisplacement(float sx, float ax)
{
  sx = sx + xvel(ax) * delaytime / 10;
  return sx;
}



float yvel(float ay)
{
  vy = vy + ay * delaytime / 10;
  return vy;
}
float ydisplacement(float sy, float ay)
{
  sy = sy + yvel(ay) * delaytime / 10;
  return sy;
}


float zvel(float az)
{
  vz = vz + az * delaytime / 10;
  return vz;
}
float zdisplacement(float sz, float az)
{
  sz = sz + zvel(az) * delaytime  / 10;
  return sz;
}



void loop() {
  // Raw accelerometer values
  int16_t ax, ay, az;

  // Get raw acceleration values
  mpu.getAcceleration(&ax, &ay, &az);

  // Convert raw values to 'g' values for accelerometer
  float accX = ax / 16384.0 ; // Assuming the accelerometer range is set to ±2g
  float accY = ay / 16384.0 ;
  float accZ = az / 16384.0 ;

  // Calculate pitch and roll angles
  float roll  = atan2(accY, accZ) * 180 / PI;
  float pitch = atan2(-accX, sqrt(accY * accY + accZ * accZ)) * 180 / PI;

  // Serial.print("Roll: "); Serial.print(roll);
  // Serial.print(" Pitch: "); Serial.print(pitch);

  // Convert pitch and roll angles to radians
  float roll_rad  = roll * PI / 180;
  float pitch_rad = pitch * PI / 180;

  // Calculate gravity components
  float gravityX = sin(pitch_rad);
  float gravityY = -sin(roll_rad) * cos(pitch_rad);
  float gravityZ = cos(roll_rad) * cos(pitch_rad);

  // Subtract gravity from accelerometer values
  float linear_accX = accX - gravityX;
  float linear_accY = accY - gravityY;
  float linear_accZ = accZ - gravityZ;

  // Print the linear accelerations


  
  Serial.print(" Linear position X: "); Serial.print(xdisplacement(sx,linear_accX));
  Serial.print(" Linear position Y: "); Serial.print(ydisplacement(sy,linear_accY));
  Serial.print(" Linear position Z: "); Serial.println(zdisplacement(sz,linear_accZ));

  delay(5);
}
