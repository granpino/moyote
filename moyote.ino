// Wemos D1 MQTT to Raspberry pi.
// reboot Wemos after upload
// 
#include "DHT.h"
#include <ESP8266WiFi.h>  // Enables the ESP8266 to connect to the local network (via WiFi)
#include <PubSubClient.h> // Connect and publish to the MQTT broker

#define DHTPIN D4        // what digital pin the DHT11 is conected to
#define DHTTYPE DHT22   // there are multiple kinds of DHT sensors
DHT dht(DHTPIN, DHTTYPE);

// WiFi
const char* ssid = "myrouter";              // Your  network SSID
const char* wifi_password = "mypassword"; // Your  network password

// MQTT
const char* mqtt_server = "raspberrypi.local";  // IP of the MQTT broker (Raspberry pi)
const char* humidity_topic = "bedroom/humidity";
const char* temperature_topic = "bedroom/temperature";
//==const char* mqtt_username = "cdavid"; // MQTT username ===not used 
//==const char* mqtt_password = "cdavid"; // MQTT password ===not used
const char* clientID = "client_bedroom"; // MQTT client ID

// Initialise the WiFi and MQTT Client objects
WiFiClient wifiClient;
// 1883 is the listener port for the Broker
PubSubClient client(mqtt_server, 1883, wifiClient); 

// Custom function to connet to the MQTT broker via WiFi
void connect_MQTT(){
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Connect to the WiFi
  WiFi.begin(ssid, wifi_password);

  // Wait until the connection has been confirmed before continuing
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Debugging - Output the IP Address of the ESP8266
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Connect to MQTT Broker
  // client.connect returns a boolean value to let us know if the connection was successful.
  // If the connection is failing, make sure you are using the correct MQTT Username and Password (Setup Earlier in the Instructable)
  //==if (client.connect(clientID, mqtt_username, mqtt_password)) {
  if (client.connect(clientID)) {
    Serial.println("Connected to MQTT Broker!");
  }
  else {
    Serial.println("Connection to MQTT Broker failed...");
  }

}

void setup() {
  Serial.begin(19200);
  dht.begin();
}

void loop() {

  connect_MQTT();
  Serial.setTimeout(2000);
  
  float h = dht.readHumidity();
  float t = dht.readTemperature(true);
  
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.println(" %");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println(" *F");

  // MQTT can only transmit strings
  String hs="Hum: "+String((float)h)+" % ";
  String ts="Temp: "+String((float)t)+" F ";

  // PUBLISH to the MQTT Broker (topic = Temperature, defined at the beginning)
  if (client.publish(temperature_topic, String(t).c_str())) {
    Serial.println("Temperature sent!");
  }
  // Again, client.publish will return a boolean value depending on whether it succeded or not.
  // If the message failed to send, we will try again, as the connection may have broken.
  else {
    Serial.println("Temperature failed to send. Reconnecting to MQTT Broker and trying again");
    //==client.connect(clientID, mqtt_username, mqtt_password);
    client.connect(clientID);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(temperature_topic, String(t).c_str());
  }

  // PUBLISH to the MQTT Broker (topic = Humidity, defined at the beginning)
  if (client.publish(humidity_topic, String(h).c_str())) {
    Serial.println("Humidity sent!");
  }
  // Again, client.publish will return a boolean value depending on whether it succeded or not.
  // If the message failed to send, we will try again, as the connection may have broken.
  else {
    Serial.println("Humidity failed to send. Reconnecting to MQTT Broker and trying again");
    //==client.connect(clientID, mqtt_username, mqtt_password);
    client.connect(clientID);
    delay(10); // This delay ensures that client.publish doesn't clash with the client.connect call
    client.publish(humidity_topic, String(h).c_str());
  }
  
  client.disconnect();  // disconnect from MQTT broker
  delay(1000*30);       // print new values every 0.5 Minute
}
