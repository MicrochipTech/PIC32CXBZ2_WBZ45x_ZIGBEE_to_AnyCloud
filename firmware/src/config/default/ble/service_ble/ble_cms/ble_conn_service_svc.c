/*******************************************************************************
  BLE Conn_serv Service Source File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_conn_serv_svc.c

  Summary:
    This file contains the BLE Conn_serv Service functions for application user.

  Description:
    This file contains the BLE Conn_serv Service functions for application user.
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
#include <stddef.h>
#include "gatt.h"
#include "ble_util/byte_stream.h"
#include "ble_cms/ble_conn_service_svc.h"

#include "osal/osal_freertos.h"

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/* Little Endian. */
#define UUID_CONN_SERV_PRIMARY_SVC_LE         0x1b, 0xc5, 0xd5, 0xa5, 0x02, 0x00, 0x89, 0x86, 0xe4, 0x11, 0x29, 0xd2, 0x01, 0x00, 0x88, 0x77

#define UUID_CONN_SERV_CHARACTERISTIC_0_LE         0x1b, 0xc5, 0xd5, 0xa5, 0x02, 0x00, 0x89, 0x86, 0xe4, 0x11, 0x29, 0xd2, 0x02, 0x00, 0x88, 0x77
#define UUID_CONN_SERV_CHARACTERISTIC_1_LE         0x1b, 0xc5, 0xd5, 0xa5, 0x02, 0x00, 0x89, 0x86, 0xe4, 0x11, 0x29, 0xd2, 0x03, 0x00, 0x88, 0x77

// *****************************************************************************
// *****************************************************************************
// Section: Local Variables
// *****************************************************************************
// *****************************************************************************

/* Primary Service Declaration */
static const uint8_t s_conn_servSvcUuid[] = {UUID_CONN_SERV_PRIMARY_SVC_LE};
static const uint16_t s_conn_servSvcUuidLen = sizeof(s_conn_servSvcUuid);

/* Conn_serv Characteristic 0 Characteristic */
static const uint8_t s_conn_servChar0[] = {ATT_PROP_READ|ATT_PROP_WRITE_CMD|ATT_PROP_WRITE_REQ|ATT_PROP_NOTIFY|ATT_PROP_INDICATE, UINT16_TO_BYTES(CONN_SERV_HDL_CHARVAL_0), UUID_CONN_SERV_CHARACTERISTIC_0_LE};
static const uint16_t s_conn_servChar0Len = sizeof(s_conn_servChar0);

/* Conn_serv Characteristic 0 Characteristic Value */
static const uint8_t s_conn_servUuidChar0[] = {UUID_CONN_SERV_CHARACTERISTIC_0_LE};
static uint8_t s_conn_servChar0Val[1] = {0x01};
static uint16_t s_conn_servChar0ValLen = sizeof(s_conn_servChar0Val);

/* Conn_serv Characteristic 0 Client Characteristic Configuration Descriptor */
static uint8_t s_conn_servCccChar0[] = {UINT16_TO_BYTES(0x0000)};
static const uint16_t s_conn_servCccChar0Len = sizeof(s_conn_servCccChar0);

/* Conn_serv Characteristic 1 Characteristic */
static const uint8_t s_conn_servChar1[] = {ATT_PROP_WRITE_CMD|ATT_PROP_WRITE_REQ, UINT16_TO_BYTES(CONN_SERV_HDL_CHARVAL_1), UUID_CONN_SERV_CHARACTERISTIC_1_LE};
static const uint16_t s_conn_servChar1Len = sizeof(s_conn_servChar1);

/* Conn_serv Characteristic 1 Characteristic Value */
static const uint8_t s_conn_servUuidChar1[] = {UUID_CONN_SERV_CHARACTERISTIC_1_LE};
static uint8_t s_conn_servChar1Val[247] = {};
static uint16_t s_conn_servChar1ValLen = sizeof(s_conn_servChar1Val);

