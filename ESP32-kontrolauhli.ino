#define BLYNK_TEMPLATE_ID ""
#define BLYNK_TEMPLATE_NAME "Násypka"
#define BLYNK_AUTH_TOKEN  ""

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Přihlašovací údaje k WiFi
const char* ssid = "wifi";
const char* pass = "12345";

// Piny
#define SENSOR_PIN 13  // Relé připojuje tento pin k GND
#define LED_SENSOR 14  // LED indikace sepnutí senzoru
#define LED_BLYNK 12   // LED indikace odeslání příkazu
#define LED_WIFI 27    // LED indikace WiFi připojení

bool wifiConnected = false;
unsigned long lastWifiBlink = 0;
unsigned long wifiBlinkInterval = 400; // interval pro blikání LED (400 ms)
unsigned long lastActionTime = 0; // Uchovává poslední čas akce
unsigned long actionInterval = 10800000; // Interval mezi notifikacemi (3 hodiny)
bool blynkSent = false;

unsigned long sensorTriggeredTime = 0;  // Čas, kdy byl senzor poprvé aktivován
unsigned long sensorDelay = 600000;  // 10 minut (600 000 ms)

void setup() {
  Serial.begin(115200);
  
  pinMode(SENSOR_PIN, INPUT_PULLUP);  // Aktivuje interní pull-up na D32
  pinMode(LED_SENSOR, OUTPUT);
  pinMode(LED_BLYNK, OUTPUT);
  pinMode(LED_WIFI, OUTPUT);

  WiFi.begin(ssid, pass);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();

  // Indikace WiFi připojení
  if (WiFi.status() == WL_CONNECTED) {
    if (!wifiConnected) {
      Serial.println("WiFi připojeno");
      wifiConnected = true;
    }
    if (millis() - lastWifiBlink > wifiBlinkInterval) {
      digitalWrite(LED_WIFI, !digitalRead(LED_WIFI));
      lastWifiBlink = millis();
    }
  } else {
    if (wifiConnected) {
      Serial.println("WiFi odpojeno");
      wifiConnected = false;
      digitalWrite(LED_WIFI, LOW);
    }
  }

  // Kontrola senzoru (pokud je SENSOR_PIN sepnut na GND)
  if (digitalRead(SENSOR_PIN) == LOW) {
    digitalWrite(LED_SENSOR, HIGH);  // LED se rozsvítí ihned po aktivaci senzoru

    if (sensorTriggeredTime == 0) {
      sensorTriggeredTime = millis();  // Uložit čas, kdy byl senzor aktivován
    }

    // Pokud senzor zůstane aktivní 10 minut, odešleme notifikaci
    if (!blynkSent && (millis() - sensorTriggeredTime >= sensorDelay)) {
      Serial.println("Senzor aktivní 10 minut! Odesílám notifikaci.");
      
      if (WiFi.status() == WL_CONNECTED) {
        Blynk.virtualWrite(V0, 1);
        Blynk.logEvent("dochz_uhl");
        lastActionTime = millis();
        blynkSent = true;

        // Blikání LED třikrát
        for (int i = 0; i < 3; i++) {
          digitalWrite(LED_BLYNK, HIGH);
          delay(700);
          digitalWrite(LED_BLYNK, LOW);
          delay(200);
        }
      } else {
        Serial.println("Chyba odesílání na Blynk");
        for (int i = 0; i < 5; i++) {
          digitalWrite(LED_BLYNK, HIGH);
          delay(150);
          digitalWrite(LED_BLYNK, LOW);
          delay(150);
        }
      }
    }
  } else {
    sensorTriggeredTime = 0; 
    digitalWrite(LED_SENSOR, LOW);  // LED zhasne ihned po deaktivaci senzoru
  }

  // Reset notifikace po 1 hodině
  if (blynkSent && (millis() - lastActionTime >= actionInterval)) {
    blynkSent = false;
  }
}

