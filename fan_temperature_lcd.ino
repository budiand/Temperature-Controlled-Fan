#include <IRremote.hpp>

#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// temeperatura
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// motor
int motorPin = 3;
// temperatura
float maxTemp;

// lumina
int photoPin = A0;
int ledPin = 7;

// infrared
IRrecv IR(5);
int curr = 0;
int prev = 0;
int count = 0;

byte cerculet[8] = {
  0b01000,
  0b10100,
  0b01000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

void setup()
{
  // lcd
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, cerculet);
  lcd.clear();

  // temperatura
  sensors.begin();
  Serial.begin(9600);               

  // motor
  pinMode(motorPin, OUTPUT);

  // lumina
  pinMode(ledPin, OUTPUT);

  //infrared
  IR.enableIRIn();
}


void loop()
{
  sensors.requestTemperatures();
  int speed = 70;
  
  //analogWrite(motorPin, speed);
  
  maxTemp = sensors.getTempCByIndex(0);
  
  if(maxTemp < 28) 
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Temperatura in");
    lcd.setCursor(0,1);
    lcd.print("camera: ");
    lcd.print(sensors.getTempCByIndex(0));
    lcd.write((byte)0);
    lcd.print("C");
    // Serial.print("TEmp:");
    // Serial.print(sensors.getTempCByIndex(0));
    analogWrite(motorPin, 0);
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temperatura");
    lcd.print(maxTemp);
    lcd.print((byte)0);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("e peste limita!");

    analogWrite(motorPin, speed);
  }

  int lightRaw = analogRead(photoPin);
  int light = map(lightRaw, 1023, 0, 10, 0);

  if(light < 1) 
    digitalWrite(ledPin, HIGH);
  else
    digitalWrite(ledPin, LOW);

// infrared with motor 
    //curr = analogRead(motorPin);
    //Serial.println(curr);
    if(IR.decode())
    {
      Serial.println(IR.decodedIRData.decodedRawData, HEX);
      if(IR.decodedIRData.decodedRawData == 0xEA15FF00)
      {
        analogWrite(motorPin, speed);
        // if(curr != prev)
        // {
        //   if(curr <= 75)
        //   {
        //     count++;
        //     if(count >= 1)
        //       count = 0;
        //   }
      }
      if(IR.decodedIRData.decodedRawData == 0xF807FF00)
      {
        analogWrite(motorPin, 0);
      }
        // prev = curr;
        // switch(count)
        // {
        //   case 0:
        //     analogWrite(motorPin, speed);
        //     break;
        //   case 1:
        //     analogWrite(motorPin, 0);
        //     break;
        // }
    }
    IR.resume();
    delay(1000);
}
