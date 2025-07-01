#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include "DHT.h"

#define wifi_SSID "Ibnu"
#define wifi_PASSWORD ""
#define FIREBASE_HOST "https://sistemkontrol-354e2-default-rtdb.firebaseio.com/:null "
#define FIREBASE_AUTH "AIzaSyDJy8q1lwiPjc8ibEzeJ8YgcC6LZ_q2GWI"
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

#define DHT_PIN D5
#define DHTTYPE DHT22
#define RELAY1_PIN D6
#define RELAY2_PIN D7
#define RELAY3_PIN D8
DHT dht(DHT_PIN, DHTTYPE);

void setup() {
  Serial.begin (115200);
  WiFi.begin(wifi_SSID, wifi_PASSWORD);
  Serial.print("Menghubungkan Wifi");

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  
  Serial.println();
  Serial.println("Wifi Tersambung");

  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  dht.begin();

  pinMode(DHT_PIN, INPUT);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);

  digitalWrite (RELAY1_PIN, HIGH);
  digitalWrite (RELAY2_PIN, HIGH);
  digitalWrite (RELAY3_PIN, HIGH);
  // put your setup code here, to run once:

}

void loop() {
 delay(1000);
  float h = dht.readHumidity();

  float t = dht.readTemperature();
  
  if (Firebase.ready()) {
    
    if (!isnan(h) && Firebase.setFloat(fbdo, "DHT/humidity", h)){
//     
       Serial.print("Humidity: ");
       Serial.println(h);
      
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    
    // Write an Float number on the database path test/float
    if (!isnan(t) && Firebase.setFloat(fbdo, "DHT/temperature", t)){
//     
       Serial.print("Temperature: ");
       Serial.println(t);
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
  //iki mlebu relay
if (Firebase.getString(fbdo, "Mesin/Kipas")) {
    String state1 = fbdo.stringData();
    Serial.print("Kipas: ");
    Serial.println(state1);
    digitalWrite(RELAY1_PIN, (state1 == "ON") ? LOW : HIGH);
  } else {
    Serial.println("Gagal baca /Kipas: " + fbdo.errorReason());
  }

  // Baca /relay2 dari Firebase
  if (Firebase.getString(fbdo, "Mesin/Lampu")) {
    String state2 = fbdo.stringData();
    Serial.print("Lampu: ");
    Serial.println(state2);
    digitalWrite(RELAY2_PIN, (state2 == "ON") ? LOW : HIGH);
  } else {
    Serial.println("Gagal baca /Lampu: " + fbdo.errorReason());
  }
  if (Firebase.getString(fbdo, "Mesin/Pompa")) {
    String state3 = fbdo.stringData();
    Serial.print("Pompa: ");
    Serial.println(state3);
    digitalWrite(RELAY3_PIN, (state3 == "ON") ? LOW : HIGH);
  } else {
    Serial.println("Gagal baca /pompa: " + fbdo.errorReason());
  }
  }

  Serial.println("______________________________");
}
