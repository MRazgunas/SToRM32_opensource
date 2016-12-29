#include <math.h>
#include <string.h>

#include "ch.h"
#include "hal.h"

#include "telemetry.h"
#include "parameters.h"
#include "eeprom.h"

void write_sentinal(uint16_t ofs);
void eeprom_write_check(const void *ptr, uint16_t ofs, uint8_t size);
void write_sentinal(uint16_t ofs);
uint8_t type_size(ap_var_type type);
bool duplicate_key(uint16_t vindex, uint16_t key);
bool scan(const Param_header *target, uint16_t *pofs);
bool is_sentinal(const Param_header *phdr);
bool get_base(const Info *info, ptrdiff_t *base);
const Info *find_var_info(const void * ptr);
const Info * find_by_header(Param_header phdr, void **ptr);
void send_parameter(const Info *info, const char *name, ap_var_type var_type);

const Info *_var_info;
uint16_t _num_vars;

// erase all EEPROM variables by re-writing the header and adding
// a sentinal
void erase_all(void)
{
    struct EEPROM_header hdr;

    // write the header
    hdr.magic[0] = k_EEPROM_magic0;
    hdr.magic[1] = k_EEPROM_magic1;
    hdr.revision = k_EEPROM_revision;
    hdr.spare    = 0;
    eeprom_write_check(&hdr, 0, sizeof(hdr));

    // add a sentinal directly after the header
    write_sentinal(sizeof(struct EEPROM_header));
}


void eeprom_write_check(const void *ptr, uint16_t ofs, uint8_t size)
{
    write_block(ofs, ptr, size);
}

// write a sentinal value at the given offset
void write_sentinal(uint16_t ofs)
{
    struct Param_header phdr;
    phdr.type = _sentinal_type;
    phdr.key = _sentinal_key;
    eeprom_write_check(&phdr, ofs, sizeof(phdr));
}

void init_param_lib(const Info *var_infop) {
    // Init library
    _var_info = var_infop;
    uint16_t i;
    for(i = 0; _var_info[i].type != AP_PARAM_NONE; i++);
    _num_vars = i;

    //Check for eeprom header
    EEPROM_header hdr;
    read_block(&hdr, 0, sizeof(hdr));
    if(hdr.magic[0] != k_EEPROM_magic0 ||
       hdr.magic[1] != k_EEPROM_magic1 ||
       hdr.revision != k_EEPROM_revision) {
        //TODO: debug message: "Bad eeprom header - erasing"
        erase_all();
    }

    //Load all defaults
    for(uint16_t i = 0; i < _num_vars; i++) {
        uint8_t type = _var_info[i].type;
        if(type <= AP_PARAM_FLOAT) {
            ptrdiff_t base;
            if(get_base(&_var_info[i], &base)) {
                set_value((ap_var_type)type, (void*) base, _var_info[i].def_value);
            }
        }
    }

}

// return the storage size for a AP_PARAM_* type
uint8_t type_size(ap_var_type type)
{
    switch (type) {
    case AP_PARAM_NONE:
    case AP_PARAM_GROUP:
        return 0;
    case AP_PARAM_INT8:
        return 1;
    case AP_PARAM_INT16:
        return 2;
    case AP_PARAM_INT32:
        return 4;
    case AP_PARAM_FLOAT:
        return 4;
    }
    return 0;
}

bool duplicate_key(uint16_t vindex, uint16_t key)
{
    for (uint16_t i=vindex+1; i<_num_vars; i++) {
        uint16_t key2 = _var_info[i].key;
        if (key2 == key) {
            // no duplicate keys allowed
            return true;
        }
    }
    return false;
}

bool check_var_info(void) {
    uint16_t total_size = sizeof(struct EEPROM_header);

    for(uint16_t i; i < _num_vars; i++) {
        uint8_t type = _var_info[i].type;
        uint16_t key = _var_info[i].key;
        if(type != AP_PARAM_GROUP) {
            uint8_t size = type_size((ap_var_type) type);
            if(size == 0) {
                // not a valid type - the top level list can't contain AP_PARAM_NONE
                return false;
            }
            total_size += size + sizeof(struct Param_header);
        }
        if(duplicate_key(i, key)) {
            return false;
        }
        if (type != AP_PARAM_GROUP && (_var_info[i].flags & AP_PARAM_FLAG_POINTER)) {
            // only groups can be pointers
            return false;
        }
    }

    return true; //All check passed
}

