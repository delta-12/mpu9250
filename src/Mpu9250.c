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

#define MPU9250_SIZE_BYTES_1    1UL
#define MPU9250_SIZE_BYTES_2    2UL
#define MPU9250_BIT             0x01U
#define MPU9250_BYTE_MASK       0xFFU
#define MPU9250_HIGH_BYTE_SHIFT 8U

#define MPU9250_WHO_AM_I_DEFAULT_VALUE 0x71U

#define MPU9250_CALIBRATION_POINTS 1500U

#define MPU9250_ACCEL_SCALE_FACTOR_2G  16384.0
#define MPU9250_ACCEL_SCALE_FACTOR_4G  8192.0
#define MPU9250_ACCEL_SCALE_FACTOR_8G  4096.0
#define MPU9250_ACCEL_SCALE_FACTOR_16G 2048.0

#define MPU9250_GYRO_SCALE_FACTOR_250DPS  131.072
#define MPU9250_GYRO_SCALE_FACTOR_500DPS  65.536
#define MPU9250_GYRO_SCALE_FACTOR_1000DPS 32.768
#define MPU9250_GYRO_SCALE_FACTOR_2000DPS 16.384

/* Typedefs
 ******************************************************************************/

typedef enum
{
  MPU9250_SENSORDATABYTE_0,
  MPU9250_SENSORDATABYTE_1,
  MPU9250_SENSORDATABYTE_2,
  MPU9250_SENSORDATABYTE_3,
  MPU9250_SENSORDATABYTE_4,
  MPU9250_SENSORDATABYTE_5
} Mpu9250_SensorDataByte_t;

/* Globals
 ******************************************************************************/

static Mpu9250_ScaleFactor_t Mpu9250_AccelScaleFactorLut[] = {MPU9250_ACCEL_SCALE_FACTOR_2G, MPU9250_ACCEL_SCALE_FACTOR_4G, MPU9250_ACCEL_SCALE_FACTOR_8G, MPU9250_ACCEL_SCALE_FACTOR_16G};
static Mpu9250_ScaleFactor_t Mpu9250_GyroScaleFactorLut[] = {MPU9250_GYRO_SCALE_FACTOR_250DPS, MPU9250_GYRO_SCALE_FACTOR_500DPS, MPU9250_GYRO_SCALE_FACTOR_1000DPS, MPU9250_GYRO_SCALE_FACTOR_2000DPS};

/* Function Prototypes
 ******************************************************************************/

static inline bool Mpu9250_ApplyGyroConfig(Mpu9250_Handle_t *const handle);
static inline bool Mpu9250_ApplyAccelConfig(Mpu9250_Handle_t *const handle);
static inline void Mpu9250_SetRawSensorReading(Mpu9250_SensorReading_t *const sensorReading, const uint8_t *const sensorData, const Mpu9250_DcBias_t biasX, const Mpu9250_DcBias_t biasY, const Mpu9250_DcBias_t biasZ);
static inline void Mpu9250_SetCalcSensorReading(Mpu9250_SensorReading_t *const sensorReading, const Mpu9250_ScaleFactor_t scaleFactor);

/* Function Definitions
 ******************************************************************************/

