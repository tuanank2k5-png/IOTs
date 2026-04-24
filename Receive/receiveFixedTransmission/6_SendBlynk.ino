void senBlynk() {
  // 1. Đẩy các số liệu cảm biến (Nhiệt, Ẩm, Áp)
  Blynk.virtualWrite(V0, nhietdo);
  Blynk.virtualWrite(V1, doam);
  Blynk.virtualWrite(V2, dienap);

  // 2. Xử lý Ngày/Tháng/Năm chuẩn form (DD/MM/YYYY)
  char currentDate[15]; // Tạo mảng rỗng chứa được 15 ký tự
  // %02d nghĩa là: in ra số nguyên, nếu có 1 chữ số thì tự động thêm số 0 ở đầu
  sprintf(currentDate, "%02d/%02d/%04d", day(), month(), year()); 
  Blynk.virtualWrite(V3, currentDate);

  // 3. Xử lý Giờ:Phút:Giây chuẩn form (HH:MM:SS)
  char currentTime[15];
  sprintf(currentTime, "%02d:%02d:%02d", hour(), minute(), second());
  Blynk.virtualWrite(V4, currentTime);
}