/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <string.h>
#include <stdio.h>
#include "app.h"
#include "WFI32/WFI32_INIT.h"
#include "definitions.h"
#include "app_zigbee/app_zigbee.h"
#include <z3device/common/include/z3Device.h>
#include <z3device/stack_interface/zgb_api.h>
#include <z3device/stack_interface/bdb/include/bdb_api.h>
#include <osal/osal_freertos.h>
/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */

extern int wfi32_initialized;
extern APP_UART_DATA appUart;
extern credentials cred;
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

int global_data;
char *dataBufferPtr;
char data_received = 0;
char test = 0;
bool readLineComplete = true;
char *linePtrBuffer;
uint16_t numberOfBytes;
char linearBuffer[2048];
static uint8_t mqttCmdTblIndex;
static uint8_t stationCmdTblIndex;

char SSID[20]="MicrochipDemoAP";
char password[20]="12345678";

static command_structure Station_mode[]=
{
    { 0, "AT+WSTAC=1,\"%s\"\r\n", cred.ssid},
    { 1, "AT+WSTAC=2,3\r\n", NULL},
    { 2, "AT+WSTAC=3,\"%s\"\r\n", cred.passphrase},
    { 3, "AT+WSTAC=4,255\r\n", NULL}, //Ensure NTP is enable
    { 4, "AT+WSTAC=12,\"pool.ntp.org\"\r\n", NULL}, //set NTP server
    { 5, "AT+WSTAC=13,1\r\n", NULL}, //Enable static (don't use NTP address provided during DHCP)
//    { 6, "AT+WSTA=1\r\n", NULL}, //Connect
};

char broker[20]="broker.hivemq.com";
char port[20]="1883";
char keepAlive[20]="60";
char TLS[20]="0";


static command_structure mqttCmdTbl[] = 
{
    { 0, "AT+MQTTC=1,\"%s\"\r\n", broker},
    { 1, "AT+MQTTC=2,1883\r\n", port},
    { 2, "AT+MQTTC=3,\"mqttexplorer12584976\"\r\n", NULL},
    { 3, "AT+MQTTC=6,0\r\n", NULL},
    { 4, "AT+MQTTC=7,0\r\n", NULL}, //Use TLS Idx "1" for secure connection
//    { 5, "AT+MQTTCONN=1\r\n", NULL},
};

bool gRDY, gOK, gATE, gGMM, gCFG, gTIME, gSOCKO, gSOCKCL, gWSTALU, gWSTALD, gDNSRESOLV,
gSOCKIND, gSOCKLST, gSOCKRXT, gSOCKRD, gSOCKRXU, gWSTAAIP, gWAPAIP,
gMQTTCONN, gMQTTPUB, gMQTTSUB, gMQTTPUBACC, gMQTTPUBCOMP, gMQTTPUBERR,
gSOCKWR_Error, gSOCKID_Error, gWSCNDONE, gWSCNIND; 


