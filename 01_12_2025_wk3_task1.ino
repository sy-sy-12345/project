#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#define leftEncoder A1
#define rightEncoder A2
#define leftSensor A4
#define rightSensor A5
const double distancePerHole = 0.010210176;
int speed = 110, leftCount = 0, rightCount = 0, num = 0, i=0;
int laterL = HIGH;
int laterR = HIGH;
unsigned long last = 0, lastLCD = 0;
double current_speed = 0, distance = 0, time = 0, totalTime;

void setup() {
  lcd.begin(16, 2);

  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);

  pinMode(leftEncoder, INPUT_PULLUP);
  pinMode(rightEncoder, INPUT_PULLUP);
  pinMode(leftSensor, INPUT);
  pinMode(rightSensor, INPUT);

  last = millis();
}

//left detect white, right detect black
void loop() {
  updateEncoder();
  updateTime();
  delayy();

  if (num > 110) {
    stop();
    displayDistance();
    while(1){
    }
  }
  if (millis() - lastLCD > 1000) {
    lcd.clear();
    lastLCD = millis();

    lcd.print("Time: ");
    lcd.print(totalTime, 1);
    lcd.print("s ");
  }
  //detect black line (LOW)
  if (digitalRead(leftSensor) == HIGH && digitalRead(rightSensor) == LOW)  //straight
  {
    move(speed);
  } else if (digitalRead(leftSensor) == LOW && digitalRead(rightSensor) == HIGH)  //left
  {
    turnLeft();
  } else if (digitalRead(leftSensor) == HIGH && digitalRead(rightSensor) == HIGH)  //right
  {
    turnRight();
  }
  
  if (digitalRead(leftSensor) == LOW && digitalRead(rightSensor) == LOW)  //stop
  {
    num++;
  } 
  else
  {
    num = 0;
  }
}

void updateEncoder() {
  int nowL = digitalRead(leftEncoder);
  int nowR = digitalRead(rightEncoder);

  if (laterL == HIGH && nowL == LOW) {
    leftCount++;
  }
  if (laterR == HIGH && nowR == LOW) {
    rightCount++;
  }

  laterL = nowL;
  laterR = nowR;
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

void turnLeft() {
  digitalWrite(1, LOW);
  digitalWrite(2, LOW);
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);

  analogWrite(3, 0);
  analogWrite(11, 255); 
  
}

void turnRight() {
  digitalWrite(1, HIGH);
  digitalWrite(2, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);

  analogWrite(3, 200);  
  analogWrite(11, speed);
  
}

void displayTime() {
  while (digitalRead(leftSensor) != LOW && digitalRead(rightSensor) != LOW) {
    lcd.clear();
    time += 1;
    lcd.print((int)time);
    lcd.print("s");
    delay(1000);  //1s
  }
}

void updateTime() {
  unsigned long now = millis();
  unsigned long diff = now - last;
  last = now;
  totalTime += (double)diff / 1000.0;
}

void delayy()
{
  distance = ((double)(leftCount + rightCount) / 2.0) * distancePerHole;
  if(distance>=2.0 && i==0)
  {
    stop();
    displayDistance();
    delay(3000);
    i++;
  }
} 

void displayDistance()  
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print("m");

  lcd.setCursor(0, 1);
  lcd.print("Time: ");
  lcd.print(totalTime);
  lcd.print("s");

}