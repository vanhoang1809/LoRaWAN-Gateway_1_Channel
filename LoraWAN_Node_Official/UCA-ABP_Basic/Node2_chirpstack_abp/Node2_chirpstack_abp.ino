
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>
#include <DHT.h>
// LoRaWAN NwkSKey, network session key
// This is the default Semtech key, which is used by the early prototype TTN
// network.
static const PROGMEM u1_t NWKSKEY[16] = {0x25, 0xA1, 0x70, 0x79, 0xFB, 0xF1, 0xA5, 0xB2, 0xB5, 0x2E, 0x13, 0x9D, 0x46, 0x27, 0x88, 0x66};

// LoRaWAN AppSKey, application session key
// This is the default Semtech key, which is used by the early prototype TTN
// network.
static const u1_t PROGMEM APPSKEY[16] = {0x5F, 0xD5, 0x10, 0x59, 0x38, 0xE4, 0x1F, 0xE2, 0x52, 0x47, 0x6E, 0x4E, 0xD6, 0x6C, 0x77, 0x78};

// LoRaWAN end-device address (DevAddr)
static const u4_t DEVADDR = 0x01623fd6; // <-- Change this address for every node!
byte counter = 0;
// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in config.h, otherwise the linker will complain).
void os_getArtEui(u1_t *buf) {}
void os_getDevEui(u1_t *buf) {}
void os_getDevKey(u1_t *buf) {}

// static uint8_t  mydata[]= "";
static osjob_t sendjob;
// static uint8_t  humidity = "";
// static uint8_t temperature = "";

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 30;

// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = LMIC_UNUSED_PIN,
    .dio = {2, 4, 5},
};
// DHT11 & LedInfo
#define DHTPIN A0
#define LedInfo A2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void onEvent(ev_t ev)
{
    Serial.print(os_getTime());
    Serial.print(": ");
    switch (ev)
    {
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
        digitalWrite(A2, LOW);
        if (LMIC.txrxFlags & TXRX_ACK)
            Serial.println(F("Received ack"));
        if (LMIC.dataLen)
        {
            Serial.println(F("Received "));
            Serial.println(LMIC.dataLen);
            Serial.println(F(" bytes of payload"));
        }
        // Schedule next transmission
        os_setTimedCallback(&sendjob, os_getTime() + sec2osticks(TX_INTERVAL), do_send);
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

void do_send(osjob_t *j)
{
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND)
    {
        Serial.println(F("OP_TXRXPEND, not sending"));
    }
    else
    {
        digitalWrite(A2, HIGH);
        delay(10);
        float humidity = dht.readHumidity();
        float temp = dht.readTemperature();
        float Pressure = random(20, 30);

        byte buffer[32];
        String message = "" + String(temp) + "," + String(humidity) + "," + String(Pressure);
        message.getBytes(buffer, message.length() + 1);
        counter++;
        Serial.println("Sending: " + message);
        LMIC_setTxData2(1, (uint8_t *)buffer, message.length(), 0);
        Serial.println(F("Packet queued"));    
    }
    
    // Next TX is scheduled after TX_COMPLETE event.
}

void setup()
{
    Serial.begin(115200);
    Serial.println(F("Starting"));
    dht.begin();
    pinMode(LedInfo,OUTPUT);
#ifdef VCC_ENABLE
    // For Pinoccio Scout boards
    pinMode(VCC_ENABLE, OUTPUT);
    digitalWrite(VCC_ENABLE, HIGH);
    delay(1000);
#endif

    // LMIC init
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();

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
    LMIC_setSession(0x1, DEVADDR, nwkskey, appskey);
#else
    // If not running an AVR with PROGMEM, just use the arrays directly
    LMIC_setSession(0x1, DEVADDR, NWKSKEY, APPSKEY);
#endif

    LMIC_setAdrMode(false);

    // Disable channel 1-8
    for (uint8_t i = 1; i < 9; i++)
    {
        LMIC_disableChannel(i);
    }

    // We'll only enable Channel 0 for a single-channel
    // LMIC_setupChannel(0, 433100000, DR_RANGE_MAP(DR_SF9, DR_SF9), BAND_CENTI);      // g-band
    LMIC_setupChannel(0, 433175000, DR_RANGE_MAP(DR_SF9, DR_SF9), BAND_CENTI); // g-band
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

    // TTN uses SF9 for its RX2 window.
    LMIC.dn2Dr = DR_SF9;

    // Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)
    LMIC_setDrTxpow(DR_SF9, 14);

    // Start job
    do_send(&sendjob);
}

void loop()
{
    os_runloop_once();
}
