void CanhBao() {
  unsigned long currentMillis = millis(); // Cập nhật thời gian thực

  // --- 1. TẠO NHỊP TIM 500ms (Dùng chung cho cả hệ thống) ---
  if (currentMillis - previousMillis >= 500) {
    previousMillis = currentMillis;
    ledState = !ledState; // Đảo trạng thái (True/False)
  }
  
  // --- 2. LOGIC CẢNH BÁO PIN YẾU (Chỉ nháy đèn đỏ) ---
  if (dienap < batLow && dienap > 0) { 
    digitalWrite(ledRun, HIGH);        // Tắt đèn xanh
    digitalWrite(ledAlarm, ledState);  // Nháy đèn đỏ báo pin yếu
  } else {
    digitalWrite(ledRun, LOW);         // Sáng đèn xanh khi bình thường
    digitalWrite(ledAlarm, HIGH);      // Tắt đèn đỏ
  }

  // --- 3. LOGIC CÒI BÁO DỮ LIỆU MỚI TỚI ---
  static int beepStep = 0;
  static unsigned long beepTimer = 0;

  // Nếu có dữ liệu mới VÀ còi đang rảnh (beepStep == 0) thì mới nạp nhịp bíp
  if (hasNewData == true && beepStep == 0) {
    beepStep = 4; // Nạp 4 nhịp (Bật -> Tắt -> Bật -> Tắt = 2 tiếng bíp)
  }

  // Xử lý còi kêu dựa trên số nhịp đã nạp
  if (beepStep > 0) {
    if (currentMillis - beepTimer >= 100) { // Mỗi nhịp cách nhau 100ms
      beepTimer = currentMillis;
      digitalWrite(buzz, !digitalRead(buzz)); // Đảo trạng thái còi (Kêu/Im)
      beepStep--;
    }
  } else {
    digitalWrite(buzz, LOW); // Đảm bảo còi luôn tắt khi hết việc
  }
}