# telemetricaesp8266
Este projeto foi desenvolvido durante o curso de **Microeletrônica**, com foco em aplicações reais de sistemas embarcados e Internet das Coisas (IoT).  O sistema realiza a leitura de dados ambientais através de um sensor DHT11, exibe as informações em um display OLED e envia os dados para a nuvem utilizando o protocolo MQTT.
# 🚀 Willys Tecnologia IoT - Monitoramento com ESP8266

Projeto de IoT utilizando ESP8266 (NodeMCU) para monitoramento de temperatura e umidade em tempo real, com envio de dados para a nuvem via MQTT (ThingsBoard) e exibição local em display OLED.

---

## 📌 Descrição

Este projeto foi desenvolvido durante o curso de **Microeletrônica**, com foco em aplicações reais de sistemas embarcados e Internet das Coisas (IoT).

O sistema realiza a leitura de dados ambientais através de um sensor DHT11, exibe as informações em um display OLED e envia os dados para a nuvem utilizando o protocolo MQTT.

---

## ⚙️ Funcionalidades

- 📡 Conexão WiFi com reconexão automática  
- ☁️ Envio de dados para ThingsBoard via MQTT  
- 🌡️ Leitura de temperatura e umidade (DHT11)  
- 🖥️ Exibição em display OLED (128x64)  
- 🔄 Execução não bloqueante (uso de `millis()`)  
- 🛠️ Estrutura organizada e escalável  

---

## 🧰 Tecnologias Utilizadas

- ESP8266 (NodeMCU)
- Arduino IDE
- MQTT (ThingsBoard)
- Sensor DHT11
- Display OLED SSD1306
- Biblioteca U8g2
- Biblioteca PubSubClient

---

## 🔌 Esquema de Ligação

| Componente | Pino ESP8266 |
|----------|-------------|
| DHT11     | D4          |
| OLED SDA  | D6 (GPIO12) |
| OLED SCL  | D5 (GPIO14) |

---

## 📦 Estrutura do Projeto
