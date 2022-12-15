// ====================================
// Chip ID
// ====================================
// To get chip ID, ESP32 will return mac address
void setDeviceID()
{
  // ==============
  // Chip ID
  // ==============
  // Get default mac address
  // Format 001122334455
  uint8_t chipid[6];
  esp_efuse_mac_get_default (chipid);

  // ==============
  // Device EUI
  // ==============
  // Format 001122334455ffff
  Serial.print("Device EUI: ");
  printHex(chipid[0]); printHex(chipid[1]); printHex(chipid[2]); printHex(chipid[3]);
  printHex(chipid[4]); printHex(chipid[5]); printHex(0xff); printHex(0xff);
  Serial.println();
  
  // ==============
  // Device Address
  // ==============
  // Format 0x22334455
  DEVADDR = chipid[2] << 24 | chipid[3] << 16 | chipid[4] << 8 | chipid[5];
  Serial.print("Device Address: ");
  printHex(chipid[2]); printHex(chipid[3]); printHex(chipid[4]); printHex(chipid[5]);
  Serial.println();
}
