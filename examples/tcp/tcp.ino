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
#include <AHTLAB_SIM7600CE.h>

#define serialData Serial2
#define baudrate 115200

#define HOST "14.161.34.188"
#define PORT 2502

void setup()
{
    Serial.begin(115200);
    delay(500);

    serialData.begin(baudrate);
    delay(500);

    GSM_Init();

    GSM_TCPOpen(HOST, PORT);
}

void loop()
{
    if (serialData.available()) 
    {
        String data = serialData.readStringUntil('\r');
        GSM_TCPSend(HOST, PORT, data.c_str(), data.length());
    }

    // TESTING FROM PC ============================ //
    if (Serial.available()) 
    {
        String data = Serial.readStringUntil('\r');
        GSM_TCPSend(HOST, PORT, data.c_str(), data.length());
    }

    delay(100);
}