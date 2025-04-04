#include "max7219.h"
#include "mpu6050.h"
#include <wiringPi.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>  // Thêm dòng này để sử dụng usleep


int main() {
    // Khởi tạo và cấu hình các thiết bị
    mpu6050_init(1);
    max7219_init(0x00, 0x0F, 0x07); // cấu hình MAX7219

    while (1) {
        float angle = get_angle_from_mpu6050(); // Hàm đọc góc nghiêng từ MPU6050

        if (angle > 40.0) {
            display_on_led(angle, "UP");  // Hiển thị trạng thái "UP"
        } else if (angle < -40.0) {
            display_on_led(angle, "DN");  // Hiển thị trạng thái "DN"
        } else {
            display_on_led(angle, "--");  // Hiển thị trạng thái "--"
        }

        usleep(500000);
    }

    return 0;
}
