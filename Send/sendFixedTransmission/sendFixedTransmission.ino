#include "LoRa_E32.h"
LoRa_E32 e32ttl(3, 2);

#include "DHT.h"
#define DHTPIN 6        // Chân nối DHT11
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);

#define tbi 1           // ID Bộ phát số 1

int volPin = A1;
int tt, hh, dienap;
float h, t, vin = 0;

void setup() {
  Serial.begin(9600);
  dht.begin();
  e32ttl.begin();
}

void loop() {
  // Đưa toàn bộ mọi việc vào trong nhịp đếm 5 giây
  if (runEvery(5000)) {
    // 1. Đọc dữ liệu cảm biến
    h = dht.readHumidity();
    t = dht.readTemperature();
    vin = analogRead(volPin) * (5.0 / 1023.0); 

    // Chốt chặn an toàn: Báo lỗi nếu lỏng dây DHT11
    if (isnan(h) || isnan(t)) {
      Serial.println("Loi: Khong doc duoc cam bien DHT11!");
      return; 
    }

    // 2. Chuyển đổi thành số nguyên để gửi đi (tiết kiệm băng thông)
    hh  = h * 10;
    tt  = t * 10;
    dienap  = vin * 0.992 * 100;

    // 3. In ra màn hình Serial để theo dõi
    Serial.print("Temp : ");  Serial.print(t);
    Serial.print(" | Hum : "); Serial.print(h);
    Serial.print(" | Vol : "); Serial.println(vin * 0.992);

    // 4. GỢI ĐÚNG ĐỊNH DẠNG: a(ID) b(Nhiệt) c(Ẩm) e(Áp) f
    String mess1 = "a" + String(tbi) + "b" + String(tt) + "c" + String(hh) + "e" + String(dienap) + "f";
    
    // 5. LỆNH GỬI TRUYỀN CỐ ĐỊNH TỚI ESP32 (ĐỊA CHỈ 0, 6 - KÊNH 9)
    ResponseStatus rs = e32ttl.sendFixedMessage(0, 6, 0x09, mess1);
    
    Serial.println("=> Dữ liệu đã đóng gói: " + mess1);
    Serial.println("-------------------------");
  }
}

// Hàm đếm thời gian không dùng delay
boolean runEvery(unsigned long interval) {
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}