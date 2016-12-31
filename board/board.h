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
#define BOARD_STORM32_v1_3
#define BOARD_NAME              "SToRM32 v1.3"

/*
 * Board frequencies.
 */
#define STM32_LSECLK            0

#define STM32_HSECLK            8000000
/*#define STM32_HSE_BYPASS*/


/*
 * MCU type, supported types are defined in ./os/hal/platforms/hal_lld.h.
 */
#define STM32F103xE

/*
 * IO pins assignments.
 */
#define GPIOB_LED0	            12
#define GPIOB_LED1		        13


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
 * PA0  - Alternate Push Pull output 2MHz   (IR).
 * PA1  - Alternate Push Pull output 2MHz   (MOT_B2).
 * PA2  - Alternate Push Pull output 2MHz   (MOT_C1).
 * PA3  - Alternate Push Pull output 2MHz   (MOT_B1).
 * PA4  - Digital input with Pull down      (NC).
 * PA5  - Battery voltage measurement       (LIPO).
 * PA6  - Alternate Push Pull output 2MHz   (MOT_A1).
 * PA7  - Alternate Push Pull output 2MHz   (MOT_C0).
 * PA8  - Digital input with Pull up        (RC2).
 * PA9  - Digital input with Pull up        (RC1/TX).
 * PA10 - Digital input with Pull up        (RC0/RX).
 * PA11 - Push Pull output 2MHz             (USB-DM).
 * PA12 - Push Pull output 2MHz             (USB-DP).
 * PA13 - Push Pull output 2MHz             (SWDIO).
 * PA14 - Push Pull output 2MHz             (SWCLK).
 * PA15 - Push Pull output 2MHz             (AUX2).
 */
#define VAL_GPIOACRL            0xAA08AAAA      /*  PA7...PA0 */
#define VAL_GPIOACRH            0x22222888      /* PA15...PA8 */
#define VAL_GPIOAODR            0xFFFF0701

/*
 * Port B setup.
 * Everything input with pull-up except:
 * PB0  - Alternate Push Pull output 2MHz   (MOT_B0).
 * PB1  - Alternate Push Pull output 2MHz   (MOT_A0).
 * PB2  - Digital input                     (BOOT1).
 * PB3  - Digital input with Pull down      (NC).
 * PB4  - Digital input with Pull down      (NC).
 * PB5  - Digital input with PullUp         (USB-D).
 * PB6  - Open Drain output 10MHz           (SCL#2).
 * PB7  - Open Drain output 10MHz           (SDA#2).
 * PB8  - Alternate Push Pull output 2MHz   (MOT_C2).
 * PB9  - Alternate Push Pull output 2MHz   (MOT_A2).
 * PB10 - Open Drain output 10MHz           (SCL).
 * PB11 - Open Drain output 10MHz           (SDA).
 * PB12 - Push Pull output 2MHz             (LED0).
 * PB13 - Push Pull output 2MHz             (LED1).
 * PB14 - Push Pull output 2MHz             (AUX0).
 * PB15 - Push Pull output 2MHz             (AUX1).
 */
#define VAL_GPIOBCRL            0x558884AA      /*  PB7...PB0 */
#define VAL_GPIOBCRH            0x222255AA      /* PB15...PB8 */
#define VAL_GPIOBODR            0xFFFF0CFC

/*
 * Port C setup.
 * Everything input with pull-up except:
 * PC0  - Analog input                      (POT0).
 * PC1  - Analog input                      (POT1).
 * PC2  - Analog input                      (POT2).
 * PC3  - Digital input with PullUp         (BUT).
 * PC4  - Push Pull output 2MHz             (XOR).
 * PC5  - Digital input with PullDown       (NC).
 * PC6  - Digital input with PullUp         (RC2-0).
 * PC7  - Digital input with PullUp         (RC2-1).
 * PC8  - Digital input with PullUp         (RC2-2).
 * PC9  - Digital input with PullUp         (RC2-3).
 * PC10 - Alternate Push Pull output 10MHz  (TX).
 * PC11 - Digital input with PullUp         (RX).
 * PC12 - Digital input with PullDown       (NC).
 * PC13 - Digital input with PullDown       (NC).
 * PC14 - Digital input with PullDown       (NC).
 * PC15 - Digital input with PullDown       (NC).
 */
#define VAL_GPIOCCRL            0x88828000      /*  PC7...PC0 */
#define VAL_GPIOCCRH            0x88888988     /* PC15...PC8 */
#define VAL_GPIOCODR            0xFFFF0FC8

/*
 * Port D setup.
 * Everything input with pull-up except:
 * PD0  - Digital input (XTAL).
 * PD1  - Digital input (XTAL).
 */
#define VAL_GPIODCRL            0x22222144      /*  PD7...PD0 */
#define VAL_GPIODCRH            0x22222222      /* PD15...PD8 */
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
