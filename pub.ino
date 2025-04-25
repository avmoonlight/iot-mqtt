#include <WiFi.h>
#include <PubSubClient.h>
// Wi-Fi
const char* ssid = "iot";
const char* password = "iotsenai502";
// MQTT Broker
const char* mqtt_server = "192.168.0.175";  // IP da Raspberry Pi
WiFiClient espClient;
PubSubClient client(espClient);
void setup_wifi() {
delay(10);
Serial.println("Conectando ao WiFi...");
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("\nConectado ao WiFi");
}
void reconnect() {
while (!client.connected()) {
Serial.print("Tentando conectar ao MQTT...");
if (client.connect("ESP32Pub")) {
Serial.println("Conectado ao broker MQTT");
} else {
Serial.print("Falhou, rc=");
Serial.print(client.state());
Serial.println(" tentando novamente em 1 segundo");
delay(1000);
}
}
}
void setup() {
Serial.begin(115200);
setup_wifi();
client.setServer(mqtt_server, 1883);
}
void loop() {
if (!client.connected()) {
reconnect();
}
client.loop();
static unsigned long lastMsg = 0;
if (millis() - lastMsg > 5000) {
lastMsg = millis();
String msg = "ligar_led"; // Comando a ser enviado
client.publish("grupo7/chat", msg.c_str());
Serial.println("Mensagem publicada: ligar_led");
}
}