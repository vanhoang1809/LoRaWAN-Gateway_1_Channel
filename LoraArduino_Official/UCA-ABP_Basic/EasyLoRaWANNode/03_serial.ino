// ====================================
// Serial
// ====================================
void setupSerial() 
{
  Serial.begin(BAUD_RATE);
}

void endSerial() 
{
  Serial.end();
}

void printHex(uint8_t value)
{
  char tBuf[8];
  sprintf(tBuf,"%x",value); // lower case
  Serial.print(tBuf);
}
