#include "mpu6050.h"
#include <wiringPiI2C.h>
#include <unistd.h>
#include <math.h>

#define MPU6050_ADDR 0x68
#define MPU6050_PWR_MGMT_1 0x6B
#define ACCEL_SCALE 2048.0
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



static int mpu6050_fd = -1;

// Đọc 2 byte liên tiếp
static int16_t read_word(int reg) {
    int high = wiringPiI2CReadReg8(mpu6050_fd, reg);
    int low  = wiringPiI2CReadReg8(mpu6050_fd, reg + 1);
    return (int16_t)((high << 8) | low);
}

int mpu6050_init(int i2c_bus) {
    mpu6050_fd = wiringPiI2CSetupInterface("/dev/i2c-1", MPU6050_ADDR);
    if (mpu6050_fd < 0) return -1;

    mpu6050_config(); // gọi cấu hình

    return 0;
}


int mpu6050_read(MPU6050_Data* data) {
    if (mpu6050_fd < 0 || !data) return -1;

    data->ax = read_word(0x3B);  // Đọc gia tốc trục X
    data->ay = read_word(0x3D);  // Đọc gia tốc trục Y
    data->az = read_word(0x3F);  // Đọc gia tốc trục Z

    data->gx = read_word(0x43);  // Đọc tốc độ góc trục X
    data->gy = read_word(0x45);  // Đọc tốc độ góc trục Y
    data->gz = read_word(0x47);  // Đọc tốc độ góc trục Z

    return 0;
}

void mpu6050_config(void) {
    // Bật MPU6050
    wiringPiI2CWriteReg8(mpu6050_fd, 0x6B, 0x00);

    // Thiết lập DLPF < 100Hz (DLPF_CFG = 4 ~ 20Hz gyro, 21Hz accel)
    wiringPiI2CWriteReg8(mpu6050_fd, 0x1A, 0x04);

    // Thiết lập tần số lấy mẫu: 200Hz = 1000Hz / (1 + SMPLRT_DIV)
    wiringPiI2CWriteReg8(mpu6050_fd, 0x19, 0x04); // SMPLRT_DIV = 4

    // Thiết lập ±1000°/s cho gyro (bits 4:3 = 10)
    wiringPiI2CWriteReg8(mpu6050_fd, 0x1B, 0x10);

    // Thiết lập ±16g cho accel (bits 4:3 = 11)
    wiringPiI2CWriteReg8(mpu6050_fd, 0x1C, 0x18);
}
float get_angle_from_mpu6050() {
    MPU6050_Data data;
    mpu6050_read(&data);  // Đọc dữ liệu từ MPU6050 vào cấu trúc 'data'

    // Giả sử đã scale giá trị acc về đơn vị 'g'
    float fax = data.ax / ACCEL_SCALE;
    float faz = data.az / ACCEL_SCALE;

    // Tính góc nghiêng theo trục X (độ)
    float angle = atan2f(fax, faz) * 180.0 / M_PI;
    return angle;
}




