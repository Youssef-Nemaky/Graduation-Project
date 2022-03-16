 /******************************************************************************
 *
 * Module: Main Source File for The Control Block
 *
 * File Name: Control_ECU.c
 *
 * Description: Source File for Control ECU Block Functions
 *
 * Created on: Dec 9, 2021
 *
 * Authors: Mourad Eldin Nash'at , Abdelaziz Abu Zaid
 *
 *******************************************************************************/

/*******************************************************************************
 *                                  Includes                                   *
 *******************************************************************************/
#include "Control_ECU.h"


/*******************************************************************************
 *                              Global Variables                               *
 *******************************************************************************/

/* Global Variable of type System_State used to block The System at a certain point for an amount of time */
System_State state = BLOCKED;

/* Global Variable that holds The Value of Times The Passcode is entered in case it was entered wrong */
uint8 passWrongAttempts = 0;

uint8 rfidWrongAttempts = 0;

uint8 faceWrongAttempts = 0;

uint8 g_first_time = 0;

/*******************************************************************************
 *                       	  Functions Definitions                            *
 *******************************************************************************/


/*******************************************************************************************************
 * [Name]: Drivers_Init
 * [Parameters]: void (none)
 * [Return]: void (none)
 * [Description]: The Function responsible for Initializing of (MCAL) Drivers like UART & I2C and (HAL)
 * Drivers like GPS/GSM Modules and Enabling Global Interrupt (I-Bit) in Control Block.
 ********************************************************************************************************/
void Drivers_Init(void)
{
    Port_Init(&Port_Configuration);
    Dio_Init(&Dio_Configuration);
    Uart_Init(&Uart_Configuration);
    I2c_Init(&I2c_Confiuration);
}


int main(void){
    Drivers_Init();
    Delay_ms(1000); /* give time for the HMI block to finish initialization */

    /* Read the EEPROM address for first time use */
    EEPROM_readByte(FIRST_TIME_ADDRESS, &g_first_time);

    if(g_first_time == 0x00){
        systemSetup();
    }
    while(1){

    }
}



void systemSetup(void){
    Uart_SendByte(HMI_BLOCK_UART, FIRST_TIME_CMD);
    passwordSetup();
    rfidSetup();
    faceSetup();
    
    /* Send setup complete command to HMI */
    Uart_SendByte(HMI_BLOCK_UART, SETUP_COMPLETE_CMD);

    /* Change first time to 0x00 to indicate that this is not the first time when reset occurs */
    EEPROM_writeByte(FIRST_TIME_ADDRESS, 0x00);
}

void passwordSetup(void){
    uint8 pass1[PASSWORD_LENGTH];
    uint8 pass2[PASSWORD_LENGTH];
    
    uint8 passwordCounter = 0;
    boolean isMatch = TRUE;

    do{
        isMatch = TRUE;
        /* send setup password command to HMI */
        Uart_SendByte(HMI_BLOCK_UART, SETUP_PASSWORD_CMD);

        /* Get the 1st password from HMI */
        for(passwordCounter = 0; passwordCounter < PASSWORD_LENGTH; passwordCounter++){
            pass1[passwordCounter] = Uart_ReceiveByte(HMI_BLOCK_UART);
        }

        /* Get the 2nd password from HMI */
        for (passwordCounter = 0; passwordCounter < PASSWORD_LENGTH; passwordCounter++) {
            pass2[passwordCounter] = Uart_ReceiveByte(HMI_BLOCK_UART);
        }

        /* Check if the two passwords match each other */
        for(passwordCounter = 0; passwordCounter < PASSWORD_LENGTH; passwordCounter++){
            if(pass1[passwordCounter] != pass2[passwordCounter]){
                isMatch = FALSE;
                Uart_SendByte(HMI_BLOCK_UART, PASSWORD_MISSMATCH_CMD);
                break;
            }
        }
    } while(isMatch == FALSE); /* Loop until the two passwords match each other */

    /* save the password into the EEPROM */
    for (passwordCounter = 0; passwordCounter < PASSWORD_LENGTH; passwordCounter++) {
        EEPROM_writeByte(PASSWORD_ADDRESS + passwordCounter, pass1[passwordCounter]);
        Delay_ms(5);
    }
} /* end of passwordSetup function */


void rfidSetup(void){
    /* Array to hold the value of the unique id of the rfid tag */
    uint8 rfidTag1[RFID_UNIQUE_ID_LENGTH];
    uint8 rfidTag2[RFID_UNIQUE_ID_LENGTH];
    
    /* A counter to loop through the array of the uid */
    uint8 uidCounter = 0;

    /* send setup rfid command to HMI */
    Uart_SendByte(HMI_BLOCK_UART, SETUP_RFID_CMD);
    
    /* get the 1st rfid uid from HMI */
    for(uidCounter = 0; uidCounter < RFID_UNIQUE_ID_LENGTH; uidCounter++){
        rfidTag1[uidCounter] = Uart_ReceiveByte(HMI_BLOCK_UART);
    }

    /* get the 2nd rfid uid from HMI */
    for (uidCounter = 0; uidCounter < RFID_UNIQUE_ID_LENGTH; uidCounter++) {
        rfidTag2[uidCounter] = Uart_ReceiveByte(HMI_BLOCK_UART);
    }

    /* Save the 1st unique RFID to the EEPROM */
    for(uidCounter = 0; uidCounter < RFID_UNIQUE_ID_LENGTH; uidCounter++){
        EEPROM_writeByte(RFID_ADDRESS + uidCounter, rfidTag1[uidCounter]);
    }

    /* Save the 2nd unique RFID to the EEPROM */
    for(uidCounter = 0; uidCounter < RFID_UNIQUE_ID_LENGTH; uidCounter++){
        EEPROM_writeByte(RFID_ADDRESS + RFID_UNIQUE_ID_LENGTH + uidCounter, rfidTag2[uidCounter]);
    }
} /* end of rfidSetup function */

void faceSetup(void){
    /* A variable to hold the response on the image taken from the raspberry pi */
    uint8 faceResponse = 0;
    /* Send look at camera command to the HMI to envoke the user to look at the user */
    Uart_SendByte(HMI_BLOCK_UART, LOOK_AT_CAMERA_CMD);
    Delay_ms(3000);

    do{
        /* Send first time command to raspberry pi */
        Uart_SendByte(RASPBERRY_PI_UART, RASP_FIRST_TIME_CMD);
        /* Get the response from raspberry pi */
        faceResponse = Uart_ReceiveByte(RASPBERRY_PI_UART);
        
        if(faceResponse == FACE_FAILED_ERROR){
            Uart_SendByte(HMI_BLOCK_UART, FACE_SETUP_FAILED_CMD);
            Uart_SendByte(HMI_BLOCK_UART, LOOK_AT_CAMERA_CMD);
            Delay_ms(3000);
        }
    } while(faceResponse == FACE_FAILED_ERROR); /* Loop until the */
}