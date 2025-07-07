#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4); 
#define  alarme 2
#define capteur_huile A3


void setup() {
  lcd.init(); 
  lcd.backlight();

  pinMode(13, OUTPUT);  
  pinMode(A3,INPUT);

  
}
void loop() {
 
int P = analogRead(capteur_huile);
  lcd.setCursor(0, 0); // Positionner le curseur à la première ligne
  lcd.print("H:");
  lcd.print(P);
  lcd.print("B");


  if (P < 50) {
    digitalWrite(alarme, HIGH); 
  } else 
    digitalWrite(alarme, LOW);
  
  
  delay(1000); 
}
