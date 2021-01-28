/**
 *    /_\ | || |_   _| |    /_\ | _ ) / __/ _ \|  \/  |
 *   / _ \| __ | | | | |__ / _ \| _ \| (_| (_) | |\/| |
 *  /_/ \_\_||_| |_| |____/_/ \_\___(_)___\___/|_|  |_| 
 * 
 *  Mini Library for Module SIM7600CE from AHTLAB
 *  Website: https://ahtlab.com
 *  Provides solutions for various types of sim modules
 * 
 */

// Custom Serial
// Use Serial2 by default ESP32
// #define _RX_GSM_ 11
// #define _TX_GSM_ 10

#include <AHTLAB_SIM7600CE.h>

// MAX SIZE OF FILE IS 2048 
#define RESPONSE_LEN 2048
char response[RESPONSE_LEN];
char data[] = "";

#define TIMEOUT_DOWNLOAD 1000
unsigned long lasttime = 0;

void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();

    GSM_Init();
}

void loop()
{
    if (Serial.available()) 
    {
        String at = Serial.readStringUntil('\r');
        GSM_SendAT(at.c_str());
    }

    if (_serial.available()) 
    {
        while(_serial.available()) 
        {
            char c = _serial.read();
            Serial.print(c);
            delay(1);
        }
    }
}