bool Mpu9250_Init(Mpu9250_Handle_t *const handle, bool (*write)(const MPU9250_RegisterAddress_t, const uint8_t *const, const size_t), bool (*read)(const MPU9250_RegisterAddress_t, uint8_t *const, const size_t))
{
  bool init = false;

  if (handle != NULL && write != NULL && read != NULL)
  {
    handle->Write = write;
    handle->Read = read;

    if (Mpu9250_WhoAmI(handle))
    {
      /* TODO configure (see SMPLRT_DIV and CONFIG) */
      uint8_t data = 0U;
      handle->Write(MPU9250_SMPLRT_DIV, &data, MPU9250_SIZE_BYTES_1);
      handle->Write(MPU9250_CONFIG, &data, MPU9250_SIZE_BYTES_1);

      /* TODO verify reset */
      data = (MPU9250_BIT << MPU_H_RESET) & MPU9250_BYTE_MASK;
      handle->Write(MPU9250_PWR_MGMT_1, &data, MPU9250_SIZE_BYTES_1);

      handle->GyroConfig.DcBiasX = 0U;
      handle->GyroConfig.DcBiasY = 0U;
      handle->GyroConfig.DcBiasZ = 0U;
      handle->GyroConfig.SelfTest = false;
      handle->GyroConfig.Scale = MPU9250_GYROSCALE_250;
      /* TODO Fchoice_b */

      handle->AccelConfig.DcBiasX = 0U;
      handle->AccelConfig.DcBiasY = 0U;
      handle->AccelConfig.DcBiasZ = 0U;
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
  }

  return init;
}

bool Mpu9250_SetGyroScale(Mpu9250_Handle_t *const handle, const Mpu9250_GyroScale_t scale)
{
  bool scaled = false;

  if (handle != NULL)
  {
    handle->GyroConfig.Scale = scale;

    scaled = Mpu9250_ApplyGyroConfig(handle);
  }

  return scaled;
}

bool Mpu9250_SetAccelScale(Mpu9250_Handle_t *const handle, const Mpu9250_AccelScale_t scale)
{
  bool scaled = false;

  if (handle != NULL)
  {
    handle->AccelConfig.Scale = scale;

    scaled = Mpu9250_ApplyAccelConfig(handle);
  }

  return scaled;
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

bool Mpu9250_Calibrate(Mpu9250_Handle_t *const handle)
{
  bool calibrated = false;

  if (handle != NULL)
  {
    Mpu9250_SensorReading_t sensorReading = {0U};
    int32_t gx = 0;
    int32_t gy = 0;
    int32_t gz = 0;
    int32_t ax = 0;
    int32_t ay = 0;
    int32_t az = 0;

    uint16_t i = 0U;
    while (i < MPU9250_CALIBRATION_POINTS)
    {
      if (Mpu9250_Available(handle))
      {
        Mpu9250_GyroRead(handle, &sensorReading);
        gx += sensorReading.RawX;
        gy += sensorReading.RawY;
        gz += sensorReading.RawZ;

        Mpu9250_AccelRead(handle, &sensorReading);
        ax += sensorReading.RawX;
        ay += sensorReading.RawY;
        az += (sensorReading.RawZ - (int16_t)handle->AccelConfig.ScaleFactor);

        i++;
      }

      /* TODO prevent deadlooping */
    }

    handle->GyroConfig.DcBiasX = (int16_t)((double)gx / (double)MPU9250_CALIBRATION_POINTS);
    handle->GyroConfig.DcBiasY = (int16_t)((double)gy / (double)MPU9250_CALIBRATION_POINTS);
    handle->GyroConfig.DcBiasZ = (int16_t)((double)gz / (double)MPU9250_CALIBRATION_POINTS);
    handle->AccelConfig.DcBiasX = (int16_t)((double)ax / (double)MPU9250_CALIBRATION_POINTS);
    handle->AccelConfig.DcBiasY = (int16_t)((double)ay / (double)MPU9250_CALIBRATION_POINTS);
    handle->AccelConfig.DcBiasZ = (int16_t)((double)az / (double)MPU9250_CALIBRATION_POINTS);

    /* TODO fix writing bias to offset registers, offset is 15 bits (see datasheet) */
    /* TODO verify that readings have been adjusted by offsets */
    // if (handle->Write(MPU9250_XG_OFFSET_H, (uint8_t *)(&handle->GyroConfig.DcBiasX), sizeof(Mpu9250_DcBias_t)) &&
    //     handle->Write(MPU9250_YG_OFFSET_H, (uint8_t *)(&handle->GyroConfig.DcBiasY), sizeof(Mpu9250_DcBias_t)) &&
    //     handle->Write(MPU9250_ZG_OFFSET_H, (uint8_t *)(&handle->GyroConfig.DcBiasZ), sizeof(Mpu9250_DcBias_t)) &&
    //     handle->Write(MPU9250_XA_OFFSET_H, (uint8_t *)(&handle->AccelConfig.DcBiasX), sizeof(Mpu9250_DcBias_t)) &&
    //     handle->Write(MPU9250_YA_OFFSET_H, (uint8_t *)(&handle->AccelConfig.DcBiasY), sizeof(Mpu9250_DcBias_t)) &&
    //     handle->Write(MPU9250_ZA_OFFSET_H, (uint8_t *)(&handle->AccelConfig.DcBiasZ), sizeof(Mpu9250_DcBias_t)))
    // {
    //   calibrated = true;
    // }

    calibrated = true;
  }

  return calibrated;
}

bool Mpu9250_Available(const Mpu9250_Handle_t *const handle)
{
  bool available = false;

  if (handle != NULL)
  {
    uint8_t data = 0U;
    if (handle->Read(MPU9250_INT_STATUS, &data, MPU9250_SIZE_BYTES_1))
    {
      available = (bool)((MPU9250_BIT << MPU9250_RAW_DATA_RDY_INT) & data);
    }
  }

  return available;
}

bool Mpu9250_GyroRead(const Mpu9250_Handle_t *const handle, Mpu9250_SensorReading_t *const sensorReading)
{
  bool read = false;

  if (handle != NULL && sensorReading != NULL)
  {
    uint8_t sensorData[MPU9250_SENSOR_DATA_SIZE];

    if (handle->Read(MPU9250_GYRO_XOUT_H, sensorData, MPU9250_SENSOR_DATA_SIZE))
    {
      Mpu9250_SetRawSensorReading(sensorReading, sensorData, handle->GyroConfig.DcBiasX, handle->GyroConfig.DcBiasY, handle->GyroConfig.DcBiasZ);
      Mpu9250_SetCalcSensorReading(sensorReading, handle->GyroConfig.ScaleFactor);

      read = true;
    }
  }

  return read;
}

bool Mpu9250_AccelRead(const Mpu9250_Handle_t *const handle, Mpu9250_SensorReading_t *const sensorReading)
{
  bool read = false;

  if (handle != NULL && sensorReading != NULL)
  {
    uint8_t sensorData[MPU9250_SENSOR_DATA_SIZE];

    if (handle->Read(MPU9250_ACCEL_XOUT_H, sensorData, MPU9250_SENSOR_DATA_SIZE))
    {
      Mpu9250_SetRawSensorReading(sensorReading, sensorData, handle->AccelConfig.DcBiasX, handle->AccelConfig.DcBiasY, handle->AccelConfig.DcBiasZ);
      Mpu9250_SetCalcSensorReading(sensorReading, handle->AccelConfig.ScaleFactor);

      read = true;
    }
  }

  return read;
}

static inline bool Mpu9250_ApplyGyroConfig(Mpu9250_Handle_t *const handle)
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

  handle->GyroConfig.ScaleFactor = Mpu9250_GyroScaleFactorLut[handle->GyroConfig.Scale];

  return handle->Write(MPU9250_GYRO_CONFIG, &data, MPU9250_SIZE_BYTES_1);
}

static inline bool Mpu9250_ApplyAccelConfig(Mpu9250_Handle_t *const handle)
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

  handle->AccelConfig.ScaleFactor = Mpu9250_AccelScaleFactorLut[handle->AccelConfig.Scale];

  /* TODO extend data to uint16_t and write to ACCELC_CONFIG and ACCEL_CONFIG_2 in single operation */
  return handle->Write(MPU9250_ACCEL_CONFIG, &data, MPU9250_SIZE_BYTES_1);
}

