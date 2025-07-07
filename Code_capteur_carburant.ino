#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); 
#define  alarme 2
#define Carburant A1


void setup() {
  lcd.init(); 
  lcd.backlight();

  pinMode(13, OUTPUT);  
  pinMode(A0,INPUT);

  
}
void loop() {
 

   int C = analogRead(Carburant);
  lcd.setCursor(0, 1); // 
  lcd.print("C:");
  lcd.print(C);
  lcd.print("L");


  if (C < 50) {
    tone(13,900,100); 
  
  }
  delay(1000); 
}
