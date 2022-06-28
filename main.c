/******************************************************************************
* File Name:   main.c
*
* Description: This is the source code for the PMG1 MCU Cryptography: True
* Random Number Generation Example for ModusToolbox.
*
* Related Document: See README.md
*
*
*******************************************************************************
* Copyright 2019-2021, Cypress Semiconductor Corporation (an Infineon company) or
* an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
*
* This software, including source code, documentation and related
* materials ("Software") is owned by Cypress Semiconductor Corporation
* or one of its affiliates ("Cypress") and is protected by and subject to
* worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
* If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
* non-transferable license to copy, modify, and compile the Software
* source code solely for use in connection with Cypress's
* integrated circuit products.  Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
* reserves the right to make changes to the Software without notice. Cypress
* does not assume any liability arising out of the application or use of the
* Software or any product or circuit described in the Software. Cypress does
* not authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*******************************************************************************/

#include "cy_pdl.h"
#include "cybsp.h"
#include "cy_cryptolite.h"
#include "stdio.h"
/*******************************************************************************
* Macros
*******************************************************************************/
/* Macro for the maximum value of the random number generated in bits */
#define ASCII_RETURN_CARRIAGE           (0x0D)

/*******************************************************************************
* Function Prototypes
********************************************************************************/
void generate_password();

/*******************************************************************************
* Global Variable
*******************************************************************************/
cy_stc_scb_uart_context_t CYBSP_UART_context;

/*******************************************************************************
* Function Name: main
********************************************************************************
* Summary:
* This is the main function for CM4 CPU. It configures the Crypto block to
* generate random number and convert it to a alpha-numeric password. A new
* password is generated every time  the user presses the Enter key.
*
* Parameters:
*  void
*
* Return:
*  int
*
*******************************************************************************/
int main(void)
{
    cy_rslt_t result;

    /* Initialize the device and board peripherals */
    result = cybsp_init();

    /* Board init failed. Stop program execution */
    if (result != CY_RSLT_SUCCESS)
    {
        CY_ASSERT(0);
    }

    /* Configure and enable the UART peripheral */
    Cy_SCB_UART_Init(CYBSP_UART_HW, &CYBSP_UART_config, &CYBSP_UART_context);

    Cy_SCB_UART_Enable(CYBSP_UART_HW);

    /* Enable global interrupts */
    __enable_irq();

    /* Transmit Header to the Terminal */
    Cy_SCB_UART_PutString(CYBSP_UART_HW, "\x1b[2J\x1b[;H");
    Cy_SCB_UART_PutString(CYBSP_UART_HW, "***********************************************************\r\n");
    Cy_SCB_UART_PutString(CYBSP_UART_HW, "PMG1 MCU Cryptography: TRNG\r\n");
    Cy_SCB_UART_PutString(CYBSP_UART_HW, "***********************************************************\r\n\n");
    Cy_SCB_UART_PutString(CYBSP_UART_HW, "Press the Enter key to generate new One-Time Password (OTP):\r\n\n");

    for(;;)
        {

            /* Check if "Enter" key is received from the terminal; */
            if (Cy_SCB_UART_Get(CYBSP_UART_HW) == ASCII_RETURN_CARRIAGE)
            {
                /* Generate a password of 8 characters in length */
                generate_password();
            }
        }
    }



/*******************************************************************************
* Function Name: generate_password
********************************************************************************
* Summary: This function generates a 8 character long password
*
* Parameters:
*  None
*
* Return
*  void
*
*******************************************************************************/
void generate_password()
{
    char str[20]={0};
    uint32_t rndNum = 0;
    cy_en_cryptolite_status_t cryptoStatus;

    /* Generate a true random number in the specified range */
    cryptoStatus = Cy_Cryptolite_Trng(CRYPTOLITE,  /* Base address of the Crypto block registers */
                                      &rndNum);    /* Pointer to generated random number */

    if (cryptoStatus == CY_CRYPTOLITE_SUCCESS)
    {
        /* Transmit OTP to the Terminal */
        sprintf(str,"OTP is: 0x%X",(int)rndNum);
        Cy_SCB_UART_PutArrayBlocking(CYBSP_UART_HW,str,18);
        Cy_SCB_UART_PutString(CYBSP_UART_HW, "\r\n\n***********************************************************\r\n");
        Cy_SCB_UART_PutString(CYBSP_UART_HW, "\r\nPress the Enter key to generate new One-Time Password (OTP):\r\n\n");
    }
}


/* [] END OF FILE */
