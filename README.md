# RAW_LoRa_IoT_bridge
## IoT raw Lora bridge: Take and send readings from sensors
By: Sergio Rivera Lavado
Date: April 2018
https://github.com/srivera1/RAW_LoRa_IoT_bridge

Hardware (3.3V):
-STM32 board (Blue pill)
-Lora module (RF96)
#
Hardware Connections
#
-5V input power from USB (from power bank)
#
-3.3V ouput power from STM32 board to LoRa module
#
-GND = GND
#
-MOSI = PA7            // LoRa radio
#
-MISO = PA6            // LoRa radio
#
-csPin = PB5;          // LoRa radio chip select
#
-resetPin = PB4;       // LoRa radio reset
#
-irqPin = PB3;         // LoRa hardware interrupt pin
#
This code is released under the [MIT License](http://opensource.org/licenses/MIT).
