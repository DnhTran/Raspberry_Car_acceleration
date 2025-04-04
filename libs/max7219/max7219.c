#include "max7219.h"
#include <wiringPiSPI.h>
#include <stdio.h>  // Để sử dụng snprintf
#include <wiringPi.h>

#define SPI_CHANNEL 0
#define SPI_SPEED   1000000

// Bảng mã 7-segment (chỉ định các giá trị cho mỗi ký tự)
static const uint8_t font_7seg[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F, // 9
    0x77, // A
    0x7C, // b
    0x39, // C
    0x5E, // d
    0x79, // E
    0x71, // F
    0x00  // Khoảng trống
};

void max7219_send(uint8_t address, uint8_t data) {
    uint8_t buffer[2] = {address, data};
    wiringPiSPIDataRW(SPI_CHANNEL, buffer, 2);
}

void max7219_init(uint8_t decode_mode, uint8_t intensity, uint8_t scan_limit) {
    wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED);
    
    max7219_send(MAX7219_REG_SHUTDOWN, 0x01);
    max7219_send(MAX7219_REG_DISPLAYTEST, 0x00);
    max7219_send(MAX7219_REG_SCANLIMIT, scan_limit & 0x07);
    max7219_send(MAX7219_REG_DECODEMODE, decode_mode);
    max7219_set_intensity(intensity);
    max7219_clear();
}

void max7219_clear(void) {
    for (uint8_t i = MAX7219_REG_DIGIT0; i <= MAX7219_REG_DIGIT7; i++) {
        max7219_send(i, 0x00);
    }
}

void max7219_set_intensity(uint8_t intensity) {
    max7219_send(MAX7219_REG_INTENSITY, intensity & 0x0F);
}

void max7219_display_number(int num) {
    for (uint8_t i = MAX7219_REG_DIGIT0; i <= MAX7219_REG_DIGIT7; i++) {
        max7219_send(i, num % 10);
        num /= 10;
        if (num == 0) break;
    }
}

void max7219_display_string(const char *str)  // Sửa từ 'char *' thành 'const char *'
    {
    for (int i = 0; str[i] != '\0' && i < 8; i++) {
        // Chuyển đổi từng ký tự sang giá trị hiển thị tương ứng với bảng mã 7-segment
        uint8_t value = 0;
        if (str[i] >= '0' && str[i] <= '9') {
            value = font_7seg[str[i] - '0'];
        } else if (str[i] >= 'A' && str[i] <= 'F') {
            value = font_7seg[str[i] - 'A' + 10]; // Nếu là chữ cái A-F
        } else if (str[i] == '-') {
            value = 0x40;  // Hiển thị dấu "-" trên LED
        } else {
            value = 0x00;  // Nếu là ký tự không hỗ trợ, hiển thị trống
        }
        
        // Gửi giá trị lên MAX7219
        max7219_send(MAX7219_REG_DIGIT0 + i, value);
    }
}

void display_on_led(float angle, const char* status) {
    char buffer[10];

    if (angle > 40.0 || angle < -40.0) {
        // Chớp LED
        max7219_clear();
        delay(500);
        max7219_display_number((int)angle);
        delay(500);
        return;
    }

    // Format: UP-35.6-P
    snprintf(buffer, sizeof(buffer), "%s-%2.1f-P", status, angle);
    max7219_display_string(buffer);
}
