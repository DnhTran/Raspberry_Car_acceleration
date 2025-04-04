#include "ds3231.h"
#include <wiringPiI2C.h>

#define DS3231_ADDR 0x68

static int ds3231_fd = -1;

// Chuyển đổi BCD <-> Decimal
static uint8_t bcd_to_dec(uint8_t val) {
    return (val >> 4) * 10 + (val & 0x0F);
}

static uint8_t dec_to_bcd(uint8_t val) {
    return ((val / 10) << 4) | (val % 10);
}

int ds3231_init(int i2c_bus) {
    ds3231_fd = wiringPiI2CSetupInterface("/dev/i2c-1", DS3231_ADDR);
    return (ds3231_fd >= 0) ? 0 : -1;
}

int ds3231_get_time(RTC_Time* time) {
    if (ds3231_fd < 0 || !time) return -1;

    time->second = bcd_to_dec(wiringPiI2CReadReg8(ds3231_fd, 0x00));
    time->minute = bcd_to_dec(wiringPiI2CReadReg8(ds3231_fd, 0x01));
    time->hour   = bcd_to_dec(wiringPiI2CReadReg8(ds3231_fd, 0x02));
    time->day    = bcd_to_dec(wiringPiI2CReadReg8(ds3231_fd, 0x03));
    time->date   = bcd_to_dec(wiringPiI2CReadReg8(ds3231_fd, 0x04));
    time->month  = bcd_to_dec(wiringPiI2CReadReg8(ds3231_fd, 0x05) & 0x1F);
    time->year   = 2000 + bcd_to_dec(wiringPiI2CReadReg8(ds3231_fd, 0x06));

    return 0;
}

int ds3231_set_time(const RTC_Time* time) {
    if (ds3231_fd < 0 || !time) return -1;

    wiringPiI2CWriteReg8(ds3231_fd, 0x00, dec_to_bcd(time->second));
    wiringPiI2CWriteReg8(ds3231_fd, 0x01, dec_to_bcd(time->minute));
    wiringPiI2CWriteReg8(ds3231_fd, 0x02, dec_to_bcd(time->hour));
    wiringPiI2CWriteReg8(ds3231_fd, 0x03, dec_to_bcd(time->day));
    wiringPiI2CWriteReg8(ds3231_fd, 0x04, dec_to_bcd(time->date));
    wiringPiI2CWriteReg8(ds3231_fd, 0x05, dec_to_bcd(time->month));
    wiringPiI2CWriteReg8(ds3231_fd, 0x06, dec_to_bcd(time->year % 100));

    return 0;
}
