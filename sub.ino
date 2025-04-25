#include <WiFi.h>
#include <PubSubClient.h>
const char* ssid = "iot";
const char* password = "iotsenai502";
const char* mqtt_server = "192.168.0.175";  // IP da Raspberry Pi
WiFiClient espClient;
PubSubClient client(espClient);
#define LED_PIN 4
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
void callback(char* topic, byte* payload, unsigned int length) {
Serial.print("Mensagem recebida em [");
Serial.print(topic);
Serial.print("]: ");
String mensagem;
for (int i = 0; i < length; i++) {
mensagem += (char)payload[i];
}
Serial.println(mensagem);
if (mensagem == "ligar_led") {
digitalWrite(LED_PIN, HIGH);
Serial.println("LED LIGADO");
delay(1000);  // Acende por 1 segundo
digitalWrite(LED_PIN, LOW);
Serial.println("LED DESLIGADO");
}
}
void reconnect() {
while (!client.connected()) {
Serial.print("Tentando conectar ao MQTT...");
if (client.connect("ESP32Sub")) {
Serial.println("Conectado ao broker MQTT");
client.subscribe("grupo7/chat");
} else {
Serial.print("Falhou, rc=");
Serial.print(client.state());
Serial.println(" tentando novamente em 1 segundo");
delay(1000);
}
}
}
void setup() {
pinMode(LED_PIN, OUTPUT);
Serial.begin(115200);
setup_wifi();
client.setServer(mqtt_server, 1883);
client.setCallback(callback);
}
void loop() {
if (!client.connected()) {
reconnect();
}
client.loop();
}