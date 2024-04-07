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



// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdio.h>
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <string.h>
#include <malloc.h>
#include <inttypes.h>   // required to print out pointers using PRIXPTR macro
#include "definitions.h"                // SYS function prototypes
#include "projGlobalDefinitions.h" // lab test structs
#include "testFuncs.h" // lab test structs
#include "printFuncs.h"  // lab print funcs

static uint8_t txBuffer[MAX_PRINT_LEN] = {0};


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

void printArray1(char *desc, 
        const uint32_t* a0, 
        volatile bool *txCompletePtr)
{
snprintf((char*)txBuffer, MAX_PRINT_LEN,
            "========= %s\r\n"
            " 0: 0x%08lx\r\n"
            " 1: 0x%08lx\r\n"
            " 2: 0x%08lx\r\n"
            " 3: 0x%08lx\r\n"
            " 4: 0x%08lx\r\n"
            " 5: 0x%08lx\r\n"
            " 6: 0x%08lx\r\n"
            " 7: 0x%08lx\r\n"
            " 8: 0x%08lx\r\n"
            " 9: 0x%08lx\r\n"
            "10: 0x%08lx\r\n"
            "\r\n",
            desc,
            a0[0],
            a0[1],
            a0[2],
            a0[3],
            a0[4],
            a0[5],
            a0[6],
            a0[7],
            a0[8],
            a0[9],
            a0[10]);
    printAndWait((char*)txBuffer,txCompletePtr);
}


void printArray3(int32_t testNum,
        char *desc, 
        const uint32_t* a, 
        const uint32_t* b,
        const uint32_t* c,
        volatile bool *txCompletePtr)
{
    char *diff[MAX_SORT_ARRAY_SIZE];
    char * match = "";
    char * noMatch = "***";
    
    for (int i = 0; i < MAX_SORT_ARRAY_SIZE; ++i)
    {
        if(b[i] == c[i])
        {
            diff[i] = match;
        }
        else
        {
            diff[i] = noMatch;
        }
    }
    snprintf((char*)txBuffer, MAX_PRINT_LEN,
            "========= Test number: %ld: Array Details\r\n"
            "========= %s\r\n"
            " 0: 0x%08lx;  0x%08lx; 0x%08lx  %s\r\n"
            " 1: 0x%08lx;  0x%08lx; 0x%08lx  %s\r\n"
            " 2: 0x%08lx;  0x%08lx; 0x%08lx  %s\r\n"
            " 3: 0x%08lx;  0x%08lx; 0x%08lx  %s\r\n"
            " 4: 0x%08lx;  0x%08lx; 0x%08lx  %s\r\n"
            " 5: 0x%08lx;  0x%08lx; 0x%08lx  %s\r\n"
            " 6: 0x%08lx;  0x%08lx; 0x%08lx  %s\r\n"
            " 7: 0x%08lx;  0x%08lx; 0x%08lx  %s\r\n"
            " 8: 0x%08lx;  0x%08lx; 0x%08lx  %s\r\n"
            " 9: 0x%08lx;  0x%08lx; 0x%08lx  %s\r\n"
            "10: 0x%08lx;  0x%08lx; 0x%08lx  %s\r\n"
           "\r\n",
            testNum,
            desc,
            a[0],b[0],c[0],diff[0],
            a[1],b[1],c[1],diff[1],
            a[2],b[2],c[2],diff[2],
            a[3],b[3],c[3],diff[3],
            a[4],b[4],c[4],diff[4],
            a[5],b[5],c[5],diff[5],
            a[6],b[6],c[6],diff[6],
            a[7],b[7],c[7],diff[7],
            a[8],b[8],c[8],diff[8],
            a[9],b[9],c[9],diff[9],
            a[10],b[10],c[10],diff[10]);
    
    printAndWait((char*)txBuffer,txCompletePtr);
}



/** 
  @Function
    int ExampleInterfaceFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Remarks
    Refer to the example_file.h interface header for function usage details.
 */
void printAndWait(char *txBuffer, volatile bool *txCompletePtr)
{
    *txCompletePtr = false;
    
#if USING_HW

    DMAC_ChannelTransfer(DMAC_CHANNEL_0, txBuffer, \
        (const void *)&(SERCOM5_REGS->USART_INT.SERCOM_DATA), \
        strlen((const char*)txBuffer));
    // spin here, waiting for timer and UART to complete
    while (*txCompletePtr == false); // wait for print to finish
    /* reset it for the next print */
    
#endif
    
    *txCompletePtr = false;


}

