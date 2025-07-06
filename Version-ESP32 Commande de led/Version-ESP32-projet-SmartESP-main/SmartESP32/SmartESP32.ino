#include "SmartESP32Utils.h"              
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// ---------- Identifiants Wi-Fi ----------
const char* ssid = "N1";
const char* password = "yannyann";

// ---------- Variables globales ----------
AsyncWebServer server(80);
String lastCommand = "";                
AutoUpdatePayload payload;              
unsigned long lastUpdateTime = 0;
const unsigned long updateInterval = 500;  

// ---------- Déclaration des broches GPIO ----------
int Salon = 23;       // GPIO23
int Chambre = 22;     // GPIO22
int Cuisine = 21;     // GPIO21

// ---------- Initialisation ----------
void setup() {
  Serial.begin(115200);                        
  connectToWiFi(ssid, password);               
  setupWebSocket(server, &lastCommand);        

  // Configuration des broches en sortie
  pinMode(Salon, OUTPUT);
  pinMode(Chambre, OUTPUT);
  pinMode(Cuisine, OUTPUT);
}

// ---------- Boucle principale ----------
void loop() {
  Smartcommande();     
  // Autoupdate();     
}

// ---------- Fonction pour traiter les commandes WebSocket ----------
void Smartcommande() {
  if (!lastCommand.isEmpty()) {

    if (lastCommand == "Salon") {
      digitalWrite(Salon, !digitalRead(Salon));                         // Inverser état
      payload.bulb1 = digitalRead(Salon) ? "true" : "false";           // Mettre à jour l’état
    } else if (lastCommand == "Chambre") {
      digitalWrite(Chambre, !digitalRead(Chambre));
      payload.bulb2 = digitalRead(Chambre) ? "true" : "false";
    } else if (lastCommand == "Cuisine") {
      digitalWrite(Cuisine, !digitalRead(Cuisine));
      payload.bulb3 = digitalRead(Cuisine) ? "true" : "false";
    } else {
      payload.notif = "commande incorrecte !";                        // Message d’erreur
    }

    lastCommand = "";                                                 
    sendAutoUpdate(payload);                                         
    payload.notif = "false";                                         
    payload.ia = "false";
  }
}

// ---------- Optionnel : Fonction de mise à jour automatique ----------
void Autoupdate() {
  if (millis() - lastUpdateTime > updateInterval) {
    payload.ecran1 = String(millis());     // Mettre à jour le champ ecran1 avec le temps
    sendAutoUpdate(payload);
    lastUpdateTime = millis();             // Met à jour le dernier temps d’envoi
  }
}
