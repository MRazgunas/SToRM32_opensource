#include "eeprom.h"

#include "ch.h"
#include "hal.h"

#include "string.h"


void wait_for_write_end(void);

static const I2CConfig i2cconfig = {
        OPMODE_I2C,
        100000, //100kHz
        STD_DUTY_CYCLE };

void init_eeprom(void) {
    i2cStart(&EEPROM_BUS, &i2cconfig);
}
// Erase whole eeprom storage
void erase_eeprom(void) {

}

bool read_block(const void *data, uint16_t addr, size_t n) {
    msg_t res;
    uint8_t *b = (uint8_t *)data;
    uint8_t buff[2];
    buff[0] = addr >> 8;
    buff[1] = addr & 0xFF;
    res = i2cMasterTransmitTimeout(&EEPROM_BUS, EEPROM_ADDRESS, buff, 2, b, n, MS2ST(5));
    if (res != 0)
        return false;
    return true;
}


bool write_block(uint16_t addr, const void *data, size_t n) {
    uint8_t send_buff[EEPROM_PAGE_SIZE + 2];
    uint8_t offset = (uint8_t) addr % EEPROM_PAGE_SIZE;
    uint8_t *b = (uint8_t *)data;
    size_t data_size = n;
    msg_t res;

    if(data_size > EEPROM_PAGE_SIZE || n == 0) {
        return false; //can't write more than one page at time or write 0 data
    }
    if(offset+data_size <= EEPROM_PAGE_SIZE) { //we do not cross page boundary send all data
        send_buff[0] = addr >> 8;
        send_buff[1] = addr & 0xFF;
        memcpy(&send_buff[2], b, n);
        res = i2cMasterTransmitTimeout(&EEPROM_BUS, EEPROM_ADDRESS, send_buff, n+2, NULL, 0, MS2ST(10));
        if(res != MSG_OK)
            return false;
        wait_for_write_end();

    } else { //crossing page boundary max data supported is EEPROM_PAGE_SIZE
        data_size = EEPROM_PAGE_SIZE - offset;
        send_buff[0] = addr >> 8;
        send_buff[1] = addr & 0xFF;
        memcpy(&send_buff[2], b, data_size);
        res = i2cMasterTransmitTimeout(&EEPROM_BUS, EEPROM_ADDRESS, send_buff, data_size+2, NULL, 0, MS2ST(10));
        if(res != MSG_OK)
            return false;
        wait_for_write_end();

        addr += data_size;
        data_size = n - data_size;
        send_buff[0] = addr >> 8;
        send_buff[1] = addr & 0xFF;
        memcpy(&send_buff[2], b, data_size);
        res = i2cMasterTransmitTimeout(&EEPROM_BUS, EEPROM_ADDRESS, send_buff, data_size+2, NULL, 0, MS2ST(10));
        if(res != MSG_OK)
            return false;
        wait_for_write_end();


       // chSysHalt("EEPROM boundry");
        return false;
    }
    return true;

}

void wait_for_write_end(void) {
    msg_t res;
    uint8_t n = 0;
    do {
        res = i2cMasterTransmitTimeout(&EEPROM_BUS, EEPROM_ADDRESS, (uint8_t *) 0xFF, 1, NULL, 0, MS2ST(5));
        n++;
        chThdSleepMicroseconds(500); //poling at 0.5ms inteval
    } while(res != MSG_OK);
}


