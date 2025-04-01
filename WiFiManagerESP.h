#ifndef WIFI_MANAGER_ESP_H
#define WIFI_MANAGER_ESP_H

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
#include <Arduino.h>
class WiFiManagerESP {
public:
    WiFiManagerESP(const char* ssid, const char* password, const char* user, const char* pass); // Constructor

    void connectToWiFi();
    bool isConnectedToWiFi();
    void getWiFiInfo();
    bool checkInternetAccess();
    bool authenticateCaptivePortal();

private:
    const char* testURL = "http://clients3.google.com/generate_204"; // Prueba de conexión
    // Configuración
    const char* _ssid;
    const char* _username;
    const char* _password;
    const char* _pass;
};

#endif