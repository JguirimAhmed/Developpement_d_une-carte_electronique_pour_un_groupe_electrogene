#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  
#define Mode 4

void setup() {
 pinMode(Mode, INPUT);

  
}
void loop() {
  lcd.init(); 
  lcd.backlight(); 

   if (digitalRead(Mode) == HIGH) {
    lcd.setCursor(11, 0); 
    lcd.print("M:Manuel   "); 
  } else {
    lcd.setCursor(11, 0); 
    lcd.print("M:Auto "); 
  }
  }
  
