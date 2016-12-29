#ifndef SRC_PARAMETERS_D_H_
#define SRC_PARAMETERS_D_H_
#include "ch.h"
#include "hal.h"

#include "parameters.h"
#include "parameters_d.h"
#include "telemetry.h"

//////////////////////////////////////////////////////////////////
// STOP!!! DO NOT CHANGE THIS VALUE UNTIL YOU FULLY UNDERSTAND THE
// COMMENTS ABOVE. IF UNSURE, ASK ANOTHER DEVELOPER!!!
static const uint16_t k_format_version = 2;
//////////////////////////////////////////////////////////////////


// EEPROM layout
enum {
    k_param_format_version = 0,
    k_param_rpm_pid_p,
    k_param_rpm_pid_i,
    k_param_rpm_pid_d,
    k_param_stream_param,
    k_param_stream_sensor,
    k_param_stream_rc_chan,
    k_param_stream_controller,
    k_param_rpm_lpf_beta,
    k_param_volt_pid_p,
    k_param_volt_pid_i,
    k_param_volt_pid_d,
    k_param_target_voltage,
    k_param_volt_lpf_beta,
    k_param_pid_report,
    k_param_max_man_thr,
};



extern float rpm_pid_p;
extern float rpm_pid_i;
extern float rpm_pid_d;
extern float rpm_lpf_beta;
extern int16_t stream_rates[NUM_STREAMS];
extern float volt_pid_p, volt_pid_i, volt_pid_d;
extern float target_voltage;
extern float volt_lpf_beta;
extern int16_t pid_report;
extern float max_man_thr;

void load_parameters(void);


#endif /* SRC_PARAMETERS_D_H_ */
