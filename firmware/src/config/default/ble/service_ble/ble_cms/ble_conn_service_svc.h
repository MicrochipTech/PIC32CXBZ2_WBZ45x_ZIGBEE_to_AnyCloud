/*******************************************************************************
  BLE Conn_serv Service Header File

  Company:
    Microchip Technology Inc.

  File Name:
    ble_conn_serv_svc.h

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

/**
 * @addtogroup BLE_CONN_SERV BLE CONN_SERV
 * @{
 * @brief Header file for the BLE Conn_serv Service.
 * @note Definitions and prototypes for the BLE Conn_serv Service stack layer application programming interface.
 */
#ifndef BLE_CONN_SERV_H
#define BLE_CONN_SERV_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>

// *****************************************************************************
// *****************************************************************************
// Section: Macros
// *****************************************************************************
// *****************************************************************************
/**@defgroup BLE_CONN_SERV_ASSIGN_HANDLE BLE_CONN_SERV_ASSIGN_HANDLE
 * @brief Assigned attribute handles of BLE Conn_serv Service.
 * @{ */
#define CONN_SERV_START_HDL                               0x8000                                   /**< The start attribute handle of conn_serv service. */
/** @} */

/**@brief Definition of BLE Conn_serv Service attribute handle */
typedef enum BLE_CONN_SERV_AttributeHandle_T
{
    CONN_SERV_HDL_SVC = CONN_SERV_START_HDL,           /**< Handle of Primary Service. */
    CONN_SERV_HDL_CHAR_0,                            /**< Handle of characteristic 0. */
    CONN_SERV_HDL_CHARVAL_0,                         /**< Handle of characteristic 0 value. */
    CONN_SERV_HDL_CCCD_0,                            /**< Handle of characteristic 0 CCCD . */
    CONN_SERV_HDL_CHAR_1,                            /**< Handle of characteristic 1. */
    CONN_SERV_HDL_CHARVAL_1,                         /**< Handle of characteristic 1 value. */
}BLE_CONN_SERV_AttributeHandle_T;

/**@defgroup BLE_CONN_SERV_ASSIGN_HANDLE BLE_CONN_SERV_ASSIGN_HANDLE
 * @brief Assigned attribute handles of BLE Conn_serv Service.
 * @{ */
#define CONN_SERV_END_HDL                                 (CONN_SERV_HDL_CHARVAL_1)         /**< The end attribute handle of conn_serv service. */
/** @} */


// *****************************************************************************
// *****************************************************************************
// Section: Function Prototypes
// *****************************************************************************
// *****************************************************************************
/**
 *@brief Initialize BLE Conn_serv Service callback function.
 *
 *
 *@return MBA_RES_SUCCESS                    Successfully register BLE Conn_serv service.
 *@return MBA_RES_NO_RESOURCE                Fail to register service.
 *
 */
uint16_t BLE_CONN_SERV_Add();


#endif

/**
  @}
 */
