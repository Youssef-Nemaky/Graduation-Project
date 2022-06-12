 /******************************************************************************
 *
 * Module: Main Source File for The HMI Block
 *
 * File Name: HMI_ECU.c
 *
 * Description: Source File for HMI ECU Block Functions
 *
 * Created on: Dec 9, 2021
 *
 * Authors: Mourad Eldin Nash'at , Abdelaziz Abu Zaid
 *
 *******************************************************************************/

/*******************************************************************************
 *                                  Includes                                   *
 *******************************************************************************/

#include "HMI_ECU.h"

/*******************************************************************************
 *                     		      Main Function                                *
 *******************************************************************************/

int main(void)
{
    uint8 command;
    /* Initialize Drivers to be ready to use when it is needed */
    Drivers_Init();
    LCD_displayString("2looo");
    Delay_ms(1000);
    LCD_clearScreen();
    /* Super Loop */
    while (1) {
        command = Uart_ReceiveByte(CONTROL_BLOCK_UART);
        LCD_clearScreen();
        switch (command)
        {
        case FIRST_TIME_CMD:
            LCD_displayStringRowColumn(0, 2, "First Time Setup");
            Delay_ms(LCD_MESSAGE_DELAY);
            break;
        case SETUP_PASSWORD_CMD:
            LCD_displayString("1: Enter password");
            LCD_moveCursor(1,7);
            getPassword();
            LCD_clearScreen();
            LCD_displayString("2: Re-Enter password");
            LCD_moveCursor(1,7);
            getPassword();
            break;
        case PASSWORD_MISSMATCH_CMD:
            LCD_displayString("Passwords Missmatch!");
            Delay_ms(LCD_MESSAGE_DELAY);
            break;
        case GET_PASSWORD_CMD:
            LCD_displayString("Enter your password");
            LCD_moveCursor(1, 7);
            getPassword();
            break;
        case WRONG_PASSWORD_CMD:
            LCD_displayStringRowColumn(0, 2, "Wrong Password!");
            Delay_ms(LCD_MESSAGE_DELAY);
            break;
        case SETUP_RFID_CMD:
            LCD_displayString("Enter your 1st tag");
            getRfidTag();
            
            LCD_clearScreen();
            LCD_displayString("Enter your 2nd tag");
            getRfidTag();
            break;
        case GET_RFID_CMD:
            LCD_displayStringRowColumn(0, 3,"Scan your tag");
            getRfidTag();
            break;
        case TAG_FAILED_CMD:
            LCD_displayStringRowColumn(0, 4,"Tag Failed!");
            Delay_ms(LCD_MESSAGE_DELAY);
            break;
        case LOOK_AT_CAMERA_CMD:
            LCD_displayString("Smile to the camera!");
            LCD_displayStringRowColumn(1,0, "Wait for green light");
            break;
        /* FOR DEBUGGING PURPOSES */
        case FACE_SETUP_FAILED_CMD:
            LCD_displayString("Face ID Failed,Retry");
            Delay_ms(LCD_MESSAGE_DELAY);
            break;
        case SETUP_COMPLETE_CMD:
            LCD_displayString("Setup Complete! :)");
            Delay_ms(LCD_MESSAGE_DELAY);
            break;
        case DISPLAY_OPTIONS_CMD:
            /* Display Available Options on The Screen */
            LCD_displayString("1: Unlock The Engine");
            LCD_moveCursor(1, 0);
            LCD_displayString("2: Change Passcode");
            getOption();
            break;
        case ACCESS_GRANTED_CMD:
            LCD_displayString("Access Granted!");
            Delay_ms(LCD_MESSAGE_DELAY);
            break;
        case LOCK_CMD:
            LCD_displayString("7ramy");
            break;
        case PASSWORD_CHANGED_CMD:
            LCD_displayString("Password Changed!");
            break;
        default:
            LCD_displayString("IDK WHO U ARE!!");
            break;
        }
    }
}

