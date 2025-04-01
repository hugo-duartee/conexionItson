#include "WiFiManagerESP.h"

WiFiManagerESP::WiFiManagerESP(const char* ssid, const char* password, const char* user, const char* pass) 
: _ssid(ssid), _password(password), _username(user), _pass(pass) {}

void WiFiManagerESP::connectToWiFi() {
    int attempts = 0;
    WiFi.begin(_ssid, _password);
    Serial.print("Conectando a WiFi, intento:"+attempts);
    

    while (WiFi.status() != WL_CONNECTED && attempts < 5) {
        delay(1000);
        Serial.print(".");
        attempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConectado a la red:"+WiFi.SSID());
        getWiFiInfo();
        if (!checkInternetAccess()) {
        authenticateCaptivePortal();
    }
    } else {
        Serial.println("\nError: No se pudo conectar a WiFi.");
    }
    
}

bool WiFiManagerESP::isConnectedToWiFi() {
    return WiFi.status() == WL_CONNECTED;
    
}

void WiFiManagerESP::getWiFiInfo() {
    if (isConnectedToWiFi()) {
        Serial.println("=====\nINFORMACION DE LA RED=====");
        Serial.printf("SSID: %s\n", WiFi.SSID().c_str());
        Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());
        Serial.printf("MAC: %s\n", WiFi.macAddress().c_str());
        
        int rssi = WiFi.RSSI();
        int signalStrength = map(rssi, -100, -50, 0, 100);
        signalStrength = constrain(signalStrength, 0, 100);
        Serial.printf("Señal WiFi: %d/100\n", signalStrength);
    } else {
        Serial.println("No conectado a WiFi.");
    }
}

bool WiFiManagerESP::checkInternetAccess() {
    if (!isConnectedToWiFi()) return false;

    HTTPClient http;
    WiFiClient client;
    http.begin(client, testURL);
    int httpCode = http.GET();
    http.end();

    return httpCode == 204; // Código 204 indica conexión sin portal cautivo
}

bool WiFiManagerESP::authenticateCaptivePortal() {
    if (checkInternetAccess()) {
        Serial.println("Ya hay acceso a Internet, no es necesario autenticarse.");
        return true;
    }
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);
    client->setInsecure();  // Ignorar certificados SSL

    Serial.println("Autenticando en portal cautivo...");
    HTTPClient http;
    //WiFiClient client;
    
    //SOlICITUD POST
    String postData = "mode=191&username=" + String(_username) + "&password=" + String(_pass) + "&a=1743186843048&producttype=0";
    http.begin(*client, "https://captiveportalnav.itson.edu.mx:8090/login.xml");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    int httpResponseCode = http.POST(postData);
    String response = http.getString();
    
    http.end();
    //FIN DE SOLICITUD

    if (httpResponseCode > 0) {
        Serial.print("Código HTTP: ");
        Serial.println(httpResponseCode);
        Serial.println("Respuesta del portal:");
        Serial.println(response);
    } else {
        Serial.print("Error en la autenticación: ");
        Serial.println(httpResponseCode);
    }
    
    return httpResponseCode == 200 && response.indexOf("LIVE") >= 0;
}