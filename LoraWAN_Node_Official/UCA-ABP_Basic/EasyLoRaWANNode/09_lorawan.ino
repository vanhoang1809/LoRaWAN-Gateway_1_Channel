// ===================================================
// MCCI Arduino-LMIC for LoRaWAN
// ===================================================

void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            break;
        case EV_RFU1:
            Serial.println(F("EV_RFU1"));
            break;
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));

            // Easy LoRa, begin
            Serial.print(F("Frequency "));
            Serial.println(LMIC.freq);

            Serial.print(F("DR "));
            Serial.println(LMIC.datarate);
            
            if (LMIC.txrxFlags & TXRX_ACK)
              Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
              Serial.print(F("Received "));
              Serial.print(LMIC.dataLen);
              Serial.println(F(" bytes of payload"));

              // To decode at http://www.unit-conversion.info/texttools/hexadecimal/
              DisplayHex(LMIC.frame+LMIC.dataBeg, LMIC.dataLen, false);   // Get the data              
            }
            // Easy LoRa, end

            // TODO: This one will stuck if TX_COMPLETE never ends
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            break;
         default:
            Serial.println(F("Unknown event"));
            break;
    }
}

void do_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        // Prepare upstream data transmission at the next possible time.
        // Unconfirmed Data Up
        LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 0);

        // Confirmed Data Up
        // LMIC_setTxData2(1, mydata, sizeof(mydata)-1, 1);
        Serial.println(F("Packet queued"));
    }
    // Next TX is scheduled after TX_COMPLETE event.
}

// Will display a byte array in hex
void DisplayHex(uint8_t * key, uint8_t len, bool lsb) 
{
  uint8_t start=lsb?len:0;
  uint8_t end = lsb?0:len;
  uint8_t * p ;
    //debug(PSTR(("0x"));
  for (uint8_t i=0; i<len ; i++) {
    p = lsb ? key+len-i-1 : key+i;
    // debug(PSTR("%02X "), *p);
    // Serial.print("%02X ", *p);
    Serial.print(*p, HEX);
  }
  // debug(PSTR("\n"));
  Serial.println();
}
