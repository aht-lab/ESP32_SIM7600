/*
 * AHTLAB_SIM7600CE.h
 * Mini library with some useful functions for SIM7600CE module
 * Website: https://ahtlab.com
 *
 *    /_\ | || |_   _| |    /_\ | _ ) / __/ _ \|  \/  |
 *   / _ \| __ | | | | |__ / _ \| _ \| (_| (_) | |\/| |
 *  /_/ \_\_||_| |_| |____/_/ \_\___(_)___\___/|_|  |_| 
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef sim7600ce_h
#define sim7600ce_h

#include <Arduino.h>
//#include <CRC32.h>

#if defined(_TX_GSM_) && defined(_RX_GSM_)
    #include <SoftwareSerial.h>
    SoftwareSerial _serial(_TX_GSM_, _RX_GSM_);
#elif defined(ESP32) || defined(MEGA)
    #define _serial Serial2
#else
    #error "GSM Serial must be define! To use custom serial: #define _TX_GSM_ AND _RX_GSM_ or ESP32 or MEGA board"
#endif

#define AT_OK_STR "OK"
#define AT_ERROR_STR "ERROR"

#ifndef BUFFER_MAX_LEN
#define BUFFER_MAX_LEN 2048
#endif

typedef enum gsm_at_response_t
{
    AT_FOUND, 
    AT_OK,
    AT_ERROR
} gsm_at_response_t;

uint32_t Global_timeSendAT = 0;
char Global_buffer[BUFFER_MAX_LEN];
uint16_t Global_bufferLen = 0;

bool Global_tcpConnected = false, Global_udpConnected = false;

void GSM_SendAT(const char* at)
{
    while (_serial.available())
    {
        _serial.read();
        delay(1);
    }

    Global_buffer[0] = '\0';
    Global_bufferLen = 0;
	
	Serial.println();
    Serial.println(at);
    _serial.println(at);

    //Global_timeSendAT = millis();
}

gsm_at_response_t GSM_ReadAT(const char* foundResponse, unsigned long timeout, bool foundBreak = false)
{
    // while (millis() - Global_timeSendAT < timeout)
	while (timeout > 0)
    {
        while(_serial.available())
        {            
            if(Global_bufferLen > BUFFER_MAX_LEN - 2)
            { 
                _serial.read();
				delay(1);
                continue;
            }
            
            char c = _serial.read();
            // Serial.print(c);
			
            if(Global_bufferLen == 0 && c == '\n') 
            {
                continue;
            }
    
            Global_buffer[Global_bufferLen++] = c;   
            Global_buffer[Global_bufferLen] = '\0';
			
			if (foundBreak)
			{
				if (strstr(Global_buffer, foundResponse) != nullptr)
				{				
					delay(100);
					while (_serial.available())
					{
						char c = _serial.read();
						// Serial.print(c);

						if (Global_bufferLen < BUFFER_MAX_LEN - 2)
						{
							Global_buffer[Global_bufferLen++] = c;
							Global_buffer[Global_bufferLen] = '\0';
						}
						
						delay(1);
					}

					return AT_OK;
				}
			}
			
            delay(1);
        }
		
		--timeout;
        delay(1);
    }

    if (strstr(Global_buffer, foundResponse) != nullptr)
    {
        Serial.println(Global_buffer);
        Serial.println(F("AT FOUND"));
        return AT_OK;   
    }

    if (strstr(Global_buffer, AT_OK_STR) != nullptr)
    {
        Serial.println(Global_buffer);
        Serial.println(F("AT NOT FOUND BUT GET OK"));
        return AT_OK;
    }

    if (strstr(Global_buffer, AT_ERROR_STR) != nullptr)
    {
        Serial.println(Global_buffer);
        Serial.println(F("AT ERROR"));
        return AT_ERROR;   
    }
    
    Serial.println(Global_buffer);
    Serial.println(F("NOT FOUND REPLY"));

    return AT_ERROR;
}

void GSM_Init(uint32_t baudrate)
{
    Serial.println(F("\n/*"));
    Serial.println("Mini Library for Module SIM7600CE from AHTLAB\n");
    Serial.println("Website: https://ahtlab.com\n");
    Serial.println("Provides solutions for various types of sim modules");
    Serial.println(F("*/\n"));

    _serial.begin(baudrate);

    #if defined(USE_7600CE_BAUDRATE_9600)
	GSM_SendAT("AT+IPR=9600");
    #endif
	if (GSM_ReadAT("OK", 1000) != AT_ERROR)
	{
        #if defined(USE_7600CE_BAUDRATE_9600)
        _serial.begin(9600);
        #endif
	}
	
    delay(1000);
}

