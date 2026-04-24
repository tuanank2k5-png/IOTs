// Khai báo biến đếm thời gian ở đầu file (nếu chưa có)
// unsigned long prevTFTMillis = 0; 

void TFT_Print() {
  // Chỉ cập nhật màn hình 1 giây (1000ms) một lần
  if (millis() - prevTFTMillis >= 1000) {
    prevTFTMillis = millis();

    // --- 1. CẬP NHẬT ĐỒNG HỒ ---
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextFont(1);
    
    // Tẩy vùng đồng hồ cũ (X: 10, Y: 8, Rộng: 38, Cao: 25)
    tft.fillRect(10, 8, 38, 25, TFT_BLACK); 
    
    tft.setCursor(10, 8); 
    if (hour() < 10) tft.print("0");
    tft.print(String(hour()) + ":");
    if (minute() < 10) tft.print("0");
    tft.print(String(minute()));
    
    tft.setCursor(20, 18);
    if (second() < 10) tft.print("0");
    tft.print(String(second()));

    // --- 2. CẬP NHẬT CẢM BIẾN ---
    tft.setTextFont(2);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    
    // Tẩy vùng số Nhiệt độ cũ
    tft.fillRect(70, 42, 60, 16, TFT_BLACK); 
    tft.setCursor(70, 42); 
    tft.print(nhietdo, 2); 
    tft.print(" C");

    // Tẩy vùng số Độ ẩm cũ
    tft.fillRect(70, 74, 60, 16, TFT_BLACK); 
    tft.setCursor(70, 74); 
    tft.print(doam, 2); 
    tft.print(" %");

    // --- 3. CẬP NHẬT PIN KÈM CẢNH BÁO ---
    if (dienap < batLow) tft.setTextColor(TFT_RED, TFT_BLACK);
    else tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    
    // Tẩy vùng số Điện áp cũ
    tft.fillRect(70, 106, 60, 16, TFT_BLACK); 
    tft.setCursor(70, 106); 
    tft.print(dienap, 2); 
    tft.print(" V");
  }
}