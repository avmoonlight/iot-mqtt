#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "iot";
const char* password = "iotsenai502";

const char* mqtt_server = "192.168.0.175";
const char* mqtt_topic = "grupo7/chat";

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
  Serial.println("\nWiFi conectado");
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida em [");
  Serial.print(topic);
  Serial.print("]: ");

  String mensagem;
  for (unsigned int i = 0; i < length; i++) {
    mensagem += (char)payload[i];
  }
  Serial.println(mensagem);

  if (mensagem == "ligar_led") {
    digitalWrite(LED_PIN, HIGH);
    Serial.println("LED LIGADO");
  } else if (mensagem == "desligar_led") {
    digitalWrite(LED_PIN, LOW);
    Serial.println("LED DESLIGADO");
  } else {
    Serial.println("Comando desconhecido");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conectar ao MQTT...");
    if (client.connect("ESP32PubSub")) {
      Serial.println("Conectado!");
      client.subscribe(mqtt_topic);
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
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  Serial.println("Digite um comando (ligar_led ou desligar_led) para enviar via MQTT:");
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (Serial.available()) {
    String comando = Serial.readStringUntil('\n');
    comando.trim();
    if (comando.length() > 0) {
      client.publish(mqtt_topic, comando.c_str());
      Serial.print("Comando enviado: ");
      Serial.println(comando);
    }
  }
}