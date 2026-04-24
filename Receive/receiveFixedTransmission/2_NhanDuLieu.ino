void NhanDuLieu() {
  if (e32ttl.available() > 1) {
    ResponseContainer rs = e32ttl.receiveMessage();
    String message = rs.data;
    
    // Dòng này để soi xem sóng có bay tới nơi không:
    Serial.println(">>> Sóng LoRa bay tới: " + message); 
    
    // 1. TÌM VỊ TRÍ CÁC "VÁCH NGĂN"
    int viTriA = message.indexOf('a');
    int viTriB = message.indexOf('b');
    int viTriC = message.indexOf('c');
    int viTriE = message.indexOf('e');
    int viTriF = message.indexOf('f');

    // 2. LỌC NHIỄU LORA: Chỉ xử lý khi chuỗi có đầy đủ các vách ngăn a, b, c, e, f theo đúng thứ tự
    if (viTriA >= 0 && viTriB > viTriA && viTriC > viTriB && viTriE > viTriC && viTriF > viTriE) {
      
      // 3. CẮT CHUỖI VÀ CHUYỂN THÀNH SỐ (Tự động co giãn theo độ dài số)
      t1 = message.substring(viTriA + 1, viTriB).toInt(); // Lấy ID
      t2 = message.substring(viTriB + 1, viTriC).toInt(); // Lấy Nhiệt
      t3 = message.substring(viTriC + 1, viTriE).toInt(); // Lấy Ẩm (Từ C đến E)
      t5 = message.substring(viTriE + 1, viTriF).toInt(); // Lấy Áp
      
      // 4. KIỂM TRA ĐÚNG TRẠM SỐ 1 VÀ PHỤC HỒI DỮ LIỆU
      if (t1 == 1) { 
        nhietdo = t2 / 10.0;
        doam    = t3 / 10.0;
        dienap  = t5 / 100.0; 
        
        hasNewData = true; // Cờ báo hiệu có dữ liệu mới
      }
    } else {
      Serial.println(">>> LỖI: Chuỗi dữ liệu bị méo hoặc nhiễu sóng!");
    }
  }
}