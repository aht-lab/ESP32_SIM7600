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

#include "AHTLAB_SIM7600CE.h"

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

//    delay(5000);
//    if (GSM_ModeAutomatic())
//    {
//        Serial.println(F("Mode Automatic"));
//    }

    GSM_Init();
//    GSM_HttpGetRequest("http://5f40df51a5e9db0016302148.mockapi.io/api/v1/users", response, RESPONSE_LEN);
//    GSM_HttpPostRequest("http://5f40df51a5e9db0016302148.mockapi.io/api/v1/users", data, response, RESPONSE_LEN);

    delay(5000);
    if (GSM_ModeLTE())
    {
        Serial.println(F("Mode LTE"));
    }
    
    GSM_DownloadFile("http://vsh.pp.ua/TinyGSM/test_1k.bin", response, RESPONSE_LEN);

    Serial.println(F("*** Response ***"));
    Serial.println(response);
    Serial.println(F("****************"));
}

void loop()
{
    delay(1000);

    if (millis() - lasttime > TIMEOUT_DOWNLOAD)
    {
//        GSM_DownloadFile("http://vsh.pp.ua/TinyGSM/test_1k.bin", response, RESPONSE_LEN);
        lasttime = millis();
    }
}
