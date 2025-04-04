#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>

typedef struct {
    int16_t ax;
    int16_t ay;
    int16_t az;
    int16_t gx;
    int16_t gy;
    int16_t gz;
} MPU6050_Data;

int mpu6050_init(int i2c_bus);
int mpu6050_read(MPU6050_Data* data);
void mpu6050_config(void);
float get_angle_from_mpu6050();



#endif
