#include "ch.h"
#include "hal.h"

#include "rc_input.h"

#define RC_MAX 1930
#define RC_MIN 1012

static virtual_timer_t rc_timeout;

icucnt_t last_width, last_period;
static bool init = false;

/*
 * RC timeout timer callback.
 */
static void rc_timeout_cb(void *arg) {
    (void) arg;
    last_width = 0;
}

static void icuwidthcb(ICUDriver *icup) {
    uint16_t width = icuGetWidthX(icup);

    if(width < RC_MAX && width > RC_MIN)
        last_width = width;

    /* Set timeout virtual timer if we don't get more callback
    * int given time it will set rpm to 0*/
    chSysLockFromISR();
    chVTSetI(&rc_timeout, MS2ST(200), rc_timeout_cb, NULL);
    chSysUnlockFromISR();
}

static void icuperiodcb(ICUDriver *icup) {

  last_period = icuGetPeriodX(icup);
}

static ICUConfig icucfg = {
  ICU_INPUT_ACTIVE_HIGH,
  1000000,               /* 1MHz ICU clock frequency.   */
  icuwidthcb,
  icuperiodcb,
  NULL,
  ICU_CHANNEL_1,
  0
};

void init_rc_input(void) {
    icuStart(&ICUD8, &icucfg);
    icuStartCapture(&ICUD8);
    icuEnableNotifications(&ICUD8);

    /* RPM timeout timer initialization.*/
    chVTObjectInit(&rc_timeout);

    init = true;

}

uint16_t get_rc_input(void) {
    if(!init)
        return 0;
    return last_width;
}

