#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#define left1 A3
SoftwareSerial BT(A4,A5);
#define ENA 3
#define ENB 11
#define left2 2
#define right2 12
#define right1 13
// motor1 is positive; motor2 is negative

int speed = 110;
unsigned long last = 0, lastLCD = 0;
double totalTime;

void setup() {

  BT.begin(9600);
  lcd.begin(16, 2);

  pinMode(left1, OUTPUT);
  pinMode(left2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(right2, OUTPUT);
  pinMode(right1, OUTPUT);

  last = millis();
  stop();
}

void loop() {
  if (BT.available()) {
    char c = BT.read();   

    if (c == 'F') move(speed);
    else if (c == 'B') reverse(speed);
    else if (c == 'L') turnLeft();
    else if (c == 'R') turnRight();
    else if (c == 'S') stop();
  }
  
  updateTime();
  if (millis() - lastLCD > 1000) {
    lcd.clear();
    lastLCD = millis();

    lcd.print("Time: ");
    lcd.print(totalTime, 1);
    lcd.print("s ");
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

void reverse(int speed) {
  digitalWrite(left1, LOW);  
  digitalWrite(left2, HIGH);
  digitalWrite(right2, HIGH);
  digitalWrite(right1, LOW);

  analogWrite(ENA, speed - 20);  
  analogWrite(ENB, speed);     

}

void stop() {
  digitalWrite(left1, LOW);
  digitalWrite(left2, LOW);
  digitalWrite(right2, LOW);
  digitalWrite(right1, LOW);

  analogWrite(ENA, 0);   //left - ENA
  analogWrite(ENB, 0);  //right - ENB
}

void turnLeft() {
  digitalWrite(left1, LOW);
  digitalWrite(left2, LOW);
  digitalWrite(right2, LOW);
  digitalWrite(right1, HIGH);

  analogWrite(ENA, 0);
  analogWrite(ENB, 255);  //+70
  
}

void turnRight() {
  digitalWrite(left1, HIGH);
  digitalWrite(left2, LOW);
  digitalWrite(right2, HIGH);
  digitalWrite(right1, LOW);

  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
  
}

void updateTime() {
  unsigned long now = millis();
  unsigned long diff = now - last;
  last = now;
  totalTime += (double)diff / 1000.0;
}

