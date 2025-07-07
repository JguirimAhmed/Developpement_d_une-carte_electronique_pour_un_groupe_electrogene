#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);  // Adresse I2C de l'afficheur LCD 20x4
#define alarme 2
#define Batterie A2



void setup() {
  pinMode(4, OUTPUT);  

  lcd.init(); // Initialiser l'afficheur LCD
  lcd.backlight(); // Activer le retroeclairage (si disponible)
}

void loop() {
 int B = analogRead(Batterie);
 int voltage = (5.0 * B) / 1023.0;
int  pourcentage = (voltage / 5.0) * 100;
  lcd.setCursor(0, 2); // Positionner le curseur à la troisième ligne
  lcd.print("B:");
  lcd.print(pourcentage);
  lcd.print("% ");

 
 if (pourcentage < 25) {
    digitalWrite(alarme, HIGH); // Activate the alarm if the percentage is less than 25
  } else {
    digitalWrite(alarme, LOW); // Deactivate the alarm otherwise
  }
  delay(1000); 
}
