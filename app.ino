#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <DHT.h>

// ------------------------
// Konfigurasi WiFi & Bot
// ------------------------
const char* ssid = "your_wifi";
const char* password = "your_paswword";

#define BOT_TOKEN "xxxx:xxxxx"

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

// ------------------------
// Konfigurasi Pin & Sensor
// ------------------------
#define DHTPIN D4 // GPIO2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define MOISTURE_DIGITAL_PIN D3  // GPIO0
#define MOISTURE_ANALOG_PIN A0

// ------------------------
// Variabel kontrol
// ------------------------
bool sendData = false;
unsigned long lastSendTime = 0;
const unsigned long interval = 5000;  // 5 detik
String savedChatID = "";

// ------------------------
// Setup
// ------------------------
void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(MOISTURE_DIGITAL_PIN, INPUT);

  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi terhubung");

  secured_client.setInsecure();  // untuk koneksi HTTPS Telegram
}

// ------------------------
// Fungsi baca data sensor
// ------------------------
String getSensorData() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int moistDigital = digitalRead(MOISTURE_DIGITAL_PIN);
  int moistAnalog = analogRead(MOISTURE_ANALOG_PIN);

  String moistStatus = (moistDigital == LOW) ? "Basah" : "Kering";

  String message = "🌡️ Suhu: " + String(temp, 1) + "°C\n";
  message += "💧 Kelembaban: " + String(hum, 1) + "%\n";
  message += "🌱 Kelembaban Tanah (Digital): " + moistStatus + "\n";
  message += "🌱 Kelembaban Tanah (Analog): " + String(moistAnalog);

  return message;
}

// ------------------------
// Loop utama
// ------------------------
void loop() {
  // Cek pesan masuk ke bot
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  while (numNewMessages) {
    for (int i = 0; i < numNewMessages; i++) {
      String text = bot.messages[i].text;
      String chat_id = bot.messages[i].chat_id;

      // Simpan chat_id jika belum ada
      if (savedChatID == "") {
        savedChatID = chat_id;
        Serial.println("📥 Chat ID terdeteksi otomatis: " + savedChatID);
      }

      if (text == "/on") {
        sendData = true;
        bot.sendMessage(savedChatID, "✅ Mulai mengirim data setiap 5 detik.", "");
      } else if (text == "/off") {
        sendData = false;
        bot.sendMessage(savedChatID, "🛑 Berhenti mengirim data.", "");
      } else {
        bot.sendMessage(chat_id, "📌 Gunakan /on atau /off", "");
      }
    }
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  }

  // Kirim data jika aktif
  if (sendData && millis() - lastSendTime > interval && savedChatID != "") {
    String data = getSensorData();
    bot.sendMessage(savedChatID, data, "");
    lastSendTime = millis();
  }
}
