#include "iconSuDung.h"
#include <TFT_eSPI.h> 
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();

#include "LoRa_E32.h"
// Khởi tạo LoRa dùng HardwareSerial2 (TX2: GPIO17, RX2: GPIO16)
LoRa_E32 e32ttl(&Serial2); 

//===== THÔNG TIN BLYNK =====
#define BLYNK_TEMPLATE_ID           "TMPL6zBa0kITd"
#define BLYNK_TEMPLATE_NAME         "ESP32 QuanTrac"
#define BLYNK_AUTH_TOKEN            "XcyJ6hblWZaJtDpmTYvkiEm3y5Y99nB-"
#define BLYNK_PRINT Serial     

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <TimeLib.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "T Ne";         // WiFi của bạn
char pass[] = "18072005";     // Mật khẩu WiFi

TaskHandle_t Task2;
BlynkTimer timer;

// ===== CHÂN CẮM NGOẠI VI =====
#define ledRun   21
#define ledAlarm 5
#define buzz     12

// ===== BIẾN LƯU TRỮ =====
float nhietdo = 0, doam = 0, dienap = 0;
float batLow = 3.60;
long t1, t2, t3, t5;

bool hasNewData = false; // Cờ báo hiệu có dữ liệu mới
unsigned long previousMillis = 0;
unsigned long prevTFTMillis = 0;
int ledState = 0;

// ==========================================
//                 SETUP
// ==========================================
void setup() {
  Serial.begin(115200);
  
  pinMode(ledRun, OUTPUT);
  pinMode(ledAlarm, OUTPUT);
  pinMode(buzz, OUTPUT);
  
  digitalWrite(ledRun, HIGH);
  digitalWrite(ledAlarm, HIGH);
  digitalWrite(buzz, LOW);

  // Khởi tạo màn hình
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  // --- GIAO DIỆN TĨNH (Chỉ vẽ 1 lần duy nhất) ---
  tft.drawRect(1, 2, 158, 126, TFT_WHITE);
  tft.drawLine(50, 0, 50, 128, TFT_WHITE);
  tft.drawLine(0, 32, 158, 32, TFT_WHITE);
  tft.drawLine(0, 64, 158, 64, TFT_WHITE);
  tft.drawLine(0, 96, 158, 96, TFT_WHITE);

  tft.setSwapBytes(true);
  tft.pushImage(13, 36, 24, 24, iconNhietDo);
  tft.pushImage(13, 68, 24, 24, iconDoAm);
  tft.pushImage(13, 101, 23, 23, iconPin);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextFont(2);
  tft.drawString("SENSOR", 75, 10); 

  // Khởi tạo LoRa
  e32ttl.begin();

  // Chạy hệ thống WiFi/Blynk ngầm trên Core 1
  xTaskCreatePinnedToCore(Task2code, "Task2", 10000, NULL, 1, &Task2, 1);          
}

// ==========================================
//          CORE 1: WIFI & BLYNK
// ==========================================
void Task2code( void * pvParameters ) {
  Blynk.begin(auth, ssid, pass);
  
  // Lấy giờ Internet múi giờ VN (GMT+7)
  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  
  unsigned long prevBlynkMillis = 0; 
  
  while (1) {
    // 1. Cập nhật giờ hệ thống
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) { 
      setTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
    }

    // 2. Chống nghẽn mạng: Đẩy lên Blynk 5 giây 1 lần
    if (millis() - prevBlynkMillis >= 5000) {
      prevBlynkMillis = millis();
      senBlynk();
    }

    Blynk.run();
    timer.run();
    vTaskDelay(10 / portTICK_PERIOD_MS); 
  }
}

// ==========================================
//          CORE 0: VÒNG LẶP CHÍNH
// ==========================================
void loop() {
  NhanDuLieu();
  TFT_Print();
  Onboard_Telemetry();
  CanhBao();

  // CHỐT CHẶN: Các phòng ban xử lý xong thì hạ cờ xuống
  if (hasNewData == true) {
    hasNewData = false; 
  }
}