static uint8_t atCmdTbleIndex;
static ATCMD_RESPONSE ATCmdResponseTbl[] = {

    { "AT Command Application", &gRDY, NULL}, // Start Banner of RN-RIO2
    { "OK", &gOK, NULL}, //  OK
    { "ATE0", &gATE, NULL}, //  ATE0
    { "ERROR:", &gRDY, NULL}, //  ERROR
    { "+GMM=", &gGMM, NULL}, //  +GMM:<MODEL_ID>
    { "+CFG:", &gCFG, NULL}, //  +CFG:<param_id>,<param_val>   1	<MAC_ADDR>	String	The MAC address of the device (Read Only)  2	<DEVICE_NAME>	String	The device name
    { "+TIME:", &gTIME, NULL}, //  +TIME:<TIME_FORMAT>,<TIME>  //<1 - UNIX timestamp>  <2 - NTP time>  <3 - RFC3338 / ISO-8601 string format>

    { "+WSTALU:", &gWSTALU, NULL}, //+WSTALU: <ASSOC_ID>,<BSSID>,<CHANNEL>
    { "+WSTALD:", &gWSTALD, NULL}, //+WSTALD: <ASSOC_ID>
    { "+WSTAAIP:", &gWSTAAIP, NULL}, //+WSTAAIP:1,"10.0.0.196"
    { "+WAPAIP:", &gWAPAIP, NULL}, //+WAPAIP: <ASSOC_ID>,<IP_ADDRESS>
    { "+DNSRESOLV:", &gDNSRESOLV, NULL}, //+DNSRESOLV:0,"api.openweathermap.org","192.241.245.161"
    { "+WSCNIND:", &gWSCNIND, NULL}, //+WSCNIND:<RSSI>,<SEC_TYPE>,<CHANNEL>,<BSSID>,<SSID>
    { "+WSCNDONE", &gWSCNDONE, NULL}, //+WSCNDONE

    { "+MQTTCONN:", &gMQTTCONN, NULL}, //+MQTTCONN:0 or +MQTTCONN:1
    { "+MQTTPUB:", &gMQTTPUB, NULL}, //+MQTTPUB:<TOPIC_NAME_LENGHT>,"<TOPIC_NAME>",<TOPIC_PAYLOAD_LENGHT>,"<TOPIC_PAYLOAD>"
    { "+MQTTSUB:", &gMQTTSUB, NULL}, //+MQTTPUB:<TOPIC_NAME_LENGHT>,"<TOPIC_NAME>",<TOPIC_PAYLOAD_LENGHT>,"<TOPIC_PAYLOAD>"
    { "+MQTTPUBACC", &gMQTTPUBACC, NULL}, //+MQTTPUBACC  QoS(1)
    { "+MQTTPUBCOMP", &gMQTTPUBCOMP, NULL}, //+MQTTPUBCOMP  QoS(2)
    { "+MQTTPUBERR", &gMQTTPUBERR, NULL}, //+MQTTPUBERR:

};


#define ATCMD_RESPONSE_SIZE sizeof (ATCmdResponseTbl) / sizeof (*ATCmdResponseTbl)



void parseRIO2RxMessage(char *buffer) {
    char *str;
    uint8_t i;

    for (i = 0; i < ATCMD_RESPONSE_SIZE; i++) 
    {
        if (str=strstr((char *) buffer, (char *) ATCmdResponseTbl[i].atCmdResp)) 
        {
            atCmdTbleIndex = i;
            *ATCmdResponseTbl[i].flag = true;

            if (ATCmdResponseTbl[i].func) //Available CallBack?
            {
            }
        }
    }

}







uint16_t read_data(char *dataBuffer) 
{
    int count;

    count = SERCOM1_USART_ReadCountGet();
    if (count == 0)
        return 0;
    if (!linePtrBuffer) //
        linePtrBuffer = dataBuffer; //Initialize to buffer address

    do {
        uint8_t byte;

        if (SERCOM1_USART_Read(&byte, 1)) //Read one byte at a  time
        {
            --count;
            *linePtrBuffer++ = byte;
            numberOfBytes++;
            if (numberOfBytes >= 2) //if I have read at least 2 bytes
            {
                //check if last 2 character are \n\r
                if ((*(linePtrBuffer - 1) == 0x0A)
                        && (*(linePtrBuffer - 2) == 0x0d)) {
                    //found CRLF
                    *linePtrBuffer = 0; // Terminate line with NULL
                    linePtrBuffer = NULL;
                    uint16_t bytes;
                    bytes = numberOfBytes;
                    numberOfBytes = 0;
                    return bytes;
                }
            }
        };
    } while (count);

    return 0;
}
/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */

/** 
  @Function
    int ExampleLocalFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Description
    Full description, explaining the purpose and usage of the function.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

  @Precondition
    List and describe any required preconditions. If there are no preconditions,
    enter "None."

  @Parameters
    @param param1 Describe the first parameter to the function.
    
    @param param2 Describe the second parameter to the function.

  @Returns
    List (if feasible) and describe the return values of the function.
    <ul>
      <li>1   Indicates an error occurred
      <li>0   Indicates an error did not occur
    </ul>

  @Remarks
    Describe any special behavior not described above.
    <p>
    Any additional remarks.

  @Example
    @code
    if(ExampleFunctionName(1, 2) == 0)
    {
        return 3;
    }
 */