static inline void Mpu9250_SetRawSensorReading(Mpu9250_SensorReading_t *const sensorReading, const uint8_t *const sensorData, const Mpu9250_DcBias_t biasX, const Mpu9250_DcBias_t biasY, const Mpu9250_DcBias_t biasZ)
{
  sensorReading->RawX = (sensorData[MPU9250_SENSORDATABYTE_0] << MPU9250_HIGH_BYTE_SHIFT) | sensorData[MPU9250_SENSORDATABYTE_1];
  sensorReading->RawY = (sensorData[MPU9250_SENSORDATABYTE_2] << MPU9250_HIGH_BYTE_SHIFT) | sensorData[MPU9250_SENSORDATABYTE_3];
  sensorReading->RawZ = (sensorData[MPU9250_SENSORDATABYTE_3] << MPU9250_HIGH_BYTE_SHIFT) | sensorData[MPU9250_SENSORDATABYTE_5];

  /* TODO remove after offsets have been properly set */
  sensorReading->RawX -= biasX;
  sensorReading->RawY -= biasY;
  sensorReading->RawZ -= biasZ;
}

static inline void Mpu9250_SetCalcSensorReading(Mpu9250_SensorReading_t *const sensorReading, const Mpu9250_ScaleFactor_t scaleFactor)
{
  sensorReading->CalcX = sensorReading->RawX / scaleFactor;
  sensorReading->CalcY = sensorReading->RawY / scaleFactor;
  sensorReading->CalcZ = sensorReading->RawZ / scaleFactor;
}