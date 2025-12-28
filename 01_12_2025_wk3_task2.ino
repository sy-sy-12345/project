#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
Adafruit_MPU6050 mpu;
#define leftSensor A1
#define rightSensor A2
#define left1 A3
#define mpuSDA A4
#define mpuSCL A5
#define ENA 3
#define ENB 11
#define left2 2
#define right2 12
#define right1 13
int speed = 200, num = 0, max = 0 ;
float pitch = 0.0;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    delay(10);

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  lcd.begin(16, 2);

  pinMode(left1, OUTPUT);
  pinMode(left2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(right2, OUTPUT);
  pinMode(right1, OUTPUT);
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);

  move (speed);
  delay(1500);
}

void loop() {
  lcd.clear();
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float ax = (a.acceleration.x); //left and right
  float ay = (a.acceleration.y); //up and down
  float az = (a.acceleration.z)-1.66; //gravity = 9.81

  pitch = atan2(ay, az) * 180.0 / PI;
  float magnitude = sqrt(ax*ax + ay*ay + az*az);
  float angle = (acos(az / magnitude) * 180.0 / PI)-3.65;

  if (pitch > max)
  {
    max = pitch;
  }

  Serial.print(" Tilt: ");
  Serial.print(angle); 
  Serial.print(" Pitch: ");
  Serial.print(pitch); //useful has + - sign
  Serial.print(" max: ");
  Serial.println(max);

  //only show the max angle on LCD
  lcd.print(max);
  delay(50);

  if (pitch > 10.0)
  {
    speed = 230;
    move(speed);
  }
  else if (pitch < 10.0 && pitch >=-10.0 && speed == 230)
  {
    spin360();
  }
  else 
  {
    move(speed);
  }
}

void move(int speed) {
  digitalWrite(left1, HIGH);  
  digitalWrite(left2, LOW);
  digitalWrite(right2, LOW);
  digitalWrite(right1, HIGH);

  analogWrite(ENA, speed - 20);  
  analogWrite(ENB, speed);   

}

void spin360() {
  while(num < 1)
  {
    stop();
    delay(4000);

    // Reset timer & gyroscope angle
    float angleTurned = 0;
    sensors_event_t a, g, temp;
    
    // Spin clockwise
    turnRight();

    unsigned long lastTime = millis();

    while(angleTurned < 340.0) {
      mpu.getEvent(&a, &g, &temp);
      unsigned long now = millis();
      float dt = (now - lastTime) / 1000.0;  // seconds
      lastTime = now;

      // g.gyro.z is angular velocity in rad/s
      float degPerSec = g.gyro.z * 180.0 / PI;
      angleTurned += abs(degPerSec * dt);
    }
    
    stop();
    delay(1000);
    num++;
    speed = 200;
  }
}

void stop() {
  digitalWrite(left1, LOW);
  digitalWrite(left2, LOW);
  digitalWrite(right2, LOW);
  digitalWrite(right1, LOW);

  analogWrite(ENA, 0);   //left - ENA
  analogWrite(ENB, 0);  //right - ENB
}

void turnRight() {
  digitalWrite(left1, HIGH);
  digitalWrite(left2, LOW);
  digitalWrite(right2, HIGH);
  digitalWrite(right1, LOW);

  analogWrite(ENA, 230);
  analogWrite(ENB, 255);
}
