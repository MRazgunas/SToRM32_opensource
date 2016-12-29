#ifndef SRC_MAVLINK_BRIDGE_H_
#define SRC_MAVLINK_BRIDGE_H_

#define MAVLINK_USE_CONVENIENCE_FUNCTIONS

#include "../mavlink_types.h"
#include "ch.h"
#include "hal.h"


mavlink_system_t mavlink_system = {1, 100}; /* SysID, CompID */

/**
 * @brief Send one char (uint8_t) over a comm channel
 *
 * @param chan MAVLink channel to use
 * @param ch Character to send
 */
static void comm_send_ch(mavlink_channel_t chan, uint8_t ch) {
    if(chan == MAVLINK_COMM_0) { /* UART1 */
        chnPutTimeout(&SD1, ch, MS2ST(5));
    }
}


#endif /* SRC_MAVLINK_BRIDGE_H_ */