/* Attribute list for Conn_serv service */
static GATTS_Attribute_T s_conn_servList[] = {
    /* Service Declaration */
    {
        (uint8_t *) g_gattUuidPrimSvc,
        (uint8_t *) s_conn_servSvcUuid,
        (uint16_t *) & s_conn_servSvcUuidLen,
        sizeof (s_conn_servSvcUuid),
        0,
        PERMISSION_READ
    },
    /* Characteristic 0 Declaration */
    {
        (uint8_t *) g_gattUuidChar,
        (uint8_t *) s_conn_servChar0,
        (uint16_t *) & s_conn_servChar0Len,
        sizeof (s_conn_servChar0),
        0,
        PERMISSION_READ
    },
    /* Characteristic 0 Value */
    {
        (uint8_t *) s_conn_servUuidChar0,
        (uint8_t *) s_conn_servChar0Val,
        (uint16_t *) & s_conn_servChar0ValLen,
        sizeof(s_conn_servChar0Val),
        SETTING_UUID_16,
        PERMISSION_READ|PERMISSION_READ_AUTHEN|PERMISSION_READ_AUTHEN_SC|PERMISSION_READ_ENC|PERMISSION_WRITE|PERMISSION_WRITE_AUTHEN|PERMISSION_WRITE_AUTHEN_SC|PERMISSION_WRITE_ENC
    },
    /* Client Characteristic Configuration Descriptor */
    {
        (uint8_t *) g_descUuidCcc,
        (uint8_t *) s_conn_servCccChar0,
        (uint16_t *) & s_conn_servCccChar0Len,
        sizeof (s_conn_servCccChar0),
        SETTING_CCCD,
        PERMISSION_READ|PERMISSION_READ_AUTHEN|PERMISSION_READ_AUTHEN_SC|PERMISSION_READ_ENC|PERMISSION_WRITE|PERMISSION_WRITE_AUTHEN|PERMISSION_WRITE_AUTHEN_SC|PERMISSION_WRITE_ENC
    },
    /* Characteristic 1 Declaration */
    {
        (uint8_t *) g_gattUuidChar,
        (uint8_t *) s_conn_servChar1,
        (uint16_t *) & s_conn_servChar1Len,
        sizeof (s_conn_servChar1),
        0,
        PERMISSION_READ
    },
    /* Characteristic 1 Value */
    {
        (uint8_t *) s_conn_servUuidChar1,
        (uint8_t *) s_conn_servChar1Val,
        (uint16_t *) & s_conn_servChar1ValLen,
        sizeof(s_conn_servChar1Val),
        SETTING_VARIABLE_LEN|SETTING_UUID_16,
        PERMISSION_WRITE|PERMISSION_WRITE_AUTHEN|PERMISSION_WRITE_AUTHEN_SC|PERMISSION_WRITE_ENC
    },
};

static const GATTS_CccdSetting_T s_conn_servCccdSetting[] = 
{
    {CONN_SERV_HDL_CCCD_0, NOTIFICATION|INDICATION},
};

/* Conn_serv Service structure */
static GATTS_Service_T s_conn_servSvc = 
{
    NULL,
    (GATTS_Attribute_T *) s_conn_servList,
    (GATTS_CccdSetting_T const *)s_conn_servCccdSetting,
    CONN_SERV_START_HDL,
    CONN_SERV_END_HDL,
    1
};

// *****************************************************************************
// *****************************************************************************
// Section: Functions
// *****************************************************************************
// *****************************************************************************

uint16_t BLE_CONN_SERV_Add()
{
    return GATTS_AddService(&s_conn_servSvc, (CONN_SERV_END_HDL - CONN_SERV_START_HDL + 1));
}


void APP_ConnService_SendNotification(uint16_t connHandle, uint8_t char_val )
{
    GATTS_HandleValueParams_T *p_hvParams;

    p_hvParams = OSAL_Malloc(sizeof(GATTS_HandleValueParams_T));
    if (p_hvParams != NULL)
    {
        s_conn_servChar0Val[0]= char_val;
        p_hvParams->charHandle = CONN_SERV_HDL_CHARVAL_0;
        p_hvParams->charLength = 1;
        p_hvParams->charValue[0] = char_val;
        //memcpy(&p_hvParams->charValue[1], p_commandPayload, commandLength);
        p_hvParams->sendType = ATT_HANDLE_VALUE_NTF;
        GATTS_SendHandleValue(connHandle, p_hvParams);
        OSAL_Free(p_hvParams);
    }
    else
    {
        // Do nothing
    }
}

