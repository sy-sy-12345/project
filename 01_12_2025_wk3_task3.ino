#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#define trigPin A1
#define echoPin A2
#define leftSensor A4
#define rightSensor A5
int speed = 110;
unsigned long last = 0, lastLCD = 0;
double current_speed = 0,  totalTime;

void setup() {

  lcd.begin(16, 2);

  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);

  last = millis();
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  if (millis() - lastLCD > 1000) {
    lcd.clear();
    lastLCD = millis();

    lcd.print("Time: ");
    lcd.print(totalTime, 1);
    lcd.print("s ");
  }
  updateTime();

  long duration = pulseIn(echoPin, HIGH);

  // Instead of calculating distance, just check pulse duration
  if (duration < 700)
  {
    stop();
    while(1) {}
  }
  else if (duration < 1500) {  
    // Something is close
    int k=0;
    while(k<3)
    {
      turnRight();
      delay(140);
      k++;
    }
    
  } 
  else {
    move(speed);
  }
}

void move(int speed) {
  digitalWrite(1, HIGH);  
  digitalWrite(2, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);

  current_speed = speed;
  analogWrite(3, speed - 20);  
  analogWrite(11, speed);      
}

void stop() {
  digitalWrite(1, LOW);
  digitalWrite(2, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, LOW);

  analogWrite(3, 0);   //left - ENA
  analogWrite(11, 0);  //right - ENB
}

void turnRight() {
  digitalWrite(1, HIGH);
  digitalWrite(2, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);

  analogWrite(3, 255);
  analogWrite(11, 255);
  
}

void updateTime() {
  unsigned long now = millis();
  unsigned long diff = now - last;
  last = now;
  totalTime += (double)diff / 1000.0;
}