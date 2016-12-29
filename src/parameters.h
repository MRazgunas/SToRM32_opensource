/*
 * parameters.h
 *
 *  Created on: 2016-10-08
 *      Author: matas
 */

#ifndef SRC_PARAMETERS_H_
#define SRC_PARAMETERS_H_

#include <math.h>
#include "ch.h"
#include "hal.h"

#define PACKED __attribute__((packed))

#define AP_MAX_NAME_SIZE 16

/*
  flags for variables in var_info and group tables
 */

// a nested offset is for subgroups that are not subclasses
#define AP_PARAM_FLAG_NESTED_OFFSET 1

// a pointer variable is for dynamically allocated objects
#define AP_PARAM_FLAG_POINTER       2

// an enable variable allows a whole subtree of variables to be made
// invisible
#define AP_PARAM_FLAG_ENABLE        4

// don't shift index 0 to index 63. Use this when you know there will be
// no conflict with the parent
#define AP_PARAM_NO_SHIFT           8

#define AP_VAREND       { AP_PARAM_NONE, "", 0, NULL, NAN, 0}

typedef enum ap_var_type_t {
    AP_PARAM_NONE    = 0,
    AP_PARAM_INT8,
    AP_PARAM_INT16,
    AP_PARAM_INT32,
    AP_PARAM_FLOAT,
    AP_PARAM_GROUP
} ap_var_type;


typedef struct PACKED EEPROM_header {
        uint8_t magic[2];
        uint8_t revision;
        uint8_t spare;
} EEPROM_header;

/* This header is prepended to a variable stored in EEPROM.
 *  The meaning is as follows:
 *
 *  - key: the k_param enum value from Parameter.h in the sketch
 *
 *  - type: the ap_var_type value for the variable
 */
typedef struct PACKED Param_header {
    uint32_t key : 11;
    uint32_t type : 5;
} Param_header;

typedef struct {
    uint32_t key : 11;
} ParamToken;

typedef struct Info {
    uint8_t type; // AP_PARAM_*
    const char *name;
    uint16_t key; // k_param_*
    const void *ptr; // pointer to variable in memory
    const float def_value;
    uint8_t flags;
} Info;

// values filled into the EEPROM header
static const uint8_t        k_EEPROM_magic0      = 0x4B;
static const uint8_t        k_EEPROM_magic1      = 0x4D; ///< "KM"
static const uint8_t        k_EEPROM_revision    = 1; ///< current format revision

static const uint16_t       _sentinal_key   = 0x7FF;
static const uint8_t        _sentinal_type  = 0x1F;

void init_param_lib(const Info *var_infop);
bool check_var_info(void);
bool load_value_using_pointer(const void * ptr);
bool set_and_save_using_pointer(const void * ptr, float value, bool force_save);
void set_value(ap_var_type type, const void *ptr, float value);
void erase_all(void);
bool load_all_parameters(void);
Info * first_param(ParamToken *token, ap_var_type *ptype);
uint16_t count_parameters(void);
float cast_to_float(ap_var_type type, const void * ptr);
const Info * next_scalar(ParamToken *token, ap_var_type *ptype);
const Info * find_using_name(const char *name, ap_var_type *ptype);
bool save_parameter(const void * ptr, bool force_save);


#endif /* SRC_PARAMETERS_H_ */
