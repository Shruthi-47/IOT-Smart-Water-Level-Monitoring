#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

// Ultrasonic sensor pins
#define trigPin 12
#define echoPin 13

// Buzzer pin
#define buzzer 9

// Tank height (cm)
const int maxTankDepth = 10;

// Buzzer alert level
const int buzzerThreshold = 80;

// LCD setup
LiquidCrystal_I2C lcd(0x27,16,2);

// Ultrasonic setup
NewPing sonar(trigPin, echoPin, 200);

void setup()
{
  lcd.begin();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("Water Level");

  lcd.setCursor(0,1);
  lcd.print("Measurement");

  delay(2000);

  pinMode(buzzer, OUTPUT);
  digitalWrite(buzzer, LOW);

  Serial.begin(9600);
}

void loop()
{
  int total = 0;

  // Take 5 readings for stability
  for(int i=0;i<5;i++)
  {
    total += sonar.ping_cm();
    delay(50);
  }

  int distance = total / 5;

  // If sensor returns 0
  if(distance == 0)
  {
    distance = maxTankDepth;
  }

  // Calculate water level %
  int waterLevel = ((maxTankDepth - distance) * 100) / maxTankDepth;
  waterLevel = constrain(waterLevel,0,100);

  // Serial Monitor output
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm  Water Level: ");
  Serial.print(waterLevel);
  Serial.println(" %");

  // LCD display
  lcd.setCursor(0,0);
  lcd.print("Water Level: ");
  lcd.print(waterLevel);
  lcd.print("%   ");

  // Buzzer alert
  if(waterLevel >= buzzerThreshold)
  {
    digitalWrite(buzzer,HIGH);
    delay(3000);
    digitalWrite(buzzer,LOW);
  }

  delay(1000);
}