// On inclut les bibliothèques nécessaires
#include "SmartESP8266Utils.h"       // Fichier avec des fonctions personnalisées (WebSocket, mise à jour...)
#include <ESP8266WiFi.h>             // Pour connecter l'ESP8266 au WiFi
#include <ESPAsyncWebServer.h>       // Pour créer un serveur web asynchrone

// ---------- Paramètres WiFi ----------
const char* ssid = "N1";             // Nom du réseau WiFi
const char* password = "yannyann";   // Mot de passe du réseau WiFi

// ---------- Variables globales ----------
AsyncWebServer server(80);           
String lastCommand = "";             
AutoUpdatePayload payload;           
unsigned long lastUpdateTime = 0;    
const unsigned long updateInterval = 500; 

// ---------- Déclaration des broches (pins) ----------
int Salon = D1;
int Chambre = D2;
int Cuisine = D3;

// ---------- Fonction d'initialisation ----------
void setup() {
  Serial.begin(115200);                          
  connectToWiFi(ssid, password);                 
  setupWebSocket(server, &lastCommand);           

  // Configuration des broches en sortie
  pinMode(Salon, OUTPUT);
  pinMode(Chambre, OUTPUT);
  pinMode(Cuisine, OUTPUT);
}

// ---------- Fonction qui tourne en boucle ----------
void loop() {



  
  Smartcommande();     // Vérifie s’il y a une commande à exécuter
  // Autoupdate();     // Optionnel : active les mises à jour automatiques si décommenté
}

// ---------- Fonction pour exécuter une commande reçue ----------
void Smartcommande() {
  // Si une commande a été reçue via WebSocket
  if (!lastCommand.isEmpty()) {

    // Si la commande est "Salon", on inverse l'état de la LED du salon
    if (lastCommand == "Salon") {
      digitalWrite(Salon, !digitalRead(Salon));  // Inverser l'état actuel
      payload.bulb1 = digitalRead(Salon) ? "true" : "false";  // Met à jour l'état dans le payload
    } 
    // Même logique pour "Chambre"
    else if (lastCommand == "Chambre") {
      digitalWrite(Chambre, !digitalRead(Chambre));
      payload.bulb2 = digitalRead(Chambre) ? "true" : "false";
    } 
    // Même logique pour "Cuisine"
    else if (lastCommand == "Cuisine") {
      digitalWrite(Cuisine, !digitalRead(Cuisine));
      payload.bulb3 = digitalRead(Cuisine) ? "true" : "false";
    } 
    // Si la commande est inconnue
    else {
      payload.notif = "commande incorrecte !";
    }

    // On réinitialise la commande
    lastCommand = "";

    // Envoi de l’état actuel à l’interface utilisateur via WebSocket
    sendAutoUpdate(payload);

    // On réinitialise les notifications et IA après l’envoi
    payload.notif = "false";
    payload.ia = "false";
  }
}

// ---------- Fonction de mise à jour automatique (optionnelle) ----------
void Autoupdate() {
  // Si le temps écoulé dépasse l’intervalle défini
  if (millis() - lastUpdateTime > updateInterval) {
    payload.ecran1 = String(millis());   // Met à jour un champ avec le temps actuel
    sendAutoUpdate(payload);            // Envoie les données au client
    lastUpdateTime = millis();          // Met à jour le temps de la dernière mise à jour
  }
}
