#include "WiFiManagerESP.h"

// Configuración editable (¡CAMBIAR ESTO!)
const char* ssid = "Potros_Wifi"; // Esta es la red wifi de Ison que tiene el portal de autenticacion
const char* password = ""; //contraseña wifi. se deja en blanco ya que no tiene seguridad
const char* user = "matricula";
const char* pass = "contraseña alumno";
WiFiManagerESP WiFiManager(ssid, password, user, pass); //Se crea el objeto de la clase WiFiManagerESP

void setup() {
  Serial.begin(115200);    
  WiFiManager.connectToWiFi(); // metodo para iniciar la conexion y autenticacion.
}

void loop() {
  // put your main code here, to run repeatedly:

}
