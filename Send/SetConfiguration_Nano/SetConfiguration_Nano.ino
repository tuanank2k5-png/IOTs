#include "Arduino.h"
#include "LoRa_E32.h"
LoRa_E32 e32ttl(3, 2); // Chân RX, TX nối với LoRa

void printParameters(struct Configuration configuration);

void setup() {
  Serial.begin(9600);
  e32ttl.begin();

  ResponseStructContainer c;
  c = e32ttl.getConfiguration();
  Configuration configuration = *(Configuration*) c.data;
  
  Serial.println(c.status.getResponseDescription());

  // ĐỒNG BỘ ĐỊA CHỈ VÀ KÊNH VỚI MẠCH THU (ESP32)
  configuration.ADDL = 0x06;  // Địa chỉ thấp (Sửa từ 0x07/0x08 thành 0x06)
  configuration.ADDH = 0x00;  // Địa chỉ cao
  configuration.CHAN = 0x09;  // Kênh tần số (Phải trùng với mạch thu)

  configuration.OPTION.fec = FEC_1_ON;
  // ĐỔI SANG CHẾ ĐỘ TRUYỀN TRONG SUỐT (TRANSPARENT)
  configuration.OPTION.fixedTransmission = FT_TRANSPARENT_TRANSMISSION;
  configuration.OPTION.ioDriveMode = IO_D_MODE_PUSH_PULLS_PULL_UPS;
  configuration.OPTION.transmissionPower = POWER_20;
  configuration.OPTION.wirelessWakeupTime = WAKE_UP_250;

  configuration.SPED.airDataRate = AIR_DATA_RATE_010_24;
  configuration.SPED.uartBaudRate = UART_BPS_9600;
  configuration.SPED.uartParity = MODE_00_8N1;

 ResponseStatus rs = e32ttl.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
  Serial.println(rs.getResponseDescription());
  
  printParameters(configuration);
  c.close();
}

void loop() {}

void printParameters(struct Configuration configuration) {
  Serial.println("----------------------------------------");
  Serial.print(F("AddH : "));  Serial.println(configuration.ADDH, HEX);
  Serial.print(F("AddL : "));  Serial.println(configuration.ADDL, HEX);
  Serial.print(F("Chan : "));  Serial.print(configuration.CHAN, HEX); Serial.print(" -> "); Serial.println(configuration.getChannelDescription());
  Serial.print(F("OptionTrans : "));  Serial.print(configuration.OPTION.fixedTransmission, BIN); Serial.print(" -> "); Serial.println(configuration.OPTION.getFixedTransmissionDescription());
  Serial.println("----------------------------------------");
}