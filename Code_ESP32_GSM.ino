#include <HardwareSerial.h> // Inclure la bibliothèque HardwareSerial

HardwareSerial SerialGSM(2); // Utiliser Serial2 pour le module GSM

// Seuils pour l'alarme
const int batterieSeuil = 25;
const int carburantSeuil = 50;
const int huileSeuil = 2;
const int tempSeuil = 90;

// Variables pour les capteurs (valeurs initialisées à des valeurs non critiques)
int B = 100;
int C = 100;
int P = 10;
int temp = 25;

void setup() {
  // Initialiser la communication série
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 16, 17); // Initialiser Serial2 avec RX sur 16 et TX sur 17
  SerialGSM.begin(9600, SERIAL_8N1, 16, 17); // Initialiser Serial2 pour le module GSM sur les broches RX et TX

  // Initialiser le module GSM
  initializeGSM();
}

void loop() {
  // Vérifier si des données sont disponibles depuis l'Arduino
  if (Serial2.available()) {
    String data = Serial2.readStringUntil('\n');
    Serial.println("Données reçues de l'Arduino : " + data);

    // Parser les données reçues et mettre à jour les variables des capteurs
   
  }

  // Vérifier les conditions et envoyer un SMS si une condition est remplie
  if (B < batterieSeuil || C < carburantSeuil || P < huileSeuil || temp > tempSeuil) {
    envoyerSMS("Alerte: Probleme dans le groupe electrogéne");
  }

  delay(2000);
}

void initializeGSM() {
  // Envoyer des commandes AT pour initialiser le module GSM
  SerialGSM.println("AT");
  delay(1000);
  SerialGSM.println("AT+CMGF=1"); // Configurer le mode texte pour SMS
  delay(1000);
  SerialGSM.println("AT+CNMI=1,2,0,0,0"); // Configurer le module pour recevoir des SMS
  delay(1000);
}

void envoyerSMS(String message) {
  SerialGSM.println("AT+CMGS=\"+55373370\""); // Remplacer par le numéro de téléphone de l'utilisateur
  delay(1000);
  SerialGSM.println(message); // Envoyer le message
  delay(1000);
  SerialGSM.write(26); // Envoyer le caractère Ctrl+Z pour terminer le SMS
  delay(1000);
}
