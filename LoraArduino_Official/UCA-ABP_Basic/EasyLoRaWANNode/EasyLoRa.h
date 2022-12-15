// ===================================================
// Put all pin mappings and library here
// ===================================================
// =====================
// Serial
// =====================
#define BAUD_RATE 115200

// =====================
// LoRaWAN
// =====================
#include <lmic.h>
#include <hal/hal.h>
#include <SPI.h>

// Sending message
static uint8_t mydata[] = "Hello, world!";

// LoRa pins - ESP32 - CHANGE HERE
#define LORA_SS 10
#define LORA_SCK 13
#define LORA_MOSI 11
#define LORA_MISO 12

#define LORA_DIO0 2
#define LORA_DIO1 4 // If not connected, can short connected DIO1 to DIO0
#define LORA_RESET 9

// LoRaWAN NwkSKey, network session key - CHANGE HERE
static const PROGMEM u1_t NWKSKEY[16] = {0x52, 0xC5, 0x24, 0x60, 0x93, 0x9B, 0x25, 0x27, 0x5C, 0xC5, 0x87, 0xE9, 0x7B, 0x86, 0x94, 0x9E};

// LoRaWAN AppSKey, application session key CHANGE HERE
static const u1_t PROGMEM APPSKEY[16] = {0x38, 0xB4, 0x78, 0xD9, 0xED, 0x43, 0xCE, 0xE1, 0x32, 0x5E, 0x56, 0x2F, 0x0D, 0x8B, 0x5B, 0xFA};

// LoRaWAN end-device address (DevAddr) - Auto generated
static u4_t DEVADDR = 0x005E249D; // MSB - Big-endian

// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in arduino-lmic/project_config/lmic_project_config.h,
// otherwise the linker will complain).
void os_getArtEui(u1_t *buf) {}
void os_getDevEui(u1_t *buf) {}
void os_getDevKey(u1_t *buf) {}

// payload to send to TTN gateway
static uint8_t payload[5];
static osjob_t sendjob;

// Schedule TX every this many seconds (might become longer due to duty
// cycle limitations).
const unsigned TX_INTERVAL = 60;

// Pin mapping
// https://github.com/mcci-catena/arduino-lmic#pin-mapping
// NSS and DIO0 are required, DIO1 is required for LoRa, DIO2 for FSK
const lmic_pinmap lmic_pins = {
    .nss = LORA_SS,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = LORA_RESET,
    .dio = {LORA_DIO0, LORA_DIO1, LMIC_UNUSED_PIN}};
