// Easy LoRaWAN Industrial Node
// 433Mhz and ABP 
// Compatible for Single Channel gateway - Easy LoRaWAN Gateway
#include "E:\Semester 6\Project 6\SourceCode\LoraArduino_Official\UCA-ABP_Basic\EasyLoRaWANNode\EasyLoRa.h"
#include <C:\Users\bietk\AppData\Local\Arduino15\packages\arduino\hardware\avr\1.6.11\libraries\SPI\src\SPI.h>
// https://github.com/matthijskooijman/arduino-lmic/pull/34

void setup() 
{
  setupSerial();
  Serial.println(F("Starting"));

  // Set Device EUI and Device Address
  setDeviceID();

  // Make sure LoRa SS in the correct state
  pinMode(LORA_SS, INPUT);
  digitalWrite(LORA_SS, HIGH);
  
  // Set custom SPI
  SPI.begin();    
  
  // LMIC init
  os_init();
  
  // Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_reset();  
  //LMIC_setClockError(MAX_CLOCK_ERROR * 1 / 100);

  // Set static session parameters. Instead of dynamically establishing a session
  // by joining the network, precomputed session parameters are be provided.
  #ifdef PROGMEM
  // On AVR, these values are stored in flash and only copied to RAM
  // once. Copy them to a temporary buffer here, LMIC_setSession will
  // copy them into a buffer of its own again.
  uint8_t appskey[sizeof(APPSKEY)];
  uint8_t nwkskey[sizeof(NWKSKEY)];
  memcpy_P(appskey, APPSKEY, sizeof(APPSKEY));
  memcpy_P(nwkskey, NWKSKEY, sizeof(NWKSKEY));
  LMIC_setSession (0x1, DEVADDR, nwkskey, appskey);
  #else
  // If not running an AVR with PROGMEM, just use the arrays directly
  LMIC_setSession (0x1, DEVADDR, NWKSKEY, APPSKEY);
  #endif

  // Disable ADR
  LMIC_setAdrMode(false);

  // Disable channel 1-8
  for (uint8_t i = 1; i < 9; i++) 
  {
    LMIC_disableChannel(i);
  }
  
  // We'll only enable Channel 0 for a single-channel
  // LMIC_setupChannel(0, 433100000, DR_RANGE_MAP(DR_SF9, DR_SF9), BAND_CENTI);      // g-band  
  LMIC_setupChannel(0, 433175000, DR_RANGE_MAP(DR_SF9, DR_SF9), BAND_CENTI);         // g-band    
  // LMIC_setupChannel(1, 433100000, DR_RANGE_MAP(DR_SF9, DR_SF9), BAND_CENTI);      // g-band
  // LMIC_setupChannel(2, 433100000, DR_RANGE_MAP(DR_SF9, DR_SF9), BAND_CENTI);      // g-band
  // LMIC_setupChannel(3, 433100000, DR_RANGE_MAP(DR_SF9, DR_SF9), BAND_CENTI);      // g-band
  // LMIC_setupChannel(4, 433100000, DR_RANGE_MAP(DR_SF9, DR_SF9), BAND_CENTI);      // g-band
  // LMIC_setupChannel(5, 433100000, DR_RANGE_MAP(DR_SF9, DR_SF9), BAND_CENTI);      // g-band
  // LMIC_setupChannel(6, 433100000, DR_RANGE_MAP(DR_SF9, DR_SF9), BAND_CENTI);      // g-band
  // LMIC_setupChannel(7, 433100000, DR_RANGE_MAP(DR_SF9, DR_SF9), BAND_CENTI);      // g-band
  // LMIC_setupChannel(8, 433100000, DR_RANGE_MAP(DR_SF9, DR_SF9), BAND_MILLI);      // g-band

  // Disable link check validation
  LMIC_setLinkCheckMode(0);

  // SF for RX2 window.
  LMIC.dn2Dr = DR_SF9;

  // Set data rate and transmit power for uplink
  LMIC_setDrTxpow(DR_SF9,14);

  // Start job
  do_send(&sendjob);
}

void loop() {
  os_runloop_once();
}