void GSM_Init()
{
    Serial.println(F("\n/*"));
    Serial.println(F(R"=====(
   /_\ | || |_   _| |    /_\ | _ ) / __/ _ \|  \/  |
  / _ \| __ | | | | |__ / _ \| _ \| (_| (_) | |\/| |
 /_/ \_\_||_| |_| |____/_/ \_\___(_)___\___/|_|  |_|
    )====="));
    Serial.println("Mini Library for Module SIM7600CE from AHTLAB\n");
    Serial.println("Website: https://ahtlab.com\n");
    Serial.println("Provides solutions for various types of sim modules");

    delay(500);
    _serial.begin(115200);
    delay(500);

    Serial.println(F("Initilizing..."));

    #if defined(USE_7600CE_BAUDRATE_9600)

    Serial.println(F("USE BAUDRATE 9600"));
	GSM_SendAT("AT");
    if (GSM_ReadAT("OK", 1000) == AT_ERROR)
	{
		_serial.begin(9600);
	}
	else 
	{
		GSM_SendAT("AT+IPR=9600");
		if (GSM_ReadAT("OK", 1000) != AT_ERROR)
		{
			_serial.begin(9600);
		}
	}
	
	GSM_SendAT("ATE0");
    GSM_ReadAT("OK", 1000);
    #else

    Serial.println(F("USE BAUDRATE 115200"));
    uint8_t times = 0;
    bool success = false;
    while (success == false && times < 5)
    {
        GSM_SendAT("AT");
        success = GSM_ReadAT("OK", 1500) == AT_OK;

        ++times;
    }

    if (success == false)
    {
        _serial.begin(9600);
        GSM_SendAT("AT+IPR=115200");
        if (GSM_ReadAT("OK", 1000) == AT_OK)
        {
            success = true;
        }
        _serial.begin(115200);
    }

	if (success)
    {
        GSM_SendAT("ATE0");
        GSM_ReadAT("OK", 1000);
    }
    #endif

    if (success) 
    {
        Serial.println(F("Init success"));
    }
    else 
    {
        Serial.println(F("Init failed"));
    }
    
    Serial.println(F("\n"));
}

bool GSM_HttpGetRequest(const char* url, char* response, const uint16_t len)
{
    char buffer[128];

    GSM_SendAT("AT+CSQ");
    GSM_ReadAT("OK", 300);

    GSM_SendAT("AT+CREG?");
    GSM_ReadAT("OK", 300);

    GSM_SendAT("AT+CGREG?");
    GSM_ReadAT("OK", 300);
    
    GSM_SendAT("AT+CGSOCKCONT=1,\"IP\",\"CMNET\"");
    if (GSM_ReadAT("OK", 1000) == AT_ERROR) 
    {
        return 0;
    }
    
    GSM_SendAT("AT+CGPADDR");
    if (GSM_ReadAT("OK", 1000) == AT_ERROR) 
    {
        return 0;
    }

    GSM_SendAT("AT+HTTPTERM");
    GSM_ReadAT("OK", 2000L);

    GSM_SendAT("AT+HTTPINIT");
    if (GSM_ReadAT("OK", 2000L) == AT_ERROR) 
    {
        return 0;
    }

    sprintf(buffer, "AT+HTTPPARA=\"URL\",\"%s\"", url);
    GSM_SendAT(buffer);
    if (GSM_ReadAT("OK", 2000L) == AT_ERROR) 
    {
        return 0;
    }
    
    GSM_SendAT("AT+HTTPACTION=0");
    if (GSM_ReadAT("OK", 5000L) == AT_ERROR) 
    {
        return 0;
    }

    // +HTTPACTION: 0,200,1024
    int code = -1, resLen = 0;
    
    char* p1 = strstr(Global_buffer, "+HTTPACTION:");
    char* p2 = nullptr;
    if (p1 == nullptr)
    {
        return 0;
    }
    p1 = strchr(Global_buffer, ',');
    if (p1 != nullptr)
    {
        p2 = strchr(p1 + 1, ',');
        if (p2 != nullptr)
        {
            *p2 = '\0';
            code = atoi(p1 + 1);
            Serial.print(F("code: "));
            Serial.println(code);
    
            p1 = p2 + 1;
            p2 = strchr(p1, '\r');
            if (p1 != nullptr && p2 != nullptr)
            {
                *p2 = '\0';
                resLen = atoi(p1);
                Serial.print(F("len: "));
                Serial.println(resLen);
            }   
        }
    }

    if (code == -1 || resLen == 0)
    {
        Serial.println(F("download file -> content len = 0"));
        GSM_SendAT("AT+HTTPTERM");
        GSM_ReadAT("OK", 2000L);
    }
    
    GSM_SendAT("AT+HTTPHEAD");
    if (GSM_ReadAT("OK", 2000L) == AT_ERROR) 
    {
        return 0;
    }

    char buf[25];
    sprintf(buf, "AT+HTTPREAD=0,%d", resLen);
    GSM_SendAT(buf);
    if (GSM_ReadAT("+HTTPREAD:", 5000L) == AT_ERROR) 
    {
        return 0;
    }
    
    memset(buf, '\0', 25);
    sprintf(buf, "+HTTPREAD: %d", resLen);
    char* startBody = strstr(Global_buffer, buf);
    if (startBody == nullptr) 
    {
        return 0;
    }
    
    startBody = strstr(startBody, "\r\n");
    if (startBody != nullptr) 
    {
        startBody = startBody + 2;
        strcpy(response, startBody);
    }

    GSM_SendAT("AT+HTTPTERM");
    GSM_ReadAT("OK", 2000L);

    return true;
}

bool GSM_HttpPostRequest(const char* url, const char* data, char* response, const uint16_t len)
{
    char buffer[128];

    GSM_SendAT("AT+CSQ");
    GSM_ReadAT("OK", 300);

    GSM_SendAT("AT+CREG?");
    GSM_ReadAT("OK", 300);

    GSM_SendAT("AT+CGREG?");
    GSM_ReadAT("OK", 300);
    
    GSM_SendAT("AT+CGSOCKCONT=1,\"IP\",\"CMNET\"");
    if (GSM_ReadAT("OK", 1000) == AT_ERROR) 
    {
        return 0;
    }
    
    GSM_SendAT("AT+CGPADDR");
    if (GSM_ReadAT("OK", 1000) == AT_ERROR) 
    {
        return 0;
    }

    GSM_SendAT("AT+HTTPTERM");
    GSM_ReadAT("OK", 2000L);

    GSM_SendAT("AT+HTTPINIT");
    if (GSM_ReadAT("OK", 2000L) == AT_ERROR) 
    {
        return 0;
    }

    sprintf(buffer, "AT+HTTPPARA=\"URL\",\"%s\"", url);
    GSM_SendAT(buffer);
    if (GSM_ReadAT("OK", 2000L) == AT_ERROR) 
    {
        return 0;
    }

    memset(buffer, '\0', sizeof(buffer));
    sprintf(buffer, "AT+HTTPDATA=%d,1000", strlen(data));
    GSM_SendAT(buffer);
    if (GSM_ReadAT("DOWNLOAD", 1000L) == AT_ERROR) 
    {
        return 0;
    }

    GSM_SendAT(data);
    if (GSM_ReadAT("OK", 1000L) == AT_ERROR) 
    {
        return 0;
    }
    
    GSM_SendAT("AT+HTTPACTION=1");
    if (GSM_ReadAT("+HTTP_PEER_CLOSED", 8000L) == AT_ERROR) 
    {
        return 0;
    }
    
    GSM_SendAT("AT+HTTPHEAD");
    if (GSM_ReadAT("+HTTPHEAD:", 3000L) == AT_ERROR) 
    {
        return 0;
    }

    GSM_SendAT("AT+HTTPREAD=0,500");
    if (GSM_ReadAT("OK", 3000L) == AT_ERROR) 
    {
        return 0;
    }
    
    // TODO: optimize to sprintf("+HTTPREAD: DATA,%d");
    // +HTTPREAD: DATA,500
    char* startBody = strstr(Global_buffer, "+HTTPREAD: ");
    if (startBody == nullptr) 
    {
        return 0;
    }
    
    startBody = strstr(startBody, "\r\n");
    if (startBody != nullptr) 
    {
        startBody = startBody + 2;
        strcpy(response, startBody);
    }

    GSM_SendAT("AT+HTTPTERM");
    GSM_ReadAT("OK", 2000L);

    return true;
}

bool GSM_DownloadFile(const char* url, char* response, const uint16_t len)
{
    char buffer[128];

    GSM_SendAT("AT+CSQ");
    GSM_ReadAT("OK", 300);

    GSM_SendAT("AT+CREG?");
    GSM_ReadAT("OK", 300);

    GSM_SendAT("AT+CGREG?");
    GSM_ReadAT("OK", 300);
    
    GSM_SendAT("AT+CGSOCKCONT=1,\"IP\",\"CMNET\"");
    if (GSM_ReadAT("OK", 1000) == AT_ERROR) 
    {
        return 0;
    }
    
    GSM_SendAT("AT+CGPADDR");
    if (GSM_ReadAT("OK", 1000) == AT_ERROR) 
    {
        return 0;
    }

    GSM_SendAT("AT+HTTPTERM");
    GSM_ReadAT("OK", 2000L);

    GSM_SendAT("AT+HTTPINIT");
    if (GSM_ReadAT("OK", 2000L) == AT_ERROR) 
    {
        return 0;
    }

    sprintf(buffer, "AT+HTTPPARA=\"URL\",\"%s\"", url);
    GSM_SendAT(buffer);
    if (GSM_ReadAT("OK", 2000L) == AT_ERROR) 
    {
        return 0;
    }
    
    GSM_SendAT("AT+HTTPACTION=0");
    if (GSM_ReadAT("OK", 5000L) == AT_ERROR) 
    {
        return 0;
    }

    // +HTTPACTION: 0,200,1024
    int code = -1, resLen = 0;
    
    char* p1 = strstr(Global_buffer, "+HTTPACTION:");
    char* p2 = nullptr;
    if (p1 == nullptr)
    {
        return 0;
    }
    p1 = strchr(Global_buffer, ',');
    if (p1 != nullptr)
    {
        p2 = strchr(p1 + 1, ',');
        if (p2 != nullptr)
        {
            *p2 = '\0';
            code = atoi(p1 + 1);
            Serial.print(F("code: "));
            Serial.println(code);
    
            p1 = p2 + 1;
            p2 = strchr(p1, '\r');
            if (p1 != nullptr && p2 != nullptr)
            {
                *p2 = '\0';
                resLen = atoi(p1);
                Serial.print(F("len: "));
                Serial.println(resLen);
            }   
        }
    }

    if (code == -1 || resLen == 0)
    {
        Serial.println(F("download file -> content len = 0"));
        GSM_SendAT("AT+HTTPTERM");
        GSM_ReadAT("OK", 2000L);
    }
    
    GSM_SendAT("AT+HTTPHEAD");
    if (GSM_ReadAT("OK", 2000L) == AT_ERROR) 
    {
        return 0;
    }

    char buf[25];
    sprintf(buf, "AT+HTTPREAD=0,%d", resLen);
    GSM_SendAT(buf);
    if (GSM_ReadAT("+HTTPREAD:", 5000L) == AT_ERROR) 
    {
        return 0;
    }
    
    memset(buf, '\0', strlen(buf));
    sprintf(buf, "+HTTPREAD: %d", resLen);
    char* startBody = strstr(Global_buffer, buf);
    if (startBody == nullptr) 
    {
        return 0;
    }
    
    startBody = strstr(startBody, "\r\n");
    if (startBody != nullptr) 
    {
        startBody = startBody + 2;
        strcpy(response, startBody);
    }
    
    GSM_SendAT("AT+HTTPTERM");
    GSM_ReadAT("OK", 2000L);

    return true;
}

bool GSM_ModeLTE()
{
    GSM_SendAT("AT+CNMP=?");
    if (GSM_ReadAT("OK", 1000L) == AT_ERROR) 
    {
        return 0;
    }

    GSM_SendAT("AT+CNMP=38");
    if (GSM_ReadAT("OK", 1000L) == AT_ERROR) 
    {
        return 0;
    }
}

bool GSM_ModeAutomatic()
{
    GSM_SendAT("AT+CNMP=?");
    if (GSM_ReadAT("OK", 1000L) == AT_ERROR) 
    {
        return 0;
    }

    GSM_SendAT("AT+CNMP=2");
    if (GSM_ReadAT("OK", 1000L) == AT_ERROR) 
    {
        return 0;
    }
}

bool GSM_UDPClose() 
{
    Global_udpConnected = false;
    
    GSM_SendAT("AT+CIPCLOSE=1");
    GSM_ReadAT("OK", 500L, true);

    GSM_SendAT("AT+NETCLOSE");
    GSM_ReadAT("OK", 500L, true);

    return 1;
}

bool GSM_UDPOpen(const char* host, uint16_t port) 
{
    char buf[56];

    // GSM_SendAT("AT+CGSOCKCONT=1,\"IP\",\"CMNET\"");
    // if (GSM_ReadAT("OK", 1000L, true) == AT_ERROR) 
    // {
    //     return 0;
    // }

    GSM_SendAT("AT+NETOPEN");
    if (GSM_ReadAT("OK", 1000L, true) == AT_ERROR) 
    {
        GSM_UDPClose();
        
        GSM_SendAT("AT+NETOPEN");
        if (GSM_ReadAT("OK", 1000L, true) == AT_ERROR) 
        {
            GSM_UDPClose();
            return 0;
        }
    }

    memset(buf, '\0', strlen(buf));
    sprintf(buf, "AT+CIPOPEN=1,\"UDP\",\"%s\",,%d", host, port);
    GSM_SendAT(buf);
    if (GSM_ReadAT("OK", 1000L, true) == AT_ERROR) 
    {
        GSM_UDPClose();
        return 0;
    }

    Global_udpConnected = true;

    return 1;
}

bool GSM_UDPSend(const char* host, uint16_t port, const char* data, uint16_t len) 
{
    char buf[56];

    if (Global_udpConnected == false) 
    {
        GSM_UDPOpen(host, port);
    }

    memset(buf, '\0', strlen(buf));
    sprintf(buf, "AT+CIPSEND=1,%d,\"%s\",%d", len, host, port);
    GSM_SendAT(buf);
    if (GSM_ReadAT(">", 3000L, true) == AT_ERROR) 
    {
        GSM_UDPClose();
        return 0;
    }

    GSM_SendAT(data);
    if (GSM_ReadAT("OK", 1500L, true) == AT_ERROR) 
    {
        GSM_UDPClose();
        return 0;
    }

    return 1;
}

bool GSM_TCPClose() 
{
    Global_tcpConnected = false;

    GSM_SendAT("AT+CIPCLOSE=1");
    GSM_ReadAT("OK", 500L, true);

    GSM_SendAT("AT+NETCLOSE");
    GSM_ReadAT("OK", 500L, true);

    return 1;
}

bool GSM_TCPOpen(const char* host, uint16_t port) 
{
    char buf[56];

    // GSM_SendAT("AT+CGSOCKCONT=1,\"IP\",\"CMNET\"");
    // if (GSM_ReadAT("OK", 1000L, true) == AT_ERROR) 
    // {
    //     return 0;
    // }

    GSM_SendAT("AT+NETOPEN");
    if (GSM_ReadAT("OK", 1000L, true) == AT_ERROR) 
    {
        GSM_TCPClose();
        
        GSM_SendAT("AT+NETOPEN");
        if (GSM_ReadAT("OK", 1000L, true) == AT_ERROR) 
        {
            GSM_TCPClose();
            return 0;
        }
    }

    memset(buf, '\0', strlen(buf));
    sprintf(buf, "AT+CIPOPEN=1,\"TCP\",\"%s\",%d", host, port);
    GSM_SendAT(buf);
    if (GSM_ReadAT("OK", 2500L) == AT_ERROR) 
    {
        GSM_TCPClose();
        return 0;
    }

    Global_tcpConnected = true;

    return 1;
}

bool GSM_TCPSend(const char* host, uint16_t port, const char* data, uint16_t len) 
{
    char buf[56];

    if (Global_tcpConnected == false) 
    {
        GSM_TCPOpen(host, port);
    }

    memset(buf, '\0', strlen(buf));
    sprintf(buf, "AT+CIPSEND=1,%d", len);
    GSM_SendAT(buf);
    if (GSM_ReadAT(">", 3000L, true) == AT_ERROR) 
    {
        GSM_TCPClose();
        return 0;
    }

    GSM_SendAT(data);
    if (GSM_ReadAT("OK", 3500L, true) == AT_ERROR) 
    {
        GSM_TCPClose();
        return 0;
    }

    return 1;
}

#endif
