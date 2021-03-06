/*
  IoT raw Lora bridge: Take and send readings from sensors
  By: Sergio Rivera Lavado
  Date: April 2018
  https://github.com/srivera1/RAW_LoRa_IoT_bridge

  Hardware (always at 3.3V)
  -STM32 board (Blue pill)
  -Lora module (RF96)

  Hardware Connections
  -5V input power from USB (from power bank)
  -3.3V ouput power from STM32 board to LoRa module
  -GND = GND
  -MOSI = PA7            // LoRa radio
  -MISO = PA6            // LoRa radio
  -csPin = PB5;          // LoRa radio chip select
  -resetPin = PB4;       // LoRa radio reset
  -irqPin = PB3;         // LoRa hardware interrupt pin


  This code is released under the [MIT License](http://opensource.org/licenses/MIT).
*/

#include <SPI.h>              // include libraries
#include <LoRa.h>

const int csPin = PB7;          // LoRa radio chip select
const int resetPin = PB6;       // LoRa radio reset
const int irqPin = PB5;         // change for your board; must be a hardware interrupt pin

String outgoing;              // outgoing message

byte localAddress = 0xAD;           // address of this device
byte clientAddress = 0xAC;

void setup() {
  Serial.begin(2000000);                   // initialize serial
  while (!Serial);

  Serial.println("LoRa Duplex");

  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(csPin, resetPin, irqPin);// set CS, reset, IRQ pin

  if (!LoRa.begin(868E6)) {             // initialize ratio at 868 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

  /**/
  // TODO: implement more modes
  int Mode = -2;
  if (Mode == -1) { // XX bps
    LoRa.setSignalBandwidth(62.5E3);
    LoRa.setSpreadingFactor(7);           // ranges from 6-12,default 7 see API docs
    LoRa.setCodingRate4(8);
    LoRa.setPreambleLength(17);
  }
  else if (Mode == -2) { // 43 bps
    LoRa.setSignalBandwidth(41.7E3);
    LoRa.setSpreadingFactor(8);           // ranges from 6-12,default 7 see API docs
    LoRa.setCodingRate4(8);
    LoRa.setPreambleLength(17);
  }
  else if (Mode == -3) { // 43 bps
    LoRa.setSignalBandwidth(20.8E3);
    LoRa.setSpreadingFactor(9);           // ranges from 6-12,default 7 see API docs
    LoRa.setCodingRate4(8);
    LoRa.setPreambleLength(17);
  }
  else if (Mode == -4) { // 43 bps
    LoRa.setSignalBandwidth(15.6E3);
    LoRa.setSpreadingFactor(9);           // ranges from 6-12,default 7 see API docs
    LoRa.setCodingRate4(5);
    LoRa.setPreambleLength(17);
  }
  else if (Mode == 0) { // 43 bps
    LoRa.setSignalBandwidth(20.8E3);
    LoRa.setSpreadingFactor(11);           // ranges from 6-12,default 7 see API docs
    LoRa.setCodingRate4(8);
    LoRa.setPreambleLength(17);
  }
  else if (Mode == 1) { // 1476 bps
    LoRa.setSignalBandwidth(20.8E3);
    LoRa.setSpreadingFactor(6);           // ranges from 6-12,default 7 see API docs
    LoRa.setCodingRate4(5);
    LoRa.setPreambleLength(17);
  }
  else if (Mode == 2) { // 915 bps
    LoRa.setSignalBandwidth(62.5E3);
    LoRa.setSpreadingFactor(8);           // ranges from 6-12,default 7 see API docs
    LoRa.setCodingRate4(8);
    LoRa.setPreambleLength(17);
  }
  else if (Mode == 3) { // 8509 bps
    LoRa.setSignalBandwidth(250.0E3);
    LoRa.setSpreadingFactor(7);           // ranges from 6-12,default 7 see API docs
    LoRa.setCodingRate4(5); // check this value
    LoRa.setPreambleLength(17);
  }
  else if (Mode == 4) { // 17738 bps
    LoRa.setSignalBandwidth(250.0E3);
    LoRa.setSpreadingFactor(6);           // ranges from 6-12,default 7 see API docs
    LoRa.setCodingRate4(5);
    LoRa.setPreambleLength(17);
  }
  else if (Mode == 5) { // 104 bps
    LoRa.setSignalBandwidth(41.7E3);
    LoRa.setSpreadingFactor(11);           // ranges from 6-12,default 7 see API docs
    LoRa.setCodingRate4(8);
    LoRa.setPreambleLength(17);
  }
  else if (Mode == 6) { // 2959 bps
    LoRa.setSignalBandwidth(41.7E3);
    LoRa.setSpreadingFactor(6);           // ranges from 6-12,default 7 see API docs
    LoRa.setCodingRate4(5);
    LoRa.setPreambleLength(17);
  }
  else if (Mode == 7) { // 841 bps
    LoRa.setSignalBandwidth(20.8E3);
    LoRa.setSpreadingFactor(7);           // ranges from 6-12,default 7 see API docs
    LoRa.setCodingRate4(5);
    LoRa.setPreambleLength(17);
  }
  else if (Mode == 8) { // 4435 bps
    LoRa.setSignalBandwidth(62.5E3);
    LoRa.setSpreadingFactor(6);           // ranges from 6-12,default 7 see API docs
    LoRa.setCodingRate4(5);
    LoRa.setPreambleLength(17);
  }
  else if (Mode == 9) {
    // not supported
    LoRa.setSignalBandwidth(500.0E3);
    LoRa.setSpreadingFactor(6);           // ranges from 6-12,default 7 see API docs
    LoRa.setCodingRate4(5);
    LoRa.setPreambleLength(17);
  }
  /**/

  Serial.println("LoRa init succeeded.");
}

void loop() {

  onReceive(LoRa.parsePacket(235));
}


void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length
  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();

  }

  int separator[] = {0, 0, 0};
  int k = 0;
  for (int i = 0; i < incoming.length(); i++) {
    if (incoming[i] == ';') {
      separator[k++] = i;
    }
  }
  int  incomingSize = incoming.substring( 0 , separator[0] ).toInt();
  String finalMsg = "";
  for (int j = separator[0] - 1; j < incomingSize; j ++ ) {
    char finalVal = twoBestFromThree(incoming [j + separator[0]],
                                     incoming [j + separator[1]],
                                     incoming [j + separator[2]]);
    finalMsg += finalVal;

  }
  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != clientAddress) {
    // Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }

  // if message is for this device, or broadcast, print details:
  /*Serial.println("Received from: 0x" + String(sender, HEX));
    Serial.println("Sent to: 0x" + String(recipient, HEX));
    Serial.println("Message ID: " + String(incomingMsgId));
    Serial.println("Message length: " + String(incomingLength));
    Serial.println("Message: " + incoming);
    Serial.println("RSSI: " + String(LoRa.packetRssi()));
    Serial.println("Snr: " + String(LoRa.packetSnr())); */
  if (k == 4) {
    Serial.println(finalMsg + ',' + String(LoRa.packetRssi()) + ',' + String(LoRa.packetSnr()) );
  }
  //Serial.println();
}

// Error correction, "best 2 out of 3"
byte twoBestFromThree (byte x, byte y, byte z) {

  byte finalVal;

  bool arraybol[] = {false, false, false, false, false, false, false, false};
  for (int bit = 7; bit >= 0; bit--) {
    // add true to array
    arraybol[bit] = (x & (1 << bit)) == (y & (1 << bit)) ? (x & (1 << bit)) :
                    (x & (1 << bit)) == (z & (1 << bit)) ? (x & (1 << bit)) :
                    (y & (1 << bit));
  }
  return byte(BoolArrayToByte(arraybol));
}

byte BoolArrayToByte(bool boolArray[8])
{
  byte result = 0;

  for (int i = 0; i < 8; i++)
  {
    if (boolArray[i])
    {
      result = result | (1 << i);
    }
  }
  return result;
}
