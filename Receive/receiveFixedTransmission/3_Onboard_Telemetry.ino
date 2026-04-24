void Onboard_Telemetry() {
  // Chỉ in ra khi có dữ liệu mới từ LoRa gửi tới
  if (hasNewData == true) { 
    Serial.printf("T: %.2f | H: %.2f | V: %.2fV\n", nhietdo, doam, dienap);
    
    // In xong thì hạ cờ xuống, chờ đợt sóng LoRa tiếp theo (5 giây sau)
  }
}