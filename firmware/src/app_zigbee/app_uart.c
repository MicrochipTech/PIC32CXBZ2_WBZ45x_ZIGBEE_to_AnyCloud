/*******************************************************************************
  Application UART Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app_uart.c

  Summary:
    This file contains the Application UART functions for this project.

  Description:
    This file contains the Application UART functions for this project.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END
// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <definitions.h>
#include <stdio.h>
#include <stdarg.h>
#include <xc.h>

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************


// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */


// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************
APP_UART_DATA appUart;
extern int wfi32_initialized;
extern bool gOK;
static void APP_USARTBufferEventHandler(
    DRV_USART_BUFFER_EVENT bufferEvent,
    DRV_USART_BUFFER_HANDLE bufferHandle,
    uintptr_t context);
extern void consoleRx(uint8_t data);
void APP_UartHandler(void);
// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************
void APP_UartInit(void)
{
    appUart.writeInProgressStatus = false;
    appUart.readInProgressStatus  = false;
    appUart.dataRead              = false;
    appUart.usartHandle           = DRV_HANDLE_INVALID;
    appUart.readBufferHandle      = DRV_USART_BUFFER_HANDLE_INVALID;
    appUart.writeBufferHandle     = DRV_USART_BUFFER_HANDLE_INVALID;
    appUart.txPointOfRead         = 0;
    appUart.txPointOfWrite        = 0;
 
    appUart.usartHandle = DRV_USART_Open(DRV_USART_INDEX_0, DRV_IO_INTENT_READWRITE);
    if (appUart.usartHandle != DRV_HANDLE_INVALID)
    {
        DRV_USART_BufferEventHandlerSet(appUart.usartHandle, APP_USARTBufferEventHandler, 0);
    }
    if((appUart.readInProgressStatus == false) && (appUart.dataRead == false))
    {
    #if (APP_USE_ISD_CONSOLE_TUNNELING != 1)

         DRV_USART_ReadBufferAdd(appUart.usartHandle, (void*)&appUart.readBuffer[0], APP_RX_READ_LENGTH, &appUart.readBufferHandle);
         if (appUart.readBufferHandle != DRV_USART_BUFFER_HANDLE_INVALID)
    #endif
         {
             appUart.readInProgressStatus = true;
         }
    }
}

static void APP_USARTBufferEventHandler(
    DRV_USART_BUFFER_EVENT bufferEvent,
    DRV_USART_BUFFER_HANDLE bufferHandle,
    uintptr_t context
)
{
    switch(bufferEvent)
    {
        case DRV_USART_BUFFER_EVENT_COMPLETE:
            if (bufferHandle == appUart.writeBufferHandle)
            {
                appUart.writeInProgressStatus = false;
            }
            if (bufferHandle == appUart.readBufferHandle)
            {
                appUart.readInProgressStatus = false;
                appUart.dataRead = true;
            }
            APP_UartHandler();
            break;

        case DRV_USART_BUFFER_EVENT_ERROR:
            break;

        default:
            break;
    }
}

bool APP_IsUartReadyToSleep(void)
{
    if((appUart.readInProgressStatus == false) && (appUart.writeInProgressStatus == false))
    {
        return true;
    }
    return false;
}

void APP_UartHandler(void)
{
    if (appUart.dataRead)
    {
        appUart.dataRead = false;

        for (uint8_t i = 0; i < APP_RX_READ_LENGTH; i++)
        {
            consoleRx(appUart.readBuffer[i]);
        }
    }
    if((appUart.writeInProgressStatus == false) && (appUart.txPointOfWrite != appUart.txPointOfRead))
    {
        uint16_t poW;
        uint16_t poR;
        uint8_t tempWriteCount = 0;
        char buffer[10000];
        char buffer1[10000];
        poW = appUart.txPointOfWrite;
        poR = appUart.txPointOfRead;
        while (poW != poR)
        {
          if((appUart.writeBuffer[poR]!='\n') && (appUart.writeBuffer[poR]!='\r'))
          {
            appUart.writeBufferToDrvUsart[tempWriteCount++] = appUart.writeBuffer[poR++];
          }
          else
          {
              poR++;
              appUart.writeBufferToDrvUsart[tempWriteCount++] ='\0';
          }
            if (poR == APP_TX_BUFFER_LENGTH)
              poR = 0;

            appUart.txPointOfRead = poR;
        }
        strncpy(buffer,appUart.writeBufferToDrvUsart,tempWriteCount);
        sprintf(buffer1,"AT+MQTTPUB=0,0,0,\"MCHP/WSG/ZIGBEE\",\"%s\"\r\n",buffer);
        for(int i=0;i<500;i++)
            asm("nop");
        if(wfi32_initialized==1)
        {
            gOK=false;
            DRV_USART_WriteBufferAdd(appUart.usartHandle, buffer1, strlen(buffer1), &appUart.writeBufferHandle);
        }
        if (appUart.writeBufferHandle != DRV_USART_BUFFER_HANDLE_INVALID)
        {
            appUart.writeInProgressStatus = true;
        }
    } 
    if((appUart.readInProgressStatus == false) && (appUart.dataRead == false))
    {
         DRV_USART_ReadBufferAdd(appUart.usartHandle, (void*)&appUart.readBuffer[0], APP_RX_READ_LENGTH, &appUart.readBufferHandle);
         if (appUart.readBufferHandle != DRV_USART_BUFFER_HANDLE_INVALID)
         {
             appUart.readInProgressStatus = true;
         }
    }    
}

void APP_UartWriteBuffer(void* buffer, uint8_t length)
{    
    uint8_t poW;
    uint8_t poR;
    uint8_t old;
    uint8_t writeCount = 0;
    poW = appUart.txPointOfWrite;
    poR = appUart.txPointOfRead;

    while (writeCount < length)
    {
      old = poW;

      if (++poW == APP_TX_BUFFER_LENGTH)
        poW = 0;

      if (poW == poR)
      { // Buffer full.
        poW = old;
        break;
      }

      appUart.writeBuffer[old] = ((uint8_t*)buffer)[writeCount++];
    }
    appUart.txPointOfWrite = poW;
}

size_t fwrite(const void *ptr, size_t size, size_t nelem, FILE *stream)
{
    APP_UartWriteBuffer((void*)ptr, nelem);
    APP_UartWriteBuffer("\n", 1);
    APP_UartHandler(); 
}

int printf(const char *format, ...)
{
    int ret;
    char s[APP_TX_BUFFER_LENGTH];
    va_list ap;
    va_start(ap, format);
    ret = vsprintf(s, format, ap);
    va_end(ap);
    APP_UartWriteBuffer((void*)s, ret);
    APP_UartHandler(); 
}


void APP_UartWriteBufferSend(void* buffer, uint8_t length)
{
    APP_UartWriteBuffer((void*)buffer, length);
    APP_UartHandler();
}
