/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef _BOARD_H_
#define _BOARD_H_


/*
 * Board identifier.
 */
#define BOARD_LOAD_REV_A
#define BOARD_NAME              "el_load Rev. A"

/*
 * Board frequencies.
 */
#define STM32_LSECLK            0

#define STM32_HSECLK            16000000
/*#define STM32_HSE_BYPASS*/


/*
 * MCU type, supported types are defined in ./os/hal/platforms/hal_lld.h.
 */
#define STM32F103xE

/*
 * IO pins assignments.
 */
#define GPIOC_LED1	            0
#define GPIOC_LED2		        1
#define GPIOC_THROTTLE_SERVO    5
#define GPIOA_RPM               8

#define EEPROM_ADDRESS  0x50
#define EEPROM_SIZE     4096 //bytes
#define EEPROM_PAGE_SIZE 32 //bytes
#define EEPROM_BUS      I2CD1


/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 *
 * The digits have the following meaning:
 *   0 - Analog input.
 *   1 - Push Pull output 10MHz.
 *   2 - Push Pull output 2MHz.
 *   3 - Push Pull output 50MHz.
 *   4 - Digital input.
 *   5 - Open Drain output 10MHz.
 *   6 - Open Drain output 2MHz.
 *   7 - Open Drain output 50MHz.
 *   8 - Digital input with PullUp or PullDown resistor depending on ODR.
 *   9 - Alternate Push Pull output 10MHz.
 *   A - Alternate Push Pull output 2MHz.
 *   B - Alternate Push Pull output 50MHz.
 *   C - Reserved.
 *   D - Alternate Open Drain output 10MHz.
 *   E - Alternate Open Drain output 2MHz.
 *   F - Alternate Open Drain output 50MHz.
 * Please refer to the STM32 Reference Manual for details.
 */

/*
 * Port A setup.
 * Everything input with pull-up except:
 * PA0  - Push Pull output           (AUX1).
 * PA1  - Digital input with pull down().
 * PA2  - Push Pull output           (CURR_PWM).
 * PA3  - Push Pull output           (AUX2).
 * PA4  - Alternate Open Drain output(CURR_DAC).
 * PA5  - Alternate Push Pull output (SCK_1).
 * PA6  - Analog                     (VOLTAGE).
 * PA7  - Alternate Push Pull output (MOSI_1).
 * PA8  - Digital input              (RPM).
 * PA9  - Digital input              (EN_B).
 * PA10 - Digital input              (EN_SW).
 * PA11 - Digital input with PullUp  (CAN_RX).
 * PA12 - Alternate Push Pull output (CAN_TX).
 */
#define VAL_GPIOACRL            0x80BF1181      /*  PA7...PA0 */
#define VAL_GPIOACRH            0x888B8488      /* PA15...PA8 */
#define VAL_GPIOAODR            0xFFFFFCFF

/*
 * Port B setup.
 * Everything input with pull-up except:
 * PB0  - Alternate Push Pull output (SERVO_THROTTLE).
 * PB1  - Push Pull output           (LCD_DC).
 * PB2  - Push Pull output           (AUX3).
 * PB6  - Alternate Open Drain output(UART_TX_1).
 * PB7  - Digital input              (UART_RX_1).
 * PB8  - Alternate Open Drain output(SCL_1).
 * PB9  - Alternate Open Drain output(SDA_1).
 * PB10 - Push Pull output           (AUX4).
 * PB11 - Push Pull output           (T_CS).
 * PB12 - Push Pull output           (SC_CS).
 * PB13 - Alternate Push Pull output (SCK_2).
 * PB14 - Digital input with Push    (MISO_2).
 * PB15 - Alternate Push Pull output (MOSI_2).
 */
#define VAL_GPIOBCRL            0x4B88811A      /*  PB7...PB0 */
#define VAL_GPIOBCRH            0xB8B331FF      /* PB15...PB8 */
#define VAL_GPIOBODR            0xFFFFFFFF

/*
 * Port C setup.
 * Everything input with pull-up except:
 * PC0  - Push Pull output           (LED_1).       
 * PC1  - Push Pull output           (LED_2).
 * PC2  - Analog input               (V_LOAD).
 * PC3  - Analog input               (CURR_MES).
 * PC5  - Push Pull output           ().
 * PC6  - Digital input with pullup  (RC_INPUT).
 * PC14 - Digital input               (OSC32).
 * PC15 - Digital input               (OSC32).
 */
#define VAL_GPIOCCRL            0x881A0011      /*  PC7...PC0 */
#define VAL_GPIOCCRH            0x44888888      /* PC15...PC8 */
#define VAL_GPIOCODR            0xFFFFFFBF

/*
 * Port D setup.
 * Everything input with pull-up except:
 * PD0  - Digital input (XTAL).
 * PD1  - Digital input (XTAL).
 */
#define VAL_GPIODCRL            0x11111144      /*  PD7...PD0 */
#define VAL_GPIODCRH            0x11111111      /* PD15...PD8 */
#define VAL_GPIODODR            0xFFFFFFFF

/*
 * Port E setup (not populated).
 * Everything input with pull-up except:
 */
#define VAL_GPIOECRL            0x11111111      /*  PE7...PE0 */
#define VAL_GPIOECRH            0x11111111      /* PE15...PE8 */
#define VAL_GPIOEODR            0xFFFFFFFF

/*
 * Port F setup (not populated).
 * Everything input with pull-up except:
 */
#define VAL_GPIOFCRL            0x11111111      /*  PE7...PE0 */
#define VAL_GPIOFCRH            0x11111111      /* PE15...PE8 */
#define VAL_GPIOFODR            0xFFFFFFFF

/*
 * Port G setup (not populated).
 * Everything input with pull-up except:
 */
#define VAL_GPIOGCRL            0x11111111      /*  PG7...PG0 */
#define VAL_GPIOGCRH            0x11111111      /* PG15...PG8 */
#define VAL_GPIOGODR            0xFFFFFFFF

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