void WFI32_task(void)
{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     {
    
    switch(app_WFI32_Data.state)
    {
        case WFIE32_INIT:
        {
            bool appInitialized = true;
            SERCOM0_USART_Write((uint8_t*)"WiFi init\r\n",strlen("WiFi init\r\n"));
            gRDY = false;
            gOK = false;
            if (appInitialized) 
            {
                app_WFI32_Data.state = WFIE32_RST;
                stationCmdTblIndex = 1;
                mqttCmdTblIndex = 1; 
            }
        }
        break;
        case WFIE32_RST:
        {
            static int rst_count=0;
            if(gOK)
            {
                SERCOM0_USART_Write((uint8_t*)"Reset Complete\r\n",strlen("Reset Complete\r\n"));
                app_WFI32_Data.state=WFIE32_ECHO;
                gOK=false;
            }
            else
            {
                if(rst_count<2)
                {
                    DRV_USART_WriteBufferAdd(appUart.usartHandle, (void *)"AT+RST\r\n",strlen("AT+RST\r\n"), &appUart.writeBufferHandle);
                    rst_count++;
                }
            }
        }
        break;
        case WFIE32_ECHO:
        {
            static int rst_count3=0;
            char wrbuffer[100];
            if(gATE && gOK)
            {
                app_WFI32_Data.state=WFI32_STATION_MODE_CONFIG;
                gOK=false;
                sprintf(wrbuffer,Station_mode[0].cmd,Station_mode[0].value);
                DRV_USART_WriteBufferAdd(appUart.usartHandle,(uint8_t*)wrbuffer,strlen(wrbuffer), &appUart.writeBufferHandle);
                SERCOM0_USART_Write((uint8_t*)wrbuffer,strlen(wrbuffer));
            }
            else
            {
                if(rst_count3<2)
                {
                    SERCOM1_USART_Write((uint8_t*)"ATE0\r\n",strlen("ATE0\r\n"));
                    rst_count3++;
                }
            }
        }
        break;
        
        case WFI32_STATION_MODE_CONFIG:
        {
            if(gOK)
            {
                if(stationCmdTblIndex<6)
                {
                    char wrbuffer[100];
                    sprintf(wrbuffer,Station_mode[stationCmdTblIndex].cmd,Station_mode[stationCmdTblIndex].value);
                    DRV_USART_WriteBufferAdd(appUart.usartHandle,(uint8_t*)wrbuffer,strlen(wrbuffer), &appUart.writeBufferHandle);
                    SERCOM0_USART_Write((uint8_t*)wrbuffer,strlen(wrbuffer));
                    stationCmdTblIndex++;
                }
                else
                {
                    app_WFI32_Data.state = WFIE32_STA_CONNECT;
                }
                gOK=false;
                gWSTAAIP = gWAPAIP = false;
                
            }
            else
            {
                app_WFI32_Data.state = WFI32_STATION_MODE_CONFIG;
            }
        }
        break;
        case WFIE32_STA_CONNECT:
        {
            vTaskDelay(5000);
            DRV_USART_WriteBufferAdd(appUart.usartHandle,(uint8_t*)"AT+WSTA=1\r\n",strlen("AT+WSTA=1\r\n"), &appUart.writeBufferHandle);
            SERCOM0_USART_Write((uint8_t*)"Connecting to wifi\r\n",strlen("Connecting to wifi\r\n"));
            app_WFI32_Data.state =WFI32_WAIT_FOR_IP;
        }
        break;
        
        case WFI32_WAIT_FOR_IP:
        {
            if(gOK && (gWSTAAIP || gWAPAIP))
            {
                SERCOM0_USART_Write((uint8_t*)"MQTT Config\r\n",strlen("MQTT Config\r\n"));
                app_WFI32_Data.state=WFI32_MQTT_CONFIG;
                DRV_USART_WriteBufferAdd(appUart.usartHandle,(uint8_t*)"AT+MQTTC=1,\"test.mosquitto.org\"\r\n",strlen("AT+MQTTC=1,\"test.mosquitto.org\"\r\n"), &appUart.writeBufferHandle);
                SERCOM0_USART_Write((uint8_t*)"AT+MQTTC=1,\"test.mosquitto.org\"\r\n",strlen("AT+MQTTC=1,\"test.mosquitto.org\"\r\n"));
                gOK=false;
            }
        }
        break;
        
        case WFI32_MQTT_CONFIG:
        {
            if(gOK)
            {
                if(mqttCmdTblIndex<5)
                {
                    char wrbuffer[100];
                    sprintf(wrbuffer,mqttCmdTbl[mqttCmdTblIndex].cmd,Station_mode[mqttCmdTblIndex].value);
                    DRV_USART_WriteBufferAdd(appUart.usartHandle,(uint8_t*)wrbuffer,strlen(wrbuffer), &appUart.writeBufferHandle);
                    SERCOM0_USART_Write((uint8_t*)wrbuffer,strlen(wrbuffer));
                    mqttCmdTblIndex++;
                }
                else
                {
                    app_WFI32_Data.state = WFI32_MQTT_CONNECT;
                    SERCOM0_USART_Write((uint8_t*)"Connecting\r\n",strlen("Connecting\r\n"));
                }
                gOK= false;
                gMQTTCONN = false;
            }
            else
            {
                app_WFI32_Data.state = WFI32_MQTT_CONFIG;
            }
        }
        break;
        
        case WFI32_MQTT_CONNECT:
        {
            vTaskDelay(4000);
            DRV_USART_WriteBufferAdd(appUart.usartHandle,(uint8_t*)"AT+MQTTCONN=1\r\n",strlen("AT+MQTTCONN=1\r\n"), &appUart.writeBufferHandle);
            app_WFI32_Data.state =WFI32_WAIT_FOR_MQTT;
            gMQTTCONN=false;gOK=false;
        }
        break;
        
        case WFI32_WAIT_FOR_MQTT:
        {
            if(gOK)
            {
                SERCOM0_USART_Write((uint8_t*)"MQTT Connected\r\n",strlen("MQTT Connected\r\n"));
                vTaskDelay(5000);
                DRV_USART_WriteBufferAdd(appUart.usartHandle,(uint8_t*)"AT+MQTTSUB=\"MCHP/WSG/ZIGBEE\",0\r\n",strlen("AT+MQTTSUB=\"MCHP/WSG/ZIGBEE\",0\r\n"), &appUart.writeBufferHandle);
                app_WFI32_Data.state = WFI32_SUBSCRIBE;
            }
        }
        break;
        
        case WFI32_SUBSCRIBE:
        {
            if(gMQTTSUB)
            {
                gMQTTSUB=false;
                app_WFI32_Data.state = WFI32_SUBSCRIBE2;
                DRV_USART_WriteBufferAdd(appUart.usartHandle,(uint8_t*)"AT+MQTTSUB=\"MCHP/WSG/ZGBMQTT\",0\r\n",strlen("AT+MQTTSUB=\"MCHP/WSG/ZGBMQTT\",0\r\n"), &appUart.writeBufferHandle);
                SERCOM0_USART_Write((uint8_t*)"Subscribing to topic 1\r\n",strlen("Subscribing to topic 1\r\n"));
            }
        }
        break;
        
        case WFI32_SUBSCRIBE2:
        {
            if(gMQTTSUB)
            {
                wfi32_initialized=1;
                gOK=true;
                app_WFI32_Data.state = WFI32_IDLE;
                SERCOM0_USART_Write((uint8_t*)"IDLE\r\n",strlen("IDLE\r\n"));
            }
        }
        break;
        
        case WFI32_IDLE:
        {
            
            //error handle
        }
        break;
    }
}
}












/* *****************************************************************************
 End of File
 */
