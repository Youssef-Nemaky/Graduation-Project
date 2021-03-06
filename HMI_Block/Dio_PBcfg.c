 /******************************************************************************
 *
 * Module: Dio
 *
 * File Name: Dio_PBcfg.c
 *
 * Description: Post Build Configuration Source file for TM4C123GH6PM Microcontroller - Dio Driver
 *
 * Author: Youssef El-Nemaky
 ******************************************************************************/

#include "Dio.h"

/*
 * Module Version 1.0.0
 */
#define DIO_PBCFG_SW_MAJOR_VERSION              (1U)
#define DIO_PBCFG_SW_MINOR_VERSION              (0U)
#define DIO_PBCFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define DIO_PBCFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define DIO_PBCFG_AR_RELEASE_MINOR_VERSION     (0U)
#define DIO_PBCFG_AR_RELEASE_PATCH_VERSION     (3U)

/* AUTOSAR Version checking between Dio_PBcfg.c and Dio.h files */
#if ((DIO_PBCFG_AR_RELEASE_MAJOR_VERSION != DIO_AR_RELEASE_MAJOR_VERSION)\
 ||  (DIO_PBCFG_AR_RELEASE_MINOR_VERSION != DIO_AR_RELEASE_MINOR_VERSION)\
 ||  (DIO_PBCFG_AR_RELEASE_PATCH_VERSION != DIO_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of PBcfg.c does not match the expected version"
#endif

/* Software Version checking between Dio_PBcfg.c and Dio.h files */
#if ((DIO_PBCFG_SW_MAJOR_VERSION != DIO_SW_MAJOR_VERSION)\
 ||  (DIO_PBCFG_SW_MINOR_VERSION != DIO_SW_MINOR_VERSION)\
 ||  (DIO_PBCFG_SW_PATCH_VERSION != DIO_SW_PATCH_VERSION))
  #error "The SW version of PBcfg.c does not match the expected version"
#endif

/* PB structure used with Dio_Init API */
const Dio_ConfigType Dio_Configuration = {
							/* Normal Channels - Port Number, Channel Number */
                            //DioConf_LED1_PORT_NUM,DioConf_LED1_CHANNEL_NUM,
				            DioConf_LCD_RS_PORT_NUM,DioConf_LCD_RS_CHANNEL_NUM,
                            DioConf_LCD_RW_PORT_NUM,DioConf_LCD_RW_CHANNEL_NUM,
                            DioConf_LCD_EN_PORT_NUM,DioConf_LCD_EN_CHANNEL_NUM,
                            DioConf_SPI_SS_PORT_NUM,DioConf_SPI_SS_CHANNEL_NUM,
                            DioConf_KEPAD_ROW0_PORT_NUM,DioConf_KEYPAD_ROW0_CHANNEL_NUM,
                            DioConf_KEPAD_ROW1_PORT_NUM,DioConf_KEYPAD_ROW1_CHANNEL_NUM,
                            DioConf_KEPAD_ROW2_PORT_NUM,DioConf_KEYPAD_ROW2_CHANNEL_NUM,
                            DioConf_KEPAD_ROW3_PORT_NUM,DioConf_KEYPAD_ROW3_CHANNEL_NUM,
                            DioConf_KEPAD_COL0_PORT_NUM,DioConf_KEYPAD_COL0_CHANNEL_NUM,
                            DioConf_KEPAD_COL1_PORT_NUM,DioConf_KEYPAD_COL1_CHANNEL_NUM,
                            DioConf_KEPAD_COL2_PORT_NUM,DioConf_KEYPAD_COL2_CHANNEL_NUM,
                            DioConf_KEPAD_COL3_PORT_NUM,DioConf_KEYPAD_COL3_CHANNEL_NUM,
							/* Channel Groups - Mask, Offset, Port Number */
                            DioConf_LCD_GRP_MASK,DioConf_LCD_GRP_OFFSET,DioConf_LCD_GRP_PORT_NUM
				         };

