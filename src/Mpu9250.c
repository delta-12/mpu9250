/**
 * @file Mpu9250.h
 *
 * @brief Driver for the MPU-9250 IMU.
 *
 ******************************************************************************/

/* Includes
 ******************************************************************************/

#include "Mpu9250.h"

/* Defines
 ******************************************************************************/

#define MPU9250_SIZE_BYTES_1           1UL
#define MPU9250_SIZE_BYTES_2           2UL
#define MPU9250_BIT                    0x01U
#define MPU9250_BYTE_MASK              0xFFU
#define MPU9250_WHO_AM_I_DEFAULT_VALUE 0x71U

/* Function Prototypes
 ******************************************************************************/

static inline bool Mpu9250_ApplyGyroConfig(const Mpu9250_Handle_t *const handle);
static inline bool Mpu9250_ApplyAccelConfig(const Mpu9250_Handle_t *const handle);

/* Function Definitions
 ******************************************************************************/

bool Mpu9250_Init(Mpu9250_Handle_t *const handle, bool (*write)(const MPU9250_RegisterAddress_t, const uint8_t *const, const size_t), bool (*read)(const MPU9250_RegisterAddress_t, uint8_t *const, const size_t))
{
  bool init = false;

  if (handle != NULL && write != NULL && read != NULL)
  {
    handle->Write = write;
    handle->Read = read;

    /* TODO configure (see SMPLRT_DIV and CONFIG) */
    uint8_t data = 0U;
    handle->Write(MPU9250_SMPLRT_DIV, &data, MPU9250_SIZE_BYTES_1);
    handle->Write(MPU9250_CONFIG, &data, MPU9250_SIZE_BYTES_1);

    handle->GyroConfig.DcBiasX = 0U;
    handle->GyroConfig.DcBiasY = 0U;
    handle->GyroConfig.DcBiasZ = 0U;
    handle->GyroConfig.SelfTest = false;
    handle->GyroConfig.Scale = MPU9250_GYROSCALE_250;
    /* TODO Fchoice_b */

    handle->AccelConfig.SelfTest = false;
    handle->AccelConfig.Scale = MPU9250_ACCELSCALE_2;
    /* TODO Fchoice_b */
    /* TODO A_DLPFCFG */
    /* TODO Low Power Accelerometer ODR Control */
    /* TODO Wake-on Motion Threshold */

    /* TODO FIFO enable */

    /* TODO I2C master and slave control if applicable */

    init = (Mpu9250_ApplyGyroConfig(handle) && Mpu9250_ApplyAccelConfig(handle));
  }

  return init;
}

bool Mpu9250_WhoAmI(const Mpu9250_Handle_t *const handle)
{
  bool id = false;

  if (handle != NULL)
  {
    uint8_t whoAmI = 0x00U;

    if (handle->Read(MPU9250_WHO_AM_I, &whoAmI, MPU9250_SIZE_BYTES_1))
    {
      id = (whoAmI == MPU9250_WHO_AM_I_DEFAULT_VALUE);
    }
  }

  return id;
}

static inline bool Mpu9250_ApplyGyroConfig(const Mpu9250_Handle_t *const handle)
{
  /* TODO set DcBiases */
  /* TODO set Fchoice_b */

  uint8_t data = (handle->GyroConfig.Scale << MPU9250_GYRO_FS_SEL) & MPU9250_BYTE_MASK;

  if (handle->GyroConfig.SelfTest)
  {
    data |= (MPU9250_BIT << MPU9250_XGYRO_CTEN) & MPU9250_BYTE_MASK;
    data |= (MPU9250_BIT << MPU9250_YGYRO_CTEN) & MPU9250_BYTE_MASK;
    data |= (MPU9250_BIT << MPU9250_ZGYRO_CTEN) & MPU9250_BYTE_MASK;
  }

  return handle->Write(MPU9250_GYRO_CONFIG, &data, MPU9250_SIZE_BYTES_1);
}

static inline bool Mpu9250_ApplyAccelConfig(const Mpu9250_Handle_t *const handle)
{
  /* TODO set Fchoice_b */
  /* TODO set A_DLPFCFG */

  uint8_t data = (handle->AccelConfig.Scale << MPU9250_ACCEL_FS_SEL) & MPU9250_BYTE_MASK;

  if (handle->AccelConfig.SelfTest)
  {
    data |= (MPU9250_BIT << MPU9250_AX_ST_EN) & MPU9250_BYTE_MASK;
    data |= (MPU9250_BIT << MPU9250_AY_ST_EN) & MPU9250_BYTE_MASK;
    data |= (MPU9250_BIT << MPU9250_AZ_ST_EN) & MPU9250_BYTE_MASK;
  }

  /* TODO extend data to uint16_t and write to ACCELC_CONFIG and ACCEL_CONFIG_2 in single operation */
  return handle->Write(MPU9250_ACCEL_CONFIG, &data, MPU9250_SIZE_BYTES_1);
}