#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define ONE_WIRE_BUS 10

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  sensors.begin();
  Serial.begin(9600);

  
  lcd.init();
  lcd.backlight();
}

void loop() {
  sensors.requestTemperatures();
  float temp = sensors.getTempCByIndex(0);

  lcd.setCursor(11, 3);
  lcd.print("T: ");
  lcd.print(temp);
  lcd.print(" C");

  // pour serial moniteur
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" C");

  delay(1000);
}
