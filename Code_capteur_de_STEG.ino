#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);  

#define STEG 3

void setup() {
  pinMode(STEG, INPUT); 
  lcd.init(); 
  lcd.backlight(); 
}

void loop() {
  if (digitalRead(STEG) == HIGH) {
    lcd.setCursor(11, 1);
    lcd.print("S:ON      ");
  } else {
    lcd.setCursor(11, 1); 
    lcd.print("S:OFF ");  
  }
  delay(1000); 
}
