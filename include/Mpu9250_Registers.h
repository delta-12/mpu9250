/**
 * @file Mpu9250_Registers.h
 *
 * @brief MPU-9250 register map.
 *
 ******************************************************************************/

#ifndef MPU9250_REGISTERS_H
#define MPU9250_REGISTERS_H

/* Includes
 ******************************************************************************/

#include <stdint.h>

/* Defines
 ******************************************************************************/

#define MPU9250_SELF_TEST_X_GYRO   0x00U
#define MPU9250_SELF_TEST_Y_GYRO   0x01U
#define MPU9250_SELF_TEST_Z_GYRO   0x02U
#define MPU9250_SELF_TEST_X_ACCEL  0x0DU
#define MPU9250_SELF_TEST_Y_ACCEL  0x0EU
#define MPU9250_SELF_TEST_Z_ACCEL  0x0FU
#define MPU9250_XG_OFFSET_H        0x13U
#define MPU9250_XG_OFFSET_L        0x14U
#define MPU9250_YG_OFFSET_H        0x15U
#define MPU9250_YG_OFFSET_L        0x16U
#define MPU9250_ZG_OFFSET_H        0x17U
#define MPU9250_ZG_OFFSET_L        0x18U
#define MPU9250_SMPLRT_DIV         0x19U
#define MPU9250_CONFIG             0x1AU
#define MPU9250_GYRO_CONFIG        0x1BU
#define MPU9250_ACCEL_CONFIG       0x1CU
#define MPU9250_ACCEL_CONFIG_2     0x1DU
#define MPU9250_LP_ACCEL_ODR       0x1EU
#define MPU9250_WOM_THR            0x1FU
#define MPU9250_FIFO_E             0x23U
#define MPU9250_I2C_MST_CTRL       0x24U
#define MPU9250_I2C_SLV0_ADDR      0x25U
#define MPU9250_I2C_SLV0_REG       0x26U
#define MPU9250_I2C_SLV0_CTRL      0x27U
#define MPU9250_I2C_SLV1_ADDR      0x28U
#define MPU9250_I2C_SLV1_REG       0x29U
#define MPU9250_I2C_SLV1_CTRL      0x2AU
#define MPU9250_I2C_SLV2_ADDR      0x2BU
#define MPU9250_I2C_SLV2_REG       0x2CU
#define MPU9250_I2C_SLV2_CTRL      0x2DU
#define MPU9250_I2C_SLV3_ADDR      0x2EU
#define MPU9250_I2C_SLV3_REG       0x2FU
#define MPU9250_I2C_SLV3_CTRL      0x30U
#define MPU9250_I2C_SLV4_ADDR      0x31U
#define MPU9250_I2C_SLV4_REG       0x32U
#define MPU9250_I2C_SLV4_DO        0x33U
#define MPU9250_I2C_SLV4_CTRL      0x34U
#define MPU9250_I2C_SLV4_DI        0x35U
#define MPU9250_I2C_MST_STATUS     0x36U
#define MPU9250_INT_PIN_CFG        0x37U
#define MPU9250_INT_ENABLE         0x38U
#define MPU9250_INT_STATUS         0x3AU
#define MPU9250_ACCEL_XOUT_H       0x3BU
#define MPU9250_ACCEL_XOUT_L       0x3CU
#define MPU9250_ACCEL_YOUT_H       0x3DU
#define MPU9250_ACCEL_YOUT_L       0x3EU
#define MPU9250_ACCEL_ZOUT_H       0x3FU
#define MPU9250_ACCEL_ZOUT_L       0x40U
#define MPU9250_TEMP_OUT_H         0x41U
#define MPU9250_TEMP_OUT_L         0x42U
#define MPU9250_GYRO_XOUT_H        0x43U
#define MPU9250_GYRO_XOUT_L        0x44U
#define MPU9250_GYRO_YOUT_H        0x45U
#define MPU9250_GYRO_YOUT_L        0x46U
#define MPU9250_GYRO_ZOUT_H        0x47U
#define MPU9250_GYRO_ZOUT_L        0x48U
#define MPU9250_EXT_SENS_DATA_00   0x49U
#define MPU9250_EXT_SENS_DATA_01   0x4AU
#define MPU9250_EXT_SENS_DATA_02   0x4BU
#define MPU9250_EXT_SENS_DATA_03   0x4CU
#define MPU9250_EXT_SENS_DATA_04   0x4DU
#define MPU9250_EXT_SENS_DATA_05   0x4EU
#define MPU9250_EXT_SENS_DATA_06   0x4FU
#define MPU9250_EXT_SENS_DATA_07   0x50U
#define MPU9250_EXT_SENS_DATA_08   0x51U
#define MPU9250_EXT_SENS_DATA_09   0x52U
#define MPU9250_EXT_SENS_DATA_10   0x53U
#define MPU9250_EXT_SENS_DATA_11   0x54U
#define MPU9250_EXT_SENS_DATA_12   0x55U
#define MPU9250_EXT_SENS_DATA_13   0x56U
#define MPU9250_EXT_SENS_DATA_14   0x57U
#define MPU9250_EXT_SENS_DATA_15   0x58U
#define MPU9250_EXT_SENS_DATA_16   0x59U
#define MPU9250_EXT_SENS_DATA_17   0x5AU
#define MPU9250_EXT_SENS_DATA_18   0x5BU
#define MPU9250_EXT_SENS_DATA_19   0x5CU
#define MPU9250_EXT_SENS_DATA_20   0x5DU
#define MPU9250_EXT_SENS_DATA_21   0x5EU
#define MPU9250_EXT_SENS_DATA_22   0x5FU
#define MPU9250_EXT_SENS_DATA_23   0x60U
#define MPU9250_I2C_SLV0_DO        0x63U
#define MPU9250_I2C_SLV1_DO        0x64U
#define MPU9250_I2C_SLV2_DO        0x65U
#define MPU9250_I2C_SLV3_DO        0x66U
#define MPU9250_I2C_MST_DELAY_CTRL 0x67U
#define MPU9250_SIGNAL_PATH_RESET  0x68U
#define MPU9250_MOT_DETECT_CTRL    0x69U
#define MPU9250_USER_CTRL          0x6AU
#define MPU9250_PWR_MGMT_1         0x6BU
#define MPU9250_PWR_MGMT_2         0x6CU
#define MPU9250_FIFO_COUNTH        0x72U
#define MPU9250_FIFO_COUNTL        0x73U
#define MPU9250_FIFO_R_W           0x74U
#define MPU9250_WHO_AM_I           0x75U
#define MPU9250_XA_OFFSET_H        0x77U
#define MPU9250_XA_OFFSET_L        0x78U
#define MPU9250_YA_OFFSET_H        0x7AU
#define MPU9250_YA_OFFSET_L        0x7BU
#define MPU9250_ZA_OFFSET_H        0x7DU
#define MPU9250_ZA_OFFSET_L        0x7EU

#define MPU9250_XGYRO_CTEN  7U
#define MPU9250_YGYRO_CTEN  6U
#define MPU9250_ZGYRO_CTEN  5U
#define MPU9250_GYRO_FS_SEL 3U
#define MPU9250_FCHOICE_B   0U

#define MPU9250_AX_ST_EN        7U
#define MPU9250_AY_ST_EN        6U
#define MPU9250_AZ_ST_EN        5U
#define MPU9250_ACCEL_FS_SEL    3U
#define MPU9250_ACCEL_FCHOICE_B 3U
#define MPU9250_A_DLPFCFG       0U

#define MPU9250_RAW_DATA_RDY_INT 0U

#define MPU_H_RESET 7U

/* Typedefs
 ******************************************************************************/

typedef uint8_t MPU9250_RegisterAddress_t;

#endif