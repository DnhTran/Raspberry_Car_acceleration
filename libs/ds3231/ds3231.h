#ifndef DS3231_H
#define DS3231_H

#include <stdint.h>

// Cấu trúc giữ thời gian
typedef struct {
    uint8_t second;
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t date;
    uint8_t month;
    uint16_t year;
} RTC_Time;

int ds3231_init(int i2c_bus);
int ds3231_get_time(RTC_Time* time);
int ds3231_set_time(const RTC_Time* time);

#endif
