import network
import time
import dht
import ujson
from machine import Pin, SoftI2C
import ssd1306

# MQTT
try:
    from umqtt.simple import MQTTClient
except ImportError:
    import mip
    mip.install("umqtt.simple")
    from umqtt.simple import MQTTClient

# --- CONFIG ---
SSID = "Wokwi-GUEST"
SENHA = ""

TB_SERVER = "thingsboard.cloud"
TB_TOKEN = "f8c5emv756ur68wfqx0w"
CLIENT_ID = "ESP32_Weather"
TOPICO_ENVIO = b"v1/devices/me/telemetry"

# --- HARDWARE ---
sensor = dht.DHT22(Pin(4))
led = Pin(2, Pin.OUT)

i2c = SoftI2C(scl=Pin(22), sda=Pin(21))
oled = ssd1306.SSD1306_I2C(128, 64, i2c)

# --- OLED ---
def atualizar_oled(l1="", l2="", l3=""):
    oled.fill(0)
    oled.text(l1, 0, 0)
    oled.text(l2, 0, 20)
    oled.text(l3, 0, 40)
    oled.show()

# --- WIFI ---
def conectar_wifi():
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)

    if not wlan.isconnected():
        atualizar_oled("WiFi...", "Conectando", "")
        wlan.connect(SSID, SENHA)

        while not wlan.isconnected():
            time.sleep(1)

    atualizar_oled("WiFi OK", wlan.ifconfig()[0], "")

# --- MQTT ---
def conectar_mqtt():
    try:
        atualizar_oled("MQTT...", "Conectando", "")
        client = MQTTClient(CLIENT_ID, TB_SERVER, user=TB_TOKEN, password="")
        client.connect()
        atualizar_oled("MQTT OK", "Conectado", "")
        return client
    except:
        atualizar_oled("MQTT ERRO", "Reconectando", "")
        return None

# --- INÍCIO ---
conectar_wifi()
mqtt = conectar_mqtt()

# --- LOOP ---
while True:
    try:
        # SENSOR
        sensor.measure()
        temp = sensor.temperature()
        umid = sensor.humidity()

        # LED indicador
        led.value(not led.value())

        # OLED
        atualizar_oled(
            "Temp: {:.1f}C".format(temp),
            "Umid: {:.1f}%".format(umid),
            "Enviando..."
        )

        # JSON (corrigido)
        pacote = ujson.dumps({
            "temperatura": temp,
            "umidade": umid
        })

        # MQTT
        if mqtt:
            mqtt.publish(TOPICO_ENVIO, pacote)
        else:
            mqtt = conectar_mqtt()

    except Exception as e:
        print("Erro:", e)
        atualizar_oled("ERRO", "SENSOR/REDE", "X")

    time.sleep(2)