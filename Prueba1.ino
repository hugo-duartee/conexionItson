#include "WiFiManagerESP.h"
#include "FirebaseESP8266.h"

#include <DHT.h> 
#define DHTPIN 4 
DHT dht(DHTPIN, DHT11);

#define FIREBASE_HOST "proyecto-5d85a-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "nrnX8Y2BEiNxvQgnOJobo0PK4g4zCIaCwCoaI08n" 
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;
bool iterar = true;

// Configuración editable (¡CAMBIAR ESTO!)
const char* ssid = "Potros_Wifi";
const char* password = "";
const char* user = "00000272171";
const char* pass = "41185Acu";
WiFiManagerESP WiFiManager(ssid, password, user, pass); //Se crea el objeto de la clase WiFiManagerESP

void setup() {
  Serial.begin(115200);    
  WiFiManager.connectToWiFi();

  dht.begin();
  pinMode(LED_BUILTIN, OUTPUT);
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  // Iniciar Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!Firebase.readStream(firebaseData)){
        Serial.println();
        Serial.println("No puedo leerel envio de datos");
        Serial.println("RASON: " + firebaseData.errorReason());
        Serial.println();
    }

    if (firebaseData.streamTimeout()){
        Serial.println();
        Serial.println("Tiempo fuera del envio, resumen de envio...");
        Serial.println();
    }

  while (iterar){
  
    Serial.print("Wifi:"+WiFi.SSID());
   float h = dht.readHumidity();
   float t = dht.readTemperature();
      
   Serial.print("\n Ambiente Humedad: ");
   Serial.print(h);
   Serial.println("");
   Serial.print("Ambiente Temperatura: ");
   Serial.print(t);
   Serial.println(" *C ");
   Firebase.setString(firebaseData, "DTH11/Humedad", h);
   Firebase.setString(firebaseData, "DTH11/Temperatura", t);

   Serial.println();

   Serial.println();
   Serial.println("Envio datos");
   Serial.println();

   delay(1000);
  
  }

}
