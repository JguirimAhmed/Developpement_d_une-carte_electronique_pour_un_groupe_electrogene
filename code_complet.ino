#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h> // Inclure la bibliothèque SoftwareSerial

LiquidCrystal_I2C lcd(0x27, 20, 4); // Adresse I2C de l'afficheur LCD 20x4
#define capteur_huile A3
#define Mode 4
#define Carburant A1
#define Batterie A2
#define STEG 3
#define Vitesse 8
#define ONE_WIRE_BUS 10
#define alarme 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
SoftwareSerial espSerial(2, 3); // RX, TX

const byte PulsesPerRevolution = 2;
const unsigned long ZeroTimeout = 100000;
const byte numReadings = 2;

volatile unsigned long LastTimeWeMeasured;
volatile unsigned long PeriodBetweenPulses = ZeroTimeout + 1000;
volatile unsigned long PeriodAverage = ZeroTimeout + 1000;
unsigned long FrequencyRaw;
unsigned long FrequencyReal;
unsigned long RPM;
unsigned int PulseCounter = 1;
unsigned long PeriodSum;

unsigned long LastTimeCycleMeasure = LastTimeWeMeasured;
unsigned long CurrentMicros = micros();
unsigned int AmountOfReadings = 1;
unsigned int ZeroDebouncingExtra;
unsigned long readings[numReadings];
unsigned long readIndex;  
unsigned long total; 
unsigned long average;

// Variables globales
int P;
int C;
int pourcentage;
int temp;

void setup() {
  pinMode(Mode, INPUT);
  pinMode(STEG, INPUT);
  pinMode(Vitesse, INPUT);
  sensors.begin();
  
  lcd.init(); // Initialiser l'afficheur LCD
  lcd.backlight(); // Activer le rétroéclairage (si disponible)

  Serial.begin(9600); // Initialiser la communication série pour le monitorage
  
  espSerial.begin(115200); // Initialiser la communication série avec l'ESP32

  attachInterrupt(digitalPinToInterrupt(Vitesse), Pulse_Event, RISING);
}

void loop() {
  // huile
  P = analogRead(capteur_huile);
  lcd.setCursor(0, 0); // Positionner le curseur à la première ligne
  lcd.print("H:");
  lcd.print(P);
  lcd.print("B");

  // carburant
  C = analogRead(Carburant);
  lcd.setCursor(0, 1); // Positionner le curseur à la deuxième ligne
  lcd.print("C:");
  lcd.print(C);
  lcd.print("L");

  // batterie
  int B = analogRead(Batterie);
 int voltage = (5.0 * B) / 1023.0;
  pourcentage = (voltage / 5.0) * 100;
  lcd.setCursor(0, 2); // Positionner le curseur à la troisième ligne
  lcd.print("B:");
  lcd.print(pourcentage);
  lcd.print("% ");

  // Mode de fonctionnement 
  if (digitalRead(Mode) == HIGH) {
    lcd.setCursor(11, 0); // Positionner le curseur à la première ligne
    lcd.print("M:Manuel   "); // Afficher "Mode: Manuel"
  } else {
    lcd.setCursor(11, 0); // Positionner le curseur à la première ligne
    lcd.print("M:Auto "); // Afficher "Mode: Auto"
  }


   // température
  sensors.requestTemperatures();
  delay(100);
  temp = sensors.getTempCByIndex(0);
  lcd.setCursor(11, 3);
  lcd.print("T: ");
  lcd.print(temp);
  lcd.print(" C");

  if (P < 2 || C < 50 || B < 25 || temp > 90) {
    digitalWrite(alarme, HIGH); // Activer l'alarme en mettant la broche à HIGH
  } else {
    digitalWrite(alarme, LOW); // Désactiver l'alarme en mettant la broche à LOW si les conditions ne sont pas remplies
  }  

    //vitesse 
  LastTimeCycleMeasure = LastTimeWeMeasured;
  CurrentMicros = micros();
  if (CurrentMicros < LastTimeCycleMeasure) {
    LastTimeCycleMeasure = CurrentMicros;
  }
  FrequencyRaw = 10000000000 / PeriodAverage;
  if (PeriodBetweenPulses > ZeroTimeout - ZeroDebouncingExtra || CurrentMicros - LastTimeCycleMeasure > ZeroTimeout - ZeroDebouncingExtra) {
    FrequencyRaw = 0;  // Set frequency as 0.
    ZeroDebouncingExtra = 2000;
  } else {
    ZeroDebouncingExtra = 0;
  }
  FrequencyReal = FrequencyRaw / 10000;

  RPM = FrequencyRaw / PulsesPerRevolution * 60;
  RPM = RPM / 10000;
  total = total - readings[readIndex];
  readings[readIndex] = RPM;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  average = total / numReadings;

  lcd.setCursor(3, 3);
  lcd.print("V: ");
  lcd.print(RPM);
  lcd.print("tr/min  ");

  // STEG
  if (digitalRead(STEG) == HIGH) {
    lcd.setCursor(11, 1); // Positionner le curseur à la deuxième ligne
    lcd.print("S:ON "); 
  } else {
    lcd.setCursor(11, 1); // Positionner le curseur à la deuxième ligne
    lcd.print("S:OFF ");  
  }
 

  // Envoyer les données à l'ESP32 via Serial
  espSerial.print(P);
  espSerial.print(",");
  espSerial.print(C);
  espSerial.print(",");
  espSerial.print(pourcentage);
  espSerial.print(",");
  espSerial.print(digitalRead(Mode));
  espSerial.print(",");
  espSerial.print(temp);
  espSerial.print(",");
  espSerial.print(RPM);
  espSerial.print(",");
  espSerial.println(digitalRead(STEG));

  delay(2000); // Délai pour laisser le temps de lire l'écran
}

void Pulse_Event() {
  PeriodBetweenPulses = micros() - LastTimeWeMeasured;
  LastTimeWeMeasured = micros();
  if (PulseCounter >= AmountOfReadings)  {
    PeriodAverage = PeriodSum / AmountOfReadings;
    PulseCounter = 1;
    PeriodSum = PeriodBetweenPulses;

    int RemapedAmountOfReadings = map(PeriodBetweenPulses, 40000, 5000, 1, 10);
    RemapedAmountOfReadings = constrain(RemapedAmountOfReadings, 1, 10);
    AmountOfReadings = RemapedAmountOfReadings;
  } else {
    PulseCounter++;
    PeriodSum = PeriodSum + PeriodBetweenPulses;
  }
}
