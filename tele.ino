/*******************************************************
 * Willys Tecnologia IoT - VERSÃO PROFISSIONAL
 * ESP8266 NodeMCU
 *
 * ✔ WiFi com reconexão
 * ✔ MQTT ThingsBoard
 * ✔ DHT11
 * ✔ OLED (U8g2)
 * ✔ Sem travamentos
 *******************************************************/

// ===== BIBLIOTECAS =====
#include <ESP8266WiFi.h>   // Controle de WiFi
#include <PubSubClient.h>  // MQTT
#include <Wire.h>          // Comunicação I2C
#include <U8g2lib.h>       // Display OLED
#include <DHT.h>           // Sensor DHT

// ===== WIFI =====
const char* ssid = "*****";          // Nome da rede WiFi
const char* password = "*******"; // Senha da rede

// ===== MQTT =====
const char* mqtt_server = "thingsboard.cloud"; // Servidor ThingsBoard
const char* token = "*******";     // Token do dispositivo

WiFiClient espClient;        // Cliente WiFi
PubSubClient client(espClient); // Cliente MQTT usando WiFi

// ===== DHT =====
#define DHTPIN D4            // Pino do sensor DHT
#define DHTTYPE DHT11        // Tipo do sensor

DHT dht(DHTPIN, DHTTYPE);    // Inicializa o sensor

// ===== OLED =====
// SDA = D6 (GPIO12) | SCL = D5 (GPIO14)
U8G2_SSD1306_128X64_NONAME_F_SW_I2C 
display(U8G2_R0, 12, 14, U8X8_PIN_NONE);

// ===== CONTROLE DE TEMPO =====
unsigned long tempoAnterior = 0; // Armazena tempo anterior
const long intervalo = 2000;     // Intervalo de leitura (2 segundos)

// ===== DISPLAY =====
// Função para escrever 3 linhas no OLED
void tela(String l1, String l2, String l3) {
  display.clearBuffer();                    // Limpa buffer
  display.setFont(u8g2_font_ncenB08_tr);    // Define fonte

  display.drawStr(0, 12, l1.c_str());       // Linha 1
  display.drawStr(0, 32, l2.c_str());       // Linha 2
  display.drawStr(0, 52, l3.c_str());       // Linha 3

  display.sendBuffer();                     // Envia para tela
}

// ===== WIFI =====
void conectarWiFi() {

  // Se já estiver conectado, sai da função
  if (WiFi.status() == WL_CONNECTED) return;

  WiFi.mode(WIFI_STA);           // Modo estação
  WiFi.begin(ssid, password);    // Inicia conexão

  tela("WiFi", "Conectando...", ""); // Mostra no display

  int tentativas = 0; // Contador de tentativas

  // Tenta conectar por até ~10 segundos
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {
    delay(500);
    tentativas++;
  }

  // Verifica se conectou
  if (WiFi.status() == WL_CONNECTED) {
    tela("WiFi OK", WiFi.localIP().toString(), "");
  } else {
    tela("Erro WiFi", "Falha", "");
  }
}

// ===== MQTT =====
void conectarMQTT() {

  // Se já estiver conectado, sai
  if (client.connected()) return;

  tela("MQTT", "Conectando...", "");

  // Loop até conectar
  while (!client.connected()) {

    // Tenta conectar usando TOKEN do ThingsBoard
    if (client.connect("ESP8266_Willys", token, "")) {
      tela("MQTT OK", "Conectado", "");
    } else {
      tela("MQTT ERRO", "Retry...", "");
      delay(2000); // Aguarda antes de tentar novamente
    }
  }
}

// ===== SETUP =====
void setup() {

  Serial.begin(115200); // Inicializa comunicação serial

  display.begin(); // Inicializa display OLED
  dht.begin();     // Inicializa sensor DHT

  tela("Willys IoT", "Inicializando...", "");

  conectarWiFi(); // Conecta no WiFi

  client.setServer(mqtt_server, 1883); // Define servidor MQTT
}

// ===== LOOP =====
void loop() {

  conectarWiFi();  // Garante WiFi conectado
  conectarMQTT();  // Garante MQTT conectado
  client.loop();   // Mantém comunicação MQTT ativa

  // ===== EXECUTA SEM BLOQUEAR =====
  if (millis() - tempoAnterior >= intervalo) {

    tempoAnterior = millis(); // Atualiza tempo

    // Leitura do sensor
    float temp = dht.readTemperature(); // Temperatura
    float umid = dht.readHumidity();    // Umidade

    // Verifica erro de leitura
    if (isnan(temp) || isnan(umid)) {
      temp = 0;
      umid = 0;
    }

    // ===== DISPLAY =====
    tela(
      "Temp: " + String(temp) + "C",
      "Umid: " + String(umid) + "%",
      "Enviando MQTT"
    );

    // ===== JSON MQTT =====
    String payload = "{";
    payload += "\"temperatura\":" + String(temp) + ",";
    payload += "\"umidade\":" + String(umid);
    payload += "}";

    // Envia dados para ThingsBoard
    client.publish("v1/devices/me/telemetry", payload.c_str());
  }
}