const Info *find_var_info(const void * ptr) {
    for(uint16_t i = 0; i < _num_vars; i++) {
        if(_var_info[i].ptr == ptr)
            return &_var_info[i];
    }
    return NULL;
}

bool load_value_using_pointer(const void * ptr) {
    const Info *info = find_var_info(ptr);

    if(info == NULL)
    {
        //we don't have any info how to load it
        return false;
    }

    Param_header phdr;

    // create header we will use to match variable
    phdr.type = info->type;
    phdr.key = info->key;

    //scan EEPROM to find the right location
    uint16_t ofs;
    if(!scan(&phdr, &ofs)) {
        // if the value isn't stored in EEPROM then set the default value
        ptrdiff_t base;
        if(!get_base(info, &base)) {
            return false;
        }

        set_value((ap_var_type)phdr.type, (void*)base, info->def_value);
        return false;
    }

    read_block(info->ptr, ofs+sizeof(phdr), type_size((ap_var_type)phdr.type));
    return true;
}

// scan the EEPROM looking for a given variable by header content
// return true if found, along with the offset in the EEPROM where
// the variable is stored
// if not found return the offset of the sentinal
// if the sentinal isn't found either, the offset is set to 0xFFFF
bool scan(const Param_header *target, uint16_t *pofs) {
    Param_header phdr;
    uint16_t ofs = sizeof(EEPROM_header);
    while(ofs < EEPROM_SIZE) {
        read_block(&phdr, ofs, sizeof(phdr));
        if(phdr.type == target->type &&
           phdr.key == target->key) {
            // found header
            *pofs = ofs;
            return true;
        }
        if(is_sentinal(&phdr)) {
            //we have reached the sentinal
            *pofs = ofs;
            return false;
        }
        ofs += type_size((ap_var_type)phdr.type) + sizeof(phdr);
    }
    *pofs = 0xFFFF;
    return false;
}

//
// return true if a header is the end of eeprom sentinal
//
bool is_sentinal(const Param_header *phdr)
{
    // note that this is an ||, not an &&, as this makes us more
    // robust to power off while adding a variable to EEPROM
    if (phdr->type == _sentinal_type ||
        phdr->key == _sentinal_key) {
        return true;
    }
    return false;
}

bool get_base(const Info *info, ptrdiff_t *base)
{
    //TODO: figure our why this is here
    if (info->flags & AP_PARAM_FLAG_POINTER) {
        *base = *(ptrdiff_t *)info->ptr;
        return (*base != (ptrdiff_t)0);
    }
    *base = (ptrdiff_t)info->ptr;
    return true;
}

// Save variable into EEPROM
//
bool set_and_save_using_pointer(const void * ptr, float value, bool force_save) {
    const Info *info = find_var_info(ptr);

    set_value((ap_var_type)info->type, info->ptr, value);

    save_parameter(ptr, force_save);
    return true;
}

bool save_parameter(const void * ptr, bool force_save) {
    const Info *info = find_var_info(ptr);

    if(info == NULL) {
        // we don't have any info how to store it
        return false;
    }

    Param_header phdr;

    phdr.type = info->type;
    phdr.key = info->key;

    uint16_t ofs;
    if(scan(&phdr, &ofs)) {
        // found an existing copy of the variable
        eeprom_write_check(info->ptr, ofs+sizeof(phdr), type_size((ap_var_type)phdr.type));
        send_parameter(info, info->name, info->type);
        return true;
    }

    if(ofs == (uint16_t)~0) {
        return false;
    }

    if(phdr.type <= AP_PARAM_FLOAT) {
        float v1 = cast_to_float((ap_var_type)phdr.type, info->ptr);
        float v2 = (float) info->def_value;
        if(v1 == v2 && !force_save) {
            send_parameter(info, info->name, info->type);
            return true;
        }
        if (!force_save &&
            (phdr.type != AP_PARAM_INT32 && (fabsf(v1-v2) < 0.0001f*fabsf(v1)))) {
            // for other than 32 bit integers, we accept values within
            // 0.01 percent of the current value as being the same
            send_parameter(info, info->name, info->type);
            return true;
        }
    }

    if(ofs+type_size((ap_var_type)phdr.type) + 2*sizeof(phdr) >= EEPROM_SIZE) {
        //We are out of EEPROM space
        //TODO: debug messages
        return false;
    }

    write_sentinal(ofs + sizeof(phdr) + type_size((ap_var_type)phdr.type));
    eeprom_write_check(info->ptr, ofs+sizeof(phdr), type_size((ap_var_type)phdr.type));
    eeprom_write_check(&phdr, ofs, sizeof(phdr));

    send_parameter(info, info->name, info->type);
    return true;


}

