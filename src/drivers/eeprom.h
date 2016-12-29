#ifndef SRC_DRIVERS_EEPROM_H_
#define SRC_DRIVERS_EEPROM_H_

#include "hal.h"

void init_eeprom(void);
// Erase whole eeprom storage
void erase_eeprom(void);

bool read_block(const void *data, uint16_t addr, size_t n);
bool write_block(uint16_t addr, const void *data, size_t n);


#endif /* SRC_DRIVERS_EEPROM_H_ */
