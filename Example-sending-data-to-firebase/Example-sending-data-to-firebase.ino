#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include "FirebaseESP32.h"

#define ESP_AP_NAME "ESP32_ACCESS_POINT_NAME"
#define ESP_AP_PASSWORD "ESP32_PASSWORD_ACCESS_POINT"

#define FIREBASE_HOST "YOUR_FIREBASE_PROJECT.firebaseio.com"
#define FIREBASE_AUTH "YOUR_FIREBASE_DATABASE_SECRET"

#define DELAY_TIME 5000

FirebaseData firebaseData;
FirebaseJson json;

String path = "/Push/Int";
int number = 1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFiManager wifiManager;
  wifiManager.setTimeout(180);
  if(wifiManager.autoConnect(ESP_AP_NAME, ESP_AP_PASSWORD)){
    // if you get here you have connected to the WiFi
    wifiManager.setWiFiAutoReconnect(true); // enable auto reconnect
    Serial.print("connected: ");
    Serial.println(WiFi.localIP());
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
  } else {
    Serial.println("Reset...");
    ESP.restart();
  }
  
  // Optional, set number of error retry
  Firebase.setMaxRetry(firebaseData, 3);
  
  // Optional, set number of error resumable queues
  Firebase.setMaxErrorQueue(firebaseData, 30);
}

void loop() {
  // put your main code here, to run repeatedly:
  json.add("number", number);
  if (Firebase.pushJSON(firebaseData, path, json)){
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.print("PUSH NAME: ");
    Serial.println(firebaseData.pushName());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  } else {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }
  json.clear();
  number = number + 1;
  delay(DELAY_TIME);
}