// set a AP_Param variable to a specified value
void set_value(ap_var_type type, const void *ptr, float value)
{
    switch (type) {
    case AP_PARAM_INT8:
        *((int8_t *)ptr) = value;
        break;
    case AP_PARAM_INT16:
        *((int16_t *)ptr) = value;
        break;
    case AP_PARAM_INT32:
        *((int32_t *)ptr) = value;
        break;
    case AP_PARAM_FLOAT:
        *((float *)ptr) = value;
        break;
    default:
        break;
    }
}

// cast a variable to a float given its type
float cast_to_float(ap_var_type type, const void * ptr)
{
    switch (type) {
    case AP_PARAM_INT8:
        return (float) *(int8_t *)ptr;
    case AP_PARAM_INT16:
        return (float) *(int16_t *)ptr;
    case AP_PARAM_INT32:
        return (float) *(int32_t *)ptr;
    case AP_PARAM_FLOAT:
        return *(float *)ptr;
    default:
        return NAN;
    }
}

// Load all variables from EEPROM
//
bool load_all_parameters(void) {
    Param_header phdr;
    uint16_t ofs = sizeof(EEPROM_header);

    while(ofs < EEPROM_SIZE) {
        read_block(&phdr, ofs, sizeof(phdr));
        if(is_sentinal(&phdr)) {
            //we've reached the sentinal
            return true;
        }

        const Info *info;
        void *ptr;

        info = find_by_header(phdr, &ptr);
        if(info != NULL) {
            read_block(ptr, ofs+sizeof(phdr), type_size((ap_var_type)phdr.type));
        }
        ofs += type_size((ap_var_type)phdr.type) + sizeof(phdr);
    }

    //TODO: debug message "no sentinal in load_all_parameters"
    return false;
}

const Info * find_by_header(Param_header phdr, void **ptr) {
    for(uint16_t i = 0; i < _num_vars; i++) {
        uint8_t type = _var_info[i].type;
        uint16_t key = _var_info[i].key;
        if(key != phdr.key) {
            // not right key
            continue;
        }
        if(type == phdr.type) {
            // found it
            ptrdiff_t base;
            if(!get_base(&_var_info[i], &base)) {
                return NULL;
            }
            *ptr = (void*)base;
            return &_var_info[i];
        }
    }
    return NULL;
}

Info * first_param(ParamToken *token, ap_var_type *ptype) {
    token-> key = 0;
    if(_num_vars == 0) {
        return NULL;
    }

    if(ptype != NULL) {
       *ptype = (ap_var_type)_var_info[0].type;
    }
 /*   ptrdiff_t base;
    if(!get_base(&_var_info[0], &base)) {
        // should be impossible
        return NULL;
    } */

    return (Info *)_var_info;
}

uint16_t count_parameters(void) {
    return _num_vars;
}

const Info * next_scalar(ParamToken *token, ap_var_type *ptype) {

    const Info *ap;
    ap_var_type type;
    uint16_t i = token->key;
    if(i >= _num_vars-1) {
        return NULL;
    }
    i++;
    type = (ap_var_type)_var_info[i].type;
    ap = &_var_info[i];

    token->key = i;
    if(ptype != NULL) {
        *ptype = type;
    }
    return ap;

}

const Info * find_using_name(const char *name, ap_var_type *ptype) {
    for(uint16_t i = 0; i < _num_vars; i++) {
        uint8_t type = _var_info[i].type;
        if(strcasecmp(name, _var_info[i].name) == 0) {
            *ptype = (ap_var_type)type;
            return &_var_info[i];
        }
    }
    return NULL;
}

// notify GCS of current value of parameter
void notify(void * ptr) {

    const Info *info = find_var_info(ptr);
    if (info == NULL) {
        // this is probably very bad
        return;
    }

    char name[AP_MAX_NAME_SIZE+1];
    strncpy(name, info->name, AP_MAX_NAME_SIZE);

    uint8_t param_header_type;
    param_header_type = info->type;

    send_parameter(info, name, (ap_var_type)param_header_type);
}

void send_parameter(const Info *info, const char *name, ap_var_type var_type) {
    if (var_type > AP_PARAM_FLOAT) {
        // invalid
        return;
    }
    if(info != NULL) {
        // nice and simple for scalar types
        send_parameter_value_all(name, var_type, cast_to_float(var_type, info->ptr));
        return;
    }
}

