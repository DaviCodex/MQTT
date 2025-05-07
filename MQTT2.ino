//Libs
#include <PubSubClient.h>
#include "WiFi.h"

//Var
const int analogPin = 15;

//WiFi
const char* ssid = "Vargas Mejia ";
const char* wifi_pass = "David2024#";

// MQTT
const char* mqtt_server = "192.168.20.19";
const char* humidity_topic = "humidity";
const char* mqtt_username = "pi";
const char* mqtt_pass = "david2002";
const char* clientID = "moisture_reporter";

//Instanciando clase wifi

WiFiClient wifiClient;

PubSubClient client(mqtt_server, 1883, wifiClient);

//Functions
float leerSensor( int pin){

  int moisture, analogSensor;
  analogSensor = analogRead(pin);
  moisture = (100-((analogSensor/4095)*100));
  return moisture;
}

void connect_MQTT(){
  // Conectando a la red xyz
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Conexion
  WiFi.begin(ssid, wifi_pass);

  // Confirmacion de conexion 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Connect to MQTT Broker
  if (client.connect(clientID, mqtt_username, mqtt_pass)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed…");
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  connect_MQTT();
  Serial.setTimeout(2000);
  int humedad = leerSensor(analogPin);
  Serial.print("Humidity: ");
  Serial.print(humedad);
  Serial.println(" %");
  // MQTT can only transmit strings
  String hs="Hum: "+String((float)humedad)+" % ";
   // PUBLISH to the MQTT Broker (topic = Humidity)
  if (client.publish(humidity_topic, String(humedad).c_str())) {
    Serial.println("Humidity sent!");
  }
  else {
    Serial.println("Humidity failed to send. Reconnecting to MQTT Broker and trying again");
    client.connect(clientID, mqtt_username, mqtt_pass);
    delay(10); // This delay ensures that client.publish doesn’t clash with the client.connect call
    client.publish(humidity_topic, String(humedad).c_str());
  }
  client.disconnect();  // disconnect from the MQTT broker
  delay(10000);       // print new values after 1 Minute
}