/*******************************************************************************************************
 * [Name]: Drivers_Init
 * [Parameters]: void (none)
 * [Return]: void (none)
 * [Description]: The Function responsible for Initializing of (MCAL) Drivers like UART and (HAL) Drivers
 * like LCD and Enabling Global Interrupt (I-Bit) in HMI Block.
 ********************************************************************************************************/
void Drivers_Init(void){
    Port_Init(&Port_Configuration);
    Dio_Init(&Dio_Configuration);
    Uart_Init(&Uart_Configuration);
    Spi_Init(&Spi_Configuration);
    RFID_Init();
    LCD_init();
}


void getPassword(void){
    /* Array to hold the passcode */
    uint8 passCode[PASSCODE_LENGTH] = { 0 };
    /* A counter to loop through the passcode array */
    uint8 passCodeCounter = 0;
    /* A variable to hold the pressed key */
    uint8 pressedKey = 0;
    /* A variable to read the continue command */
    uint8 continueCMD = 0;

    /* loop through the passcode array and save the input pressed key */
    for(passCodeCounter = 0; passCodeCounter < PASSCODE_LENGTH;){
        /* Get the pressed key from the keypad */
        pressedKey = KEYPAD_getPressedKey();
        /* Make sure it's a number not a character */
        if((pressedKey >= 0) && (pressedKey <= 9)){
            /* Only increment the variable passCodeCounter when the entered key is a number */
            passCode[passCodeCounter++] = pressedKey;
            LCD_displayCharacter('*');
        }
        /* A small delay to avoid button de-bouncing */
        Delay_ms(KEYPAD_DELAY);
    } /* end of for loop */

    /* Send the password to control block after reading it from the user */
    for(passCodeCounter = 0; passCodeCounter < PASSCODE_LENGTH; passCodeCounter++){
        Uart_SendByte(CONTROL_BLOCK_UART, passCode[passCodeCounter]);
        continueCMD = Uart_ReceiveByte(CONTROL_BLOCK_UART);
    } /* end of for loop */
} /* end of getPassword function */

void getRfidTag(void){
    /* Array to hold the value of the unique id of the rfid tag */
    uint8 rfidTag[RFID_UNIQUE_ID_LENGTH];
    /* A counter to loop through the array of the uid */
    uint8 uidCounter = 0;
    /* A variable to read the continue command */
    uint8 continueCMD = 0;

    /* Read the rfid tag */
    rfidReadTag(rfidTag);

    /* Send the rfid tag to the control block after scanning it */
    for(uidCounter = 0; uidCounter < RFID_UNIQUE_ID_LENGTH; uidCounter++){
        Uart_SendByte(CONTROL_BLOCK_UART, rfidTag[uidCounter]);
        continueCMD = Uart_ReceiveByte(CONTROL_BLOCK_UART);
    } /* end of for loop */
} /* end of getRfidTag function */

void rfidReadTag(uint8 * a_rfid_tag){
    uint8 byte;

    byte = RFID_Read(ComIEnReg);
    RFID_Write(ComIEnReg, byte | 0x20);
    byte = RFID_Read(DivIEnReg);
    RFID_Write(DivIEnReg, byte | 0x80);

    while (1) {
        byte = RFID_Request(PICC_REQALL, a_rfid_tag);
        if (byte == CARD_FOUND)
        {
            byte = RFID_getCardSerial(a_rfid_tag);
            /* Str has the UID of the rfid tag */

                 /* Delete me later */
            if (byte == CARD_FOUND)
            {
                LCD_clearScreen();
                for (byte = 0;byte < 8;byte++) {
                    LCD_displayHex(a_rfid_tag[byte]);
                }
                Delay_ms(2500);
                return;
            }
        }
        Delay_ms(1000);
    }
}

void getOption(void){
    /* A variable to hold the chosen option */
    uint8 option = 0;

    /* Loop until the user chooses a valid option */
    do {
        option = KEYPAD_getPressedKey();
        Delay_ms(KEYPAD_DELAY);
    } while (option != 1 && option != 2);

    /* Send the option back to the control block */
    Uart_SendByte(CONTROL_BLOCK_UART, option);
} /* end of getOption function */

