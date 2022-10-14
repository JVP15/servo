#include <Servo.h>
#include <Wire.h>

Servo myservo;  // create servo object to control a servo
int pos = 90; // always start with the appendage pointing upwards   

const int MPU = 0x68; // MPU6050 I2C address
float AccX, AccY, AccZ;
float accAngleX; 

void get_acceleration(){
  // this whole function was taken directly from https://howtomechatronics.com/tutorials/arduino/arduino-and-mpu6050-accelerometer-and-gyroscope-tutorial/
  // === Read acceleromter data === //
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  //For a range of +-2g, we need to divide the raw values by 16384, according to the datasheet
  AccX = (Wire.read() << 8 | Wire.read()) / 16384.0; // X-axis value
  AccY = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
  AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0; // Z-axis value

  accAngleX = (atan(AccY / sqrt(pow(AccX, 2) + pow(AccZ, 2))) * 180 / PI); // standard way of calculating roll
}

void setup() {
  myservo.attach(9);
  
  Serial.begin(9600);
  
  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
}

void loop() {
  get_acceleration();
  
  Serial.print("X Acceleration: ");
  Serial.print(AccX);
  
  Serial.print(", Y Acceleration: ");
  Serial.print(AccY);

  Serial.print(", Z Acceleration: ");
  Serial.print(AccZ);
  
  Serial.print("\n");
  
  if(Serial.available()) {
    pos = Serial.parseInt();
    Serial.print("New Position = ");
    
    Serial.print(pos);
    Serial.print("\n");
  }

  myservo.write(pos + accAngleX);
  delay(15);
}
