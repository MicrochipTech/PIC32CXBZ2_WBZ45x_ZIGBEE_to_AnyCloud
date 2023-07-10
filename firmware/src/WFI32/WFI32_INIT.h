/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _WFI32_INIT_H    /* Guard against multiple inclusion */
#define _WFI32_INIT_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "configuration.h"
#include "osal/osal_freertos_extend.h"
/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    /* ************************************************************************** */
    /** Descriptive Constant Name

      @Summary
        Brief one-line summary of the constant.
    
      @Description
        Full description, explaining the purpose and usage of the constant.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
      @Remarks
        Any additional remarks
     */
#define EXAMPLE_CONSTANT 0
typedef enum
{
    /* Application's state machine's initial state. */
    WFIE32_INIT,
    WFIE32_RST,
    WFIE32_ECHO,
    WFI32_STATION_MODE_CONFIG,
    WFI32_MQTT_CONFIG,
    WFI32_MQTT_CONNECT,
    WFI32_WAIT_FOR_IP,
    WFI32_WAIT_FOR_MQTT,
    WFI32_SUBSCRIBE,
    WFI32_SUBSCRIBE2,
    WFI32_IDLE,
    WFIE32_STA_CONNECT
    /* TODO: Define states used by the application state machine. */

} APP_WFI32_STATES;

typedef struct
{
    /* The application's current state */
    APP_WFI32_STATES state;

} APP_WFI32_DATA;

APP_WFI32_DATA app_WFI32_Data;






typedef struct{
    int index;
    const char *cmd;
    const char *value;
}command_structure;


typedef void (*callback)(uint8_t u8MsgType, void **pvMsg);

void atCmdRespHandler(void);
void *wifi_cb(void);
void *mqtt_cb(void);


typedef struct {
    const char *atCmdResp; //  AT command RN-RIO2 response string to search for
    bool *flag; //  Associated flag
    callback func; //  CallBack function
//    const uint8_t event; //  Event associated with Callback
//    char **param; //  AT command parameter

} ATCMD_RESPONSE; // a simple command descriptor


    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    // *****************************************************************************

    /** Descriptive Data Type Name

      @Summary
        Brief one-line summary of the data type.
    
      @Description
        Full description, explaining the purpose and usage of the data type.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

      @Remarks
        Any additional remarks
        <p>
        Describe enumeration elements and structure and union members above each 
        element or member.
     */
    typedef struct _example_struct_t {
        /* Describe structure member. */
        int some_number;

        /* Describe structure member. */
        bool some_flag;

    } example_struct_t;


    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */

    // *****************************************************************************
    /**
      @Function
        int ExampleFunctionName ( int param1, int param2 ) 

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

void WFI32_task(void);
uint16_t read_data(char *dataBuffer);
void parseRIO2RxMessage(char *buffer);
    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
