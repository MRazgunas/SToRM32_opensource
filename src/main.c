#include "ch.h"
#include "hal.h"
#include "chprintf.h"

#include <math.h>

#define M_2PI_3 (2*M_PI/3)

/*
 * Blinker thread #1.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

    (void) arg;

    chRegSetThreadName("blinker");
    while (true) {
        palSetPad(GPIOB, GPIOB_LED0);
        chThdSleepMilliseconds(250);
        palClearPad(GPIOB, GPIOB_LED0);
        chThdSleepMilliseconds(250);
    }
}

static PWMConfig pwmcfg = {
  72000000,                                 /* 72MHz PWM clock frequency.   */
  10000,                                    /* PWM frequency 7.2kHz      */
  NULL,
  {
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},
   {PWM_OUTPUT_ACTIVE_HIGH, NULL}
  },
  0,
  0,
#if STM32_PWM_USE_ADVANCED
  0
#endif
};


int main(void) {
    halInit();
    chSysInit();

    pwmStart(&PWMD3, &pwmcfg);
    PWMD3.tim->CR1 |= STM32_TIM_CR1_CMS(1); //Set Center aligned mode

    sdStart(&SD3, NULL);
    /*
     * Creates the example threads.
     */
    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 1, Thread1,
    NULL);

    float angle = 0;
    while (TRUE) {
        pwmEnableChannel(&PWMD3, 3, 5000 + 2500*sinf(angle));
        pwmEnableChannel(&PWMD3, 2, 5000 + 2500*sinf(angle - M_2PI_3));
        pwmEnableChannel(&PWMD3, 1, 5000 + 2500*sinf(angle + M_2PI_3));
        angle += 0.1f;
        chThdSleepMilliseconds(100);
    }
}
