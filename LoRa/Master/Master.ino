
#include <SPI.h> // include libraries
#include <LoRa.h>

#define ss 10  // GPIO 15
#define rst 9  // GPIO 16
#define dio0 2 // GPIO 4

byte MasterNode = 0xFF;
byte Node1 = 0xBB;
byte Node2 = 0xCC;

String SenderNode = "";
String outgoing; // outgoing message

byte msgCount = 0; // count of outgoing messages
String incoming = "";

// Tracks the time since last event fired
unsigned long previousMillis = 0;
unsigned long int previoussecs = 0;
unsigned long int currentsecs = 0;
unsigned long currentMillis = 0;
int interval = 1; // updated every 1 second
int Secs = 0;

int temperature;
int humidity;
int soilmoisturepercent;
int soilMoistureValue;

void setup()
{
  Serial.begin(115200); // initialize serial
  delay(500);

  while (!Serial)
    ;

  Serial.println("LoRa Master Node");

  LoRa.setPins(ss, rst, dio0);

  if (!LoRa.begin(433E6))
  {
    Serial.println("Starting LoRa failed!");
    while (1)
      ;
  }
}

void loop()
{

  currentMillis = millis();
  currentsecs = currentMillis / 1000;
  if ((unsigned long)(currentsecs - previoussecs) >= interval)
  {
    Secs = Secs + 1;
    // Serial.println(Secs);
    if (Secs >= 11)
    {
      Secs = 0;
    }
    //    if ( (Secs >= 1) && (Secs <= 5) )
    //    {
    //
    //      String message = "10";
    //      sendMessage(message, MasterNode, Node1);
    //    }

    if ((Secs >= 1) && (Secs <= 10))
    {

      String message = "20";
      sendMessage(message, MasterNode, Node2);
    }

    previoussecs = currentsecs;
  }

  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
}

void sendMessage(String outgoing, byte MasterNode, byte otherNode)
{
  LoRa.beginPacket();            // start packet
  LoRa.write(otherNode);         // add destination address
  LoRa.write(MasterNode);        // add sender address
  LoRa.write(msgCount);          // add message ID
  LoRa.write(outgoing.length()); // add payload length
  LoRa.print(outgoing);          // add payload
  LoRa.endPacket();              // finish packet and send it
  msgCount++;                    // increment message ID
}

void onReceive(int packetSize)
{
  if (packetSize == 0)
    return; // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read(); // recipient address
  byte sender = LoRa.read();   // sender address
  if (sender == 0XBB)
    SenderNode = "Node1:";
  if (sender == 0XCC)
    SenderNode = "Node2:";
  byte incomingMsgId = LoRa.read();  // incoming msg ID
  byte incomingLength = LoRa.read(); // incoming msg length

  while (LoRa.available())
  {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length())
  { // check length for error
    // Serial.println("error: message length does not match length");
    ;
    return; // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != Node1 && recipient != MasterNode)
  {
    // Serial.println("This message is not for me.");
    ;
    return; // skip rest of function
  }

  //   if message is for this device, or broadcast, print details:
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();

  if (sender == 0XCC)
  {
    String t = getValue(incoming, ',', 0); // Temperature
    String h = getValue(incoming, ',', 1); // Humidity

    temperature = t.toInt();
    humidity = h.toInt();
    incoming = "";
    // clear display
    Serial.print(SenderNode);
    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.print(" ");
    Serial.print("C");
    //
    Serial.print("Humi: ");
    Serial.print(humidity);
    Serial.print(" %");
  }

  if (sender == 0XBB)
  {

    String v = getValue(incoming, ',', 0); // Soil Moisture Value
    String p = getValue(incoming, ',', 1); // Soil Moisture percentage

    soilMoistureValue = v.toInt();
    soilmoisturepercent = p.toInt();

    incoming = "";

    Serial.print(SenderNode);

    Serial.println("Soil RH:");

    Serial.print(soilmoisturepercent);

    Serial.println(" %");
  }
}
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
