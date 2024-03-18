/**
 * @file Mpu9250.h
 *
 * @brief Driver for the MPU-9250 IMU.
 *
 ******************************************************************************/

#ifndef MPU9250_H
#define MPU9250_H

/* Includes
 ******************************************************************************/

#include "Mpu9250_Registers.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Defines
 ******************************************************************************/

#define MPU2950_I2C_ADDR 0x68U

/* Typedefs
 ******************************************************************************/

typedef enum
{
  MPU9250_GYROSCALE_250,  /* +/- 250 DPS */
  MPU9250_GYROSCALE_500,  /* +/- 500 DPS */
  MPU9250_GYROSCALE_1000, /* +/- 1000 DPS */
  MPU9250_GYROSCALE_2000  /* +/- 2000 DPS */
} Mpu9250_GyroScale_t;

typedef enum
{
  MPU9250_ACCELSCALE_2, /* +/- 2g */
  MPU9250_ACCELSCALE_4, /* +/- 4g */
  MPU9250_ACCELSCALE_8, /* +/- 8g */
  MPU9250_ACCELSCALE_16 /* +/- 16g */
} Mpu9250_AccelScale_t;

typedef enum
{
  MPU9250_MAGRESOLUTION_14_BIT, /* 0.6µT/LSB */
  MPU9250_MAGRESOLUTION_16_BIT  /* 15µT/LSB */
} Mpu9250_MagResolution_t;

typedef struct
{
  bool (*Write)(const MPU9250_RegisterAddress_t, const uint8_t *const, const size_t);
  bool (*Read)(const MPU9250_RegisterAddress_t, uint8_t *const, const size_t);
} Mpu9250_Handle_t;

/* Function Prototypes
 ******************************************************************************/

bool Mpu9250_Init(Mpu9250_Handle_t *const handle, bool (*write)(const MPU9250_RegisterAddress_t, const uint8_t *const, const size_t), bool (*read)(const MPU9250_RegisterAddress_t, uint8_t *const, const size_t));
bool Mpu9250_WhoAmI(const Mpu9250_Handle_t *const handle);

#endif