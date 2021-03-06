#include "readAccel.h"
#include "math.h"

#define Xhigh 0x3B
#define Xlow  0x3C

#define Yhigh 0x3D
#define Ylow  0x3E

#define Zhigh 0x3F
#define Zlow  0x40

i2c_cmd_handle_t cmd;
esp_err_t res;
/*
Example program that reads data from MPU6050
Connections:
Vcc -> 3.3V
GND -> GND
SCL -> 32
SDA -> 33
AD0 -> GND
*/
/*
From the Datasheet, page 33:
The slave address of the MPU-60X0 is b110100X which is 7 bits long.
The LSB bit of the 7 bit address is determined by the logic level on pin AD0.
This allows two MPU-60X0s to be connected to the same I2C bus.
*/
// If AD0 is LOW, the I2C address of the board will be 0x68. Otherwise, the
// address will be 0x69.
#define MPU6050_ADDR 0x68
/*
From Datasheet, page 40:
Register (Hex): 6B
Bit7 DEVICE_RESET
Bit6 SLEEP
Bit5 CYCLE
Bit4 -
Bit3 TEMP_DIS
Bit2, Bit1, Bit0 CLKSEL[2:0]
*/
#define MPU6050_PWR_MGMT_1 0x6B
/*
Register (Hex) 19
Bit7, Bit6, Bit5, Bit4, Bit3, Bit2, Bit1, Bit0: SMPLRT_DIV[7:0]
*/
#define MPU6050_SMPLRT_DIV 0x19
/*
From Datasheet, page 45:
Register (Hex): 75
Bit7 -
Bit6, Bit5, Bit4, Bit3, Bit2, Bit1: WHO_AM_I[6:1]
Bit0: -
*/
#define MPU6050_WHO_AM_I 0x75
void init() {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 33,  // select GPIO specific to your project
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = 32,  // select GPIO specific to your project
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed =
            100000,  // select frequency specific to your project
        // .clk_flags = 0,          /*!< Optional, you can use
        // I2C_SCLK_SRC_FLAG_* flags to choose i2c source clock here. */
    };

    esp_err_t res = i2c_param_config(I2C_NUM_0, &conf);
    ESP_ERROR_CHECK(res);
    res = i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);
    ESP_ERROR_CHECK(res);

    // configure power mode
    // here we set all bits of the PWR_MGMT_1 register to 0
    // create command
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    // start command
    res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);
    // set address + write and check for ack
    res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_WRITE, 1);
    ESP_ERROR_CHECK(res);
    // write the register address and check for ack
    res = i2c_master_write_byte(cmd, MPU6050_PWR_MGMT_1, 1);
    ESP_ERROR_CHECK(res);
    // write value of the regiter: 0, and check for ack
    res = i2c_master_write_byte(cmd, 0x00, 1);
    ESP_ERROR_CHECK(res);
    // end of command
    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);
    // send the command, 1 second timeout
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    ESP_ERROR_CHECK(res);
    // delete command now that it's not needed
    i2c_cmd_link_delete(cmd);

    // set the sampling frequency
    // the sampling freq is gyro sampling freq / (1 + divider)
    // setting divider to 250 leads to sampling freq. of 32 Hz
    cmd = i2c_cmd_link_create();
    res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_WRITE,
                                1);  // WRITE bit set!
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, MPU6050_SMPLRT_DIV,
                                1);  // write to SMPLRT_DIV
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, 250, 1);  // set SMPLRT_DIV to 250
    ESP_ERROR_CHECK(res);
    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    // ESP_ERROR_CHECK(res);
    i2c_cmd_link_delete(cmd);
}

int16_t readaccel(uint16_t registerHigh, uint16_t registerLow) {
    int16_t tempRaw = 0;
    uint8_t buffer;
    // read low register
    // send just the register number with no other data
    cmd = i2c_cmd_link_create();
    res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_WRITE,
                                1);  // WRITE bit set!
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, registerLow,
                                1);  // read low first
    ESP_ERROR_CHECK(res);
    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    ESP_ERROR_CHECK(res);
    i2c_cmd_link_delete(cmd);
    // wait a little
    vTaskDelay(10 / portTICK_RATE_MS);
    // now read the answer
    cmd = i2c_cmd_link_create();
    res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_READ,
                                1);  // READ bit set!
    ESP_ERROR_CHECK(res);
    res = i2c_master_read(cmd, &buffer, 1, I2C_MASTER_NACK);
    ESP_ERROR_CHECK(res);
    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    ESP_ERROR_CHECK(res);
    i2c_cmd_link_delete(cmd);
    tempRaw = buffer;
    // read high register
    cmd = i2c_cmd_link_create();
    res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_WRITE,
                                1);  // WRITE bit set!
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, registerHigh, 1);  // read high
    ESP_ERROR_CHECK(res);
    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    ESP_ERROR_CHECK(res);
    i2c_cmd_link_delete(cmd);
    vTaskDelay(10 / portTICK_RATE_MS);
    cmd = i2c_cmd_link_create();
    res = i2c_master_start(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_write_byte(cmd, MPU6050_ADDR << 1 | I2C_MASTER_READ,
                                1);  // READ bit set!
    ESP_ERROR_CHECK(res);
    res = i2c_master_read(cmd, &buffer, 1, I2C_MASTER_NACK);
    ESP_ERROR_CHECK(res);
    res = i2c_master_stop(cmd);
    ESP_ERROR_CHECK(res);
    res = i2c_master_cmd_begin(I2C_NUM_0, cmd, 1000 / portTICK_RATE_MS);
    ESP_ERROR_CHECK(res);
    i2c_cmd_link_delete(cmd);
    // combine high and low registers into a signed integer
    tempRaw |= ((int16_t)buffer) << 8;

    return tempRaw;
}

u_int32_t getaccelmagnitude() {
    double Xaccel = readaccel(Xhigh, Xlow);
    double Yaccel = readaccel(Yhigh, Ylow);
    double Zaccel = readaccel(Zhigh, Zlow);

    return (u_int32_t)sqrt(pow(Xaccel, 2) + pow(Yaccel, 2) + pow(Zaccel, 2));
}