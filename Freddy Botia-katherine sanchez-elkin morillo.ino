/*
  LiquidCrystal Library - Hello World

  Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
  library works with all LCD displays that are compatible with the
  Hitachi HD44780 driver. There are many of them out there, and you
  can usually tell them by the 16-pin interface.

  This sketch prints "Hello World!" to the LCD
  and shows the time.

  The circuit:
   LCD RS pin to digital pin 12
   LCD Enable pin to digital pin 11
   LCD D4 pin to digital pin 5
   LCD D5 pin to digital pin 4
   LCD D6 pin to digital pin 3
   LCD D7 pin to digital pin 2
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)

  Library originally added 18 Apr 2008
  by David A. Mellis
  library modified 5 Jul 2009
  by Limor Fried (http://www.ladyada.net)
  example added 9 Jul 2009
  by Tom Igoe
  modified 22 Nov 2010
  by Tom Igoe
  modified 7 Nov 2016
  by Arturo Guadalupi

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal.h>
#include "AsyncTaskLib.h"

#define DEBUG(a) Serial.print(millis()); Serial.print(": "); Serial.println(a);

void measure_temp();
void desicion();

AsyncTask asyncTask(1000, measure_temp);
AsyncTask asyncTask1(100, desicion);


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#include "DHTStable.h"

DHTStable DHT;

#define DHT11_PIN 7
#define LED_GREEN 8
#define LED_RED 9
#define LED_BLUE 10

int value_humedad = 0;
int value_temperatura = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  asyncTask.Start();
 asyncTask1.Start();
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

}

void loop() {
  asyncTask.Update(asyncTask1);
  asyncTask1.Update(asyncTask);
}

void desicion() {

Serial.println("Desicion");
  if (value_temperatura > 28) {
    digitalWrite(LED_RED, HIGH);
    //digitalWrite(LED_GREEN, LOW);
    //digitalWrite(LED_BLUE, LOW);

  } else if (value_temperatura < 25) {
    digitalWrite(LED_BLUE, HIGH);
    // digitalWrite(LED_RED, LOW);
    //digitalWrite(LED_GREEN, LOW);

  } else {
    digitalWrite(LED_GREEN, HIGH);
    //digitalWrite(LED_BLUE, LOW);
    //digitalWrite(LED_RED, LOW);
  }


}

void measure_temp() {
  DEBUG("TEMPERATURE");

  int chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:
      Serial.print("OK,\t");
      break;
    case DHTLIB_ERROR_CHECKSUM:
      Serial.print("Checksum error,\t");
      break;
    case DHTLIB_ERROR_TIMEOUT:
      Serial.print("Time out error,\t");
      break;
    default:
      Serial.print("Unknown error,\t");
      break;
  }
  
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE, LOW);
  digitalWrite(LED_RED, LOW);
  value_humedad = DHT.getHumidity();
  value_temperatura = DHT.getTemperature();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("hume:");
  lcd.print(DHT.getHumidity());
  lcd.setCursor(0, 1);
  lcd.print("temp:");
  lcd.println(DHT.getTemperature());

}
