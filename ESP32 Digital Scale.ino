//WiFi and HTTP
#include <WiFi.h>
#include <HTTPClient.h>
const char* ssid = "VeendySuseno";
const char* password = "Admin12345";
const char* serverName = "http://192.168.0.102/timbangan/input.php";

//MQTT
#include <PubSubClient.h>
const char* mqtt_server = "mqtt.veenbotronik-jgk.com";
int mqtt_port = 11219;
char* topic1 = "/veendysuseno/veenbotronik-jgk/j9682121213121dwdq/tes1";
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

//HX711 LOADCELL
#include <Arduino.h>
#include "HX711.h"
#include "soc/rtc.h"
const int LOADCELL_DOUT_PIN = 16;
const int LOADCELL_SCK_PIN = 4;
HX711 scale;

//PUSH BUTTON
#define pinButton1 27
#define pinButton2 26
boolean nilaiButton1 = 0;
boolean nilaiButton2 = 0;

//LCD I2C
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Debouncing Push Button
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

boolean readButton(int pin) {
  boolean currentReading = digitalRead(pin);
  if ((millis() - lastDebounceTime) > debounceDelay) {
    lastDebounceTime = millis();
    return currentReading;
  }
  return LOW;
}

//MQTT Callback
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

//MQTT Reconnection
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      client.publish(topic1, "hello world");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);

  //PUSH BUTTON
  pinMode(pinButton1, INPUT);
  pinMode(pinButton2, INPUT);

  //LCD I2C
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting...");

  //WiFi and HTTP
  WiFi.begin(ssid, password);
  unsigned long wifiTimeout = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - wifiTimeout < 30000) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to WiFi");
    return;
  }
  Serial.println("WiFi connected!");

  //MQTT
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  //LCD Update
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connected...");
  delay(2000);
  lcd.clear();

  //HX711 LOADCELL
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(390.332);
}

void loop() {
  // HX711 LOADCELL
  float kg = scale.get_units() / 1000;
  if (kg <= 0) kg = 0;
  Serial.println("Kg: " + String(kg));
  scale.power_down();
  delay(10);
  scale.power_up();

  // LCD I2C
  static float lastKg = -1;
  if (kg != lastKg) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Timbangan IoT");
    lcd.setCursor(0, 1);
    lcd.print(String(kg) + "Kg   ");
    lastKg = kg;
  }

  // PUSH BUTTON
  nilaiButton1 = readButton(pinButton1);
  nilaiButton2 = readButton(pinButton2);
  Serial.print(nilaiButton1);
  Serial.println(nilaiButton2);

  // Tare Logic
  if (nilaiButton1 == HIGH) {
    scale.tare();
    Serial.println("TARE....");
    delay(500);
  }

  // Sending Data Logic
  if (nilaiButton2 == HIGH) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      String requestBody = "berat=" + String(kg);
      int httpResponseCode = http.POST(requestBody);
      int retryCount = 0;
      while (httpResponseCode < 0 && retryCount < 3) {
        httpResponseCode = http.POST(requestBody);
        retryCount++;
        delay(1000);
      }
      if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println(httpResponseCode);
        Serial.println(response);
      } else {
        Serial.println("Error on HTTP request");
      }
      http.end();
    }
  }

  // MQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  unsigned long now = millis();
  if (now - lastMsg > 500) {
    lastMsg = now;
    Serial.print("Publish message: ");
    Serial.println(kg);
    String string_kg = String(kg, 2);
    client.publish(topic1, string_kg.c_str());
  }
}
